/*!
* @brief Code for BLE & SAR ADC Sampling
*/

/*============= I N C L U D E S ============*/
// System include 
#include <stdint.h>
#include <string.h>
#include "nordic_common.h"
#include "nrf.h"
#include "app_timer.h"
#include "bsp.h"
#include "ble_hci.h"
#include "ble_advdata.h"
#include "ble_advertising.h"
#include "ble_conn_params.h"
#include "nrf_sdh.h"
#include "nrf_sdh_soc.h"
#include "nrf_sdh_ble.h"
#include "nrf_ble_gatt.h"
#include "nrf_ble_qwr.h"
#include "nrf_drv_timer.h"
#include "ble_nus.h"
#include "app_util_platform.h"
#include "bsp_btn_ble.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_delay.h"
// Custom service include 
#include "ble_serv_cus.h"
#include "adc_sar_onchip.h"
#include "pwm_cus.h"

/*============= D E F I N E S ==============*/
#define SAMPLE_RATE 10

const   nrf_drv_timer_t TIMER_1 = NRF_DRV_TIMER_INSTANCE(1);//Timer0 used by softdevice, Timer2 used by PWM
static  uint8_t l_debug_i = 0;

static  nrf_saadc_value_t l_adc_val;
static  float l_adc_val_f = 0.0;
static  uint16_t l_adc_ble;
static  uint8_t l_adc_ble_h = 0;
static  uint8_t l_adc_ble_l = 0;

/*============= F U N C T I O N S ==============*/

/**@brief   RTC for log timestamp
 */
uint32_t rtc_get_counter(void)
{
  return NRF_RTC1->COUNTER;
}

/**@brief   Handler Function for timer0
 * @detail  Timer, Log, PwrMgmt
 */
void cs_timer1_event_handler(nrf_timer_event_t event_type, void* p_context)
{
    switch (event_type)
    {
        case NRF_TIMER_EVENT_COMPARE0:
            l_debug_i     ++;
            l_adc_val     = adc_sar_onchip_sample_block(0);
            l_adc_val_f  += (float)l_adc_val / (1000.0 / SAMPLE_RATE);
            if(l_debug_i == 1000 / SAMPLE_RATE)
            {
              l_adc_ble = l_adc_val_f * 3.6 / 22.0;
              l_adc_ble_h = l_adc_ble>>8;
              l_adc_ble_l = l_adc_ble;
              ble_nus_data_send_datapkg(l_adc_ble_h,l_adc_ble_l);
              NRF_LOG_INFO("Timer1 Trigger Compare0:%d %d.",l_adc_ble_h,l_adc_ble_l);
              l_adc_val_f = 0;
              l_debug_i   = 0;
            }
            break;
        case NRF_TIMER_EVENT_COMPARE1:
            NRF_LOG_INFO("Timer1 Trigger Compare1:%d.",l_adc_val);
            break;
        default:
            break;
    }
}

static int cs_timer1_init(void)
{
    ret_code_t  err_code;
    uint32_t    time_ms; //Time(in miliseconds) between consecutive compare events.
    uint32_t    time_ticks;

    nrf_drv_timer_config_t timer_cfg = NRF_DRV_TIMER_DEFAULT_CONFIG;
    err_code = nrf_drv_timer_init(&TIMER_1, &timer_cfg, cs_timer1_event_handler);
    APP_ERROR_CHECK(err_code);

    time_ms = 1;time_ticks = nrf_drv_timer_ms_to_ticks(&TIMER_1, time_ms);
    //nrf_drv_timer_compare(&TIMER_1, NRF_TIMER_CC_CHANNEL0, time_ticks, true);
    nrf_drv_timer_extended_compare(&TIMER_1, NRF_TIMER_CC_CHANNEL0, time_ticks, NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, true);

    //time_ms = 1500;time_ticks = nrf_drv_timer_ms_to_ticks(&TIMER_1, time_ms);
    //nrf_drv_timer_extended_compare(&TIMER_1, NRF_TIMER_CC_CHANNEL1, time_ticks, NRF_TIMER_SHORT_COMPARE1_CLEAR_MASK, true);

    nrf_drv_timer_enable(&TIMER_1);
    return err_code;
}

/**@brief   Function for initializing the Common Service Peripherals
 * @detail  APP_Timer, Log, PwrMgmt, Sardac
 */
static void cs_periph_init(void)
{
    ret_code_t err_code;

    err_code = NRF_LOG_INIT(rtc_get_counter);
    APP_ERROR_CHECK(err_code);
    NRF_LOG_DEFAULT_BACKENDS_INIT();

    err_code = app_timer_init();
    APP_ERROR_CHECK(err_code);

    err_code = nrf_pwr_mgmt_init();
    APP_ERROR_CHECK(err_code);

    err_code = cs_timer1_init();
    APP_ERROR_CHECK(err_code);

    err_code = adc_sar_onchip_init();//ADC Configurtion: 10Bit PGA:1/6 VRef 0.6V
    APP_ERROR_CHECK(err_code);

    err_code = pwm_init();
    APP_ERROR_CHECK(err_code);
}

/**@brief Application main function.
 */
int main(void)
{
    // Common Service Initialize.
    cs_periph_init();

    // BLE Initialize
    ble_stack_init();
    ble_gap_params_init();
    ble_gatt_init();
    ble_services_init();
    ble_advertising_init_1();
    ble_conn_params_init_1();
    ble_advertising_start_1();
    
    // Start execution.
    NRF_LOG_INFO("Debug logging for UART over RTT started.");

    // Enter main loop.
    for (;;)
    {
        // If there is no pending log operation, then sleep until next the next event occurs.
        if (NRF_LOG_PROCESS() == false)
        {
            nrf_pwr_mgmt_run();
        }
    }
}
