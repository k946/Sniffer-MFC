#include "stdafx.h"
#include "PackInfoAnalysis.h"
#include <map>
#include <memory>
using namespace std;

CString global_s_ip;
CString	global_d_ip;
CString global_s_mac;
CString global_d_mac;
CString global_ACK;
CString global_SYN;
CString global_FIN;
CString global_ack;
CString global_seq;
CString global_s_port;
CString global_d_port;

CString GetValue(const const u_char* pkt_data, CString key) {
	std::map<CString, CString> info;		//报文头部的键值对：字段名-值

	std::unique_ptr<PackInfoAnalysis> protocol; //协议分析类,使用智能指针方便回收内存
	protocol.reset(new Ethernet(0));

	//遍历协议，构造需要PackInfoView显示的信息以及协议头的偏移量protocol_offset
	while (protocol != NULL) {
		protocol->GetInfo(pkt_data + protocol->offset, info);		//获取当前协议头的键值对，保存在info
		
		//构造PackInfoView显示的信息
		for (auto iter = info.begin(); iter != info.end(); iter++) {
			if (key.Compare(iter->first) == 0)
				return iter->second;
		}

		info.clear();
		protocol.reset(protocol->NextProtocol(pkt_data + protocol->offset));
	}

	return (CString)"";
}

PackInfoAnalysis* Ethernet::NextProtocol(const const u_char* pkt_data) {

	CString kind;
	CString kind_ip("0800");
	CString kind_arp("0806");
	CString kind_rarp("8035");
	CString kind_ipv6("86DD");

	kind.Format((CString)"%02X%02X", pkt_data[12], pkt_data[13]);

	//判断下一层报文使用的协议
	if (kind.Compare(kind_ip) == 0) {
		return new IP(14);
	}
	else if (kind.Compare(kind_arp) == 0) {
		return new ARP(14);
	}
	else if (kind.Compare(kind_rarp) == 0) {
		return new ARP(14);
	}
	else if (kind.Compare(kind_ipv6) == 0) {
		return new IPv6(14);
	}
	return NULL;
}

void Ethernet::GetInfo(const u_char* pkt_data ,std::map<CString,CString> &result) {
	CString s_mac;
	CString d_mac;
	CString kind;

	d_mac.Format((CString)"%02X-%02X-%02X-%02X-%02X-%02X", pkt_data[0], pkt_data[1], pkt_data[2], pkt_data[3], pkt_data[4], pkt_data[5]);
	s_mac.Format((CString)"%02X-%02X-%02X-%02X-%02X-%02X", pkt_data[6], pkt_data[7], pkt_data[8], pkt_data[9], pkt_data[10], pkt_data[11]);
	kind.Format((CString)"%02X%02X", pkt_data[12], pkt_data[13]);
	
	result[(CString)"源MAC"] = s_mac;
	result[(CString)"目的MAC"] = d_mac;
	result[(CString)"类型"] = kind;
}


PackInfoAnalysis* IEEE802_3::NextProtocol(const u_char* pkt_data) {

	CString kind;

	kind.Format((CString)"%02X%02X", pkt_data[20], pkt_data[21]);

	//判断下一层报文使用的协议
	if (kind.Compare((CString)"0800") == 0) {
		return new IP(22);
	}
	else if (kind.Compare((CString)"0806") == 0) {
		return new ARP(22);
	}
	else if (kind.Compare((CString)"8035") == 0) {
		return new ARP(22);
	}
	else if (kind.Compare((CString)"86DD") == 0) {
		return new IPv6(22);
	}

	return NULL;
}

