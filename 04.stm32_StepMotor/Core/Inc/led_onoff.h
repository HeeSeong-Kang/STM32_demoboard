#include "main.h"

#define BLINK_UP 1
#define BLINK_DOWN 2
#define ON_UP 3
#define ON_DOWN 4
#define RESET 5

static int mode = 0;
static int blink_dot = 0;
static int on_dot = 0;
static int button1 = 0;
static int button2 = 0;

void demoboard_led2_onoff_hal_delay();
void dempboard_led2_button_onoff();
void dempboard_led2_togle_button_onoff();
void dempboard_led_togle_button1_onoff();
void demoboard_led2_onoff_systick();
void systick_time(uint16_t *ms);

void led_all_off();
void led_all_on();
void led_blink_down();
void led_blink_up();
void led_on_down();
void led_on_up();
void dempboard_togle_button1_onoff();
void button1_led_all_onoff();
void button1_togle_blink_led_all_onoff();

void button_control();
