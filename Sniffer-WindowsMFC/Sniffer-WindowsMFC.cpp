
// Sniffer-WindowsMFC.cpp: 定义应用程序的类行为。
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "Sniffer-WindowsMFC.h"
#include "MainFrm.h"
#include "FilterDlg.h"
#include "ChoiceDevs.h"
#include "SnifferThread.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSnifferWindowsMFCApp
BEGIN_MESSAGE_MAP(CSnifferWindowsMFCApp, CWinApp)
	ON_COMMAND(ID_BAR_START, OnBarStart)
	//ON_UPDATE_COMMAND_UI(ID_BAR_START, OnBarStart)
	ON_COMMAND(ID_BAR_OPEN, OnBarOpen)
	//ON_UPDATE_COMMAND_UI(ID_BAR_OPEN, OnBarOpen)
	ON_COMMAND(ID_BAR_SAVE, OnBarSave)
	//ON_UPDATE_COMMAND_UI(ID_BAR_SAVE, OnBarSave)
	ON_COMMAND(ID_BAR_END, OnBarEnd)
	//ON_UPDATE_COMMAND_UI(ID_BAR_END, OnBarEnd)
	ON_COMMAND(ID_BAR_OPEN_FILTER, OnSwitchFilter)
	ON_COMMAND(ID_BAR_FILTER_LIST, OnFilterList)
	//ON_UPDATE_COMMAND_UI(ID_BAR_START, OnBarStart)
	ON_COMMAND(ID_APP_EXIT, &CSnifferWindowsMFCApp::OnAppExit)
END_MESSAGE_MAP()

// CSnifferWindowsMFCApp 构造

CSnifferWindowsMFCApp::CSnifferWindowsMFCApp() noexcept
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// 如果应用程序是利用公共语言运行时支持(/clr)构建的，则: 
	//     1) 必须有此附加设置，“重新启动管理器”支持才能正常工作。
	//     2) 在您的项目中，您必须按照生成顺序向 System.Windows.Forms 添加引用。
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: 将以下应用程序 ID 字符串替换为唯一的 ID 字符串；建议的字符串格式
	//为 CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("SnifferWindowsMFC.AppID.NoVersion"));

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的 CSnifferWindowsMFCApp 对象

CSnifferWindowsMFCApp theApp;

// CSnifferWindowsMFCApp 初始化

BOOL CSnifferWindowsMFCApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。  否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// 使用 RichEdit 控件需要 AfxInitRichEdit2()
	// AfxInitRichEdit2();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));


	// 若要创建主窗口，此代码将创建新的框架窗口
	// 对象，然后将其设置为应用程序的主窗口对象
	CFrameWnd* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;

	// 创建并加载框架及其资源
	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, nullptr,
		nullptr);

	{
		
	}

	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	return TRUE;
}

int CSnifferWindowsMFCApp::ExitInstance()
{
	//TODO: 处理可能已添加的附加资源
	AfxOleTerm(FALSE);

	return CWinApp::ExitInstance();
}

// CSnifferWindowsMFCApp 消息处理程序


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() noexcept;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// 用于运行对话框的应用程序命令
void CSnifferWindowsMFCApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CSnifferWindowsMFCApp 消息处理程序

//开始嗅探
void CSnifferWindowsMFCApp::OnBarStart() {

	if (NULL == m_curDev) {
		ChoiceDevs::GetDefaultDev(m_curDev);
		if( NULL == m_curDev)
			::AfxMessageBox((CString)"适配器开启失败");
	}

	if (m_isSniffer == FALSE) {
		m_packListCtrl->ClearList();
		for (int i = 0; i < m_allPacket.size(); ++i) {
			delete [] m_allPacket[i].pkt_data;
			delete m_allPacket[i].pkt_header;
		}
		m_allPacket.clear();
		m_isSniffer = TRUE;
		::AfxBeginThread(SnifferThread::MyThreadFunction, m_curDev);
	}
}

void CSnifferWindowsMFCApp::OnBarOpen() {

}

void CSnifferWindowsMFCApp::OnBarSave() {

}

//结束嗅探
void CSnifferWindowsMFCApp::OnBarEnd() {
	if (m_isSniffer == TRUE) {
		pcap_breakloop(m_curDev);
		m_isSniffer = FALSE;
	}
	
}

//开启过滤
void CSnifferWindowsMFCApp::OnSwitchFilter() {
	m_haveFilter = !m_haveFilter;
}

//设置过滤
void CSnifferWindowsMFCApp::OnFilterList() {

	FilterDlg filterDlg;
	filterDlg.DoModal();
}






void CSnifferWindowsMFCApp::OnAppExit()
{	
	if(m_curDev != NULL)
		pcap_close(m_curDev);

	for (int i = 0; i < m_allPacket.size(); ++i) {
		delete[] m_allPacket[i].pkt_data;
		delete m_allPacket[i].pkt_header;
	}
	m_allPacket.clear();
	m_packHexViewCtrl->DestroyWindow();
	m_packInfoViewCtrl->DestroyWindow();
	m_packListCtrl->DestroyWindow();
	exit(0);
	// TODO: 在此添加命令处理程序代码
}
