#include "timer.h"
#include "adc.h"
#include "led.h"
#include "beep.h"
#include "JW.h"
#include "usart.h"
int Sec = 90;//����ʱ����
int Waring_Time ;//DAC�����ֵ��
unsigned int System_Flag,System_Time;//ϵͳ��־λ��ϵͳ����ʱ��
int flag=1;
unsigned int Sys_Tick_Timer;



void TIM4_Init(void)//0.1ms��ʱ��������ϵͳ�ĺ���
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStrue;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	TIM_InternalClockConfig(TIM4);
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;//ָ��ʱ��Ƶ��
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//TIM���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_Period = 7200-1;	   //ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1-1; //PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; 
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);//������ʱ���ж�
	
	NVIC_InitStrue.NVIC_IRQChannel=TIM4_IRQn;
	NVIC_InitStrue.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStrue.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStrue.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStrue);//��ʼ���жϣ������жϵ����ȼ�
	
	TIM_Cmd(TIM4,ENABLE);//ʹ�ܶ�ʱ��
}

void TIM4_IRQHandler(void)//ϵͳ�߼�ִ���ж�
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)!=RESET)
	{
		if(++System_Time % 10000 == 0 && System_Flag <= 90){System_Flag++;Sec--;}//һ����̼�������ϵ���ҪԤ��һ�ְ��ӣ�5V����1���ӣ�
    
		if(System_Flag== 93&&flag==1)
		{
		
      if((MQ4.CH4 >= CH4_max&&alarm_CH4==1)||(MQ7.CO >= CO_max&&alarm_CO==1)||(MQ8.H2 >= H2_max&&alarm_H2==1))//COֵ����50����Σ�գ�����ѡ��40-50ΪԤ���׶�
			{
				Waring_Time++;
				if(Waring_Time<=500)
				{
					Beep_Set(BEEP_ON); //Σ�ռ�϶����

					Led_Set(LED_ON);
				}
				else if(Waring_Time<=1000)
				{

          Beep_Set(BEEP_OFF);
					Led_Set(LED_OFF);
				}
				else if(Waring_Time>1000)
					Waring_Time = 0;
			}
			else if(1)//COŨ�ȴ���Σ�ս׶Σ�����
			{
				Waring_Time = 0;
//        Beep_Set(BEEP_ON);

//				Led_Set(LED_ON);
			}
		}
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);//����жϱ�־λ
	}
}