void IEEE802_3::GetInfo(const u_char* pkt_data ,std::map<CString, CString> &result) {
	CString s_mac;
	CString d_mac;
	CString kind;

	s_mac.Format((CString)"%02X-%02X-%02X-%02X-%02X-%02X", pkt_data[0], pkt_data[1], pkt_data[2], pkt_data[3], pkt_data[4], pkt_data[5]);
	d_mac.Format((CString)"%02X-%02X-%02X-%02X-%02X-%02X", pkt_data[6], pkt_data[7], pkt_data[8], pkt_data[9], pkt_data[10], pkt_data[11]);
	kind.Format((CString)"%02X%02X", pkt_data[20], pkt_data[21]);
	
	result[(CString)"源MAC"] = s_mac;
	result[(CString)"目的MAC"] = d_mac;
	result[(CString)"类型"] = kind;
}

PackInfoAnalysis* IP::NextProtocol(const u_char* pkt_data) {
	int kind = pkt_data[9];
	int len = pkt_data[0] & 0x0f;

	global_s_ip.Format((CString)"%d.%d.%d.%d", pkt_data[12], pkt_data[13], pkt_data[14], pkt_data[15]);
	global_d_ip.Format((CString)"%d.%d.%d.%d", pkt_data[16], pkt_data[17], pkt_data[18], pkt_data[19]);


	//判断下一层报文使用的协议
	if(kind == 1){ 
		return new ICMP(offset + len * 4);
	}
	else if (kind == 6) {
		return new TCP(offset + len * 4);
	}
	else if (kind == 17) {
		return new UDP(offset + len * 4);
	}
	else if (kind == 2) {
		return new IGMP(offset + len * 4);
	}

	return NULL;
}
void IP::GetInfo(const u_char* pkt_data ,std::map<CString, CString> &result) {
	CString version;		//版本：IP报文版本号 IPV4:4，IPV6:6 
	CString IHL;			//首部长度：IP header 长度，没有选项，则一般为5（5x32bit＝20B）
	CString total_len;		//总长度：header＋数据 总长度 
	CString id;				//16位标识：IP 报文的唯一id，分片报文的id 相同，便于进行重组
	CString fragment;		//3位标志：标明是否分片。
	CString offset;			//13位片偏移
	CString TTL;			//TTL：生存时间，即路由器的跳数，每经过一个路由器，该TTL 减一，因此路由器需要重新计算IP报文的校验和
	CString protocol;		//8位协议：ICMP：1，TCP：6，UDP：17，其他的自行百度
	CString	verification;	//首部校验和：IP header校验和，接收端收到报文进行计算如果校验和错误，直接丢弃
	CString s_ip;			//源IP地址
	CString d_ip;			//目的IP地址


	version.Format((CString)"%d", (pkt_data[0] & 0xf0) >> 4);
	result[(CString)"版本"] = version;

	IHL.Format((CString)"%d", pkt_data[0] & 0x0f);
	result[(CString)"首部长度"] = IHL;

	total_len.Format((CString)"%d", (pkt_data[2] << 8) + pkt_data[3]);
	result[(CString)"总长度"] = total_len;

	id.Format((CString)"%d", (pkt_data[4] << 8) + pkt_data[5]);
	result[(CString)"标识"] = id;

	fragment.Format((CString)"%d", (pkt_data[6] & 0xe0) >> 5);
	result[(CString)"标志"] = fragment;

	offset.Format((CString)"%d", ((pkt_data[6] & 0x1f) << 8) + pkt_data[7]);
	result[(CString)"偏移"] = offset;

	TTL.Format((CString)"%d", pkt_data[8]);
	result[(CString)"生存时间"] = TTL;

	protocol.Format((CString)"%d", pkt_data[9]);
	result[(CString)"协议"] = protocol;

	verification.Format((CString)"%d", (pkt_data[10] << 8) + pkt_data[11]);
	result[(CString)"首部检验和"] = verification;

	s_ip.Format((CString)"%d.%d.%d.%d", pkt_data[12], pkt_data[13], pkt_data[14], pkt_data[15]);
	result[(CString)"源IP"] = s_ip;

	d_ip.Format((CString)"%d.%d.%d.%d", pkt_data[16], pkt_data[17], pkt_data[18], pkt_data[19]);
	result[(CString)"目的IP"] = d_ip;
}


