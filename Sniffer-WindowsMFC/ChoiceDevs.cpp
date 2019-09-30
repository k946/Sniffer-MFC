// ChoiceDevs.cpp: 实现文件
//

#include "stdafx.h"
#include "Sniffer-WindowsMFC.h"
#include "ChoiceDevs.h"
#include "afxdialogex.h"
#define HAVE_REMOTE
#include <remote-ext.h>


// ChoiceDevs 对话框

IMPLEMENT_DYNAMIC(ChoiceDevs, CDialogEx)

ChoiceDevs::ChoiceDevs(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHOICE_DEVS, pParent)
{
}

ChoiceDevs::~ChoiceDevs()
{
	pcap_freealldevs(m_allDevs);
}

void ChoiceDevs::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_combo);
}


BEGIN_MESSAGE_MAP(ChoiceDevs, CDialogEx)
	ON_BN_CLICKED(IDOK, &ChoiceDevs::OnBnClickedOk)
END_MESSAGE_MAP()


// ChoiceDevs 消息处理程序

//初始下拉列表，选择网卡
BOOL ChoiceDevs::OnInitDialog() {

	CDialog::OnInitDialog();
	char errbuf[PCAP_ERRBUF_SIZE];
	pcap_if_t * temp;

	//获取网卡设备
	if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL, &m_allDevs, errbuf) == -1) {
		MessageBox((CString)errbuf);
		return FALSE;
	}
	
	//添加项到下拉列表
	for (temp = m_allDevs; temp ; temp = temp->next) {
		m_combo.AddString((CString)temp->description + temp->name);
	}
	//下拉列表默认选择第一项
	m_combo.SetCurSel(0);
	
}

//点击确定，选择网卡
void ChoiceDevs::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();

	int nIndex = m_combo.GetCurSel();
	char errbuf[PCAP_ERRBUF_SIZE];
	pcap_if_t * temp;

	temp = m_allDevs;
	for (int i = 0; i < nIndex; ++i) {
		temp = temp->next;
	}
	
	if( (theApp.m_curDev = pcap_open(temp->name, 65536, 1, 1000, NULL, errbuf)) == NULL){
		MessageBox((CString)errbuf);
		return;
	}

	::AfxMessageBox((CString)temp->description);
}

//获取默认网卡(第一个扫描出来的网卡)
void ChoiceDevs::GetDefaultDev(pcap_t * & param) {
	pcap_if_t* alldevs;
	char errbuf[PCAP_ERRBUF_SIZE];

	if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL, &alldevs, errbuf) == -1) {
		::AfxMessageBox((CString)errbuf);
		param = NULL;
		return;
	}

	pcap_if_t* temp = alldevs;
	if ((param =  pcap_open(temp->name, 65536, 1, 1000, NULL, errbuf)) == NULL) {
		::AfxMessageBox((CString)errbuf);
		return;
	}

	pcap_freealldevs(alldevs);

}
