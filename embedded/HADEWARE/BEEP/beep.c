#include "beep.h"

#include "stm32f10x.h"


BEEP_INFO beep_info = {0};

void Beep_Set(_Bool status)
{
    GPIO_WriteBit(GPIOB, GPIO_Pin_8, status == BEEP_ON ? Bit_SET : Bit_RESET); // ���status����BUZZER_ON��������Ϊ�ߵ�ƽ����������Ϊ�͵�ƽ

    beep_info.Beep_Status = status;
}

void Beep_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOB,GPIO_Pin_8);

    Beep_Set( BEEP_OFF);           // �رշ�����
}


