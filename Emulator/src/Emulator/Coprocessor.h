#pragma once
#include <cstdint>

class Coprocessor
{
public:
	virtual void ExecuteCycle()            = 0;
	virtual uint32_t Read(uint32_t)        = 0;
	virtual void Write(uint32_t, uint32_t) = 0;
};