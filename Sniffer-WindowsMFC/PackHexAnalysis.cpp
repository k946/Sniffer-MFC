#include "stdafx.h"
#include "PackHexAnalysis.h"


PackHexAnalysis::PackHexAnalysis()
{
}


PackHexAnalysis::~PackHexAnalysis()
{
}


CString PackHexAnalysis::charToHex(const u_char * data, int size) {
	CString result;
	CString temp;
	int i;
	for (i = 0; i < size - 1; ++i) {
		temp.Format((CString)"%02X ", data[i]);
		result += temp;
	}
	temp.Format((CString)"%02X", data[size-1]);
	result += temp;

	return result;
}

CString PackHexAnalysis::charToASCII(const u_char * data, int size) {
	CString result;
	int i;
	char ch;
	for (i = 0; i < size; ++i) {
		ch = (unsigned char)data[i];
		result += _istprint(ch) ? ch : '.';
	}
	
	return result;
}


CString PackHexAnalysis::charFormatHex(const u_char * pkt_data, int size) {
	//获取16进制及ASCII
	int i;
	CString result;
	for (i = 0; i + 16 < size; i = i + 16) {
		result += PackHexAnalysis::charToHex(pkt_data + i, 8);
		result += (CString)"-" + PackHexAnalysis::charToHex(pkt_data + i + 8, 8);
		result += (CString)"\r\n";
	}

	if (size - i <= 8) {
		result += PackHexAnalysis::charToHex(pkt_data + i, size - i);
	}
	else {
		result += PackHexAnalysis::charToHex(pkt_data + i, 8);
		result += (CString)"-" + PackHexAnalysis::charToHex(pkt_data + i + 8, size - i - 8);
	}
	return result;
}


CString PackHexAnalysis::charFormatHexASCII(const u_char * pkt_data, int size) {
	CString hexCount;
	CString result;
	int i;
	//获取16进制及ASCII
	for (i = 0; i + 16 < size; i = i + 16) {
		//hexCount.Format((CString)"%04X:   ", i);
		//result += hexCount;
		result += PackHexAnalysis::charToHex(pkt_data + i, 8);
		result += (CString)"-" + PackHexAnalysis::charToHex(pkt_data + i + 8, 8);
		result += "    ";
		result += PackHexAnalysis::charToASCII(pkt_data + i, 8);
		result += PackHexAnalysis::charToASCII(pkt_data + i + 8, 8);
		result += (CString)"\r\n";
	}

	hexCount.Format((CString)"%04X:   ", i);
	result += hexCount;
	if (size - i <= 8) {
		result += PackHexAnalysis::charToHex(pkt_data + i, size - i);

		for (int j = 0; j < 16 - size + i; ++j) {
			result += "   ";
		}
		result += "    ";
		result += PackHexAnalysis::charToASCII(pkt_data + i, size - i);
	}
	else {
		result += PackHexAnalysis::charToHex(pkt_data + i, 8);
		result += (CString)"-" + PackHexAnalysis::charToHex(pkt_data + i + 8, size - i - 8);
		for (int j = 0; j < 16 - size + i; ++j) {
			result += "   ";
		}
		result += "    ";
		result += PackHexAnalysis::charToASCII(pkt_data + i, 8);
		result += PackHexAnalysis::charToASCII(pkt_data + i + 8, size - i - 8);
	}
	return result;
}


u_char * PackHexAnalysis::hexToChar(const CString hex) {

	int len = hex.GetLength();

	u_char* result = new u_char[len / 3 + 1];
	u_char c;
	char s[2];

	for (int i = 0; i + 1 < len; i = i + 3) {
		s[0] = hex.GetAt(i);
		s[1] = hex.GetAt(i + 1);
		sscanf(s, "%X", &result[i / 3]);
	}
	return result;
}