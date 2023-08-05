#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <io.h>
#include <kernel/kbd_controller.h>
#include <kernel/debug.h>

#include "lpc_kbd.h"

bool kbd_output_buffer_empty() {
	return (inb(KBD_STATUS_REG) & 0x01) == 0;
}

bool kbd_input_buffer_empty() {
	return (inb(KBD_STATUS_REG) & 0x02) == 0;
}

void kbd_wait_input_buffer_empty() {
	while (!kbd_input_buffer_empty());
}

void kbd_send_command(uint8_t command) {
	kbd_wait_input_buffer_empty();
	outb(KBD_COMMAND_REG, command);
}


uint8_t kbd_read_config_byte() {
	kbd_send_command(KBD_CMD_READ_CONFIG_BYTE);
	return inb(KBD_INPUT_REG);
}

void kbd_write_config_byte(uint8_t data) {
	kbd_send_command(KBD_CMD_WRITE_CONFIG_BYTE);
	kbd_wait_input_buffer_empty();
	outb(KBD_INPUT_REG, data);
}

uint8_t kbd_status_rd() {
	return inb(KBD_STATUS_REG);
}

uint8_t kbd_input_rd() {
	return inb(KBD_INPUT_REG);
}

uint8_t kbd_test_port() {
	kbd_send_command(KBD_CMD_TEST_PORT);
	return inb(KBD_INPUT_REG);
}

int8_t ps2_controller_present() {
	uint32_t *rsdp = (uint32_t*) 0x000E0000;
	while ((uint32_t) rsdp < 0x000FFFFF) {
		if (*rsdp == FADT_SIGNATURE) {
			uint8_t *fadt = (uint8_t*)(rsdp+1);
			uint8_t iapc_boot_arch_flags = *(fadt + FADT_IAPC_BOOT_ARCH_FLAGS_OFFSET);
			return (iapc_boot_arch_flags & (1 << PS2_CONTROLLER_FLAG_BIT)) != 0;
		}
		rsdp += 4;
	}
	
	// ACPI not supported or FADT not found
	return 0;
}

// Perform PS/2 controller initialization
bool ps2_controller_init() {
	if (ps2_controller_present()) {
		// disable device
		kbd_send_command(KBD_CMD_DISABLE_KEYBOARD);
		
		// flush the output buffer
		while (!kbd_input_buffer_empty()) {
			kbd_input_rd();
		}
		
		// set the controller config byte
		uint8_t config_byte = kbd_read_config_byte();
		// disable all IRQs and translation
		config_byte &= ~0b01100001;
		kbd_write_config_byte(config_byte);
		
		// perform controller self test
		kbd_send_command(KBD_CMD_SELF_TEST);
		if (kbd_input_rd() != 0x55) {
			return false;	// controller self test failed
		}
		
		// peform interface tests
		if (kbd_test_port() != 0x00) {
			return false;
		}
		
		// enable devices
		kbd_send_command(KBD_CMD_ENABLE_KEYBOARD);
		
		// reset devices
		outb(KBD_INPUT_REG, 0xFF);
		
		return true;
	}
	return false;
}

void kbd_controller_init() {
	if (ps2_controller_init()) {
		debug_print("ps/2 controller initialized");
	} else {
		debug_print("couldn't initialize ps/2 controller");
	}
}

bool kbd_status_obf_extract(uint8_t status) {
	return (status & (1 << 0)) != 0;
}

bool has_key() {
	uint8_t status = kbd_status_rd();
	return kbd_status_obf_extract(status);
}

uint8_t get_key() {
	while (!has_key());
	return kbd_input_rd();
}

// Interact with the keyboard

void core_loop_step() {
	if (has_key()) {
		uint8_t input = get_key();
		// process the input key here
		debug_putchar(input);
	}
}

void core_loop() {
	while (true) {
		core_loop_step();
	}
}

