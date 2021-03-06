#include "cdhdata.h"


//孙先生的点数处理函数（类似于我的CDH_read_dataa函数功能）
Fencha_struct front_cdh16,back_cdh16,backl_cdh8,backr_cdh8;//定义一个Fencha_struct类型的全局变量
Fencha_struct findtemp={0,0,0,0,0,0};//孙先生函数中的结构体数组定义

Fencha_struct g_CDH8_qian_1;
Fencha_struct g_CDH8_qian_2;

Fencha_struct g_CDH8_hou_1;
Fencha_struct g_CDH8_hou_2;

//16位磁导航数据处理函数

//dst:磁导航的原始16进制的16位数
//num:处理时正常情况的最多点数（1~15）
//dir:分叉方向1：左分叉，2：右分叉
//返回值：Fencha_struct结构体

Fencha_struct FindSpace(u16 dst,u8 num,u8 dir)
{
	u16 i=0,dirr=dir;
	u16 p=~dst,numr=0,numl=0;
	Fencha_struct find;
	AA:	
	find.zeroNum = 0;
	find.zeroContinue = 0;
	find.Num = 0;
	find.Distance = 0;
	find.Error = 0;
	//计算有效点的个数
	for(i=0;i<16;i++)
	{
		if((p>>i)&0x0001)
		{
			find.Num++;
		}
	}
	//判断右边0的个数
	for(i=0;i<16;i++)
	{
		if(((p>>i)&0x0001)==0)
		{
			numr++;
		}
		else
		{
			i=16;//直接退出
		}
	}
	
	//判断左边0的个数
	for(i=0;i<16;i++)
	{
		if(((p<<i)&0x8000)==0)
		{
			numl++;
		}
		else
		{
			i=16;//直接退出
		}
	}		
	//计算0的个数，并计算0出现的位置
	for(i=numr;i<(16-numl);i++)
	{
		if(((p>>i)&0x0001)==0)
		{
			find.zeroLoctaion[find.zeroNum]=i;
			find.zeroNum++;
		}
	}
		
	//判断断点是否连续
	if(find.zeroNum>0)
	{
		find.zeroContinue = 1;
		if(find.zeroNum>1)
		{
			for(i = 0;i<(find.zeroNum-1);i++)
			{
				if(find.zeroLoctaion[i+1]-find.zeroLoctaion[i]==1)
				{
					continue;
				}
				else
				{
					find.zeroContinue++;
				}
			}
		}
	}
	//计算有效点距离中心位置的距离
	if(find.Num==0)//没找到有效点，距离返回0
	{
		find.Distance = 16;
		find.Error = 1;		
	}

	else
	{
		if(find.zeroNum>0)//有断点，返回0
		{
			if(find.zeroContinue==1)//一个分叉
			{
				if(dirr==2)//右分叉
				{
					for(i=numr;i<16;i++)
					{
						if((p>>i)&0x0001)
						{
							p = p&(~(0x0001<<i));
						}
						else
						{
							i = 16;
						}
					}
					goto AA;
				}
				else if(dirr==1)//左分叉
				{
					for(i=numl;i<16;i++)
					{
						if((p<<i)&0x8000)
						{
							p = p&(~(0x8000>>i));
						}
						else
						{
							i = 16;
						}
					}
					goto AA;
				}
				else
				{
					find.Error = 1;
				}
			}
			
		}
		else//正常情况
		{
			if(find.Num<=num)
			{				
				find.Distance = numl - numr ;
			}
			else
			{
				find.Error = 1;
			}
		}
	}
	
	findtemp = find;
	return find;

}
//8位磁导航数据处理函数
//dst:磁导航的原始16进制的8位数
//num:处理时正常情况的最多点数（1~7）
//dir:分叉方向1：左分叉，2：右分叉
//返回值：Fencha_struct结构体

