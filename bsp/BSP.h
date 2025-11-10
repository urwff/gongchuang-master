#ifndef _BSP_H_
#define _BSP_H_

#include "sys.h"

#include "delay.h"
#include "usart.h"
#include "led.h"

#include "bsp_Timer.h"
#include "bsp_Motor_4w.h"
#include "bsp_usart.h"

#include "Targets.h"

void BSP_Init(void);
u16 my_pow(u16 Di_Shu,u16 Zhi_Shu);
#endif
