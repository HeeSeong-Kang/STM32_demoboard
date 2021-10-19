#include "uart.h"

////command rule: led0on\n
////bluetooth
//volatile uint8_t bt_buffer[COMMAND_LENGTH]; // 2. uart로 부터 rx수신된 char를 저장하는 공간 \n 만날때까지 저장
//volatile int bt_write_index=0; // read pointer while
//volatile uint8_t bt_rx_cmd_flag=0;  // 완전한 문장을 만났다(\n)는 indicator 예) led0on\n
//
////PC
//volatile uint8_t command_buffer[COMMAND_MAX][COMMAND_LENGTH];
//// pc로부터 온 명령어를 저장하는 2차원 array(Circular queue)
//volatile int command_index=0;
//volatile int read_index=0;  // while에서 가져 가는 것
//volatile int write_index=0; // UART save
//uint8_t command_count=0; // command_buffer에 명령이 몇개가 들어있는지를 알려 주는 indicator
//
//volatile uint8_t rx_data; // 1. PC로부터 1byte의 INT가 들어오면 저장 하는 변수
//volatile uint8_t rxbt_data; // 1. PC로부터 1byte의 INT가 들어오면 저장 하는 변수
//uint8_t rx_cmd_flag;  // 완전한 문장을 만났다(\n)는 indicator 예) led0on\n
//
//extern UART_HandleTypeDef huart1;
//extern UART_HandleTypeDef huart2;
////UART로 부터 1byte가 수신되면 H/W가 call을 해준다.
////UART1 / UART2번으로 부터 1 byte가 수신(stop bit 수신)하면 ex interrupt가 발생
//
//void bluetooth_command_processing();
//void pc_command_processing();

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart == &huart2) // PC
	{
		//30byte가 넘는 command에 대한 예외 ㅊ리
		if (write_index < COMMAND_LENGTH)
		{

			if (rx_data == '\n' || rx_data == '\r')
			{
				rx_cmd_flag=1;	  // 완전한 command를 만났다는 indicator flag를 set
				write_index=0;    // 다음 command를 저장하기 위해서 변수를 0으로
				command_index++;  // 다음 저장할 command index를 준비
				command_index %= COMMAND_MAX;
				command_count++;  // 현재 들어온 command 갯수
			}
			else
			{
			command_buffer[command_index][write_index++] = rx_data;
			}
			// led1on\n
			// 1. command_buffer[command_index][write_index++] = rx_data;
			// 2. write_index++
		}
		else
		{
			printf("buffer overflow !!!\n");
		}
		HAL_UART_Receive_IT(&huart2, &rx_data, 1); // !!반드시 UART_receive_IT를 콜해줘야 다음 인터럽트

	}
	if (huart == &huart1) // Bluetooth
	{
		if (bt_write_index < COMMAND_LENGTH)
		{
			if(rxbt_data == '\n' || rxbt_data == '\r')
			{
				bt_buffer[bt_write_index++] = 0;
				bt_write_index=0; // 다음 save할 index를 준비
				bt_rx_cmd_flag=1;
			}
			else
			{
				bt_buffer[bt_write_index++] = rxbt_data;
			}
		}
		else
		{
			bt_write_index=0;
			printf("BT buffer overflow !!!\n");
		}
		HAL_UART_Receive_IT(&huart1, &rxbt_data, 1);   //블루투스니까 uart1
	}
}
//uart2
void pc_command_processing()
{
	if (command_count)  // command_buffer message가 있으면
	{
		command_count--;
		rx_cmd_flag=0;
		printf("pc: %s\n", command_buffer[read_index]);   // &command buffer[read_index][0] 와 같은것
		if (!strncmp((const char *)command_buffer[read_index], "led2on", strlen("led2on")-1))
		{
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
		}
		if (!strncmp((const char *)command_buffer[read_index], "led2off", strlen("led2off")-1))
		{
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
		}
		memset(command_buffer[read_index], 0, COMMAND_LENGTH);
		read_index++;
		read_index %= COMMAND_MAX;  // 다음 읽을 pointer
	}
}
//uart1
void bluetooth_command_processing()
{
	if(bt_rx_cmd_flag)  // BT로 부터 완전한 command가 들어 왔으면
	{
		bt_rx_cmd_flag=0;
		printf("%s\n", bt_buffer);
		if (!strncmp((const char *)bt_buffer, "led2on", strlen("led2on")-1))
		{
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
		}
		if (!strncmp((const char *)bt_buffer, "led2off", strlen("led2off")-1))
		{
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
		}
	}
}


void uart_main()
{

}