Fencha_struct FindSpace1(u8 dst,u8 num,u8 dir)
{
	u8 i=0,dirr=dir;
	u8 p=~dst,numr=0,numl=0;
	Fencha_struct find;
	AA:	
	find.zeroNum = 0;
	find.zeroContinue = 0;
	find.Num = 0;
	find.Distance = 0;
	find.Error = 0;
	//计算有效点的个数
	for(i=0;i<8;i++)
	{
		if((p>>i)&0x01)
		{
			find.Num++;
		}
	}
	//判断右边0的个数
	for(i=0;i<8;i++)
	{
		if(((p>>i)&0x01)==0)
		{
			numr++;
		}
		else
		{
			i=8;//直接退出
		}
	}
	
	//判断左边0的个数
	for(i=0;i<8;i++)
	{
		if(((p<<i)&0x80)==0)
		{
			numl++;
		}
		else
		{
			i=8;//直接退出
		}
	}		
	//计算0的个数，并计算0出现的位置
	for(i=numr;i<(8-numl);i++)
	{
		if(((p>>i)&0x01)==0)
		{
			find.zeroLoctaion[find.zeroNum]=i;
			find.zeroNum++;						//空点个数
		}
	}
		
	//判断断点是否连续
	if(find.zeroNum>0)
	{
		find.zeroContinue = 1;
		if(find.zeroNum>1)
		{
			for(i = 0;i<(find.zeroNum-1);i++)
			{
				if(find.zeroLoctaion[i+1]-find.zeroLoctaion[i]==1)
				{
					continue;
				}
				else
				{
					find.zeroContinue++;
				}
			}
		}
	}
	//计算有效点距离中心位置的距离
	if(find.Num==0)//没找到有效点，距离返回0
		find.Error = 1;
	else
	{
//		if((findtemp.zeroNum==0)&&(find.zeroNum>4)&&(findtemp.Num>0))
//		{
//				if(find.zeroLoctaion[find.zeroNum/2]>=9)
//					dirr = 2;
//				else
//					dirr = 1;
//		}
		if(find.zeroNum>0)//有断点，返回0
		{
			if(find.zeroContinue==1)//一个分叉
			{
				if(dirr==2)//右分叉
				{
					for(i=numr;i<8;i++)
					{
						if((p>>i)&0x01)
						{
							p = p&(~(0x01<<i));
						}
						else
						{
							i = 8;
						}
					}
					goto AA;
				}
				else if(dirr==1)//左分叉
				{
					for(i=numl;i<8;i++)
					{
						if((p<<i)&0x80)
						{
							p = p&(~(0x80>>i));
						}
						else
						{
							i = 8;
						}
					}
					goto AA;
				}
				else
				{
					find.Error = 1;
				}
			}
			
		}
		else//正常情况
		{
			if(find.Num<=num)
			{
				find.Distance = numl - numr;
			}
			else
			{
				find.Error = 1;
			}
		}
	}
	
	findtemp = find;
	return find;

}












float Cdh1_Inc;
float Cdh2_Inc;
float Cdh3_Inc;
float Cdh4_Inc;

PIDspeed PID_speed;
//磁导航PID调节
void PID_AUTO_QianLun(u16 j_speed,float kp,float ki,float kd)
{
	u8 num = 0;
	PID.Kp = j_speed * kp /1000;
	PID.Ki = j_speed * ki /1000;
	PID.Kd = kd; 
	
	PID.SetTarget = 0;		//PID目标值
	
	if(g_AGV_Sta.Car_dir == 0)
	{
		while( g_CDH8_qian_1.Num==0 && num<100 )				//空点检测0.5秒
		{
			num++;
			delay_rtos(0,0,0,10);
		}			
		if(g_CDH8_qian_1.Num==0)							//未检出到磁条停止
		{
			AGV_System_Stop();					//停止
			HmiTaskState = 4;								//触摸屏显示车辆停止
			speek("脱离轨道");
			delay_rtos(0,0,0,500);
		
		
		}
		else
		{
			HmiTaskState = 5;								//正在运行
			
			
			Cdh1_Inc = IncPIDCalc(g_CDH8_qian_1.Distance);	//1 位置式PID//输入差值						
			PID_speed.moter1_speed = j_speed+Cdh1_Inc;
			PID_speed.moter2_speed = j_speed-Cdh1_Inc;
			
			if(PID_speed.moter1_speed>j_speed)	PID_speed.moter1_speed = j_speed;
			if(PID_speed.moter2_speed>j_speed)	PID_speed.moter2_speed = j_speed;


			if(PID_speed.moter1_speed<0)	PID_speed.moter1_speed = 0;
			if(PID_speed.moter2_speed<0)	PID_speed.moter2_speed = 0;

			LunZi_Go(1,PID_speed.moter1_speed);LunZi_Go(2,PID_speed.moter2_speed);
			
						
		}
	}
	else if(g_AGV_Sta.Car_dir == 1)					//后退
	{
		while(g_CDH8_qian_2.Num==0 && num!=100)		//空点检测0.5秒
		{
			num++;
			delay_rtos(0,0,0,10);
		}
		if(g_CDH8_qian_2.Num==0)
		{
			AGV_System_Stop();					//停止
			HmiTaskState = 4;					//无磁条停止
			speek("脱离轨道");
			delay_rtos(0,0,0,500);
		}
		else
		{
			HmiTaskState = 5;					//正在运行
			

			
			Cdh2_Inc = IncPIDCalc(g_CDH8_qian_2.Distance);//2 位置式PID//输入差值
			PID_speed.moter2_speed = j_speed+Cdh2_Inc;	
			PID_speed.moter1_speed = j_speed-Cdh2_Inc;
							
			
			if(PID_speed.moter1_speed>j_speed)	PID_speed.moter1_speed = j_speed;
			if(PID_speed.moter2_speed>j_speed)	PID_speed.moter2_speed = j_speed;

			if(PID_speed.moter1_speed<0)	PID_speed.moter1_speed = 0;
			if(PID_speed.moter2_speed<0)	PID_speed.moter2_speed = 0;

			LunZi_Back(2,PID_speed.moter2_speed);LunZi_Back(1,PID_speed.moter1_speed);
			
					
		}

	}
}

