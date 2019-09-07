#pragma once


// SendPacket 对话框

class SendPacket : public CDialog
{
	DECLARE_DYNAMIC(SendPacket)

public:
	SendPacket(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~SendPacket();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SendPacket };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 十六进制编辑框
	CEdit m_hexView;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	static void sendPacket(u_char* pkt_data, int len);
};
