/**
  ******************************************************************************
  * @file    main.c
  * @author  Tmax Sco
  * @version V1.0.0
  * @date    2020-2-22
  * @brief   完成速度环位置环等初始化任务
  ******************************************************************************
  * @attention
  * 请注意不要随意随意修改init()中函数的初始化顺序
  * 本程序已进行实际测试们可以正常使用
  * 如果使用效果不满意，可以在ctrl.c中修改相应的pid参数
  * 本程序版权归东北大学ACTION实验室所有
	* 该版本由湖工大竞技机器人队修改
  ******************************************************************************
  */

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_it.h"
#include "timer.h"
#include "gpio.h"
#include "usart.h"
#include "misc.h"
#include "can.h"
#include "math.h"
#include "stdio.h"
#include "arm_math.h"
#include "ctrl.h"
#include "comm.h"

extern DriverType Driver[8];
extern MotorType Motor[8];

void init(void)
{
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  //USART3_DMA_Init(921600);//外接串口初始化

  CAN_Config(CAN1, 1000, GPIOB, GPIO_Pin_8, GPIO_Pin_9);
  CAN_Config(CAN2, 1000, GPIOB, GPIO_Pin_5, GPIO_Pin_6);

  TIM_Init(TIM2, 999, 83, 0, 0); //1ms控制一次
  //TIM_Cmd(TIM2,DISABLE);

  TIM_Delayms(TIM3, 200); //用于延时用的定时器

  for (int i = 0; i < 8; i++)
  {
    Motor[i].type = RM_3508;
    Driver[i].posCtrl.actualPos = 0.0f;
    MotorOn(i);
  }

  DriverInit(); //初始化速度环以及位置环参数
}

int main(void)
{
  init();
  GPIO_Init_Pins(GPIOA, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3, GPIO_Mode_OUT);
  GPIO_ResetBits(GPIOA, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3);

  while (1)
  {

    GPIO_ToggleBits(GPIOA, GPIO_Pin_0|GPIO_Pin_1);

    CanSendPeriodMsgs();

    CANRespond();

    TIM_Delayms(TIM3, 50);
  }
}

/************************ (C) COPYRIGHT 2019 ACTION ********************/
