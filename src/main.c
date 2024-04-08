#include <stdbool.h>
#include <stm8s.h>
//#include <stdio.h>
#include "main.h"
#include "milis.h"
//#include "delay.h"
#include "daughterboard.h"
#include "uart1.h"

void init(void) {
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1); // taktovani MCU na 16MHz

    GPIO_Init(PWM_R_PORT, PWM_R_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(PWM_G_PORT, PWM_R_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(PWM_B_PORT, PWM_R_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);

    GPIO_Init(S1_PORT, S1_PIN, GPIO_MODE_IN_PU_NO_IT);
    GPIO_Init(S2_PORT, S2_PIN, GPIO_MODE_IN_PU_NO_IT);
    GPIO_Init(S3_PORT, S3_PIN, GPIO_MODE_IN_PU_NO_IT);

    GPIO_Init(LED7_PORT, LED7_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(LED5_PORT, LED5_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(LED8_PORT, LED8_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);

    init_milis();
    init_uart1();

    TIM2_DeInit();
    TIM2_TimeBaseInit(TIM2_PRESCALER_16, 100);
    TIM2_OC1Init(                // konfigurace output channel1
        TIM2_OCMODE_PWM1,        // mod PWM1
        TIM2_OUTPUTSTATE_ENABLE, // povolím
        50,                      // nastavuju šířku impulzu
        TIM2_OCPOLARITY_HIGH);   // nastavení polarity
    TIM2_OC2Init(TIM2_OCMODE_PWM1, TIM2_OUTPUTSTATE_ENABLE, 10,
                 TIM2_OCPOLARITY_HIGH);
    TIM2_OC3Init(TIM2_OCMODE_PWM1, TIM2_OUTPUTSTATE_ENABLE, 90,
                 TIM2_OCPOLARITY_HIGH);

    TIM2_OC1PreloadConfig(ENABLE);
    TIM2_OC2PreloadConfig(ENABLE);
    TIM2_OC3PreloadConfig(ENABLE);
    TIM2_Cmd(ENABLE);
}

int main(void) {

    uint32_t time = 0;
    int8_t r = 1, g = 1, b = 1;
    int8_t *color_pointer = &r;
    bool S1memory = false;
    bool S2memory = false;
    bool S3memory = false;
    uint8_t color = 0;

    init();

    while (1) {
        if (milis() - time > 33) {
            time = milis();

            if (PUSH(S1) && !S1memory) {
                color++;
                if (color > 3) {
                    color = 1;
                }
                LOW(LED7);
                LOW(LED5);
                LOW(LED8);
                // if (color == 1) HIGH(LED1);
                // if (color == 2) HIGH(LED4);
                // if (color == 3) HIGH(LED2);
                //  místo if if if je možné použít switch
                switch (color) {
                case 1:
                    HIGH(LED7);
                    color_pointer = &r;
                    break;
                case 2:
                    HIGH(LED5);
                    color_pointer = &g;
                    break;
                case 3:
                    HIGH(LED8);
                    color_pointer = &b;
                    break;
                default:
                    break;
                }
            }

            S1memory = PUSH(S1);

            // zvysuje jas
            if (!PUSH(S3) && S3memory) {
                *color_pointer += 10;   // kdyz dam * pracuji s hodnotou na kterou ukazuje (v tomto pripade r, g a b)
                if (*color_pointer > 100)  
                    *color_pointer = 0; 
            }

            S3memory = PUSH(S3);

            // snizuje jas
            if (!PUSH(S2) && S2memory) {
                *color_pointer -= 10;
            if (*color_pointer < 0)
                *color_pointer = 100;
            }

            S2memory = PUSH(S2);

            TIM2_SetCompare1(b);
            TIM2_SetCompare2(g);
            TIM2_SetCompare3(r);
        }
    }
}
/*-------------------------------  Assert -----------------------------------*/
#include "__assert__.h"
