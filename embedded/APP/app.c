#include "app.h"
#include "usart.h"
#include "math.h"
#include "timer.h"
#include "beep.h"
#include "led.h"
#include "esp8266.h"
#include "JW.h"
#include "adc.h"

void Hardware_Init(void)
{
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//�жϿ�������������

  TIM4_Init();
  delay_init();
  
	Usart1_Init(9600);							//����1����ӡ��Ϣ���Լ���ʼ��CO2������
	
	Usart2_Init(115200);							//����2������ESP8266��
	
  Usart3_Init(115200);	
  
  initialize_MQ4(&MQ4, MQ4_ADC_Value,CH4);
  
  initialize_MQ8(&MQ8, MQ8_ADC_Value,H2);
  
  initialize_MQ7(&MQ7, MQ7_ADC_Value,CO);
  
	LED_Init();

		Beep_Init();
		Adc_Init();		  		//ADC��ʼ��
  
		OLED_Init();
    
		OLED_Clear(0); 
    OLED_ShowString(0,0,"Hardware init OK",16); 
    delay_ms(100);
		
//	UsartPrintf(USART_DEBUG, " Hardware init OK\r\n");
	
}

void Display1_Init(void)
{
  //	UsartPrintf(USART_DEBUG, "Connect MQTTs Server...\r\n");
	OLED_Clear(0); OLED_ShowString(0,0,"Connect MQTTs Server...",16);
	while(ESP8266_SendCmd(ESP8266_ONENET_INFO, "CONNECT"))
		delay_ms(500);
//	UsartPrintf(USART_DEBUG, "Connect MQTT Server Success\r\n");
	OLED_ShowString(0,4,"Connect MQTT Server Success",16); delay_ms(5000);

	OLED_Clear(0); OLED_ShowString(0,0,"Device login ...",16);delay_ms(5000);
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
				
			  MQ4.ADC_Value = Get_Adc_Average(ADC_Channel_0,10);//���鴫����ADC��ֵ��ģ������
				MQ4_PPM_Calibration();
				MQ4.CH4 = MQ4_GetPPM();//�����Ũ��ppm��ֵ
				OLED_ShowNum(40,2,MQ4.CH4,3,16);//����Ũ��
				
        UsartPrintf(USART_DEBUG, " CH4: %d��MQ4.CH4�� %d\r\n",CH4,MQ4.CH4);
        
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
				
				MQ7.ADC_Value = Get_Adc_Average(ADC_Channel_4,10);//��������ADC��ֵ��ģ������
				MQ7_PPM_Calibration();
				MQ7.CO= MQ7_GetPPM();//һ����̼��Ũ��ppm��ֵ
				OLED_ShowNum(72,6,MQ7.CO,3,16);//һ����̼Ũ��

      

				OLED_ShowString(96,7,"PPM",3);
				
				
				
//				OLED_ShowNum(48,6,ADC_Value,4,16);//��ѹֵ�������ã�
				System_Flag = 93;
  
}

void Refresh_Data(void)
{
  
        MQ4.ADC_Value =  Get_Adc_Average(ADC_Channel_0,10);//��������ADC��ֵ��ģ������
				MQ4.CH4 = MQ4_GetPPM();//һ����̼��Ũ��ppm��ֵ
				
				MQ7.ADC_Value =  Get_Adc_Average(ADC_Channel_4,10);//��������ADC��ֵ��ģ������
				MQ7.CO = MQ7_GetPPM();//һ����̼��Ũ��ppm��ֵ
				
				MQ8.ADC_Value =  Get_Adc_Average(ADC_Channel_1,10);//��������ADC��ֵ��ģ������
				MQ8.H2 = MQ8_GetPPM();//һ����̼��Ũ��ppm��ֵ
				
//			UsartPrintf(USART_DEBUG, " MQ4 ADCVALUE: %d�� \r\n",MQ4.ADC_Value);
//					delay_ms(1000);
        

				OLED_ShowNum(40,2,MQ4.CH4,3,16);//��ѹֵ
				
				OLED_ShowNum(104,2,MQ8.H2,3,16);//��ѹֵ
				
			  OLED_ShowNum(72,4,JW_Value,3,16);//��ѹֵ
				
				OLED_ShowNum(72,6,MQ7.CO,3,16);//��ѹֵ
//				OLED_ShowNum(48,6,ADC_Value,4,16);//��ѹֵ�������ã�
}