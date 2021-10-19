#include "stepmotor.h"
#include "button.h"
#include "uart.h"

////////////////////////////////////////////////////////////////
//						Elevator Version					  //
////////////////////////////////////////////////////////////////
void elevator_processing() {
	elevatorButtonState();

	switch (floorState) {
	case FIRST_FLOOR:
		elevator_stepmotor_drive(FIRST_FLOOR);
		break;
	case SECOND_FLOOR:
		elevator_stepmotor_drive(SECOND_FLOOR);
		break;
	case THIRD_FLOOR:
		elevator_stepmotor_drive(THIRD_FLOOR);
		break;
	}
}

void elevatorButtonState() {
	if (getButtonState(GPIOC, BUTTON1_Pin, &prevButton1State)) {
		floorState = FIRST_FLOOR;
		floorflag = START;
	}
	if (getButtonState(GPIOC, BUTTON2_Pin, &prevButton3State)) {
		floorState = SECOND_FLOOR;
		floorflag = START;
	}
	if (getButtonState(GPIOC, BUTTON3_Pin, &prevButton3State)) {
		floorState = THIRD_FLOOR;
		floorflag = START;
	}
}

void elevatorPrvStateSave(uint8_t state) {
	if (floorState == state) {
		prvState = state;
		floorflag = STOP;
	}
}

void elevator_stepmotor_drive(uint8_t state) {
	if (floorflag) {
		stepmotor_half_drive(state);
		delay_us(1126);
	}
}

void stepmotor_half_drive(uint8_t directrion) {

	static int step = 0;

	switch (step) {
	case 0:
		HAL_GPIO_WritePin(GPIOC, STEP_SET_0, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, STEP_RESET_0, GPIO_PIN_RESET);
		break;
	case 1:
		HAL_GPIO_WritePin(GPIOC, STEP_SET_1, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, STEP_RESET_1, GPIO_PIN_RESET);
		break;
	case 2:
		HAL_GPIO_WritePin(GPIOC, STEP_SET_2, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, STEP_RESET_2, GPIO_PIN_RESET);
		break;
	case 3:
		HAL_GPIO_WritePin(GPIOC, STEP_SET_3, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, STEP_RESET_3, GPIO_PIN_RESET);
		break;
	case 4:
		HAL_GPIO_WritePin(GPIOC, STEP_SET_4, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, STEP_RESET_4, GPIO_PIN_RESET);
		break;
	case 5:
		HAL_GPIO_WritePin(GPIOC, STEP_SET_5, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, STEP_RESET_5, GPIO_PIN_RESET);
		break;
	case 6:
		HAL_GPIO_WritePin(GPIOC, STEP_SET_6, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, STEP_RESET_6, GPIO_PIN_RESET);
		break;
	case 7:
		HAL_GPIO_WritePin(GPIOC, STEP_SET_7, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, STEP_RESET_7, GPIO_PIN_RESET);
		break;
	}

	if (floorflag) {
		if (prvState < directrion) {
			step++;
			step %= 8;
		} else {
			step--;
			if (step < 0)
				step = 7;
		}
	}
}

#if 0
////////////////////////////////////////////////////////////////
//						MAIN Version						  //
////////////////////////////////////////////////////////////////
void stepmotor_half_drive(int directrion) {

	static int step = 0;

	switch (step) {
	case 0:
		HAL_GPIO_WritePin(GPIOC, STEP_SET_0, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, STEP_RESET_0, GPIO_PIN_RESET);
		break;
	case 1:
		HAL_GPIO_WritePin(GPIOC, STEP_SET_1, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, STEP_RESET_1, GPIO_PIN_RESET);
		break;
	case 2:
		HAL_GPIO_WritePin(GPIOC, STEP_SET_2, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, STEP_RESET_2, GPIO_PIN_RESET);
		break;
	case 3:
		HAL_GPIO_WritePin(GPIOC, STEP_SET_3, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, STEP_RESET_3, GPIO_PIN_RESET);
		break;
	case 4:
		HAL_GPIO_WritePin(GPIOC, STEP_SET_4, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, STEP_RESET_4, GPIO_PIN_RESET);
		break;
	case 5:
		HAL_GPIO_WritePin(GPIOC, STEP_SET_5, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, STEP_RESET_5, GPIO_PIN_RESET);
		break;
	case 6:
		HAL_GPIO_WritePin(GPIOC, STEP_SET_6, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, STEP_RESET_6, GPIO_PIN_RESET);
		break;
	case 7:
		HAL_GPIO_WritePin(GPIOC, STEP_SET_7, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, STEP_RESET_7, GPIO_PIN_RESET);
		break;
	}

	if (directrion == FORWARD) {
		step++;
		step %= 8;
	} else if (directrion == BACKWARD) {
		step--;
		if (step < 0)
			step = 7;
	}
}

