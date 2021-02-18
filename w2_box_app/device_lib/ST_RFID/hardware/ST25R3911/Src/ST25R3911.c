#include "ST25R3911.h"
#include "delay.h"


ST25R3911_StatusTypeDef  ST25R3911Status;
ST25R3911_ControlTypeDef ST25R3911_Control;

unsigned char AS3911_IRQ_FLAG = 0;

unsigned char ST25R3911FIFO[ST25R3911_FIFO_SIZE];

int time_node[10];
int rf_time[10];

int ST25R3911_Init(void)
{
	
		
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;

	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;


	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA, ENABLE );
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOC, ENABLE );

	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_SPI1,  ENABLE );
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);




	GPIO_InitStructure.GPIO_Pin = BeepPin; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(BeepPort, &GPIO_InitStructure);



	GPIO_InitStructure.GPIO_Pin = ST25R3911_SPI_CSPin; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(ST25R3911_SPI_CSPort, &GPIO_InitStructure);


	GPIO_InitStructure.GPIO_Pin  = ST25R3911_IRQPin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(ST25R3911_IRQPort, &GPIO_InitStructure);


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PB13/14/15复用推挽输出 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOB

	GPIO_SetBits(GPIOA, GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7); 



	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;	
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;	
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	
	SPI_InitStructure.SPI_CRCPolynomial = 7;	
	SPI_Init(SPI1, &SPI_InitStructure);  

	SPI_Cmd(SPI1, ENABLE); 


	delay_ms(50);


	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;			
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;	
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;							
	NVIC_Init(&NVIC_InitStructure); 

	EXTI_InitStructure.EXTI_Line=EXTI_Line3;	
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	 	

	GPIO_EXTILineConfig(ST25R3911_IRQIntPortSource, ST25R3911_IRQIntPinSource);


	//ST25R3911_IRQCmd(DISABLE);


	GPIO_SetBits(ST25R3911_SPI_CSPort, ST25R3911_SPI_CSPin);
	delay_ms(3);



	ST25R3911Status.UnreadFIFOLength = 0;
	ST25R3911_NoBeep();



	return 0;

}

void ST25R3911_IRQCmd(FunctionalState NewState)
{
	EXTI->PR=1<<3;
	
	if(NewState == ENABLE)
	{
		EXTI->IMR|=1<<3;
		
		if( GPIO_ReadInputDataBit(ST25R3911_IRQPort, ST25R3911_IRQPin) == SET && AS3911_IRQ_FLAG == 0)
			EXTI_GenerateSWInterrupt(EXTI_Line3);
	}
	
	
	else
		EXTI->IMR&=~(1<<3);
}

int ST25R3911ReadReg(unsigned char RegAddress, unsigned char *RegValue)
{
	
    RegAddress |= ST25R3911_READ_MODE;

	
	//TO DO : Disable MCU IRQ for ST25R3911
	ST25R3911_IRQCmd(DISABLE);
	//TO DO : Disable MCU IRQ for ST25R3911
	

	GPIO_ResetBits(ST25R3911_SPI_CSPort,ST25R3911_SPI_CSPin);

	


	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI1, RegAddress); 
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	*RegValue = SPI_I2S_ReceiveData(SPI1);

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI1, 0x00); 
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	*RegValue = SPI_I2S_ReceiveData(SPI1);
		


	GPIO_SetBits(ST25R3911_SPI_CSPort,ST25R3911_SPI_CSPin);
	
	

	
	//TO DO : Enable MCU IRQ for ST25R3911
	ST25R3911_IRQCmd(ENABLE);
	//TO DO : Enable MCU IRQ for ST25R3911
	
	
	
	return 0;
	

}


int ST25R3911WriteReg(unsigned char RegAddress, unsigned char RegValue)
{

	
	RegAddress |= ST25R3911_WRITE_MODE;
	
	//TO DO : Disable MCU IRQ for ST25R3911
	ST25R3911_IRQCmd(DISABLE);
	//TO DO : Disable MCU IRQ for ST25R3911
	GPIO_ResetBits(ST25R3911_SPI_CSPort,ST25R3911_SPI_CSPin);

	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI1, RegAddress); 
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI1, RegValue); 
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);

	
	

	
	GPIO_SetBits(ST25R3911_SPI_CSPort,ST25R3911_SPI_CSPin);
	//TO DO : Enable MCU IRQ for ST25R3911
	ST25R3911_IRQCmd(ENABLE);
	//TO DO : Enable MCU IRQ for ST25R3911
	
	return 0;
	

}




