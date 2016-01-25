#include "valutils.h" 



begin_gtl_namespace
begin_gdb_namespace

bool ValUtils::changeType(VALUE & g, GVT pt){
	if (pt == g.type)
		return true;
	bool b = false;
	//character
	if (g.type > GVT_BEGIN_CHAR && g.type < GVT_END_CHAR) {
		std::vector<wchar_t> v;
		b=extractValue<wchar_t>(g, v);
		if (!b)
			return false;
		b=resetValue<wchar_t>(g, v, pt);
		if (!b)
			return false;
	}
	// signed integer
	if (g.type > GVT_BEGIN_SIGNED_INTEGER && g.type < GVT_END_SIGNED_INTEGER) {
		std::vector<long long> v;
		b=extractValue<long long>(g, v);
		if (!b)
			return false;
		b=resetValue<long long>(g, v, pt);
		if (!b)
			return false;
	}
	// unsigned integer
	if (g.type > GVT_BEGIN_UNSIGNED_INTEGER && g.type < GVT_END_UNSIGNED_INTEGER) {
		std::vector<unsigned long long> v;
		b=extractValue<unsigned long long>(g, v);
		if (!b)
			return false;
		b=resetValue<unsigned long long>(g, v, pt);
		if (!b)
			return false;
	}
	// decimal
	if (g.type > GVT_BEGIN_DECIMAL && g.type < GVT_END_DECIMAL) {
		std::vector<double> v;
		b=extractValue<double>(g, v);
		if (!b)
			return false;
		b=resetValue<double>(g, v, pt);
		if (!b)
			return false;
	}
	// boolean
	if (g.type > GVT_BEGIN_BOOLEAN && g.type < GVT_END_BOOLEAN) {
		std::vector<unsigned long long> v;
		b=extractValue<unsigned long long>(g, v);
		if (!b)
			return false;
		b=resetValue<unsigned long long>(g, v, pt);
		if (!b)
			return false;
	}
	// time
	if (g.type > GVT_BEGIN_TIME && g.type < GVT_END_TIME) {
		std::vector<long long> v;
		b=extractValue<long long>(g, v);
		if (!b)
			return false;
		b=resetValue<long long>(g, v, pt);
		if (!b)
			return false;
	}
	return true;
}
void ValUtils::initial(VALUE & v){
	v.type = GVT_UNKN;
	v.count = 0;
	v.pvalue = 0;
	v.u64 = 0;
}
void ValUtils::clear(VALUE & g){
	if (g.count > 1) {
		if (g.pvalue)
			delete[]((unsigned char*)g.pvalue);
		g.pvalue = 0;
	}
	else {
		if (g.type == GVT_DATE) {
			delete[] (g.date);
			g.date = 0;
		}
		else if (g.type == GVT_DATETIME) {
			delete[](g.datetime);
			g.datetime = 0;
		}			
	}
	g.type = GVT_UNKN;
	g.count = 0;
	g.u64 = 0;
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
	else
		return false;
}

bool ValUtils::isBoolean(GVT pt) {
	if (pt>GVT_BEGIN_BOOLEAN && pt < GVT_END_BOOLEAN)
		return true;
	else
		return false;
}
bool ValUtils::isBoolean(VALUE & g) {
	return isBoolean(g.type);
}
bool ValUtils::isTime(VALUE & g) {
	return isTime(g.type);
}
bool ValUtils::isTime(GVT pt) {
	if (pt>GVT_BEGIN_TIME && pt < GVT_END_TIME)
		return true;
	else
		return false;
}

