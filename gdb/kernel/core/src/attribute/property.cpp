#include "property.h" 


begin_gtl_namespace
begin_gdb_namespace 
//
//bool Property::Type::isNumber() {
//	if(strcmp(_type.c_str(),PROPTYPES[0])==0 || strcmp(_type.c_str(),PROPTYPES[1])==0 ) 
//		return true;
//	else
//		return false;
//}
//
//void Property::Type::copy(const Property::Type & t){
//	_name=t._name;
//	_code=t._code;
//	_type=t._type;
//	_length=t._length;
//	_decimal=t._decimal;
//	_nullable=t._nullable;
//}
//
//Property::Type Property::Type::operator = (const Property::Type & t){
//	_name=t._name;
//	_code=t._code;
//	_type=t._type;
//	_length=t._length;
//	_decimal=t._decimal;
//	_nullable=t._nullable;
//	return *this;
//}
//
//bool Property::Type::operator==(const Property::Type & t){
//	if (_name == t._name &&	_code == t._code &&	
//		_type == t._type &&	
//		_length == t._length &&
//		_decimal == t._decimal &&
//		_nullable == t._nullable){
//			return true;
//	}
//	else{
//		return false;
//	}	
//}
//
//void Property::Type::write(std::ostream & f){
//
//	int s = _name.size();
//	f.write((char*)(&s),sizeof(int));
//	f.write((char*)(_name.c_str()),s);
//
//	s = _code.size();
//	f.write((char*)(&s),sizeof(int));
//	f.write((char*)(_code.c_str()),s);
//
//	s = _type.size();
//	f.write((char*)(&s),sizeof(int));
//	f.write((char*)(_type.c_str()),s);
//
//	f.write((char*)(&_length),sizeof(int));
//
//	f.write((char*)(&_decimal),sizeof(int));
//
//	s=_nullable?1:0;
//	f.write((char*)(&s),sizeof(int));
//}
//void Property::Type::read(std::istream & f){
//	int s = 0;
//	char ca[255];
//	f.read((char*)(&s),sizeof(int));
//	f.read((char*)(ca),s);
//	_name.assign(ca,ca+s);
//
//
//	f.read((char*)(&s),sizeof(int));
//	f.read((char*)(ca),s);
//	_code.assign(ca,ca+s);
//
//	f.read((char*)(&s),sizeof(int));
//	f.read((char*)(ca),s);
//	_type.assign(ca,ca+s);
//
//
//	f.read((char*)(&_length),sizeof(int));
//
//	f.read((char*)(&_decimal),sizeof(int));
//
//	f.read((char*)(&s),sizeof(int));
//
//	_nullable = (s!=0);
//}


/////////////////////////////////////////////////////////////////////////
Property::Property() :Value(){

};
void Property::copy(const Property & p){
	*((Value*)(this)) = *((Value*)(&p));
	_name = p._name;
}
Property::Property(const Property & p) :Value((const Value &)p){
	_name = p._name;
}
Property::Property(std::string sz, const VALUE & v) : Value(){
	ValUtils::copy(m_data, v);
	_name = sz;
}
Property Property::operator = (const Property & p){
	copy(p);
	return *this;
}


void Property::write(Buffer & bs){
	int len = (int) _name.size();
	bs.write(&len, sizeof(int));
	bs.write((void*)_name.c_str(), len*sizeof(char));
	Value::write(bs);
}

void Property::read(Buffer & bs){
	int len = 0;
	bs.read(&len, sizeof(int));
	char * pc = new char[len];
	bs.read(pc, len);
	_name.assign(pc, len);
	Value::read(bs);
}

end_gdb_namespace
end_gtl_namespace


