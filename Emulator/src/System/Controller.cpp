#include "Controller.h"

void Controller::ExecuteCycle()
{
}

void Controller::Call(CPU* cpu)
{
	switch (cpu->registers[1])
	{
	case 0x00:
		if (m_KeyState.size() <= cpu->registers[2])
			cpu->registers[0] = 0;
		else
			cpu->registers[0] = m_KeyState[cpu->registers[2]];
		break;
	}
}