PackInfoAnalysis* IPv6::NextProtocol(const u_char* pkt_data) { 
	int kind = pkt_data[6];

	global_s_ip.Format((CString)"%X:%X:%X:%X:%X:%X:%X:%X", ((pkt_data[8] << 8) + pkt_data[9]), ((pkt_data[10] << 8) + pkt_data[11]), ((pkt_data[12] << 8) + pkt_data[13]), ((pkt_data[14] << 8) + pkt_data[15]), ((pkt_data[16] << 8) + pkt_data[17]), ((pkt_data[18] << 8) + pkt_data[19]), ((pkt_data[20] << 8) + pkt_data[21]), ((pkt_data[22] << 8) + pkt_data[23]));
	global_d_ip.Format((CString)"%X:%X:%X:%X:%X:%X:%X:%X", ((pkt_data[24] << 8) + pkt_data[25]), ((pkt_data[26] << 8) + pkt_data[27]), ((pkt_data[28] << 8) + pkt_data[29]), ((pkt_data[30] << 8) + pkt_data[31]), ((pkt_data[32] << 8) + pkt_data[33]), ((pkt_data[34] << 8) + pkt_data[35]), ((pkt_data[36] << 8) + pkt_data[37]), ((pkt_data[38] << 8) + pkt_data[39]));

	//判断下一层报文使用的协议
	if (kind == 1) {
		return new ICMP(offset + 40);
	}
	else if (kind == 6) {
		return new TCP(offset + 40);
	}
	else if (kind == 17) {
		return new UDP(offset + 40);
	}
	else if (kind == 2) {
		return new IGMP(offset + 40);
	}

	return NULL; 
}
void IPv6::GetInfo(const u_char* pkt_data, std::map<CString, CString> &result){
	CString version;			//Version（版本）4比特 6（固定）
	CString traffic_level;		//流量级别，8比特
	CString traffic_class;		//Traffic Class(流量类别) 20比特 不固定
	CString payload_length;     //Payload length (净荷长度) 2字节 不固定（表示报头携带的数据）
	CString next_header;		//Next Header（下一报头） 1字节 有特殊含义
	CString hop_limit;			//Hop Limit（跳数限制） 1字节 255
	CString s_ip;				//源地址，16字节
	CString d_ip;				//目的地址，16字节

	version.Format((CString)"%d", pkt_data[0] & 0xf0);
	result[(CString)"版本"] = version;

	traffic_level.Format((CString)"%d", ((pkt_data[0] & 0x0f) << 4) + ((pkt_data[1] & 0xf0) >> 4) );
	result[(CString)"流量等级"] = traffic_level;

	traffic_class.Format((CString)"%d", ((pkt_data[1] & 0x0f) << 16) + (pkt_data[2] << 8) + pkt_data[3]);
	result[(CString)"流量类别"] = traffic_class;

	payload_length.Format((CString)"%d", (pkt_data[4] << 8) + pkt_data[5]);
	result[(CString)"净荷长度"] = payload_length;

	next_header.Format((CString)"%d", pkt_data[6]);
	result[(CString)"下一报头"] = next_header;

	hop_limit.Format((CString)"%d", pkt_data[7]);
	result[(CString)"跳限制"] = hop_limit;

	global_s_ip.Format((CString)"%X:%X:%X:%X:%X:%X:%X:%X", ((pkt_data[8] << 8) + pkt_data[9]), ((pkt_data[10] << 8) + pkt_data[11]), ((pkt_data[12] << 8) + pkt_data[13]), ((pkt_data[14] << 8) + pkt_data[15]), ((pkt_data[16] << 8) + pkt_data[17]), ((pkt_data[18] << 8) + pkt_data[19]), ((pkt_data[20] << 8) + pkt_data[21]), ((pkt_data[22] << 8) + pkt_data[23]));
	result[(CString)"源IP"] = s_ip;

	global_d_ip.Format((CString)"%X:%X:%X:%X:%X:%X:%X:%X", ((pkt_data[24] << 8) + pkt_data[25]), ((pkt_data[26] << 8) + pkt_data[27]), ((pkt_data[28] << 8) + pkt_data[29]), ((pkt_data[30] << 8) + pkt_data[31]), ((pkt_data[32] << 8) + pkt_data[33]), ((pkt_data[34] << 8) + pkt_data[35]), ((pkt_data[36] << 8) + pkt_data[37]), ((pkt_data[38] << 8) + pkt_data[39]));
	result[(CString)"目的IP"] = d_ip;
}

