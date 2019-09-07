#include "stdafx.h"
#include "PackInfoAnalysis.h"
#include <map>

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

PackInfoAnalysis* Ethernet::NextProtocol(const const u_char* pkt_data) {

	CString kind;
	CString kind_ip("0800");
	CString kind_arp("0806");
	CString kind_rarp("8035");
	CString kind_ipv6("86DD");

	kind.Format((CString)"%02X%02X", pkt_data[12], pkt_data[13]);

	//�ж���һ�㱨��ʹ�õ�Э��
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

	s_mac.Format((CString)"%02X-%02X-%02X-%02X-%02X-%02X", pkt_data[0], pkt_data[1], pkt_data[2], pkt_data[3], pkt_data[4], pkt_data[5]);
	d_mac.Format((CString)"%02X-%02X-%02X-%02X-%02X-%02X", pkt_data[6], pkt_data[7], pkt_data[8], pkt_data[9], pkt_data[10], pkt_data[11]);
	kind.Format((CString)"%02X%02X", pkt_data[12], pkt_data[13]);
	
	result[(CString)"ԴMAC"] = s_mac;
	result[(CString)"Ŀ��MAC"] = d_mac;
	result[(CString)"����"] = kind;
}


PackInfoAnalysis* IEEE802_3::NextProtocol(const u_char* pkt_data) {

	CString kind;

	kind.Format((CString)"%02X%02X", pkt_data[20], pkt_data[21]);

	//�ж���һ�㱨��ʹ�õ�Э��
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
	
	result[(CString)"ԴMAC"] = s_mac;
	result[(CString)"Ŀ��MAC"] = d_mac;
	result[(CString)"����"] = kind;
}

PackInfoAnalysis* IP::NextProtocol(const u_char* pkt_data) {
	int kind = pkt_data[9];
	int len = pkt_data[0] & 0x0f;

	global_s_ip.Format((CString)"%d.%d.%d.%d", pkt_data[12], pkt_data[13], pkt_data[14], pkt_data[15]);
	global_d_ip.Format((CString)"%d.%d.%d.%d", pkt_data[16], pkt_data[17], pkt_data[18], pkt_data[19]);


	//�ж���һ�㱨��ʹ�õ�Э��
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
	CString version;		//�汾��IP���İ汾�� IPV4:4��IPV6:6 
	CString IHL;			//�ײ����ȣ�IP header ���ȣ�û��ѡ���һ��Ϊ5��5x32bit��20B��
	CString total_len;		//�ܳ��ȣ�header������ �ܳ��� 
	CString id;				//16λ��ʶ��IP ���ĵ�Ψһid����Ƭ���ĵ�id ��ͬ�����ڽ�������
	CString fragment;		//3λ��־�������Ƿ��Ƭ��
	CString offset;			//13λƬƫ��
	CString TTL;			//TTL������ʱ�䣬��·������������ÿ����һ��·��������TTL ��һ�����·������Ҫ���¼���IP���ĵ�У���
	CString protocol;		//8λЭ�飺ICMP��1��TCP��6��UDP��17�����������аٶ�
	CString	verification;	//�ײ�У��ͣ�IP headerУ��ͣ����ն��յ����Ľ��м������У��ʹ���ֱ�Ӷ���
	CString s_ip;			//ԴIP��ַ
	CString d_ip;			//Ŀ��IP��ַ


	version.Format((CString)"%d", (pkt_data[0] & 0xf0) >> 4);
	result[(CString)"�汾"] = version;

	IHL.Format((CString)"%d", pkt_data[0] & 0x0f);
	result[(CString)"�ײ�����"] = IHL;

	total_len.Format((CString)"%d", (pkt_data[2] << 8) + pkt_data[3]);
	result[(CString)"�ܳ���"] = total_len;

	id.Format((CString)"%d", (pkt_data[4] << 8) + pkt_data[5]);
	result[(CString)"��ʶ"] = id;

	fragment.Format((CString)"%d", (pkt_data[6] & 0xe0) >> 5);
	result[(CString)"��־"] = fragment;

	offset.Format((CString)"%d", ((pkt_data[6] & 0x1f) << 8) + pkt_data[7]);
	result[(CString)"ƫ��"] = offset;

	TTL.Format((CString)"%d", pkt_data[8]);
	result[(CString)"����ʱ��"] = TTL;

	protocol.Format((CString)"%d", pkt_data[9]);
	result[(CString)"Э��"] = protocol;

	verification.Format((CString)"%d", (pkt_data[10] << 8) + pkt_data[11]);
	result[(CString)"�ײ������"] = verification;

	s_ip.Format((CString)"%d.%d.%d.%d", pkt_data[12], pkt_data[13], pkt_data[14], pkt_data[15]);
	result[(CString)"ԴIP"] = s_ip;

	d_ip.Format((CString)"%d.%d.%d.%d", pkt_data[16], pkt_data[17], pkt_data[18], pkt_data[19]);
	result[(CString)"Ŀ��IP"] = d_ip;
}


