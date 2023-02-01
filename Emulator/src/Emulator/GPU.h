#pragma once
#include <vector>
#include "Coprocessor.h"


class CPU;

class GPU : public Coprocessor
{
public:
	GPU();
	virtual void ExecuteCycle() override;
	virtual uint32_t Read(class CPU* cpu, uint8_t address) override;
	virtual void Write(class CPU* cpu, uint8_t address, uint32_t value) override;
	
	void RenderMeshes();

private:
	std::vector<std::pair<uint32_t, unsigned int>> m_VBOs;
	unsigned int m_VertexArray;
};