PackInfoAnalysis* ARP::NextProtocol(const u_char* pkt_data) { 
	//ARP之后不会有其他高层的协议
	global_s_ip.Format((CString)"%d.%d.%d.%d", pkt_data[14], pkt_data[15], pkt_data[16], pkt_data[17]);
	global_d_ip.Format((CString)"%d.%d.%d.%d", pkt_data[24], pkt_data[25], pkt_data[26], pkt_data[27]);
	
	return NULL;	
}
void ARP::GetInfo(const u_char* pkt_data, std::map<CString, CString> &result) {
	CString op;
	CString s_mac;
	CString s_ip;
	CString d_mac;
	CString d_ip;

	op.Format((CString)"%d", (pkt_data[6] << 8) + pkt_data[7]);
	result[(CString)"操作类型"] = op;

	s_mac.Format((CString)"%02X-%02X-%02X-%02X-%02X-%02X", pkt_data[8], pkt_data[9], pkt_data[10], pkt_data[11], pkt_data[12], pkt_data[13]);
	result[(CString)"源MAC"] = s_mac;

	s_ip.Format((CString)"%d.%d.%d.%d", pkt_data[14], pkt_data[15], pkt_data[16], pkt_data[17]);
	result[(CString)"源IP"] = s_ip;

	d_mac.Format((CString)"%02X-%02X-%02X-%02X-%02X-%02X", pkt_data[18], pkt_data[19], pkt_data[20], pkt_data[21], pkt_data[22], pkt_data[23]);
	result[(CString)"目的MAC"] = d_mac;

	d_ip.Format((CString)"%d.%d.%d.%d", pkt_data[24], pkt_data[25], pkt_data[26], pkt_data[27]);
	result[(CString)"目的IP"] = d_ip;
}

PackInfoAnalysis* ICMP::NextProtocol(const u_char* pkt_data) { 
	//ICMP之后没用更多协议
	return NULL; 
}
void ICMP::GetInfo(const u_char* pkt_data, std::map<CString, CString> &result) {
	CString kind;		//8位类型
	CString	code;		//8位代码
	CString verification;	//16位检验和

	kind.Format((CString)"%d", pkt_data[0]);
	result[(CString)"类型"] = kind;

	code.Format((CString)"%d", pkt_data[1]);
	result[(CString)"代码"] = code;

	verification.Format((CString)"%d", (pkt_data[2] << 8) + pkt_data[3]);
	result[(CString)"检验和"] = verification;
}

