
#include <stdbool.h>
#include "nrf.h"
#include "nrf_drv_gpiote.h"
#include "app_error.h"
#include "boards.h"

#define LED LED_1
#define Btn BUTTON_1

void input_pin_handle(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
  nrf_gpio_pin_toggle(LED); //Toggle LED on interrupt
}

void gpio_init() 
{
  ret_code_t err_code;

  err_code = nrf_drv_gpiote_init();
  APP_ERROR_CHECK(err_code);

  nrf_gpio_cfg_output(LED); //Initialize led
  nrf_gpio_pin_set(LED);    // turn off the led

  nrf_drv_gpiote_in_config_t in_config = GPIOTE_CONFIG_IN_SENSE_HITOLO(true); //Falling edge int. True -> alta velocidad; False -> usa reloj de baja velocidad (no tanta precison)
  in_config.pull = NRF_GPIO_PIN_PULLUP;

  err_code = nrf_drv_gpiote_in_init(Btn, &in_config, input_pin_handle);   //Assign the interrupt config to a pin
  APP_ERROR_CHECK(err_code);

  nrf_drv_gpiote_in_event_enable(Btn, true);  //Enable interrupts for that pin

}

/**
 * @brief Function for application main entry.
 */
int main(void)
{
    gpio_init();

    while (true)
    {
        // Do nothing.
    }
}


/** @} */
