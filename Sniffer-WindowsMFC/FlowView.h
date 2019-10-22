#pragma once


// FlowView 视图

class FlowView : public CEditView
{
	DECLARE_DYNCREATE(FlowView)

protected:
	FlowView();           // 动态创建所使用的受保护的构造函数
	virtual ~FlowView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void OnInitialUpdate();
	DECLARE_MESSAGE_MAP()

public:
	void ShowText(CString & text);

	void ShowText(int numOfPKT, int numOfTrans, double total,		//总流量
		double output,	//上传流量
		double input,		//下载流量
		double secondOutput,	//每秒的上传流量
		double secondInput//每秒的下载流量
	);
};


