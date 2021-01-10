/* USER CODE BEGIN Header */
/*  @Descripttion: @version: 1.0 @Author: hxk @Date: 2020-10-03 14:13:55 @LastEd
 * itors: hxk @LastEditTime: 2020-10-04 12:57:48

 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------ */
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ---------------------------------------------------------- */
/* USER CODE BEGIN Includes */
#include "control.h"
#include "protocol.h"
/* USER CODE END Includes */

/* Private typedef ----------------------------------------------------------- */
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------ */
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro ------------------------------------------------------------- */
/* USER CODE BEGIN PM */
//串口中断接收缓冲区
uint8_t aRx;
uint8_t aRxbuffer[255];
uint8_t aRxCount = 0;
//控制启动Systick中断
int Time_flag = 0;
/* USER CODE END PM */

/* Private variables --------------------------------------------------------- */

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes ----------------------------------------------- */
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code --------------------------------------------------------- */
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void) {
    /* USER CODE BEGIN 1 */

    /* USER CODE END 1 */

    /* MCU Configuration-------------------------------------------------------- */

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_TIM2_Init();
    MX_TIM1_Init();
    MX_TIM3_Init();
    MX_TIM4_Init();
    MX_TIM5_Init();
    MX_USART3_UART_Init();
    /* USER CODE BEGIN 2 */
    // 初始化PID调试助手结构体
    protocol_init();
    //初始化PWM输出
    PWM_Init();
    //初始化编码器
    Encoder_Init();
    //初始化串口中断接收
    HAL_UART_Receive_IT(& huart3, (uint8_t *) & aRx, 1);
    // 向上位机发送启动指令 set_computer_value(SEND_START_CMD, CURVES_CH1, NULL, 0);
    // 启动Systick中断处理
    
		HAL_Delay(100);
		Time_flag = 1;
		Remoter(50);
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1) {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
        printf("l1=%d,l2=%d,l3=%d,l4=%d\r\n", speed_now_LF,speed_now_RF,speed_now_LB,speed_now_RB);
        HAL_Delay(50);
        // receiving_process(); SendData();
    }
    /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {
        0
    };
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {
        0
    };

    /** Initializes the CPU, AHB and APB busses clocks
  */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
    if (HAL_RCC_OscConfig(& RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }
    /** Initializes the CPU, AHB and APB busses clocks
  */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(& RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef * huart) {
    //将接受到的数据发送给协议解析函数
    protocol_data_recv(&aRx, 1);
    HAL_UART_Receive_IT(& huart3, (uint8_t *) & aRx, 1);
}

void HAL_SYSTICK_Callback(void) {
    if (1) {
        count_time++;
        // 50ms时进入
        if (count_time == 50) {
            //读取编码器数据
            ReadEncoder();
            //PID计算输出控制量
            PID_Calculate();
            //驱动电机
            Motor();
            //清零计数值
            count_time = 0;
        }
    }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void) {
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */

    /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t * file, uint32_t line) {
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif/* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
