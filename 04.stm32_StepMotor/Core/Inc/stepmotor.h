#include "main.h"

#define SETPPERREV 4096		// moter 1회전 필요한 step수

#define PIN6 GPIO_PIN_6
#define PIN7 GPIO_PIN_7
#define PIN8 GPIO_PIN_8
#define PIN9 GPIO_PIN_9

#define STEP_SET_0 PIN6
#define STEP_SET_1 PIN6 | PIN7
#define STEP_SET_2 PIN7
#define STEP_SET_3 PIN7 | PIN8
#define STEP_SET_4 PIN8
#define STEP_SET_5 PIN8 | PIN9
#define STEP_SET_6 PIN9
#define STEP_SET_7 PIN6 | PIN9

#define STEP_RESET_0 PIN7 | PIN8 | PIN9
#define STEP_RESET_1 PIN8 | PIN9
#define STEP_RESET_2 PIN6 | PIN8 | PIN9
#define STEP_RESET_3 PIN6 | PIN9
#define STEP_RESET_4 PIN6 | PIN7 | PIN9
#define STEP_RESET_5 PIN6 | PIN7
#define STEP_RESET_6 PIN6 | PIN7 | PIN8
#define STEP_RESET_7 PIN7 | PIN8
#define STEP_RESET	 PIN6 | PIN7 | PIN8 | PIN9

#define IDLE 		0
#define FORWARD 	1
#define BACKWARD	2

static uint8_t runState = IDLE;
static uint8_t runflag = 1;

// Elevator State
#define FIRST_FLOOR		11
#define SECOND_FLOOR	22
#define THIRD_FLOOR		33

#define STOP			0
#define START			1

volatile uint8_t floorState;
volatile uint8_t floorflag;
volatile uint8_t prvState;

void elevator_processing();
void elevatorButtonState();
void elevatorPrvStateSave(uint8_t);
void elevator_stepmotor_drive(uint8_t);
void stepmotor_half_drive(uint8_t);
//void stepmotor_main();
//void ButtonModeState(uint8_t mode);
//void pc_stepmotor_processing();


