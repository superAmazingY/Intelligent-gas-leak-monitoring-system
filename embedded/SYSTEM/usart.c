/**
	************************************************************
	************************************************************
	************************************************************
	*	�ļ����� 	usart.c
	*
	*	���ߣ� 		�ż���
	*
	*	���ڣ� 		2016-11-23
	*
	*	�汾�� 		V1.0
	*
	*	˵���� 		��Ƭ�����������ʼ������ʽ����ӡ
	*
	*	�޸ļ�¼��	
	************************************************************
	************************************************************
	************************************************************
**/

//Ӳ������
#include "usart.h"
#include "delay.h"
#include "timer.h"

//C��
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

//Ӳ������
#include "adc.h"
#include "JW.h"
unsigned char Serial_Head = 0;
unsigned char Serial_Tead = 0;
unsigned char Serial_Data_Buff[SERIAL_DATA_LEN];

static int Send_buf[10] = {0} ;
/*
************************************************************
*	�������ƣ�	Usart1_Init
*
*	�������ܣ�	����1��ʼ��
*
*	��ڲ�����	baud���趨�Ĳ�����
*
*	���ز�����	��
*
*	˵����		TX-PA9		RX-PA10
************************************************************
*/

void Usart3_Init(unsigned int baud)
{
  GPIO_InitTypeDef gpio_initstruct;
	USART_InitTypeDef usart_initstruct;
	NVIC_InitTypeDef nvic_initstruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	
	//PB10	TXD
	gpio_initstruct.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio_initstruct.GPIO_Pin = GPIO_Pin_10;
	gpio_initstruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpio_initstruct);
	
	//PB11	RXD
	gpio_initstruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	gpio_initstruct.GPIO_Pin = GPIO_Pin_11;
	gpio_initstruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpio_initstruct);
	
	usart_initstruct.USART_BaudRate = baud;
	usart_initstruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//��Ӳ������
	usart_initstruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;						//���պͷ���
	usart_initstruct.USART_Parity = USART_Parity_No;									//��У��
	usart_initstruct.USART_StopBits = USART_StopBits_1;								//1λֹͣλ
	usart_initstruct.USART_WordLength = USART_WordLength_8b;							//8λ����λ
	USART_Init(USART3, &usart_initstruct);
	
	USART_Cmd(USART3, ENABLE);														//ʹ�ܴ���
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);									//ʹ�ܽ����ж�
	
	nvic_initstruct.NVIC_IRQChannel = USART3_IRQn;
	nvic_initstruct.NVIC_IRQChannelCmd = ENABLE;
	nvic_initstruct.NVIC_IRQChannelPreemptionPriority = 1;
	nvic_initstruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&nvic_initstruct);
  
}



void Usart1_Init(unsigned int baud)
{
  
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    //�ٴ���ʱ��ʹ�ܣ�GPIO ʱ��ʹ�ܣ�����ʱ��ʹ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE); //ʹ�� USART1,GPIOA ʱ��
    //�ڴ��ڸ�λ
    USART_DeInit(USART1); //��λ���� 1
    //��GPIO �˿�ģʽ����
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //ISART1_TX PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ�� GPIOA.9

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //USART1_RX PA.10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
    GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ�� GPIOA.10
    //�ܴ��ڲ�����ʼ��
    USART_InitStructure.USART_BaudRate = 9600; //����������
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; //�ֳ�Ϊ 8 λ
    USART_InitStructure.USART_StopBits = USART_StopBits_1; //һ��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No; //����żУ��λ
    USART_InitStructure.USART_HardwareFlowControl= USART_HardwareFlowControl_None; //��Ӳ������������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//�շ�ģʽ

    USART_Init(USART1, &USART_InitStructure); //��ʼ������
    //�ݿ����ж�
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //�����ж�
    //��ʹ�ܴ���
    USART_Cmd(USART1, ENABLE); //ʹ�ܴ���
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ; //��ռ���ȼ� 3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //�����ȼ� 3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ ͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure); //�ж����ȼ���ʼ��

}

