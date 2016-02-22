#include "fielddefinition.h" 


begin_gtl_namespace
begin_gdb_namespace
FieldDefinition::FieldDefinition() {
	_name = "noname";
	_code = "noname";
	_caption = "noname";
	_type = GVT::GVT_CHAR8;
	_length = 50;
	_decimal = 0;
	_nullable = 0;
	_defaultValue = 0;
	_justification = 0;
	_ignore = 0;
}
/**
这是一个简化的构造函数，只需要输入字段名称，字段类型和个数，函数自动计算length的值
例如：FieldDefinition（"Field1",GVT_CHAR8,50）构建50个字符宽度的文本列，length=50
FieldDefinition（"Field2",GVT_CHAR16,50）构建50个字符宽度的宽文本列，length=100
FieldDefinition（"Field3",GVT_INT64,1）构建64BITS的整数列，length=8
FieldDefinition（"Field4",GVT_INT64,10）构建64BITS的整数数组列，length=80
FieldDefinition（"Field5",GVT_DATETIME,1）构建datetime列，length=28
*/
FieldDefinition::FieldDefinition(std::string name, GVT type, int count ) {
	_name = name;
	_code = name;
	_caption = name;
	_type = type;
	_decimal = -1;
	_nullable = 1;
	_justification = 0;
	_ignore = 0;
	_defaultValue = 0;
	_length = ValUtils::getTypeSize(type)*count;
}
/*
GVT            NAME      SIZE       C/C++                 RANGE
GVT_BOOL       bool      1          bool
GVT_INT8       int8      1          signed char
GVT_INT16      int16     2          short
GVT_INT32      int32     4          int
GVT_INT64      int64     8          long long
GVT_UINT8      uint8     1          unsigned char
GVT_UINT16     uint16    2          unsigned short
GVT_UINT32     uint32    4          unsigned int
GVT_UINT64     uint64    8          unsigned long long
GVT_FLOAT32    float32   4          float                 -3.40E+38 ~ +3.40E+38
GVT_FLOAT64    float64   8          double                -1.79E+308 ~ +1.79E+308
GVT_DATE       date      4*3=12     int [3]
GVT_DATETIME   datetime  4*7=28     int [7]
GVT_CHAR8      char8     1-2GB      char
GVT_CHAR16     char16    2-2GB      wchar_t
*/
FieldDefinition::FieldDefinition(std::string name, std::string  gvt_typename, int count ) {
	_name = name;
	_code = name;
	_caption = name;
	_type = (GVT)ValUtils::getType(gvt_typename.c_str());
	_decimal = -1;
	_nullable = 1;
	_justification = 0;
	_ignore = 0;
	_defaultValue = 0;
	_length = ValUtils::getTypeSize(_type)*count;
}
FieldDefinition::FieldDefinition(std::string name,
	GVT type, int length, int decimal ,
	bool nullable  , const VALUE * defaultVal  ,
	std::string code  , std::string caption , int justification , bool ignore ) {
	_name = name;
	_code = code;
	_caption = caption;
	_type = type;
	_length = length;
	_decimal = decimal;
	_nullable = nullable ? 1 : 0;
	_justification = justification;
	_ignore = ignore ? 1 : 0;
	if (defaultVal) {
		_defaultValue = ValUtils::newValue();
		ValUtils::copy(_defaultValue, *defaultVal);
	}
	//调整length
	int tsize = ValUtils::getTypeSize(_type);
	if (_length < tsize)
		_length = tsize;
	else if (_length>tsize) {
		int m = _length % tsize;
		if (m > 0) {
			int t = _length / tsize;
			t++;
			_length = tsize*t;
		}
	}
}
FieldDefinition::FieldDefinition(const FieldDefinition & t) {
	_defaultValue = 0;
	copy(t);
}
/** @} */

