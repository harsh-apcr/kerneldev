#ifndef _ARCH_I386_IO_H
#define _ARCH_I386_IO_H

#include <stdint.h>

static inline uint8_t inb(uint16_t port) {
	uint8_t data;
	asm volatile("inb %1, %0" : "=a" (data) : "d" (port));
	return data;
}

static inline void outb(uint16_t port, uint8_t data) {
	asm volatile("outb %0, %1" : : "a" (data), "d" (port));
}

static inline uint16_t inw(uint16_t port) {
	uint16_t data;
	asm volatile("inw %1, %0" : "=a" (data) : "d" (port));
	return data;
}

static inline void outw(uint16_t port, uint16_t data) {
	asm volatile("outw %0, %1" : : "a" (data), "d" (port));
}

#endif
