#include <stdio.h>
#include <windows.h>
#include <iostream>
using namespace std;

//判断是否重复运行
BOOL IsAlreadyRun()
{
	//创建互斥对象
	HANDLE hMutex = NULL;
	hMutex = ::CreateMutex(NULL, FALSE, "TEST Eastmount");

	//如果互斥对象创建成功则返回命名互斥对象存在 表示程序重复运行
	if (hMutex) {
		if (ERROR_ALREADY_EXISTS == ::GetLastError()) {
			return TRUE;
		}
	}
}

int main()
{
	//判断是否重复运行
	if (IsAlreadyRun()) {
		printf("Already RUN!!!\n");
	}
	else {
		printf("NOT Already RUN!!!\n");
	}
	Sleep(100000);
	
	//system("pause");
	return 0;
}