int ST25R3911SetRegBits(unsigned char RegAddress, unsigned char RegBits)
{
	
	
	unsigned char reg_data;
	
	ST25R3911ReadReg(RegAddress, &reg_data);
	
	reg_data |= RegBits;
	
	ST25R3911WriteReg(RegAddress, reg_data);
	
	return 0;
	
	
}


int ST25R3911ClearRegBits(unsigned char RegAddress, unsigned char RegBits)
{
	
	unsigned char reg_data;
	
	ST25R3911ReadReg(RegAddress, &reg_data);
	
	reg_data &= ~RegBits;
	
	ST25R3911WriteReg(RegAddress, reg_data);
	
	return 0;
	
}



int ST25R3911ReadMultipleReg(unsigned char RegAddress, unsigned char *RegValue, unsigned char Number)
{
	
	int i;
	
	
	RegAddress |= ST25R3911_READ_MODE;
	
	//TO DO : Disable MCU IRQ for ST25R3911
	
	//TO DO : Disable MCU IRQ for ST25R3911
	
	GPIO_ResetBits(ST25R3911_SPI_CSPort,ST25R3911_SPI_CSPin);
	
	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI1, RegAddress); 
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	*RegValue = SPI_I2S_ReceiveData(SPI1);
	
	
	for(i=0;i<Number;i++)
	{
		
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
		SPI_I2S_SendData(SPI1, 0x00); 
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
		*RegValue = SPI_I2S_ReceiveData(SPI1);
		
		RegValue++;
		
	}
	
	
	GPIO_SetBits(ST25R3911_SPI_CSPort,ST25R3911_SPI_CSPin);
	

	//TO DO : Enable MCU IRQ for ST25R3911
	
	//TO DO : Enable MCU IRQ for ST25R3911
	
	
	
	return 0;
	
}




int ST25R3911WriteMultipleReg(unsigned char RegAddress, unsigned char *RegValue, unsigned char Number)
{
	
	int i;
	
	
	RegAddress |= ST25R3911_WRITE_MODE;

	//TO DO : Disable MCU IRQ for ST25R3911

	//TO DO : Disable MCU IRQ for ST25R3911
	
	GPIO_ResetBits(ST25R3911_SPI_CSPort,ST25R3911_SPI_CSPin);


	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI1, RegAddress); 
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);

	for(i=0;i<Number;i++)
	{
		
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
		SPI_I2S_SendData(SPI1, *RegValue); 
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);

		RegValue++;
		
	}



	GPIO_SetBits(ST25R3911_SPI_CSPort,ST25R3911_SPI_CSPin);
	//TO DO : Enable MCU IRQ for ST25R3911

	//TO DO : Enable MCU IRQ for ST25R3911

	return 0;
	
	
	
}



int ST25R3911DirectCommand(unsigned char Command)
{
	
	Command |= ST25R3911_CMD_MODE;
	
	//TO DO : Disable MCU IRQ for ST25R3911
	ST25R3911_IRQCmd(DISABLE);
	//TO DO : Disable MCU IRQ for ST25R3911
	GPIO_ResetBits(ST25R3911_SPI_CSPort,ST25R3911_SPI_CSPin);
	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI1, Command); 
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
	

	GPIO_SetBits(ST25R3911_SPI_CSPort,ST25R3911_SPI_CSPin);
	//TO DO : Enable MCU IRQ for ST25R3911
	ST25R3911_IRQCmd(ENABLE);
	//TO DO : Enable MCU IRQ for ST25R3911
	
	return 0;

}





