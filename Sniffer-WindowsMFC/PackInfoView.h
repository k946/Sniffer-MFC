#pragma once


// PackInfoView 视图

class PackInfoView : public CEditView
{
	DECLARE_DYNCREATE(PackInfoView)

protected:
	PackInfoView();           // 动态创建所使用的受保护的构造函数
	virtual ~PackInfoView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnSend();
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	void ShowText(CString & text);
	CButton send;

};


