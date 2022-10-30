#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "nox_soc.h"
#include "uart.h"

volatile uint32_t* const addr_print  = (uint32_t*) UART_TX_SIM;
volatile uint32_t* const uart_stats  = (uint32_t*) UART_STATS;
volatile uint32_t* const uart_print  = (uint32_t*) UART_TX;
volatile uint32_t* const uart_rx_csr = (uint32_t*) UART_RX;
volatile uint32_t* const uart_cfg    = (uint32_t*) UART_CFG;

/// \cond
// Private functions
static void __uart_itoa(uint32_t x, char *res) __attribute__((unused)); // GCC: do not output a warning when this variable is unused
static void __uart_tohex(uint32_t x, char *res) __attribute__((unused)); // GCC: do not output a warning when this variable is unused
static void __uart_touppercase(uint32_t len, char *ptr) __attribute__((unused)); // GCC: do not output a warning when this variable is unused
/// \endcond

#ifdef UART_SIM
  #warning UART - Verilator mode
#else
  #warning UART - Real serial mode
#endif
// #################################################################################################
// Override default STDIO functions
// #################################################################################################

/**********************************************************************//**
 * Send char via UART0
 *
 * @param[in] Char to be send.
 * @return Char that has been sent.
 **************************************************************************/
int putchar(int ch) {
  uart0_putc((char)ch);
  return ch;
}


/**********************************************************************//**
 * Read char from UART0.
 *
 * @return Read char.
 **************************************************************************/
int getchar(void) {
  return (int)uart0_getc();
}

/**********************************************************************//**
 * Enable and configure primary UART (UART0).
 *
 * @note The 'UART0_SIM_MODE' compiler flag will configure UART0 for simulation mode: all UART0 TX data will be redirected to simulation output. Use this for simulations only!
 * @note To enable simulation mode add <USER_FLAGS+=-DUART0_SIM_MODE> when compiling.
 *
 * @warning The baud rate is computed using INTEGER operations (truncation errors might occur).
 *
 * @param[in] baudrate Targeted BAUD rate (e.g. 9600).
 **************************************************************************/
void uart0_setup(uint32_t baudrate) {
  *uart_cfg = FREQ_SYSTEM/baudrate;
}

/**********************************************************************//**
 * Send single char via UART0.
 *
 * @note This function is blocking.
 *
 * @param[in] c Char to be send.
 **************************************************************************/
void uart0_putc(char c) {
#ifdef UART_SIM
  *addr_print = c;
#else
  while((*uart_stats & 0x10000) == 0);
  *uart_print = c;
#endif
}

/**********************************************************************//**
 * Get char from UART0.
 *
 * @note This function is blocking and does not check for UART frame/parity errors.
 *
 * @return Received char.
 **************************************************************************/
char uart0_getc(void) {
  uint32_t d = 0;
  d = *uart_rx_csr;
  return (char)d;
}


/**********************************************************************//**
 * Check if UART0 has received a char.
 *
 * @note This function is non-blocking.
 * @note Use uart0_char_received_get(void) to get the char.
 *
 * @return =!0 when a char has been received.
 **************************************************************************/
int uart0_char_received(void) {
  if ((*uart_stats & 0x1) == 0) {
    return 0;
  }
  else {
    return 1;
  }
}


/**********************************************************************//**
 * Get a received char from UART0.
 *
 * @note This function is non-blocking.
 * @note Should only be used in combination with uart_char_received(void).
 *
 * @return Received char.
 **************************************************************************/
char uart0_char_received_get(void) {
  return (char)*uart_rx_csr;
}


/**********************************************************************//**
 * Print string (zero-terminated) via UART0. Print full line break "\r\n" for every '\n'.
 *
 * @note This function is blocking.
 *
 * @param[in] s Pointer to string.
 **************************************************************************/
void uart0_print(const char *s) {
  char c = 0;
  while ((c = *s++)) {
    if (c == '\n') {
      uart0_putc('\r');
    }
    uart0_putc(c);
  }
}


