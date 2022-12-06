+----+-------+--------------+----+-----------+-------------+----+---+
| �  | ����. |    437ZIT         |           |   479VIT6        | � |   �� - �������� �� ���������
|����|       |     FPGA          |           |   ������         | � |
+----+-------+--------------+----+-----------+-------------+----+---+
|    |       | 43  PA7      | I  | PAN_READY | 49 PB15     | O  | 1 |����� ������������ ������� ���������� � ������������ � ������� �������������
|    |       | 44  PC4      | I  | PAN_DATA  | 48 PB14     | O  | 0 |����� ������������ ������� ���������� ������ ��� �������� � ����������
|    |       | 127 PG12 CS  | O  | CS        | 50 PD8      | I  |   |�� ����� ������� �� �������� �� �������� ���������� ������/������
|    |       | 119 PD5  WR  | O  | WR        | 89 PD5  WR  | I  |   |������� ����, ��� �������� �� ������������ �������� ������ � ������
|    |       | 118 PD4  RD  | O  | RD        | 88 PD4  RD  | I  |   |������� ����, ��� �������� �� ������������ �������� ������ �� ������
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

 ������.
 
void HAL_BUS::Update()
{
    while (pinCS.IsActive() == 0)
    {
        if (pinWR.IsActive() == 0)                  // ������ ����� �� ����������
        {
            uint8 data = DataBus::ReadValue();

            pinReady.SetPassive();

            PDecoder::AddData(data);        // \todo ������ ���������� - ���� �� ���������� PDecoder::AddData(), ���������� �� ����� ������

            //while(pinCS.IsActive()) {};
            //while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET) { }
            volatile uint state = GPIOD->IDR & GPIO_PIN_8;
            while (state == 0)
            {
                state = GPIOD->IDR & GPIO_PIN_8;
            }

            //pinReady.SetActive();
            GPIOB->BSRR = (uint)GPIO_PIN_12 << 16U;
        }
        else if (pinRD.IsActive() && queueData.Size())   // ������ ����� � ��������� //-V2570 //-V2516
        {
            CONFIG_TO_WRITE;

            // ������������� ������ �� ��
            HAL_BUS::DataBus::WriteValue(queueData.Front());

            pinReady.SetPassive();

            pinCS.WaitPassive();

            pinReady.SetActive();

            if (queueData.Size() == 0)
            {
                pinData.SetPassive();
            }

            CONFIG_TO_READ;
        }
    }
}
