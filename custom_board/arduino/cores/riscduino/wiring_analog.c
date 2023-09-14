#include "Arduino.h"
#include "wiring_analog.h"
#include "platform.h"

__BEGIN_DECLS

static int _readResolution = 10;
static int _writeResolution = 8;

static uint8_t pwm_enabled[VARIANT_NUM_PWM];
static int8_t pwm_enabled_pin[VARIANT_NUM_PIN];

void analogReadResolution(int res) {
	_readResolution = res;
}

void analogWriteResolution(int res) {
	_writeResolution = res;
}

void analogWritePhase(uint32_t pin, uint32_t phase)
{
  int8_t pwm_num;
  
  if(pin >= variant_pin_map_size)
    return;

  pwm_num = variant_pin_map[pin].pwm_num;
  if(pwm_num > variant_pwm_size)
    {
      //TODO -- Not sure what this function is supposed to
      //        do exactly
    }
}
      
void analogOutputInit( void )
{
}


void analogWrite(uint32_t pin, uint32_t ulValue)
{

  volatile uint8_t pwm_num;
  volatile uint8_t pwm_cmp_num;
  uint32_t pwm_period;
  
  if (pin > variant_pin_map_size) {
    return;
  }

  pwm_num = variant_pin_map[pin].pwm_num;
  pwm_cmp_num = variant_pin_map[pin].pwm_cmp_num;

  if (pwm_num > variant_pwm_size) {
    return;
  }

  pwm_period = (1 << _writeResolution) - 1;

  
 #if (RISCDUINO_SOC >= 122023) // for SOC from MPW-7 onwards
  // This also sets the scale to 0.
  if (!pwm_enabled[pwm_num]) {
    *((volatile uint32_t*) (variant_pwm[pwm_num] + PWM_CFG))   = 0;
    *((volatile uint32_t*) (variant_pwm[pwm_num] + PWM_COUNT)) = 0;
    *((volatile uint32_t*) (variant_pwm[pwm_num] + PWM_CMP0))  = pwm_period;
    *((volatile uint32_t*) (variant_pwm[pwm_num] + PWM_CFG))   = (PWM_CFG_ZEROCMP | PWM_CFG_ENALWAYS);
    pwm_enabled[pwm_num] = 1;
  }
  
  if (!pwm_enabled_pin[pin]) {
    GPIO_REG(GPIO_PULLUP_EN)  &= ~digitalPinToBitMask(pin);
    GPIO_REG(GPIO_OUTPUT_XOR)    &= ~digitalPinToBitMask(pin);
    GPIO_REG(GPIO_IOF_SEL)    |= digitalPinToBitMask(pin);
    GPIO_REG(GPIO_IOF_EN)     |= digitalPinToBitMask(pin);
    pwm_enabled_pin[pin] = 1;
  }

  *((volatile uint32_t*) (variant_pwm[pwm_num] + PWM_CMP0 + pwm_cmp_num*4)) =
    (ulValue > pwm_period) ? 0 : (pwm_period -  ulValue);

 #else 

  // This also sets the scale to 0.
  if (!pwm_enabled[pwm_num]) {
    // Since PWM register does not reset immeditaly with change, we are reducing the
    // Timing tick from 1us to 200ns to run the PWM logic run 5 time faster - this timer
    // value need to corrected for MPW-7 onward - Atten - Dinesh A
    *((volatile uint32_t*)(TIMER_BASE_ADDR+TIMER_GLBL_CFG)) = 0x1;
    *((volatile uint32_t*)(GLBL_BASE_ADDR+GLBL_MULTI_FUNC))  |= 1 << pwm_num;

    // Configure PWM Port for one Time
    *((volatile uint32_t*)(GLBL_BASE_ADDR+GLBL_MULTI_FUNC))  |= 1 << pwm_num;
    pwm_enabled[pwm_num] = 1;
  }
  
  if (!pwm_enabled_pin[pin]) {
    pwm_enabled_pin[pin] = 1;
  }

    *((volatile uint32_t*) (pwm_num*4 + PWM_BASE_ADDR +PWM_CFG_HIGH_BASE))   = ulValue & 0xFF;
    *((volatile uint32_t*) (pwm_num*4 + PWM_BASE_ADDR +PWM_CFG_LOW_BASE))   = (255-ulValue-1) & 0xFF;


 #endif

}

// FE300 Does not have analog inputs
uint32_t analogRead(uint32_t pin)
{
  return 0;
}

__END_DECLS
