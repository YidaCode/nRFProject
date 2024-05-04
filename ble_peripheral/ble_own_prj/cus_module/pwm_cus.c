/*!
* @brief 
*/

/*============= I N C L U D E S ============*/
#include "pwm_cus.h"
#include "nrf_gpio.h"

/*============= D E F I N E S ==============*/
APP_PWM_INSTANCE(PWM0,2);           // Create the instance "PWM1" using TIMER2.

/*============= D A T A ====================*/

/*============= C O D E ====================*/
static void pwm_ready_callback(uint32_t pwm_id)    // PWM callback function
{
}

/*!
* @brief   
* @param value, range: 0~100 
*/
void pwm_set_duty_cycle(uint8_t value)
{
    while (app_pwm_channel_duty_set(&PWM0, 0, value) == NRF_ERROR_BUSY);
}

uint32_t pwm_init(void)
{
    ret_code_t err_code;

    //app_pwm_config_t pwm1_cfg = APP_PWM_DEFAULT_CONFIG_1CH(500L, PUMP_PIN);
    //pwm1_cfg.pin_polarity[1] = APP_PWM_POLARITY_ACTIVE_HIGH;
    //err_code = app_pwm_init(&PWM0,&pwm1_cfg,pwm_ready_callback);
    //APP_ERROR_CHECK(err_code);
    //app_pwm_enable(&PWM0);

    nrf_gpio_cfg(13,
    NRF_GPIO_PIN_DIR_OUTPUT,
    NRF_GPIO_PIN_INPUT_DISCONNECT,
    NRF_GPIO_PIN_NOPULL,
    NRF_GPIO_PIN_H0H1,    //High driver capacity for >5mA
    NRF_GPIO_PIN_NOSENSE);

    nrf_gpio_pin_write(13,1);
  
    return err_code;
}