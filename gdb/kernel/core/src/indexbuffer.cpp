
#include "indexbuffer.h"






begin_gtl_namespace
begin_gdb_namespace

IndexBuffer::IndexBuffer(){
	_type = DBF_UINT32;
}
IndexBuffer::IndexBuffer(unsigned int * pi,size_t number,bool b_alloc)//构造函数，如果b_alloc为真，则会新申请一块大小为s的内存块，并复制d的内容，反之，则指向d
:DescribableBuffer(pi,number*sizeof(unsigned int),DBF_UINT32,b_alloc)
{

}
void IndexBuffer::copy(const IndexBuffer & b)//拷贝Buffer函数，如果b的_alloc成员为假，则拷贝后的Buffer对象与b公用一个内存块，否则为内存块拷贝
{
	DescribableBuffer::copy((const DescribableBuffer&)b);
}
IndexBuffer::IndexBuffer(const IndexBuffer & b)//拷贝构造函数
{
	copy(b);
}
IndexBuffer IndexBuffer::operator = (const IndexBuffer & b)//等号赋值操作
{
	copy(b);
	return * this;
}


void IndexBuffer::add(unsigned int i){
	Buffer::write(&i,sizeof(unsigned int));
}
void IndexBuffer::add(unsigned int i,unsigned int j){
	unsigned int ps [] = {i,j};
	Buffer::write(ps,sizeof(unsigned int)*2);
}
void IndexBuffer::add(unsigned int i,unsigned int j,unsigned int k){
	unsigned int ps [] = {i,j,k};
	Buffer::write(ps,sizeof(unsigned int)*3);
}
void IndexBuffer::add(unsigned int i,unsigned int j,unsigned int k,unsigned int m){
	unsigned int ps [] = {i,j,k,m};
	Buffer::write(ps,sizeof(unsigned int)*4);
}

void IndexBuffer::add(const long * data,const long size){
	unsigned int * ps   = new unsigned int [size];
	for(long i = 0;i<size;i++)
		ps[i]=data[i];
	Buffer::write(ps,sizeof(unsigned int)*size);
	delete [] ps;
}
end_gdb_namespace
end_gtl_namespace


