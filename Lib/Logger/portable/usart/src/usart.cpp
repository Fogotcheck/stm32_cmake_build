#include "usart.h"

extern UART_HandleTypeDef huart3;
UART_HandleTypeDef *LogUart = &huart3;

LoggerPortUsart::LoggerPortUsart()
{
	init();
}

LoggerPortUsart::~LoggerPortUsart()
{
	HAL_UART_MspDeInit(LogUart);
}

int32_t LoggerPortUsart::write(const uint8_t *Data, uint16_t Size)
{
	return HAL_UART_Transmit(LogUart, Data, Size, 100);
}

int32_t LoggerPortUsart::read(uint8_t *Data, uint16_t Size)
{
	return HAL_UART_Receive_DMA(LogUart, Data, Size);
}

void LoggerPortUsart::init(void)
{
	MX_USART3_UART_Init();
}
