// PackInfoView.cpp: 实现文件
//

#include "stdafx.h"
#include "Sniffer-WindowsMFC.h"
#include "PackInfoView.h"
#include "SendPacket.h"


// PackInfoView

IMPLEMENT_DYNCREATE(PackInfoView, CEditView)

PackInfoView::PackInfoView()
{
	theApp.m_packInfoViewCtrl = this;
}

PackInfoView::~PackInfoView()
{
}

BEGIN_MESSAGE_MAP(PackInfoView, CEditView)
	ON_COMMAND(1,OnSend)
END_MESSAGE_MAP()


// PackInfoView 诊断

#ifdef _DEBUG
void PackInfoView::AssertValid() const
{
	CEditView::AssertValid();
}

#ifndef _WIN32_WCE
void PackInfoView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif
#endif //_DEBUG


// PackInfoView 消息处理程序

void PackInfoView::OnInitialUpdate()
{
	CEditView::OnInitialUpdate();

	static BOOL intied = FALSE;
	if (intied)	return;
	intied = TRUE;
	send.Create((CString)"发送数据包", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(550, 0, 650, 40), this, 1);
	this->GetEditCtrl().SetReadOnly();	//编辑区设置为只读
	// TODO: 在此添加专用代码和/或调用基类
}


void PackInfoView::ShowText(CString& text) {
	this->SetWindowText(text);
}

int PackInfoView::OnCreate(LPCREATESTRUCT lpCreateStruct){
	return 0;
}

void PackInfoView::OnSend() {
	SendPacket dlg;
	dlg.DoModal();
}
