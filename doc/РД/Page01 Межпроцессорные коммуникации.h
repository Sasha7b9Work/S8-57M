+----+-------+--------------+----+-----------+-------------+----+---+
| �  | ����. |    437ZIT         |           |   479VIT6        | � |   �� - �������� �� ���������
|����|       |     FPGA          |           |   ������         | � |
+----+-------+--------------+----+-----------+-------------+----+---+
|    |       | 43  PA7      | I  | PAN_READY | 49 PB15     | O  | 1 | ����� ������������ ������� ���������� � ������������ � ������� �������������
|    |       | 44  PC4      | I  | PAN_DATA  | 48 PB14     | O  | 0 | ����� ������������ ������� ���������� ������ ��� �������� � ����������
|    |       | 127 PG12 CS  | O  | CS        | 50 PD8      | I  |   | ������������� � ���� �������� ��������, ��� �� ����� ��������/������� ������
|    |       | 119 PD5  WR  | O  | WR        | 89 PD5  WR  | I  |   | ������������� � ���� �������� ��������, ��� �� ����� �������� ������
|    |       | 118 PD4  RD  | O  | RD        | 88 PD4  RD  | I  |   | ������� ����, ��� �������� �� ������������ �������� ������ �� ������
|    |       |              |    |           |             |    |   |
|    | AD0   | 85  PD14 D0  | IO | D0        | 53 PD14 DA0 | IO |   |
|    | AD1   | 86  PD15 D1  | IO | D1        | 54 PD15 DA1 | IO |   |
|    | AD2   | 114 PD0  D2  | IO | D2        | 84 PD0  DA2 | IO |   |
|    | AD3   | 115 PD1  D3  | IO | D3        | 85 PD1  DA3 | IO |   |
|    | AD4   | 58  PE7  D4  | IO | D4        | 32 PE7  DA4 | IO |   |
|    | AD5   | 59  PE8  D5  | IO | D5        | 33 PE8  DA5 | IO |   |
|    | AD6   | 60  PE9  D6  | IO | D6        | 36 PE9  DA6 | IO |   |
|    | AD7   | 63  PE10 D7  | IO | D7        | 38 PE7  DA7 | IO |   |
+----+-------+--------------+----+-----------+-------------+----+---+


 *** ��������� ������/������ ***

 * ������ *
 
void HAL_BUS::Update()
{
    while(in_CS.IsActive())
    {
        if (in_WR.IsActive())                           // ������ ����� �� ����������
        {
            uint8 data = DataBus::ReadValue();

            pin_ready.SetPassive();

            PDecoder::AddData(data);                    // \todo ������ ���������� - ���� �� ���������� PDecoder::AddData(), ���������� �� ����� ������

            while(in_CS.IsActive()) {};

            pin_ready.SetActive();
        }
        else if (in_RD.IsActive() && queueData.Size())   // ������ ����� � ��������� //-V2570 //-V2516
        {
            CONFIG_TO_WRITE;

            // ������������� ������ �� ��
            DataBus::WriteValue(queueData.Front());

            out_ready.SetPassive();

            in_CS.WaitPassive();

            out_ready.SetActive();

            if (queueData.Size() == 0)
            {
                out_data.SetPassive();
            }

            CONFIG_TO_READ;
        }
    }
}


 * ���������� *
 
bool HAL_BUS::Panel::Receive() //-V2506
{
    //if(in_ready.IsPassive() || in_data.IsPassive())
    //if((GPIOA->IDR & GPIO_PIN_7) != GPIO_PIN_RESET || (GPIOC->IDR & GPIO_PIN_4) != GPIO_PIN_RESET)
    if ((GPIOA->IDR & GPIO_PIN_7) || (GPIOC->IDR & GPIO_PIN_4)) //-V2570
    {
        return false;
    }

    interactionWithPanel = true;

    if (mode != Mode::PanelRead)
    {
        mode = Mode::PanelRead;

        out_RD.Init();

        // ������������� �� �� ������

        GPIOD->MODER &= 0x0ffffff0U;        // �������� ���� 14, 15, 0, 1 �� ������ D0, D1, D2, D3

        GPIOE->MODER &= 0xffc03fffU;        // �������� ���� 7, 8, 9, 10 �� ������ D4, D5, D6, D7
    }

    out_RD.SetActive();

    out_CS.SetActive();

    in_ready.WaitPassive();

    while (in_ready.IsActive()) {};

    uint8 data = 0;

    while (in_ready.IsActive())
    {
        if (in_data.IsPassive())
        {
            goto exit;
        }
    }

    //                                                 4,5,6,7              2,3                          0,1
    data = static_cast<uint8>((GPIOE->IDR >> 3) & 0xF0 | (GPIOD->IDR << 2) & 0x0C | (GPIOD->IDR >> 14));

    DDecoder::AddData(data);

exit:

    out_RD.SetPassive();

    out_CS.SetPassive();

    interactionWithPanel = false;

    ///
    // \todo ���� ������ ������ ��� ������, �� ����� ������ ��������, ������ ��� ��� ����� �������� ��������
    // 

    if (otherActionsAllowed)
    {
        if (Device::InModeRecorder())
        {
            Recorder::RecordPoints();
        }
        else if (OSCI_IN_MODE_P2P)
        {
            Roller::ReadPoint();
        }
        else
        {
            Timer::PauseOnTicks(10);
        }
    }
    else
    {
        Timer::PauseOnTicks(10);
    }

    return true;
}


