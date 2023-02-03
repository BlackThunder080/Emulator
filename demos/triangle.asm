li  $4, 0x0000
lui $4, 0x0008


li  $0, 0x0000
lui $0, 0xbf00
sw  $0, $4, 0x0000

li  $0, 0x0000
lui $0, 0xbf00
sw  $0, $4, 0x0004

li  $0, 0x0000
lui $0, 0x0000
sw  $0, $4, 0x0008

li  $0, 0x0000
lui $0, 0x3f00
sw  $0, $4, 0x000C

li  $0, 0x0000
lui $0, 0x3f00
sw  $0, $4, 0x0010

li  $0, 0x0000
lui $0, 0xbf00
sw  $0, $4, 0x0014


li $1, 1
sys 1

loop:
	li $1, 2
	li $2, 0
	li $3, 3
	sys 1

	b loop