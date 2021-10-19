#include "main.h"
#include "led_onoff.h"
#include "DHT11.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TRIG_PORT GPIOC
#define TRIG_PIN GPIO_PIN_5

extern volatile int TIM11_10ms_ultrasonic_counter;
extern volatile int TIM11_10ms_buzzer_counter;
extern volatile int TIM11_10ms_LED_counter;

uint32_t distance = 0;
uint32_t distance2 = 0;
extern uint8_t temp, humi;
uint8_t led_flag = 0;

uint8_t ic_cpt_flag = 0;  // rising edge or falling edge를 detect하는 flag

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	static uint8_t is_first_captured = 0;

	if(is_first_captured == 0) // rising edge detect
	{
		__HAL_TIM_SET_COUNTER(htim, 0);
		is_first_captured = 1;
	}
	else if (is_first_captured == 1) // falling edge detect
	{
		is_first_captured = 0;
		distance = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
		ic_cpt_flag = 1;
	}
}

void ultrasonic_processing()
{
	if(TIM11_10ms_ultrasonic_counter >= 50) // timer 1sec reached
	{
		TIM11_10ms_ultrasonic_counter = 0;
		make_trigger();

		if(ic_cpt_flag == 1){
			ic_cpt_flag = 0;
			distance = distance * 0.034 / 2;  // 1us 0.034cm가 이동 /2는 왕복
			//printf("distance: %d\n", distance);
			distance2 = distance;
		}
	}
}

void ultrasonic_tapeline()
{
	uint8_t led_patten[9] = {0x00, 0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f, 0x7f, 0xff};

	ultrasonic_processing();
	DHT11_processing();

	printf("distance2: %d", distance2);
	printf("  [Tmp]%d",(int)temp);
	printf("  [Wet]%d\n",(int)humi);

	if(distance2 <= 8)
	{
		if(temp >= 28)	buzzer_Temp();
		else			HAL_GPIO_WritePin(GPIOD, Buz_Pin, GPIO_PIN_RESET);

		led_all_off();
		HAL_GPIO_WritePin(GPIOB, led_patten[distance2], GPIO_PIN_SET);
	}
	else if(distance2 > 8)
	{
		if(temp >= 28)	buzzer_Temp();
		else			HAL_GPIO_WritePin(GPIOD, Buz_Pin, GPIO_PIN_SET);

		if (TIM11_10ms_LED_counter >= 100)
		{
			TIM11_10ms_LED_counter = 0;
			led_flag = !led_flag;

			if(led_flag)	led_all_off();
			else			led_all_on();
		}
	}
}

void buzzer_Temp()
{
	if (TIM11_10ms_buzzer_counter >= 100)
	{
		TIM11_10ms_buzzer_counter = 0;
		HAL_GPIO_TogglePin(GPIOD, Buz_Pin);
	}
}

void make_trigger(void)
{
	HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_RESET);
	delay_us(2);
	HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_SET);
	delay_us(10);
	HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_RESET);
}
