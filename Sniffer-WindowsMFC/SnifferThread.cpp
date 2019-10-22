#include "stdafx.h"
#include "SnifferThread.h"
#include "PackInfoAnalysis.h"
#include <pcap.h>
#include <map>
#include <memory>
#include "Sniffer-WindowsMFC.h"
#include "FlowTraffic.h"
#include "Staticstics.h"


extern CString global_s_ip;
extern CString	global_d_ip;
extern CString global_ACK;
extern CString global_SYN;
extern CString global_FIN;
extern CString global_ack;
extern CString global_seq;
extern CString global_s_port;
extern CString global_d_port;
int count;

SnifferThread::SnifferThread()
{
}


SnifferThread::~SnifferThread()
{
}

static void packet_handler(u_char* s, const struct pcap_pkthdr* pkt_head, const u_char* pkt_data)
{
	static std::unique_ptr<FlowTraffic> flowTraffic;	//流量统计
	static std::unique_ptr<Staticstics> staticstics;	//TCP连接等跟踪
	static long old_time_sec = 0;						//用于每秒更新流量统计的显示
	
	if (0 == count) {//重新抓包，重新计数
		flowTraffic.reset(new FlowTraffic());
		staticstics.reset(new Staticstics());
	}

	CString last_protocol;						//数据包的最顶层协议
	std::unique_ptr<PackInfoAnalysis> protocol; //执行协议类的指针，为了多态

	CString s_count;							//数据包序号
	s_count.Format((CString)"%d", count);		
	CString pkt_len;							//数据包长度
	pkt_len.Format((CString)"%d", pkt_head->len);

	char time[128];								//数据包获取的时间
	time_t l_time = (pkt_head->ts.tv_sec);
	strftime(time, 64, "%Y-%m-%d %H:%M:%S", localtime(&l_time));

	protocol.reset(new Ethernet(0));
	
	
	PackInfoAnalysis::clear_global();
	//获取最上层的协议
	while (protocol != NULL) {
		last_protocol = protocol->GetProtocolName();
		protocol.reset(protocol->NextProtocol(pkt_data + protocol->offset));
	}

	if ( theApp.m_haveFilter && !(FilterDlg::Filter(global_s_ip, last_protocol)) ) {	//返回真表示ip或协议在过滤项中
		return;
	}

	//流量更新及显示
	flowTraffic->UpdateFlow(l_time, pkt_head->len, pkt_data);
	staticstics->Update(pkt_data);
	if ( l_time - old_time_sec > 0) {
		theApp.m_flowView->ShowText(staticstics->GetNumOfPKT(), staticstics->GetNumOfTrans(),
			flowTraffic->GetTotalFlow(),		//总流量
			flowTraffic->GetOutputFlow(),		//上传流量
			flowTraffic->GetInputFlow(),		//下载流量
			flowTraffic->GetSecondOutputFlow(),	//每秒的上传流量
			flowTraffic->GetSecondInputFlow()	//每秒的下载流量
		);
	}

	//向主界面的列表中插入行
	CListCtrl& m_list = theApp.m_packListCtrl->GetListCtrl();
	m_list.InsertItem(count, s_count);
	m_list.SetItemText(count, 1, (CString)time);
	m_list.SetItemText(count, 2, pkt_len);
	m_list.SetItemText(count, 3, global_s_ip + " ：" + global_s_port);
	m_list.SetItemText(count, 4, global_d_ip + " ：" + global_d_port);
	m_list.SetItemText(count, 5, last_protocol);
	m_list.SetItemText(count, 6, global_ACK);
	m_list.SetItemText(count, 7, global_SYN);
	m_list.SetItemText(count, 8, global_FIN);
	m_list.SetItemText(count, 9, global_seq);
	m_list.SetItemText(count, 10, global_ack);
	count++;	

	//复制一份数据包并保存
	u_char* copy = new u_char[pkt_head->len];
	pcap_pkthdr * copy2 = new pcap_pkthdr;
	memcpy(copy2, pkt_head, sizeof(pcap_pkthdr));
	memcpy(copy, pkt_data, pkt_head->len);
	theApp.m_allPacket.push_back(pkt(copy, copy2));
}

UINT SnifferThread::MyThreadFunction(LPVOID pParam) {
	count = 0;
	
	pcap_t * devs = (pcap_t*)pParam;
	char curFileName[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, (LPWSTR)curFileName);
	pcap_dumper_t * dump = pcap_dump_open(theApp.m_curDev, curFileName);

	//循环抓包
	pcap_loop(theApp.m_curDev, 0, packet_handler, (u_char*)0);
	return 0;
}

