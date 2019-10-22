#include "stdafx.h"
#include "PackInfoAnalysis.h"
#include <set>
#include "Sniffer-WindowsMFC.h"



class Staticstics {
	std::set<CString> ip_list;	//key为ip地址,表示整体表示当前机器与map中的IP建立TCP连接
	int numOfPKT;				//数据包的数量
	int numOfTrans;				//正在请求传输的数量

public:
	Staticstics() {
		numOfPKT = 0;
		numOfTrans = 0;
	};
	~Staticstics() {};

	void Update(const u_char* pkt_data) {
		CString mac = GetValue(pkt_data, (CString)"源MAC");

		//检测源MAC使得ip_list中不包含自己的ip
		if (mac.Compare(theApp.m_mac) != 0) {
			CString FIN = GetValue(pkt_data, (CString)"FIN");
			if (FIN.Compare((CString)"1") == 0) {
				ip_list.erase(GetValue(pkt_data, (CString)"源IP"));		//FIN为1表示连接结束将目标IP去除
			}
			else {
				ip_list.insert(GetValue(pkt_data, (CString)"源IP"));		//将目标IP加入
			}
		}

		numOfPKT += 1;
		numOfTrans = ip_list.size();	//因为除去自己的IP
	}

	int GetNumOfPKT() {
		return numOfPKT;
	}
	int	GetNumOfTrans() {
		return numOfTrans;
	}
};