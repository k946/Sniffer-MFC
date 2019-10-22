#pragma once
#include "stdafx.h"
#include "PackInfoAnalysis.h"
#include <set>
#include "Sniffer-WindowsMFC.h"

class FlowTraffic
{
private:
	double total;		//总流量
	double output;	//上传流量
	double input;		//下载流量
	double secondOutput;	//每秒的上传流量
	double secondInput;	//每秒的下载流量

public:
	FlowTraffic():total(0),output(0),input(0),secondOutput(0),secondInput(0) {
	
	};

	~FlowTraffic() {};
	void UpdateFlow(long time_sec, long leng, const u_char* pkt_data) {
		static long old_time = 0;
		if (old_time == 0) {
			old_time = time_sec;
		}

		CString mac = GetValue(pkt_data, (CString)"源MAC");
		total += leng / 1024.0;				//总流量增加
		if (mac.Compare(theApp.m_mac) == 0) {		//通过pkt的源ip字段判断或者源mac字段判断
			output += leng / 1024.0;			//发送者为自己，则上传总流量增加
			if (time_sec - old_time > 0) {	//判断时间是否已经超过1秒
				old_time = time_sec;			//设置新的时间锚点
				secondOutput = leng / 1024.0;	//重置每秒上传流量
			}
			else {
				secondOutput += leng / 1024.0;//未超过时间锚点1秒，每秒上传流量增加
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

