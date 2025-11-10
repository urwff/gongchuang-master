/*	
	功能：读取对应的光电平
	输入参数：x=L，读取左光电平
						x=R，读取右光电平
	返回参数：x的光电平
*/
u8 ReadGuang(char x)
{
	u8 res=0; 
	switch (x)
	{
		case 'L': res=  Left_Guang ; break;
		case 'R': res=  Right_Guang; break;
	}
	return res;
}

/*
	功能：读取对应灰度的光电平信号
	输入参数：上，读取灰度传感器值-1
	返回参数：对应灰度值
*/
u8 ReadD(u8 i)
{
	u8 res=0;
	switch (i)
	{
		case  0: res=  D1 ;break;
		case  1: res=  D2 ;break;
		case  2: res=  D3 ;break;
		case  3: res=  D4 ;break;
		case  4: res=  D5 ;break;
		case  5: res=  D6 ;break;
		case  6: res=  D7 ;break;
		case  7: res=  D8 ;break;
		case  8: res=  D9 ;break;
		case  9: res=  D10;break;
		case 10: res=  D11;break;
		case 11: res=  D12;break;	
	}
	return res;
}


/*
	功能：从左后方向前方有白线
	输入参数：需要几个灰度对应
	返回参数：1，检测到线
						0，没检测到
*/

u8 zuohoufangdaoyouqianfangyoubaixian(u8 num) //左后方向前方有白线
{
	u16 check=0,check_flag=0x8000;//0000 0000 0000 ****(低4位有效位)，左右分别对应灰度的D1~D12
	u8 i=0;   //1000 0000 0000 ****
	
	if(D1){ check |= (check_flag>>i); i++; }
	while(i<num)
	{
		track_PID_moni(zhi_xian);
		if(  ( check&( check_flag >> (i-1)) )  &&   ReadD(i) )
		{
			check |= (check_flag>>i);  i++;
		}
		//然后可以写一个延时函数，防止死循环
		else{break;}
	}
	if(i==num) { return 1; }
	else { return 0; }
}

/*
	功能：从右后方向左前方有白线
	输入参数：需要几个灰度对应
	返回参数：1，检测到线
						0，没检测到
*/
u8 youhoufangdaozuoqianfangyoubaixian(u8 num)
{
	u16 check=0,check_flag=0x0010;//0000 0000 0000 ****(低4位有效位)，左右分别对应灰度的D1~D12
	u8 i=0;   //1000 0000 0000 ****
	
	if(D12){ check |= (check_flag<<i); i++; }
	while(i<num)
	{
		track_PID_moni(zhi_xian);
		if(  ( check&( check_flag << (i-1)) )  &&   ReadD(12-i-1) )
		{
			check |= (check_flag << i );  i++;
		}
		//然后可以写一个延时函数，防止死循环
		else{break;}
	}
	if( (i) == num ) { return 1; }
	else { return 0; }
}


/*
		功能：圆点回归关键
		输入参数：time1，time2，得到小车在路上的时间时间
							time3，time4，白线的个数
		返回参数：无
*/

u8 yuan_point=0; //圆点位置
void yuandianhuijia(void)// 2.左转90度 3.前进 4.右转  5.前进回到路上 
{
	if(!yuan_point)
	{		
		while(   ReadGuang('L')   ||   ReadGuang('R')  )
		{  
			track_PID_moni(zhi_xian); 
			//路上前进调匀，不要太快
		}
		yuan_point++;
	}
	
	if(yuan_point ==1)
	{
		/*2.左转90度*/
		Turn_Left_6050(480,80);
		while(D6||D7)
		{
			Hui_Du_Turn_Left(600);
		}
		motor2(600,600);
		yuan_point++;
	}
	
	if(yuan_point==2)
	{
		/*3.前进一段距离*/
		while(   ReadGuang('L')   ||   ReadGuang('R')  )
		{ 
			track_PID_moni(zhi_xian);
			//路上前进调匀，不要太快
		}
		yuan_point++;
	}
	
	/*4.然后一段距离的路*/
	if(yuan_point==3)
	{
		/*5.右转90度*/
		Turn_Right_6050(480,80);
		while(D6||D7)
		{
			Hui_Du_Turn_Right(600);
		}
		yuan_point++;
	}
	
	if(yuan_point==4)
	{
		/*6.前进一段距离的路*/
		zuohoufangdaoyouqianfangyoubaixian(6);
		
		if(!D3){motor2(0,0);}//当Dx黑线时间，当前D3说明当前时刻的长度已经在交叉点

		/*7.再转一个角度使小车能够回到家的位置*/
		Turn_Left_6050(480,20);
		while(D6||D7)
		{
			Hui_Du_Turn_Left(600);
		}		
		yuan_point++;
	}
	
	
	if(yuan_point==5)
	{
		/*8.直接过障碍*/
		motor2(100,100);
		/***************然后写写写***************/
		yuan_point++;
	}

	if(yuan_point==6)
	{
		/*9.当看到路口的交叉路线时，然后转弯*/
		motor2(600,600);
		if(!D10){motor2(0,0);}//右灰度Dx(根据实际10)扫到黑线说明当前时刻已经在交叉点，然后转弯
		
		while(D6||D7)
		{
			Turn_Right_6050(480,1);
		}
		
		/*10.回家*/
		back_home_last_step();
	}

}


