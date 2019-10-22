#pragma once

#include <vector>
// ChoiceDevs 对话框
#define IPTOSBUFFERS 12

class ChoiceDevs : public CDialogEx
{
	DECLARE_DYNAMIC(ChoiceDevs)

public:
	ChoiceDevs(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~ChoiceDevs();


// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHOICE_DEVS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_combo;	//下拉栏绑定变量
	pcap_if_t* m_allDevs;
	afx_msg void OnBnClickedOk();
	static void GetDefaultDev(pcap_t * & param);
};
