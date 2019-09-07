#include "stdafx.h"
#include "SnifferThread.h"
#include "PackInfoAnalysis.h"
#include <pcap.h>
#include <map>
#include "Sniffer-WindowsMFC.h"

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

	CString last_protocol;
	CString kind;
	CString kind_ip("0800");
	CString kind_arp("0806");
	CString kind_rarp("8035");
	CString kind_ipv6("86DD");
	PackInfoAnalysis* protocol;

	CString s_count;
	s_count.Format((CString)"%d", count);
	CString pkt_len;
	pkt_len.Format((CString)"%d", pkt_head->len);

	char time[128];
	time_t l = (pkt_head->ts.tv_sec);
	strftime(time, 64, "%Y-%m-%d %H:%M:%S", localtime(&l));

	//判断协议类型
	kind.Format((CString)"%02X%02X", pkt_data[12], pkt_data[13]);
	if (kind.Compare(kind_ip) == 0 || kind.Compare(kind_arp) == 0 || kind.Compare(kind_rarp) == 0 || kind.Compare(kind_ipv6) == 0) {
		protocol = &Ethernet(0);
	}
	else {
		protocol = &IEEE802_3(0);
	}
	PackInfoAnalysis::clear_global();
	//获取最上层的协议
	while (protocol != NULL) {
		last_protocol = protocol->GetProtocolName();
		protocol = protocol->NextProtocol(pkt_data + protocol->offset);
	}

	if (std::count(theApp.m_filter_Protocol.begin(), theApp.m_filter_Protocol.end(), last_protocol) || std::count(theApp.m_filter_IPAddress.begin(), theApp.m_filter_IPAddress.end(), global_d_ip) || std::count(theApp.m_filter_IPAddress.begin(), theApp.m_filter_IPAddress.end(), global_s_ip)) {
		return;
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
	pcap_loop(theApp.m_curDev, 0, packet_handler, (u_char*)dump);

	CString temp;

	return 0;
}

