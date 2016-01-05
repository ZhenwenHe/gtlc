#include "describablebuffer.h"




begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace
bool DescribableBuffer::changeType(DBF t){
	return true;
}

DescribableBuffer::DescribableBuffer():Buffer(){
	_type = DBF_VOID;
}
DescribableBuffer::DescribableBuffer(void * d,size_t s,DBF t ,bool b_alloc):Buffer(d,s,b_alloc)//构造函数，如果b_alloc为真，则会新申请一块大小为s的内存块，并复制d的内容，反之，则指向d
{
	_type = DBF_VOID;
}
void DescribableBuffer::copy(const DescribableBuffer & b)//拷贝Buffer函数，如果b的_alloc成员为假，则拷贝后的Buffer对象与b公用一个内存块，否则为内存块拷贝
{
	Buffer::copy(b);
	_type = b._type;
}
DescribableBuffer::DescribableBuffer(const DescribableBuffer & b)//拷贝构造函数
{
	copy(b);
}
DescribableBuffer DescribableBuffer::operator = (const DescribableBuffer & b)//等号赋值操作
{
	copy(b);
	return *this;
}
int DescribableBuffer::sizeType(DBF t ){
	switch (t)
	{
	case DBF_VOID:
	case DBF_INT8:
	case DBF_UINT8:
		return 1;
	case DBF_INT16:
	case DBF_UINT16:
		return 2;
	case DBF_INT32:
	case DBF_UINT32:
		return 4;
	case DBF_INT64:
	case DBF_UINT64:
		return 8;

	case DBF_FLOAT:
		return sizeof(float);
	case DBF_3FLOAT:
		return sizeof(float)*3;
	case DBF_4FLOAT:
		return sizeof(float)*4;
	case DBF_5FLOAT:
		return sizeof(float)*5;
	case DBF_6FLOAT:
		return sizeof(float)*6;
	case DBF_7FLOAT:
		return sizeof(float)*7;
	case DBF_8FLOAT:
		return sizeof(float)*8;
	case DBF_9FLOAT:
		return sizeof(float)*9;
	case DBF_16FLOAT:
		return sizeof(float)*16;

	case DBF_DOUBLE:
		return sizeof(double);
	case DBF_3DOUBLE:
		return sizeof(double)*3;
	case DBF_4DOUBLE:
		return sizeof(double)*4;
	case DBF_5DOUBLE:
		return sizeof(double)*5;
	case DBF_6DOUBLE:
		return sizeof(double)*6;
	case DBF_7DOUBLE:
		return sizeof(double)*7;
	case DBF_8DOUBLE:
		return sizeof(double)*8;
	case DBF_9DOUBLE:
		return sizeof(double)*9;
	case DBF_16DOUBLE:
		return sizeof(double)*16;
	}
	return 1;
}
void DescribableBuffer::get(std::vector<double> & dv){
	switch (_type)
	{
	case DBF_VOID:
		{
			int n = size()/sizeof(double);
			double * p = (double * ) _data;
			dv.resize(n);
			for(int i=0;i<n;i++)
				dv[i]= *(p+i);
			break;
		}
	case DBF_INT8:
		{
			int n = size()/sizeType(_type);
			char * p = (char * ) _data;
			dv.resize(n);
			for(int i=0;i<n;i++)
				dv[i]= *(p+i);
			break;
		}
	case DBF_UINT8:
		{
			int n = size()/sizeType(_type);
			unsigned char * p = (unsigned char * ) _data;
			dv.resize(n);
			for(int i=0;i<n;i++)
				dv[i]= *(p+i);
			break;
		}
	case DBF_INT16:
		{
			int n = size()/sizeType(_type);
			short * p = (short * ) _data;
			dv.resize(n);
			for(int i=0;i<n;i++)
				dv[i]= *(p+i);
			break;
		}
	case DBF_UINT16:
		{
			int n = size()/sizeType(_type);
			unsigned short * p = (unsigned short  * ) _data;
			dv.resize(n);
			for(int i=0;i<n;i++)
				dv[i]= *(p+i);
			break;
		}
	case DBF_INT32:
		{
			int n = size()/sizeType(_type);
			int * p = (int * ) _data;
			dv.resize(n);
			for(int i=0;i<n;i++)
				dv[i]= *(p+i);
			break;
		}
	case DBF_UINT32:
		{
			int n = size()/sizeType(_type);
			unsigned int  * p = (unsigned int * ) _data;
			dv.resize(n);
			for(int i=0;i<n;i++)
				dv[i]= *(p+i);
			break;
		}
	case DBF_INT64:
		{
			int n = size()/sizeType(_type);
			__int64 * p = (__int64 * ) _data;
			dv.resize(n);
			for(int i=0;i<n;i++)
				dv[i]= *(p+i);
			break;
		}
	case DBF_UINT64:
		{
			int n = size()/sizeType(_type);
			__int64 * p = (__int64 * ) _data;
			dv.resize(n);
			for(int i=0;i<n;i++)
				dv[i]= *(p+i);
			break;
		}
	case DBF_FLOAT:
	case DBF_3FLOAT:
	case DBF_4FLOAT:
	case DBF_5FLOAT:
	case DBF_6FLOAT:
	case DBF_7FLOAT:
	case DBF_8FLOAT:
	case DBF_9FLOAT:
	case DBF_16FLOAT:
		{
			int n = size()/sizeof(float);
			float * p = (float * ) _data;
			dv.resize(n);
			for(int i=0;i<n;i++)
				dv[i]= *(p+i);
			break;
		}
	case DBF_DOUBLE:
	case DBF_3DOUBLE:
	case DBF_4DOUBLE:
	case DBF_5DOUBLE:
	case DBF_6DOUBLE:
	case DBF_7DOUBLE:
	case DBF_8DOUBLE:
	case DBF_9DOUBLE:
	case DBF_16DOUBLE:
		{
			int n = size()/sizeof(double);
			double * p = (double * ) _data;
			dv.resize(n);
			for(int i=0;i<n;i++)
				dv[i]= *(p+i);
			break;
		}
	}
}
void DescribableBuffer::get(std::vector<float> & dv){
	switch (_type)
	{
	case DBF_VOID:
		{
			int n = size()/sizeof(float);
			float * p = (float * ) _data;
			dv.resize(n);
			for(int i=0;i<n;i++)
				dv[i]= *(p+i);
			break;
		}
	case DBF_INT8:
		{
			int n = size()/sizeType(_type);
			char * p = (char * ) _data;
			dv.resize(n);
			for(int i=0;i<n;i++)
				dv[i]= *(p+i);
			break;
		}
	case DBF_UINT8:
		{
			int n = size()/sizeType(_type);
			unsigned char * p = (unsigned char * ) _data;
			dv.resize(n);
			for(int i=0;i<n;i++)
				dv[i]= *(p+i);
			break;
		}
	case DBF_INT16:
		{
			int n = size()/sizeType(_type);
			short * p = (short * ) _data;
			dv.resize(n);
			for(int i=0;i<n;i++)
				dv[i]= *(p+i);
			break;
		}
	case DBF_UINT16:
		{
			int n = size()/sizeType(_type);
			unsigned short * p = (unsigned short  * ) _data;
			dv.resize(n);
			for(int i=0;i<n;i++)
				dv[i]= *(p+i);
			break;
		}
	case DBF_INT32:
		{
			int n = size()/sizeType(_type);
			int * p = (int * ) _data;
			dv.resize(n);
			for(int i=0;i<n;i++)
				dv[i]= *(p+i);
			break;
		}
	case DBF_UINT32:
		{
			int n = size()/sizeType(_type);
			unsigned int  * p = (unsigned int * ) _data;
			dv.resize(n);
			for(int i=0;i<n;i++)
				dv[i]= *(p+i);
			break;
		}
	case DBF_INT64:
		{
			int n = size()/sizeType(_type);
			__int64 * p = (__int64 * ) _data;
			dv.resize(n);
			for(int i=0;i<n;i++)
				dv[i]= *(p+i);
			break;
		}
	case DBF_UINT64:
		{
			int n = size()/sizeType(_type);
			__int64 * p = (__int64 * ) _data;
			dv.resize(n);
			for(int i=0;i<n;i++)
				dv[i]= *(p+i);
			break;
		}
	case DBF_FLOAT:
	case DBF_3FLOAT:
	case DBF_4FLOAT:
	case DBF_5FLOAT:
	case DBF_6FLOAT:
	case DBF_7FLOAT:
	case DBF_8FLOAT:
	case DBF_9FLOAT:
	case DBF_16FLOAT:
		{
			int n = size()/sizeof(float);
			float * p = (float * ) _data;
			dv.resize(n);
			for(int i=0;i<n;i++)
				dv[i]= *(p+i);
			break;
		}
	case DBF_DOUBLE:
	case DBF_3DOUBLE:
	case DBF_4DOUBLE:
	case DBF_5DOUBLE:
	case DBF_6DOUBLE:
	case DBF_7DOUBLE:
	case DBF_8DOUBLE:
	case DBF_9DOUBLE:
	case DBF_16DOUBLE:
		{
			int n = size()/sizeof(double);
			double * p = (double * ) _data;
			dv.resize(n);
			for(int i=0;i<n;i++)
				dv[i]= *(p+i);
			break;
		}
	}
}
void DescribableBuffer::get( int * pc , float ** pdv){
	switch (_type)
	{
	case DBF_VOID:
		{
			int n = size()/sizeof(float);
			float * p = (float * ) _data;
			float * dv = new float[n];
			*pdv = dv;
			*pc = n;
			for(int i=0;i<n;i++)
				dv[i]= *(p+i);
			break;
		}
	case DBF_INT8:
		{
			int n = size()/sizeType(_type);
			char * p = (char * ) _data;
			float * dv = new float[n];
			*pdv = dv;
			*pc = n;
			for(int i=0;i<n;i++)
				dv[i]= *(p+i);
			break;
		}
	case DBF_UINT8:
		{
			int n = size()/sizeType(_type);
			unsigned char * p = (unsigned char * ) _data;
			float * dv = new float[n];
			*pdv = dv;
			*pc = n;
			for(int i=0;i<n;i++)
				dv[i]= *(p+i);
			break;
		}
	case DBF_INT16:
		{
			int n = size()/sizeType(_type);
			short * p = (short * ) _data;
			float * dv = new float[n];
			*pdv = dv;
			*pc = n;
			for(int i=0;i<n;i++)
				dv[i]= *(p+i);
			break;
		}
	case DBF_UINT16:
		{
			int n = size()/sizeType(_type);
			unsigned short * p = (unsigned short  * ) _data;
			float * dv = new float[n];
			*pdv = dv;
			*pc = n;
			for(int i=0;i<n;i++)
				dv[i]= *(p+i);
			break;
		}
	case DBF_INT32:
		{
			int n = size()/sizeType(_type);
			int * p = (int * ) _data;
			float * dv = new float[n];
			*pdv = dv;
			*pc = n;
			for(int i=0;i<n;i++)
				dv[i]= *(p+i);
			break;
		}
	case DBF_UINT32:
		{
			int n = size()/sizeType(_type);
			unsigned int  * p = (unsigned int * ) _data;
			float * dv = new float[n];
			*pdv = dv;
			*pc = n;
			for(int i=0;i<n;i++)
				dv[i]= *(p+i);
			break;
		}
	case DBF_INT64:
		{
			int n = size()/sizeType(_type);
			__int64 * p = (__int64 * ) _data;
			float * dv = new float[n];
			*pdv = dv;
			*pc = n;
			for(int i=0;i<n;i++)
				dv[i]= *(p+i);
			break;
		}
	case DBF_UINT64:
		{
			int n = size()/sizeType(_type);
			__int64 * p = (__int64 * ) _data;
			float * dv = new float[n];
			*pdv = dv;
			*pc = n;
			for(int i=0;i<n;i++)
				dv[i]= *(p+i);
			break;
		}
	case DBF_FLOAT:
	case DBF_3FLOAT:
	case DBF_4FLOAT:
	case DBF_5FLOAT:
	case DBF_6FLOAT:
	case DBF_7FLOAT:
	case DBF_8FLOAT:
	case DBF_9FLOAT:
	case DBF_16FLOAT:
		{
			int n = size()/sizeof(float);
			float * dv = new float[n];
			*pdv = dv;
			*pc = n;
			memcpy(dv,_data,size());
			break;
		}
	case DBF_DOUBLE:
	case DBF_3DOUBLE:
	case DBF_4DOUBLE:
	case DBF_5DOUBLE:
	case DBF_6DOUBLE:
	case DBF_7DOUBLE:
	case DBF_8DOUBLE:
	case DBF_9DOUBLE:
	case DBF_16DOUBLE:
		{
			int n = size()/sizeof(double);
			double * p = (double *) _data;
			float * dv = new float[n];
			*pdv = dv;
			*pc = n;
			for(int i=0;i<n;i++){
				dv[i]=*(p+i);
			}
			break;
		}
	}
}
void DescribableBuffer::get( int * pc , double ** pdv){
	switch (_type)
	{
	case DBF_VOID:
		{
			int n = size()/sizeof(double);
			float * p = (float * ) _data;
			double * dv = new double[n];
			*pdv = dv;
			*pc = n;
			for(int i=0;i<n;i++)
				dv[i]= *(p+i);
			break;
		}
	case DBF_INT8:
		{
			int n = size()/sizeType(_type);
			char * p = (char * ) _data;
			double * dv = new double[n];
			*pdv = dv;
			*pc = n;
			for(int i=0;i<n;i++)
				dv[i]= *(p+i);
			break;
		}
	case DBF_UINT8:
		{
			int n = size()/sizeType(_type);
			unsigned char * p = (unsigned char * ) _data;
			double * dv = new double[n];
			*pdv = dv;
			*pc = n;
			for(int i=0;i<n;i++)
				dv[i]= *(p+i);
			break;
		}
	case DBF_INT16:
		{
			int n = size()/sizeType(_type);
			short * p = (short * ) _data;
			double * dv = new double[n];
			*pdv = dv;
			*pc = n;
			for(int i=0;i<n;i++)
				dv[i]= *(p+i);
			break;
		}
	case DBF_UINT16:
		{
			int n = size()/sizeType(_type);
			unsigned short * p = (unsigned short  * ) _data;
			double * dv = new double[n];
			*pdv = dv;
			*pc = n;
			for(int i=0;i<n;i++)
				dv[i]= *(p+i);
			break;
		}
	case DBF_INT32:
		{
			int n = size()/sizeType(_type);
			int * p = (int * ) _data;
			double * dv = new double[n];
			*pdv = dv;
			*pc = n;
			for(int i=0;i<n;i++)
				dv[i]= *(p+i);
			break;
		}
	case DBF_UINT32:
		{
			int n = size()/sizeType(_type);
			unsigned int  * p = (unsigned int * ) _data;
			double * dv = new double[n];
			*pdv = dv;
			*pc = n;
			for(int i=0;i<n;i++)
				dv[i]= *(p+i);
			break;
		}
	case DBF_INT64:
		{
			int n = size()/sizeType(_type);
			__int64 * p = (__int64 * ) _data;
			double * dv = new double[n];
			*pdv = dv;
			*pc = n;
			for(int i=0;i<n;i++)
				dv[i]= *(p+i);
			break;
		}
	case DBF_UINT64:
		{
			int n = size()/sizeType(_type);
			__int64 * p = (__int64 * ) _data;
			double * dv = new double[n];
			*pdv = dv;
			*pc = n;
			for(int i=0;i<n;i++)
				dv[i]= *(p+i);
			break;
		}
	case DBF_FLOAT:
	case DBF_3FLOAT:
	case DBF_4FLOAT:
	case DBF_5FLOAT:
	case DBF_6FLOAT:
	case DBF_7FLOAT:
	case DBF_8FLOAT:
	case DBF_9FLOAT:
	case DBF_16FLOAT:
		{
			int n = size()/sizeof(float);
			float * p = (float*) _data;
			double * dv = new double[n];
			*pdv = dv;
			*pc = n;
			for(int i=0;i<n;i++)
				dv[i]= *(p+i);
			break;
		}
	case DBF_DOUBLE:
	case DBF_3DOUBLE:
	case DBF_4DOUBLE:
	case DBF_5DOUBLE:
	case DBF_6DOUBLE:
	case DBF_7DOUBLE:
	case DBF_8DOUBLE:
	case DBF_9DOUBLE:
	case DBF_16DOUBLE:
		{
			int n = size()/sizeof(double);
			double * dv = new double[n];
			*pdv = dv;
			*pc = n;
			memcpy(dv,_data,size());
			break;
		}
	}
}
void DescribableBuffer::get(std::vector<int> & dv)
{
	switch (_type)
	{
	case DBF_VOID:
		{
			int n = size()/sizeof(int);
			int * p = (int * ) _data;
			dv.resize(n);
			for(int i=0;i<n;i++)
				dv[i]= *(p+i);
			break;
		}
	case DBF_INT8:
		{
			int n = size()/sizeType(_type);
			char * p = (char * ) _data;
			dv.resize(n);
			for(int i=0;i<n;i++)
				dv[i]= *(p+i);
			break;
		}
	case DBF_UINT8:
		{
			int n = size()/sizeType(_type);
			unsigned char * p = (unsigned char * ) _data;
			dv.resize(n);
			for(int i=0;i<n;i++)
				dv[i]= *(p+i);
			break;
		}
	case DBF_INT16:
		{
			int n = size()/sizeType(_type);
			short * p = (short * ) _data;
			dv.resize(n);
			for(int i=0;i<n;i++)
				dv[i]= *(p+i);
			break;
		}
	case DBF_UINT16:
		{
			int n = size()/sizeType(_type);
			unsigned short * p = (unsigned short  * ) _data;
			dv.resize(n);
			for(int i=0;i<n;i++)
				dv[i]= *(p+i);
			break;
		}
	case DBF_INT32:
		{
			int n = size()/sizeType(_type);
			int * p = (int * ) _data;
			dv.resize(n);
			for(int i=0;i<n;i++)
				dv[i]= *(p+i);
			break;
		}
	case DBF_UINT32:
		{
			int n = size()/sizeType(_type);
			unsigned int  * p = (unsigned int * ) _data;
			dv.resize(n);
			for(int i=0;i<n;i++)
				dv[i]= *(p+i);
			break;
		}
	case DBF_INT64:
		{
			int n = size()/sizeType(_type);
			__int64 * p = (__int64 * ) _data;
			dv.resize(n);
			for(int i=0;i<n;i++)
				dv[i]= *(p+i);
			break;
		}
	case DBF_UINT64:
		{
			int n = size()/sizeType(_type);
			__int64 * p = (__int64 * ) _data;
			dv.resize(n);
			for(int i=0;i<n;i++)
				dv[i]= *(p+i);
			break;
		}
	case DBF_FLOAT:
	case DBF_3FLOAT:
	case DBF_4FLOAT:
	case DBF_5FLOAT:
	case DBF_6FLOAT:
	case DBF_7FLOAT:
	case DBF_8FLOAT:
	case DBF_9FLOAT:
	case DBF_16FLOAT:
		{
			int n = size()/sizeof(float);
			float * p = (float * ) _data;
			dv.resize(n);
			for(int i=0;i<n;i++)
				dv[i]= *(p+i);
			break;
		}
	case DBF_DOUBLE:
	case DBF_3DOUBLE:
	case DBF_4DOUBLE:
	case DBF_5DOUBLE:
	case DBF_6DOUBLE:
	case DBF_7DOUBLE:
	case DBF_8DOUBLE:
	case DBF_9DOUBLE:
	case DBF_16DOUBLE:
		{
			int n = size()/sizeof(double);
			double * p = (double * ) _data;
			dv.resize(n);
			for(int i=0;i<n;i++)
				dv[i]= *(p+i);
			break;
		}
	}
}
void DescribableBuffer::get(int * pc, int ** pdv)//需要用户自己释放*pp
{
	switch (_type)
	{
	case DBF_VOID:
		{
			int n = size()/sizeof(int);
			int * p = (int * ) _data;
			int * dv = new int[n];
			*pdv = dv;
			*pc = n;
			memcpy(dv,_data,size());
			break;
		}
	case DBF_INT8:
		{
			int n = size()/sizeType(_type);
			char * p = (char * ) _data;
			int * dv = new int[n];
			*pdv = dv;
			*pc = n;
			for(int i=0;i<n;i++)
				dv[i]= *(p+i);
			break;
		}
	case DBF_UINT8:
		{
			int n = size()/sizeType(_type);
			unsigned char * p = (unsigned char * ) _data;
			int * dv = new int[n];
			*pdv = dv;
			*pc = n;
			for(int i=0;i<n;i++)
				dv[i]= *(p+i);
			break;
		}
	case DBF_INT16:
		{
			int n = size()/sizeType(_type);
			short * p = (short * ) _data;
			int * dv = new int[n];
			*pdv = dv;
			*pc = n;
			for(int i=0;i<n;i++)
				dv[i]= *(p+i);
			break;
		}
	case DBF_UINT16:
		{
			int n = size()/sizeType(_type);
			unsigned short * p = (unsigned short  * ) _data;
			int * dv = new int[n];
			*pdv = dv;
			*pc = n;
			for(int i=0;i<n;i++)
				dv[i]= *(p+i);
			break;
		}
	case DBF_INT32:
		{
			int n = size()/sizeType(_type);
			int * p = (int * ) _data;
			int * dv = new int[n];
			*pdv = dv;
			*pc = n;
			for(int i=0;i<n;i++)
				dv[i]= *(p+i);
			break;
		}
	case DBF_UINT32:
		{
			int n = size()/sizeType(_type);
			unsigned int  * p = (unsigned int * ) _data;
			int * dv = new int[n];
			*pdv = dv;
			*pc = n;
			for(int i=0;i<n;i++)
				dv[i]= *(p+i);
			break;
		}
	case DBF_INT64:
		{
			int n = size()/sizeType(_type);
			__int64 * p = (__int64 * ) _data;
			int * dv = new int[n];
			*pdv = dv;
			*pc = n;
			for(int i=0;i<n;i++)
				dv[i]= *(p+i);
			break;
		}
	case DBF_UINT64:
		{
			int n = size()/sizeType(_type);
			__int64 * p = (__int64 * ) _data;
			int * dv = new int[n];
			*pdv = dv;
			*pc = n;
			for(int i=0;i<n;i++)
				dv[i]= *(p+i);
			break;
		}
	case DBF_FLOAT:
	case DBF_3FLOAT:
	case DBF_4FLOAT:
	case DBF_5FLOAT:
	case DBF_6FLOAT:
	case DBF_7FLOAT:
	case DBF_8FLOAT:
	case DBF_9FLOAT:
	case DBF_16FLOAT:
		{
			int n = size()/sizeof(float);
			float * p =(float*) _data;
			int * dv = new int[n];
			*pdv = dv;
			*pc = n;
			for(int i=0;i<n;i++)
				dv[i]= *(p+i);
			break;
		}
	case DBF_DOUBLE:
	case DBF_3DOUBLE:
	case DBF_4DOUBLE:
	case DBF_5DOUBLE:
	case DBF_6DOUBLE:
	case DBF_7DOUBLE:
	case DBF_8DOUBLE:
	case DBF_9DOUBLE:
	case DBF_16DOUBLE:
		{
			int n = size()/sizeof(double);
			double * p = (double*) _data;
			int * dv = new int[n];
			*pdv = dv;
			*pc = n;
			for(int i=0;i<n;i++)
				dv[i]= *(p+i);
			break;
		}
	}

}
end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
