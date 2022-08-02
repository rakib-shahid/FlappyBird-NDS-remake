
@{{BLOCK(bird)

@=======================================================================
@
@	bird, 22x16@8, 
@	+ palette 256 entries, not compressed
@	+ bitmap not compressed
@	Total size: 512 + 384 = 896
@
@	Time-stamp: 2022-08-01, 20:21:28
@	Exported by Cearn's GBA Image Transmogrifier, v0.8.17
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global birdBitmap		@ 384 unsigned chars
	.hidden birdBitmap
birdBitmap:
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x40406766,0x47110F0C,0x00000041,0x00000000,0x00000000,0x38383F00,0x16163C09,0x2C1E255F
	.word 0x00003727,0x00000000,0x00000000,0x163C2600,0x3B464646,0x2C2C3919,0x0000302C,0x00000000
	.word 0x00000000,0x3C200000,0x3B464646,0x2C2C5A33,0x00134315,0x00000000,0x0D0D0500,0x013E0D0D
	.word 0x3B46165E,0x2C2C3919,0x00494341,0x00000000,0x2C2C2F00,0x4C4B2C2C,0x3B465434,0x2C4B1238
	.word 0x00080252,0x00000000,0x2C2C0200,0x4E2C2C2C,0x46160E29,0x14171A3C,0x002E2C2C,0x00000000
	.word 0x2C184500,0x23222C2C,0x46160E56,0x58032146,0x00115858,0x00000000,0x631B0F00,0x4F066335
	.word 0x46165F66,0x4462573C,0x2D444444,0x0000042D,0x593F0000,0x6126593D,0x5E51461C,0x5B68622B

	.word 0x07696969,0x00001F5B,0x00000000,0x2A280000,0x103A103A,0x00311D56,0x00000000,0x00000000
	.word 0x00000000,0x425D0000,0x10533A2A,0x314D4824,0x31313131,0x0000000A,0x00000000,0x36330000
	.word 0x552A320B,0x1D0B6442,0x1D1D1D1D,0x00000050,0x00000000,0x00000000,0x60246500,0x004A5C64
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000

	.section .rodata
	.align	2
	.global birdPal		@ 512 unsigned chars
	.hidden birdPal
birdPal:
	.hword 0x0000,0x014B,0x5294,0x0297,0x0067,0x294A,0x2B3B,0x0159
	.hword 0x739C,0x033D,0x0045,0x012D,0x0254,0x4A52,0x01D1,0x00E8
	.hword 0x029B,0x0C41,0x31CE,0x6318,0x7BDE,0x2108,0x039F,0x18A5
	.hword 0x33FF,0x0001,0x018F,0x0E54,0x02FC,0x00D0,0x39AD,0x00AC
	.hword 0x012B,0x037E,0x67FF,0x139F,0x0258,0x11AF,0x01F2,0x39CE
	.hword 0x00A7,0x1042,0x02DD,0x02D8,0x7FFF,0x00F2,0x6739,0x5AD6
	.hword 0x4210,0x017B,0x0216,0x0022,0x0655,0x3FDF,0x014D,0x0C63
	.hword 0x0109,0x41EE,0x02BC,0x035F,0x03BF,0x0298,0x35F0,0x0064

	.hword 0x0213,0x0842,0x02FE,0x3DEF,0x00D1,0x0A76,0x037F,0x4E73
	.hword 0x010C,0x77BD,0x01B2,0x6B39,0x1CA5,0x0048,0x7BFF,0x016D
	.hword 0x0023,0x033F,0x2D6B,0x029C,0x031C,0x02BD,0x0021,0x02B7
	.hword 0x1062,0x02B8,0x3DEE,0x017A,0x0259,0x00A8,0x033E,0x02D9
	.hword 0x0237,0x0065,0x014E,0x3FFF,0x0236,0x012C,0x01AF,0x0233
	.hword 0x0049,0x015A,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000

	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000

	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000

@}}BLOCK(bird)
