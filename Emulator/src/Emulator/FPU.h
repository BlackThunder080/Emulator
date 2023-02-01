#pragma once
#include "Coprocessor.h"

class FPU : public Coprocessor
{
public:
	virtual void ExecuteCycle() override {};
	virtual uint32_t Read(CPU* cpu, uint8_t address) override;
	virtual void Write(CPU* cpu, uint8_t address, uint32_t value) override;
private:
	float a = 0, b = 0;
};