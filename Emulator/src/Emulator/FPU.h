#pragma once
#include "Coprocessor.h"

class FPU : public Coprocessor
{
public:
	virtual void ExecuteCycle() override {};
	virtual uint32_t Read(uint32_t address) override;
	virtual void Write(uint32_t address, uint32_t value) override;
private:
	float a, b;
};