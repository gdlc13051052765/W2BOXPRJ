#include "bsp_init.h"
#include "main.h"
#include "can_drv.h" 
#include "application.h"


ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

CAN_HandleTypeDef hcan1;

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;


UART_HandleTypeDef huart1;

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */
  /** Common config 
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 2;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel 
  */
  sConfig.Channel = ADC_CHANNEL_9;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel 
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_REGULAR_RANK_2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}
static uint32_t get_filter_by_s1(uint32_t filter, uint8_t s1_addr)
{
  return (( filter & 0xFFE1FFFF ) | ((s1_addr & 0xF) << 17));
}

static uint32_t get_filter_by_s2(uint32_t filter, uint8_t s2_addr)
{
  return (( filter & 0xfffE1fff ) | ((s2_addr & 0xF) << 13));
}

/**
  * @brief CAN Initialization Function
  * @param None
  * @retval None
  */
void MX_CAN_Init(uint8_t s1_addr, uint8_t s2_addr)
{
	uint32_t filter_value = SIGNAL_SLAVE_FILTERID;
	uint32_t filter_mask = SIGNAL_SLAVE_MASK;
	
  /* USER CODE BEGIN CAN_Init 0 */

	CAN_FilterTypeDef CAN_FilterInitStructure;
  /* USER CODE END CAN_Init 0 */

  /* USER CODE BEGIN CAN_Init 1 */
	//500k
  /* USER CODE END CAN_Init 1 */
  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 12;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_3TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_2TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = ENABLE;
  hcan1.Init.AutoWakeUp = ENABLE;
  hcan1.Init.AutoRetransmission = DISABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN_Init 2 */

   /*===========================安卓-->餐格 过滤器fifo0 过滤=============================*/
  //广播包过滤、bit120~bit13  0xf-0x0  bit12:1   0x00FF8000 0x007F8000
  filter_value = (0x00FF8000 >> 3);
  filter_mask =  (0x00FFFC00 >> 3);
	
  CAN_FilterInitStructure.FilterBank  = 0;     /* 过滤器组 */
  CAN_FilterInitStructure.FilterMode    = CAN_FILTERMODE_IDMASK;//列表模式
  CAN_FilterInitStructure.FilterScale   = CAN_FILTERSCALE_32BIT; ///* 32位 */
   
  CAN_FilterInitStructure.FilterIdHigh      = FILTER_H(filter_value);  
  CAN_FilterInitStructure.FilterIdLow       = FILTER_L(filter_value);
  CAN_FilterInitStructure.FilterMaskIdHigh  = FILTER_MASK_H(filter_mask);
  CAN_FilterInitStructure.FilterMaskIdLow   = FILTER_MASK_L(filter_mask);
    
  CAN_FilterInitStructure.FilterFIFOAssignment  = CAN_FILTER_FIFO0;  ///* 能够通过该过滤器的报文存到fifo0中 */
  CAN_FilterInitStructure.FilterActivation = ENABLE;
  HAL_CAN_ConfigFilter(&hcan1,&CAN_FilterInitStructure);

  //广播包过滤、bit120~bit13  0xf-0x0  bit12:1   0x00FF8000 0x007F8000
  filter_value = get_filter_by_s1(0x00FF8000 >> 3, s1_addr);
  filter_mask =  0x00FFFC00 >> 3;

  CAN_FilterInitStructure.FilterBank  = 1;     /* 过滤器组 */
  CAN_FilterInitStructure.FilterMode    = CAN_FILTERMODE_IDMASK;//列表模式
  CAN_FilterInitStructure.FilterScale   = CAN_FILTERSCALE_32BIT; ///* 32位 */
   
  CAN_FilterInitStructure.FilterIdHigh      = FILTER_H(filter_value);  
  CAN_FilterInitStructure.FilterIdLow       = FILTER_L(filter_value);
  CAN_FilterInitStructure.FilterMaskIdHigh  = FILTER_MASK_H(filter_mask);
  CAN_FilterInitStructure.FilterMaskIdLow   = FILTER_MASK_L(filter_mask);
    
  CAN_FilterInitStructure.FilterFIFOAssignment  = CAN_FILTER_FIFO0;  ///* 能够通过该过滤器的报文存到fifo0中 */
  CAN_FilterInitStructure.FilterActivation = ENABLE;
  HAL_CAN_ConfigFilter(&hcan1,&CAN_FilterInitStructure);
	

  //安卓-->餐格数据包过滤   0x00FF8000 0x007F8000
  filter_value = get_filter_by_s1(0x00FF8000 >> 3, s1_addr);
  filter_value = get_filter_by_s2(filter_value, s2_addr);
  filter_mask =  0x00FFFC00 >> 3;

  CAN_FilterInitStructure.FilterBank  = 2;     /* 过滤器组 */
  CAN_FilterInitStructure.FilterMode    = CAN_FILTERMODE_IDMASK;//列表模式
  CAN_FilterInitStructure.FilterScale   = CAN_FILTERSCALE_32BIT; ///* 32位 */
   
  CAN_FilterInitStructure.FilterIdHigh      = FILTER_H(filter_value);  
  CAN_FilterInitStructure.FilterIdLow       = FILTER_L(filter_value);
  CAN_FilterInitStructure.FilterMaskIdHigh  = FILTER_MASK_H(filter_mask);
  CAN_FilterInitStructure.FilterMaskIdLow   = FILTER_MASK_L(filter_mask);
    
  CAN_FilterInitStructure.FilterFIFOAssignment  = CAN_FILTER_FIFO0;  ///* 能够通过该过滤器的报文存到fifo0中 */
  CAN_FilterInitStructure.FilterActivation = ENABLE;
  HAL_CAN_ConfigFilter(&hcan1,&CAN_FilterInitStructure);

  //安卓->box多包接收
  filter_value = get_filter_by_s1(0x00F00000 >> 3, s1_addr);
  filter_value = get_filter_by_s2(filter_value, s2_addr);
  filter_mask = 0x00FF0000 >> 3;//GET_FILTER_BY_S1(0x00FF8000, 0xF);
	filter_mask = 0x00F00000 >> 3;//GET_FILTER_BY_S1(0x00FF8000, 0xF);
	 
  CAN_FilterInitStructure.FilterBank  = 3;     /* 过滤器组 */
  CAN_FilterInitStructure.FilterMode    = CAN_FILTERMODE_IDMASK;//CAN_FilterMode_IdList;//CAN_FilterMode_IdMask;  /* 屏敝模式 */
  CAN_FilterInitStructure.FilterScale   = CAN_FILTERSCALE_32BIT; ///* 32位 */
   
  CAN_FilterInitStructure.FilterIdHigh      = FILTER_H(filter_value);  
  CAN_FilterInitStructure.FilterIdLow       = FILTER_L(filter_value);
  CAN_FilterInitStructure.FilterMaskIdHigh  = FILTER_MASK_H(filter_mask);
  CAN_FilterInitStructure.FilterMaskIdLow   = FILTER_MASK_L(filter_mask);
    
  CAN_FilterInitStructure.FilterFIFOAssignment  = CAN_FILTER_FIFO0;  ///* 能够通过该过滤器的报文存到fifo0中 */
  CAN_FilterInitStructure.FilterActivation = ENABLE;
  HAL_CAN_ConfigFilter(&hcan1,&CAN_FilterInitStructure);

   /*===========================CC-->餐格 过滤器fifo0 过滤=============================*/
  //CC-->餐格广播包过滤   0x00DF8000 0x00D68000
  filter_value = 0x00DF8000 >> 3;
  filter_mask =  0x00FFFC00 >> 3;

  CAN_FilterInitStructure.FilterBank  = 4;     /* 过滤器组 */
  CAN_FilterInitStructure.FilterMode    = CAN_FILTERMODE_IDMASK;//列表模式
  CAN_FilterInitStructure.FilterScale   = CAN_FILTERSCALE_32BIT; ///* 32位 */
   
  CAN_FilterInitStructure.FilterIdHigh      = FILTER_H(filter_value);  
  CAN_FilterInitStructure.FilterIdLow       = FILTER_L(filter_value);
  CAN_FilterInitStructure.FilterMaskIdHigh  = FILTER_MASK_H(filter_mask);
  CAN_FilterInitStructure.FilterMaskIdLow   = FILTER_MASK_L(filter_mask);
    
  CAN_FilterInitStructure.FilterFIFOAssignment  = CAN_FILTER_FIFO0;  ///* 能够通过该过滤器的报文存到fifo0中 */
  CAN_FilterInitStructure.FilterActivation = ENABLE;
  HAL_CAN_ConfigFilter(&hcan1,&CAN_FilterInitStructure);

  //CC-->餐格数据包过滤   0x00D68000
  filter_value = get_filter_by_s2(0x00DF8000 >> 3, s2_addr);
  filter_mask =  0x00FFFC00 >> 3;

  CAN_FilterInitStructure.FilterBank  = 5;     /* 过滤器组 */
  CAN_FilterInitStructure.FilterMode    = CAN_FILTERMODE_IDMASK;//列表模式
  CAN_FilterInitStructure.FilterScale   = CAN_FILTERSCALE_32BIT; ///* 32位 */
   
  CAN_FilterInitStructure.FilterIdHigh      = FILTER_H(filter_value);  
  CAN_FilterInitStructure.FilterIdLow       = FILTER_L(filter_value);
  CAN_FilterInitStructure.FilterMaskIdHigh  = FILTER_MASK_H(filter_mask);
  CAN_FilterInitStructure.FilterMaskIdLow   = FILTER_MASK_L(filter_mask);
    
  CAN_FilterInitStructure.FilterFIFOAssignment  = CAN_FILTER_FIFO0;  ///* 能够通过该过滤器的报文存到fifo0中 */
  CAN_FilterInitStructure.FilterActivation = ENABLE;
  HAL_CAN_ConfigFilter(&hcan1,&CAN_FilterInitStructure);
	
	//安卓广播->box多包接收
//  filter_value = get_filter_by_s1(0x00F00000 >> 3, s1_addr);
//  filter_value = get_filter_by_s2(filter_value, 0x0f);
//  filter_mask = 0x00FF0000 >> 3;//GET_FILTER_BY_S1(0x00FF8000, 0xF);
//	
//	 
//  CAN_FilterInitStructure.FilterBank  = 0;     /* 过滤器组 */
//  CAN_FilterInitStructure.FilterMode    = CAN_FILTERMODE_IDMASK;//CAN_FilterMode_IdList;//CAN_FilterMode_IdMask;  /* 屏敝模式 */
//  CAN_FilterInitStructure.FilterScale   = CAN_FILTERSCALE_32BIT; ///* 32位 */
//   
//  CAN_FilterInitStructure.FilterIdHigh      = FILTER_H(filter_value);  
//  CAN_FilterInitStructure.FilterIdLow       = FILTER_L(filter_value);
//  CAN_FilterInitStructure.FilterMaskIdHigh  = FILTER_MASK_H(filter_mask);
//  CAN_FilterInitStructure.FilterMaskIdLow   = FILTER_MASK_L(filter_mask);
//    
//  CAN_FilterInitStructure.FilterFIFOAssignment  = CAN_FILTER_FIFO0;  ///* 能够通过该过滤器的报文存到fifo0中 */
//  CAN_FilterInitStructure.FilterActivation = ENABLE;
//  HAL_CAN_ConfigFilter(&hcan1,&CAN_FilterInitStructure);

  //CAN1->IER |= CAN_IT_BOF|CAN_IT_FMP0|CAN_IT_ERR;
  __HAL_CAN_ENABLE_IT(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING); //FIFO0消息挂起中断允许.   
  //__HAL_CAN_ENABLE_IT(&hcan1, CAN_IT_RX_FIFO1_MSG_PENDING); //FIFO0消息挂起中断允许.  
    
  HAL_CAN_Start(&hcan1);
  HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
  //HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO1_MSG_PENDING);
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */
//	__HAL_SPI_ENABLE(&hspi1);
  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi2.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */
	__HAL_SPI_ENABLE(&hspi2);
  /* USER CODE END SPI2_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 72-1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 20000;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3); 
  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 65535;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1); 
  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 40 - 1;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = (7200 - 1);
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }

  HAL_TIM_Base_Start_IT(&htim4);

}
/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */
	__HAL_UART_ENABLE(&huart1);
  /* USER CODE END USART1_Init 2 */

}

