#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "app_uart.h"
#include "app_error.h"
#include "nrf_delay.h"
#include "nrf.h"
#include "bsp.h"
#include "nrf_uart.h"
#include "boards.h"

#define UART_TX_BUFF_SIZE 128
#define UART_RX_BUFF_SIZE 128

#define UART_HWFC APP_UART_FLOW_CONTROL_DISABLED

/**
 * @#brief Function for main application entry.
 */

void uart_err_handle(app_uart_evt_type_t *p)
{
  
}

void myPrintf(char *message)
{
  uint8_t i = 0;
  while(message[i] != NULL)
  {
    app_uart_put(message[i++]);
  }
}

int main(void)
{
  uint32_t err_code;
  
  bsp_board_init(BSP_INIT_LEDS); //Initialize leds

  const app_uart_comm_params_t comms_params = 
  {
    RX_PIN_NUMBER,
    TX_PIN_NUMBER,
    RTS_PIN_NUMBER,
    CTS_PIN_NUMBER,
    UART_HWFC,
    false,
    NRF_UART_BAUDRATE_115200
  };

  APP_UART_FIFO_INIT(&comms_params, UART_RX_BUFF_SIZE, UART_TX_BUFF_SIZE, uart_err_handle, APP_IRQ_PRIORITY_LOWEST, err_code);
  APP_ERROR_CHECK(err_code);


  myPrintf("Hello from nrf52840 using UART!\n\r");

  while(true)
  {
    uint8_t character;

    while(app_uart_get(&character) != NRF_SUCCESS); //Wait until it receives a character from pc

    if(character == 't')
    {
      bsp_board_leds_on();
      myPrintf("All leds turned on!\n\r");
    }

    if(character == 'k')
    {
      bsp_board_leds_off();
      myPrintf("All leds turned off!\n\r");
    }

  }

}


/** @} */
