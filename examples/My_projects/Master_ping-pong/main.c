
#include "ble_advdata.h"
#include "bsp.h"
#include "nordic_common.h"
#include "nrf_delay.h"
#include "nrf_drv_clock.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_sdh.h"
#include "nrf_sdh_ble.h"
#include "nrf_soc.h"
#include <stdbool.h>
#include <stdint.h>

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "app_error.h"
#include "app_uart.h"
#include "boards.h"
#include "nrf_uart.h"

#include "nrf_gpio.h"
#include <time.h>
#include "timers.h"
#include "config.h"
#include "uart.h"
#include "data.h"
#include "bleModule.h"
#include "buttons.h"

  static void instructions_print(void) {
    NRF_LOG_INFO("Master nrf52840 id: %d started!", COORDINATOR_ID);
    #ifdef AUTONOMO
      NRF_LOG_INFO("Autonomous behaviour");
      NRF_LOG_INFO("It will start sending 10 advertisements (with 55 data size, 8dBm tx power and 100ms between advs) with same nseq. Then, wait for response from slave and restart the cicle with next nseq (+1)");
    #else
      NRF_LOG_INFO("Following orders by UART Modbus packet behaviour");
    #endif
  }

  
  /**@brief Function for initializing logging. */
  static void log_init(void) {
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
  }

  /**@brief Function for initializing LEDs. */
  static void buttons_leds_init(void) {
    ret_code_t err_code;

    err_code = bsp_init(BSP_INIT_LEDS | BSP_INIT_BUTTONS, bsp_evt_handler);
    APP_ERROR_CHECK(err_code);
    bsp_board_led_on(PHY_SELECTION_LED);
  }

  /**@brief Function for initializing power management.
   */
  static void power_management_init(void) {
    ret_code_t err_code;
    err_code = nrf_pwr_mgmt_init();
    APP_ERROR_CHECK(err_code);
  }

  /**@brief Function for handling the idle state (main loop).
   *
   * @details If there is no pending log operation, then sleep until next the next event occurs.
   */
  static void idle_state_handle(void) {
    if (NRF_LOG_PROCESS() == false) {
      nrf_pwr_mgmt_run();
    }
  }

  // ***************** FUNCIONES TIMER ******************
  static void lfclk_config(void) {
#ifndef SOFTDEVICE_PRESENT
    ret_code_t err_code = nrf_drv_clock_init();
    APP_ERROR_CHECK(err_code;)

    nrf_drv_clock_lfclk_request(NULL);
#endif
  }

  /**
   * @brief Function for application main entry.
   */
  int main(void) {
    uint32_t err_code;
    // Initialize.
    lfclk_config();
    log_init();
    timers_init();
    buttons_leds_init();
    power_management_init();
    ble_stack_init();
    InicializaBufferUART();

    instructions_print();

    err_code = sd_ble_gap_addr_get(&coordAddr);
    APP_ERROR_CHECK(err_code);

    APP_UART_FIFO_INIT(&comms_params, UART_RX_BUFF_SIZE, UART_TX_BUFF_SIZE, uart_evt_handle, APP_IRQ_PRIORITY_LOWEST, err_code);
    APP_ERROR_CHECK(err_code);

    #ifdef AUTONOMO
      set_current_adv_params_and_start_advertising();
    #else
      analize_buffer_uart(APP_TIMER_TICKS(50), true, NULL);
    #endif
    while (true) {
      idle_state_handle();
    }
  }

  /**
   * @}
   */