#pragma once

#define MYWINDAPIEXPORT __declspec(dllexport)

//定义为全局HOOK 返回的钩子过程
HHOOK g_HookProc;                   

//设置HOOK钩子 
void MYWINDAPIEXPORT SetHook();     

//取消设置HOOK
void MYWINDAPIEXPORT UnHook();      

//设置HOOK过程中需要的回调函数
LRESULT CALLBACK MyProc(int nCode, WPARAM wParam, LPARAM lParam); 