int ST25R3911ReadFifo(unsigned char *Buf, unsigned char Length)
{
	
	int i;
	
	if(Length == 0)
		return -1;
	
	
	
	
	//TO DO : Disable MCU IRQ for ST25R3911
	ST25R3911_IRQCmd(DISABLE);
	//TO DO : Disable MCU IRQ for ST25R3911
	
	GPIO_ResetBits(ST25R3911_SPI_CSPort,ST25R3911_SPI_CSPin);
	
	
	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI1, ST25R3911_FIFO_READ); 
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	i = SPI_I2S_ReceiveData(SPI1);
	
	
	for(i=0;i<Length;i++)
	{
		
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
		SPI_I2S_SendData(SPI1, 0x00); 
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
		*Buf = SPI_I2S_ReceiveData(SPI1);
		
		Buf++;
		
	}
	
	
	
	GPIO_SetBits(ST25R3911_SPI_CSPort,ST25R3911_SPI_CSPin);
	
	
	//TO DO : Enable MCU IRQ for ST25R3911
	ST25R3911_IRQCmd(ENABLE);
	//TO DO : Enable MCU IRQ for ST25R3911
	
	return 0;
	

}




int ST25R3911WriteFifo(unsigned char *Buf, unsigned char Length)
{
	
	
		int i;
	
	if(Length == 0)
		return -1;
	
	
	
	
	//TO DO : Disable MCU IRQ for ST25R3911
	ST25R3911_IRQCmd(DISABLE);
	//TO DO : Disable MCU IRQ for ST25R3911
	
	GPIO_ResetBits(ST25R3911_SPI_CSPort,ST25R3911_SPI_CSPin);
	
	
	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI1, ST25R3911_FIFO_LOAD); 
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
	
	
	for(i=0;i<Length;i++)
	{
		
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
		SPI_I2S_SendData(SPI1, *Buf); 
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
		Buf++;
		
	}
	
	
	
	GPIO_SetBits(ST25R3911_SPI_CSPort,ST25R3911_SPI_CSPin);
	
	
	//TO DO : Enable MCU IRQ for ST25R3911
	ST25R3911_IRQCmd(ENABLE);
	//TO DO : Enable MCU IRQ for ST25R3911
	
	return 0;
	
	
	
}



void ST25R3911ReadIRQStatus(unsigned int *pStatusBuf)
{
	
	unsigned char main_reg_data;
	unsigned char timer_reg_data;
	unsigned char err_reg_data;
	
	unsigned int status_temp;

	
	*pStatusBuf = 0;
	
	
	
	//Main Interrupt Register
	ST25R3911ReadReg(ST25R3911_REG_IRQ_MAIN, &main_reg_data);
	
	*pStatusBuf = main_reg_data & 0xFC;
	
	
	
	//Timer and NFC Interrupt Register
	if( (main_reg_data & 0x02) != 0 )
	{
		ST25R3911ReadReg(ST25R3911_REG_IRQ_TIMER_NFC, &timer_reg_data);
		
		status_temp = 0;
		status_temp = timer_reg_data;
		status_temp <<= 8;
		
		*pStatusBuf += status_temp;
			
	}
	
	//Error and Wake-up Interrupt Register
	if( (main_reg_data & 0x01) != 0 )
	{
		ST25R3911ReadReg(ST25R3911_REG_IRQ_ERROR_WUP, &err_reg_data);
		
		status_temp = 0;
		status_temp = err_reg_data;
		status_temp <<= 16;
		
		*pStatusBuf += status_temp;
		
		
	}
	
}



void ST25R3911_Beep(void)
{
		
	GPIO_ResetBits(BeepPort, BeepPin);

}


void ST25R3911_NoBeep(void)
{
	
	GPIO_SetBits(BeepPort, BeepPin);
	
}




void EXTI3_IRQHandler(void)						
{
	
	AS3911_IRQ_FLAG = 1;
	ST25R3911ReadIRQStatus(&(ST25R3911Status.IRQStatus));

	EXTI_ClearITPendingBit(EXTI_Line3);
	AS3911_IRQ_FLAG = 0;
}


