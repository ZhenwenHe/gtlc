#include "valutils.h" 

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

bool ValUtils::_numb_to_numb(VALUE & g, GVT pt){
	double * da = 0;
	int n = 0;
	toNumber(g, &da, &n);

	if (n == 0 || da == 0)
		return false;

	switch (pt)
	{
	case GVT_UCHAR:{
		unsigned char * p = new unsigned char[g.count];
		for (int i = 0; i < g.count; i++){
			p[i] = (unsigned char)da[i];
		}
		delete[](unsigned char*)g.pvalue;
		g.pvalue = p;
		g.type = pt;
	}
	case GVT_SHORT:{
		short * p = new short[g.count];
		for (int i = 0; i < g.count; i++){
			p[i] = (short)da[i];
		}
		delete[](unsigned char*)g.pvalue;
		g.pvalue = p;
		g.type = pt;
	}
	case GVT_USHORT:{
		unsigned short * p = new unsigned short[g.count];
		for (int i = 0; i < g.count; i++){
			p[i] = (unsigned short)da[i];
		}
		delete[](unsigned char*)g.pvalue;
		g.pvalue = p;
		g.type = pt;
	}
	case GVT_INT:{
		int * p = new int[g.count];
		for (int i = 0; i < g.count; i++){
			p[i] = (int)da[i];
		}
		delete[](unsigned char*)g.pvalue;
		g.pvalue = p;
		g.type = pt;
	}
	case GVT_UINT:{
		unsigned int * p = new unsigned int[g.count];
		for (int i = 0; i < g.count; i++){
			p[i] = (unsigned int)da[i];
		}
		delete[](unsigned char*)g.pvalue;
		g.pvalue = p;
		g.type = pt;
	}
	case GVT_LONG:{
		long * p = new long[g.count];
		for (int i = 0; i < g.count; i++){
			p[i] = (long)da[i];
		}
		delete[](unsigned char*)g.pvalue;
		g.pvalue = p;
		g.type = pt;
	}
	case GVT_ULONG:{
		unsigned long * p = new unsigned long[g.count];
		for (int i = 0; i < g.count; i++){
			p[i] = (unsigned long)da[i];
		}
		delete[](unsigned char*)g.pvalue;
		g.pvalue = p;
		g.type = pt;
	}
	case GVT_ULONGLONG:{
		unsigned long long * p = new unsigned long long[g.count];
		for (int i = 0; i < g.count; i++){
			p[i] = (unsigned long long)da[i];
		}
		delete[](unsigned char*)g.pvalue;
		g.pvalue = p;
		g.type = pt;
	}
	case GVT_FLOAT:{
		float* p = new float[g.count];
		for (int i = 0; i < g.count; i++){
			p[i] = (float)da[i];
		}
		delete[](unsigned char*)g.pvalue;
		g.pvalue = p;
		g.type = pt;
	}
	case GVT_DOUBLE:{
		double * p = new double[g.count];
		for (int i = 0; i < g.count; i++){
			p[i] = da[i];
		}
		delete[](unsigned char*)g.pvalue;
		g.pvalue = p;
		g.type = pt;
	}
	default:
	{
		if (da != 0)
			delete[] da;
		return false;
	}
	}
	if (da != 0)
		delete[] da;
	return true;
}
bool ValUtils::_numb_to_char(VALUE & g, GVT pt){
	if (pt == GVT_CHAR){
		switch (g.type)
		{
		case GVT_UCHAR:{
			g.type = pt;
			break;
		}
		case GVT_SHORT:{
			char * p = new char[g.count];
			for (int i = 0; i < g.count; i++){
				p[i] = (char)((short*)g.pvalue)[i];
			}
			delete[](short*)g.pvalue;
			g.pvalue = p;
			g.type = pt;
			break;
		}
		case GVT_USHORT:{
			char * p = new char[g.count];
			for (int i = 0; i < g.count; i++){
				p[i] = (char)((unsigned short*)g.pvalue)[i];
			}
			delete[](unsigned short*)g.pvalue;
			g.pvalue = p;
			g.type = pt;
			break;
		}
		case GVT_INT:{
			char * p = new char[g.count];
			for (int i = 0; i < g.count; i++){
				p[i] = ((int*)g.pvalue)[i];
			}
			delete[](int*)g.pvalue;
			g.pvalue = p;
			g.type = pt;
			break;
		}
		case GVT_UINT:{
			char * p = new char[g.count];
			for (int i = 0; i < g.count; i++){
				p[i] = ((unsigned int*)g.pvalue)[i];
			}
			delete[](unsigned int*)g.pvalue;
			g.pvalue = p;
			g.type = pt;
			break;
		}
		case GVT_LONG:{
			char * p = new char[g.count];
			for (int i = 0; i < g.count; i++){
				p[i] = (char)((long*)g.pvalue)[i];
			}
			delete[](long*)g.pvalue;
			g.pvalue = p;
			g.type = pt;
			break;
		}
		case GVT_ULONG:{
			char * p = new char[g.count];
			for (int i = 0; i < g.count; i++){
				p[i] = (char)((unsigned long*)g.pvalue)[i];
			}
			delete[](unsigned long*)g.pvalue;
			g.pvalue = p;
			g.type = pt;
			break;
		}
		case GVT_ULONGLONG:{
			char * p = new char[g.count];
			for (int i = 0; i < g.count; i++){
				p[i] = (char)((unsigned long long *)g.pvalue)[i];
			}
			delete[](unsigned long long *)g.pvalue;
			g.pvalue = p;
			g.type = pt;
			break;
		}
		case GVT_FLOAT:{
			char * p = new char[g.count];
			for (int i = 0; i < g.count; i++){
				p[i] = (char)((float*)g.pvalue)[i];
			}
			delete[](float*)g.pvalue;
			g.pvalue = p;
			g.type = pt;
			break;
		}
		case GVT_DOUBLE:{
			char * p = new char[g.count];
			for (int i = 0; i < g.count; i++){
				p[i] = (char)((double*)g.pvalue)[i];
			}
			delete[](double*)g.pvalue;
			g.pvalue = p;
			g.type = pt;
			break;
		}
		default:
			return false;
		}
		return true;
	}
	else if (pt == GVT_WCHAR){
		switch (g.type)
		{
		case GVT_UCHAR:{
			g.type = pt;
			break;
		}
		case GVT_SHORT:{
			wchar_t * p = new wchar_t[g.count];
			for (int i = 0; i < g.count; i++){
				p[i] = ((short*)g.pvalue)[i];
			}
			delete[](short*)g.pvalue;
			g.pvalue = p;
			g.type = pt;
			break;
		}
		case GVT_USHORT:{
			wchar_t * p = new wchar_t[g.count];
			for (int i = 0; i < g.count; i++){
				p[i] = ((unsigned short*)g.pvalue)[i];
			}
			delete[](unsigned short*)g.pvalue;
			g.pvalue = p;
			g.type = pt;
			break;
		}
		case GVT_INT:{
			wchar_t * p = new wchar_t[g.count];
			for (int i = 0; i < g.count; i++){
				p[i] = ((int*)g.pvalue)[i];
			}
			delete[](int*)g.pvalue;
			g.pvalue = p;
			g.type = pt;
			break;
		}
		case GVT_UINT:{
			wchar_t * p = new wchar_t[g.count];
			for (int i = 0; i < g.count; i++){
				p[i] = ((unsigned int*)g.pvalue)[i];
			}
			delete[](unsigned int*)g.pvalue;
			g.pvalue = p;
			g.type = pt;
			break;
		}
		case GVT_LONG:{
			wchar_t * p = new wchar_t[g.count];
			for (int i = 0; i < g.count; i++){
				p[i] = (wchar_t)((long*)g.pvalue)[i];
			}
			delete[](long*)g.pvalue;
			g.pvalue = p;
			g.type = pt;
			break;
		}
		case GVT_ULONG:{
			wchar_t * p = new wchar_t[g.count];
			for (int i = 0; i < g.count; i++){
				p[i] = (wchar_t)((unsigned long*)g.pvalue)[i];
			}
			delete[](unsigned long*)g.pvalue;
			g.pvalue = p;
			g.type = pt;
			break;
		}
		case GVT_ULONGLONG:{
			wchar_t * p = new wchar_t[g.count];
			for (int i = 0; i < g.count; i++){
				p[i] = (wchar_t)((unsigned long long*)g.pvalue)[i];
			}
			delete[](unsigned long long *)g.pvalue;
			g.pvalue = p;
			g.type = pt;
			break;
		}
		case GVT_FLOAT:{
			wchar_t * p = new wchar_t[g.count];
			for (int i = 0; i < g.count; i++){
				p[i] = (wchar_t)((float*)g.pvalue)[i];
			}
			delete[](float*)g.pvalue;
			g.pvalue = p;
			g.type = pt;
			break;
		}
		case GVT_DOUBLE:{
			wchar_t * p = new wchar_t[g.count];
			for (int i = 0; i < g.count; i++){
				p[i] = (wchar_t)((double*)g.pvalue)[i];
			}
			delete[](double*)g.pvalue;
			g.pvalue = p;
			g.type = pt;
			break;
		}
		default:
			return false;
		}
		return true;
	}
	return false;
}
bool ValUtils::_char_to_char(VALUE & g, GVT pt){
	if (pt == GVT_CHAR){//宽字符转换成8位字符
		char * p = new char[g.count];
		wchar_t * pp = (wchar_t*)g.pvalue;
		wchar_t* pp0 = pp;
		g.pvalue = p;
		for (int i = 0; i < g.count; i++)
		{
			*p = (char)*pp;
			pp++;
			p++;
		}
		delete[] pp0;
		g.type = pt;
		return true;
	}
	else {//8位字符转换成宽字符
		wchar_t * p = new wchar_t[g.count];
		char * pp = (char*)g.pvalue;
		char * pp0 = pp;
		g.pvalue = p;
		for (int i = 0; i < g.count; i++)
		{
			*p = *pp;
			pp++;
			p++;
		}
		delete[] pp0;
		g.type = pt;
		return true;
	}
	return false;
}
bool ValUtils::_char_to_numb(VALUE & g, GVT pt){
	if (g.type == GVT_CHAR){
		switch (pt)
		{
		case GVT_UCHAR:{
			g.type = pt;
			break;
		}
		case GVT_SHORT:{
			short * p = new short[g.count];
			for (int i = 0; i < g.count; i++){
				p[i] = ((char*)g.pvalue)[i];
			}
			delete[](char*)g.pvalue;
			g.pvalue = p;
			g.type = pt;
			break;
		}
		case GVT_USHORT:{
			unsigned short * p = new unsigned short[g.count];
			for (int i = 0; i < g.count; i++){
				p[i] = ((char*)g.pvalue)[i];
			}
			delete[](char*)g.pvalue;
			g.pvalue = p;
			g.type = pt;
			break;
		}
		case GVT_INT:{
			int * p = new int[g.count];
			for (int i = 0; i < g.count; i++){
				p[i] = ((char*)g.pvalue)[i];
			}
			delete[](char*)g.pvalue;
			g.pvalue = p;
			g.type = pt;
			break;
		}
		case GVT_UINT:{
			unsigned int * p = new unsigned int[g.count];
			for (int i = 0; i < g.count; i++){
				p[i] = ((char*)g.pvalue)[i];
			}
			delete[](char*)g.pvalue;
			g.pvalue = p;
			g.type = pt;
			break;
		}
		case GVT_LONG:{
			long * p = new long[g.count];
			for (int i = 0; i < g.count; i++){
				p[i] = ((char*)g.pvalue)[i];
			}
			delete[](char*)g.pvalue;
			g.pvalue = p;
			g.type = pt;
			break;
		}
		case GVT_ULONG:{
			unsigned long * p = new unsigned long[g.count];
			for (int i = 0; i < g.count; i++){
				p[i] = ((char*)g.pvalue)[i];
			}
			delete[](char*)g.pvalue;
			g.pvalue = p;
			g.type = pt;
			break;
		}
		case GVT_ULONGLONG:{
			unsigned long long * p = new unsigned long long[g.count];
			for (int i = 0; i < g.count; i++){
				p[i] = ((char*)g.pvalue)[i];
			}
			delete[](char*)g.pvalue;
			g.pvalue = p;
			g.type = pt;
			break;
		}
		case GVT_FLOAT:{
			float * p = new float[g.count];
			for (int i = 0; i < g.count; i++){
				p[i] = ((char*)g.pvalue)[i];
			}
			delete[](char*)g.pvalue;
			g.pvalue = p;
			g.type = pt;
			break;
		}
		case GVT_DOUBLE:{
			double * p = new double[g.count];
			for (int i = 0; i < g.count; i++){
				p[i] = ((char*)g.pvalue)[i];
			}
			delete[](char*)g.pvalue;
			g.pvalue = p;
			g.type = pt;
			break;
		}
		default:
			return false;
		}
		return true;
	}
	else if (g.type == GVT_WCHAR){
		switch (pt)
		{
		case GVT_UCHAR:{
			unsigned char * p = new unsigned char[g.count];
			for (int i = 0; i < g.count; i++){
				p[i] = (char)((wchar_t*)g.pvalue)[i];
			}
			delete[](wchar_t*)g.pvalue;
			g.pvalue = p;
			g.type = pt;
			break;
		}
		case GVT_SHORT:{
			short * p = new short[g.count];
			for (int i = 0; i < g.count; i++){
				p[i] = ((wchar_t*)g.pvalue)[i];
			}
			delete[](wchar_t*)g.pvalue;
			g.pvalue = p;
			g.type = pt;
			break;
		}
		case GVT_USHORT:{
			unsigned short * p = new unsigned short[g.count];
			for (int i = 0; i < g.count; i++){
				p[i] = ((wchar_t*)g.pvalue)[i];
			}
			delete[](wchar_t*)g.pvalue;
			g.pvalue = p;
			g.type = pt;
			break;
		}
		case GVT_INT:{
			int * p = new int[g.count];
			for (int i = 0; i < g.count; i++){
				p[i] = ((wchar_t*)g.pvalue)[i];
			}
			delete[](wchar_t*)g.pvalue;
			g.pvalue = p;
			g.type = pt;
			break;
		}
		case GVT_UINT:{
			unsigned int * p = new unsigned int[g.count];
			for (int i = 0; i < g.count; i++){
				p[i] = ((wchar_t*)g.pvalue)[i];
			}
			delete[](wchar_t*)g.pvalue;
			g.pvalue = p;
			g.type = pt;
			break;
		}
		case GVT_LONG:{
			long * p = new long[g.count];
			for (int i = 0; i < g.count; i++){
				p[i] = ((wchar_t*)g.pvalue)[i];
			}
			delete[](wchar_t*)g.pvalue;
			g.pvalue = p;
			g.type = pt;
			break;
		}
		case GVT_ULONG:{
			unsigned long * p = new unsigned long[g.count];
			for (int i = 0; i < g.count; i++){
				p[i] = ((wchar_t*)g.pvalue)[i];
			}
			delete[](wchar_t*)g.pvalue;
			g.pvalue = p;
			g.type = pt;
			break;
		}
		case GVT_ULONGLONG:{
			unsigned long long * p = new unsigned long  long[g.count];
			for (int i = 0; i < g.count; i++){
				p[i] = ((wchar_t*)g.pvalue)[i];
			}
			delete[](wchar_t*)g.pvalue;
			g.pvalue = p;
			g.type = pt;
			break;
		}
		case GVT_FLOAT:{
			float * p = new float[g.count];
			for (int i = 0; i < g.count; i++){
				p[i] = ((wchar_t*)g.pvalue)[i];
			}
			delete[](wchar_t*)g.pvalue;
			g.pvalue = p;
			g.type = pt;
			break;
		}
		case GVT_DOUBLE:{
			double * p = new double[g.count];
			for (int i = 0; i < g.count; i++){
				p[i] = ((wchar_t*)g.pvalue)[i];
			}
			delete[](wchar_t*)g.pvalue;
			g.pvalue = p;
			g.type = pt;
			break;
		}
		default:
			return false;
		}
		return true;
	}
	return false;
}