/*
************************************************************
*	�������ƣ�	Usart2_Init
*
*	�������ܣ�	����2��ʼ��
*
*	��ڲ�����	baud���趨�Ĳ�����
*
*	���ز�����	��
*
*	˵����		TX-PA2		RX-PA3
************************************************************
*/
void Usart2_Init(unsigned int baud)
{

	GPIO_InitTypeDef gpio_initstruct;
	USART_InitTypeDef usart_initstruct;
	NVIC_InitTypeDef nvic_initstruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
	//PA2	TXD
	gpio_initstruct.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio_initstruct.GPIO_Pin = GPIO_Pin_2;
	gpio_initstruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio_initstruct);
	
	//PA3	RXD
	gpio_initstruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	gpio_initstruct.GPIO_Pin = GPIO_Pin_3;
	gpio_initstruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio_initstruct);
	
	usart_initstruct.USART_BaudRate = baud;
	usart_initstruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//��Ӳ������
	usart_initstruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;						//���պͷ���
	usart_initstruct.USART_Parity = USART_Parity_No;									//��У��
	usart_initstruct.USART_StopBits = USART_StopBits_1;								//1λֹͣλ
	usart_initstruct.USART_WordLength = USART_WordLength_8b;							//8λ����λ
	USART_Init(USART2, &usart_initstruct);
	
	USART_Cmd(USART2, ENABLE);														//ʹ�ܴ���
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);									//ʹ�ܽ����ж�
	
	nvic_initstruct.NVIC_IRQChannel = USART2_IRQn;
	nvic_initstruct.NVIC_IRQChannelCmd = ENABLE;
	nvic_initstruct.NVIC_IRQChannelPreemptionPriority = 0;
	nvic_initstruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&nvic_initstruct);

}