void PID_AUTO_HouLun(u16 j_speed2,float kp2,float ki2,float kd2)	//后轮的PID调节
{
	u8 num2 = 0;
	PID2.Kp = j_speed2 * kp2 /1000;
	PID2.Ki = j_speed2 * ki2 /1000;
	PID2.Kd = kd2; 

	
	PID2.SetTarget = 0;		//PID2目标值
	
	if(g_AGV_Sta.Car_dir == 0)	//前进
	{
		while( g_CDH8_hou_1.Num==0 && num2<50 )				//空点检测0.5秒
		{
			num2++;
			delay_rtos(0,0,0,10);
		}			
		if(g_CDH8_hou_1.Num==0)							//未检出到磁条停止
		{
			AGV_System_Stop();							//停止
			HmiTaskState = 4;								//触摸屏显示车辆停止
		
		
			speek("脱离轨道");
			delay_rtos(0,0,0,500);
		}
		else
		{
			HmiTaskState = 5;								//正在运行
			
			Cdh3_Inc = IncPIDCalc2(g_CDH8_hou_1.Distance);	//1 位置式PID//输入差值						
			PID_speed.moter3_speed = j_speed2+Cdh3_Inc;
			PID_speed.moter4_speed = j_speed2-Cdh3_Inc;
			
			if(PID_speed.moter3_speed>j_speed2)	PID_speed.moter3_speed = j_speed2;
			if(PID_speed.moter4_speed>j_speed2)	PID_speed.moter4_speed = j_speed2;


			if(PID_speed.moter3_speed<0)	PID_speed.moter3_speed = 0;
			if(PID_speed.moter4_speed<0)	PID_speed.moter4_speed = 0;

			LunZi_Go(3,PID_speed.moter3_speed);LunZi_Go(4,PID_speed.moter4_speed);
			
		}
	}
	else if(g_AGV_Sta.Car_dir == 1)					//后退
	{
		while(g_CDH8_hou_2.Num==0 && num2!=50)		//空点检测0.5秒
		{
			num2++;
			delay_rtos(0,0,0,10);
		}
		if(g_CDH8_hou_2.Num==0)
		{
			AGV_System_Stop();
			HmiTaskState = 4;					//无磁条停止
				speek("脱离轨道");
			delay_rtos(0,0,0,500);
		}
		else
		{
			HmiTaskState = 5;					//正在运行
			

			
			Cdh4_Inc = IncPIDCalc2(g_CDH8_hou_2.Distance);//2 位置式PID//输入差值
			PID_speed.moter4_speed = j_speed2+Cdh4_Inc;	
			PID_speed.moter3_speed = j_speed2-Cdh4_Inc;
							
			
			if(PID_speed.moter4_speed>j_speed2)	PID_speed.moter4_speed = j_speed2;
			if(PID_speed.moter3_speed>j_speed2)	PID_speed.moter3_speed = j_speed2;

			if(PID_speed.moter4_speed<0)	PID_speed.moter4_speed = 0;
			if(PID_speed.moter3_speed<0)	PID_speed.moter3_speed = 0;
	
			LunZi_Back(4,PID_speed.moter4_speed);LunZi_Back(3,PID_speed.moter3_speed);
			
					
		}

	}
}








int DWQ1_Inc;
int DWQ2_Inc;
int DWQ3_Inc;
int DWQ4_Inc;

