// PackListView.cpp: 实现文件
//

#include "stdafx.h"
#include "Sniffer-WindowsMFC.h"
#include "PackListView.h"
#include "PackInfoAnalysis.h"
#include "PackHexAnalysis.h"
#include <map>


// PackListView

IMPLEMENT_DYNCREATE(PackListView, CListView)

PackListView::PackListView()
{
	theApp.m_packListCtrl = this;
}

PackListView::~PackListView()
{
}

BEGIN_MESSAGE_MAP(PackListView, CListView)
	ON_NOTIFY_REFLECT(NM_CLICK, OnClickList)	//单击调用
END_MESSAGE_MAP()

// PackListView 诊断

#ifdef _DEBUG
void PackListView::AssertValid() const
{
	CListView::AssertValid();
}

#ifndef _WIN32_WCE
void PackListView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif
#endif //_DEBUG


// PackListView 消息处理程序


void PackListView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	static BOOL inited = FALSE;
	if (inited) return;
	inited = TRUE;
	// 以下代码只在首次调用 OnInitialUpdate 时执行

	theApp.m_packList = this->GetSafeHwnd(); //消息句柄更新

	CListCtrl& m_list = GetListCtrl();

	LONG lStyle;
	lStyle = GetWindowLong(m_list.m_hWnd, GWL_STYLE);	//获取当前窗口风格
	lStyle &= ~LVS_TYPEMASK;							//清除显示方式位
	lStyle |= LVS_REPORT;								//设置报表风格
	SetWindowLong(m_list.m_hWnd, GWL_STYLE, lStyle);	//设置窗口风格

	//设置
	DWORD dwStyle = m_list.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;	//选中某行使整行高亮（只适用于报表风格的listctrl）
	dwStyle |= LVS_EX_GRIDLINES;		//网格线（只适用与报表风格的listctrl）
	m_list.SetExtendedStyle(dwStyle);	//设置扩展风格

	//设置各种颜色
	m_list.SetBkColor(RGB(200, 200, 200));		//设置背景颜色
	m_list.SetTextBkColor(RGB(200, 200, 200));	//设置文本背景颜色
	m_list.SetTextColor(RGB(10, 10, 80));		//设置文本颜色


	//添加栏
	m_list.InsertColumn(0, (CString)"序号", LVCFMT_CENTER, 100);
	m_list.InsertColumn(1, (CString)"时间", LVCFMT_CENTER, 250);
	m_list.InsertColumn(2, (CString)"长度", LVCFMT_CENTER, 100);

	//插入列的标题，为了简单起见，我只插入三列,第二步详细分析显示
	m_list.InsertColumn(3, (CString)"源IP", LVCFMT_CENTER, 250);
	m_list.InsertColumn(4, (CString)"目的IP", LVCFMT_CENTER, 250);
	m_list.InsertColumn(5, (CString)"包类型", LVCFMT_CENTER, 150);
	m_list.InsertColumn(6, (CString)"ACK", LVCFMT_CENTER, 50);
	m_list.InsertColumn(7, (CString)"SYN", LVCFMT_CENTER, 50);
	m_list.InsertColumn(8, (CString)"FIN", LVCFMT_CENTER, 50);
	m_list.InsertColumn(9, (CString)"序号", LVCFMT_CENTER, 150);
	m_list.InsertColumn(10, (CString)"确认号", LVCFMT_CENTER, 150);


	this->UpdateData();
	// TODO: 在此添加专用代码和/或调用基类
}


void PackListView::OnDraw(CDC* /*pDC*/)
{
	// TODO: 在此添加专用代码和/或调用基类
}



void PackListView::OnClickList(NMHDR * pNMHDR, LRESULT * pResult) {
	CListCtrl& m_list = GetListCtrl();			//窗体控制
	int nIndex = m_list.GetSelectionMark();		//获取点击的行号
	
	if (nIndex >= 0 || theApp.m_allPacket.size() > nIndex) {
		theApp.m_nIndex = nIndex;
		const u_char* pkt_data = theApp.m_allPacket[nIndex].pkt_data;
		CString kind;					//报文类型
		CString kind_ip("0800");
		CString kind_arp("0806");
		CString kind_rarp("8035");
		CString kind_ipv6("86DD");
		CString temp;
		std::map<CString, CString> info;		//报文头部的键值对：字段名-值
		std::map<int, int>	protocol_offset;	//各个协议头的偏移量
		PackInfoAnalysis* protocol;				//协议分析类
		int start;								//协议开始的字节
		int end;								//协议结束的字节

		//判断协议类型
		kind.Format((CString)"%02X%02X", pkt_data[12], pkt_data[13]);
		if (kind.Compare(kind_ip) == 0 || kind.Compare(kind_arp) == 0 || kind.Compare(kind_rarp) == 0 || kind.Compare(kind_ipv6) == 0) {
			protocol = &Ethernet(0);
		}
		else {
			protocol = &IEEE802_3(0);
		}

		//遍历协议，构造需要PackInfoView显示的信息以及协议头的偏移量protocol_offset
		while (protocol != NULL) {
			protocol->GetInfo(pkt_data + protocol->offset, info);		//获取当前协议头的键值对，保存在info
			start = protocol->offset;		

			//构造PackInfoView显示的信息
			temp += protocol->GetProtocolName() + (CString)":\r\n";
			for (auto iter = info.begin(); iter != info.end(); iter++) {
				temp += iter->first + (CString)" : " + iter->second + (CString)"\r\n";
			}
			temp += (CString)"\r\n\r\n";
			info.clear();
			
			protocol = protocol->NextProtocol(pkt_data + protocol->offset);
			if(protocol != NULL)
				end = protocol->offset;
			protocol_offset[start] = end;
		}
		theApp.m_packInfoViewCtrl->ShowText(temp);		
		
		theApp.m_packHexViewCtrl->ShowText( PackHexAnalysis::charFormatHexASCII(pkt_data, theApp.m_allPacket[nIndex].pkt_header->len), protocol_offset );
	}

	
}

void PackListView::ClearList() {
	CListCtrl& m_list = GetListCtrl();
	m_list.DeleteAllItems();
}