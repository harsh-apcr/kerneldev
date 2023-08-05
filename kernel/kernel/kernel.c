#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/debug.h>

void kernel_main(void) {
	
	serial_initialize();
	debug_print("Hello serial!\n");
	
	terminal_initialize();
	printf("Hello, kernel World!\n");
}
