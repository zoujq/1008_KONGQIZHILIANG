#include "sem.h"
//#include "m_include.h"

 u8   SEM[MAX_SEM];
 BOOL MUTEX[MAX_MUTEX];
/********************************************//**
 * \brief    ��ȡ�ź���
 *
 * \param
 * \param
 * \return
 *
 ***********************************************/
BOOL getSem(u8* k)
{
    if(*k)
    {
        *k = 0;
        return true;
    }

    return false;
}
