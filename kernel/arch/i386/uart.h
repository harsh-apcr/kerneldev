#ifndef ARCH_I386_UART_H
#define ARCH_I386_UART_H

enum uart_register_offsets {
	UART_DATA_REG = 0x00,
	UART_IER_REG = 0x01,
	UART_FCR_REG = 0x02,
	UART_LCR_REG = 0x03,
	UART_MCR_REG = 0x04,
	UART_LSR_REG = 0x05
};

#define UART_PORT 0x3F8

#endif