PackInfoAnalysis* IPv6::NextProtocol(const u_char* pkt_data) { 
	int kind = pkt_data[6];

	global_s_ip.Format((CString)"%X:%X:%X:%X:%X:%X:%X:%X", ((pkt_data[8] << 8) + pkt_data[9]), ((pkt_data[10] << 8) + pkt_data[11]), ((pkt_data[12] << 8) + pkt_data[13]), ((pkt_data[14] << 8) + pkt_data[15]), ((pkt_data[16] << 8) + pkt_data[17]), ((pkt_data[18] << 8) + pkt_data[19]), ((pkt_data[20] << 8) + pkt_data[21]), ((pkt_data[22] << 8) + pkt_data[23]));
	global_d_ip.Format((CString)"%X:%X:%X:%X:%X:%X:%X:%X", ((pkt_data[24] << 8) + pkt_data[25]), ((pkt_data[26] << 8) + pkt_data[27]), ((pkt_data[28] << 8) + pkt_data[29]), ((pkt_data[30] << 8) + pkt_data[31]), ((pkt_data[32] << 8) + pkt_data[33]), ((pkt_data[34] << 8) + pkt_data[35]), ((pkt_data[36] << 8) + pkt_data[37]), ((pkt_data[38] << 8) + pkt_data[39]));

	//�ж���һ�㱨��ʹ�õ�Э��
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
	CString version;			//Version���汾��4���� 6���̶���
	CString traffic_level;		//��������8����
	CString traffic_class;		//Traffic Class(�������) 20���� ���̶�
	CString payload_length;     //Payload length (���ɳ���) 2�ֽ� ���̶�����ʾ��ͷЯ�������ݣ�
	CString next_header;		//Next Header����һ��ͷ�� 1�ֽ� �����⺬��
	CString hop_limit;			//Hop Limit���������ƣ� 1�ֽ� 255
	CString s_ip;				//Դ��ַ��16�ֽ�
	CString d_ip;				//Ŀ�ĵ�ַ��16�ֽ�

	version.Format((CString)"%d", pkt_data[0] & 0xf0);
	result[(CString)"�汾"] = version;

	traffic_level.Format((CString)"%d", ((pkt_data[0] & 0x0f) << 4) + ((pkt_data[1] & 0xf0) >> 4) );
	result[(CString)"�����ȼ�"] = traffic_level;

	traffic_class.Format((CString)"%d", ((pkt_data[1] & 0x0f) << 16) + (pkt_data[2] << 8) + pkt_data[3]);
	result[(CString)"�������"] = traffic_class;

	payload_length.Format((CString)"%d", (pkt_data[4] << 8) + pkt_data[5]);
	result[(CString)"���ɳ���"] = payload_length;

	next_header.Format((CString)"%d", pkt_data[6]);
	result[(CString)"��һ��ͷ"] = next_header;

	hop_limit.Format((CString)"%d", pkt_data[7]);
	result[(CString)"������"] = hop_limit;

	global_s_ip.Format((CString)"%X:%X:%X:%X:%X:%X:%X:%X", ((pkt_data[8] << 8) + pkt_data[9]), ((pkt_data[10] << 8) + pkt_data[11]), ((pkt_data[12] << 8) + pkt_data[13]), ((pkt_data[14] << 8) + pkt_data[15]), ((pkt_data[16] << 8) + pkt_data[17]), ((pkt_data[18] << 8) + pkt_data[19]), ((pkt_data[20] << 8) + pkt_data[21]), ((pkt_data[22] << 8) + pkt_data[23]));
	result[(CString)"ԴIP"] = s_ip;

	global_d_ip.Format((CString)"%X:%X:%X:%X:%X:%X:%X:%X", ((pkt_data[24] << 8) + pkt_data[25]), ((pkt_data[26] << 8) + pkt_data[27]), ((pkt_data[28] << 8) + pkt_data[29]), ((pkt_data[30] << 8) + pkt_data[31]), ((pkt_data[32] << 8) + pkt_data[33]), ((pkt_data[34] << 8) + pkt_data[35]), ((pkt_data[36] << 8) + pkt_data[37]), ((pkt_data[38] << 8) + pkt_data[39]));
	result[(CString)"Ŀ��IP"] = d_ip;
}

