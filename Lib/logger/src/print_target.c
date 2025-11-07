#include "main.h"

#include "logger.h"

static UART_HandleTypeDef *handle = NULL;

void retarget_init(UART_HandleTypeDef *huart, uart_init_t *init)
{
	handle = huart;
	if (init)
		init();
}

int _write(int file, char *ptr, int len)
{
	if (handle)
		HAL_UART_Transmit(handle, (uint8_t *)ptr, len, HAL_MAX_DELAY);
	return len;
}
