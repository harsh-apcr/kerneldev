#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <io.h>
#include <kernel/debug.h>

#include "uart.h"

void serial_initialize() {
	// Initialize UART
	outb(UART_PORT + UART_IER_REG, 0x00);
	outb(UART_PORT + UART_LCR_REG, 0x03);
	outb(UART_PORT + UART_FCR_REG, 0xC7);
	outb(UART_PORT + UART_LCR_REG, 0x03);
}

uint8_t is_transmitter_ready() {
	// check if 'thre' bit is set in line status register (UART_LSR_REG)
	uint8_t thre = inb(UART_PORT + UART_LSR_REG) & (1 << 5);
	return thre;
}

void write_char(uint8_t data) {
	// a simple write to the data register (UART_DATA_REG)
	outb(UART_PORT + UART_DATA_REG, data);
}

void debug_putchar(const char data) {
	while (!is_transmitter_ready());
	write_char(data);
}

void debug_print(const char *message) {
	for(;*message != 0;message++) {
		debug_putchar(*message);
	}
}

