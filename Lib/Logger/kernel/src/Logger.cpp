#include "Logger.h"

Logger::Logger(LoggerPort *p_port)
{
	handle.port = p_port;

	handle.rtos.xSemaphore = xSemaphoreCreateMutex();
	if (handle.rtos.xSemaphore == nullptr) {
		Error_Handler();
	}

	handle.rtos.xQueue =
		xQueueCreate(LOGGER_QUEUE_ITEM_SIZE, sizeof(LoggerMSG_t));
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
	vSemaphoreDelete(handle.rtos.xSemaphore);
	vQueueDelete(handle.rtos.xQueue);
}

int32_t Logger::AddMSG(char *ModuleName, const uint8_t *Data, uint16_t Size)
{
	if (ModuleName == nullptr || Data == nullptr || Size == 0) {
		return -1;
	}
	if (xSemaphoreTake(handle.rtos.xSemaphore, LOGGER_SEMAPHORE_TIMEOUT) ==
	    pdFALSE) {
		return -1;
	}

	strncpy(handle.name, ModuleName, LOGGER_MODULE_SIZE_NAME);
	handle.name[LOGGER_MODULE_SIZE_NAME] = '\0';

	uint8_t NameSize = strlen(handle.name);
	uint16_t availableDataSize = LOGGER_DATA_MSG_SIZE - NameSize;

	LoggerMSG_t msg = { 0 };

	memcpy(msg.data, handle.name, NameSize);

	uint16_t dataSizeToCopy =
		(Size < availableDataSize) ? Size : availableDataSize;
	memcpy(msg.data + NameSize, Data, dataSizeToCopy);
	msg.size = NameSize + dataSizeToCopy;

	QueueAddMSG(&msg);

	uint16_t offset = dataSizeToCopy;
	while (offset < Size) {
		msg = { 0 };

		uint16_t remainingDataSize = Size - offset;
		dataSizeToCopy = (remainingDataSize < availableDataSize) ?
					 remainingDataSize :
					 availableDataSize;

		memcpy(msg.data, Data + offset, dataSizeToCopy);
		msg.size = dataSizeToCopy;

		QueueAddMSG(&msg);

		offset += dataSizeToCopy;
	}
	xSemaphoreGive(handle.rtos.xSemaphore);

	return 0;
}

void Logger::QueueAddMSG(LoggerMSG_t *msg)
{
	BaseType_t xStatus = xQueueSend(handle.rtos.xQueue, msg, 10);
	if (xStatus != pdPASS) {
		vTaskDelay(10);
		xStatus = xQueueSend(handle.rtos.xQueue, msg, 10);
		if (xStatus != pdPASS) {
			xStatus = xQueueOverwrite(handle.rtos.xQueue, msg);
		}
	}
}

void Logger::LoggerThr(void *arg)
{
	Logger *log = static_cast<Logger *>(arg);
	static LoggerMSG_t msg = { 0 };

	uint8_t tmp[] =
		"mama_mama_mama_mama_mama_mama_mama_mama_mama_mama_mama_mama_mama_mama_mama_mama_mama_mama_mama_mama_mama_mama_mama_mama_ Init::\tOK\r\n";
	if (log->AddMSG(log->handle.name, tmp, sizeof(tmp))) {
		vTaskDelete(log->handle.rtos.xTask);
	}

	while (1) {
		if (xQueueReceive(log->handle.rtos.xQueue, &msg,
				  portMAX_DELAY) == pdPASS) {
			log->handle.port->write(msg.data, msg.size);
			vTaskDelay(1000);
		}
	}
}
