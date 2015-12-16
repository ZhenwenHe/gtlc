#include "value.h"
#include <limits>
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

///////////////////////////////////////////////////////////////////////////////
void Value::write(Buffer & bs){
	ValUtils::write(bs, *m_data);
}
void Value::read(Buffer & bs){
	ValUtils::read(bs, *m_data);
}
Value::Value(){
	m_data = new ValUtils::VALUE;
	_need_release = true;
	ValUtils::initial(*m_data);
}
void Value::copy (const Value & v){
	clear();
	m_data = new ValUtils::VALUE;
	_need_release = true;
	ValUtils::initial(*m_data);
	ValUtils::copy(m_data,* v.m_data);
}
void Value::copy (const ValUtils::VALUE & v){
	if(m_data)
		ValUtils::copy(m_data,v);
	else{
		m_data = new ValUtils::VALUE;
		_need_release = true;
		ValUtils::initial(*m_data);
		ValUtils::copy(m_data,v);
	}
}
Value::Value(const Value & v){
	m_data = new ValUtils::VALUE;
	_need_release = true;
	ValUtils::initial(*m_data);
	ValUtils::copy(m_data,* v.m_data);
}
Value::Value( ValUtils::VALUE * pv, bool auto_release ){
	if(auto_release){
		m_data = pv;
		_need_release = auto_release;
	}
	else{
		m_data = new ValUtils::VALUE;
		_need_release = true;
		ValUtils::initial(*m_data);
		ValUtils::copy(m_data,*pv);
	}		
}
Value Value::operator = (const Value & v){
	copy(v);
	return *this;
}
Value Value::operator = (char v){
	clear();
	ValUtils::initialChar(m_data,v);
	return * this;
}

Value Value::operator = (unsigned char v){
	clear();
	ValUtils::initialUChar(m_data,v);
	return * this;
}

Value Value::operator = (short v){
	clear();
	ValUtils::initialShort(m_data,v);
	return * this;
}

Value Value::operator = (unsigned short v){
	clear();
	ValUtils::initialShort(m_data,v);
	return * this;
}

Value Value::operator = (int v) {
	clear();
	ValUtils::initialInt(m_data,v);
	return * this;
}
Value Value::operator = (long v) {
	clear();
	ValUtils::initialLong(m_data,v);
	return * this;
}
Value Value::operator = (unsigned long v)
{
	clear();
	ValUtils::initialULong(m_data,v);
	return * this;
}
Value Value::operator = (unsigned long long v){
	clear();
	ValUtils::initialULongLong(m_data,v);
	return * this;
}
Value Value::operator = (Identifier & v){
	clear();
	ValUtils::initialULongLong(m_data,v.get());
	return * this;
}
Value Value::operator = (float v){
	clear();
	ValUtils::initialFloat(m_data,v);
	return * this;
}
Value Value::operator = (double v) {
	clear();
	ValUtils::initialDouble(m_data,v);
	return * this;
}
Value Value::operator = (string v) {
	clear();
	ValUtils::initialChar(m_data,(char*)v.c_str(),(int)v.size());
	return * this;
}
Value Value::operator = (const char *  v) {
	clear();
	ValUtils::initialChar(m_data,(char*)v,(int)strlen(v));
	return * this;
}

Value::~Value(){
	clear();
}

void Value::clear(){	
	if(_need_release && m_data){
		ValUtils::clear(*m_data);
		delete m_data;
	}
	m_data=0;
	_need_release=false;
}
void Value::attach( ValUtils::VALUE * pv) {
	clear();
	m_data = pv;
	_need_release = true;
}
ValUtils::VALUE * Value::detach() {
	ValUtils::VALUE * pv=m_data;
	_need_release = true;
	m_data= new ValUtils::VALUE;
	ValUtils::initial(*m_data);
	return pv;
}

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
