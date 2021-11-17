#include "sw.h"

#include "volume.h"

typedef struct {
    GPIO_TypeDef* GPIOx;
    uint16_t GPIO_Pin;
    GPIO_PinState prev_state;
    bool changed;
    bool pushed;
    bool pressed;
    uint16_t press_cnt;
    uint8_t change_cnt;
} Btn_State_t;

typedef enum {
    ENC_NO_CANGE,
    ENC_CW,
    ENC_CCW,
} Enc_State_t;

Enc_State_t Enc_state_update();

#define BTN_NUM (2)
const uint16_t PRESS_MS = 2000;

static uint8_t g_enc_state[2];
static bool g_enc_state_cursor;
static uint8_t g_enc_change_cnt;
static bool g_enc_changed_f;
static uint16_t g_cnt_pow_on = 0;
static bool mute_enabled = false;

Btn_State_t g_btn_state[BTN_NUM] = {
    {
        .GPIOx = BTN_1_GPIO_Port,
        .GPIO_Pin = BTN_1_Pin,
        .prev_state = GPIO_PIN_SET,
        .changed = false,
        .pushed = false,
        .pressed = false,
        .change_cnt = 0,
        .press_cnt = 0,
    },
    {
        .GPIOx = BTN_2_GPIO_Port,
        .GPIO_Pin = BTN_2_Pin,
        .prev_state = GPIO_PIN_SET,
        .changed = false,
        .pushed = false,
        .pressed = false,
        .change_cnt = 0,
        .press_cnt = 0,
    },
};

void Sw_In_While() {
    Enc_State_t enc_res;
    Btn_State_t* btn;
    GPIO_PinState now_state;
    if (!mute_enabled) {
        if (g_cnt_pow_on >= 1000) {
            HAL_GPIO_WritePin(MUTE_GPIO_Port, MUTE_Pin, GPIO_PIN_SET);
            mute_enabled = true;
        }
    }
    if (g_enc_changed_f && g_enc_change_cnt >= 3) {
        g_enc_changed_f = false;
        g_enc_state[g_enc_state_cursor] = HAL_GPIO_ReadPin(ENC_A_GPIO_Port, ENC_A_Pin) << 1;
        g_enc_state[g_enc_state_cursor] |= HAL_GPIO_ReadPin(ENC_B_GPIO_Port, ENC_B_Pin);
        g_enc_state_cursor = !g_enc_state_cursor;
        enc_res = Enc_state_update();
        if (enc_res == ENC_CCW) {  // volume down
            Vol_Step(false);
        } else if (enc_res == ENC_CW) {  // volume up
            Vol_Step(true);
        }
    }
    for (uint8_t i = 0; i < BTN_NUM; i++) {
        btn = g_btn_state + i;
        now_state = HAL_GPIO_ReadPin(btn->GPIOx, btn->GPIO_Pin);
        if (now_state != btn->prev_state && !btn->changed) {
            btn->changed = true;
            btn->change_cnt = 0;
        }
        if (btn->changed && btn->change_cnt >= 3) {
            btn->prev_state = now_state;
            btn->changed = false;
            if (!now_state) {  // push
                btn->press_cnt = 0;
                btn->pushed = true;
                btn->pressed = false;
            } else {  // release
                btn->pushed = false;
                if (btn->press_cnt <= PRESS_MS) {
                    Vol_Read(i);
                }
            }
        }
        if (btn->pushed && !btn->pressed && btn->press_cnt > PRESS_MS) {//press
            btn->pressed = true;
            Vol_Save(i);
        }
    }
}
void Sw_In_Timer() {
    if (g_cnt_pow_on < UINT16_MAX) {
        g_cnt_pow_on++;
    }
    for (uint8_t i = 0; i < BTN_NUM; i++) {
        if (g_btn_state[i].change_cnt < UINT8_MAX) {
            g_btn_state[i].change_cnt++;
        }
        if (g_btn_state[i].press_cnt < UINT16_MAX) {
            g_btn_state[i].press_cnt++;
        }
    }
    if (g_enc_change_cnt < UINT8_MAX) {
        g_enc_change_cnt++;
    }
}

void Sw_In_Interrupt(uint16_t GPIO_Pin) {
    if (GPIO_Pin == ENC_A_Pin && !g_enc_changed_f) {
        g_enc_changed_f = true;
        g_enc_change_cnt = 0;
    }
    if (GPIO_Pin == POWER_SW_Pin) {//POWER OFF
        HAL_GPIO_WritePin(MUTE_GPIO_Port, MUTE_Pin, GPIO_PIN_RESET);
        mute_enabled = false;
        g_cnt_pow_on = 0;
    }
}

Enc_State_t Enc_state_update() {
    /**
     * CW
     * A :~|__|~ //int
     * B :|~~|__
     * CCW
     * A :~|__|~ //int
     * B :|__|~~
     */
    uint8_t enc;
    if (g_enc_state_cursor) {  // cursor = 1 --> newest data : state[0]
        enc = g_enc_state[1] << 2;
        enc |= g_enc_state[0];
    } else {  // cursor = 0 --> newest data : state[1]
        enc = g_enc_state[0] << 2;
        enc |= g_enc_state[1];
    }
    if (enc == 0b0110) {
        return ENC_CW;
    } else if (enc == 0b0011) {
        return ENC_CCW;
    }
    return ENC_NO_CANGE;
}