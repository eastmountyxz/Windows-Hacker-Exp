 
#ifndef _SKINPLUSPLUS_H_
#define _SKINPLUSPLUS_H_

#ifdef SKINPLUSPLUSDLL
	#define SKINPLUSPLUS_DLL_DECLARE  _declspec(dllexport)
#else
	#define SKINPLUSPLUS_DLL_DECLARE  _declspec(dllimport)
	#pragma comment(lib,"SkinPlusPlusDLL.lib") 
	#pragma message("Automatically linking with SkinPlusPlusDLL.dll") 
#endif

#pragma pack(push, 8)
///////////////////////////////////////////////////////////////////////////
//Function: 
//Parameters: 
//Return Values:
//Remarks:

#define WM_TOOLBARPAINTPRE (WM_USER + 802)
#define WM_TOOLBARPAINTEND (WM_USER + 803)

#define WM_GETSPLITINFO (WM_USER + 804)

struct CRowColInfo
{
	int nMinSize;     
	int nIdealSize;   
	int nCurSize;
};

struct SplitInfo{
	HWND hWnd;
	int nMaxRows;
	int nMaxCols;
	int cxSplitter;
	int cySplitter;   
	int cxBorderShare;
	int cyBorderShare;
	int cxSplitterGap;
	int cySplitterGap;
	int cxBorder;
	int cyBorder;

	// current state information
	int nRows;
	int nCols;
	BOOL bHasHScroll;
	BOOL bHasVScroll;
	CRowColInfo* pColInfo;
	CRowColInfo* pRowInfo;
};

enum BUTTONSTATE 
{
	NORMAL	= 0,
	PRESS	= 1,
	DISABLE	= 2,
	HOT		= 3,
	FOCUS   = 4,
	LAST	= 5
};

enum DRAWTYPE
{
	BUTTON = 0,
	SCROLLARROWUP = 1,
	SCROLLARROWDOWN = 2,
	SPLITTERBARHORZ = 3,
	SPLITTERBARVERT = 4,
	LISTHEAD = 5,
};

SKINPLUSPLUS_DLL_DECLARE BOOL  __stdcall InitializeSkin(char* SkinFile);

SKINPLUSPLUS_DLL_DECLARE BOOL  __stdcall ExitSkin();

SKINPLUSPLUS_DLL_DECLARE BOOL  __stdcall LoadSkin(char* SkinFile);

SKINPLUSPLUS_DLL_DECLARE BOOL  __stdcall RemoveSkin();

SKINPLUSPLUS_DLL_DECLARE BOOL  __stdcall SetMenuItemImage(UINT nID, HIMAGELIST hImgList, int nIndex);

SKINPLUSPLUS_DLL_DECLARE HMENU __stdcall GetMainFrameMenu(void);

SKINPLUSPLUS_DLL_DECLARE void  __stdcall DrawSkinObject(HDC hdc,RECT rect,DRAWTYPE eDrawType,int nState);

SKINPLUSPLUS_DLL_DECLARE void  __stdcall DrawSkinBitmap(HDC hdc,RECT rect,HBITMAP hBitmap,int nTopHeight = 0,int nBottomHeight = 0,
													    int nLeftWidth = 0,int nRightWidth = 0,COLORREF colTrans = -1,BOOL bTrans = FALSE);

SKINPLUSPLUS_DLL_DECLARE COLORREF __stdcall GetSkinSysColor(int nColorIndex);

SKINPLUSPLUS_DLL_DECLARE COLORREF __stdcall GetDefaultSysColor(int nColorIndex);

SKINPLUSPLUS_DLL_DECLARE void __stdcall SetNoSkinHwnd(HWND hWnd);

typedef int (__stdcall * PGETSPLITTERINFO)(SplitInfo& si);

SKINPLUSPLUS_DLL_DECLARE void __stdcall GetSplitterInfo(PGETSPLITTERINFO func);

extern PGETSPLITTERINFO  g_CallBack;

//Splitter
#define SKINPLUSPLUS_SPLITTER() \
	int CALLBACK GetSplitterInfoProc(SplitInfo& si); \
	class CExSplitterWnd : public CSplitterWnd \
	{friend int CALLBACK GetSplitterInfoProc(SplitInfo& si);}; \
	CExSplitterWnd*	g_wndSplitter = NULL; \
	int CALLBACK GetSplitterInfoProc(SplitInfo& si) \
	{ \
		si.nMaxRows = g_wndSplitter->m_nMaxRows; \
		si.nMaxCols = g_wndSplitter->m_nMaxCols; \
		si.cxSplitter = g_wndSplitter->m_cxSplitter; \
		si.cySplitter = g_wndSplitter->m_cySplitter; \
		si.cxBorderShare = g_wndSplitter->m_cxBorderShare; \
		si.cyBorderShare = g_wndSplitter->m_cyBorderShare; \
		si.cxSplitterGap = g_wndSplitter->m_cxSplitterGap; \
		si.cySplitterGap = g_wndSplitter->m_cySplitterGap; \
		si.cxBorder = g_wndSplitter->m_cxBorder; \
		si.cyBorder = g_wndSplitter->m_cyBorder; \
		si.nRows = g_wndSplitter->m_nRows; \
		si.nCols = g_wndSplitter->m_nCols; \
		si.bHasHScroll = g_wndSplitter->m_bHasHScroll; \
		si.bHasVScroll = g_wndSplitter->m_bHasVScroll; \
		si.pColInfo = (CRowColInfo *)g_wndSplitter->m_pColInfo; \
		si.pRowInfo = (CRowColInfo *)g_wndSplitter->m_pRowInfo; \
		return 0; \
	} \

#define SKINPLUSPLUS_INIT_SPLITTER(classname) \
	g_wndSplitter = (CExSplitterWnd *)&classname;\
	GetSplitterInfo(GetSplitterInfoProc);\
//Splitter
#pragma pack(pop)
#endif
