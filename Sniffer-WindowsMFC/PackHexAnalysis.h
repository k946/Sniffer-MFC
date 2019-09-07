#pragma once
class PackHexAnalysis
{
public:
	PackHexAnalysis();
	~PackHexAnalysis();
	//字符转16进制
	static CString charToHex(const u_char * data, int size);
	//字符转ASCII
	static CString charToASCII(const u_char * data, int size);
	//字符转16进制，字符之间有空格分隔
	static CString charFormatHex(const u_char * data, int size);
	//字符转16进制及ASCII，字符之间有空格分隔
	static CString charFormatHexASCII(const u_char * data, int size);
	//十六进制转字符
	static u_char * hexToChar(const CString hex);
};

