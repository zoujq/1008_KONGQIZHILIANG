#ifndef __SYSTICK_H__
#define __SYSTICK_H__
/*   MyPt V1.1 HeT WangDong 2017-08-18 */
#include "typedef.h"
/*--------------------------------------------------------------------------------------------------------------------------------*/
void SysTickInit(void);
void WdogInit(void);
void WdogRst(void);
extern  volatile u32 gU32Systick ;
/*--------------------------------------------------------------------------------------------------------------------------------*/
#endif /* __THREAD_H__ */

/** @} */
/** @} */

