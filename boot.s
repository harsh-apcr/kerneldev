.set ALIGN,		1<<0			# memory is pgaligned
.set MEMINFO,		1<<1   			# provides memmap
.set FLAGS, 		ALIGN | MEMINFO 	# set the multiboot flag field
.set MAGIC, 		0x1BADB002		# 'magic number' lets bootloader find the header
.set CHECKSUM,		-(MAGIC+FLAGS)		# checksum of above, to prove we are multiboot


/*Declare a multiboot header that marks the program as kernel. These are magic values
that are documented in the multiboot standard. The bootloader will search for this 
signature in the first 8KB aligned at 32-bit boundary */
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM


/*Set up stack before writing C-programs. Stack grows downwards on x86 systems*/
.section .bss
.align 16
stack_bottom:
.skip 16384 # 16KB
stack_top:

/*define _start as the entry point to the kernel and the bootloader will jump to this
position once the kernel has been loaded. It doesn't make sense to return from this
function as the bootloader is gone*/
.section .text
.global _start
.type _start, @function
_start:
	/*Enter the high-level kernel. The ABI requires the stack is 16-byte aligned
	at the time of call instruction and we've pushed 16KB to the stack since, so
	the alignment has been preserved and the call is well defined.*/
	call kernel_main

	/*
	If the system has nothing more to do, put the system into an infinite loop
	*/
	cli				# disable external interrupts (clears eflags interrupt bit)
1:	hlt				# hlt the current cpu until next interrupt arrives
	jmp 1b			# jump back to 1

.size _start, . - _start



