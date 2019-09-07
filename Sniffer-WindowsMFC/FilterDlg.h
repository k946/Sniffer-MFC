#pragma once


// FILFER 对话框
#include <map>

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

	afx_msg void OnBnClickedFilterOther2();
};
