#include "timers.h"
#include "app_timer.h"
#include "config.h"
#include "data.h"
#include "bleModule.h"

APP_TIMER_DEF(m_1Mbps_led_slow_blink_timer_id);                /**< Timer used to toggle LED for phy selection indication on the dev.kit. */                  
APP_TIMER_DEF(m_8dBm_led_slow_blink_timer_id);                 /**< Timer used to toggle LED for output power selection indication on the dev.kit. */                  
APP_TIMER_DEF(m_time_keeper_timer_id);                         /**< Timer used to make sure app_timer runs continuously. Used by the log module.*/
APP_TIMER_DEF(m_adv_sent_led_show_timer_id);

APP_TIMER_DEF(m_timeout_for_advertise_timer_id);      //For waiting a determined amount of time since first adv received


// ************ HANDLERS DE LOS TIMER ***************
void led_1Mbps_slow_blink_timeout_handler(void * p_context)
{
    bsp_board_led_invert(PHY_SELECTION_LED);    
}                     


void adv_sent_led_show_timeout_handler (void *p_context)
{
    UNUSED_PARAMETER(p_context);
    bsp_board_led_invert(SENDING_ADV_LED);
}

void led_8dBm_slow_blink_timeout_handler(void * p_context)
{
	bsp_board_led_invert(OUTPUT_POWER_SELECTION_LED);
}

void timeout_for_advertise_timeout_handler(void * p_context)
{
    UNUSED_PARAMETER(p_context);
    //TODO: Stop the scan and start the advertising stage
    scan_stop();
    advReceived = false; //Reset this flag, for starting again when i change to scan stage
    advertising_stop(); 
    advertising_init();
    advertising_start();
}

void time_keeper_timeout_handler(void * p_context)
{
    // Dummy handler
}


/**@brief Function for initializing timers. */
void timers_init(void)
{
    ret_code_t err_code = app_timer_init();
    APP_ERROR_CHECK(err_code);

    err_code = app_timer_create(&m_1Mbps_led_slow_blink_timer_id, APP_TIMER_MODE_REPEATED, led_1Mbps_slow_blink_timeout_handler);
    APP_ERROR_CHECK(err_code);

    err_code = app_timer_create(&m_adv_sent_led_show_timer_id, APP_TIMER_MODE_REPEATED, adv_sent_led_show_timeout_handler);
    APP_ERROR_CHECK(err_code);
  
    err_code = app_timer_create(&m_8dBm_led_slow_blink_timer_id, APP_TIMER_MODE_REPEATED, led_8dBm_slow_blink_timeout_handler);
    APP_ERROR_CHECK(err_code);

    err_code = app_timer_create(&m_timeout_for_advertise_timer_id, APP_TIMER_MODE_SINGLE_SHOT, timeout_for_advertise_timeout_handler);
    APP_ERROR_CHECK(err_code);

    // Creating the timer used by the log module. 
    err_code = app_timer_create(&m_time_keeper_timer_id, APP_TIMER_MODE_REPEATED, time_keeper_timeout_handler);
    APP_ERROR_CHECK(err_code);
}


void m_1Mbps_led_slow_blink(uint32_t timer_ticks, bool start, void * p_context) {
    uint32_t err_code;
    if (start) {
      err_code = app_timer_start(m_1Mbps_led_slow_blink_timer_id, timer_ticks, p_context);
    } else {
      err_code = app_timer_stop(m_1Mbps_led_slow_blink_timer_id);
    }
    
    APP_ERROR_CHECK(err_code);
}

void m_adv_sent_led_show(uint32_t timer_ticks, bool start, void * p_context) {
    uint32_t err_code;
    if (start) {
      err_code = app_timer_start(m_adv_sent_led_show_timer_id, timer_ticks, p_context);
    } else {
      err_code = app_timer_stop(m_adv_sent_led_show_timer_id);
    }
    
    APP_ERROR_CHECK(err_code);
}

void m_8dBm_led_slow_blink(uint32_t timer_ticks, bool start, void * p_context) {
    uint32_t err_code;
    if (start) {
      err_code = app_timer_start(m_8dBm_led_slow_blink_timer_id, timer_ticks, p_context);
    } else {
      err_code = app_timer_stop(m_8dBm_led_slow_blink_timer_id);
    }
    
    APP_ERROR_CHECK(err_code);
}

void m_timeout_for_advertise(uint32_t timer_ticks, bool start, void * p_context) {
    uint32_t err_code;
    if (start) {
      err_code = app_timer_start(m_timeout_for_advertise_timer_id, timer_ticks, p_context);
    } else {
      err_code = app_timer_stop(m_timeout_for_advertise_timer_id);
    }
    
    APP_ERROR_CHECK(err_code);
}

void m_time_keeper(uint32_t timer_ticks, bool start, void * p_context) {
    uint32_t err_code;
    if (start) {
      err_code = app_timer_start(m_time_keeper_timer_id, timer_ticks, p_context);
    } else {
      err_code = app_timer_stop(m_time_keeper_timer_id);
    }
    
    APP_ERROR_CHECK(err_code);
}

