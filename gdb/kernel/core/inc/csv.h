
/*
* Geosciences Template Library
*
* Copyright (c) 2008
* Zhenwen He (zwhe@hotmail.com)
*
* Permission to use, copy, modify, distribute and sell this software
* and its documentation for any purpose is hereby granted without fee,
* provided that the above copyright notice appear in all copies and
* that both that copyright notice and this permission notice appear
* in supporting documentation.  Zhenwen He  makes no  representations
* about the suitability of this software for any purpose.
* It is provided "as is" without express or implied warranty.
*/
//Comma-Separated Values
#pragma once 
#include "config.h"
#include "value.h"
#include "csl.h"

begin_gtl_namespace
begin_gdb_namespace

enum CSVCompareCriteria {
	CC_ExactString,
	CC_ApproxString,
	CC_Integer,
	CC_Decimal
};
class CORE_API CommaSeparatedValues {
	/* Cache for whole file */
	int         nLineCount;//包含了字段行的数，如果文件有字段名这一行，则第0行为字段名集合
	char        **papszFieldNames;//拆分后的字段名数组
	char        **papszLines;//字符串的指针数组，数组中的每个元素指向每行的开始位置
	char        *pszRawData;//整个文件的数据存放在这里
	char         cSeparator;//记录分割符号
public:	
	CommaSeparatedValues();
	CommaSeparatedValues(const char * pathname);
	CommaSeparatedValues(const std::string& pathname);
	bool load(const char * pathname);
	bool load(const std::string &  pathname);
	void clear();
public:
	inline int  getFieldCount();
	inline int getLineCount();
	inline const char * getFieldName(int c);
	inline const char ** getFieldNames() { return (const char **)papszFieldNames; }
	inline const char * getLine(int r);
	inline char ** parseLine(int r);
	inline char ** getLine(const char * keyfieldname, const char * keyvalue, CSVCompareCriteria v);
	inline int indexField(const char* fieldname);
	inline int indexLine(const char* fieldname, const char * val);
	inline int indexLine(const char* fieldname, const char * val, CSVCompareCriteria v);
	inline std::string getValue(const char * keyfieldname, const char * keyvalue, const char * targetfieldname);
	inline std::string getValue(const char * keyfieldname, const char * keyvalue,CSVCompareCriteria v, const char * targetfieldname);
	inline char getSeparator();
public: 
	static char ** tokenizeLine(const char *pszLine, char chDelimiter);
	static char detectSeperator(const char* pszLine);
	static char *findNextLine(char *pszThisLine);
	static int compare(const char * scr, const char * dest, CSVCompareCriteria cc);
};

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
inline char CommaSeparatedValues::getSeparator(){ return this->cSeparator; }
inline int  CommaSeparatedValues::getFieldCount() {
	if (papszFieldNames)
		return cslCount(papszFieldNames);
	else
		return 0;
}
inline int CommaSeparatedValues::getLineCount() { return nLineCount; }
inline const char * CommaSeparatedValues::getFieldName(int c) {
	if (papszFieldNames)
		return papszFieldNames[c];
	else
		return 0;
}
inline const char * CommaSeparatedValues::getLine(int r) {
	if (papszLines && r <= nLineCount)
		return papszLines[r];
	else
		return 0;
}
inline char ** CommaSeparatedValues::parseLine(int r) {
	if (papszLines && r <= nLineCount) {
		return CommaSeparatedValues::tokenizeLine(papszLines[r], cSeparator); 
	}
	else
		return 0;
}
inline int CommaSeparatedValues::indexField(const char* fieldname) {
	if (papszFieldNames) {
		int c = cslCount(papszFieldNames);
		for (int i = 0; i < c; i++) {
			if (cslIEqualString(papszFieldNames[i], fieldname))
				return i;
		}
		return -1;
	}
	else
		return -1;
}
inline int CommaSeparatedValues::indexLine(const char* fieldname, const char * val) {
	int i = indexField(fieldname);
	if (i == -1)
		return -1;
	char ** psca = 0;
	const char * curLine = 0; 
	for (int k = 0; k < nLineCount; k++) {
		curLine = getLine(k);
		psca = tokenizeLine(curLine, cSeparator);
		if (cslIEqualString(psca[i], val)) {
			cslDestroy(psca);
			return k;
		}
		else {
			cslDestroy(psca);
			continue;
		}
	}
	return -1;
}
inline int CommaSeparatedValues::indexLine(const char* fieldname, const char * val, CSVCompareCriteria v){
	int i = -1;
	if((i=indexField(fieldname))==-1)
		return -1;
	char ** psca = 0;
	const char * curLine = 0;
	for (int k = 0; k < nLineCount; k++) {
		curLine = getLine(k);
		psca = tokenizeLine(curLine, cSeparator);
		if (compare(psca[i], val,v)==0) {
			cslDestroy(psca);
			return k;
		}
		else {
			cslDestroy(psca);
			continue;
		}
	}
	return -1;
}
inline std::string CommaSeparatedValues::getValue(const char * keyfieldname, const char * keyvalue, const char * targetfieldname) {
	std::string sz;
	int i = indexField(keyfieldname);
	int ti = indexField(targetfieldname);
	if (ti == -1 || i == -1)
		return sz;

	char ** psca = 0;
	const char * curLine = 0; 
	for (int k = 0; k < nLineCount; k++) {
		curLine = getLine(k); 
		psca = tokenizeLine(curLine, cSeparator);
		if (cslIEqualString(psca[i], keyvalue)) {
			sz = psca[ti];
			cslDestroy(psca);
			return sz;
		}
		else {
			cslDestroy(psca);
			continue;
		}
	}
	return sz;
}
inline std::string CommaSeparatedValues::getValue(const char * keyfieldname, const char * keyvalue,
	CSVCompareCriteria v, const char * targetfieldname)
{
	std::string sz;
	int i = indexField(keyfieldname);
	int ti = indexField(targetfieldname);
	if (ti == -1 || i == -1)
		return sz;

	char ** psca = 0;
	const char * curLine = 0; 
	for (int k = 0; k < nLineCount; k++) {
		curLine = getLine(k); 
		psca = tokenizeLine(curLine, cSeparator);
		if (compare(psca[i], keyvalue,v)==0) {
			sz = psca[ti];
			cslDestroy(psca);
			return sz;
		}
		else {
			cslDestroy(psca);
			continue;
		}
	}
	return sz;
}

inline char ** CommaSeparatedValues::getLine(const char * keyfieldname, const char * keyvalue, CSVCompareCriteria v) {
	int i = -1;
	if ((i = indexField(keyfieldname)) == -1)
		return 0;
	char ** psca = 0;
	const char * curLine = 0; 
	for (int k = 0; k < nLineCount; k++) {
		curLine = getLine(k); 
		psca = tokenizeLine(curLine, cSeparator);
		if (compare(psca[i], keyvalue, v) == 0) { 
			return psca;
		}
		else {
			cslDestroy(psca);
			continue;
		}
	}
	return psca;
}
end_gdb_namespace
end_gtl_namespace 

