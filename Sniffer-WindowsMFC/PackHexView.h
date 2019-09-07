#pragma once
#include <map>

// PackHexView 视图

class PackHexView : public CRichEditView
{
	DECLARE_DYNCREATE(PackHexView)

protected:
	PackHexView();           // 动态创建所使用的受保护的构造函数
	virtual ~PackHexView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
	CRichEditCtrl* m_edit;
public:
	virtual void OnInitialUpdate();
	void ShowText(CString & text);
	void ShowText(CString & text, std::map<int, int> offset);
};