/**********************************************************************//**
 * Custom version of 'printf' function using UART0.
 *
 * @note This function is blocking.
 *
 * @param[in] format Pointer to format string.
 *
 * <TABLE>
 * <TR><TD>%s</TD><TD>String (array of chars, zero-terminated)</TD></TR>
 * <TR><TD>%c</TD><TD>Single char</TD></TR>
 * <TR><TD>%d/%i</TD><TD>32-bit signed number, printed as decimal</TD></TR>
 * <TR><TD>%u</TD><TD>32-bit unsigned number, printed as decimal</TD></TR>
 * <TR><TD>%x</TD><TD>32-bit number, printed as 8-char hexadecimal - lower-case</TD></TR>
 * <TR><TD>%X</TD><TD>32-bit number, printed as 8-char hexadecimal - upper-case</TD></TR>
 * <TR><TD>%p</TD><TD>32-bit pointer, printed as 8-char hexadecimal - lower-case</TD></TR>
 * </TABLE>
 **************************************************************************/
void uart0_printf(const char *format, ...) {

  char c, string_buf[11];
  int32_t n;

  va_list a;
  va_start(a, format);

  while ((c = *format++)) {
    if (c == '%') {
      c = *format++;
      switch (c) {
        case 's': // string
          uart0_print(va_arg(a, char*));
          break;
        case 'c': // char
          uart0_putc((char)va_arg(a, int));
          break;
        case 'i': // 32-bit signed
        case 'd':
          n = (int32_t)va_arg(a, int32_t);
          if (n < 0) {
            n = -n;
            uart0_putc('-');
          }
          __uart_itoa((uint32_t)n, string_buf);
          uart0_print(string_buf);
          break;
        case 'u': // 32-bit unsigned
          __uart_itoa(va_arg(a, uint32_t), string_buf);
          uart0_print(string_buf);
          break;
        case 'x': // 32-bit hexadecimal
        case 'p':
        case 'X':
          __uart_tohex(va_arg(a, uint32_t), string_buf);
          if (c == 'X') {
            __uart_touppercase(11, string_buf);
          }
          uart0_print(string_buf);
          break;
        default: // unsupported format
          uart0_putc('%');
          uart0_putc(c);
          break;
      }
    }
    else {
      if (c == '\n') {
        uart0_putc('\r');
      }
      uart0_putc(c);
    }
  }
  va_end(a);
}

// #################################################################################################
// Shared functions
// #################################################################################################

/**********************************************************************//**
 * Private function for 'printf' to convert into decimal.
 *
 * @param[in] x Unsigned input number.
 * @param[in,out] res Pointer for storing the reuslting number string (11 chars).
 **************************************************************************/
static void __uart_itoa(uint32_t x, char *res) {

  static const char numbers[] = "0123456789";
  char buffer1[11];
  uint16_t i, j;

  buffer1[10] = '\0';
  res[10] = '\0';

  // convert
  for (i=0; i<10; i++) {
    buffer1[i] = numbers[x%10];
    x /= 10;
  }

  // delete 'leading' zeros
  for (i=9; i!=0; i--) {
    if (buffer1[i] == '0')
      buffer1[i] = '\0';
    else
      break;
  }

  // reverse
  j = 0;
  do {
    if (buffer1[i] != '\0')
      res[j++] = buffer1[i];
  } while (i--);

  res[j] = '\0'; // terminate result string
}


/**********************************************************************//**
 * Private function for 'printf' to convert into hexadecimal.
 *
 * @param[in] x Unsigned input number.
 * @param[in,out] res Pointer for storing the resulting number string (9 chars).
 **************************************************************************/
static void __uart_tohex(uint32_t x, char *res) {

  static const char symbols[] = "0123456789abcdef";

  int i;
  for (i=0; i<8; i++) { // nibble by nibble
    uint32_t num_tmp = x >> (4*i);
    res[7-i] = (char)symbols[num_tmp & 0x0f];
  }

  res[8] = '\0'; // terminate result string
}


/**********************************************************************//**
 * Private function to cast a string to UPPERCASE.
 *
 * @param[in] len Total length of input string.
 * @param[in,out] ptr Pointer for input/output string.
 **************************************************************************/
static void __uart_touppercase(uint32_t len, char *ptr) {

  char tmp;

  while (len > 0) {
    tmp = *ptr;
    if ((tmp >= 'a') && (tmp <= 'z')) {
      *ptr = tmp - 32;
    }
    ptr++;
    len--;
  }
}