bool ValUtils::changeType(VALUE & g, GVT pt){
	if (pt == g.type)
		return true;
	if (isNumber(pt)){
		if (isNumber(g)){//都属于数字型
			return _numb_to_numb(g, pt);
		}
		else{//字符型转换成数字型
			return _char_to_numb(g, pt);
		}
	}
	else if (isChar(pt)){
		if (isNumber(g)){//数字型转换为字符型
			return _numb_to_char(g, pt);
		}
		else{//都属于字符型
			return _char_to_char(g, pt);
		}
	}
	return false;
}
void ValUtils::initial(VALUE & v){
	v.type = GVT_UNKN;
	v.count = 0;
	v.pvalue = 0;
}
void ValUtils::clear(VALUE & g){
	if (g.pvalue)
		delete[]((unsigned char*)g.pvalue);
	g.type = GVT_UNKN;
	g.count = 0;
	g.pvalue = 0;
}
bool ValUtils::isDecimal(GVT pt){
	if (pt > GVT_BEGIN_DECIMAL && pt<GVT_END_DECIMAL)
		return true;
	else
		return false;
}
bool ValUtils::isInteger(GVT pt){
	if (pt>GVT_BEGIN_INTEGER && pt<GVT_END_INTEGER)
		return true;
	else
		return false;
}
bool ValUtils::isNumber(GVT pt){
	if (pt>GVT_BEGIN_NUMBER && pt<GVT_END_NUMBER)
		return true;
	else
		return false;
}
bool ValUtils::isChar(GVT pt){
	if (pt>GVT_BEGIN_CHAR && pt < GVT_END_CHAR)
		return true;
	else
		return false;
}
bool ValUtils::isDecimal(VALUE & g){
	return isDecimal(g.type);
}
bool ValUtils::isInteger(VALUE & g){
	return isInteger(g.type);
}
bool ValUtils::isNumber(VALUE & g){
	return isNumber(g.type);
}
bool ValUtils::isChar(VALUE & g){
	return isChar(g.type);
}
bool ValUtils::isArray(VALUE & g){
	if (g.count>1)
		return true;
	else return false;
}
int ValUtils::getTypeSize(GVT t){
	switch (t){
	case GVT_UNKN:
		return 0;
	case GVT_CHAR:
	case GVT_UCHAR:
		return sizeof(char);
	case GVT_WCHAR:
		return sizeof(wchar_t);
	case GVT_SHORT:
	case GVT_USHORT:
		return sizeof(short);
	case GVT_INT:
	case GVT_UINT:
		return sizeof(int);
	case GVT_LONG:
	case GVT_ULONG:
		return sizeof(long);
	case GVT_ULONGLONG:
		return sizeof(unsigned long long);
	case GVT_FLOAT:
		return sizeof(float);
	case GVT_DOUBLE:
		return sizeof(double);
	default:
		return 0;
	}
}

