#include "stdafx.h"
#include "PackInfoAnalysis.h"
#include <set>
#include "Sniffer-WindowsMFC.h"



class Staticstics {
	std::set<CString> ip_list;	//keyΪip��ַ,��ʾ�����ʾ��ǰ������map�е�IP����TCP����
	int numOfPKT;				//���ݰ�������
	int numOfTrans;				//���������������

public:
	Staticstics() {
		numOfPKT = 0;
		numOfTrans = 0;
	};
	~Staticstics() {};

	void Update(const u_char* pkt_data) {
		CString mac = GetValue(pkt_data, (CString)"ԴMAC");

		//���ԴMACʹ��ip_list�в������Լ���ip
		if (mac.Compare(theApp.m_mac) != 0) {
			CString FIN = GetValue(pkt_data, (CString)"FIN");
			if (FIN.Compare((CString)"1") == 0) {
				ip_list.erase(GetValue(pkt_data, (CString)"ԴIP"));		//FINΪ1��ʾ���ӽ�����Ŀ��IPȥ��
			}
			else {
				ip_list.insert(GetValue(pkt_data, (CString)"ԴIP"));		//��Ŀ��IP����
			}
		}

		numOfPKT += 1;
		numOfTrans = ip_list.size();	//��Ϊ��ȥ�Լ���IP
	}

	int GetNumOfPKT() {
		return numOfPKT;
	}
	int	GetNumOfTrans() {
		return numOfTrans;
	}
};