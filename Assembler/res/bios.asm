lui $3, 0xbf00
li  $4, 0xb717
lui $4, 0x38d1


loop:
	cowr $0, $0, $3
	cowr $0, $1, $4
	cord $3, $0, $3

	li  $0, 0x80000
	lui $1, 0x08
	sw  $3, $1, 0x0000
	sw  $2, $1, 0x0004
	lui $2, 0x0000
	sw  $2, $1, 0x0008
	lui $2, 0x3f00
	sw  $2, $1, 0x000C
	sw  $2, $1, 0x0010
	lui $2, 0xbf00
	sw  $2, $1, 0x0014
	
	b loop