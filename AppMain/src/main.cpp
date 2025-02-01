#include "main.h"

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
	while (1) {
		HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);
		vTaskDelay(1000);
	}
}