double ValUtils::toNumber(VALUE & g){

	if (isArray(g))
		return (std::numeric_limits<double>::max)();

	double d = (std::numeric_limits<double>::min)();

	switch (g.type)
	{
	case GVT_CHAR:
		d = double(*((char*)g.pvalue));
		break;
	case GVT_UCHAR:
		d = double(*((unsigned char*)g.pvalue));
		break;
	case GVT_WCHAR:
		d = double(*((wchar_t*)g.pvalue));
		break;
	case GVT_SHORT:
		d = double(*((short*)g.pvalue));
		break;
	case GVT_USHORT:
		d = double(*((unsigned short*)g.pvalue));
		break;
	case GVT_INT:
		d = double(*((int*)g.pvalue));
		break;
	case GVT_UINT:
		d = double(*((unsigned int*)g.pvalue));
		break;
	case GVT_LONG:
		d = double(*((long*)g.pvalue));
		break;
	case GVT_ULONG:
		d = double(*((unsigned long*)g.pvalue));
		break;
	case GVT_FLOAT:
		d = double(*((float*)g.pvalue));
		break;
	case GVT_DOUBLE:
		d = *((double*)g.pvalue);
		break;
	default:
		break;
	}
	return d;
}
int ValUtils::toNumber(VALUE & g, double ** da, int * n){
	*n = g.count;
	*da = new double[g.count];

	switch (g.type)
	{
	case GVT_CHAR:{
		for (int i = 0; i < g.count; i++)
			(*da)[i] = ((char*)g.pvalue)[i];
		break;
	}
	case GVT_UCHAR:{
		for (int i = 0; i < g.count; i++)
			(*da)[i] = ((unsigned char*)g.pvalue)[i];
		break;
	}
	case GVT_WCHAR:{
		for (int i = 0; i < g.count; i++)
			(*da)[i] = ((wchar_t*)g.pvalue)[i];
		break;
	}
	case GVT_SHORT:{
		for (int i = 0; i < g.count; i++)
			(*da)[i] = ((short*)g.pvalue)[i];
		break;
	}
	case GVT_USHORT:{
		for (int i = 0; i < g.count; i++)
			(*da)[i] = ((unsigned short*)g.pvalue)[i];
		break;
	}
	case GVT_INT:{
		for (int i = 0; i < g.count; i++)
			(*da)[i] = ((int*)g.pvalue)[i];
		break;
	}
	case GVT_UINT:{
		for (int i = 0; i < g.count; i++)
			(*da)[i] = ((unsigned int*)g.pvalue)[i];
		break;
	}
	case GVT_LONG:{
		for (int i = 0; i < g.count; i++)
			(*da)[i] = ((long*)g.pvalue)[i];
		break;
	}
	case GVT_ULONG:{
		for (int i = 0; i < g.count; i++)
			(*da)[i] = ((unsigned long*)g.pvalue)[i];
		break;
	}
	case GVT_FLOAT:{
		for (int i = 0; i < g.count; i++)
			(*da)[i] = ((float*)g.pvalue)[i];
		break;
	}
	case GVT_DOUBLE:{
		for (int i = 0; i < g.count; i++)
			(*da)[i] = ((double*)g.pvalue)[i];
		break;
	}
	default:{
		delete[] * da;
		*n = 0;
		return 0;
	}
	}
	return g.count;
}
std::string ValUtils::toString(VALUE & g){
	if (isChar(g)){
		std::wstring szw = toWString(g);
		std::string sz;
		wstringToString(szw, sz);
		return sz;
	}
	else{
		if (!isArray(g)){
			double d = toNumber(g);
			char cc[256];
			printf_s("%lf", d, cc);
			std::string sz;
			sz.assign(cc);
			return sz;
		}
		else{
			std::string sz, s;
			char cc[256];
			for (int i = 0; i < g.count; i++){
				double d = toNumber(g);
				printf_s("%lf", d, cc);
				std::string sz;
				sz.assign(cc);
				s = s + sz;
			}
			return s;
		}
	}
}
std::wstring ValUtils::toWString(VALUE & g){
	if (isChar(g)){
		if (g.count > 1){
			if (g.type == GVT_CHAR){
				std::wstring sz;
				for (int i = 0; i < g.count; i++)
					sz.insert(sz.end(), (wchar_t)(((char*)g.pvalue)[i]));
				return sz;
			}
			else{//GVT_WCHAR
				std::wstring sz;
				sz.assign((wchar_t*)g.pvalue, ((wchar_t*)g.pvalue) + g.count - 1);
				return sz;
			}

		}
		else{
			if (g.type == GVT_CHAR){
				std::wstring sz;
				sz.insert(sz.end(), (wchar_t)(*((char*)g.pvalue)));
				return sz;
			}
			else{//GVT_WCHAR
				std::wstring sz;
				sz.insert(sz.end(), *((wchar_t*)g.pvalue));
				return sz;
			}

		}
	}
	else{
		if (!isArray(g)){
			double d = toNumber(g);
			wchar_t cc[256];
			wprintf_s(L"%lf", d, cc);
			std::wstring sz;
			sz.assign(cc);
			return sz;
		}
		else{
			std::wstring sz, s;
			wchar_t cc[256];
			for (int i = 0; i < g.count; i++){
				double d = toNumber(g);
				wprintf_s(L"%lf", d, cc);
				std::wstring sz;
				sz.assign(cc);
				s = s + sz;
			}
			return s;
		}
	}
}
int ValUtils::compare(VALUE &  v1, VALUE &v2)  {
	if (isArray(v1) || isArray(v2)){
		int vi1 = getTypeSize(v1.type)*v1.count;
		int vi2 = getTypeSize(v2.type)*v2.count;
		int i = memcmp(v1.pvalue, v2.pvalue, vi1>vi2?vi2:vi1 /*std::min<int>(vi1, vi2)*/);
		if (i == 0){
			if (vi1 > vi2)
				return 1;
			else if (vi1<vi2)
				return -1;
		}
		return i;
	}
	else{
		double d1 = toNumber(v1);
		double d2 = toNumber(v2);
		if (doubleEqual(d1, d2))
			return 0;
		else{
			if (d1>d2)
				return 1;
			else
				return -1;
		}
	}
}
int ValUtils::getSize(const VALUE & v){
	return v.count * getTypeSize(v.type);
}
void ValUtils::copy(VALUE * g, const VALUE & v){
	clear(*g);
	g->type = v.type;
	g->count = v.count;
	g->pvalue = (void*) new char[getSize(v)];
}

