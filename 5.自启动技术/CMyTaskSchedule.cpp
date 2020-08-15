//CMyTaskSchedule.cpp

#include "pch.h"
#include "CMyTaskSchedule.h"

CMyTaskSchedule::CMyTaskSchedule()
{
    m_lpITS = NULL;
    m_lpRootFolder = NULL;
    //初始化COM
    HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr))
    {
        MessageBox(NULL, L"初始化COM接口环境失败!", L"Error", MB_OK);
        return;
    }

    //创建任务服务对象
    hr = CoCreateInstance(CLSID_TaskScheduler, NULL,
        CLSCTX_INPROC_SERVER, IID_ITaskService,
        (LPVOID*)(&m_lpITS));
    if (FAILED(hr))
    {
        MessageBox(NULL, L"创建任务服务对象失败!", L"Error", MB_OK);
        return;
    }

    //连接到任务服务
    hr = m_lpITS->Connect(_variant_t(), _variant_t(), _variant_t(), _variant_t());
    if (FAILED(hr))
    {
        MessageBox(NULL, L"连接到任务服务失败!", L"Error", MB_OK);
        return;
    }

    //获取根任务的指针
    //获取Root Task Folder的指针,这个指针指向的是新注册的任务
    hr = m_lpITS->GetFolder(_bstr_t("\\"), &m_lpRootFolder);
    if (FAILED(hr))
    {
        MessageBox(NULL, L"获取根任务的指针失败!", L"Error", MB_OK);
        return;
    }
}

CMyTaskSchedule::~CMyTaskSchedule()
{
    if (m_lpITS)
    {
        m_lpITS->Release();
    }
    if (m_lpRootFolder)
    {
        m_lpRootFolder->Release();
    }
    // 卸载COM
    CoUninitialize();
}

//创建任务计划
BOOL CMyTaskSchedule::NewTask(char* lpszTaskName, char* lpszProgramPath, char* lpszParameters, char* lpszAuthor)
{
    if (NULL == m_lpRootFolder)
    {
        return FALSE;
    }

    //如果存在相同的计划任务则删除
    Delete(lpszTaskName);

    //创建任务定义对象来创建任务
    ITaskDefinition *pTaskDefinition = NULL;
    HRESULT hr = m_lpITS->NewTask(0, &pTaskDefinition);
    if (FAILED(hr))
    {
        MessageBox(NULL, L"创建任务失败!", L"Error", MB_OK);
        return FALSE;
    }
    //设置注册信息
    IRegistrationInfo *pRegInfo = NULL;
    CComVariant variantAuthor(NULL);
    variantAuthor = lpszAuthor;
    hr = pTaskDefinition->get_RegistrationInfo(&pRegInfo);
    if (FAILED(hr))
    {
        MessageBox(NULL, L"设置注册信息失败!", L"Error", MB_OK);
        return FALSE;
    }
    // 设置作者信息
    hr = pRegInfo->put_Author(variantAuthor.bstrVal);
    pRegInfo->Release();
    // 设置登录类型和运行权限
    IPrincipal *pPrincipal = NULL;
    hr = pTaskDefinition->get_Principal(&pPrincipal);
    if (FAILED(hr))
    {
        MessageBox(NULL, L"设置登录类型和运行权限失败!", L"Error", MB_OK);
        return FALSE;
    }
    // 设置登录类型
    hr = pPrincipal->put_LogonType(TASK_LOGON_INTERACTIVE_TOKEN);
    // 设置运行权限
    // 最高权限
    hr = pPrincipal->put_RunLevel(TASK_RUNLEVEL_HIGHEST);
    pPrincipal->Release();
    // 设置其他信息
    ITaskSettings *pSettting = NULL;
    hr = pTaskDefinition->get_Settings(&pSettting);
    if (FAILED(hr))
    {
        MessageBox(NULL, L"设置其他信息失败!", L"Error", MB_OK);
        return FALSE;
    }
    // 设置其他信息
    hr = pSettting->put_StopIfGoingOnBatteries(VARIANT_FALSE);
    hr = pSettting->put_DisallowStartIfOnBatteries(VARIANT_FALSE);
    hr = pSettting->put_AllowDemandStart(VARIANT_TRUE);
    hr = pSettting->put_StartWhenAvailable(VARIANT_FALSE);
    hr = pSettting->put_MultipleInstances(TASK_INSTANCES_PARALLEL);
    pSettting->Release();
    // 创建执行动作
    IActionCollection *pActionCollect = NULL;
    hr = pTaskDefinition->get_Actions(&pActionCollect);
    if (FAILED(hr))
    {
        MessageBox(NULL, L"创建执行动作失败!", L"Error", MB_OK);
        return FALSE;
    }
    IAction *pAction = NULL;
    // 创建执行操作
    hr = pActionCollect->Create(TASK_ACTION_EXEC, &pAction);
    pActionCollect->Release();
    // 设置执行程序路径和参数
    CComVariant variantProgramPath(NULL);
    CComVariant variantParameters(NULL);
    IExecAction *pExecAction = NULL;
    hr = pAction->QueryInterface(IID_IExecAction, (PVOID *)(&pExecAction));
    if (FAILED(hr))
    {
        pAction->Release();
        MessageBox(NULL, L"创建执行动作失败!", L"Error", MB_OK);
        return FALSE;
    }
    pAction->Release();
    // 设置程序路径和参数
    variantProgramPath = lpszProgramPath;
    variantParameters = lpszParameters;
    pExecAction->put_Path(variantProgramPath.bstrVal);
    pExecAction->put_Arguments(variantParameters.bstrVal);
    pExecAction->Release();
    // 设置触发器信息，包括用户登录时触发
    ITriggerCollection *pTriggers = NULL;
    hr = pTaskDefinition->get_Triggers(&pTriggers);
    if (FAILED(hr))
    {
        MessageBox(NULL, L"设置触发器信息失败!", L"Error", MB_OK);
        return FALSE;
    }
    // 创建触发器
    ITrigger *pTrigger = NULL;
    hr = pTriggers->Create(TASK_TRIGGER_LOGON, &pTrigger);
    if (FAILED(hr))
    {
        MessageBox(NULL, L"创建触发器失败!", L"Error", MB_OK);
        return FALSE;
    }
    // 注册任务计划
    IRegisteredTask *pRegisteredTask = NULL;
    CComVariant variantTaskName(NULL);
    variantTaskName = lpszTaskName;
    hr = m_lpRootFolder->RegisterTaskDefinition(variantTaskName.bstrVal,
        pTaskDefinition,
        TASK_CREATE_OR_UPDATE,
        _variant_t(),
        _variant_t(),
        TASK_LOGON_INTERACTIVE_TOKEN,
        _variant_t(""),
        &pRegisteredTask);
    if (FAILED(hr))
    {
        pTaskDefinition->Release();
        MessageBox(NULL, L"注册任务计划失败!", L"Error", MB_OK);
        return FALSE;
    }
    pTaskDefinition->Release();
    pRegisteredTask->Release();
    return TRUE;
}

//删除计划任务
BOOL CMyTaskSchedule::Delete(char* lpszTaskName)
{
    if(NULL == m_lpRootFolder)
    {
        return FALSE;
    }
    CComVariant variantTaskName(NULL);
    variantTaskName = lpszTaskName;
    HRESULT hr = m_lpRootFolder->DeleteTask(variantTaskName.bstrVal, 0);
    if (FAILED(hr))
    {
        return FALSE;
    }
    return TRUE;
}