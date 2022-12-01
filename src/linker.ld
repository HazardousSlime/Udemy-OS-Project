ENTRY(_start)
OUTPUT_FORMAT(binary)
SECTIONS
{
	. = 1M;				/*Start at 1M into memory*/	
	.text : ALIGN(4096)
	{
		*(.text)	
	}
	.rodata : ALIGN(4096)

	{
		*(.rodata)
	}

	.data : ALIGN(4096)

	{
		*(.data)
	}

	.bss : ALIGN(4096)
	{
		*(COMMON)
		*(.bss)
	}
	.asm :	ALIGN(4096)		/*Place assembly at the end to prevent alignment issues*/
	{
		*(.asm)
	}
}
