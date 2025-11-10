#include "BSP.h"
#include "pwm.h"
#include "Key.h"
#include "elevator_control.h"


uint8_t KeyNum;


/*串口区域变量*/
u8  USART1_RX_BUF[USART1_REC_LEN];
u8 	USART2_RX_BUF[USART2_REC_LEN];
u8  USART3_RX_BUF[USART3_REC_LEN];
u8  UART4_RX_BUF[UART4_REC_LEN];
u8  UART5_RX_BUF[UART5_REC_LEN];
u8  opencv_receive[8] = {0};
u8  counter_usart1, counter_usart2, counter_uart4,counter_uart5;                  //串口字节个数的循环计数
u8 Usart1_Receive_Flag=1;
u8 Uart5_Receive_Flag;
/*************/

/*模块使用变量*/
u16 QR_code_buffer1=132,QR_code_buffer2=123;		//二维码变量
float pitch, roll, yaw; 					//欧拉角
unsigned int TIM1_CH1_CCR_INC;			    //1修改速度（值越大速度越慢）
unsigned int TIM1_CH2_CCR_INC;			    //2修改速度（值越大速度越慢）
unsigned int TIM1_CH3_CCR_INC;			    //3修改速度（值越大速度越慢）
unsigned int TIM1_CH4_CCR_INC;			    //4修改速度（值越大速度越慢）

u16 elevator_pulse;							//升降步进输出的脉冲数

unsigned int target_pulse;					//底盘步进目标脉冲
unsigned int ch1_pulse_number,ch2_pulse_number,ch3_pulse_number,ch4_pulse_number =0x00;		//底盘步进输出的脉冲数
unsigned int current_pulse,current_pulse2,current_pulse3,current_pulse4=0;

/*杂变量*/
u8 back_flag;
u8 Pulse_Sended_Flag;
u8 Continue_Flag;
u8 Xia_Guang=1;
u8 Pluse_Flag;
/*************/
/*调试变量*/
/*************/
u16 count;


void Test_Duo_(void)
{
//		while(1)
//		{
//			HMI_Elevator_();
//		}
////
}
void Final(void);

