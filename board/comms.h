#pragma once

typedef struct {
  uint8_t request;
  uint16_t param1;
  uint16_t param2;
  uint16_t length;
} __attribute__((packed)) ControlPacket_t;

extern int _app_start[0xc000]; // Only first 3 sectors of size 0x4000 are used

// Prototypes
void set_safety_mode(uint16_t mode, uint16_t param);
bool is_car_safety_mode(uint16_t mode);

int comms_control_handler(ControlPacket_t *req, uint8_t *resp);
void comms_endpoint2_write(const uint8_t *data, uint32_t len);
void comms_can_write(const uint8_t *data, uint32_t len);
int comms_can_read(uint8_t *data, uint32_t max_len);
void comms_can_reset(void); 