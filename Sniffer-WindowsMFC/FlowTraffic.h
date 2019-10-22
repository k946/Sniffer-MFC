#pragma once
#include "stdafx.h"
#include "PackInfoAnalysis.h"
#include <set>
#include "Sniffer-WindowsMFC.h"

class FlowTraffic
{
private:
	double total;		//������
	double output;	//�ϴ�����
	double input;		//��������
	double secondOutput;	//ÿ����ϴ�����
	double secondInput;	//ÿ�����������

public:
	FlowTraffic():total(0),output(0),input(0),secondOutput(0),secondInput(0) {
	
	};

	~FlowTraffic() {};
	void UpdateFlow(long time_sec, long leng, const u_char* pkt_data) {
		static long old_time = 0;
		if (old_time == 0) {
			old_time = time_sec;
		}

		CString mac = GetValue(pkt_data, (CString)"ԴMAC");
		total += leng / 1024.0;				//����������
		if (mac.Compare(theApp.m_mac) == 0) {		//ͨ��pkt��Դip�ֶ��жϻ���Դmac�ֶ��ж�
			output += leng / 1024.0;			//������Ϊ�Լ������ϴ�����������
			if (time_sec - old_time > 0) {	//�ж�ʱ���Ƿ��Ѿ�����1��
				old_time = time_sec;			//�����µ�ʱ��ê��
				secondOutput = leng / 1024.0;	//����ÿ���ϴ�����
			}
			else {
				secondOutput += leng / 1024.0;//δ����ʱ��ê��1�룬ÿ���ϴ���������
			}
		}
		else {
			input += leng / 1024.0;
			if (time_sec - old_time > 0) {
				old_time = time_sec;
				secondInput = leng / 1024.0;
			}
			else {
				secondInput += leng / 1024.0;
			}
		}

	}

	double GetTotalFlow() {
		return total;
	}

	double GetInputFlow() {
		return input;
	}

	double GetOutputFlow() {
		return output;
	}

	double GetSecondOutputFlow() {
		return secondOutput;
	}

	double GetSecondInputFlow() {
		return secondInput;
	}
};

