#include <stdbool.h>
#include <stdint.h>

#include <stdio.h>

#include <stdlib.h> 
#include <string.h>

#include "nordic_common.h"




#include "nrf_pwr_mgmt.h"
#include "nrf_delay.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "nrf_ble_lesc.h"


#include "config.h"
#include "buttons.h"
#include "bleModule.h"
#include "data.h"
#include "timers.h"

 static void instructions_print(void)
{
    NRF_LOG_INFO("Slave %d nrf52840 started! Passive scanning until a sequence of Master adv are received. Then, this slave will response", SLAVE_ID);
}

/**@brief Function for initializing logging. */
static void log_init(void)
{
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
}


/**@brief Function for initializing power management.
 */
static void power_management_init(void)
{
    ret_code_t err_code;
    err_code = nrf_pwr_mgmt_init();
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for handling the idle state (main loop).
 *
 * @details If there is no pending log operation, then sleep until next the next event occurs.
 */
static void idle_state_handle(void)
{
    if (NRF_LOG_PROCESS() == false)
    {
        nrf_pwr_mgmt_run();        
    }
}


/**
 * @brief Function for application main entry.
 */
int main(void)
{
    uint32_t err_code;
    // Initialize.
    log_init();

    timers_init();
    buttons_leds_init();

    power_management_init();
    ble_stack_init();

    instructions_print();

    set_current_scan_params_and_start_scanning();

    // Start the timer for the LOG module
    /*err_code = app_timer_start(m_time_keeper_timer_id, SLOW_BLINK_INTERVAL, NULL);
    APP_ERROR_CHECK(err_code);*/

    // Enter main loop.
    while (true)
    {
      idle_state_handle();
    }
}
