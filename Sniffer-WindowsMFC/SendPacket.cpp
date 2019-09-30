// SendPacket.cpp: 实现文件
//

#include "stdafx.h"
#include "Sniffer-WindowsMFC.h"
#include "SendPacket.h"
#include "afxdialogex.h"
#include "PackHexAnalysis.h"


// SendPacket 对话框

IMPLEMENT_DYNAMIC(SendPacket, CDialog)

SendPacket::SendPacket(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_SendPacket, pParent)
{

}

SendPacket::~SendPacket()
{
}

void SendPacket::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_hexView);
}


BEGIN_MESSAGE_MAP(SendPacket, CDialog)
	ON_BN_CLICKED(IDOK, &SendPacket::OnBnClickedOk)
END_MESSAGE_MAP()


// SendPacket 消息处理程序


void SendPacket::OnBnClickedOk()
{
	CString temp;
	int len;
	// TODO: 在此添加控件通知处理程序代码
	m_hexView.GetWindowTextW(temp);
	len = temp.GetLength() / 3 + 1;
	u_char* pkt_data = PackHexAnalysis::hexToChar(temp);
	temp = PackHexAnalysis::charToHex(pkt_data, len);
	::AfxMessageBox(temp);
	sendPacket(pkt_data, len);
	CDialog::OnOK();
}


BOOL SendPacket::OnInitDialog()
{
	CDialog::OnInitDialog();

	//字体
	CFont *font;
	LOGFONT lf;
	font = new CFont();
	font->CreatePointFont(115, (LPCTSTR)"新宋体");
	font->GetLogFont(&lf);
	//lf.lfUnderline=TRUE; 
	font->DeleteObject();
	font->CreateFontIndirect(&lf);

	m_hexView.SetFont(font);
	m_hexView.SetFocus();

	delete font;


	CString hex;
	u_char * pkt_data;
	int len;
	if (theApp.m_nIndex >= 0 && !theApp.m_allPacket.empty()) {
		pkt_data = theApp.m_allPacket[theApp.m_nIndex].pkt_data;
		len = theApp.m_allPacket[theApp.m_nIndex].pkt_header->len;
		hex += PackHexAnalysis::charToHex(pkt_data, len);
		m_hexView.SetWindowTextW(hex);
	}


	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void SendPacket::sendPacket(u_char* pkt_data, int len) {
	pcap_sendpacket(theApp.m_curDev, pkt_data, len);
}
