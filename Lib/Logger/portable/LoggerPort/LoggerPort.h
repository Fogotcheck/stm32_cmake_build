#pragma once

#include <cstdint>

class LoggerPort {
    public:
	// Виртуальный деструктор (важно для полиморфизма)

	virtual ~LoggerPort() = default;

	// Чисто виртуальные методы (должны быть реализованы в производных классах)
	virtual int32_t write(const uint8_t *data, uint16_t size) = 0;
	virtual int32_t read(uint8_t *data, uint16_t size) = 0;
	virtual void init() = 0;
};
