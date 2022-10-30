#ifndef UART_H
#define UART_H

// Libs required by functions
#include <stdarg.h>
#include "nox_soc.h"

void uart0_setup(uint32_t baudrate);
void uart0_putc(char c);
char uart0_getc(void);
int  uart0_char_received(void);
char uart0_char_received_get(void);
void uart0_print(const char *s);
void uart0_printf(const char *format, ...);

#endif
