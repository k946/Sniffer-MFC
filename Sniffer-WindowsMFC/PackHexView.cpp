// PackHexView.cpp: 实现文件
//

#include "stdafx.h"
#include "Sniffer-WindowsMFC.h"
#include "PackHexView.h"


// PackHexView

IMPLEMENT_DYNCREATE(PackHexView, CRichEditView)

PackHexView::PackHexView()
{
	theApp.m_packHexViewCtrl = this;
}

PackHexView::~PackHexView()
{
}

BEGIN_MESSAGE_MAP(PackHexView, CRichEditView)
END_MESSAGE_MAP()


// PackHexView 诊断

#ifdef _DEBUG
void PackHexView::AssertValid() const
{
	CRichEditView::AssertValid();
}

#ifndef _WIN32_WCE
void PackHexView::Dump(CDumpContext& dc) const
{
	CRichEditView::Dump(dc);
}
#endif
#endif //_DEBUG


// PackHexView 消息处理程序


void PackHexView::OnInitialUpdate()
{
	CRichEditView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	static BOOL inited = FALSE;
	if (inited) return;
	inited = TRUE;

	m_edit = &GetRichEditCtrl();

	m_edit->SetReadOnly();
}

void PackHexView::ShowText(CString & text) {
	this->SetWindowText(text);	

	CHARFORMAT cf;
	ZeroMemory(&cf, sizeof(CHARFORMAT));
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_BOLD | CFM_COLOR | CFM_FACE |
		CFM_ITALIC | CFM_SIZE | CFM_UNDERLINE;
	cf.dwEffects = 0;
	cf.yHeight = 15 * 15;//文字高度 
	//cf.crTextColor = 0xff0000; //文字颜色 
	strcpy((char*)cf.szFaceName, (char*)_T("隶书"));//设置字体 
	m_edit->SetSel(0, text.GetLength()); //设置处理区域 
	m_edit->SetSelectionCharFormat(cf);
}

void PackHexView::ShowText(CString & text, std::map<int, int> offset) {
	this->SetWindowText(text);

	CHARFORMAT cf;
	ZeroMemory(&cf, sizeof(CHARFORMAT));
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_BOLD | CFM_COLOR | CFM_FACE |
		CFM_ITALIC | CFM_SIZE | CFM_UNDERLINE;
	cf.dwEffects |= CFM_BOLD;
	cf.yHeight = 15 * 15;//文字高度 
	//cf.crTextColor = 0xff0000; //文字颜色 
	strcpy((char*)cf.szFaceName, (char*)_T("隶书"));//设置字体 
	m_edit->SetSel(0, text.GetLength()); //设置处理区域 
	m_edit->SetSelectionCharFormat(cf);


	int start;
	int end;
	int color[] = { 0X96CDCD, 0XEE9A49, 0XEE6363, 0XCDBE70, 0XCD950C, 0X43CD80, 0XA2CD5A };
	int i = 0;
	for (auto iter = offset.begin(); iter != offset.end(); iter++) {
		start = iter->first;
		end = iter->second;
		cf.crTextColor = color[i++];
		m_edit->SetSel(start*3, end*3); //设置处理区域 
		m_edit->SetSelectionCharFormat(cf);
	}
}
