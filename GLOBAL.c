
#include"GLOBAL.h"
uint32_t g_uiSysFlags; //global variable, used as a 32bit flag register.
void Init_Flags(void)
{
    g_uiSysFlags = 0;
}
void Set_Flag(uint32_t uiSysFlag)
{
    BBA( &g_uiSysFlags, uiSysFlag ) = 1;
}
void Clear_Flag(uint32_t uiSysFlag)
{
    BBA( &g_uiSysFlags, uiSysFlag ) = 0;
}
bool Check_Flag(uint32_t uiSysFlag)
{
    return BBA(&g_uiSysFlags, uiSysFlag);
}

