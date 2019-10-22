// FilterDlg.cpp: 实现文件
//	控制过滤器对话框
//

#include "stdafx.h"
#include "Sniffer-WindowsMFC.h"
#include "FilterDlg.h"
#include "afxdialogex.h"


// FilterDlg 对话框

IMPLEMENT_DYNAMIC(FilterDlg, CDialog)

FilterDlg::FilterDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_FILTER, pParent)
{
	theApp.m_haveFilter = true;
}

FilterDlg::~FilterDlg()
{
}

void FilterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(FilterDlg, CDialog)
	ON_BN_CLICKED(IDOK, &FilterDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// FilterDlg 消息处理程序


BOOL FilterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CString temp;
	int protocol_size = filter_Protocol.size();
	int ipaddress_size = filter_IPAddress.size();
	int j = 0;

	// TODO:  在此添加额外的初始化
	//初始化，选项框
	for (int i = IDC_FILTER_TCP; i <= IDC_FILTER_HTTPS && j < protocol_size; ++i) {
		CButton* pBtn = (CButton*)GetDlgItem(i);
		pBtn->GetWindowText(temp);

		if (temp.Compare(filter_Protocol[j]) == 0) {
			pBtn->SetCheck(1);
			++j;
		}
	}

	//初始化，编辑框
	temp = "";
	for (int i = 0; i < ipaddress_size; ++i) {
		temp += filter_IPAddress[i];
		if (i != ipaddress_size - 1)
			temp += (CString)";";
	}
	SetDlgItemText(IDC_FILTER_LIST, temp);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void FilterDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnOK();
	CString s;
	GetDlgItemText(IDC_FILTER_LIST,s);
	AnalysisFilterList(s);
}

//将需要过滤的IP的协议保存到fitler_IPAddress和filter_Protocol变量中
void FilterDlg::AnalysisFilterList(CString & filterList) {

	CString temp;
	int len = filterList.GetLength();
	filter_Protocol.clear();
	filter_IPAddress.clear();

	//将需要过滤的IP地址添加进filter_IPAddress
	for (int i = 0; i < len; ++i) {

		if (filterList[i] == ';') {
			filter_IPAddress.push_back(temp);
			temp = "";
			continue;
		}
		
		temp += filterList[i];
	}
	filter_IPAddress.push_back(temp);	//将最后的过滤项加入过滤器

	//将需要过滤的协议添加进filter_Protocol
	for(int i = IDC_FILTER_TCP; i <= IDC_FILTER_HTTPS; ++i){
		if (BST_CHECKED == IsDlgButtonChecked(i)) {
			GetDlgItemTextW(i,temp);
			filter_Protocol.push_back(temp);
		}
	}

}


void FilterDlg::OnBnClickedFilterOther2()
{
	// TODO: 在此添加控件通知处理程序代码
}

bool FilterDlg::Filter(const CString &ip, const CString& protocol) {
	if ( std::count(filter_Protocol.begin(), filter_Protocol.end(), protocol ) || std::count(filter_IPAddress.begin(), filter_IPAddress.end(), ip)){ 
		return true;
	}

	return false;
}