PackInfoAnalysis* ARP::NextProtocol(const u_char* pkt_data) { 
	//ARP֮�󲻻��������߲��Э��
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
	result[(CString)"��������"] = op;

	s_mac.Format((CString)"%02X-%02X-%02X-%02X-%02X-%02X", pkt_data[8], pkt_data[9], pkt_data[10], pkt_data[11], pkt_data[12], pkt_data[13]);
	result[(CString)"ԴMAC"] = s_mac;

	s_ip.Format((CString)"%d.%d.%d.%d", pkt_data[14], pkt_data[15], pkt_data[16], pkt_data[17]);
	result[(CString)"ԴIP"] = s_ip;

	d_mac.Format((CString)"%02X-%02X-%02X-%02X-%02X-%02X", pkt_data[18], pkt_data[19], pkt_data[20], pkt_data[21], pkt_data[22], pkt_data[23]);
	result[(CString)"Ŀ��MAC"] = d_mac;

	d_ip.Format((CString)"%d.%d.%d.%d", pkt_data[24], pkt_data[25], pkt_data[26], pkt_data[27]);
	result[(CString)"Ŀ��IP"] = d_ip;
}

PackInfoAnalysis* ICMP::NextProtocol(const u_char* pkt_data) { 
	//ICMP֮��û�ø���Э��
	return NULL; 
}
void ICMP::GetInfo(const u_char* pkt_data, std::map<CString, CString> &result) {
	CString kind;		//8λ����
	CString	code;		//8λ����
	CString verification;	//16λ�����

	kind.Format((CString)"%d", pkt_data[0]);
	result[(CString)"����"] = kind;

	code.Format((CString)"%d", pkt_data[1]);
	result[(CString)"����"] = code;

	verification.Format((CString)"%d", (pkt_data[2] << 8) + pkt_data[3]);
	result[(CString)"�����"] = verification;
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
	CString s_port;		//Դ�˿�
	CString d_port;		//Ŀ�Ķ˿�
	CString seq;		//���
	CString ack;		//ȷ�Ϻ�
	CString head_len;	//�ײ�����
	CString	ACK;		//ȷ��ACK
	CString SYN;		//ͬ��SYN
	CString FIN;		//��ֹFIN 
	CString window;		//���ڣ����з��͵����
	CString verification;		//�����,���鷶Χ�����ײ�������������



	s_port.Format((CString)"%d", (pkt_data[0] << 8) + pkt_data[1]);
	result[(CString)"Դ�˿�"] = s_port;

	d_port.Format((CString)"%d", (pkt_data[2] << 8) + pkt_data[3]);
	result[(CString)"Ŀ�Ķ˿�"] = d_port;

	seq.Format((CString)"%u", (pkt_data[4] << 24) + (pkt_data[5] << 16) + (pkt_data[6] << 8) + pkt_data[7]);
	result[(CString)"���"] = seq;

	ack.Format((CString)"%u", (pkt_data[8] << 24) + (pkt_data[9] << 16) + (pkt_data[10] << 8) + pkt_data[11]);
	result[(CString)"ȷ�����"] = ack;

	head_len.Format((CString)"%d", (pkt_data[12] & 0xf0) >> 4);
	result[(CString)"�ײ�����"] = head_len;

	ACK.Format((CString)"%d", (pkt_data[13] & 0x10) >> 4);
	result[(CString)"ACK"] = ACK;

	SYN.Format((CString)"%d", (pkt_data[13] & 0x02) >> 1);
	result[(CString)"SYN"] = SYN;

	FIN.Format((CString)"%d", pkt_data[13] & 0x01);
	result[(CString)"FIN"] = FIN;

	window.Format((CString)"%d", (pkt_data[14] << 8) + pkt_data[15]);
	result[(CString)"����"] = window;

	verification.Format((CString)"%d", (pkt_data[16] << 8) + pkt_data[17]);
	result[(CString)"�����"] = verification;

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
	CString s_port;		//Դ�˿�
	CString d_port;		//Ŀ�Ķ˿�
	CString len;	//�ײ�����
	CString verification;		//�����,���鷶Χ�����ײ�������������

	s_port.Format((CString)"%d", (pkt_data[0] << 8) + pkt_data[1]);
	result[(CString)"Դ�˿�"] = s_port;

	d_port.Format((CString)"%d", (pkt_data[2] << 8) + pkt_data[3]);
	result[(CString)"Ŀ�Ķ˿�"] = d_port;

	len.Format((CString)"%d", (pkt_data[4] << 8) + pkt_data[5]);
	result[(CString)"����"] = len;

	verification.Format((CString)"%d", (pkt_data[6] << 8) + pkt_data[7]);
	result[(CString)"�����"] = verification;
}