void Test_Ma_Duo(void)
{
	Move_group(3, 100);	
	As_QR_code_Put_To_Area(213,1);
	while(1);
}
int  main(void)
{ 
	
	///////////////////////////////////////////////
	/////////////////系统原地初始化////////////////
	///////////////////////////////////////////////
	BSP_Init();
	PWM_Init();
	NVIC_Configuration();
	
	Key_Init();
//	Elevator_Init();
	Light_off_shang;
	Elevator_Height_Init();
	Init_Finished();
//	Move_group_Bing(5,200);
//	Move_group(2,200);
	Move_group(1,200);
//	Move_group(1,1000);
	

	while(1){
		KeyNum=Key_GetNum();
    delay_ms(1500);
		Di_Pan_Go('F',7,3);
    delay_ms(500);
    Di_Pan_Go('B',7,3);
    delay_ms(500);
    Di_Pan_Go('F',7,3);
    delay_ms(500);
    Di_Pan_Go('B',7,3);
    delay_ms(500);
    while(1);
//		Elevator_Move_Bottom();
		
//		Elevator_Move_Plate();
		
//		Elevator_Move_StackHeight();
//		Elevator_Move_Mid();
//		Move_group(4,500);
	
		if(KeyNum==1){
			///////////////////////////////////////////////
			////////////////////第一轮/////////////////////
			///////////////////////////////////////////////
			Di_Pan_Go('L',0.75,3);
			Di_Pan_Go('F',2.05,3);
//			while(USART1_RX_BUF[6]==0);
//			usart1_proc();
//			DisPlay(QR_code_buffer1,QR_code_buffer2);
			
			
			Di_Pan_Go('F',3.83,3);
			Move_group(3, 100);
			Elevator_Move_Plate();
			Di_Pan_Go('R',0.275,3);
			delay_ms(50);
			
				
			As_QR_code_Catch_Body_TurnTable1(QR_code_buffer1);
			Move_group(6, 100);
			
			Turntable_To_Area0();
			
			As_QR_code_Put_To_Area(QR_code_buffer1,0);
			
			As_QR_code_Put_To_Car(QR_code_buffer1);	
			
			Area0_To_Area1(QR_code_buffer1,0);
			As_QR_code_Put_To_Area(QR_code_buffer1,0);
			
			Area1_To_Turntable(QR_code_buffer1);
			
			///////////////////////////////////////////////
			////////////////////第二轮/////////////////////
			///////////////////////////////////////////////
			As_QR_code_Catch_Body_TurnTable2(QR_code_buffer2);
			Move_group(6, 100);
			Di_Pan_Go('L',0.27,3);
			Turntable_To_Area0();
			
			As_QR_code_Put_To_Area(QR_code_buffer2,0);
			As_QR_code_Put_To_Car(QR_code_buffer2);	
			
			Area0_To_Area1_Stack(QR_code_buffer2,0);
			As_QR_code_Put_To_Area(QR_code_buffer2,1);
			
//			Area1_To_Home(QR_code_buffer2);
			
			
			
//			///////////////////////////////////////////////
//			////////////////////第一轮/////////////////////
//			///////////////////////////////////////////////
//			//1.横向左移
//			Di_Pan_Go('L',1,3);				 // ------<距离
//			//2.前向移动，速度得调 直到扫码
//			//Di_Pan_Go('F',5.9,3);  			 // ------<距离 速度可以快点p
//			Move_group(3, 150);										//底盘朝外
//			//3.横向右移，距离得调 
//			Di_Pan_Go('R',0.27,3);  			 // ------<距离  
//			//4.可写四方向 移动 直到视觉里看到宝物
//			//#################################//
//			//5.先调整抓取的位置，在执行抓任务
//			As_QR_code_Catch_Body_TurnTable(QR_code_buffer1);
//			Move_group(6, 200);
//			//6.从原料区向前平移
//			Turntable_To_Area0();
//			//7.前移到交差点转90°向左平移到达调整斜率中点
//			//8.粗加工区域完成放置和抓的任务
//			As_QR_code_Put_To_Area(QR_code_buffer1,0);   // ------<斜率调整到点 到 调整三色点的距离
//			As_QR_code_Put_To_Car(QR_code_buffer1);	
//			//9.根据  粗加工区域  最后一个颜色  到达  交差点
//			Area0_To_Area1(QR_code_buffer1,0);
//			//10.完成 半成品区去···的  放置
//			As_QR_code_Put_To_Area(QR_code_buffer1,0);
//			//10.半成品到粗加工调整一下
//			Area1_To_Area0(QR_code_buffer1);			
//			//11.粗加工到转盘
//			Area0_To_Turntable(2);
//			//#################################//
//			///////////////////////////////////////////////
//			////////////////////第二轮/////////////////////
//			///////////////////////////////////////////////		
//			//1.执行抓任务
//			As_QR_code_Catch_Body_TurnTable(QR_code_buffer2);	
//			//2.从原料区向前平移
//			Turntable_To_Area0();
//			//3.执行 粗加工区 放置任务
//			As_QR_code_Put_To_Area(QR_code_buffer2,0);
//			As_QR_code_Put_To_Car(QR_code_buffer2);
//			//4.粗加工 到 半成品区
//			Area0_To_Area1(QR_code_buffer2,1);	
//			//5.执行 半成品区 码垛放置任务
//			As_QR_code_Put_To_Area(QR_code_buffer2,1);	
//			//5.半成品区 回家
//			Area1_To_Home(QR_code_buffer2);
			while(1);
		}
		else if(KeyNum==2){
			while(1);
		}
	}
}

void Final(void)
{
	///////////////////////////////////////////////
	/////////////////系统原地初始化////////////////
	///////////////////////////////////////////////
    BSP_Init();
//	Elevator_Init();
	Light_off_shang;
    Move_group(1, 100); 
	
	//1.横向左移 扫描二维码
	Di_Pan_Go('L',0.75,3);				 // ------<距离
    DisPlay(QR_code_buffer1,QR_code_buffer2);
    Di_Pan_Go('F',7.6, 3);				 // ------<距离
	DisPlay(QR_code_buffer1,QR_code_buffer2);
    Turn_Left_GY_61(9000 , 90);
    Di_Pan_Go('F', 3.4, 3);				 // ------<距离
	delay_ms(100);	
	Move_group(3, 100); 
    //调整斜率
    while(1)
    {
        if(Di_Pan_Vision_slope_Adjust('k') == 1)
            break;
    }
		delay_ms(200);
    while(1)
    {
        if(Di_Pan_Vision_static_dist_Adjust('f') == 1)
            break;
    } 
    Area0_To_Area1(2,0);
//    current_pt=As_QR_code_Catch_From_Area(QR_code_buffer1,0);
    //Area1_To_Area0(4-current_pt);
		As_QR_code_Put_To_Area(QR_code_buffer1,0);          // ------<中点 到 调整三色点的距离
		As_QR_code_Put_To_Car(QR_code_buffer1);	

    Area0_To_Turntable(QR_code_buffer1); 
    As_QR_code_Put_To_Turn_Table(QR_code_buffer1,0);
		while(1);
}