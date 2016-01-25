#include "value.h"
#include <limits>


begin_gtl_namespace
begin_gdb_namespace
///////////////////////////////////////////////////////////////////////////////
Value::Value() {
	m_data = new ValUtils::VALUE;
	_need_release = true;
	ValUtils::initial(*m_data);
}
Value::Value(const char * sz) {
	m_data = new ValUtils::VALUE;
	_need_release = true;
	ValUtils::initial(*m_data);
	ValUtils::set(*m_data, sz);
}
Value::Value(const wchar_t * sz) {
	m_data = new ValUtils::VALUE;
	_need_release = true;
	ValUtils::initial(*m_data);
	ValUtils::set(*m_data, sz);
}
Value::Value(signed char i8) {
	m_data = new ValUtils::VALUE;
	_need_release = true;
	m_data->type = ValUtils::GVT::GVT_INT8;
	m_data->count = 1;
	m_data->i8 = i8;
}
Value::Value(short i16) {
	m_data = new ValUtils::VALUE;
	_need_release = true;
	m_data->type = ValUtils::GVT::GVT_INT16;
	m_data->count = 1;
	m_data->i16 = i16;
}
Value::Value(int i32) {
	m_data = new ValUtils::VALUE;
	_need_release = true;
	m_data->type = ValUtils::GVT::GVT_INT32;
	m_data->count = 1;
	m_data->i32 = i32;
}
Value::Value(long long i64) {
	m_data = new ValUtils::VALUE;
	_need_release = true;
	m_data->type = ValUtils::GVT::GVT_INT64;
	m_data->count = 1;
	m_data->i64 = i64;
}
Value::Value(unsigned char u8) {
	m_data = new ValUtils::VALUE;
	_need_release = true;
	m_data->type = ValUtils::GVT::GVT_UINT8;
	m_data->count = 1;
	m_data->u8 = u8;
}
Value::Value(unsigned short u16) {
	m_data = new ValUtils::VALUE;
	_need_release = true;
	m_data->type = ValUtils::GVT::GVT_UINT16;
	m_data->count = 1;
	m_data->u16 = u16;
}
Value::Value(unsigned int u32) {
	m_data = new ValUtils::VALUE;
	_need_release = true;
	m_data->type = ValUtils::GVT::GVT_UINT32;
	m_data->count = 1;
	m_data->u32= u32;
}
Value::Value(unsigned long long u64) {
	m_data = new ValUtils::VALUE;
	_need_release = true;
	m_data->type = ValUtils::GVT::GVT_UINT64;
	m_data->count = 1;
	m_data->u64 = u64;
}
Value::Value(float f32) {
	m_data = new ValUtils::VALUE;
	_need_release = true;
	m_data->type = ValUtils::GVT::GVT_FLOAT32;
	m_data->count = 1;
	m_data->f32 = f32;
}
Value::Value(double f64) {
	m_data = new ValUtils::VALUE;
	_need_release = true;
	m_data->type = ValUtils::GVT::GVT_FLOAT64;
	m_data->count = 1;
	m_data->f64 = f64;
}
Value::Value(bool b) {
	m_data = new ValUtils::VALUE;
	_need_release = true;
	m_data->type = ValUtils::GVT::GVT_BOOL;
	m_data->count = 1;
	m_data->boolval = b;
}
Value::Value(int year, int month, int day) {
	m_data = new ValUtils::VALUE;
	_need_release = true;
	m_data->type = ValUtils::GVT::GVT_DATE;
	m_data->count = 1;
	m_data->date =new int[3];
	m_data->date[0] = year;
	m_data->date[1] = month;
	m_data->date[2] = day;
}
Value::Value(int year, int month, int day, int hour, int minute, int second, int millisecond  ) {
	m_data = new ValUtils::VALUE;
	_need_release = true;
	m_data->type = ValUtils::GVT::GVT_DATETIME;
	m_data->count = 1;
	m_data->datetime = new int[7];
	m_data->datetime[0] = year;
	m_data->datetime[1] = month;
	m_data->datetime[2] = day;
	m_data->datetime[3] = hour;
	m_data->datetime[4] = minute;
	m_data->datetime[5] = second;
	m_data->datetime[6] = millisecond;
}
Value::Value(int* i32s, int n) {
	m_data = new ValUtils::VALUE;
	_need_release = true;
	ValUtils::initial(*m_data);
	m_data->type = ValUtils::GVT::GVT_INT32;
	m_data->count = n;
	m_data->pi32 = new int[n];
	std::copy(i32s, i32s + n, m_data->pi32);
}
Value::Value(long long * i64s, int n) {
	m_data = new ValUtils::VALUE;
	_need_release = true;
	ValUtils::initial(*m_data);
	m_data->type = ValUtils::GVT::GVT_INT64;
	m_data->count = n;
	m_data->pi64 = new long long[n];
	std::copy(i64s, i64s + n, m_data->pi64);
}
Value::Value(unsigned char * u8s, int n) {
	m_data = new ValUtils::VALUE;
	_need_release = true;
	ValUtils::initial(*m_data);
	m_data->type = ValUtils::GVT::GVT_UINT8;
	m_data->count = n;
	m_data->pu8 = new unsigned char[n];
	std::copy(u8s, u8s + n, m_data->pu8);
}
Value::Value(unsigned int * u32s, int n) {
	m_data = new ValUtils::VALUE;
	_need_release = true;
	ValUtils::initial(*m_data);
	m_data->type = ValUtils::GVT::GVT_UINT32;
	m_data->count = n;
	m_data->pu32 = new unsigned int[n];
	std::copy(u32s, u32s + n, m_data->pu32);
}
Value::Value(unsigned long long * u64s, int n) {
	m_data = new ValUtils::VALUE;
	_need_release = true;
	ValUtils::initial(*m_data);
	m_data->type = ValUtils::GVT::GVT_UINT64;
	m_data->count = n;
	m_data->pu64 = new unsigned long long [n];
	std::copy(u64s, u64s + n, m_data->pu64);
}
Value::Value(float * f32s, int n) {
	m_data = new ValUtils::VALUE;
	_need_release = true;
	ValUtils::initial(*m_data);
	m_data->type = ValUtils::GVT::GVT_FLOAT32;
	m_data->count = n;
	m_data->pf32 = new float [n];
	std::copy(f32s, f32s + n, m_data->pf32);
}
Value::Value(double* f64s, int n) {
	m_data = new ValUtils::VALUE;
	_need_release = true;
	ValUtils::initial(*m_data);
	m_data->type = ValUtils::GVT::GVT_FLOAT64;
	m_data->count = n;
	m_data->pf64 = new double[n];
	std::copy(f64s, f64s + n, m_data->pf64);
}
Value::Value(const Value & v) {
	m_data = new ValUtils::VALUE;
	_need_release = true;
	ValUtils::initial(*m_data);
	ValUtils::copy(m_data, *(v.m_data));
}
Value::Value(ValUtils::VALUE * pv, bool auto_release) {
	if (auto_release) {
		m_data = pv;
		_need_release = auto_release;
	}
	else {
		m_data = new ValUtils::VALUE;
		_need_release = true;
		ValUtils::initial(*m_data);
		ValUtils::copy(m_data, *pv);
	}
}

