#include "defines.h"
#include "ff_gen_drv.h"
#include "usbh_diskio.h"
#include "FlashDrive/FlashDrive.h"


DSTATUS USBH_initialize(BYTE);
DSTATUS USBH_status(BYTE);
DRESULT USBH_read(BYTE, BYTE *, DWORD, UINT);

#if _USE_WRITE == 1
DRESULT USBH_write(BYTE, const BYTE *, DWORD, UINT);
#endif /* _USE_WRITE == 1 */

#if _USE_IOCTL == 1
DRESULT USBH_ioctl(BYTE, BYTE, void *);
#endif /* _USE_IOCTL == 1 */

const Diskio_drvTypeDef  USBH_Driver =
{
  USBH_initialize,
  USBH_status,
  USBH_read,
#if  _USE_WRITE == 1
  USBH_write,
#endif /* _USE_WRITE == 1 */
#if  _USE_IOCTL == 1
  USBH_ioctl,
#endif /* _USE_IOCTL == 1 */
};

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes a Drive
  * @retval DSTATUS: Operation status
  */
DSTATUS USBH_initialize(BYTE)
{
    /* CAUTION : USB Host library has to be initialized in the application */

    return RES_OK;
}

/**
  * @brief  Gets Disk Status
  * @param  lun : lun id
  * @retval DSTATUS: Operation status
  */
DSTATUS USBH_status(BYTE lun)
{
    DRESULT res = RES_ERROR;

    if (USBH_MSC_UnitIsReady(reinterpret_cast<USBH_HandleTypeDef *>(FDrive::handle), lun))
    {
        res = RES_OK;
    }

    return static_cast<DSTATUS>(res);
}

/**
  * @brief  Reads Sector(s)
  * @param  lun : lun id
  * @param  *buff: Data buffer to store read data
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to read (1..128)
  * @retval DRESULT: Operation result
  */
DRESULT USBH_read(BYTE lun, BYTE *buff, DWORD sector, UINT count)
{
    DRESULT res = RES_ERROR;
    MSC_LUNTypeDef info;

    if (USBH_MSC_Read(reinterpret_cast<USBH_HandleTypeDef *>(FDrive::handle), lun, sector, buff, count) == USBH_OK)
    {
        res = RES_OK;
    }
    else
    {
        USBH_MSC_GetLUNInfo(reinterpret_cast<USBH_HandleTypeDef *>(FDrive::handle), lun, &info);

        switch (info.sense.asc)
        {
        case SCSI_ASC_LOGICAL_UNIT_NOT_READY:
        case SCSI_ASC_MEDIUM_NOT_PRESENT:
        case SCSI_ASC_NOT_READY_TO_READY_CHANGE:
            USBH_ErrLog("USB Disk is not ready!");
            res = RES_NOTRDY;
            break;
        }
    }

    return res;
}

/**
  * @brief  Writes Sector(s)
  * @param  lun : lun id
  * @param  *buff: Data to be written
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to write (1..128)
  * @retval DRESULT: Operation result
  */
#if _USE_WRITE == 1
DRESULT USBH_write(BYTE lun, const BYTE *buff, DWORD sector, UINT count)
{
    DRESULT res = RES_ERROR;
    MSC_LUNTypeDef info;

    if (USBH_MSC_Write(reinterpret_cast<USBH_HandleTypeDef *>(FDrive::handle), lun, sector, const_cast<BYTE *>(buff), count) == USBH_OK)
    {
        res = RES_OK;
    }
    else
    {
        USBH_MSC_GetLUNInfo(reinterpret_cast<USBH_HandleTypeDef *>(FDrive::handle), lun, &info);

        switch (info.sense.asc)
        {
        case SCSI_ASC_WRITE_PROTECTED:
            USBH_ErrLog("USB Disk is Write protected!");
            res = RES_WRPRT;
            break;

        case SCSI_ASC_LOGICAL_UNIT_NOT_READY:
        case SCSI_ASC_MEDIUM_NOT_PRESENT:
        case SCSI_ASC_NOT_READY_TO_READY_CHANGE:
            USBH_ErrLog("USB Disk is not ready!");
            res = RES_NOTRDY;
            break;
        }
    }

    return res;
}
#endif /* _USE_WRITE == 1 */

/**
  * @brief  I/O control operation
  * @param  lun : lun id
  * @param  cmd: Control code
  * @param  *buff: Buffer to send/receive control data
  * @retval DRESULT: Operation result
  */
#if _USE_IOCTL == 1
DRESULT USBH_ioctl(BYTE lun, BYTE cmd, void *buff)
{
    DRESULT res = RES_ERROR;
    MSC_LUNTypeDef info;

    switch (cmd)
    {
        /* Make sure that no pending write process */
    case CTRL_SYNC:
        res = RES_OK;
        break;

        /* Get number of sectors on the disk (DWORD) */
    case GET_SECTOR_COUNT:
        if (USBH_MSC_GetLUNInfo(reinterpret_cast<USBH_HandleTypeDef *>(FDrive::handle), lun, &info) == USBH_OK)
        {
            *static_cast<DWORD *>(buff) = info.capacity.block_nbr; //-V525
            res = RES_OK;
        }
        break;

        /* Get R/W sector size (WORD) */
    case GET_SECTOR_SIZE:
        if (USBH_MSC_GetLUNInfo(reinterpret_cast<USBH_HandleTypeDef *>(FDrive::handle), lun, &info) == USBH_OK) //-V1037
        {
            *static_cast<DWORD *>(buff) = info.capacity.block_size;
            res = RES_OK;
        }
        break;

        /* Get erase block size in unit of sector (DWORD) */
    case GET_BLOCK_SIZE:

        if (USBH_MSC_GetLUNInfo(reinterpret_cast<USBH_HandleTypeDef *>(FDrive::handle), lun, &info) == USBH_OK)
        {
            *static_cast<DWORD *>(buff) = info.capacity.block_size;
            res = RES_OK;
        }
        break;

    default:
        res = RES_PARERR;
        break;
    }

    return res;
}
#endif /* _USE_IOCTL == 1 */
