#include "main.h"
#include "usart.h"
#include "Logger.h"
#include <stdio.h>
void MainThr(__attribute__((unused)) void *arg);
void TestThr(void *arg);
Logger *pLog = nullptr;
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
	pLog = &log;
	vTaskDelay(1000);
	for (uint32_t i = 0; i < 20; i++) {
		BaseType_t ret = xTaskCreate(TestThr, "TestTask", 256,
					     (void *)i, (i + 1), NULL);
		if (ret != pdPASS) {
			Error_Handler();
		}
	}

	while (1) {
		HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);
		vTaskDelay(1000);
	}
}

void TestThr(void *arg)
{
	uint32_t i = (uint32_t)arg;

	char tmp[256] = { 0 };
	char name[16] = { 0 };
	sprintf((char *)name, "%s::%lu::\t", __func__, i);
	sprintf((char *)tmp,
		"mama_mama_mama_mama_mama_mama_mama_mama_mama_mama_mama_mama_mama_mama_mama_mama_mama_mama_mama_mama_mama_mama_mama_mama_\r\n");

	while (1) {
		if (pLog) {
			if (pLog->AddMSG(name, (uint8_t *)tmp, strlen(tmp))) {
				Error_Handler();
			}
		}

		vTaskDelay(500);
	}
}
