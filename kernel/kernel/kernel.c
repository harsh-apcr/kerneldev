#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/debug.h>
#include <kernel/kbd_controller.h>

void kernel_main(void) {
	
	serial_initialize();
	debug_print("Hello serial!\n");
	
	kbd_controller_init();
	
	terminal_initialize();
	printf("Hello, kernel World!\n");
}
