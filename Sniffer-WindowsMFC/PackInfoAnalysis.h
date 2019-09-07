#pragma once

/*
**需求：
**	获取某个协议的信息
**	
*/

#include <vector>
#include <map>

class PackInfoAnalysis
{
public:
	virtual CString GetProtocolName() = 0;
	virtual PackInfoAnalysis* NextProtocol(const u_char* pkt_data) = 0;		//获取更高层的协议
	virtual void GetInfo(const u_char* pkt_data, std::map<CString, CString> &result) = 0;	//格式获取成员信息
	int offset;
	static void clear_global();
};


class Ethernet: public PackInfoAnalysis {
public:
	
	PackInfoAnalysis* NextProtocol(const u_char* pkt_data);
	void GetInfo(const u_char* pkt_data ,std::map<CString, CString> & result);

	Ethernet(int offset) {
		this->offset = offset;
	}

	CString GetProtocolName() {
		return (CString)"Ethernet";
	}

};

class IEEE802_3 : public PackInfoAnalysis {
public:
	PackInfoAnalysis* NextProtocol(const u_char* pkt_data);
	void GetInfo(const u_char* pkt_data ,std::map<CString, CString> &result);

	IEEE802_3(int offset) {
		this->offset = offset;
	}

	CString GetProtocolName() {
		return (CString)"802.3";
	}
};

class IP : public PackInfoAnalysis {
public:
	PackInfoAnalysis* NextProtocol(const u_char* pkt_data);
	void GetInfo(const u_char* pkt_data ,std::map<CString, CString> &result);

	IP(int offset) {
		this->offset = offset;
	}

	CString GetProtocolName() {
		return (CString)"IP";
	}
};

class IPv6 : public PackInfoAnalysis {
public:
	PackInfoAnalysis* NextProtocol(const u_char* pkt_data);
	void GetInfo(const u_char* pkt_data ,std::map<CString, CString> &result);

	IPv6(int offset) {
		this->offset = offset;
	}

	CString GetProtocolName() {
		return (CString)"IPv6";
	}
};

class ARP : public PackInfoAnalysis {
public:
	PackInfoAnalysis* NextProtocol(const u_char* pkt_data);
	void GetInfo(const u_char* pkt_data ,std::map<CString, CString> &result);

	ARP(int offset) {
		this->offset = offset;
	}

	CString GetProtocolName() {
		return (CString)"ARP";
	}
};

class ICMP : public PackInfoAnalysis {
public:
	PackInfoAnalysis* NextProtocol(const u_char* pkt_data);
	void GetInfo(const u_char* pkt_data ,std::map<CString, CString> &result);

	ICMP(int offset) {
		this->offset = offset;
	}

	CString GetProtocolName() {
		return (CString)"ICMP";
	}
};

class TCP : public PackInfoAnalysis {
public:
	PackInfoAnalysis* NextProtocol(const u_char* pkt_data);
	void GetInfo(const u_char* pkt_data ,std::map<CString, CString> &result);

	TCP(int offset) {
		this->offset = offset;
	}

	CString GetProtocolName() {
		return (CString)"TCP";
	}
};

class UDP : public PackInfoAnalysis {
public:
	PackInfoAnalysis* NextProtocol(const u_char* pkt_data);
	void GetInfo(const u_char* pkt_data ,std::map<CString, CString> &result);

	UDP(int offset) {
		this->offset = offset;
	}

	CString GetProtocolName() {
		return (CString)"UDP";
	}
};

class IGMP : public PackInfoAnalysis {
public:
	PackInfoAnalysis* NextProtocol(const u_char* pkt_data);
	void GetInfo(const u_char* pkt_data ,std::map<CString, CString> &result);

	IGMP(int offset) {
		this->offset = offset;
	}

	CString GetProtocolName() {
		return (CString)"IGMP";
	}
};

class OSPF : public PackInfoAnalysis {
public:
	PackInfoAnalysis* NextProtocol(const u_char* pkt_data);
	void GetInfo(const u_char* pkt_data ,std::map<CString, CString> &result);

	OSPF(int offset) {
		this->offset = offset;
	}

	CString GetProtocolName() {
		return (CString)"OSPF";
	}
};

class DNS : public PackInfoAnalysis {
public:
	PackInfoAnalysis* NextProtocol(const u_char* pkt_data);
	void GetInfo(const u_char* pkt_data ,std::map<CString, CString> &result);

	DNS(int offset) {
		this->offset = offset;
	}

	CString GetProtocolName() {
		return (CString)"DNS";
	}
};


class HTTP : public PackInfoAnalysis {
public:
	PackInfoAnalysis* NextProtocol(const u_char* pkt_data);
	void GetInfo(const u_char* pkt_data ,std::map<CString, CString> &result);

	HTTP(int offset) {
		this->offset = offset;
	}

	CString GetProtocolName() {
		return (CString)"HTTP";
	}
};

class HTTPS : public PackInfoAnalysis {
public:
	PackInfoAnalysis* NextProtocol(const u_char* pkt_data);
	void GetInfo(const u_char* pkt_data, std::map<CString, CString> &result);

	HTTPS(int offset) {
		this->offset = offset;
	}

	CString GetProtocolName() {
		return (CString)"HTTPS";
	}
};

class FTP : public PackInfoAnalysis {
public:
	PackInfoAnalysis* NextProtocol(const u_char* pkt_data);
	void GetInfo(const u_char* pkt_data ,std::map<CString, CString> &result);

	FTP(int offset) {
		this->offset = offset;
	}

	CString GetProtocolName() {
		return (CString)"FTP";
	}
};


