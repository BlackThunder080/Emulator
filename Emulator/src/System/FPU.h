#pragma once
#include "Coprocessor.h"

class CPU;
class FPU : public Coprocessor
{
public:
	virtual void ExecuteCycle() override;
	virtual void Call(CPU* cpu) override;
};