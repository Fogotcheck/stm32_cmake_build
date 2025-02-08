#include "main.h"
#include "usart.h"
#include "Logger.h"

void MainThr(__attribute__((unused)) void *arg);

int main(void)
{
	system_init();
	BaseType_t ret = xTaskCreate(MainThr, "MainTask", 500, NULL, 1, NULL);
	if (ret != pdPASS) {
		Error_Handler();
	}
	vTaskStartScheduler();
	Error_Handler();
}

void MainThr(__attribute__((unused)) void *arg)
{
	if (osKernelInitialize()) {
		Error_Handler();
	}

	LoggerPortUsart loggerPort_usart;

	Logger log(&loggerPort_usart);
	while (1) {
		HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);
		vTaskDelay(1000);
	}
}
