#pragma once

#include "string.h"

#include "stm32f4xx_system_init.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "LoggerPort.h"

#define LOGGER_TASK_NAME "LoggerThr"
#define LOGGER_STACK_SIZE configMINIMAL_STACK_SIZE
#define LOGGER_PRIORITY (configMAX_PRIORITIES - 1)
#define LOGGER_MODULE_SIZE_NAME 16
#define LOGGER_SEMAPHORE_TIMEOUT 1000
#define LOGGER_QUEUE_ITEM_SIZE 10

typedef struct LoggerMSG {
	uint8_t *name;
	uint8_t *data;
	uint16_t size;
} LoggerMSG_t;

typedef struct LoggerFreeRTOS {
	TaskHandle_t xTask;
	SemaphoreHandle_t xSemaphore = nullptr;
} LoggerFreeRTOS_t;

typedef struct LoggerHandle {
	char name[LOGGER_MODULE_SIZE_NAME + 1] = LOGGER_TASK_NAME "::\t";
	LoggerPort *port;
	LoggerFreeRTOS_t rtos;
} LoggerHandle_t;

class Logger {
    private:
	LoggerHandle_t handle;
	void LoggerThr(void *arg);
	LoggerMSG_t msg;

    public:
	Logger(LoggerPort *p_port);
	~Logger();
	int32_t AddMSG(char *ModuleName, uint8_t *Data, uint16_t Size);
};
