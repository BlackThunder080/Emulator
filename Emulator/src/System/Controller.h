#pragma once
#include <vector>
#include <GLFW/glfw3.h>
#include "Coprocessor.h"


class CPU;
class Controller : public Coprocessor
{
public:
	virtual void ExecuteCycle() override;
	virtual void Call(CPU* cpu) override;

	void SetKeyState(int key, int state)
	{
		if (m_KeyState.size() <= key)
			m_KeyState.resize(key + 1);
		m_KeyState[key] = state;
	}
private:
	std::vector<int> m_KeyState;
};