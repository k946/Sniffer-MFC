#pragma once


// FILFER 对话框
#include <map>

static std::vector<CString>	filter_IPAddress;	//需要过滤的IP地址
static std::vector<CString>	filter_Protocol;		//需要过滤的协议

class FilterDlg : public CDialog
{
	DECLARE_DYNAMIC(FilterDlg)

public:
	FilterDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~FilterDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FILTER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();				//点击确定按钮是调用
	void AnalysisFilterList(CString & filterList);
	static bool Filter(const CString &ip, const CString &protocol);
	afx_msg void OnBnClickedFilterOther2();
};