void stepmotor_main() {
	while (1) {
		pc_stepmotor_processing();

		switch (runState) {
		case IDLE:
			ButtonModeState(IDLE);
			break;
		case FORWARD:
			ButtonModeState(FORWARD);
			break;
		case BACKWARD:
			ButtonModeState(BACKWARD);
			break;
		}
	}
}

void ButtonModeState(uint8_t mode) {
	if (mode == IDLE) {
		if (getButtonState(GPIOC, BUTTON1_Pin, &prevButton1State))
			runState = FORWARD;
	} else {
		if (runflag) {
			stepmotor_half_drive(mode);
			delay_us(1126);
		}

		if (getButtonState(GPIOC, BUTTON1_Pin, &prevButton1State))
			runState = (mode == BACKWARD ? FORWARD : BACKWARD);
		else if (getButtonState(GPIOC, BUTTON2_Pin, &prevButton2State))
			runflag = !runflag;
	}
}

void pc_stepmotor_processing() {
	if (command_count)  //command_buffer에 message가 있으면
	{
		rx_cmd_flag = 0;
		command_count--;
		printf("%s\n", command_buffer[read_index]); //&command_buffer[read_index][0]
		printf("%s\n", bt_buffer);
		if (!strncmp((const char*) command_buffer[read_index], "FORWARD",
				strlen("FORWARD") - 1))
			runState = FORWARD;
		if (!strncmp((const char*) command_buffer[read_index], "BACKWARD",
				strlen("BACKWARD") - 1))
			runState = BACKWARD;
		if (!strncmp((const char*) command_buffer[read_index], "STOP",
				strlen("STOP") - 1))
			runflag = 0;
		if (!strncmp((const char*) command_buffer[read_index], "START",
				strlen("START") - 1))
			runflag = 1;

		memset(command_buffer[read_index], 0, COMMAND_LENGTH);
		read_index++;
		read_index %= COMMAND_MAX;
	}
}
#endif
////////////////////////////////////////////////////////////////
//						DEMO Version						  //
////////////////////////////////////////////////////////////////

#if 0 // DEMO Version
void stepmotor_half_drive(int step) {

	switch (step) {
	case 0:
		HAL_GPIO_WritePin(GPIOC, STEP_SET_0, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, STEP_RESET_0, GPIO_PIN_RESET);
		break;
	case 1:
		HAL_GPIO_WritePin(GPIOC, STEP_SET_1, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, STEP_RESET_1, GPIO_PIN_RESET);
		break;
	case 2:
		HAL_GPIO_WritePin(GPIOC, STEP_SET_2, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, STEP_RESET_2, GPIO_PIN_RESET);
		break;
	case 3:
		HAL_GPIO_WritePin(GPIOC, STEP_SET_3, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, STEP_RESET_3, GPIO_PIN_RESET);
		break;
	case 4:
		HAL_GPIO_WritePin(GPIOC, STEP_SET_4, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, STEP_RESET_4, GPIO_PIN_RESET);
		break;
	case 5:
		HAL_GPIO_WritePin(GPIOC, STEP_SET_5, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, STEP_RESET_5, GPIO_PIN_RESET);
		break;
	case 6:
		HAL_GPIO_WritePin(GPIOC, STEP_SET_6, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, STEP_RESET_6, GPIO_PIN_RESET);
		break;
	case 7:
		HAL_GPIO_WritePin(GPIOC, STEP_SET_7, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, STEP_RESET_7, GPIO_PIN_RESET);
		break;
	}
}

// 모터가 1바퀴(360도) 도는데 512개의 sequence가 걸림
void setpmotor_main()
{
	while (1)
	{
		for (int i = 0; i < 512; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				stepmotor_half_drive(j);
				delay_us(1126);
			}
		}
	}
}
#endif