void HAL_BUS::Panel::Send(const uint8 *data, int size)
{
    if (!(GPIOA->IDR & GPIO_PIN_7) && !(GPIOC->IDR & GPIO_PIN_4)) //-V2570
    {
        while (Receive()) {}
    }

    interactionWithPanel = true;

    if (mode != Mode::PanelWrite)
    {
        mode = Mode::PanelWrite;

        out_WR.Init();

        // ������������� �� �� ������

        GPIOD->MODER &= 0x0ffffff0U;        // �������� ���� 14, 15, 0, 1 �� ������ D0, D1, D2, D3
        GPIOD->MODER |= 0x50000005U;        // ������������� ��� ���� ����� GPIO_MODE_OUTPUT_PP

        GPIOE->MODER &= 0xffc03fffU;        // �������� ���� 7, 8, 9, 10 �� ������ D4, D5, D6, D7
        GPIOE->MODER |= 0x00154000U;        // ������������� ��� ���� ����� GPIO_MODE_OUTPUT_PP
    }

    for (int i = 0; i < size; i++)
    {
        uint8 d = *data++;

        //                                                                             ���� 0,1                                 ���� 2,3
        GPIOD->ODR = (GPIOD->ODR & 0x3ffc) + static_cast<uint16>((static_cast<int16>(d) & 0x03) << 14) + ((static_cast<uint16>(d & 0x0c)) >> 2);  // ���������� ������ � �������� ����
        //                                                                          ���� 4,5,6,7
        GPIOE->ODR = (GPIOE->ODR & 0xf87f) + static_cast<uint16>((static_cast<int16>(d) & 0xf0) << 3);

        //out_WR.SetActive();                  // ��� ������ ������
        GPIOD->BSRR = (uint)GPIO_PIN_5 << 16U;

        //while(in_ready.IsPassive()) {}   // � ������� ������ ������ � ���, ��� ��� ��������
        //while(HAL_PIO::Read(HPort::_A, HPin::_7) == 1) { };
        volatile uint state = GPIOA->IDR & GPIO_PIN_7;
        while (state)
        {
            state = GPIOA->IDR & GPIO_PIN_7;
        }

        //out_CS.SetActive();                  // ��� ������� ����, ��� ������ ���������� � ����� �� ���������
        GPIOG->BSRR = (uint)GPIO_PIN_12 << 16U;

        //while(in_ready.IsActive()) {}    // ������������ PIN_PAN_READY � ���������� ��������� ��������, ��� ������ ������� ������ � ������������ ��
        state = GPIOA->IDR & GPIO_PIN_7;
        while (state == 0)
        {
            state = GPIOA->IDR & GPIO_PIN_7;
        }

        //out_WR.SetPassive();                 // \ ������������� WR � CS � ���������� ��������� - ������������ ���� ������ �������
        GPIOD->BSRR = GPIO_PIN_5;

        //out_CS.SetPassive();                 // /
        GPIOG->BSRR = GPIO_PIN_12;
    }

    interactionWithPanel = false;

    if (otherActionsAllowed)
    {
        if (Device::InModeRecorder())
        {
            Recorder::RecordPoints();
        }
        else if (OSCI_IN_MODE_P2P) //-V2516
        {
            Roller::ReadPoint();
        }

        if (funcAfterInteraction)
        {
            funcAfterInteraction();
            funcAfterInteraction = nullptr;
        }
    }
}
