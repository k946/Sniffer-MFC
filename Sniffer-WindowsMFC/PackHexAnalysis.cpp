#include "stdafx.h"
#include "PackHexAnalysis.h"

#define OFFSET(line, lienlen, offset) (line * lienlen + offset)  

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
		hexCount.Format((CString)"%04X:   ", i);
		result += hexCount;
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

/*
**重新计算格式化后的偏移量
**默认格式前一行的长度为16字节
** start格式化前的起点
** end格式前的终点
** offset格式后与格式化前的第一个字符的偏移
** lienLenght格式化后，一行的长度
** placehold 格式前的一个字节在格式后占用的字节 
*/
std::map<int, int> PackHexAnalysis::computeOffset(int start, int end, int offset, int lineLenght, int placehold) {
	std::map<int, int> result;
	int firstLine = start / 16;		//start所在的行
	int endLine = end / 16;			//end所在的行
	int firstLineOffset = start % 16;	//start与格式前的行首的距离
	int endLineOffset = end % 16;		//end与格式化前的行首的距离
	int startAfter;					//格式后start与行首的距离
	int endAfter;					//格式后start与行首的距离
	if (firstLine == endLine) {
		int startAfter = OFFSET(firstLine, lineLenght, offset) + firstLineOffset * placehold;
		int endAfter = OFFSET(firstLine, lineLenght, offset) + endLineOffset  * placehold;
		result[startAfter] = endAfter;
		return result;
	}

	int lineOffset = endLine - firstLine;//格式化前
	startAfter = OFFSET(firstLine, lineLenght, offset) + firstLineOffset * placehold;
	endAfter = OFFSET(firstLine, lineLenght, offset) + 16 * placehold;
	result[startAfter] = endAfter;
	startAfter = OFFSET(endLine, lineLenght, offset);
	endAfter = OFFSET(endLine, lineLenght, offset) + endLineOffset * placehold;
	result[startAfter] = endAfter;
	for (int i = 1; i < lineOffset; ++i) {
		startAfter = OFFSET(firstLine + i, lineLenght, offset);
		endAfter = OFFSET(firstLine + i, lineLenght, offset) + 16 * placehold;
		result[startAfter] = endAfter;
	}
	
	return result;

}

