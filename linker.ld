/*The bootloader looks at this image and starts execution at the symbol designated as entry point*/
ENTRY(_start)

/*Tell where the various sections of object file will be put in the final image*/
SECTIONS {

	/*Begin putting sections at 1M*/
	. = 1M;
	
	/*firstly, put multiboot header, it is required to be put very early in the image
	Next, put the text section*/
	.text BLOCK(4K) : ALIGN(4K) {
		*(.multiboot)
		*(.text)
	}
	
	/*Read-only data*/
	.rodata BLOCK(4K) : ALIGN(4K) {
		*(.rodata)
	}
	
	/*Read-write data*/
	.data BLOCK(4K) : ALIGN(4K) {
		*(.data) 
	}
	
	.bss BLOCK(4K) : ALIGN(4K) {
		*(COMMON)
		*(.bss)
	}
	
	/*Compiler may put other sections, by default linker will put them in a segment with same
	name */
}