/*
************************************************************
*	�������ƣ�	Usart_SendString
*
*	�������ܣ�	�������ݷ���
*
*	��ڲ�����	USARTx��������
*				str��Ҫ���͵�����
*				len�����ݳ���
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void Usart_SendString(USART_TypeDef *USARTx, unsigned char *str, unsigned short len)
{

	unsigned short count = 0;
	
	for(; count < len; count++)
	{
		USART_SendData(USARTx, *str++);									//��������
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);		//�ȴ��������
	}

}
void Usart_SendHexData(USART_TypeDef *USARTx, unsigned char *data, unsigned short len)
{
    unsigned short count = 0;
    
    for (; count < len; count++)
    {
        USART_SendData(USARTx, data[count]); // ��������
        while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET); // �ȴ��������
    }
}

/*
************************************************************
*	�������ƣ�	UsartPrintf
*
*	�������ܣ�	��ʽ����ӡ
*
*	��ڲ�����	USARTx��������
*				fmt����������
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void UsartPrintf(USART_TypeDef *USARTx, char *fmt,...)
{

	unsigned char UsartPrintfBuf[296];
	va_list ap;
	unsigned char *pStr = UsartPrintfBuf;
	
	va_start(ap, fmt);
	vsnprintf((char *)UsartPrintfBuf, sizeof(UsartPrintfBuf), fmt, ap);							//��ʽ��
	va_end(ap);
	
	while(*pStr != 0)
	{
		USART_SendData(USARTx, *pStr++);
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
	}

}

/*
************************************************************
*	�������ƣ�	USART1_IRQHandler
*
*	�������ܣ�	����1�շ��ж�
*
*	��ڲ�����	��
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/

void USART1_IRQHandler(void)
{
   
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //�����ж�
    {
       
        Serial_Data_Buff[Serial_Head] = USART_ReceiveData(USART1);
        
        Serial_Head = (Serial_Head +1 ) % SERIAL_DATA_LEN;
        USART_ClearFlag(USART1, USART_FLAG_RXNE);
    }
}


u8 USART3_RX_STA = 0;     //����3����״̬���
u8 USART3_RX_CMD;

void USART3_IRQHandler(void)                           	 
{
  
  
    u8 Res = 0;

    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  
    {
        Res = USART_ReceiveData(USART3);	               
        USART3_RX_CMD = Res;                               
        USART3_RX_STA = 1;	
//       UsartPrintf(USART_DEBUG,"��Ϣ1��%d\r\n",USART3_RX_CMD);
//          	UsartPrintf(USART_DEBUG,"��׼��%d\r\n",USART3_RX_STA);

    }
}

void USART3_SendByte(uint8_t  Data)                                 //���ڷ���һ���ֽڣ��ֽ� (byte)    1byte=8bit
{
    while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);    //USART_FLAG_TXE���ͼĴ�����

    USART_SendData(USART3, Data);                                   //�Ӵ���2���ʹ��������

    while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);	    //USART_FLAG_TC������ɱ�־
}

/*
 * ��������������3����(SU-03T)��������
 * �����߼����������ܹ�Ԫƽ̨�����ݽ���Э�飬�������ݹ�ȥ
*/
void Uart3_SU03T_SendCmd(int len)
{
  int i = 0 ;

    USART3_SendByte(0xAA);       //��ʼ
    USART3_SendByte(0x55);

    for(i = 0; i < len; i++)     //����
    {
        USART3_SendByte(Send_buf[i]);   //len Ϊ8 �����ν�Send_buf[0]��Send_buf[1]~Send_buf[8]  ���ͳ���
    }

    USART3_SendByte(0x55);
    USART3_SendByte(0xAA);      //����

}
/*
 * ��������������3����(SU-03T)��������(2λ����)
 * �����߼����������ܹ�Ԫƽ̨�����ݽ���Э�飬�������ݹ�ȥ
*/
void Uart3_SU03T_SendCMD1(int dat1, int dat2)
{
    Send_buf[0] = (int)(dat1);         //datal
    Send_buf[1] = (int)(dat2);         //data2
    Uart3_SU03T_SendCmd(2);            //���ʹ�֡����
}
/*
 * ��������������3����(SU-03T)��������(1λ����)
 * �����߼����������ܹ�Ԫƽ̨�����ݽ���Э�飬�������ݹ�ȥ
*/
void Uart3_SU03T_SendCMD2(int dat1)
{
    Send_buf[0] = (int)(dat1);         //datal
   
    Uart3_SU03T_SendCmd(1);            //���ʹ�֡����

}
//�������ܣ�SU-03T����DHT11��ʪ��(���Բ���С���͵�)
void SU03T_DHT11_Play(void)
{
    u8 t = 0;
    if(t % 10 == 0) //ÿ10ms��ȡһ��
    {
     Uart3_SU03T_SendCMD1(0X05, MQ4.CH4); /*(����)�ϴ���SU-03T���д���*/
     Uart3_SU03T_SendCMD1(0X06,CH4_max); /*(������ֵ)�ϴ���SU-03T���д���*/
      
     Uart3_SU03T_SendCMD1(0X07,MQ7.CO);  
     Uart3_SU03T_SendCMD1(0X08,CO_max); 
      
     Uart3_SU03T_SendCMD1(0X09,JW_Value);
     Uart3_SU03T_SendCMD1(0X0A,CO2_max);  //��Ϣ��10
      
     Uart3_SU03T_SendCMD1(0X0B,MQ8.H2);
     Uart3_SU03T_SendCMD1(0X0C,H2_max);
    }
     
    delay_ms(10);
    t++;

    if(t == 20)/*200ms�����ʱ�䣬�������㣬�����ۼӼ���*/
    {
        t = 0;
    }

}

void Uart3_SU03T_alarm(void)
{
     

        if(alarm_CH4==1&&MQ4.CH4>=CH4_max)
        {
          Uart3_SU03T_SendCMD1(0X05, MQ4.CH4);
          Uart3_SU03T_SendCMD2(0X0D);
         
      
        }else if(alarm_H2==1&&MQ8.H2>=H2_max)
        {
          Uart3_SU03T_SendCMD1(0X0B,MQ8.H2);
          Uart3_SU03T_SendCMD2(0X0E);
        
      
        }else if(alarm_CO==1&&MQ7.CO>=CO_max)
        {
        Uart3_SU03T_SendCMD1(0X07,MQ7.CO);
        Uart3_SU03T_SendCMD2(0X0F);
       
        
        }
    
}




