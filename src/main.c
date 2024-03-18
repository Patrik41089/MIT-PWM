#include <stdbool.h>
#include <stm8s.h>
//#include <stdio.h>
#include "main.h"
#include "milis.h"
//#include "delay.h"
#include "uart1.h"
#include "daughterboard.h"

void init(void)
{
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);      // taktovani MCU na 16MHz

    GPIO_Init(PWM_R_PORT, PWM_R_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(PWM_G_PORT, PWM_R_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(PWM_B_PORT, PWM_R_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);

    init_milis();
    init_uart1();

    TIM2_DeInit();
    TIM2_TimeBaseInit(TIM2_PRESCALER_16, 10e-3 -1);
    TIM2_OC1Init( //konfigurace output channel1
        TIM2_OCMODE_PWM1, //mod PWM1
         TIM2_OUTPUTSTATE_ENABLE, //povolím
          1000, //nastavuju šířku impulzu
          TIM2_OCPOLARITY_HIGH); //nastavení polarity
    TIM2_OCInit(TIM2_OCMODE_PWM2, TIM2_OUTPUTSTATE_ENABLE, 5000, TIM2_OCPOLARITY_HIGH);
    TIM2_OCInit(TIM2_OCMODE_PWM3, TIM2_OUTPUTSTATE_ENABLE, 9000, TIM2_OCPOLARITY_HIGH);
    TIM2_OC1PrescalerConfig(ENABLE);
    TIM2_OC2PrescalerConfig(ENABLE);
    TIM2_OC3PrescalerConfig(ENABLE);
    TIM2_Cmd(ENABLE);
}


int main(void)
{
  
    uint32_t time = 0;

    init();

    while (1) {
        if (milis() - time > 333 ) { 
            time = milis();
            TIM2_SetCompare1(4000);
        }

    }
}
}
/*-------------------------------  Assert -----------------------------------*/
#include "__assert__.h"
