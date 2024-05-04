/*!
* @brief 
*/

/*============= I N C L U D E S ============*/
#include "adc_sar_onchip.h"

/*============= D E F I N E S ==============*/

/*============= C O D E ====================*/
static void saadc_callback(nrf_drv_saadc_evt_t const * p_event)
{

}

nrf_saadc_value_t adc_sar_onchip_sample_block(uint8_t channel)
{
  nrf_saadc_value_t val;
  nrf_drv_saadc_sample_convert(channel,&val);
  return val;
}

uint32_t adc_sar_onchip_init(void)
{
    ret_code_t err_code;
    nrf_saadc_channel_config_t channel_config =
        NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN1);

    err_code = nrf_drv_saadc_init(NULL, saadc_callback);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_saadc_channel_init(0, &channel_config);
    APP_ERROR_CHECK(err_code);

    return err_code;
}