/** @name 复制与等号赋值重载
*  @{
*/
void FieldDefinition::copy(const FieldDefinition & t) {
	_name = t._name;
	_code = t._code;
	_caption = t._caption;
	_type = t._type;
	_length = t._length;
	_decimal = t._decimal;
	_nullable = t._nullable;
	_justification = t._justification;
	_ignore = t._ignore;
	if (t._defaultValue) {
		if (_defaultValue)
			ValUtils::copy(_defaultValue, *(t._defaultValue));
		else {
			_defaultValue = ValUtils::newValue();
			ValUtils::copy(_defaultValue, *(t._defaultValue));
		}
	}
	else {
		if (_defaultValue) {
			ValUtils::clear(*_defaultValue);
			delete _defaultValue;
		}
		_defaultValue = 0;
	}
}
FieldDefinition FieldDefinition::operator = (const FieldDefinition & t) {
	copy(t);
	return *this;
}
bool FieldDefinition::operator==(const FieldDefinition & t) {
	if (_name == t._name &&	_type == t._type &&	_length == t._length) {
		return true;
	}
	else {
		return false;
	}
}
void  FieldDefinition::write(std::ostream & f)
{
	//1 name
	int s = _name.size();
	f.write((char*)(&s), sizeof(int));
	if (s>0)
		f.write((char*)(_name.c_str()), s);
	//2 code
	s = _code.size();
	f.write((char*)(&s), sizeof(int));
	if (s>0)
		f.write((char*)(_code.c_str()), s);
	//3 caption
	s = _caption.size();
	f.write((char*)(&s), sizeof(int));
	if (s>0)
		f.write((char*)(_caption.c_str()), s);
	//4 type
	s = _type;
	f.write((char*)s, sizeof(int));
	//5 length
	f.write((char*)(&_length), sizeof(int));
	//6 decimal
	f.write((char*)(&_decimal), sizeof(int));
	//7 nullable
	f.write((char*)(&_nullable), sizeof(int));
	//8 _justification
	f.write((char*)(&_justification), sizeof(int));
	//9 _ignore 
	f.write((char*)(&_ignore), sizeof(int));
	//10 default value
	if (_defaultValue) {
		s = 1;
		f.write((char*)s, sizeof(int));
		ValUtils::write(f, *_defaultValue);
	}
	else {
		s = 0;
		f.write((char*)s, sizeof(int));
	}

}
/**  从流对象中读取属性类型对象
*	 @param [in,out]  f 流对象
*   @return void
*/
void  FieldDefinition::read(std::istream & f) {
	char ca[256];
	//1 name
	int s = 0;
	f.read((char*)(&s), sizeof(int));
	if (s > 0) {
		f.read((char*)(ca), s);
		_name.assign(ca, s);
	}
	//2 code
	s = _code.size();
	f.read((char*)(&s), sizeof(int));
	if (s > 0) {
		f.read((char*)(ca), s);
		_code.assign(ca, s);
	}
	//3 caption
	s = _caption.size();
	f.read((char*)(&s), sizeof(int));
	if (s > 0) {
		f.read((char*)(ca), s);
		_caption.assign(ca, s);
	}
	//4 type
	f.read((char*)s, sizeof(int));
	_type = (GVT)s;
	//5 length
	f.read((char*)(&_length), sizeof(int));
	//6 decimal
	f.read((char*)(&_decimal), sizeof(int));
	//7 nullable
	f.read((char*)(&_nullable), sizeof(int));
	//8 _justification
	f.read((char*)(&_justification), sizeof(int));
	//9 _ignore 
	f.read((char*)(&_ignore), sizeof(int));
	//10 default value
	f.read((char*)s, sizeof(int));
	if (s != 0) {
		if (_defaultValue)
			ValUtils::read(f, *_defaultValue);
		else {
			_defaultValue = ValUtils::newValue();
			ValUtils::read(f, *_defaultValue);
		}
	}
}
end_gdb_namespace
end_gtl_namespace


