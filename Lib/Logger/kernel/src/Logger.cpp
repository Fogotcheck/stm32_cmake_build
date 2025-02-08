#include "Logger.h"

Logger::Logger(LoggerPort *p_port)
{
	handle.port = p_port;

	handle.rtos.xQueue = xQueueCreate(10, sizeof(LoggerMSG_t));
	if (handle.rtos.xQueue == nullptr) {
		Error_Handler();
	}

	strcat(handle.name, LOGGER_TASK_NAME);
	strcat(handle.name, "::\t");

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
	vQueueDelete(handle.rtos.xQueue);
}

int32_t Logger::AddMSG(char *ModuleName, const uint8_t *Data, uint16_t Size)
{
	ModuleName[LOGGER_MODULE_SIZE_NAME - 1] = 0;
	uint8_t NameSize = strlen(ModuleName);

	uint16_t offset = 0;
	while (offset < Size) {
		LoggerMSG_t msg = { 0 };

		if (offset == 0) {
			memcpy(msg.data, (uint8_t *)ModuleName, NameSize);
		}

		uint16_t availableDataSize =
			LOGGER_DATA_SIZE - (offset == 0 ? NameSize : 0);

		uint16_t dataSizeToCopy = (Size - offset > availableDataSize) ?
						  availableDataSize :
						  Size - offset;

		memcpy(msg.data + (offset == 0 ? NameSize : 0), Data + offset,
		       dataSizeToCopy);
		msg.size = (offset == 0 ? NameSize : 0) + dataSizeToCopy;

		BaseType_t xStatus = xQueueSend(handle.rtos.xQueue, &msg, 0);
		if (xStatus != pdPASS) {
			return -1;
		}

		offset += dataSizeToCopy;
	}

	return 0;
}

void Logger::LoggerThr(void *arg)
{
	Logger *log = static_cast<Logger *>(arg);
	static LoggerMSG_t msg = { 0 };

	if (log->AddMSG(log->handle.name, (uint8_t *)"Init::OK\r\n",
			sizeof("Init::\tOK\r\n"))) {
		vTaskDelete(log->handle.rtos.xTask);
	}

	while (1) {
		if (xQueueReceive(log->handle.rtos.xQueue, &msg,
				  portMAX_DELAY) == pdPASS) {
			log->handle.port->write(msg.data, msg.size);
			vTaskDelay(100);
		}
	}
}
