#include "ois.h"
// Optical Insulated Serial

#include "volume.h"

#define RX_BUF_SIZE 16  //UARTのリングバッファサイズ

extern UART_HandleTypeDef huart1;

uint8_t uart_rx_data[RX_BUF_SIZE] = "fuesdayo";

void OIS_In_While(){
    static uint8_t prev_cursor = 0;
    uint8_t uart_cursor = 0;
    uint8_t last_char_cursor;
    uint8_t i;
    uint8_t k;
    char strBuff[RX_BUF_SIZE];
    uart_cursor = RX_BUF_SIZE - huart1.hdmarx->Instance->CNDTR;//バッファーサイズ - バッファの残り
    if(prev_cursor != uart_cursor){
        last_char_cursor = uart_cursor-1;
        i = last_char_cursor;
        if(uart_rx_data[i] == '\n'){
            k = 0;
            while(1){
                if(i != 0){
                    i--;
                }else{
                    i = RX_BUF_SIZE-1;
                }
                if(uart_rx_data[i] == '\n'){
                    strBuff[k] = '\0';
                    break;
                }else{
                    strBuff[k] = uart_rx_data[i];//逆順になる
                    k++;
                }
            }
            if(i != last_char_cursor){
                // if(strcmp(strBuff,"") != 0){
                //     __NOP();
                // }
                if(strcmp(strBuff,"NWODLOV") == 0){
                    Vol_Down_Temp(20.0);
                }
            }
        }
        prev_cursor = uart_cursor;
    }
    // b = hdma_usart1_rx.Instance->CNDTR;//バッファの残り?
    // sprintf(strBuff,"%s\n^^%d,%d\n",uart_rx_data,a,b);
    // HAL_UART_Transmit_DMA(&huart1,strBuff,sizeof(strBuff));
}
void OIS_Init(){
    // hdma_usart1_tx.XferCpltCallback = *tx_end;
    memset(uart_rx_data,0,RX_BUF_SIZE);
    __HAL_UART_DISABLE_IT(&huart1, UART_IT_PE);                //pality error interrupt disable
    __HAL_UART_DISABLE_IT(&huart1, UART_IT_ERR);               //UART error interrupt disable
    HAL_UART_Receive_DMA(&huart1, uart_rx_data, RX_BUF_SIZE);  //UART DMA setting
}