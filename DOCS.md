# Emulator
## Instructions
* MW [reg1], [reg2]         -> reg1 = reg2
* LW [reg1], [reg2], [addr] -> reg1 = [reg1 + addr]
* SW [reg1], [reg2], [addr] -> [reg2 + addr] = reg1
* LI [reg], [imm]           -> reg = imm
* LUI [reg], [imm]          -> (reg << 16) = imm
* B [addr]                  -> pc = addr
* SYS [coprocessor]         -> coprocessors[coprocessors].Call()
