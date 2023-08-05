#ifndef _KERNEL_DEBUG_H
#define _KERNEL_DEBUG_H

/*
Debugger that prints debug messages to serial port
We'll implement this for (PC16550D UART) in arch/i686/
*/

#include <stddef.h>
#include <stdbool.h>

void serial_initialize();
void debug_putchar(const char data);
void debug_print(const char *message);

#endif
