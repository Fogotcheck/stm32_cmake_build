#pragma once

#include "stm32f4xx_system_init.h"
#include "LoggerPort.h"

class LoggerPortUsart : public LoggerPort {
    private:
    public:
	LoggerPortUsart();
	~LoggerPortUsart() override;

	int32_t write(const uint8_t *Data, uint16_t Size) override;
	int32_t read(uint8_t *Data, uint16_t Size) override;
	void init(void) override;
};