void ValUtils::initialChar(VALUE * pv, char * cc, int n){
	pv->type = GVT_CHAR;
	pv->count = n;
	pv->pvalue = (void*) new char[pv->count];
	memcpy(pv->pvalue, cc, getSize(*pv));
}
void ValUtils::initialChar(VALUE * pv, char  cc){
	initialChar(pv, &cc, 1);
}
void ValUtils::initialUChar(VALUE * pv, unsigned char * cc, int n){
	pv->type = GVT_UCHAR;
	pv->count = n;
	pv->pvalue = (void*) new unsigned char[pv->count];
	memcpy(pv->pvalue, cc, getSize(*pv));
}
void ValUtils::initialUChar(VALUE * pv, unsigned char  cc){
	initialUChar(pv, &cc, 1);
}

void ValUtils::initialWChar(VALUE * pv, wchar_t * cc, int n){
	pv->type = GVT_SHORT;
	pv->count = n;
	pv->pvalue = (void*) new  wchar_t[pv->count];
	memcpy(pv->pvalue, cc, getSize(*pv));
}
void ValUtils::initialWChar(VALUE * pv, wchar_t  cc){
	initialWChar(pv, &cc, 1);
}

void ValUtils::initialShort(VALUE * pv, short * cc, int n){
	pv->type = GVT_SHORT;
	pv->count = n;
	pv->pvalue = (void*) new  short[pv->count];
	memcpy(pv->pvalue, cc, getSize(*pv));
}
void ValUtils::initialShort(VALUE * pv, short  cc){
	initialShort(pv, &cc, 1);
}