PackInfoAnalysis* IGMP::NextProtocol(const u_char* pkt_data) { 
	return NULL; 
}
void IGMP::GetInfo(const u_char* pkt_data, std::map<CString, CString> &result) {
	CString type;	//IGMP��������
	CString	max_resp_code;	//�����Ӧʱ��
	CString checkSum;		//�����
	CString group_address;	//���ַ

	type.Format((CString)"%d", pkt_data[0]);
	result[(CString)"IGMP��������"] = type;

	max_resp_code.Format((CString)"%d", pkt_data[1]);
	result[(CString)"�����Ӧʱ��"] = max_resp_code;

	checkSum.Format((CString)"%d", (pkt_data[2] << 8) + pkt_data[3]);
	result[(CString)"�����"] = checkSum;

	group_address.Format((CString)"%d.%d.%d.%d", pkt_data[4], pkt_data[5], pkt_data[6], pkt_data[7]);
	result[(CString)"���ַ"] = group_address;

}

PackInfoAnalysis* OSPF::NextProtocol(const u_char* pkt_data) { return NULL; }
void OSPF::GetInfo(const u_char* pkt_data, std::map<CString, CString> &result) {
	CString version;		//�汾��
	CString type;			//����
	CString packet_len;		//������
	CString route_id;		//·����ID
	CString area_id;		//����ID
	CString	checkSum;		//�����
	CString auType;			//��֤����

	version.Format((CString)"%d", pkt_data[0]);
	result[(CString)"OSPF�汾"] = version;

	type.Format((CString)"%d", pkt_data[1]);
	result[(CString)"OSPF��������"] = type;

	packet_len.Format((CString)"%d", (pkt_data[2] << 8) + pkt_data[3]);
	result[(CString)"������"] = packet_len;

	route_id.Format((CString)"%d", (pkt_data[4] << 24) + (pkt_data[5] << 16) + (pkt_data[6] << 8) + pkt_data[7]);
	result[(CString)"·����ID"] = route_id;

	area_id.Format((CString)"%d", (pkt_data[8] << 24) + (pkt_data[9] << 16) + (pkt_data[10] << 8) + pkt_data[11]);
	result[(CString)"����ID"] = area_id;

	checkSum.Format((CString)"%d", (pkt_data[12] << 8) + pkt_data[13]);
	result[(CString)"�����"] = checkSum;

	auType.Format((CString)"%d", (pkt_data[14] << 8) + pkt_data[15]);
	result[(CString)"��֤����"] = auType;
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




