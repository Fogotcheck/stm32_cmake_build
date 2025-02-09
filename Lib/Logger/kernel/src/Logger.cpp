#include "Logger.h"

Logger::Logger(LoggerPort *p_port)
{
	handle.port = p_port;

	handle.rtos.xSemaphore = xSemaphoreCreateMutex();
	if (handle.rtos.xSemaphore == nullptr) {
		Error_Handler();
	}

	BaseType_t ret = xTaskCreate(
		[](void *arg) { static_cast<Logger *>(arg)->LoggerThr(arg); },
		LOGGER_TASK_NAME, LOGGER_STACK_SIZE, this, LOGGER_PRIORITY,
		&handle.rtos.xTask);
	if (ret != pdPASS) {
		Error_Handler();
	}
}

Logger::~Logger()
{
	vTaskDelete(handle.rtos.xTask);
	vSemaphoreDelete(handle.rtos.xSemaphore);
}

int32_t Logger::AddMSG(char *ModuleName, uint8_t *Data, uint16_t Size)
{
	if (ModuleName == nullptr || Data == nullptr || Size == 0) {
		return -1;
	}
	if (xSemaphoreTake(handle.rtos.xSemaphore, LOGGER_SEMAPHORE_TIMEOUT) ==
	    pdFALSE) {
		return -1;
	}

	msg.name = (uint8_t *)ModuleName;
	msg.data = Data;
	msg.size = Size;
	xTaskNotifyGive(handle.rtos.xTask);

	xSemaphoreGive(handle.rtos.xSemaphore);

	return 0;
}

void Logger::LoggerThr(void *arg)
{
	Logger *log = static_cast<Logger *>(arg);

	if (log->handle.port->write((uint8_t *)log->handle.name,
				    strlen(log->handle.name))) {
		Error_Handler();
	}

	static uint8_t tmp[] = "Init::\tOK\r\n";
	if (log->handle.port->write(tmp, sizeof(tmp))) {
		log->~Logger();
	}

	while (1) {
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		if (log->handle.port->write((uint8_t *)log->msg.name,
					    strlen((char *)log->msg.name))) {
			Error_Handler();
		}

		if (log->handle.port->write(log->msg.data, log->msg.size)) {
			Error_Handler();
		}
	}
}
