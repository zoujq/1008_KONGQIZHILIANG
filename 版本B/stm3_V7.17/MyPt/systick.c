#include "systick.h"
#include "timer.h"
//#include "m_include.h"
/*---------------------------------------------------------------------------*/
/** \brief
 *
 * \param
 * \param
 * \return
 *
 */
 volatile u32 gU32Systick=0;
/*---------------------------------------------------------------------------*/
/** \brief  systick init
 *
 * \param
 * \param
 * \return
 *
 */

void SysTickInit(void)
{
    /* SYSTICK configuration */
    
    WdogInit();
    //
    //1ms
    DI();
    //add you code here

		// Tim0_Init();
		TIM3_Int_Init(9,7199);//10Khz的计数频率，计数到5000为500ms  //1ms

    EI();
}

/*---------------------------------------------------------------------------*/
/** \brief  watchdog init
 *
 * \param
 * \param
 * \return
 *
 */
void WdogInit(void)
{
   //add you code here
}
/*---------------------------------------------------------------------------*/
/** \brief  watchdog reset
 *
 * \param
 * \param
 * \return
 *
 */
void WdogRst(void)
{
    //add you code here
}