void ValUtils::initialUShort(VALUE * pv, unsigned short * cc, int n){
	pv->type = GVT_USHORT;
	pv->count = n;
	pv->pvalue = (void*) new  short[pv->count];
	memcpy(pv->pvalue, cc, getSize(*pv));
}
void ValUtils::initialUShort(VALUE * pv, unsigned short  cc){
	initialUShort(pv, &cc, 1);
}

void ValUtils::initialInt(VALUE * pv, int * cc, int n){
	pv->type = GVT_INT;
	pv->count = n;
	pv->pvalue = (void*) new  int[pv->count];
	memcpy(pv->pvalue, cc, getSize(*pv));
}
void ValUtils::initialInt(VALUE * pv, int  cc){
	initialInt(pv, &cc, 1);
}

void ValUtils::initialUInt(VALUE * pv, unsigned int * cc, int n){
	pv->type = GVT_UINT;
	pv->count = n;
	pv->pvalue = (void*) new  int[pv->count];
	memcpy(pv->pvalue, cc, getSize(*pv));
}
void ValUtils::initialUInt(VALUE * pv, unsigned int  cc){
	initialUInt(pv, &cc, 1);
}

void ValUtils::initialLong(VALUE * pv, long * cc, int n){
	pv->type = GVT_LONG;
	pv->count = n;
	pv->pvalue = (void*) new  long[pv->count];
	memcpy(pv->pvalue, cc, getSize(*pv));
}
void ValUtils::initialLong(VALUE * pv, long  cc){
	initialLong(pv, &cc, 1);
}