Value::~Value() {
	clear();
}

void Value::clear() {
	if (_need_release && m_data) {
		ValUtils::clear(*m_data);
		delete m_data;
	}
	m_data = 0;
	_need_release = false;
}

void Value::attach(ValUtils::VALUE * pv) {
	clear();
	m_data = pv;
	_need_release = true;
}

ValUtils::VALUE * Value::detach() {
	ValUtils::VALUE * pv = m_data;	
	m_data = new ValUtils::VALUE;
	_need_release = true;
	ValUtils::initial(*m_data);
	return pv;
}

///////////////////////////////////////////////////////////////////////////////
void Value::write(Buffer & bs){
	ValUtils::write(bs, *m_data);
}
void Value::read(Buffer & bs){
	ValUtils::read(bs, *m_data);
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
//////////////////////////////////////////////////////////////////////////
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
	ValUtils::initialUInt8(m_data,v);
	return * this;
}

Value Value::operator = (short v){
	clear();
	ValUtils::initialInt16(m_data,v);
	return * this;
}

Value Value::operator = (unsigned short v){
	clear();
	ValUtils::initialInt16(m_data,v);
	return * this;
}

Value Value::operator = (int v) {
	clear();
	ValUtils::initialInt32(m_data,v);
	return * this;
}
Value Value::operator = (long v) {
	clear();
	ValUtils::initialInt32(m_data,v);
	return * this;
}
Value Value::operator = (unsigned long v)
{
	clear();
	ValUtils::initialUInt32(m_data,v);
	return * this;
}
Value Value::operator = (unsigned long long v){
	clear();
	ValUtils::initialUInt64(m_data,v);
	return * this;
}
Value Value::operator = (Identifier & v){
	clear();
	ValUtils::initialUInt64(m_data,v.get());
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



end_gdb_namespace
end_gtl_namespace


