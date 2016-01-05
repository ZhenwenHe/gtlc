#include "stdlib.h"
#include "stdio.h"
#include "identifier.h"



begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

////////////////////////////////////////////////////////////////////////
static unsigned long global_number_id=0;



Identifier::Identifier(){//采用随机用户ID生成Identifier
	_id = 0;
	
	global_number_id=global_number_id % 0xffffff;
	ULONGNumber n;
	n.ns=global_number_id++;
	_sid.numbers[0]=n.bs[0];
	_sid.numbers[1]=n.bs[1];
	_sid.numbers[2]=n.bs[2];
	_sid.time_stamp = (unsigned long) ::time(NULL);

	srand(GetTickCount() );		
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
end_cug_namespace
end_edu_namespace
end_cn_namespace
