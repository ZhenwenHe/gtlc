#include "stdlib.h"
#include "stdio.h"
#include "identifier.h"





begin_gtl_namespace
begin_gdb_namespace

////////////////////////////////////////////////////////////////////////
static unsigned long global_number_id=0;

bool operator==(const Identifier & id1, const Identifier & id2) {
	return id1._id == id2._id;
}

bool operator==(const Identifier & id1, unsigned long long  id2) {
	return id1._id == id2;
}

bool operator!=(const Identifier & id1, const Identifier & id2) {
	return id1._id != id2._id;
}

bool operator!=(const Identifier & id1, unsigned long long  id2) {
	return id1._id != id2;
}

bool operator>(const Identifier & id1, const Identifier & id2) {
	return id1._id>id2._id;
}

bool operator>(const Identifier & id1, unsigned long long  id2) {
	return id1._id>id2;
}

bool operator<(const Identifier & id1, const Identifier & id2) {
	return id1._id<id2._id;
}

bool operator<(const Identifier & id1, unsigned long long  id2) {
	return id1._id<id2;
}

Identifier operator+(const Identifier & id1, const Identifier & id2) {
	return Identifier(id1._id + id2._id);
}

Identifier operator+(const Identifier & id1, unsigned long long  id2) {
	return Identifier(id1._id + id2);
}

Identifier operator-(const Identifier & id1, const Identifier & id2) {
	return Identifier(id1._id - id2._id);
}

Identifier operator-(const Identifier & id1, unsigned long long  id2) {
	return Identifier(id1._id - id2);
}


std::ostream & operator <<(std::ostream & s, const Identifier & id1) {
	s << id1._id;
	return s;
}

std::istream & operator >> (std::istream & s, const Identifier & id1) {
	s >> id1._id;
	return s;
}

Identifier::Identifier(){//采用随机用户ID生成Identifier
	_id = 0;

	global_number_id=global_number_id % 0xffffff;
	ULONGNumber n;
	n.ns=global_number_id++;
	_sid.numbers[0]=n.bs[0];
	_sid.numbers[1]=n.bs[1];
	_sid.numbers[2]=n.bs[2];
	_sid.time_stamp = (unsigned long) ::time(NULL);

#if(USING_OS_TYPE==0)
	srand(GetTickCount());
#else
    {
        struct timeval tv1;
        gettimeofday(&tv1, NULL);
        srand(tv1.tv_usec);
    }
#endif

	_sid.user_id = rand()%255;
}

Identifier::raw_type  Identifier::generate() {
	Identifier i;
	return i.get();
}
Identifier::Identifier(unsigned char user_id/*=0*/){
	_id = 0;
	_sid.user_id = user_id;

	global_number_id=global_number_id % 0xffffff;
	ULONGNumber n;
	n.ns=global_number_id++;
	_sid.numbers[0]=n.bs[0];
	_sid.numbers[1]=n.bs[1];
	_sid.numbers[2]=n.bs[2];
	_sid.time_stamp = (unsigned long) ::time(NULL);
}

Identifier::Identifier(const Identifier & ids){
	_id = ids._id;
}

Identifier Identifier::operator= (const Identifier & ids) {
	_id = ids._id;
	return *this;
}
Identifier Identifier::operator = (unsigned long long  ids){
	_id = ids;
	return *this;
}
Identifier Identifier::operator = (int  ids){
	_id = ids;
	return *this;
}
Identifier Identifier::operator = (unsigned long  ids){
	_id = ids;
	return *this;
}
Identifier Identifier::operator = (unsigned int  ids){
	_id = ids;
	return *this;
}
void  Identifier::write(std::ostream & f){
	f.write((const char * )_bid,sizeof(unsigned char) *8);
}
void  Identifier::read(std::istream & f){
	f.read(( char * )_bid,sizeof(unsigned char) *8);
}

end_gdb_namespace
end_gtl_namespace


