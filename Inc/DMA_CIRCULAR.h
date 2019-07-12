#include "stm32f1xx_hal.h"

#define DMA_RX_BUFFER_SIZE          512
#define UART_BUFFER_SIZE            512

void USART_IrqHandler (UART_HandleTypeDef *huart, DMA_HandleTypeDef *hdma);

void DMA_IrqHandler (DMA_HandleTypeDef *hdma);


