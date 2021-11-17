#include "volume.h"

#include <math.h>
#include "backup.h"
#include "led.h"

static const uint16_t VOL_UNDO_CNT = 5000;
static const uint8_t MAX_HALF_PITCH_VOL = 0b01100000;
static const float MAX_HALF_PITCH_DB_VOL = 48.0;
static const uint8_t MAX_VOL = 0b01111111;
static const float MAX_DB_VOL = 78.0;

typedef struct {
    uint8_t step;
    uint16_t data[2];
    uint8_t data_cursor;
} VolumeState_t;
VolumeState_t g_vol_state;
uint8_t g_volume = 0;
uint8_t g_volume_temp = 0;
uint16_t g_volume_temp_cnt;
bool g_volume_temp_now = false;

float Vol_to_Float(uint8_t vol);
uint8_t Float_to_Vol(float db_vol);
void Vol_Send_Start(uint8_t vol);

extern TIM_HandleTypeDef htim3;

void Vol_Init() {
    Backup_Read();
    g_volume = backup->volume[0];
    Vol_Send_Start(g_volume);
}
void Vol_Step(bool up) {
    if (up) {
        if (g_volume > 0) {  // not max volume
            g_volume--;
            Vol_Send_Start(g_volume);
        }
    } else {
        if (g_volume <= 0b01111111) {  // not min volume
            g_volume++;
            Vol_Send_Start(g_volume);
        }
    }
}
void Vol_Save(uint8_t slot) {
    Backup_Read();
    backup->volume[slot] = g_volume;
    Backup_Write();
    g_cnt_led_bring = 0;
}
void Vol_Read(uint8_t slot) {
    Backup_Read();
    g_volume = backup->volume[slot];
    Vol_Send_Start(g_volume);
}

void Vol_Send_Start(uint8_t vol) {
    g_volume_temp_now = false;
    char buff[8];
    float v = Vol_to_Float(vol);
    // sprintf(buff, "%4.1f", v);//使用のためにリンカフラグ"-u _printf_float"が必要。FLASH削減のため使用しない
    uint8_t intger = floor(v);
    uint8_t dicimal = (v-intger)*10;
    sprintf(buff, "%2d.%d", intger,dicimal);
    LED_Update(buff);
    g_vol_state.data[0] = 0x0100 | (uint16_t)vol;
    g_vol_state.data[1] = 0x0000 | (uint16_t)vol;
    HAL_TIM_Base_Start_IT(&htim3);
    g_cnt_led_show = 0;
}

void Vol_In_While() {
    if (g_volume_temp_now && g_volume_temp_cnt > VOL_UNDO_CNT) {
        Vol_Send_Start(g_volume);
    }
}
void Vol_Down_Temp(float relative_db_volume) {
    float tempvolume;
    tempvolume = Vol_to_Float(g_volume);
    g_volume_temp = Float_to_Vol(tempvolume + relative_db_volume);
    Vol_Send_Start(g_volume_temp);
    g_volume_temp_now = true;
    g_volume_temp_cnt = 0;
}
void Vol_In_Timer_1ms() {
    if (g_volume_temp_cnt < UINT16_MAX) {
        g_volume_temp_cnt++;
    }
}

void Vol_In_Timer_2us() {
    uint8_t data_num = g_vol_state.step >> 1;
    if ((g_vol_state.step & 1) == 0) {  // falling edge
        if (g_vol_state.step != 32) {
            HAL_GPIO_WritePin(VOL_CLK_GPIO_Port, VOL_CLK_Pin, GPIO_PIN_RESET);
        }
        if (g_vol_state.step == 0) {
            HAL_GPIO_WritePin(VOL_LOAD_GPIO_Port, VOL_LOAD_Pin, GPIO_PIN_RESET);
        }
        if (data_num < 16) {
            if (g_vol_state.data[g_vol_state.data_cursor] & (0x8000 >> (data_num))) {
                HAL_GPIO_WritePin(VOL_DAT_GPIO_Port, VOL_DAT_Pin, GPIO_PIN_SET);
            } else {
                HAL_GPIO_WritePin(VOL_DAT_GPIO_Port, VOL_DAT_Pin, GPIO_PIN_RESET);
            }
        } else {
            HAL_GPIO_WritePin(VOL_DAT_GPIO_Port, VOL_DAT_Pin, GPIO_PIN_SET);
        }
    } else {  // rising edge
        HAL_GPIO_WritePin(VOL_CLK_GPIO_Port, VOL_CLK_Pin, GPIO_PIN_SET);
        if (g_vol_state.step == 33) {
            HAL_GPIO_WritePin(VOL_LOAD_GPIO_Port, VOL_LOAD_Pin, GPIO_PIN_SET);
            g_vol_state.step = 0;
            if (g_vol_state.data_cursor >= 1) {
                g_vol_state.data_cursor = 0;
                HAL_TIM_Base_Stop_IT(&htim3);
                HAL_TIM_Base_Init(&htim3);
            } else {
                g_vol_state.data_cursor++;
            }
            return;
        }
    }
    g_vol_state.step++;
}

float Vol_to_Float(uint8_t vol) {
    float att;
    if (vol <= MAX_HALF_PITCH_VOL) {  // ~48db
        att = (float)vol / 2;
    } else if (vol <= MAX_VOL) {  // ~78db
        att = vol - MAX_HALF_PITCH_VOL + MAX_HALF_PITCH_DB_VOL;
    } else {
        att = 99.9;  // mute;
    }
    return att;
}

uint8_t Float_to_Vol(float db_vol) {
    uint8_t vol;
    if (db_vol <= MAX_HALF_PITCH_DB_VOL) {
        vol = db_vol * 2;
    } else if (db_vol <= MAX_DB_VOL) {
        vol = db_vol - MAX_HALF_PITCH_DB_VOL + MAX_HALF_PITCH_VOL;
    } else {
        vol = MAX_VOL + 1;  // mute
    }
    return vol;
}