PackInfoAnalysis* TCP::NextProtocol(const u_char* pkt_data) { 
	int s_port = (pkt_data[0] << 8) + pkt_data[1];
	int d_port = (pkt_data[2] << 8) + pkt_data[3];
	int head_len = (pkt_data[12] & 0xf0) >> 4;

	global_ACK.Format((CString)"%d", (pkt_data[13] & 0x10) >> 4);
	global_SYN.Format((CString)"%d", (pkt_data[13] & 0x02) >> 1);
	global_seq.Format((CString)"%u", (pkt_data[4] << 24) + (pkt_data[5] << 16) + (pkt_data[6] << 8) + pkt_data[7]);
	global_ack.Format((CString)"%u", (pkt_data[8] << 24) + (pkt_data[9] << 16) + (pkt_data[10] << 8) + pkt_data[11]);
	global_FIN.Format((CString)"%d", pkt_data[13] & 0x01);
	global_s_port.Format((CString)"%d", (pkt_data[0] << 8) + pkt_data[1]);
	global_d_port.Format((CString)"%d", (pkt_data[2] << 8) + pkt_data[3]);

	if (s_port == 80 || d_port == 80)
	{
		return new HTTP(offset + head_len * 4);
	}
	else if (s_port == 21 || d_port == 21)
	{
		return new FTP(offset + head_len * 4);
	}
	else if (s_port == 443 || d_port == 443) {
		return new HTTPS(offset + head_len * 4);
	}

	return NULL; 
}
void TCP::GetInfo(const u_char* pkt_data, std::map<CString, CString> &result) {
	CString s_port;		//源端口
	CString d_port;		//目的端口
	CString seq;		//序号
	CString ack;		//确认号
	CString head_len;	//首部长度
	CString	ACK;		//确认ACK
	CString SYN;		//同步SYN
	CString FIN;		//终止FIN 
	CString window;		//窗口，运行发送的序号
	CString verification;		//检验和,检验范围包括首部和数据两部分



	s_port.Format((CString)"%d", (pkt_data[0] << 8) + pkt_data[1]);
	result[(CString)"源端口"] = s_port;

	d_port.Format((CString)"%d", (pkt_data[2] << 8) + pkt_data[3]);
	result[(CString)"目的端口"] = d_port;

	seq.Format((CString)"%u", (pkt_data[4] << 24) + (pkt_data[5] << 16) + (pkt_data[6] << 8) + pkt_data[7]);
	result[(CString)"序号"] = seq;

	ack.Format((CString)"%u", (pkt_data[8] << 24) + (pkt_data[9] << 16) + (pkt_data[10] << 8) + pkt_data[11]);
	result[(CString)"确认序号"] = ack;

	head_len.Format((CString)"%d", (pkt_data[12] & 0xf0) >> 4);
	result[(CString)"首部长度"] = head_len;

	ACK.Format((CString)"%d", (pkt_data[13] & 0x10) >> 4);
	result[(CString)"ACK"] = ACK;

	SYN.Format((CString)"%d", (pkt_data[13] & 0x02) >> 1);
	result[(CString)"SYN"] = SYN;

	FIN.Format((CString)"%d", pkt_data[13] & 0x01);
	result[(CString)"FIN"] = FIN;

	window.Format((CString)"%d", (pkt_data[14] << 8) + pkt_data[15]);
	result[(CString)"窗口"] = window;

	verification.Format((CString)"%d", (pkt_data[16] << 8) + pkt_data[17]);
	result[(CString)"检验和"] = verification;

}

PackInfoAnalysis* UDP::NextProtocol(const u_char* pkt_data) { 
	global_s_port.Format((CString)"%d", (pkt_data[0] << 8) + pkt_data[1]);
	global_d_port.Format((CString)"%d", (pkt_data[2] << 8) + pkt_data[3]);


	int s_port = (pkt_data[0] << 8) + pkt_data[1];
	int d_port = (pkt_data[2] << 8) + pkt_data[3];

	if (s_port == 53 || d_port == 53)
	{
		return new DNS(offset + 8);
	}
	

	return NULL; 
}
void UDP::GetInfo(const u_char* pkt_data, std::map<CString, CString> &result) {
	CString s_port;		//源端口
	CString d_port;		//目的端口
	CString len;	//首部长度
	CString verification;		//检验和,检验范围包括首部和数据两部分

	s_port.Format((CString)"%d", (pkt_data[0] << 8) + pkt_data[1]);
	result[(CString)"源端口"] = s_port;

	d_port.Format((CString)"%d", (pkt_data[2] << 8) + pkt_data[3]);
	result[(CString)"目的端口"] = d_port;

	len.Format((CString)"%d", (pkt_data[4] << 8) + pkt_data[5]);
	result[(CString)"长度"] = len;

	verification.Format((CString)"%d", (pkt_data[6] << 8) + pkt_data[7]);
	result[(CString)"检验和"] = verification;
}

