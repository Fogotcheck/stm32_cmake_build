#include "main.h"

#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "cmsis_os2.h"

#include "logger.h"

void MainThr(__attribute__((unused)) void *arg);

typedef struct {
	const char *name = "MainTask";
	const StackType_t stack = 512;
	const UBaseType_t priority = osPriorityLow;

	TaskHandle_t handle = NULL;
} app_t;

int main(void)
{
	system_init();

	app_t *app = new app_t();
	if (app == nullptr)
		Error_Handler();

	BaseType_t ret = xTaskCreate(MainThr, app->name, app->stack,
				     (void *)app, app->priority, &app->handle);
	if (ret != pdPASS) {
		Error_Handler();
	}

	extern UART_HandleTypeDef huart3;
	retarget_init(&huart3, MX_USART3_UART_Init);

	vTaskStartScheduler();
	Error_Handler();
}

void MainThr(__attribute__((unused)) void *arg)
{
	if (osKernelInitialize()) {
		Error_Handler();
	}

	app_t *app = (app_t *)arg;
	if (app == nullptr) {
		FERROR("App task arg is nullptr");
		Error_Handler();
	}

	FINFO("Initial %s::\tOK", app->name);
	while (1) {
		HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);
		vTaskDelay(1000);
	}
}
