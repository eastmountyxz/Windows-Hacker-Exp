// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:     //动态库装载时调用
        //保存DLL模块句柄
        g_Inst = (HINSTANCE)hModule;
        MessageBox(NULL, TEXT("加载DLL!"), TEXT("提示"), MB_OK);
        break;
    case DLL_THREAD_ATTACH:     //进程中有线程创建时调用
    case DLL_THREAD_DETACH:     //进程中有线程结束时调用
    case DLL_PROCESS_DETACH:    //动态库卸载是调用
        break;
    }
    return TRUE;
}

//钩子回调函数
LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam)
{
    //如果code小于0必须调用CallNextHookEx传递消息
    if (code < 0) {
        return CallNextHookEx(g_Hook, code, wParam, lParam);
    }

    //如果code等于HC_ACTION表示消息中包含按键消息
    //如果为WM_KEYDOWN则显示按键对应的文本
    if (code == HC_ACTION && lParam > 0) {
        char szBuf[MAXBYTE] = { 0 };
        GetKeyNameText(lParam, szBuf, MAXBYTE);
        MessageBox(NULL, szBuf, "提示", MB_OK);
    }
    return CallNextHookEx(g_Hook, code, wParam, lParam);
}

//安装钩子
VOID SetHookOn()
{
    g_Hook = SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, g_Inst, 0);
}

//卸载钩子
VOID SetHookOff()
{
    UnhookWindowsHookEx(g_Hook);
}

