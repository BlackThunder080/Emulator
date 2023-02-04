# Emulator
## Docs
### Instructions
* MW [reg1], [reg2]         -> reg1 = reg2
* LW [reg1], [reg2], [addr] -> reg1 = [reg1 + addr]
* SW [reg1], [reg2], [addr] -> [reg2 + addr] = reg1
* LI [reg], [imm]           -> reg = imm
* LUI [reg], [imm]          -> (reg << 16) = imm
* B [addr]                  -> pc = addr
* SYS [coprocessor]         -> coprocessors[coprocessors].Call()

### Coprocessor Calls
* FPU
  * float IntToFloat(int value)
  * int FloatToInt(float value)
  * float AddFloats(float a, float b)
  * float SubFloats(float a, float b)
  * float MulFloats(float a, float b)
  * float DivFloats(float a, float b)
* GPU
  * void ClearAllMeshes()
  * void AddNewMesh()
  * void UploadMesh(int index, int numvertices, const void* vertices, int texture)
  * void UploadTexture(itn index, const void* pixels)
* Controller
  * int GetButtonState(int button)