int ValUtils::getTypeSize(GVT t){
	int r = 0;
	switch (t)
	{
	case GVT_CHAR://c8
		r=sizeof(char);
		break;
	case GVT_WCHAR://c16
		r = sizeof(wchar_t);
		break;
	case GVT_INT8://i8
		r = sizeof(signed char);
		break;
	case GVT_INT16://i16
		r = sizeof(short);
		break;
	case GVT_INT32://i32
		r = sizeof(int);
		break;
	case GVT_INT64://i64
		r = sizeof(long long);
		break;
	case GVT_UINT8://u8
		r = sizeof(unsigned char);
		break;
	case GVT_UINT16://u16
		r = sizeof(unsigned short);
		break;
	case GVT_UINT32://u32
		r = sizeof(unsigned int);
		break;
	case GVT_UINT64://u64
		r = sizeof(unsigned long long);
		break;
	case GVT_FLOAT32://f32
		r = sizeof(float);
		break;
	case GVT_FLOAT64://f64
		r = sizeof(double);
		break;
	case GVT_BOOL://boolval 
		r = sizeof(bool);
		break;
	case GVT_DATE://DATE 
		r = sizeof(int)*3;
		break;
	case GVT_DATETIME://DATETIME 
		r = sizeof(int)*7;
		break;
	default:
		break;
	}
	return r;
}
std::string ValUtils::toString(VALUE & g){
	std::string sz;
	switch (g.type)
	{
	case GVT_CHAR://c8
	{	
		if (g.count > 1) {
			sz.assign(g.carray, g.count);
		}
		else {
			sz.resize(1, g.c8);
		}
		break;
	}
	case GVT_WCHAR://c16
	{
		if (g.count > 1) {
			std::wstring wsz;
			wsz.assign(g.wcarray, g.count); 
			wstringToString(wsz, sz);
		}
		else {
			std::wstring wsz;
			wsz.resize(1, g.c16);
			wstringToString(wsz, sz);
		}
		break;
	}
	case GVT_INT8://i8	
	{
		char t[32];
		if (g.count > 1) {
			int n = g.count;
			itoa((int)((g.carray)[0]), (char*)t, 10);
			sz  = ((const char *)t);
			for (int i = 1; i < n; i++) {
				itoa((int)((g.carray)[i]),(char*) t, 10);
				sz.append(1, ',');
				sz += ((const char *)t);
			}
		}
		else {			
			itoa((int)(g.i8), t, 10);
			sz = ((const char *)t);
		}
		break;
	}
	case GVT_INT16://i16
	{
		char t[32];
		if (g.count > 1) {
			int n = g.count;
			itoa((int)(((short*)(g.pvalue))[0]), (char*)t, 10);
			for (int i = 1; i < n; i++) {
				itoa((int)(((short*)(g.pvalue))[i]), (char*)t, 10);
				sz.append(1, ',');
				sz += ((const char *)t);
			}
		}
		else {
			itoa((int)(g.i16), t, 10);
			sz = ((const char *)t);
		}
		break;
	}
	case GVT_INT32://i32
	{
		char t[32];
		if (g.count > 1) {
			int n = g.count;
			itoa((int)(((int*)(g.pvalue))[0]), (char*)t, 10);
			for (int i = 1; i < n; i++) {
				itoa((int)(((int*)(g.pvalue))[i]), (char*)t, 10);
				sz.append(1, ',');
				sz += ((const char *)t);
			}
		}
		else {
			itoa((int)(g.i32), t, 10);
			sz = ((const char *)t);
		}
		break;
	}
	case GVT_INT64://i64
	{
		char t[64];
		if (g.count > 1) {
			int n = g.count;
			_i64toa((long long)(((long long *)(g.pvalue))[0]), (char*)t, 10);
			for (int i = 1; i < n; i++) {
				_i64toa((long long)(((long long *)(g.pvalue))[i]), (char*)t, 10);
				sz.append(1, ',');
				sz += ((const char *)t);
			}
		}
		else {
			_i64toa((long long)(g.i64), t, 10);
			sz = ((const char *)t);
		}
		break;
	}
	case GVT_UINT8://u8
	{
		char t[32];
		if (g.count > 1) {
			int n = g.count;
			ultoa((unsigned long)(((unsigned char *)(g.pvalue))[0]), (char*)t, 10);
			for (int i = 1; i < n; i++) {
				ultoa((unsigned long)(((unsigned char *)(g.pvalue))[i]), (char*)t, 10);
				sz.append(1, ',');
				sz += ((const char *)t);
			}
		}
		else {
			ultoa((unsigned long)(g.u8), t, 10);
			sz = ((const char *)t);
		}
		break;
	}
	case GVT_UINT16://u16
	{
		char t[32];
		if (g.count > 1) {
			int n = g.count;
			ultoa((unsigned long)(((unsigned short *)(g.pvalue))[0]), (char*)t, 10);
			for (int i = 1; i < n; i++) {
				ultoa((unsigned long)(((unsigned short *)(g.pvalue))[i]), (char*)t, 10);
				sz.append(1, ',');
				sz += ((const char *)t);
			}
		}
		else {
			ultoa((unsigned long)(g.u16), t, 10);
			sz = ((const char *)t);
		}
		break;
	}
	case GVT_UINT32://u32
	{
		char t[32];
		if (g.count > 1) {
			int n = g.count;
			ultoa((unsigned long)(((unsigned int *)(g.pvalue))[0]), (char*)t, 10);
			for (int i = 1; i < n; i++) {
				ultoa((unsigned long)(((unsigned int *)(g.pvalue))[i]), (char*)t, 10);
				sz.append(1, ',');
				sz += ((const char *)t);
			}
		}
		else {
			ultoa((unsigned long)(g.u32), t, 10);
			sz = ((const char *)t);
		}
		break;
	}
	case GVT_UINT64://u64
	{
		char t[64];
		if (g.count > 1) {
			int n = g.count;
			ultoa((unsigned long long)(((unsigned long long *)(g.pvalue))[0]), (char*)t, 10);
			for (int i = 1; i < n; i++) {
				ultoa((unsigned long long)(((unsigned long long *)(g.pvalue))[i]), (char*)t, 10);
				sz.append(1, ',');
				sz += ((const char *)t);
			}
		}
		else {
			ultoa((unsigned long long)(g.u64), t, 10);
			sz = ((const char *)t);
		}
		break;
	}
	case GVT_FLOAT32://f32
	{
		if (g.count > 1) {
			int n = g.count;
			std::basic_ostringstream<char> oss;
			oss << ((float*)(g.pvalue))[0];
			for (int i = 1; i < n; i++) {
				oss<<","<< ((float*)(g.pvalue))[i];
			}
			sz = oss.str();
		}
		else {
			std::basic_ostringstream<char> oss;
			oss << g.i32;
			sz = oss.str();
		}
		
		break;
	}
	case GVT_FLOAT64://f64
	{
		if (g.count > 1) {
			int n = g.count;
			std::basic_ostringstream<char> oss;
			oss << ((double*)(g.pvalue))[0];
			for (int i = 1; i < n; i++) {
				oss << "," << ((double*)(g.pvalue))[i];
			}
			sz = oss.str();
		}
		else {
			std::basic_ostringstream<char> oss;
			oss << g.i32;
			sz = oss.str();
		}
		break;
	}
	case GVT_BOOL://boolval 
	{
		if (g.count > 1) {
			int n = g.count;
			std::basic_ostringstream<char> oss;
			oss << ((bool*)(g.pvalue))[0];
			for (int i = 1; i < n; i++) {
				oss << "," << ((bool*)(g.pvalue))[i];
			}
			sz = oss.str();
		}
		else {
			std::basic_ostringstream<char> oss;
			oss << g.boolval;
			sz = oss.str();
		}
		break;
	}
	case GVT_DATE://DATE 
	{
		if (g.count > 1) {
			int n = g.count;
			std::basic_ostringstream<char> oss;
			oss << g.date[0] << "-" << g.date[1] << "-" << g.date[2];
			for (int i = 1; i < n; i++) {
				oss << "," << g.date[3*i] << "-" << g.date[3*i+1] << "-" << g.date[3*i+2];
			}
			sz = oss.str();
		}
		else {
			std::basic_ostringstream<char> oss;
			oss << g.date[0]<<"-"<< g.date[1]<<"-"<< g.date[2];
			sz = oss.str();
		}
		break;
	}
	case GVT_DATETIME://DATETIME YYYY-MM-DD HH:MM:SS MMMM
	{
		if (g.count > 1) {
			int n = g.count;
			std::basic_ostringstream<char> oss;
			oss << g.datetime[0] << "-" << g.datetime[1] << "-" << g.datetime[2]<<" "
				<< g.datetime[3]<<":"<<g.datetime[4]<<":"<< g.datetime[5]<<" "
				<< g.datetime[6];
			for (int i = 1; i < n; i++) {
				oss << g.datetime[i*7+0] << "-" << g.datetime[i*7+1] << "-" << g.datetime[i*7+2] << " "
					<< g.datetime[i*7+3] << ":" << g.datetime[i*7+4] << ":" << g.datetime[i*7+5] << " " 
					<< g.datetime[i*7+6];
			}
			sz = oss.str();
		}
		else {
			std::basic_ostringstream<char> oss;
			oss << g.datetime[0] << "-" << g.datetime[1] << "-" << g.datetime[2] << " "
				<< g.datetime[3] << ":" << g.datetime[4] << ":" << g.datetime[5] << " "
				<< g.datetime[6];
			sz = oss.str();
		}
		break;
	}
	default:
		break;
	}
	return sz;
}
std::wstring ValUtils::toWString(VALUE & g){
	std::wstring wsz;	 
	if(g.type== GVT_WCHAR){//c16		
		if (g.count > 1) {			
			wsz.assign(g.wcarray, g.count); 
		}
		else { 
			wsz.resize(1, g.c16);
		}	
	}
	else {
		std::string sz = toString(g);
		stringToWString(sz, wsz);
	}
	return wsz;
}
int ValUtils::compare(VALUE &  v1, VALUE &v2)  {
	if (v1.type == v2.type) {//同类型比较
		if (v1.count == 1 && v2.count == 1) {//同类型单值
			switch (v1.type)
			{
			case GVT_CHAR://c8
			case GVT_WCHAR://c16
			case GVT_INT8://i8
			case GVT_INT16://i16
			case GVT_INT32://i32
			case GVT_INT64://i64
			case GVT_BOOL://boolval 
			{
				if (v1.i64 > v2.i64)
					return 1;
				else if (v1.i64 == v2.i64)
					return 0;
				else
					return -1;
			}
			case GVT_UINT8://u8
			case GVT_UINT16://u16
			case GVT_UINT32://u32
			case GVT_UINT64://u64
			{
				if (v1.u64 > v2.u64)
					return 1;
				else if (v1.u64 == v2.u64)
					return 0;
				else
					return -1;
			}
			case GVT_FLOAT32://f32
			{
				if (v1.f32 > v2.f32)
					return 1;
				else if (floatEqual(v1.f32, v2.f32))
					return 0;
				else
					return -1;
			}
			case GVT_FLOAT64://f64
			{
				if (v1.f64 > v2.f64)
					return 1;
				else if (doubleEqual(v1.f64, v2.f64))
					return 0;
				else
					return -1;
			}
			case GVT_DATE://DATE 
			{
				for (int i = 0; i < 3; i++) {
					if (v1.date[i] > v2.date[i])
						return 1;
					else if (v1.date[i] < v2.date[i])
						return -1;
					else
						continue;
				}
				return 0;
			}
			case GVT_DATETIME://DATETIME 
			{
				for (int i = 0; i < 7; i++) {
					if (v1.datetime[i] > v2.datetime[i])
						return 1;
					else if (v1.datetime[i] < v2.datetime[i])
						return -1;
					else
						continue;
				}
				return 0;
			}
			default:
				return 0;
			}
		}
		else if(v1.count>1 && v2.count>1){//同类型数组,比较内存块
			int vi1 = getTypeSize(v1.type)*v1.count;
			int vi2 = getTypeSize(v2.type)*v2.count;
			int i = memcmp(v1.pvalue, v2.pvalue, vi1>vi2 ? vi2 : vi1 /*std::min<int>(vi1, vi2)*/);
			if (i == 0) {
				if (vi1 > vi2)
					return 1;
				else if (vi1<vi2)
					return -1;
			}
			return i;
		}
		else {//同类型，但一个是数组，一个是单值，转换成字符串比较
			std::wstring sz1, sz2;
			get(v1, sz1);
			get(v2, sz2);
			return wcscmp(sz1.c_str(), sz2.c_str());
		}
	}
	else {//不同类型比较
		if (v1.count == 1 && v2.count == 1) {//不同类型的单值
			if (isNumber(v1.type) && isNumber(v2.type)) {//都是数值类型的单值
				double d1, d2;
				extractValue<double>(v1, d1);
				extractValue<double>(v2, d2);
				{
					if (d1 > d2)
						return 1;
					else if (doubleEqual(d1, d2))
						return 0;
					else
						return -1;
				}
			}
			else if (isChar(v1.type) && isChar(v2.type)) {//都是字符类型单值,其中一个是宽字符
				if (v1.type == GVT_CHAR) {
					if (v1.c8 > v2.c16)
						return 1;
					else if (v1.c8 == v2.c16)
						return 0;
					else
						return -1;
				}
				else {
					if (v1.c16 > v2.c8)
						return 1;
					else if (v1.c16 == v2.c8)
						return 0;
					else
						return -1;
				}
			}
			else if (isTime(v1.type) && isTime(v2.type)) {//都是时间类型单值,一个DATE,一个是DATETIME
				//比较年月日；										  
				for (int i = 0; i < 3; i++) {
					if (v1.date[i] > v2.date[i])
						return 1;
					else if (v1.date[i] < v2.date[i])
						return -1;
					else
						continue;
				}
				//年月日相同，接下来比较时间；
				if (v1.type == GVT_DATE) {					
					for (int i = 3; i < 7; i++)
						if (v2.datetime[i] != 0)
							return -1;
					//如果都是0，则相等
					return 0;
				}
				else {
					for (int i = 3; i < 7; i++)
						if (v1.datetime[i] != 0)
							return 1;
					//如果都是0，则相等
					return 0;
				}
			}
			else {//转换成字符串比较
				std::wstring sz1, sz2;
				get(v1, sz1);
				get(v2, sz2);
				return wcscmp(sz1.c_str(), sz2.c_str());
			}
		}
		else if (v1.count >1 && v2.count > 1) {//都是数组类型，比较内存块
			int vi1 = getTypeSize(v1.type)*v1.count;
			int vi2 = getTypeSize(v2.type)*v2.count;
			int i = memcmp(v1.pvalue, v2.pvalue, vi1>vi2 ? vi2 : vi1 /*std::min<int>(vi1, vi2)*/);
			if (i == 0) {
				if (vi1 > vi2)
					return 1;
				else if (vi1<vi2)
					return -1;
			}
			return i;
		}
		else {//不同类型，其中一个是数组类型，一个是单质类型，转换成字符串比较
			std::wstring sz1, sz2;
			get(v1, sz1);
			get(v2, sz2);
			return wcscmp(sz1.c_str(), sz2.c_str());
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
	if (v.count > 1) {
		int s = getSize(v);
		g->pvalue = (void*) new char[s];
		memcpy(g->pvalue, v.pvalue, s);
	}
	else {
		switch (v.type)
		{
		case GVT_CHAR://c8
			g->c8 = v.c8;
			break;
		case GVT_WCHAR://c16
			g->c16 = v.c16;
			break;
		case GVT_INT8://i8
			g->i8 = v.i8;
			break;
		case GVT_INT16://i16
			g->i16 = v.i16;
			break;
		case GVT_INT32://i32
			g->i32 = v.i32;
			break;
		case GVT_INT64://i64
			g->i64=v.i64;
			break;
		case GVT_UINT8://u8
			g->u8=v.u8;
			break;
		case GVT_UINT16://u16
			g->u16=v.u16;
			break;
		case GVT_UINT32://u32
			g->u32=v.u32;
			break;
		case GVT_UINT64://u64
			g->u64 = v.u64;
			break;
		case GVT_FLOAT32://f32
			g->f32=v.f32;
			break;
		case GVT_FLOAT64://f64
			g->f64=v.f64;
			break;
		case GVT_BOOL://boolval 
			g->boolval=v.boolval;
			break;
		case GVT_DATE://DATE 
			memcpy(g->date,v.date,sizeof(int)*3);
			break;
		case GVT_DATETIME://DATETIME 
			memcpy(g->datetime, v.datetime, sizeof(int) * 7);
			break;
		default:
			break;
		}
	}	
}

void ValUtils::initialChar(VALUE * pv, char * cc, int n){
	if (n == 1)
		initialChar(pv, *cc);
	else {
		pv->type = GVT_CHAR;
		pv->count = n;
		pv->carray=  new char[pv->count];
		memcpy(pv->carray, cc, n);
	}	
}
void ValUtils::initialChar(VALUE * pv, char  cc){
	pv->type = GVT_CHAR;
	pv->count = 1;
	pv->c8 = cc;
	//initialChar(pv, &cc, 1);
}

void ValUtils::initialInt8(VALUE * pv, signed char * cc, int n) {
	if (n == 1)
		initialChar(pv, *cc);
	else {
		pv->type = GVT_INT8;
		pv->count = n;
		pv->pvalue =(void *) new signed char[pv->count];
		memcpy(pv->pvalue, cc, n);
	}
}
void ValUtils::initialInt8(VALUE * pv, signed char  cc) {
	pv->type = GVT_INT8;
	pv->count = 1;
	pv->i8 = cc;
	//initialChar(pv, &cc, 1);
}
void ValUtils::initialUInt8(VALUE * pv, unsigned char * cc, int n){
	if (n == 1)
		initialUInt8(pv, *cc);
	else {
		pv->type = GVT_UINT8;
		pv->count = n;
		pv->barray =  new unsigned char[pv->count];
		memcpy(pv->barray, cc, n);
	}	
}
void ValUtils::initialUInt8(VALUE * pv, unsigned char  cc){
	pv->type = GVT_UINT8;
	pv->count = 1;
	pv->u8 = cc;
	//initialUChar(pv, &cc, 1);
}

void ValUtils::initialWChar(VALUE * pv, wchar_t * cc, int n){
	if (n == 1)
		initialWChar(pv, *cc);
	else {
		pv->type = GVT_WCHAR;
		pv->count = n;
		pv->pvalue = (void*) new  wchar_t[pv->count];
		memcpy(pv->pvalue, cc, getSize(*pv));
	}	
}
void ValUtils::initialWChar(VALUE * pv, wchar_t  cc){
	pv->type = GVT_WCHAR;
	pv->c16 = cc;
	pv->count = 1;
}

void ValUtils::initialInt16(VALUE * pv, short * cc, int n){
	if (n == 1)
		initialInt16(pv, *cc);
	else {
		pv->type = GVT_INT16;
		pv->count = n;
		pv->pvalue = (void*) new  short[pv->count];
		memcpy(pv->pvalue, cc, getSize(*pv));
	}
}
void ValUtils::initialInt16(VALUE * pv, short  cc){
	pv->type = GVT_INT16;
	pv->i16 = cc;
	pv->count = 1;
}

void ValUtils::initialUInt16(VALUE * pv, unsigned short * cc, int n){
	if(n==1)
		initialUInt16(pv, *cc);
	else {
		pv->type = GVT_UINT16;
		pv->count = n;
		pv->pvalue = (void*) new  unsigned short[pv->count];
		memcpy(pv->pvalue, cc, getSize(*pv));
	}	
}
void ValUtils::initialUInt16(VALUE * pv, unsigned short  cc){
	pv->type = GVT_UINT16;
	pv->count = 1;
	pv->u16 = cc;
}

void ValUtils::initialInt32(VALUE * pv, int * cc, int n){
	if(n==1)
		initialInt32(pv, *cc);
	else {
		pv->type = GVT_INT32;
		pv->count = n;
		pv->pvalue = (void*) new  int[pv->count];
		memcpy(pv->pvalue, cc, getSize(*pv));
	}
}
void ValUtils::initialInt32(VALUE * pv, int  cc){
	pv->type = GVT_INT32;
	pv->count = 1;
	pv->i32 = cc;
}

void ValUtils::initialUInt32(VALUE * pv, unsigned int * cc, int n){
	if (n == 1)
		initialUInt32(pv, *cc);
	else {
		pv->type = GVT_UINT32;
		pv->count = n;
		pv->pvalue = (void*) new  int[pv->count];
		memcpy(pv->pvalue, cc, getSize(*pv));
	}	
}
void ValUtils::initialUInt32(VALUE * pv, unsigned int  cc){
	pv->type = GVT_UINT32;
	pv->count = 1;
	pv->u32 = cc;
	//initialUInt(pv, &cc, 1);
}

void ValUtils::initialInt32(VALUE * pv, long * cc, int n){
	if (n == 1)
		initialInt32(pv, *cc);
	else {
		pv->type = GVT_INT32;
		pv->count = n;
		int * data  =   new  int[pv->count];
		for (int i = 0; i < n; i++)
			data[i] = cc[i];
		//memcpy(pv->pvalue, cc, getSize(*pv));
	}	
}
void ValUtils::initialInt32(VALUE * pv, long  cc){
	pv->type = GVT_INT32;
	pv->count = 1;
	pv->i32 = cc;

	//initialLong(pv, &cc, 1);
}

void ValUtils::initialUInt32(VALUE * pv, unsigned long * cc, int n){
	if (n == 1)
		initialUInt32(pv, *cc);
	else {
		pv->type = GVT_UINT32;
		pv->count = n;
		unsigned int * data = new  unsigned int[pv->count];
		for (int i = 0; i < n; i++)
			data[i] = cc[i];
		pv->pvalue = (void*)data;
		//memcpy(pv->pvalue, cc, getSize(*pv));
	}
}
void ValUtils::initialUInt32(VALUE * pv, unsigned long  cc){
	pv->type = GVT_UINT32;
	pv->count = 1;
	pv->u32 = cc;
	//initialULong(pv, &cc, 1);
}
void ValUtils::initialUInt64(VALUE * pv, unsigned long long cc){
	pv->type = GVT_UINT64;
	pv->count = 1;
	pv->u64 = cc;
	//pv->pvalue = (void*) new  unsigned long long[pv->count];
	//memcpy(pv->pvalue, &cc, getSize(*pv));
}
void ValUtils::initialUInt64(VALUE * pv, unsigned long long * cc, int n) {
	if (n == 1) {
		pv->type = GVT_UINT64;
		pv->count = 1;
		pv->u64 = *cc;
	}
	else {
		pv->type = GVT_UINT64;
		pv->count = n;
		pv->pvalue = (void*) new  unsigned long long[n];
		memcpy(pv->pvalue, cc, getSize(*pv));
	}
}
void ValUtils::initialFloat(VALUE * pv, float * cc, int n){
	if (n == 1) {
		pv->type = GVT_FLOAT32;
		pv->count = 1;
		pv->f32 = *cc;
	}
	else {
		pv->type = GVT_FLOAT32;
		pv->count = n;
		pv->pvalue = (void*) new  float[pv->count];
		memcpy(pv->pvalue, cc, getSize(*pv));
	}	
}
void ValUtils::initialFloat(VALUE * pv, float  cc){
	pv->type = GVT_FLOAT32;
	pv->count = 1;
	pv->f32 = cc;
	//initialFloat(pv, &cc, 1);
}

void ValUtils::initialDouble(VALUE * pv, double * cc, int n){
	if (n == 1) {
		pv->type = GVT_FLOAT64;
		pv->count = 1;
		pv->f64 = *cc;
	}
	else {
		pv->type = GVT_FLOAT64;
		pv->count = n;
		pv->pvalue = (void*) new  double[pv->count];
		memcpy(pv->pvalue, cc, getSize(*pv));
	}	
}
void ValUtils::initialDouble(VALUE * pv, double  cc){
	pv->type = GVT_FLOAT64;
	pv->count = 1;
	pv->f64 = cc;
	//initialDouble(pv, &cc, 1);
}



void ValUtils::initialBoolean(VALUE * pv, bool * cc, int n  ) {
	if (n == 1) {
		pv->count = 1;
		pv->type = GVT_BOOL;
		pv->boolval = cc;
	}
	else {
		pv->count = n;
		pv->type = GVT_BOOL;
		pv->pvalue = (void*) new bool[n];
		memcpy(pv->pvalue, cc, getSize(*pv));
	}
}
void ValUtils::initialBoolean(VALUE * pv, bool  cc) {
	pv->count = 1;
	pv->type = GVT_BOOL;
	pv->boolval = cc;
}
void ValUtils::initialDate(VALUE * pv, int year, int month, int day){
	pv->count = 1;
	pv->type = GVT_DATE;
	pv->date = new int [3];
	(pv->date)[0] = year;
	(pv->date)[1] = month;
	(pv->date)[2] = day;
}
void ValUtils::initialDateTime(VALUE * pv, int year, int month, int day, int hour, int minute, int second, int millisecond){
	pv->count = 1;
	pv->type = GVT_DATETIME;
	pv->datetime = new int[7];
	(pv->datetime)[0] = year;
	(pv->datetime)[1] = month;
	(pv->datetime)[2] = day;
	(pv->datetime)[3] = hour;
	(pv->datetime)[4] = minute;
	(pv->datetime)[5] = second;
	(pv->datetime)[6] = millisecond;
}

void ValUtils::write(Buffer & bs, VALUE & g) {
	int t = g.type;
	bs.write(&t, sizeof(int));
	t = g.count;
	bs.write(&t, sizeof(int));
	if (g.count > 1) {
		bs.write(g.pvalue, getSize(g));
	}
	else {
		switch (g.type)
		{
		case GVT_CHAR://c8
			bs.write((void*)(&(g.c8)), sizeof(char));
			break;
		case GVT_WCHAR://c16
			bs.write((void*)(&(g.c16)), sizeof(wchar_t));
			break;
		case GVT_INT8://i8
			bs.write((void*)(&(g.i8)), sizeof(signed char));
			break;
		case GVT_INT16://i16
			bs.write((void*)(&(g.i16)), sizeof(short));
			break;
		case GVT_INT32://i32
			bs.write((void*)(&(g.i32)), sizeof(int));
			break;
		case GVT_INT64://i64
			bs.write((void*)(&(g.i64)), sizeof(long long));
			break;
		case GVT_UINT8://u8
			bs.write((void*)(&(g.u8)), sizeof(unsigned char));
			break;
		case GVT_UINT16://u16
			bs.write((void*)(&(g.u16)), sizeof(unsigned short));
			break;
		case GVT_UINT32://u32
			bs.write((void*)(&(g.u32)), sizeof(unsigned int));
			break;
		case GVT_UINT64://u64
			bs.write((void*)(&(g.u64)), sizeof(unsigned long long));
			break;
		case GVT_FLOAT32://f32
			bs.write((void*)(&(g.f32)), sizeof(float));
			break;
		case GVT_FLOAT64://f64
			bs.write((void*)(&(g.f64)), sizeof(double));
			break;
		case GVT_BOOL://boolval 
			bs.write((void*)(&(g.boolval)), sizeof(bool));
			break;
		case GVT_DATE://DATE 
			bs.write((void*)(&(g.date)), sizeof(int)*3);
			break;
		case GVT_DATETIME://DATETIME 
			bs.write((void*)(&(g.datetime)), sizeof(int) * 7);
			break;
		default:
			break;
		}
	}

}
void ValUtils::read(Buffer & bs, VALUE & g){
	ValUtils::clear(g);
	int t = 0;
	bs.read(&t, sizeof(int));
	g.type = (ValUtils::GVT)t;
	bs.read(&(g.count), sizeof(int));
	if (g.count > 1) {
		int s = getSize(g);
		g.pvalue = (void*) new char[s];
		bs.read(g.pvalue, s);
	}
	else {
		switch (g.type)
		{
		case GVT_CHAR://c8
			bs.read((void*)(&(g.c8)), sizeof(char));
			break;
		case GVT_WCHAR://c16
			bs.read((void*)(&(g.c16)), sizeof(wchar_t));
			break;
		case GVT_INT8://i8
			bs.read((void*)(&(g.i8)), sizeof(signed char));
			break;
		case GVT_INT16://i16
			bs.read((void*)(&(g.i16)), sizeof(short));
			break;
		case GVT_INT32://i32
			bs.read((void*)(&(g.i32)), sizeof(int));
			break;
		case GVT_INT64://i64
			bs.read((void*)(&(g.i64)), sizeof(long long));
			break;
		case GVT_UINT8://u8
			bs.read((void*)(&(g.u8)), sizeof(unsigned char));
			break;
		case GVT_UINT16://u16
			bs.read((void*)(&(g.u16)), sizeof(unsigned short));
			break;
		case GVT_UINT32://u32
			bs.read((void*)(&(g.u32)), sizeof(unsigned int));
			break;
		case GVT_UINT64://u64
			bs.read((void*)(&(g.u64)), sizeof(unsigned long long));
			break;
		case GVT_FLOAT32://f32
			bs.read((void*)(&(g.f32)), sizeof(float));
			break;
		case GVT_FLOAT64://f64
			bs.read((void*)(&(g.f64)), sizeof(double));
			break;
		case GVT_BOOL://boolval 
			bs.read((void*)(&(g.boolval)), sizeof(bool));
			break;
		case GVT_DATE://DATE 
			bs.read((void*)(&(g.date)), sizeof(int) * 3);
			break;
		case GVT_DATETIME://DATETIME 
			bs.read((void*)(&(g.datetime)), sizeof(int) * 7);
			break;
		default:
			break;
		}
	}
}


bool ValUtils::get(VALUE & g, std::vector<long long> & v) {
	return extractValue<long long>(g, v);
}
bool ValUtils::get(VALUE & g, std::vector<unsigned long long> & v) {
	return extractValue<unsigned long long>(g, v);
}
bool ValUtils::get(VALUE & g, std::vector<double> & v) {
	return extractValue<double>(g, v);
}
bool ValUtils::get(VALUE & g, double & v) {
	if (g.count > 1)
		return false;
	return extractValue<double>(g, v);
}
bool ValUtils::get(VALUE & g, long long & v) {
	if (g.count > 1)
		return false;
	return extractValue<long long>(g, v);
}
bool ValUtils::get(VALUE & g, unsigned long long & v) {
	if (g.count > 1)
		return false;
	return extractValue<unsigned long long>(g, v);
}
bool ValUtils::get(VALUE & g, std::string & sz) {
	if (g.type == GVT_CHAR || g.type == GVT_INT8) {
		if(g.count>1)
			sz.assign(g.carray, g.count);
		else if(g.count==1)
			sz.append(1, g.c8);
		else
			return false;
		return true;
	}
	else if (g.type == GVT_WCHAR || g.type==GVT_INT16) {
		std::wstring   wsz;
		if (get(g, wsz)) {
			wstringToString(wsz, sz);
			return true;
		}
		else
			return false;
	}
	else {
		sz = toString(g);
	}
	return true;
}
bool ValUtils::get(VALUE & g, std::wstring & sz) {
	if (g.type == GVT_WCHAR || g.type == GVT_INT16) {
		if (g.count>1)
			sz.assign((wchar_t*)(g.pvalue), g.count);
		else if (g.count == 1)
			sz.append(1, g.c16);
		else
			return false;
		return true;
	}
	else if (g.type == GVT_CHAR || g.type == GVT_INT8) {
		std::string   ssz;
		if (get(g, ssz)) {
			stringToWString(ssz, sz);
			return true;
		}
		else
			return false;
	}
	else {
		sz = toWString(g);
	}
	return true;
}
bool ValUtils::set(VALUE & g, std::vector<long long> & tv, GVT newtype) {
	return resetValue<long long>(g, tv, newtype);
}

bool ValUtils::set(VALUE & g, std::vector<unsigned long long> & newvals, GVT newtype) {
	return resetValue<unsigned long long>(g, newvals, newtype);
}

bool ValUtils::set(VALUE & g, std::vector<double> & newvals, GVT newtype) {
	return resetValue<double>(g, newvals, newtype);
}

bool ValUtils::set(VALUE & g,  long long  tv, GVT newtype) {
	return resetValue<long long>(g, tv, newtype);
}

bool ValUtils::set(VALUE & g,  unsigned long long  newvals, GVT newtype) {
	return resetValue<unsigned long long>(g, newvals, newtype);
}

bool ValUtils::set(VALUE & g,  double newvals, GVT newtype) {
	return resetValue<double>(g, newvals, newtype);
}

bool ValUtils::set(VALUE & g, const char * sz) {
	clear(g);
	int n = strlen(sz);
	if (n >1 ) {
		g.count = n;
		g.type = GVT_CHAR;
		g.carray = new char[n];
		memcpy(g.carray, sz, n*sizeof(char));
	}
	else if (n == 1) {
		g.count = n;
		g.type = GVT_CHAR;
		g.c8 = sz[0];
	}
	else
		return false;
}
bool ValUtils::set(VALUE & g, const wchar_t* sz) {
	clear(g);
	int n = wcslen(sz);
	if (n >1) {
		g.count = n;
		g.type = GVT_WCHAR;
		g.pvalue =(void*) new wchar_t[n];
		memcpy(g.pvalue, sz, n*sizeof(wchar_t));
	}
	else if (n == 1) {
		g.count = n;
		g.type = GVT_WCHAR;
		g.c16 = sz[0];
	}
	else
		return false;
}
bool ValUtils::set(VALUE & g, std::string & sz) {
	clear(g);
	int n = sz.size();
	if (n >1) {
		g.count = n;
		g.type = GVT_CHAR;
		g.carray = new char[n];
		const char * csz = sz.c_str();
		memcpy(g.carray, csz, n*sizeof(char));
	}
	else if (n == 1) {
		g.count = n;
		g.type = GVT_CHAR;
		g.c8 = sz[0];
	}
	else
		return false;
}
bool ValUtils::set(VALUE & g, std::wstring& sz) {
	clear(g);
	int n = sz.size();
	if (n >1) {
		g.count = n;
		g.type = GVT_CHAR;
		g.pvalue = (void*) new wchar_t[n];
		const wchar_t * csz = sz.c_str();
		memcpy(g.pvalue, csz, n*sizeof(wchar_t));
	}
	else if (n == 1) {
		g.count = n;
		g.type = GVT_WCHAR;
		g.c16= sz[0];
	}
	else
		return false;
}
end_gdb_namespace
end_gtl_namespace


