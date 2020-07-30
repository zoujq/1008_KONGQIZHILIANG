#ifndef  __USER_IAQ2000_H__
#define  __USER_IAQ2000_H__


//extern 	volatile unsigned long	uTVOC;//

extern 	volatile unsigned int	uHCHO_S_Former_TVOC;//甲醛浓度值,//配合TVOC


 void User_IAQ2000_Read(void);//空气质量
 void User_IAQ2000_Init(void);//空气质量
 


#endif
