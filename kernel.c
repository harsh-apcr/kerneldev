#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/*Check if the compiler target OS is linux */
#if defined(__linux__)
#error "Compiler target is not specified correctly"
#endif

/*Only i386 compiler can compile this kernel*/
#if !defined(__i386__)
#error "Kernel needs to be compiled with ix86-elf compiler"
#endif

// Color is determined by a 4-bit number
enum vga_color {
	VGA_COLOR_BLACK = 0x0,
	VGA_COLOR_BLUE = 0x1,
	VGA_COLOR_GREEN = 0x2,
	VGA_COLOR_CYAN = 0x3,
	VGA_COLOR_RED = 0x4,
	VGA_COLOR_MAGENTA = 0x5,
	VGA_COLOR_BROWN = 0x6,
	VGA_COLOR_LIGHT_GREY = 0x7,
	VGA_COLOR_DARK_GREY = 0x8,
	VGA_COLOR_LIGHT_BLUE = 0x9,
	VGA_COLOR_LIGHT_GREEN = 0xA,
	VGA_COLOR_LIGHT_CYAN = 0xB,
	VGA_COLOR_LIGHT_RED = 0xC,
	VGA_COLOR_LIGHT_MAGENTA = 0xD,
	VGA_COLOR_LIGHT_BROWN = 0xE,
	VGA_COLOR_WHITE = 0xF
};


// fg and bg of text-color (high 4-bit : bg, low 4-bit : fg)
static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
	return fg | bg << 4;
}

// VGA-text entry (MSB - ASCII text, LSB - text fg/bg colors)
static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
	return (uint16_t) uc | (uint16_t) color << 8;
}

size_t strlen(const char* str) {
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

void terminal_initialize(void) {
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	
	// display buffer (at addr < 1MB)
	terminal_buffer = (uint16_t*) 0xB8000;
	
	for(size_t y= 0;y < VGA_HEIGHT;y++) {
		for(size_t x = 0;x < VGA_WIDTH;x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

void terminal_setcolor(uint8_t color) {
	terminal_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

void terminal_putchar(char c) {
	terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
	if (++terminal_column == VGA_WIDTH) {
		terminal_column = 0;
		if(++terminal_row == VGA_HEIGHT)
			terminal_row = 0;
	}
}


void terminal_write(const char *data, size_t size) {
	for(size_t i = 0;i < size;i++)
		terminal_putchar(data[i]);
}

void terminal_writestring(const char *data) {
	terminal_write(data, strlen(data));
}

void kernel_main(void) {
	/*initialize terminal window*/
	terminal_initialize();
	/*newline support is left as an exercise*/
	terminal_writestring("Hello, kernel !\n");
}














