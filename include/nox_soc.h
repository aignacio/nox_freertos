#ifndef NOX_SOC_H
#define NOX_SOC_H

#define BOOT_ROM_BASE_ADDR      0x00000000
#define DRAM_BASE_ADDR          0x10000000
#define IRAM_BASE_ADDR          0xA0000000
#define UART_BASE_ADDR          0xB0000000
#define RESET_CTRL_BASE_ADDR    0xC0000000
#define MTIMER_BASE_ADDR        0xF0000000

#define MTIMER_CMP_ADDR         (MTIMER_BASE_ADDR + 0x08)

//**********
//  UART   *
//**********
#define UART_TX                 (UART_BASE_ADDR + 0x0C)
#define UART_RX                 (UART_BASE_ADDR + 0x08)
#define UART_STATS              (UART_BASE_ADDR + 0x04)
#define UART_CFG                (UART_BASE_ADDR + 0x00)
#define UART_TX_SIM             (RESET_CTRL_BASE_ADDR + 0x10)
#define FREQ_SYSTEM             50000000 // 50MHz
#define BR_UART                 115200

#endif