/*
	功能：从五好台回家
	输入参数：time1，time2，得到小车在路上的时间时间
	返回参数：无
*/
u8 wuhaotai_point=0;
void congwuhaotaihuijia(void)
{
	if(!wuhaotai_point)
	{
		/*1.后退*/
		
		wuhaotai_point++;
	}
	
	if(wuhaotai_point==1)
	{
		/*2.循迹直线*/
		motor2(600,600);
//		
//		track_PID_moni(zhi_xian);
//		
//		/*3.准备过*/
//		TIMER_Zhun_guo_kan(320);
		
		wuhaotai_point++;
	}
	
	
	if(wuhaotai_point==2)
	{
		/*4.直接到路上*/
		motor2(600,600);
		while(   ReadGuang('L')   ||   ReadGuang('R')  ){  track_PID_moni(zhi_xian); }
		yuan_point++;
	}
	
	if(wuhaotai_point==3)
	{
		/*5.右转90度*/
		Turn_Right_6050(480,80);
		while(D6||D7)
		{
			Hui_Du_Turn_Right(600);
		}
		wuhaotai_point++;
	}
	
	if(wuhaotai_point==4)
	{
		/*6.直接到另一条路*/
		motor2(600,600);
		while(   ReadGuang('L')   ||   ReadGuang('R')  ){  track_PID_moni(zhi_xian); }
		yuan_point++;
	}
	
	if(wuhaotai_point==5)
	{
			/*7.左转90度*/
		Turn_Left_6050(480,80);//左转80度
		while(D6||D7)
		{
			Hui_Du_Turn_Left(600);
		}
		wuhaotai_point++;
	}

	if(wuhaotai_point==6)
	{
		/*8.前进到另一条路*/
		youhoufangdaozuoqianfangyoubaixian(6);
		
		if(!D10){  motor2(0, 0); }//当Dx(根据实际10)扫到黑线说明当前时刻已经在交叉点
		
		/*9.转弯*/
		Turn_Right_6050(480,90);
		while(D6||D7)
		{
			Hui_Du_Turn_Right(600);
		}
		
		wuhaotai_point++;
	}
	
	if(wuhaotai_point==7)
	{
		/*10.直接过*/
		/**************然后写写***********/
		
		/*11.然后到路上*/
		/**************然后写写***********/
		wuhaotai_point++;
	}
	
	if(wuhaotai_point==8)
	{
		/*12.转弯*/
		if(!D10){motor2(0, 0);}
		Turn_Right_6050(480,20);
		while(D6||D7)
		{
			Hui_Du_Turn_Right(600);
		}
		
		/*13.回家*/
		back_home_last_step();
	}
	
}

/*
	功能：计算跳过个数
	输入参数：上，使用哪个传感器过去
					  L-左传感器过去
						R-右传感器过去
	返回参数：返回传感器的跳过个数
	(需要自己过去tiao_num变量，没有需要定义)
*/
u8 Count_tiaozi(char x)
{
	if( ReadGuang(x) ){ tiao_flag=1; }
	if( tiao_flag && ReadGuang(x) ){ tiao_flag=0; tiao_num++; }
	return tiao_num;
}

/*
	功能：从十字路口的交叉路回家
	输入参数：无
	返回参数：无
*/
void back_home_last_step(void)
{
		motor2(600,600);
		
		while( ReadGuang('L') && ReadGuang('R') ){track_PID_moni(zhi_xian);}
		
		while( !ReadGuang('L')  ||  !ReadGuang('R') );
		
		while(tiao_num<2){ Count_tiaozi('R');}
		
		tiao_num=0;
		motor2(0,0);
}