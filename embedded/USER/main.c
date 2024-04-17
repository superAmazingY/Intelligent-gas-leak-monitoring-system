#include "stm32f10x.h"
#include "delay.h"
#include "adc.h"
#include "oled.h"
#include "usart.h"
#include "math.h"
#include "timer.h"
#include "beep.h"
#include "led.h"
#include "esp8266.h"
#include "JW.h"
//����Э���
#include "onenet.h"


void Hardware_Init(void);//Ӳ����ʼ��
void Display1_Init(void);//�˵�һ����½��ʼ��
void Display2_Init(void);//�˵��������˵�
void Refresh_Data(void);//ˢ�¼��ֵ
void SNR8016_Handle(void);






#define ESP8266_ONENET_INFO		"AT+CIPSTART=\"TCP\",\"mqtts.heclouds.com\",1883\r\n"

int main(void)
{
  unsigned short timeCount = 0;	//���ͼ������
	
	unsigned char *dataPtr = NULL;
  
	Hardware_Init();				//��ʼ����ΧӲ��
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  Usart1_Init(9600);
  ESP8266_Init();					//��ʼ��ESP8266
  
  Display1_Init();       //��ʼ������1
  
  OneNET_Subscribe();     //�����Ʒ�����

	while(1)
	{
      if(Sys_Tick_Timer >= 300)
      {
        JW_Deal_Timer_Count();
        Sys_Tick_Timer = 0;
      }   
      JW_Data_Receive();
      JW_Pross();
    timeCount++;
//      System_Flag=93;
      
		if(System_Flag <= 90)
		{
			OLED_ShowNum(80,5,Sec,2,16);//��ѹֵ
		}
		else
		{
			if(System_Flag == 91)
			{
				OLED_Clear(0);
        Uart3_SU03T_SendCMD2(0x04);

				System_Flag = 92;
			}
			else if(System_Flag == 92)
			{
			 Display2_Init();//��ϵͳ�˵���ʾ
			
			}
			else if(System_Flag == 93)
			{
        
//      	UsartPrintf(USART_DEBUG, "CH4:%d\r\n",MQ4.CH4);delay_ms(10);
//        UsartPrintf(USART_DEBUG, "����ʱ����time:%d\r\n",timeCount);delay_ms(500);
        if(timeCount >= 50)								//���ͼ��2s
          {
//            UsartPrintf(USART_DEBUG, "������Ϣ");delay_ms(10);
             OneNet_SendData();									//��������		
             timeCount = 0;
             ESP8266_Clear();
             UsartPrintf(USART_DEBUG, "��ɷ���");delay_ms(10);
           }
         dataPtr = ESP8266_GetIPD(0);
          if(dataPtr != NULL)
          OneNet_RevPro(dataPtr);//  ok
          
           Refresh_Data();//�����˵�������ʾ����ֵ
           SNR8016_Handle();
//          UsartPrintf(USART_DEBUG, "OneNet_SendData\r\n");delay_ms(500);
          delay_ms(5);
			}
		}
	}
}

 void Hardware_Init(void)
{
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//�жϿ�������������

  TIM4_Init();
  delay_init();
  
	Usart1_Init(9600);							//����1����ӡ��Ϣ���Լ���ʼ��CO2������
	
	Usart2_Init(115200);							//����2������ESP8266��
	
  Usart3_Init(9600);
  initialize_MQ4(&MQ4, MQ4_ADC_Value,CH4);
  
  initialize_MQ8(&MQ8, MQ8_ADC_Value,H2);
  
  initialize_MQ7(&MQ7, MQ7_ADC_Value,CO);
  
	LED_Init();

		Beep_Init();
		Adc_Init();		  		//ADC��ʼ��
  
		OLED_Init();
       OLED_ShowString(0,0,"Hardware init ...",16); 
     delay_ms(5000);
		OLED_Clear(0); 
    Uart3_SU03T_SendCMD2(0x01);//����Ӳ����ʼ���ɹ�
    OLED_ShowString(0,0,"Hardware init OK",16); 
    delay_ms(100);
		
//	UsartPrintf(USART_DEBUG, " Hardware init OK\r\n");
	
}
void Display1_Init(void)
{
  Uart3_SU03T_SendCMD2(0x02);//��������һ������
	OLED_Clear(0); OLED_ShowString(0,0,"Connect MQTTs Server...",16);
	while(ESP8266_SendCmd(ESP8266_ONENET_INFO, "CONNECT"))
		delay_ms(500);
//	UsartPrintf(USART_DEBUG, "Connect MQTT Server Success\r\n");
	OLED_ShowString(0,4,"Connect MQTT Server Success",16); delay_ms(5000);

	OLED_Clear(0); OLED_ShowString(0,0,"Device login ...",16);delay_ms(5000);
    Uart3_SU03T_SendCMD2(0x03);//���������ɹ�����ӭʹ�ñ�ϵͳ
    OLED_Clear(0); OLED_ShowString(0,0,"Device Success",16);delay_ms(5000);
	while(OneNet_DevLink())			//����OneNET
	{
		ESP8266_SendCmd(ESP8266_ONENET_INFO, "CONNECT");
		delay_ms(500);
	}
		
  
  OLED_Clear(0);
  delay_ms(2000);
	OLED_ShowCHinese(16-8,2,0);//��
	OLED_ShowCHinese(32-8,2,1);//��
	OLED_ShowCHinese(48-8,2,2);//��
	OLED_ShowCHinese(64-8,2,3);//��
	OLED_ShowCHinese(80-8,2,4);//��
	OLED_ShowCHinese(96-8,2,5);//Ԥ
	OLED_ShowCHinese(112-8,2,6);//��
	OLED_ShowCHinese(16,5,7);//��
	OLED_ShowCHinese(32,5,8);//��
	OLED_ShowCHinese(48,5,9);//��
	OLED_ShowCHinese(64,5,10);//��
	OLED_ShowCHinese(96,5,11);//��
  
}
void Display2_Init(void)
{
  	//��һ��
				OLED_ShowCHinese(0,0,12);//��
				OLED_ShowCHinese(16,0,13);//��
				OLED_ShowCHinese(32,0,14);//��
				OLED_ShowCHinese(48,0,15);//��
				OLED_ShowCHinese(64,0,16);//��
				OLED_ShowCHinese(80,0,17);//��
				OLED_ShowCHinese(96,0,18);//ϵ
			  OLED_ShowCHinese(112,0,19);//ͳ
					//�ڶ���
				
	      OLED_ShowCHinese(0,2,20);//��
		  	OLED_ShowCHinese(16,2,21);//��
				OLED_ShowChar(32,2,':',16);
        
        if(CH4_flag==1)
        {
          MQ4.ADC_Value = Get_Adc_Average(ADC_Channel_0,10);//���鴫����ADC��ֵ��ģ������
          MQ4_PPM_Calibration();
          MQ4.CH4 = MQ4_GetPPM();//�����Ũ��ppm��ֵ
        }else MQ4.CH4=0;
				
			  
				OLED_ShowNum(40,2,MQ4.CH4,3,16);//����Ũ��
				
//        UsartPrintf(USART_DEBUG, " CH4: %d��MQ4.CH4�� %d\r\n",CH4,MQ4.CH4);
        
				OLED_ShowCHinese(64,2,23);//��
				OLED_ShowCHinese(80,2,24);//��
				OLED_ShowChar(96,2,':',16);
				
				MQ8.ADC_Value = Get_Adc_Average(ADC_Channel_1,10);//����������ADC��ֵ��ģ������
				MQ8_PPM_Calibration();
				MQ8.H2 = MQ8_GetPPM();//������Ũ��ppm��ֵ
				OLED_ShowNum(104,2,MQ8.H2,3,16);//����Ũ��
						
					//������
				OLED_ShowCHinese(0,4,26);//��
				OLED_ShowCHinese(16,4,27);//��
				OLED_ShowCHinese(32,4,28);//��
				OLED_ShowCHinese(48,4,29);//̼
				OLED_ShowCHinese(64,4,30);//��
				OLED_ShowNum(72,4,JW_Value,3,16);//��ѹֵ
				
		
				
				OLED_ShowString(96,5,"PPM",3);
				//������
				OLED_ShowCHinese(0,6,31);//һ
				OLED_ShowCHinese(16,6,32);//��
				OLED_ShowCHinese(32,6,33);//��
				OLED_ShowCHinese(48,6,34);//̼
				OLED_ShowCHinese(64,6,35);//��
				
        if(CO_flag==1)
        {
         	MQ7.ADC_Value = Get_Adc_Average(ADC_Channel_4,10);//��������ADC��ֵ��ģ������
          MQ7_PPM_Calibration();
          MQ7.CO= MQ7_GetPPM();//һ����̼��Ũ��ppm��ֵ
        }else MQ7.CO=0;
				OLED_ShowNum(72,6,MQ7.CO,3,16);//һ����̼Ũ��

      

				OLED_ShowString(96,7,"PPM",3);
				
				
				
//				OLED_ShowNum(48,6,ADC_Value,4,16);//��ѹֵ�������ã�
				System_Flag = 93;
  
}

