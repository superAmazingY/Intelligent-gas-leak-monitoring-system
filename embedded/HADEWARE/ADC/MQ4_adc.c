#include "adc.h"
#include "delay.h"		 
#include "math.h"
//��ʼ��ADC			
int alarm_CH4=1;
int alarm_CO=1;
int alarm_H2=1;

int CH4_flag=1;
int CO_flag=1;
int H2_flag=1;
uint16_t CH4_max=17;
uint16_t H2_max=23;
uint16_t CO_max=25;

 unsigned int CH4;
 uint16_t MQ4_ADC_Value;
  MQ4_Sensor MQ4;
 unsigned int H2;
 uint16_t MQ8_ADC_Value;
 MQ8_Sensor MQ8;

 unsigned int CO;
 uint16_t MQ7_ADC_Value;
  MQ7_Sensor MQ7;



void initialize_MQ4(MQ4_Sensor *sensor, uint16_t adc_value,unsigned int CH4) {
    sensor->R0 = MQ4_R0;  
    sensor->ADC_Value = adc_value; 
    sensor->CH4 = CH4; 
}

void MQ4_PPM_Calibration(void)
{
    float Vrl = 3.3f * MQ4.ADC_Value / 4095.f; // �������ѹֵ
    float RS = (3.3f - Vrl) / Vrl * RL; // ���ݷ�ѹԭ������RS�ĵ���

    // ����һ���������ӻ����������ӿ�R0ֵ����������
    float growth_factor = 1.25f; // �������ӣ�����Ը����������
    MQ4.R0 = (RS / pow(CAL_PPM / 98.322, 1 / -1.458f)) * growth_factor; // ������µ�R0ֵ

    // OLED_ShowNum(48, 0, R0, 4, 16); // R0�����ֵ��ʾ��OLED��Ļ�ϣ������ã�
}
 // ��ȡ��������ֵ
float MQ4_GetPPM(void)
{
	float Vrl = 3.3f * MQ4.ADC_Value / 4095.f;//�������ѹֵ
	float RS = (3.3f - Vrl) / Vrl * RL;//���ݷ�ѹԭ������RS�ĵ���
	
	float ppm = 98.322f * pow(RS/MQ4.R0, -1.458f);//�����PPM��ֵ
	return  ppm; 
}













void initialize_MQ8(MQ8_Sensor *sensor, uint16_t adc_value,unsigned int H2) {
    sensor->R0 = MQ8_R0;  
    sensor->ADC_Value = adc_value; 
    sensor->H2 = H2;
}
void MQ8_PPM_Calibration(void)
{
    float Vrl = 3.3f * MQ8.ADC_Value / 4095.f;
    float RS = (3.3f - Vrl) / Vrl * RL;
    float growth_factor = 1.5f; // �������ӵ���Ϊ1.5
    MQ8.R0 = (RS / pow(CAL_PPM / 98.322, 1 / -1.458f)) * growth_factor;
    // OLED_ShowNum(48,0,R0,4,16);
}
 // ��ȡ��������ֵ
float MQ8_GetPPM(void)
{
	float Vrl = 3.3f * MQ8.ADC_Value / 4095.f;//�������ѹֵ
	float RS = (3.3f - Vrl) / Vrl * RL;//���ݷ�ѹԭ������RS�ĵ���
	float ppm = 98.322f * pow(RS/MQ8.R0, -1.458f);//�����PPM��ֵ
	return  ppm; 
}







void initialize_MQ7(MQ7_Sensor *sensor, uint16_t adc_value,unsigned int CO)
 {

    sensor->R0 = MQ7_R0;  
    sensor->ADC_Value = adc_value; 
    sensor->CO = CO;
}
  
void MQ7_PPM_Calibration(void)
{
	float Vrl = 3.3f * MQ7.ADC_Value / 4095.f;//�������ѹֵ
	float RS = (3.3f - Vrl) / Vrl * RL;//���ݷ�ѹԭ������RS�ĵ���
	
	  MQ7.R0 = (RS / pow(CAL_PPM / 98.322, 1 / -1.458f));//����MATLAB���������Ƶ���R0�Ķ�Ӧ��ʽ
//	OLED_ShowNum(48,0,R0,4,16);//R0�����ֵ��ʾ��OLED��Ļ�ϣ������ã�
}
 // ��ȡ��������ֵ
float MQ7_GetPPM(void)
{
	float Vrl = 3.3f * MQ7.ADC_Value / 4095.f;//�������ѹֵ
	float RS = (3.3f - Vrl) / Vrl * RL;//���ݷ�ѹԭ������RS�ĵ���
	
	 float growth_factor = 1.50f; // �������ӣ�����Ը����������
	float ppm = 98.322f * pow(RS/MQ7.R0, -1.458f) * growth_factor;;//�����PPM��ֵ
	return  ppm; 
}




void Adc_Init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1, ENABLE );	  //ʹ��ADC1ͨ��ʱ��
 

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M

	//PA0 ��Ϊģ��ͨ����������                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_4|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	

	ADC_DeInit(ADC1);  //��λADC1 

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//ģ��ת�������ڵ�ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//ģ��ת�������ڵ���ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 2;	//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���   

	ADC_Cmd(ADC1, ENABLE);	//ʹ��ָ����ADC1
	
	ADC_ResetCalibration(ADC1);	//ʹ�ܸ�λУ׼  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼����
	
	ADC_StartCalibration(ADC1);	 //����ADУ׼
 
	while(ADC_GetCalibrationStatus(ADC1));	 //�ȴ�У׼����
 
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������

}				  
//���ADCֵ
//ch:ͨ��ֵ 0~3
u16 Get_Adc(u8 ch)   
{
  	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADCͨ��,����ʱ��Ϊ239.5����	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������

	return ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����
}

u16 Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		delay_ms(5);
	}
	return temp_val/times;
} 





































