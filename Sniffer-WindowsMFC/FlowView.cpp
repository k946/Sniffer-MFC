// FlowView.cpp: 实现文件
//

#include "stdafx.h"
#include "Sniffer-WindowsMFC.h"
#include "FlowView.h"

// FlowView

IMPLEMENT_DYNCREATE(FlowView, CEditView)
FlowView::FlowView()
{
	theApp.m_flowView = this;
}

FlowView::~FlowView()
{
}

BEGIN_MESSAGE_MAP(FlowView, CEditView)
END_MESSAGE_MAP()


// FlowView 诊断

#ifdef _DEBUG
void FlowView::AssertValid() const
{
	CEditView::AssertValid();
}

#ifndef _WIN32_WCE
void FlowView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif
#endif //_DEBUG


// FlowView 消息处理程序
void FlowView::OnInitialUpdate()
{
	CEditView::OnInitialUpdate();

	static BOOL intied = FALSE;
	if (intied)	return;
	intied = TRUE;
	this->GetEditCtrl().SetReadOnly();	//编辑区设置为只读
	// TODO: 在此添加专用代码和/或调用基类
}

void FlowView::ShowText(CString& text) {
	this->SetWindowText(text);
}

void FlowView::ShowText(int numOfPKT, int numOfTrans, 
	double total,		//总流量
	double output,	//上传流量
	double input,		//下载流量
	double secondOutput,	//每秒的上传流量
	double secondInput	//每秒的下载流量
) 
{
	
	CString text;
	text.Format((CString)"数据包：%d  TCP连接：%d  总流量：%0.2lf kb 上传：%0.2lf kb 下载：%0.2lf kb  每秒上传：%0.2lf k/sec  每秒下载：%0.2lf k/sec",
						numOfPKT,			numOfTrans, total,		output,		input,		secondOutput,			secondInput);
	this->SetWindowText(text);
}