// ��ȫ�ֱ������崦����һ����ʱ������
uint32_t voice_timer = 49;

void Refresh_Data(void)
{
    if(CH4_flag==1)
    {
       MQ4.ADC_Value =  Get_Adc_Average(ADC_Channel_0,10);
       MQ4.CH4 = MQ4_GetPPM();
    }
    else MQ4.CH4=0;
      
    if(CO_flag==1)
    {
       MQ7.ADC_Value =  Get_Adc_Average(ADC_Channel_4,10);
       MQ7.CO = MQ7_GetPPM();
    }
    else MQ7.CO=0;
    
    if(H2_flag==1)
    {
        MQ8.ADC_Value =  Get_Adc_Average(ADC_Channel_1,10);
        MQ8.H2 = MQ8_GetPPM();
    }
    else MQ8.H2=0;
    
    OLED_ShowNum(40,2,MQ4.CH4,3,16);
    OLED_ShowNum(104,2,MQ8.H2,3,16);
    OLED_ShowNum(72,4,JW_Value,3,16);
    OLED_ShowNum(72,6,MQ7.CO,3,16);
    
    // ��ʱ������
    voice_timer++;
  
    // ÿ20�봥��һ����������
    if(voice_timer == 60)
    {
      

       Uart3_SU03T_alarm();
        voice_timer = 0;
    }
}
void SNR8016_Handle(void)
{
  if(USART3_RX_STA == 1)
   {
      switch(USART3_RX_CMD)
      {
        //�رվ���
        case 0x01:
           flag=0;
           Beep_Set(BEEP_OFF);
           Led_Set(LED_OFF);
          break ;
        //�򿪱�������
        case 0x02:
          flag=1;
           break;
          //���ļ�����ֵ 
        case 0x03:  
          CH4_max=21;
           break;
        case 0x04:
          CH4_flag=0;
           break;
        case 0x05:
          CO_max=27;
          break;
        case 0x06:
          CO_flag=0;
          break;
        case 0x07:
          CO2_max=55;
          break;
         case 0x08:
          CO2_flag=0;
          break;
         case 0x09:
          H2_max=24;
          break;
         case 0x00:
          H2_flag=0;
          break;
         case 0x0B:
          CH4_flag=1;
          break;
         case 0x0C:
          CO2_flag=1;
          break;
         case 0x0D:
          CO_flag=1;
          break;
         case 0x0E:
          H2_flag=1;
          break;
         case 0x10:
         alarm_CH4=1;
          break;
         case 0x11:
         alarm_H2=1;
          break;
         case 0x12:
         alarm_CO=1;
          break;
         case 0x13:
         alarm_CO2=1;
          break;
         case 0x14:
         alarm_CH4=0;
          break;
         case 0x15:
         alarm_H2=0;
          break;
         case 0x16:
         alarm_CO=0;
          break;
         case 0x17:
         alarm_CO2=0;
          break;
      
   
     //���ʹ�����Ũ��,��ֵ
        case 0x0A:
          SU03T_DHT11_Play();
     


           break;
       

     }	
   }  

}



