
// Sniffer-WindowsMFC.h: Sniffer-WindowsMFC 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号
#include "PackListView.h"
#include "PackInfoView.h"
#include "PackHexView.h"
#include "pcap.h"
#include <vector>
#include "SnifferThread.h"


// CSnifferWindowsMFCApp:
// 有关此类的实现，请参阅 Sniffer-WindowsMFC.cpp
//

class CSnifferWindowsMFCApp : public CWinApp
{
public:
	CSnifferWindowsMFCApp() noexcept;

	//自定义变量
	HWND m_packList;					//PackListView的句柄
	PackListView *m_packListCtrl;		//调用PackListView的成员方法	
	PackInfoView *m_packInfoViewCtrl;	//调用PackInfoView的成员方法
	PackHexView *m_packHexViewCtrl;		//调用PackHexView的成员方法
	std::vector<CString>	m_filter_IPAddress = {};	//需要过滤的IP地址
	std::vector<CString>	m_filter_Protocol = {};		//需要过滤的协议
	pcap_t* m_curDev = 0;					//已开启的网卡设备
	bool m_isSniffer = FALSE;					//是否正在嗅探
	BOOL m_haveFilter = FALSE;					//是否开启过滤
	int m_nIndex = -1;
	std::vector<pkt>  m_allPacket;

// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 实现

public:
	void OnBarStart();				//开始抓包
	void OnBarOpen();				//打开文件
	void OnBarSave();				//保存文件
	void OnBarEnd();				//结束抓包
	void OnSwitchFilter();			//过滤器开关
	void OnFilterList();			//更新过滤器
	afx_msg void OnAppAbout();		//关于
	DECLARE_MESSAGE_MAP()
	afx_msg void OnAppExit();
};

extern CSnifferWindowsMFCApp theApp;
