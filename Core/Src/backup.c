#include "backup.h"


static uint8_t work_ram[BACKUP_FLASH_SIZE] __attribute__ ((aligned(4)));
extern char _backup_flash_start;//define in ld
Buckup_data_t *backup = work_ram;



void Backup_Read(){
    memcpy(work_ram,&_backup_flash_start,BACKUP_FLASH_SIZE);
}
void Backup_Write(){
    HAL_FLASH_Unlock();
    //erase
    FLASH_EraseInitTypeDef EraseInitStruct = {
      .TypeErase = FLASH_TYPEERASE_PAGES,
      .PageAddress = &_backup_flash_start,
      .NbPages = 1,
    };
    uint32_t error_sector;
    HAL_FLASHEx_Erase(&EraseInitStruct, &error_sector);
    //flash
    uint32_t *p_work_ram = (uint32_t*)work_ram;
    for(int i=0;i<BACKUP_FLASH_SIZE_WORD;i++){
      HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,(uint32_t)(&_backup_flash_start),p_work_ram[i]);
    }
    HAL_FLASH_Lock();
}