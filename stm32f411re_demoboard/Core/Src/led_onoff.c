#include "led_onoff.h"
#include "button.h"

extern volatile int Tms_counter;
void button_control()
{

	if(getButtonState(GPIOC, BUTTON1_Pin, &prevButton1State))
	{

		button1 = !button1;
		if(button1)
		{
			mode = BLINK_UP;
		}
		else
		{
			mode = BLINK_DOWN;
		}
	}
	else if(getButtonState(GPIOC, BUTTON2_Pin, &prevButton2State))
		{
		button2 = !button2;
			if(button2)
			{
				mode = ON_UP;
			}
			else
			{
				mode = ON_DOWN;
			}
		}
	else if(getButtonState(GPIOC, BUTTON3_Pin, &prevButton3State))
		{

			mode = RESET;
		}
	switch(mode)
			{
			case BLINK_UP:
			led_blink_up();
			break;

			case BLINK_DOWN:
			led_blink_down();
			break;

			case ON_UP:
			led_on_up();
			break;

			case ON_DOWN:
			led_on_down();
			break;

			case RESET:
			button1=0;
			button2=0;
			blink_dot = 0;
			on_dot = 0;
			led_all_off();
			break;
			default:
			break;
			}
}


void button1_led_all_onoff()
{
	static int button1 = 0;

	if(getButtonState(BUTTON1_GPIO_Port, BUTTON1_Pin, &prevButton1State))
	{
		button1++;
		button1 %= 2;
		if(button1)
		{
			led_all_on();
			printf("led_all_on()\n");
		}
		else
		{
			led_all_off();
			printf("led_all_off()\n");
		}
	}
}

// 3
void dempboard_led2_togle_button_onoff()
{
	if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET)
	{
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
		HAL_Delay(100);
	}
}


void led_blink_down()
{
	led_all_off();

	if(blink_dot > 7)
		blink_dot = 0;
	else
		blink_dot++;

	HAL_GPIO_WritePin(GPIOB, 0x80 >> blink_dot, GPIO_PIN_SET);

	systick_time(100);
}

void led_blink_up()
{
	led_all_off();

	if(blink_dot > 7)
		blink_dot = 0;
	else
		blink_dot++;

	HAL_GPIO_WritePin(GPIOB, 0x01 << blink_dot, GPIO_PIN_SET);

	systick_time(100);
}

void led_on_down()
{
	if(on_dot > 7)
	{
		on_dot = 0;
		led_all_off();
	}
	else
		on_dot++;
	HAL_GPIO_WritePin(GPIOB, 0x80 >> on_dot, GPIO_PIN_SET);

	systick_time(100);

}

void led_on_up()
{
	if(on_dot > 7)
	{
		on_dot = 0;
		led_all_off();
	}
	else
		on_dot++;

	HAL_GPIO_WritePin(GPIOB, 0x01 << on_dot, GPIO_PIN_SET);

	systick_time(100);
}

void led_all_off()
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);
}

void led_all_on()
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_SET);
}

// 2
void dempboard_led2_button_onoff()
{
	if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1) == GPIO_PIN_RESET)
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
	}
}

// 1
void demoboard_led2_onoff_hal_delay()
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0);
	delay_us(65530);  // 65ms
	//HAL_Delay(1000);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 1);
	delay_us(65530);  // 65ms
	//HAL_Delay(1000);
}
void demoboard_led2_onoff_systick()
{
	if(Tms_counter>=100)
	{
		Tms_counter=0;
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
	}
}
void systick_time(uint16_t *ms)
{
	Tms_counter=0;
	while(Tms_counter<=ms)
	{
	}
}

