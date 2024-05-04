/*!
* @brief 
*/
#ifndef __ADC_SAR_ONCHIP_H__
#define __ADC_SAR_ONCHIP_H__

/*============= I N C L U D E S ============*/
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "nrf.h"
#include "nrf_drv_saadc.h"
#include "nrf_drv_ppi.h"
#include "nrf_drv_timer.h"
#include "boards.h"
#include "app_error.h"
#include "nrf_delay.h"
#include "app_util_platform.h"
#include "nrf_pwr_mgmt.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

nrf_saadc_value_t adc_sar_onchip_sample_block(uint8_t channel);
uint32_t          adc_sar_onchip_init(void);
#endif