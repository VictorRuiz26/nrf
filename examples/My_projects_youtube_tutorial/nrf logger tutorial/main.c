
#include <stdbool.h>
#include <stdint.h>

#include "nrf.h"
#include "nordic_common.h"
#include "boards.h"
#include "nrf_delay.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"


/**
 * @brief Function for application main entry.
 */
int main(void)
{

    APP_ERROR_CHECK(NRF_LOG_INIT(NULL));
    NRF_LOG_DEFAULT_BACKENDS_INIT();

    //Dos tipos, en modo defered (no lo manda directamente y lo manda cuando se entra en modo reposo) y modo normal
    //Esto se configura en sdk_confih.h NRF_LOG_DEFERRED

    NRF_LOG_INFO("This is log data from nrf52840 device!\n\r");

    uint32_t count = 0;


    while (true)
    {
      NRF_LOG_INFO("Counter value: %d", count);
      nrf_delay_ms(500);
      count++;
    }
}
/** @} */
