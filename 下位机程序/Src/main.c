/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2021 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_hal.h"

/* USER CODE BEGIN Includes */
#include "stdio.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;

TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
#define true 1
#define false 0
int BPM;                   // used to hold the pulse rate
int Signal;                // holds the incoming raw data
int IBI = 600;             // holds the time between beats, must be seeded! 
unsigned char Pulse = false;     // true when pulse wave is high, false when it's low
unsigned char QS = false;        // becomes true when Arduoino finds a beat.
int rate[10];                    // array to hold last ten IBI values
unsigned long sampleCounter = 0;          // used to determine pulse timing
unsigned long lastBeatTime = 0;           // used to find IBI
int P =512;                      // used to find peak in pulse wave, seeded
int T = 512;                     // used to find trough in pulse wave, seeded
int thresh = 512;                // used to find instant moment of heart beat, seeded
int amp = 100;                   // used to hold amplitude of pulse waveform, seeded
int Num;
unsigned char firstBeat = true;        // used to seed rate array so we startup with reasonable BPM
unsigned char secondBeat = false;      // used to seed rate array so we startup with reasonable BPM
/* USER CODE END PV */
/* Private variables ---------------------------------------------------------*/
int BPM2;                   // used to hold the pulse rate
int Signal2;                // holds the incoming raw data
int IBI2 = 600;             // holds the time between beats, must be seeded! 
unsigned char Pulse2 = false;     // true when pulse wave is high, false when it's low
unsigned char QS2 = false;        // becomes true when Arduoino finds a beat.
int rate2[10];                    // array to hold last ten IBI values
unsigned long sampleCounter2 = 0;          // used to determine pulse timing
unsigned long lastBeatTime2 = 0;           // used to find IBI
int P2 =512;                      // used to find peak in pulse wave, seeded
int T2 = 512;                     // used to find trough in pulse wave, seeded
int thresh2 = 512;                // used to find instant moment of heart beat, seeded
int amp2 = 100;                   // used to hold amplitude of pulse waveform, seeded
int Num2;
unsigned char firstBeat2 = true;        // used to seed rate array so we startup with reasonable BPM
unsigned char secondBeat2 = false;      // used to seed rate array so we startup with reasonable BPM
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_ADC2_Init(void);
static void MX_TIM3_Init(void);
static void MX_USART2_UART_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
void sendDataToProcessing(char symbol, int dat );
#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

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
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_TIM3_Init();
  MX_USART2_UART_Init();

  /* USER CODE BEGIN 2 */
	HAL_TIM_Base_Start_IT(&htim3);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		//sendDataToProcessing('S', Signal);     // send Processing the raw Pulse Sensor data
		printf("P1\r\n");
		printf("%d\r\n",Signal);
		printf("P2\r\n");
		printf("%d\r\n",Signal2);
		
		if (QS == 0x01)
		{
//				sendDataToProcessing('B',BPM);   // send heart rate with a 'B' prefix
//				sendDataToProcessing('Q',IBI);   // send time between beats with a 'Q' prefix
				printf("B1\r\n");
				printf("%d\r\n",BPM);
				printf("Q1\r\n");
				printf("%d\r\n",IBI);
				QS = 0x00;                      // reset the Quantified Self flag for next time
		}
		if (QS2 == 0x01)
		{
				printf("B2\r\n");
				printf("%d\r\n",BPM2);
				printf("Q2\r\n");
				printf("%d\r\n",IBI2);
				QS = 0x00;                      // reset the Quantified Self flag for next time
		}
		HAL_Delay(20);	//delay for 20ms
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* ADC1 init function */
static void MX_ADC1_Init(void)
{

  ADC_ChannelConfTypeDef sConfig;

    /**Common config 
    */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure Regular Channel 
    */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_13CYCLES_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* ADC2 init function */
static void MX_ADC2_Init(void)
{

  ADC_ChannelConfTypeDef sConfig;

    /**Common config 
    */
  hadc2.Instance = ADC2;
  hadc2.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc2.Init.ContinuousConvMode = DISABLE;
  hadc2.Init.DiscontinuousConvMode = DISABLE;
  hadc2.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc2.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure Regular Channel 
    */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* TIM3 init function */
static void MX_TIM3_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 7;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 1999;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* USART2 init function */
static void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(USER_LED_GPIO_Port, USER_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : USER_LED_Pin */
  GPIO_InitStruct.Pin = USER_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(USER_LED_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the EVAL_COM1 and Loop until the end of transmission */
  HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xFFFF); 

  return ch;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	unsigned int runningTotal;
	unsigned int runningTotal2;
	if(htim->Instance==htim3.Instance)
	{
		Signal=HAL_ADC_GetValue(&hadc1)>>2;					 // read the Pulse Senso
		sampleCounter += 2;                         // keep track of the time in mS with this variable
		Num = sampleCounter - lastBeatTime;       // monitor the time since the last beat to avoid noise
		HAL_ADC_Start(&hadc1);									//restart ADC conversion

		//  find the peak and trough of the pulse wave
  if(Signal < thresh && Num > (IBI/5)*3){       // avoid dichrotic noise by waiting 3/5 of last IBI
    if (Signal < T){                        // T is the trough
      T = Signal;                         // keep track of lowest point in pulse wave 
    }
  }

  if(Signal > thresh && Signal > P){          // thresh condition helps avoid noise
    P = Signal;                             // P is the peak
  }                                        // keep track of highest point in pulse wave

  //  NOW IT'S TIME TO LOOK FOR THE HEART BEAT
  // signal surges up in value every time there is a pulse
  if (Num > 250){                                   // avoid high frequency noise
    if ( (Signal > thresh) && (Pulse == false) && (Num > (IBI/5)*3) ){        
      Pulse = true;                               // set the Pulse flag when we think there is a pulse
      HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);                // turn on pin 13 LED
      IBI = sampleCounter - lastBeatTime;         // measure time between beats in mS
      lastBeatTime = sampleCounter;               // keep track of time for next pulse

      if(secondBeat){                        // if this is the second beat, if secondBeat == TRUE
        secondBeat = false;                  // clear secondBeat flag
        for(int i=0; i<=9; i++){             // seed the running total to get a realisitic BPM at startup
          rate[i] = IBI;                      
        }
      }

      if(firstBeat){                         // if it's the first time we found a beat, if firstBeat == TRUE
        firstBeat = false;                   // clear firstBeat flag
        secondBeat = true;                   // set the second beat flag
 //       sei();                               // enable interrupts again
        return;                              // IBI value is unreliable so discard it
      }   


      // keep a running total of the last 10 IBI values
      runningTotal = 0;                  // clear the runningTotal variable    

      for(int i=0; i<=8; i++){                // shift data in the rate array
        rate[i] = rate[i+1];                  // and drop the oldest IBI value 
        runningTotal += rate[i];              // add up the 9 oldest IBI values
      }

      rate[9] = IBI;                          // add the latest IBI to the rate array
      runningTotal += rate[9];                // add the latest IBI to runningTotal
      runningTotal /= 10;                     // average the last 10 IBI values 
      BPM = 60000/runningTotal;               // how many beats can fit into a minute? that's BPM!
      QS = 0x01;                              // set Quantified Self flag 
      // QS FLAG IS NOT CLEARED INSIDE THIS ISR
    }                       
  }

  if (Signal < thresh && Pulse == true){   // when the values are going down, the beat is over
     HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);            // turn off pin 13 LED
    Pulse = false;                         // reset the Pulse flag so we can do it again
    amp = P - T;                           // get amplitude of the pulse wave
    thresh = amp/2 + T;                    // set thresh at 50% of the amplitude
    P = thresh;                            // reset these for next time
    T = thresh;
  }

  if (Num > 2500){                           // if 2.5 seconds go by without a beat
    thresh = 512;                          // set thresh default
    P = 512;                               // set P default
    T = 512;                               // set T default
    lastBeatTime = sampleCounter;          // bring the lastBeatTime up to date        
    firstBeat = true;                      // set these to avoid noise
    secondBeat = false;                    // when we get the heartbeat back
		
  }

	//part2
	
		Signal2=HAL_ADC_GetValue(&hadc2)>>2;					 // read the Pulse Senso
		sampleCounter2 += 2;                         // keep track of the time in mS with this variable
		Num2 = sampleCounter2 - lastBeatTime2;       // monitor the time since the last beat to avoid noise
		HAL_ADC_Start(&hadc2);									//restart ADC conversion

		//  find the peak and trough of the pulse wave
  if(Signal2 < thresh2 && Num2 > (IBI2/5)*3){       // avoid dichrotic noise by waiting 3/5 of last IBI
    if (Signal2 < T2){                        // T is the trough
      T2 = Signal2;                         // keep track of lowest point in pulse wave 
    }
  }

  if(Signal2 > thresh2 && Signal2 > P2){          // thresh condition helps avoid noise
    P2 = Signal2;                             // P is the peak
  }                                        // keep track of highest point in pulse wave

  //  NOW IT'S TIME TO LOOK FOR THE HEART BEAT
  // signal surges up in value every time there is a pulse
  if (Num2 > 250){                                   // avoid high frequency noise
    if ( (Signal2 > thresh2) && (Pulse2 == false) && (Num2 > (IBI2/5)*3) ){        
      Pulse2 = true;                               // set the Pulse flag when we think there is a pulse
      //HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);                // turn on pin 13 LED
      IBI2 = sampleCounter2 - lastBeatTime2;         // measure time between beats in mS
      lastBeatTime2 = sampleCounter2;               // keep track of time for next pulse

      if(secondBeat2){                        // if this is the second beat, if secondBeat == TRUE
        secondBeat2 = false;                  // clear secondBeat flag
        for(int i=0; i<=9; i++){             // seed the running total to get a realisitic BPM at startup
          rate2[i] = IBI2;                      
        }
      }

      if(firstBeat2){                         // if it's the first time we found a beat, if firstBeat == TRUE
        firstBeat2 = false;                   // clear firstBeat flag
        secondBeat2 = true;                   // set the second beat flag
 //       sei();                               // enable interrupts again
        return;                              // IBI value is unreliable so discard it
      }   


      // keep a running total of the last 10 IBI values
      runningTotal2 = 0;                  // clear the runningTotal variable    

      for(int i=0; i<=8; i++){                // shift data in the rate array
        rate2[i] = rate2[i+1];                  // and drop the oldest IBI value 
        runningTotal2 += rate2[i];              // add up the 9 oldest IBI values
      }

      rate2[9] = IBI2;                          // add the latest IBI to the rate array
      runningTotal2 += rate2[9];                // add the latest IBI to runningTotal
      runningTotal2 /= 10;                     // average the last 10 IBI values 
      BPM2 = 60000/runningTotal2;               // how many beats can fit into a minute? that's BPM!
      QS2 = 0x01;                              // set Quantified Self flag 
      // QS FLAG IS NOT CLEARED INSIDE THIS ISR
    }                       
  }

  if (Signal2 < thresh2 && Pulse2 == true){   // when the values are going down, the beat is over
     //HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);            // turn off pin 13 LED
    Pulse2 = false;                         // reset the Pulse flag so we can do it again
    amp2 = P2 - T2;                           // get amplitude of the pulse wave
    thresh2 = amp2/2 + T2;                    // set thresh at 50% of the amplitude
    P2 = thresh2;                            // reset these for next time
    T2 = thresh2;
  }

  if (Num2 > 2500){                           // if 2.5 seconds go by without a beat
    thresh2 = 512;                          // set thresh default
    P2 = 512;                               // set P default
    T2 = 512;                               // set T default
    lastBeatTime2 = sampleCounter2;          // bring the lastBeatTime up to date        
    firstBeat2 = true;                      // set these to avoid noise
    secondBeat2 = false;                    // when we get the heartbeat back
		
  }
	//part2 ends

	}
}

void sendDataToProcessing(char symbol, int dat )
{
    putchar(symbol);                // symbol prefix tells Processing what type of data is coming
		printf("%d\r\n",dat);						// the data to send culminating in a carriage return
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char * file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler_Debug */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
