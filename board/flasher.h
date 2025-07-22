// from the linker script
#ifdef STM32H7
  #define APP_START_ADDRESS 0x8020000U
#elif defined(STM32F4)
  #define APP_START_ADDRESS 0x8004000U
#endif

#include <stdint.h>
#include <stdbool.h>

// Type definitions
typedef struct {
  uint8_t request;
  uint8_t param1;
  uint16_t param2;
  uint32_t param3;
} ControlPacket_t;

// flasher state variables
extern uint32_t *prog_ptr;
extern bool unlocked;

// Function declarations
void spi_init(void);
int comms_control_handler(ControlPacket_t *req, uint8_t *resp);
void comms_can_write(const uint8_t *data, uint32_t len);
int comms_can_read(uint8_t *data, uint32_t max_len);
void refresh_can_tx_slots_available(void);
void comms_endpoint2_write(const uint8_t *data, uint32_t len);
void soft_flasher_start(void);