PackInfoAnalysis* IGMP::NextProtocol(const u_char* pkt_data) { 
	return NULL; 
}
void IGMP::GetInfo(const u_char* pkt_data, std::map<CString, CString> &result) {
	CString type;	//IGMP报文类型
	CString	max_resp_code;	//最大响应时间
	CString checkSum;		//检验和
	CString group_address;	//组地址

	type.Format((CString)"%d", pkt_data[0]);
	result[(CString)"IGMP报文类型"] = type;

	max_resp_code.Format((CString)"%d", pkt_data[1]);
	result[(CString)"最大响应时间"] = max_resp_code;

	checkSum.Format((CString)"%d", (pkt_data[2] << 8) + pkt_data[3]);
	result[(CString)"检验和"] = checkSum;

	group_address.Format((CString)"%d.%d.%d.%d", pkt_data[4], pkt_data[5], pkt_data[6], pkt_data[7]);
	result[(CString)"组地址"] = group_address;

}

PackInfoAnalysis* OSPF::NextProtocol(const u_char* pkt_data) { return NULL; }
void OSPF::GetInfo(const u_char* pkt_data, std::map<CString, CString> &result) {
	CString version;		//版本号
	CString type;			//类型
	CString packet_len;		//包长度
	CString route_id;		//路由器ID
	CString area_id;		//区域ID
	CString	checkSum;		//检验和
	CString auType;			//认证类型

	version.Format((CString)"%d", pkt_data[0]);
	result[(CString)"OSPF版本"] = version;

	type.Format((CString)"%d", pkt_data[1]);
	result[(CString)"OSPF报文类型"] = type;

	packet_len.Format((CString)"%d", (pkt_data[2] << 8) + pkt_data[3]);
	result[(CString)"包长度"] = packet_len;

	route_id.Format((CString)"%d", (pkt_data[4] << 24) + (pkt_data[5] << 16) + (pkt_data[6] << 8) + pkt_data[7]);
	result[(CString)"路由器ID"] = route_id;

	area_id.Format((CString)"%d", (pkt_data[8] << 24) + (pkt_data[9] << 16) + (pkt_data[10] << 8) + pkt_data[11]);
	result[(CString)"区域ID"] = area_id;

	checkSum.Format((CString)"%d", (pkt_data[12] << 8) + pkt_data[13]);
	result[(CString)"检验和"] = checkSum;

	auType.Format((CString)"%d", (pkt_data[14] << 8) + pkt_data[15]);
	result[(CString)"认证类型"] = auType;
}

PackInfoAnalysis* DNS::NextProtocol(const u_char* pkt_data) { return NULL; }
void DNS::GetInfo(const u_char* pkt_data, std::map<CString, CString> &result) {
	
}

PackInfoAnalysis* HTTP::NextProtocol(const u_char* pkt_data) { return NULL; }

void HTTP::GetInfo(const u_char* pkt_data, std::map<CString, CString> &result) {

}


PackInfoAnalysis* HTTPS::NextProtocol(const u_char* pkt_data) { return NULL; }

void HTTPS::GetInfo(const u_char* pkt_data, std::map<CString, CString> &result) {

}

PackInfoAnalysis* FTP::NextProtocol(const u_char* pkt_data) { return NULL; }

void FTP::GetInfo(const u_char* pkt_data, std::map<CString, CString> &result) {

}


void PackInfoAnalysis::clear_global() {
	global_s_ip = "";
	global_d_ip = "";
	global_s_mac = "";
	global_d_mac = "";
	global_ACK = "";
	global_SYN = "";
	global_FIN = "";
	global_ack = "";
	global_seq = "";
	global_s_port = "";
	global_d_port = "";
}




