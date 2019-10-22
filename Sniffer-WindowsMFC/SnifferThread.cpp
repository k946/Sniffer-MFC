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
	static std::unique_ptr<FlowTraffic> flowTraffic;	//����ͳ��
	static std::unique_ptr<Staticstics> staticstics;	//TCP���ӵȸ���
	static long old_time_sec = 0;						//����ÿ���������ͳ�Ƶ���ʾ
	
	if (0 == count) {//����ץ�������¼���
		flowTraffic.reset(new FlowTraffic());
		staticstics.reset(new Staticstics());
	}

	CString last_protocol;						//���ݰ������Э��
	std::unique_ptr<PackInfoAnalysis> protocol; //ִ��Э�����ָ�룬Ϊ�˶�̬

	CString s_count;							//���ݰ����
	s_count.Format((CString)"%d", count);		
	CString pkt_len;							//���ݰ�����
	pkt_len.Format((CString)"%d", pkt_head->len);

	char time[128];								//���ݰ���ȡ��ʱ��
	time_t l_time = (pkt_head->ts.tv_sec);
	strftime(time, 64, "%Y-%m-%d %H:%M:%S", localtime(&l_time));

	protocol.reset(new Ethernet(0));
	
	
	PackInfoAnalysis::clear_global();
	//��ȡ���ϲ��Э��
	while (protocol != NULL) {
		last_protocol = protocol->GetProtocolName();
		protocol.reset(protocol->NextProtocol(pkt_data + protocol->offset));
	}

	if ( theApp.m_haveFilter && !(FilterDlg::Filter(global_s_ip, last_protocol)) ) {	//�������ʾip��Э���ڹ�������
		return;
	}

	//�������¼���ʾ
	flowTraffic->UpdateFlow(l_time, pkt_head->len, pkt_data);
	staticstics->Update(pkt_data);
	if ( l_time - old_time_sec > 0) {
		theApp.m_flowView->ShowText(staticstics->GetNumOfPKT(), staticstics->GetNumOfTrans(),
			flowTraffic->GetTotalFlow(),		//������
			flowTraffic->GetOutputFlow(),		//�ϴ�����
			flowTraffic->GetInputFlow(),		//��������
			flowTraffic->GetSecondOutputFlow(),	//ÿ����ϴ�����
			flowTraffic->GetSecondInputFlow()	//ÿ�����������
		);
	}

	//����������б��в�����
	CListCtrl& m_list = theApp.m_packListCtrl->GetListCtrl();
	m_list.InsertItem(count, s_count);
	m_list.SetItemText(count, 1, (CString)time);
	m_list.SetItemText(count, 2, pkt_len);
	m_list.SetItemText(count, 3, global_s_ip + " ��" + global_s_port);
	m_list.SetItemText(count, 4, global_d_ip + " ��" + global_d_port);
	m_list.SetItemText(count, 5, last_protocol);
	m_list.SetItemText(count, 6, global_ACK);
	m_list.SetItemText(count, 7, global_SYN);
	m_list.SetItemText(count, 8, global_FIN);
	m_list.SetItemText(count, 9, global_seq);
	m_list.SetItemText(count, 10, global_ack);
	count++;	

	//����һ�����ݰ�������
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

	//ѭ��ץ��
	pcap_loop(theApp.m_curDev, 0, packet_handler, (u_char*)0);
	return 0;
}

