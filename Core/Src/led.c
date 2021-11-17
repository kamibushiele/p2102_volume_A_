#include "led.h"

uint8_t g_cnt_led_dyn = 0;
uint8_t g_led_seg[3];
uint16_t g_cnt_led_show = 0;
uint16_t g_cnt_led_bring = 0;
const uint16_t g_led_show_ms = 3000;
bool led_stoped = false;

void LED_Drive();
void LED_Disable();

void LED_Init() {
    LED_Disable();
}

void Led_In_While() {
    if (
        g_cnt_led_show < g_led_show_ms ||
        (g_cnt_led_bring < 200) ||
        (g_cnt_led_bring > 400 && g_cnt_led_bring < 600)) {
        LED_Drive();
        led_stoped = false;
    } else {
        if (led_stoped == false) {
            led_stoped = true;
            LED_Disable();
        }
    }
}
void LED_In_Timer() {
    if (g_cnt_led_show < UINT16_MAX) {
        g_cnt_led_show++;
    }
    if (g_cnt_led_dyn < UINT8_MAX) {
        g_cnt_led_dyn++;
    }
    if (g_cnt_led_bring < UINT16_MAX) {
        g_cnt_led_bring++;
    }
}

void LED_Drive() {
    static bool apply = true;
    static uint8_t digit = 0;
    switch (digit) {
        case 0:
            if (g_cnt_led_dyn > 5) {
                digit++;
                apply = true;
            }
            break;
        case 1:
            if (g_cnt_led_dyn > 10) {
                digit++;
                apply = true;
            }
            break;
        case 2:
            if (g_cnt_led_dyn > 15) {
                g_cnt_led_dyn = 0;
                digit = 0;
                apply = true;
            }
            break;
    }
    if (apply) {
        apply = false;
        HAL_GPIO_WritePin(SEG_LED_1_GPIO_Port, SEG_LED_1_Pin, GPIO_PIN_RESET);  //一旦消す
        HAL_GPIO_WritePin(SEG_LED_2_GPIO_Port, SEG_LED_2_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(SEG_LED_3_GPIO_Port, SEG_LED_3_Pin, GPIO_PIN_RESET);

        HAL_GPIO_WritePin(SEG_LED_A_GPIO_Port, SEG_LED_A_Pin, g_led_seg[digit] & 0x01);
        HAL_GPIO_WritePin(SEG_LED_B_GPIO_Port, SEG_LED_B_Pin, g_led_seg[digit] & 0x02);
        HAL_GPIO_WritePin(SEG_LED_C_GPIO_Port, SEG_LED_C_Pin, g_led_seg[digit] & 0x04);
        HAL_GPIO_WritePin(SEG_LED_D_GPIO_Port, SEG_LED_D_Pin, g_led_seg[digit] & 0x08);
        HAL_GPIO_WritePin(SEG_LED_E_GPIO_Port, SEG_LED_E_Pin, g_led_seg[digit] & 0x10);
        HAL_GPIO_WritePin(SEG_LED_F_GPIO_Port, SEG_LED_F_Pin, g_led_seg[digit] & 0x20);
        HAL_GPIO_WritePin(SEG_LED_G_GPIO_Port, SEG_LED_G_Pin, g_led_seg[digit] & 0x40);
        HAL_GPIO_WritePin(SEG_LED_DP_GPIO_Port, SEG_LED_DP_Pin, g_led_seg[digit] & 0x80);

        HAL_GPIO_WritePin(SEG_LED_1_GPIO_Port, SEG_LED_1_Pin, digit == 0);
        HAL_GPIO_WritePin(SEG_LED_2_GPIO_Port, SEG_LED_2_Pin, digit == 1);
        HAL_GPIO_WritePin(SEG_LED_3_GPIO_Port, SEG_LED_3_Pin, digit == 2);
    }
}
void LED_Disable() {
    HAL_GPIO_WritePin(SEG_LED_A_GPIO_Port, SEG_LED_A_Pin, 0);
    HAL_GPIO_WritePin(SEG_LED_B_GPIO_Port, SEG_LED_B_Pin, 0);
    HAL_GPIO_WritePin(SEG_LED_C_GPIO_Port, SEG_LED_C_Pin, 0);
    HAL_GPIO_WritePin(SEG_LED_D_GPIO_Port, SEG_LED_D_Pin, 0);
    HAL_GPIO_WritePin(SEG_LED_E_GPIO_Port, SEG_LED_E_Pin, 0);
    HAL_GPIO_WritePin(SEG_LED_F_GPIO_Port, SEG_LED_F_Pin, 0);
    HAL_GPIO_WritePin(SEG_LED_G_GPIO_Port, SEG_LED_G_Pin, 0);
    HAL_GPIO_WritePin(SEG_LED_DP_GPIO_Port, SEG_LED_DP_Pin, 0);

    HAL_GPIO_WritePin(SEG_LED_1_GPIO_Port, SEG_LED_1_Pin, 0);
    HAL_GPIO_WritePin(SEG_LED_2_GPIO_Port, SEG_LED_2_Pin, 0);
    HAL_GPIO_WritePin(SEG_LED_3_GPIO_Port, SEG_LED_3_Pin, 0);
}
void LED_Update(const char *string) {
    int i;
    int k = 0;
    for (i = 0; string[i] != '\0'; i++) {
        switch (string[i]) {
            case '0':
                g_led_seg[k] = 0b00111111;
                break;
            case '1':
                g_led_seg[k] = 0b00000110;
                break;
            case '2':
                g_led_seg[k] = 0b01011011;
                break;
            case '3':
                g_led_seg[k] = 0b01001111;
                break;
            case '4':
                g_led_seg[k] = 0b01100110;
                break;
            case '5':
                g_led_seg[k] = 0b01101101;
                break;
            case '6':
                g_led_seg[k] = 0b01111101;
                break;
            case '7':
                g_led_seg[k] = 0b00000111;
                break;
            case '8':
                g_led_seg[k] = 0b01111111;
                break;
            case '9':
                g_led_seg[k] = 0b01101111;
                break;
            case ' ':
                g_led_seg[k] = 0b00000000;
                break;
            case '.':
                if (k > 0) {
                    g_led_seg[k - 1] |= 0b10000000;
                }
                break;
            default:
                g_led_seg[k] = 0b01001001;
                break;
        }
        if (string[i] != '.') {
            k++;
            if (k >= 3) {
                if (string[i + 1] == '.') {
                    g_led_seg[k - 1] |= 0b10000000;
                }
                break;
            }
        }
    }
}