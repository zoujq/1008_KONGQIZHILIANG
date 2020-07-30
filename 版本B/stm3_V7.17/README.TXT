本代码为战舰STM32开发板综合实验，详细介绍，请看《STM32开发指南》第六十一章。


注意：  
本实验请屏蔽stm32f10x_it.h里面的如下代码：
void NMI_Handler(void)        __attribute__ ((alias("NMIException")));
void HardFault_Handler(void)  __attribute__ ((alias("HardFaultException")));
void MemManage_Handler(void)  __attribute__ ((alias("MemManageException")));
void BusFault_Handler(void)   __attribute__ ((alias("BusFaultException")));
void UsageFault_Handler(void) __attribute__ ((alias("UsageFaultException")));
void DebugMon_Handler(void)   __attribute__ ((alias("DebugMonitor")));
void SVC_Handler(void)        __attribute__ ((alias("SVCHandler")));
void PendSV_Handler(void)     __attribute__ ((alias("PendSVC")));
void SysTick_Handler(void)    __attribute__ ((alias("SysTickHandler")));
将以上代码注释掉！！！


          	正点原子@ALIENTEK
               	2012-10-8
		广州市星翼电子科技有限公司
                联系电话（传真）：020-38271790
	       	购买：http://shop62103354.taobao.com
                      http://shop62057469.taobao.com
               	技术支持论坛：www.openedv.com
           