ISO14443AAnticollisionState ST25R3911Anticollision(unsigned int SelectData, unsigned char ValidByteNumber, unsigned char ValidBitNumber)
{

	unsigned char this_node_select_data_byte[7];
	unsigned char this_node_valid_select_byte_number;
	unsigned char this_node_valid_select_bit_number;

	
	unsigned char child_node_select_data_byte[7];
	unsigned int child_node_select_data;
	unsigned char child_node_valid_select_byte_number;
	unsigned char child_node_valid_select_bit_number;
	
	unsigned char send_full_byte_number;
	unsigned char send_bit_number;
	unsigned char send_fifo_length;
	
	unsigned char collision_state_valid_byte_number;
	unsigned char collision_state_valid_bit_number;
	
	unsigned char reg_data;
	
	int i;
	
	int retry;
	unsigned char err_reg_data;
	
	if(ST25R3911Status.REQAFlag == 1)
	{
		
//		ST25R3911ClearRegBits(ST25R3911_REG_OP_CONTROL, ST25R3911_REG_OP_CONTROL_rx_en);
//		delay_ms(10);
//		ST25R3911SetRegBits(ST25R3911_REG_OP_CONTROL, ST25R3911_REG_OP_CONTROL_rx_en);
//		delay_ms(10);
			
			
		while(1)
		{

			//Start REQA
			ST25R3911Status.IRQStatus = 0;
			ST25R3911DirectCommand(ST25R3911_CMD_CLEAR_FIFO);
			ST25R3911DirectCommand(ST25R3911_CMD_CLEAR_SQUELCH); //EN NO IRQ
			
			ST25R3911ReadReg(ST25R3911_REG_IRQ_MAIN, &err_reg_data);
			ST25R3911ReadReg(ST25R3911_REG_IRQ_TIMER_NFC, &err_reg_data);
			ST25R3911ReadReg(ST25R3911_REG_IRQ_ERROR_WUP, &err_reg_data);
			
			
			//ST25R3911SetRegBits(ST25R3911_REG_ISO14443A_NFC, ST25R3911_REG_ISO14443A_NFC_antcl);
			ST25R3911DirectCommand(ST25R3911_CMD_TRANSMIT_REQA); //EN&TX_EN NO IRQ	

			
			
			//////////////////////////////////////////////////////////////
			
			retry = 1000;
			
			while(ST25R3911Status.IRQStatus == 0)
			{		
				delay_us(1);
				
				retry--;
				
				if(retry == 0)
					return NOATQA;
			}
				

			//Receive ATQA
			if( (ST25R3911Status.IRQStatus & ST25R3911_IRQ_STATUS_RECEIVE_END) != 0)
			{

				//Read ATQA
				ST25R3911ReadReg(ST25R3911_REG_FIFO_RX_STATUS1, &(ST25R3911Status.UnreadFIFOLength) );
				ST25R3911ReadReg(ST25R3911_REG_FIFO_RX_STATUS2, &(ST25R3911Status.FIFOStatus) );
				
//				if( ST25R3911Status.FIFOStatus != 0)
//					return ISO14443A_ERROR;
				
				ST25R3911ReadFifo(ST25R3911FIFO, ST25R3911Status.UnreadFIFOLength);
				

				
				break;

			}
		}
		
		ST25R3911Status.REQAFlag = 0;
	
	}
	
	time_node[1] = TIM3->CNT;
	
	this_node_select_data_byte[2] = SelectData>>24;
	this_node_select_data_byte[3] = SelectData>>16;
	this_node_select_data_byte[4] = SelectData>>8;
	this_node_select_data_byte[5] = SelectData;
	
	//Check Byte Must Be Initialized
	this_node_select_data_byte[6] = 0;
	
	this_node_valid_select_byte_number = ValidByteNumber;
	this_node_valid_select_bit_number = ValidBitNumber;
	
	send_full_byte_number = this_node_valid_select_byte_number + 2;
	send_bit_number = this_node_valid_select_bit_number;
		
		
	if(send_bit_number == 0)
		send_fifo_length = send_full_byte_number;
	else
		send_fifo_length = send_full_byte_number + 1;
	
	
	//ISO14443A SELECT cmd
	this_node_select_data_byte[0] = 0x93;
	this_node_select_data_byte[1] = (send_full_byte_number<<4) + send_bit_number;
	
	//Clear FIFO&IRQ
	ST25R3911DirectCommand(ST25R3911_CMD_CLEAR_FIFO);
	ST25R3911DirectCommand(ST25R3911_CMD_CLEAR_SQUELCH); //EN NO IRQ
	
	
	
	
	ST25R3911WriteFifo(this_node_select_data_byte, send_fifo_length);

	
	ST25R3911WriteReg(ST25R3911_REG_NUM_TX_BYTES1, (send_full_byte_number>>5) );
	ST25R3911WriteReg(ST25R3911_REG_NUM_TX_BYTES2, ( (send_full_byte_number<<3) | send_bit_number ) );


	ST25R3911Status.IRQStatus = 0;
	ST25R3911DirectCommand(ST25R3911_CMD_TRANSMIT_WITHOUT_CRC);
	
	time_node[2] = TIM3->CNT;
	retry = 1000;
	while(ST25R3911Status.IRQStatus == 0)
	{
		
		delay_us(1);
		
		retry--;
		
		if(retry == 0)
			return SELECT_NO_RESPONSE;

	}
	
	
	time_node[3] = TIM3->CNT;
	
	
	//Collision Occurred	
	if( (ST25R3911Status.IRQStatus & ST25R3911_IRQ_STATUS_BIT_COLLISION) != 0)
	{
		

		
		
		//1) Read Collision Display Register to identify how much is the valid data before the collison occured
		ST25R3911ReadReg(ST25R3911_REG_COLLISION_STATUS, &reg_data);
		
		
		//TO DO :Add Parity bit Check
		
		//TO DO :Add Parity bit Check
		collision_state_valid_byte_number = ((reg_data&0xF0)>>4)-2;
		collision_state_valid_bit_number = (reg_data&0x0E)>>1;
		
		send_full_byte_number = collision_state_valid_byte_number + 2;
		send_bit_number = (collision_state_valid_bit_number + 1)%8;
		
		if(send_bit_number == 0)
		{
			send_full_byte_number++;
			send_fifo_length = send_full_byte_number;
			
		}
		
		else
			send_fifo_length = send_full_byte_number + 1;
		
		
		child_node_valid_select_byte_number = send_full_byte_number-2;
		child_node_valid_select_bit_number = send_bit_number;
		
		//TO DO :Add Valid Check
		
		//TO DO :Add Valid Check
		
		
		// 2) Read FIFO for the response from PICC
		ST25R3911ReadReg(ST25R3911_REG_FIFO_RX_STATUS1, &(ST25R3911Status.UnreadFIFOLength) );
		ST25R3911ReadReg(ST25R3911_REG_FIFO_RX_STATUS2, &(ST25R3911Status.FIFOStatus) );
		
//		if( ST25R3911Status.FIFOStatus != 0)
//			return ISO14443A_ERROR;
		
		ST25R3911ReadFifo(ST25R3911FIFO, ST25R3911Status.UnreadFIFOLength);
		
		
		child_node_select_data_byte[0] = 0x93;
		child_node_select_data_byte[1] = 0x20;
		
		//Copy the SELECT data of current node to child node
//		for(i=0; i<this_node_valid_select_byte_number; i++)
//			child_node_select_data_byte[i+2] = this_node_select_data_byte[i+2];
//			
//		if(this_node_valid_select_bit_number != 0)
//			child_node_select_data_byte[i+2] = this_node_select_data_byte[i+2];
		
		for(i=0; i<4; i++)
			child_node_select_data_byte[i+2] = this_node_select_data_byte[i+2];

		
		
		
		
		//Complete the rest collison state UID received from the PICC
		for(i=this_node_valid_select_byte_number; i<collision_state_valid_byte_number; i++)
			child_node_select_data_byte[i+2] |= ST25R3911FIFO[i-this_node_valid_select_byte_number];
		
		if(collision_state_valid_bit_number != 0)
			child_node_select_data_byte[i+2] |= ST25R3911FIFO[i-this_node_valid_select_byte_number];
		
			
		//TO DO :Add Parity bit Check
		
		//TO DO :Add Parity bit Check
		
		//wait receive full byte end
		//delay_ms(1);
		delay_us(500);
		//Set the collision bit to 1
		child_node_select_data_byte[i+2] |= (1<<collision_state_valid_bit_number);
		
		child_node_select_data = child_node_select_data_byte[2];
		child_node_select_data <<= 8;
		child_node_select_data |= child_node_select_data_byte[3];
		child_node_select_data <<= 8;
		child_node_select_data |= child_node_select_data_byte[4];
		child_node_select_data <<= 8;
		child_node_select_data |= child_node_select_data_byte[5];
		
	
		if(ST25R3911Anticollision(child_node_select_data, child_node_valid_select_byte_number, child_node_valid_select_bit_number) == ISO14443A_ERROR)
			return ISO14443A_ERROR;
		
		

		
		
		
		
		//Set the collision bit to 0
		child_node_select_data_byte[i+2] &= ~(1<<collision_state_valid_bit_number);
		
		child_node_select_data = child_node_select_data_byte[2];
		child_node_select_data <<= 8;
		child_node_select_data |= child_node_select_data_byte[3];
		child_node_select_data <<= 8;
		child_node_select_data |= child_node_select_data_byte[4];
		child_node_select_data <<= 8;
		child_node_select_data |= child_node_select_data_byte[5];
		
	
		if( ST25R3911Anticollision(child_node_select_data, child_node_valid_select_byte_number, child_node_valid_select_bit_number)  == ISO14443A_ERROR)
			return ISO14443A_ERROR;
	
		
	}
	
	
	//No Collision
	else if( (ST25R3911Status.IRQStatus & ST25R3911_IRQ_STATUS_RECEIVE_END) != 0)
	{
		
		//CRC err
		if( (ST25R3911Status.IRQStatus & 0x00F00000) != 0)
			return ISO14443A_ERROR;
		
		ST25R3911ReadReg(ST25R3911_REG_FIFO_RX_STATUS1, &(ST25R3911Status.UnreadFIFOLength) );
		ST25R3911ReadReg(ST25R3911_REG_FIFO_RX_STATUS2, &(ST25R3911Status.FIFOStatus) );
		
		if( ST25R3911Status.FIFOStatus != 0)
			return ISO14443A_ERROR;
						
		ST25R3911ReadFifo(ST25R3911FIFO, ST25R3911Status.UnreadFIFOLength);
		
		

		
		this_node_select_data_byte[0] = 0x93;
		this_node_select_data_byte[1] = 0x70;
		
				
		//Complete the rest UID received from the PICC
		for(i=this_node_valid_select_byte_number;i<7;i++)
			this_node_select_data_byte[i+2] |= ST25R3911FIFO[i-this_node_valid_select_byte_number];
		
		
		
		
		//TO DO :Add Parity bit Check
		
		//TO DO :Add Parity bit Check
		
		
		//The last byte is check byte received from PICC, should be checked if is same as the result calculated from select data, TO DO

		
		
		
		//SELECT to Get Full UID
		ST25R3911ClearRegBits(ST25R3911_REG_ISO14443A_NFC, ST25R3911_REG_ISO14443A_NFC_antcl);
		ST25R3911ClearRegBits(ST25R3911_REG_AUX, ST25R3911_REG_AUX_no_crc_rx);
		

		
		ST25R3911DirectCommand(ST25R3911_CMD_CLEAR_FIFO);
		ST25R3911DirectCommand(ST25R3911_CMD_CLEAR_SQUELCH); //EN NO IRQ

		
		send_fifo_length = 7;
		send_full_byte_number = 7;
		send_bit_number = 0;
		
		ST25R3911WriteFifo(this_node_select_data_byte, send_fifo_length);

		ST25R3911WriteReg(ST25R3911_REG_NUM_TX_BYTES1, (send_full_byte_number>>5) );
		ST25R3911WriteReg(ST25R3911_REG_NUM_TX_BYTES2, ( (send_full_byte_number<<3) | send_bit_number ) );


		ST25R3911Status.IRQStatus = 0;
		ST25R3911DirectCommand(ST25R3911_CMD_TRANSMIT_WITH_CRC);
		
		
		time_node[4] = TIM3->CNT;
		retry = 1000;
		while(ST25R3911Status.IRQStatus == 0)
		{
			
			delay_us(1);
			
			retry--;
			
			if(retry == 0)
			{
				
				ST25R3911SetRegBits(ST25R3911_REG_ISO14443A_NFC, ST25R3911_REG_ISO14443A_NFC_antcl);
				ST25R3911SetRegBits(ST25R3911_REG_AUX, ST25R3911_REG_AUX_no_crc_rx);
				
				return ISO14443A_ERROR;
			}
		}
		
		
		
		time_node[5] = TIM3->CNT;
		ST25R3911SetRegBits(ST25R3911_REG_ISO14443A_NFC, ST25R3911_REG_ISO14443A_NFC_antcl);
		ST25R3911SetRegBits(ST25R3911_REG_AUX, ST25R3911_REG_AUX_no_crc_rx);

			
		if( (ST25R3911Status.IRQStatus & ST25R3911_IRQ_STATUS_RECEIVE_END) != 0)
		{
			
			if( (ST25R3911Status.IRQStatus & 0x00F00000) != 0)
				return ISO14443A_ERROR;
			
			ST25R3911ReadReg(ST25R3911_REG_FIFO_RX_STATUS1, &(ST25R3911Status.UnreadFIFOLength) );
			ST25R3911ReadReg(ST25R3911_REG_FIFO_RX_STATUS2, &(ST25R3911Status.FIFOStatus) );
			
			if( ST25R3911Status.FIFOStatus != 0)
					return ISO14443A_ERROR;
							
			ST25R3911ReadFifo(ST25R3911FIFO, ST25R3911Status.UnreadFIFOLength);
			

			if( (ST25R3911FIFO[0] & 0x04) == 0)
			{		
				

				
				ST25R3911Status.PICCUID[ST25R3911Status.PICCNumber][0] = this_node_select_data_byte[2];
				ST25R3911Status.PICCUID[ST25R3911Status.PICCNumber][1] = this_node_select_data_byte[3];
				ST25R3911Status.PICCUID[ST25R3911Status.PICCNumber][2] = this_node_select_data_byte[4];
				ST25R3911Status.PICCUID[ST25R3911Status.PICCNumber][3] = this_node_select_data_byte[5];
				
				
				ST25R3911Status.PICCNumber++;
				ST25R3911Status.REQAFlag = 1;
				
				//HALT the Selected PICC
				ST25R3911ClearRegBits(ST25R3911_REG_ISO14443A_NFC, ST25R3911_REG_ISO14443A_NFC_antcl);
				ST25R3911ClearRegBits(ST25R3911_REG_AUX, ST25R3911_REG_AUX_no_crc_rx);
				

				
				ST25R3911DirectCommand(ST25R3911_CMD_CLEAR_FIFO);
				ST25R3911DirectCommand(ST25R3911_CMD_CLEAR_SQUELCH); //EN NO IRQ

				
				send_fifo_length = 2;
				send_full_byte_number = 2;
				send_bit_number = 0;
				
				this_node_select_data_byte[0] = 0x55;
				this_node_select_data_byte[1] = 0x00;
				
				ST25R3911WriteFifo(this_node_select_data_byte, send_fifo_length);

				ST25R3911WriteReg(ST25R3911_REG_NUM_TX_BYTES1, (send_full_byte_number>>5) );
				ST25R3911WriteReg(ST25R3911_REG_NUM_TX_BYTES2, ( (send_full_byte_number<<3) | send_bit_number ) );


				ST25R3911Status.IRQStatus = 0;
				ST25R3911DirectCommand(ST25R3911_CMD_TRANSMIT_WITH_CRC);
				
				time_node[6] = TIM3->CNT;
				
				//while(ST25R3911Status.IRQStatus == 0);
				
				retry = 300;
				
				while(ST25R3911Status.IRQStatus == 0)
				{
					
					delay_us(1);
					
					retry--;
					
					if(retry == 0)
					{
						
						ST25R3911SetRegBits(ST25R3911_REG_ISO14443A_NFC, ST25R3911_REG_ISO14443A_NFC_antcl);
						ST25R3911SetRegBits(ST25R3911_REG_AUX, ST25R3911_REG_AUX_no_crc_rx);
						time_node[7] = TIM3->CNT;
						return NOCOLLISION;
					}
				}
		
		
				
				ST25R3911SetRegBits(ST25R3911_REG_ISO14443A_NFC, ST25R3911_REG_ISO14443A_NFC_antcl);
				ST25R3911SetRegBits(ST25R3911_REG_AUX, ST25R3911_REG_AUX_no_crc_rx);
				time_node[7] = TIM3->CNT;
		
				
				
				return ISO14443A_ERROR;
						

			}
			
			else
				return ISO14443A_ERROR;
		}
		
		else
			return ISO14443A_ERROR;
		
	}
	
	
	else 
		return ISO14443A_ERROR;
	
	
	

}


						
						

	
	

