#pragma once
class PackHexAnalysis
{
public:
	PackHexAnalysis();
	~PackHexAnalysis();
	//�ַ�ת16����
	static CString charToHex(const u_char * data, int size);
	//�ַ�תASCII
	static CString charToASCII(const u_char * data, int size);
	//�ַ�ת16���ƣ��ַ�֮���пո�ָ�
	static CString charFormatHex(const u_char * data, int size);
	//�ַ�ת16���Ƽ�ASCII���ַ�֮���пո�ָ�
	static CString charFormatHexASCII(const u_char * data, int size);
	//ʮ������ת�ַ�
	static u_char * hexToChar(const CString hex);
};

