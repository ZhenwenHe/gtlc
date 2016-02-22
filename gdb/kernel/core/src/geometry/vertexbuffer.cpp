#include "vertexbuffer.h"



begin_gtl_namespace
begin_gdb_namespace

//Matrix4x4SharedPtr VertexBuffer::_matrixTransform = Matrix4x4SharedPtr((Matrix4x4*)0);
//
//Matrix4x4SharedPtr VertexBuffer::getMatrix(){
//	return VertexBuffer::_matrixTransform;
//}
//void VertexBuffer::transform(const double * pd, float * pf,size_t number_of_vertices,VBF t){
//	
//	size_t offset = sizeType(DescribableBuffer::DBF(t))/sizeof(float);
//
//	if(_matrixTransform)
//	{	
//		size_t s = 3;
//		switch (t)
//		{
//		case VBF_3FLOAT_XYZ:
//		case VBF_6FLOAT_XYZRGB:
//		case VBF_7FLOAT_XYZRGBA:
//		case VBF_5FLOAT_XYZUV:
//		case VBF_9FLOAT_XYZUVRGBA:
//		case VBF_3DOUBLE_XYZ:
//		case VBF_6DOUBLE_XYZRGB:
//		case VBF_7DOUBLE_XYZRGBA:
//		case VBF_5DOUBLE_XYZUV:
//		case VBF_9DOUBLE_XYZUVRGBA:
//			s=3;
//			break;
//		case VBF_4FLOAT_XYZW:
//		case VBF_8FLOAT_XYZWRGBA:
//		case VBF_4DOUBLE_XYZW:
//		case VBF_8DOUBLE_XYZWRGBA:
//		case VBF_10FLOAT_XYZWUVRGBA:
//		case VBF_10DOUBLE_XYZWUVRGBA:
//			s=4;
//			break;
//		default:
//			s=3;
//		}
//
//		for(size_t i=0;i<number_of_vertices;i++){
//			_matrixTransform->transformVertex(pd+i*offset,pf+i*offset,s);
//		}
//
//	}
//	else{		
//		size_t sn = offset * number_of_vertices;
//		for(size_t i=0;i<sn;i++){
//			pf[i]=(float) pd[i];
//		}
//	}	
//}

VertexBuffer::VertexBuffer(VBF t ):DescribableBuffer(){
	_type =(DescribableBuffer::DBF) t;
}
VertexBuffer::VertexBuffer(void * vertices,size_t s,VBF t ,bool b_alloc)//构造函数，如果b_alloc为真，则会新申请一块大小为s的内存块，并复制d的内容，反之，则指向d
:DescribableBuffer(vertices,s,(DescribableBuffer::DBF)t,b_alloc)
{
	
}
void VertexBuffer::set(void * d,size_t s,VBF t ,bool b_alloc)//构造函数，如果b_alloc为真，则会新申请一块大小为s的内存块，并复制d的内容，反之，则指向d
{
	Buffer::set(d,s,b_alloc);
	_type =(DescribableBuffer::DBF) t;
}
void VertexBuffer::copy(const VertexBuffer & b)//拷贝Buffer函数，如果b的_alloc成员为假，则拷贝后的Buffer对象与b公用一个内存块，否则为内存块拷贝
{
	DescribableBuffer::copy((const DescribableBuffer &)b);
}
VertexBuffer::VertexBuffer(const VertexBuffer & b)//拷贝构造函数
{
	copy(b);
}
VertexBuffer VertexBuffer::operator = (const VertexBuffer & b)//等号赋值操作
{
	copy(b);
	return *this;
}

void VertexBuffer::add(double x, double y, double z){
	if(isFloat()){
		float pd[] = { float(x),float(y),float(z)};
		Buffer::write(&pd,sizeof(float)*3);
	}
	else{
		double pd[] = { x,y,z};
		Buffer::write(&pd,sizeof(double)*3);
	}
}
void VertexBuffer::add(double x, double y, double z,double w){
	if(isFloat()){
		float pd[] = { float(x),float(y),float(z),float(w)};
		Buffer::write(&pd,sizeof(float)*4);
	}
	else{
		double pd[] = { x,y,z,w};
		Buffer::write(&pd,sizeof(double)*4);
	}
}
void VertexBuffer::add(double x, double y, double z,float u, float v){
	if(isFloat()){
		float pd[] = { float(x),float(y),float(z),u,v};
		Buffer::write(&pd,sizeof(float)*5);
	}
	else{
		double pd[] = { x,y,z,double(u),double(v)};
		Buffer::write(&pd,sizeof(double)*5);
	}
}
void VertexBuffer::add(double x, double y, double z,float r ,float g ,float b){
	if(isFloat()){
		float pd[] = { float(x),float(y),float(z),r,g,b};
		Buffer::write(&pd,sizeof(float)*6);
	}
	else{
		double pd[] = { x,y,z,double(r),double(g),double(b)};
		Buffer::write(&pd,sizeof(double)*6);
	}
}
void VertexBuffer::add(double x, double y, double z,float r ,float g ,float b,float a){
	if(isFloat()){
		float pd[] = { float(x),float(y),float(z),r,g,b,a};
		Buffer::write(&pd,sizeof(float)*7);
	}
	else{
		double pd[] = { x,y,z,double(r),double(g),double(b),double(a)};
		Buffer::write(&pd,sizeof(double)*7);
	}
}
void VertexBuffer::add(double x, double y, double z,double w,float r ,float g ,float b,float a){
	if(isFloat()){
		float pd[] = { float(x),float(y),float(z),float(w),r,g,b,a};
		Buffer::write(&pd,sizeof(float)*8);
	}
	else{
		double pd[] = { x,y,z,w,double(r),double(g),double(b),double(a)};
		Buffer::write(&pd,sizeof(double)*8);
	}
}

void VertexBuffer::add(double x, double y, double z,float u,float v,float r ,float g ,float b,float a) {
	if(isFloat()){
		float pd[] = { float(x),float(y),float(z),u,v,r,g,b,a};
		Buffer::write(&pd,sizeof(float)*9);
	}
	else{
		double pd[] = { x,y,z,double(u),double(v),double(r),double(g),double(b),double(a)};
		Buffer::write(&pd,sizeof(double)*9);
	}
}
void VertexBuffer::add(double x, double y, double z,double w,float u,float v,float r ,float g ,float b,float a){
	if(isFloat()){
		float pd[] = { float(x),float(y),float(z),float(w),u,v,r,g,b,a};
		Buffer::write(&pd,sizeof(float)*10);
	}
	else{
		double pd[] = { x,y,z,w,double(u),double(v),double(r),double(g),double(b),double(a)};
		Buffer::write(&pd,sizeof(double)*10);
	}
}

inline  bool VertexBuffer::isFloat(){
	if(_type>=VBF_3FLOAT_XYZ && _type <= VBF_10FLOAT_XYZWUVRGBA)
		return true;
	else
		return false;
}
end_gdb_namespace
end_gtl_namespace


