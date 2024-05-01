#include "uart_transmit.h"

extern UART_HandleTypeDef huart2;

uint8_t arr_to_transmit[FLOATS_TRANSFER_COUNT*4+4];

void uart_transmit_init() {
    arr_to_transmit[FLOATS_TRANSFER_COUNT*sizeof(float)] = '\n';
    arr_to_transmit[FLOATS_TRANSFER_COUNT*sizeof(float)+1] = '\n';
    arr_to_transmit[FLOATS_TRANSFER_COUNT*sizeof(float)+2] = '\n';
    arr_to_transmit[FLOATS_TRANSFER_COUNT*sizeof(float)+3] = '\n';
    HAL_UART_Transmit_DMA(&huart2, arr_to_transmit, FLOATS_TRANSFER_COUNT*4+4);
}
