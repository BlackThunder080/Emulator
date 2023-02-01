lui $1, 0x08

lui $2, 0xbf00
sw  $2, $1, 0x0000
sw  $2, $1, 0x0004
li  $2, 0x0000
sw  $2, $1, 0x0008
lui $2, 0x3f00
sw  $2, $1, 0x000C
sw  $2, $1, 0x0010
lui $2, 0xbf00
sw  $2, $1, 0x0014


li  $0, 3
cowr $1, $0, $1
	
loop:
	b loop