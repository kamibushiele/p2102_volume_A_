#ifndef __BACKUP_H
#define __BACKUP_H

#include "main.h"

#define BACKUP_FLASH_SIZE	(1024*1) //backup flash sector 7
#define BACKUP_FLASH_SIZE_WORD	(BACKUP_FLASH_SIZE>>4) //backup flash sector 7

typedef struct{
  uint8_t volume[2];
}Buckup_data_t;
extern Buckup_data_t *backup;
void Backup_Read();
void Backup_Write();

#endif