/** 
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) 
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */ 
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(MAIN_LED_GPIO_Port, MAIN_LED_Pin, GPIO_PIN_RESET);
	
  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, PTC_CTRL_Pin|ST25R_CS_Pin|BEEP_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, MOTOR_POWER_Pin, GPIO_PIN_RESET);
	
	HAL_GPIO_WritePin(GPIOB, OLED_DC_Pin|OLED_RES1_Pin|OELD_RES2_Pin|OLED_CS2_Pin 
												|CS_FLASH_Pin|OLED_CS1_Pin, GPIO_PIN_SET);

 
	  /*Configure GPIO pin : MAIN_LED_Pin */
  GPIO_InitStruct.Pin = MAIN_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(MAIN_LED_GPIO_Port, &GPIO_InitStruct);
	
  /*Configure GPIO pins : PTC_CTRL_Pin ST25R_CS_Pin BEEP_Pin */
  GPIO_InitStruct.Pin = PTC_CTRL_Pin|ST25R_CS_Pin|BEEP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : ST25R_IRQ_Pin */
  GPIO_InitStruct.Pin = ST25R_IRQ_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(ST25R_IRQ_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : OLED_DC_Pin OLED_RES1_Pin OELD_RES2_Pin OLED_CS2_Pin 
                           MOTOR_POWER_Pin CS_FLASH_Pin OLED_CS1_Pin */
  GPIO_InitStruct.Pin = OLED_DC_Pin|OLED_RES1_Pin|OELD_RES2_Pin|OLED_CS2_Pin 
                          |MOTOR_POWER_Pin|CS_FLASH_Pin|OLED_CS1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : HALL_Pin */
  GPIO_InitStruct.Pin = HALL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(HALL_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI3_IRQn, 0xf, 0);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */






/*==================================================================================
* 函 数 名： bsp_init
* 参    数： None
* 功能描述:  板级初始化
* 返 回 值： None
* 备    注： 
* 作    者： xiaozh
* 创建时间： 2019-09-24 111301
==================================================================================*/
void bsp_init(void)
{
	SystemClock_Config();

	MX_GPIO_Init();

	MX_DMA_Init(); 
	MX_ADC1_Init();
  MX_CAN_Init(mApp_Param.cc_can_addr, mApp_Param.can_addr);//重新分配地址时，需要重新初始化过滤器
	MX_SPI1_Init();
	MX_SPI2_Init();
	MX_TIM2_Init();
	MX_TIM3_Init();
  MX_TIM4_Init();
	MX_USART1_UART_Init();
}

/*==================================================================================
* 函 数 名： systerm_init_complete
* 参    数： None
* 功能描述:  系统初始化完成
* 返 回 值： None
* 备    注： 
* 作    者： xiaozh
* 创建时间： 2019-11-08 143228
==================================================================================*/
void systerm_init_complete(void)
{
	
	//使能CAN总线
	__HAL_CAN_ENABLE_IT(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);	//FIFO0消息挂起中断允许.	 
	__HAL_CAN_ENABLE_IT(&hcan1, CAN_IT_RX_FIFO1_MSG_PENDING);	//FIFO0消息挂起中断允许.	 
	HAL_CAN_Start(&hcan1);
	HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
	HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO1_MSG_PENDING);
}
