#include <stdint.h>
#include <stdbool.h>
#include "board/early_init.h"
#include "board/config.h"

// STM32 peripheral includes will be needed for DBGMCU and SCB
// These are defined in the STM32 HAL headers that are included via config

// Forward declarations for external dependencies
void enable_interrupts(void);
void disable_interrupts(void);
void init_registers(void);
void NVIC_SystemReset(void);
void early_gpio_float(void);
void detect_board_type(void);
void led_init(void);
void led_set(int led, int state);

// External variables
extern void *g_pfnVectors;
extern uint32_t enter_bootloader_mode;
extern uint8_t global_critical_depth;
extern struct board *current_board;

// Constants
#define LED_GREEN 1

static void jump_to_bootloader(void) {
  // do enter bootloader
  enter_bootloader_mode = 0;

  bootloader_fcn_ptr bootloader_ptr = (bootloader_fcn_ptr)BOOTLOADER_ADDRESS;
  bootloader_fcn bootloader = *bootloader_ptr;

  // jump to bootloader
  enable_interrupts();
  bootloader();

  // reset on exit
  enter_bootloader_mode = BOOT_NORMAL;
  NVIC_SystemReset();
}

void early_initialization(void) {
  // Reset global critical depth
  disable_interrupts();
  global_critical_depth = 0;

  // Init register and interrupt tables
  init_registers();

  // after it's been in the bootloader, things are initted differently, so we reset
  if ((enter_bootloader_mode != BOOT_NORMAL) &&
      (enter_bootloader_mode != ENTER_BOOTLOADER_MAGIC) &&
      (enter_bootloader_mode != ENTER_SOFTLOADER_MAGIC)) {
    enter_bootloader_mode = BOOT_NORMAL;
    NVIC_SystemReset();
  }

  // if wrong chip, reboot
  volatile unsigned int id = DBGMCU->IDCODE;
  if ((id & 0xFFFU) != MCU_IDCODE) {
    enter_bootloader_mode = ENTER_BOOTLOADER_MAGIC;
  }

  // setup interrupt table
  SCB->VTOR = (uint32_t)&g_pfnVectors;

  // early GPIOs float everything
  early_gpio_float();

  detect_board_type();

  if (enter_bootloader_mode == ENTER_BOOTLOADER_MAGIC) {
    led_init();
    current_board->init_bootloader();
    led_set(LED_GREEN, 1);
    jump_to_bootloader();
  }
}