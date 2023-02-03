#pragma once
#include <vector>
#include "Coprocessor.h"


class GPU : public Coprocessor
{
public:
	GPU();
	virtual void ExecuteCycle() override;
	virtual void Call(CPU* cpu) override;
	
	void RenderMeshes();

private:
	std::vector<std::pair<uint32_t, unsigned int>> m_VBOs;
	unsigned int m_VertexArray;
};

