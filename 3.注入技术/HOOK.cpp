#include <Windows.h>
#include "HOOK.H"

//设置HOOK
void MYWINDAPIEXPORT SetHook()
{
    //参数：HOOK的类型 Hook的回调地址 模块句柄 线程ID(0代表是全局钩子)
    g_HookProc = ::SetWindowsHookEx(WH_CBT, MyProc, 
        GetModuleHandle(TEXT("GlobalHook_Test.dll")), 0); 
}

//取消HOOK
void MYWINDAPIEXPORT UnHook()                                                                    
{
    if (NULL != g_HookProc) {
        ::UnhookWindowsHookEx(g_HookProc);
    }
}

//回调函数 处理程序
LRESULT CALLBACK MyProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    //执行我们的程序
    MessageBox(NULL, "加载钩子!", "提示", MB_OK);
    //继续调用钩子过程
    return CallNextHookEx(g_HookProc, nCode, wParam, lParam);
}