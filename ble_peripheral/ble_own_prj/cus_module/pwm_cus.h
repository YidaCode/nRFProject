/*!
* @brief 
*/
#ifndef __PWM_CUS_H__
#define __PWM_CUS_H__

/*============= I N C L U D E S ============*/
#include <stdbool.h>
#include <stdint.h>
#include "nrf.h"
#include "app_error.h"
#include "nrf_delay.h"
#include "app_pwm.h"

/*============= D E F I N E S ==============*/
#define PUMP_PIN  NRF_GPIO_PIN_MAP(0,13)

/*============= E X P O R T S ==============*/
void pwm_set_duty_cycle(uint8_t value);
uint32_t pwm_init(void);

#endif