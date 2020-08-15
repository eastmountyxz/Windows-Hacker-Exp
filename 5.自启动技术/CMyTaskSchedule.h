//CMyTaskSchedule.h

#pragma once

#include <taskschd.h>
#pragma comment(lib, "taskschd.lib")

class CMyTaskSchedule
{
public:
    CMyTaskSchedule();
    ~CMyTaskSchedule();

    //************************************
    // 函数名:  CMyTaskSchedule::NewTask
    // 返回类型:   BOOL
    // 功能: 创建计划任务
    // 参数1: char * lpszTaskName    计划任务名
    // 参数2: char * lpszProgramPath    计划任务路径
    // 参数3: char * lpszParameters        计划任务参数
    // 参数4: char * lpszAuthor            计划任务作者
    //************************************
    BOOL NewTask(char* lpszTaskName, char* lpszProgramPath, char* lpszParameters, char* lpszAuthor);

    //************************************
    // 函数名:  CMyTaskSchedule::Delete
    // 返回类型:   BOOL
    // 功能: 删除计划任务
    // 参数1: char * lpszTaskName    计划任务名
    //************************************
    BOOL Delete(char* lpszTaskName);

private:
    ITaskService *m_lpITS;
    ITaskFolder *m_lpRootFolder;
};