PIDspeed PID__SD_speed;
u8 g_Trun_Val = 10;			//手动转向角度
void PID_SD_Adjust(u16 j_speed,float kp,u16 ki,float kd)
{
	PID2.Kp = PID.Kp = j_speed * kp /1000;
	PID2.Ki = PID.Ki = j_speed * ki /1000; 
	PID2.Kd = PID.Kd = kd; 
	
	//0:停止1:前进 2:后退 3:左转 4:右转 5左上 6右上 7左下 8右下 9左旋 10右旋 	
	
	HmiTaskState = 5;								//正在运行 -- 手动

	switch(g_AGV_yaokong.SD_ir)	//设定目标值
	{
		
		case 1:					//前进
			PID.SetTarget  = g_DWQ.qianlun_zhong_val + g_AGV_yaokong.jiaodu;		//PID目标值--前_驱动轮电位器中值 
			PID2.SetTarget = g_DWQ.houlun_zhong_val;		//PID目标值--后_驱动轮电位器中值 						
			break;	
		
		case 2:					//后退
			PID2.SetTarget = g_DWQ.houlun_zhong_val + g_AGV_yaokong.jiaodu;		//PID目标值--后_驱动轮电位器中值 	
			PID.SetTarget  = g_DWQ.qianlun_zhong_val;		//PID目标值--前_驱动轮电位器中值 
				break;	
				
		default:
			break;
	}	
	
	
	

	switch(g_AGV_yaokong.SD_ir)	//实际控制电机的速度			
	{
		case 1:					//前进

			DWQ1_Inc = IncPIDCalc(g_After_filter[1]);		//1 位置式PID//输入差值	 0-4096				
			PID__SD_speed.moter1_speed = j_speed+DWQ1_Inc;
			PID__SD_speed.moter2_speed = j_speed-DWQ1_Inc;
			
			DWQ2_Inc = IncPIDCalc2(g_After_filter[2]);		//2 位置式PID//输入差值						
			PID__SD_speed.moter3_speed = j_speed+DWQ2_Inc;
			PID__SD_speed.moter4_speed = j_speed-DWQ2_Inc;
			break;
		
		case 2:					//后退

			DWQ2_Inc = IncPIDCalc2(g_After_filter[2]);		//1 位置式PID//输入差值	 0-4096				
			PID__SD_speed.moter4_speed = j_speed+DWQ2_Inc;
			PID__SD_speed.moter3_speed = j_speed-DWQ2_Inc;
			
			DWQ1_Inc = IncPIDCalc(g_After_filter[1]);		//2 位置式PID//输入差值						
			PID__SD_speed.moter2_speed = j_speed+DWQ1_Inc;
			PID__SD_speed.moter1_speed = j_speed-DWQ1_Inc;
			break;			
		
		default:
			break;

	}
	

	
	if(PID__SD_speed.moter1_speed > 300*AGV_SYS.SD_Speed_bili)	PID__SD_speed.moter1_speed = 300*AGV_SYS.SD_Speed_bili;
	if(PID__SD_speed.moter2_speed > 300*AGV_SYS.SD_Speed_bili)	PID__SD_speed.moter2_speed = 300*AGV_SYS.SD_Speed_bili;
	if(PID__SD_speed.moter3_speed > 300*AGV_SYS.SD_Speed_bili)	PID__SD_speed.moter3_speed = 300*AGV_SYS.SD_Speed_bili;
	if(PID__SD_speed.moter4_speed > 300*AGV_SYS.SD_Speed_bili)	PID__SD_speed.moter4_speed = 300*AGV_SYS.SD_Speed_bili;


	if(PID__SD_speed.moter1_speed<0)	PID__SD_speed.moter1_speed = 0;
	if(PID__SD_speed.moter2_speed<0)	PID__SD_speed.moter2_speed = 0;
	if(PID__SD_speed.moter3_speed<0)	PID__SD_speed.moter3_speed = 0;
	if(PID__SD_speed.moter4_speed<0)	PID__SD_speed.moter4_speed = 0;


	switch(g_AGV_yaokong.SD_ir)	//实际控制电机的速度			
	{
		case 1:					//前进
		case 5:					//前进左拐	
		case 6:					//前进右拐

			LunZi_Go(1,PID__SD_speed.moter1_speed);LunZi_Go(2,PID__SD_speed.moter2_speed);
			LunZi_Go(3,PID__SD_speed.moter3_speed);LunZi_Go(4,PID__SD_speed.moter4_speed);
		
		
			break;
		
		case 2:					//后退
		case 8:					//后退左拐	
		case 7:					//后退右拐
			
			LunZi_Back(4,PID__SD_speed.moter4_speed);LunZi_Back(3,PID__SD_speed.moter3_speed);		
			LunZi_Back(2,PID__SD_speed.moter2_speed);LunZi_Back(1,PID__SD_speed.moter1_speed);	
		
			break;			
		
		default:
			break;

	}

}

//遥控器线
//1正2负3H4L
//