void ValUtils::initialULong(VALUE * pv, unsigned long * cc, int n){
	pv->type = GVT_ULONG;
	pv->count = n;
	pv->pvalue = (void*) new  long[pv->count];
	memcpy(pv->pvalue, cc, getSize(*pv));
}
void ValUtils::initialULong(VALUE * pv, unsigned long  cc){
	initialULong(pv, &cc, 1);
}
void ValUtils::initialULongLong(VALUE * pv, unsigned long long cc){
	pv->type = GVT_ULONGLONG;
	pv->count = 1;
	pv->pvalue = (void*) new  unsigned long long[pv->count];
	memcpy(pv->pvalue, &cc, getSize(*pv));
}
void ValUtils::initialFloat(VALUE * pv, float * cc, int n){
	pv->type = GVT_FLOAT;
	pv->count = n;
	pv->pvalue = (void*) new  float[pv->count];
	memcpy(pv->pvalue, cc, getSize(*pv));
}
void ValUtils::initialFloat(VALUE * pv, float  cc){
	initialFloat(pv, &cc, 1);
}

void ValUtils::initialDouble(VALUE * pv, double * cc, int n){
	pv->type = GVT_DOUBLE;
	pv->count = n;
	pv->pvalue = (void*) new  double[pv->count];
	memcpy(pv->pvalue, cc, getSize(*pv));
}
void ValUtils::initialDouble(VALUE * pv, double  cc){
	initialDouble(pv, &cc, 1);
}


