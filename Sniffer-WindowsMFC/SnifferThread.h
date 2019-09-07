#pragma once
class SnifferThread
{
public:
	SnifferThread();
	~SnifferThread();
	static UINT MyThreadFunction(LPVOID pParam);
};

class pkt {
public:	
	pkt(u_char* data, struct pcap_pkthdr* head) {
		pkt_data = data;
		pkt_header = head;
	}

	u_char* pkt_data;
	struct pcap_pkthdr* pkt_header;
};



