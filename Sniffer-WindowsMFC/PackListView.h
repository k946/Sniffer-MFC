#pragma once


// PackListView 视图

class PackListView : public CListView
{
	DECLARE_DYNCREATE(PackListView)

protected:
	PackListView();           // 动态创建所使用的受保护的构造函数
	virtual ~PackListView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	virtual void OnDraw(CDC* /*pDC*/);
	virtual void ClearList();
	virtual void OnClickList(NMHDR * pNMHDR, LRESULT * pResult);
};


