// **** libc ****

#include <stdint.h>
#include <stdbool.h>

// Function declarations
void delay(uint32_t a);
void assert_fatal(bool condition, const char *msg);
void *memset(void *str, int c, unsigned int n);
void *memcpy(void *dest, const void *src, unsigned int len);
int memcmp(const void *ptr1, const void *ptr2, unsigned int num);