void ValUtils::write(Buffer & bs, VALUE & g){
	int t = g.type;
	bs.write(&t, sizeof(int));
	t = g.count;
	bs.write(&t, sizeof(int));

	bs.write(g.pvalue, getSize(g));
}
void ValUtils::read(Buffer & bs, VALUE & g){
	ValUtils::clear(g);
	int t = 0;
	bs.read(&t, sizeof(int));
	g.type = (ValUtils::GVT)t;
	bs.read(&(g.count), sizeof(int));
	g.pvalue = (void*) new char[getSize(g)];
	bs.read(g.pvalue, getSize(g));
}

void ValUtils::stringToWString(std::string & s, std::wstring & sd){
	sd.clear();
	sd.resize(s.length());
	std::wstring::iterator it1 = sd.begin();
	for (std::string::iterator it = s.begin(); it != s.end(); it++, it1++){
		*it1 = (wchar_t)(*it);
	}
}
//只有处于同一编码方式的情况下才是正确的，如果编码体系不同，则本函数不能正确的执行
void ValUtils::wstringToString(std::wstring & s, std::string & sd){
	sd.clear();
	for (std::wstring::iterator it = s.begin(); it != s.end(); it++){
		char h = (char)((*it) >> 8);
		char l = (char)(*it);

		if (h != 0)
			sd.append(1, (char)h);
		sd.append(1, (char)l);
	}
}

bool ValUtils::doubleEqual(double d1, double d2){
	if (fabs(d1 - d2) < TOLERANCE)
		return true;
	else
		return false;
}
bool ValUtils::floatEqual(float d1, float d2){
	if (fabs(d1 - d2) < TOLERANCE)
		return true;
	else
		return false;
}

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
