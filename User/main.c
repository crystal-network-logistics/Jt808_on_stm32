#include "./usart/usart.h"
#include "./usart2/usart2.h"
#include "./led/bsp_led.h"
#include "./gps/gps_config.h"
#include "./delay/delay.h"
#include "./ec20/ec20.h"
#include "./sys/sys.h"
#include "string.h"
#include "client_manager.h"
#include "jt808_packager.h"
#include "./internal_flash/bsp_internal_flash.h"  
#include "ff.h"
#include <stdio.h>
#include <stdint.h>

void Tim3_Int_Init(u16 arr,u16 psc);
void TIM3_IRQHandler(void);

int main(void)
{
	int i=0;
	int isTCPconnected=0;
	int isRegistered=0;
	int isAuthenticated=0;
	unsigned int v_alarm_value = 0;
	unsigned int v_status_value = 0;


	
//	LED_GPIO_Config();	//LED �˿ڳ�ʼ��
//  
//  GPIO_SetBits(GPIOD,GPIO_Pin_2);

	NVIC_Configuration(); 	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	delay_init();	    	 		//��ʱ������ʼ��
	uart_init(115200); //���ڳ�ʼ��
  USART2_Init(115200);
  GPS_Config();
	
	initSystemParameters();
	
	while(1)
	{
		//���ӷ�����
		if(isTCPconnected == 0)
		{
			if(ec20_init() == SUCCESS)
			{	
				printf("server connected\r\n");
				isTCPconnected=1;
				delay_ms(2000);
				continue;
			}
		}

		//�����ֻ��ţ�Ψһʶ��id��
		setTerminalPhoneNumber("15637142116", 11);

		//�ն�ע��
		if(isRegistered == 0)	
		{
			isRegistered = jt808TerminalRegister(isRegistered);
			
			if(isRegistered==0)
			{
				isTCPconnected=0;
				continue;
			}
			continue;
		}

		//�ն˼�Ȩ
		if(isAuthenticated == 0)
		{
			isAuthenticated =jt808TerminalAuthentication(isAuthenticated);		
			
			if(isAuthenticated==0)
			{
				isRegistered=0;
				isTCPconnected=0;
				continue;
			}
			continue;
		}
		
		//����λ���ϱ�����λ��״̬λ
		initLocationInfo(v_alarm_value, v_status_value);
		setStatusBit();
						
		while(1)
		{
			//λ���ϱ� �����߼�λ����ϱ�10��δ�յ�ƽ̨��Ӧ��Ϣ���������ӷ�����
			i = jt808LocationReport(i);
			printf("%d \r\n",i);
			if(i>10)
			{
				isRegistered=0;
				isTCPconnected=0;
				isAuthenticated=0;
				break;
			}

	//			Tim3_Int_Init(flashWriteInfo.write_time_interval*10000-1,7199);
		
		}
	}
}




void Tim3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = arr;
	TIM_TimeBaseInitStruct.TIM_Prescaler = psc;
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	
	NVIC_InitStruct.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 3;
	NVIC_Init(&NVIC_InitStruct);
	
	TIM_Cmd(TIM3,ENABLE);
}

void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update) == 1)
	{
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
		//jt808LocationReport();
	}
	
}

