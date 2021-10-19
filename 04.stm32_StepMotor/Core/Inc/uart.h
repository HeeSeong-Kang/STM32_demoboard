#include "main.h"
#include "string.h"


#define COMMAND_MAX 50
#define COMMAND_LENGTH 30

//command rule: led0on\n
//bluetooth
volatile uint8_t bt_buffer[COMMAND_LENGTH]; // 2. uart로 부터 rx수신된 char를 저장하는 공간 \n 만날때까지 저장
volatile int bt_write_index; // read pointer while
volatile uint8_t bt_rx_cmd_flag;  // 완전한 문장을 만났다(\n)는 indicator 예) led0on\n

//PC
volatile uint8_t command_buffer[COMMAND_MAX][COMMAND_LENGTH];
// pc로부터 온 명령어를 저장하는 2차원 array(Circular queue)
volatile int command_index;
volatile int read_index;  // while에서 가져 가는 것
volatile int write_index; // UART save
uint8_t command_count; // command_buffer에 명령이 몇개가 들어있는지를 알려 주는 indicator

volatile uint8_t rx_data; // 1. PC로부터 1byte의 INT가 들어오면 저장 하는 변수
volatile uint8_t rxbt_data; // 1. PC로부터 1byte의 INT가 들어오면 저장 하는 변수
uint8_t rx_cmd_flag;  // 완전한 문장을 만났다(\n)는 indicator 예) led0on\n

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
//UART로 부터 1byte가 수신되면 H/W가 call을 해준다.
//UART1 / UART2번으로 부터 1 byte가 수신(stop bit 수신)하면 ex interrupt가 발생

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void bluetooth_command_processing();
void pc_command_processing();

