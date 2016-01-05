#include "mesh.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

Cell::Cell(){
	_x=0;
	_y=0;
	_z=0;
	_valid=false;
}
Cell::~Cell(){
}
void Cell::output(ofstream & f){
	int flag = _valid?1:0;
	f<<_x<<" "<<_y<<" "<<_z<<" "<<flag<<std::endl;
	f<< _vertices.size() << std::endl;
	for(size_t i =0;i<_vertices.size();i++)
	{
		f<<_vertices[i].x<<" "<<_vertices[i].y<<" "<<_vertices[i].z<<std::endl;
	}
	f<< _propValues.size() << std::endl;
	for(size_t i =0;i<_propValues.size();i++)
	{
		f<<_propValues[i]<<" ";
	}
	f<<std::endl;
}
void Cell::reset(unsigned long long ix,unsigned long long iy,unsigned long long iz,
	std::vector<double>& p, std::vector<Vertex3d> & v){
	_x=ix;
	_y=iy;
	_z=iz;
	_propValues.resize(p.size());
	std::copy(p.begin(),p.end(),_propValues.begin());
	_vertices.resize(v.size());
	std::copy(v.begin(),v.end(),_vertices.begin());
}
std::vector<double> & Cell::getPropValues(){
	return this->_propValues;
}
std::vector<Vertex3d> & Cell::getVertices(){
	return this->_vertices;
}
void Cell::copy(const Cell & c){
	_x=c._x;
	_y=c._y;
	_z=c._z;
	_valid=c._valid;
	_propValues.resize(c._propValues.size());
	std::copy(c._propValues.begin(),c._propValues.end(),_propValues.begin());
	_vertices.resize(c._vertices.size());
	std::copy(c._vertices.begin(),c._vertices.end(),_vertices.begin());
}
void Cell::clear(){
	_x=0;
	_y=0;
	_z=0;
	_valid=false;
	this->_propValues.clear();
	this->_vertices.clear();
}
Cell Cell::operator = (const Cell & c){
	copy(c);
	return *this;
}
Cell::Cell(const Cell & c)
{
	copy(c);
}
void  Cell::getEnvelope(Envelope3d & e3d) {
	for(size_t i = 0;i < _vertices.size(); i++){
		e3d.extend(_vertices[i]);
	}
}
/** 获取对象写入到Buffer中所占用的字节数，便于从Buffer对象中构建几何对象
     * @param  [in] 无 
     * @return 返回几何对象写入到Buffer中所占用的字节数
	 */
size_t Cell::sizeBuffer() {
	unsigned long long  _propValuesNumber = _propValues.size();
	unsigned long long  _verticesNumber = _vertices.size();
	size_t s = sizeof(unsigned long long) * 5//unsigned long long _x,_y,_z;
		+ _propValuesNumber * sizeof(double)//_propValues
		+ _verticesNumber * sizeof(double)*3//_vertices
		+ sizeof(bool);
	return s;
}
/** 从Buffer中读取信息填充本几何对象
    * @param  [in,out] buf Buffer & ，缓冲区对象引用 
    * @return 成功返回true，否则返回false
*/
bool Cell::readBuffer( Buffer & buf) {
	unsigned char v = 0;
	buf.read((void*)(&v),sizeof(unsigned char));
	_valid = v==0?false:true;

	buf.read((void*)(&_x),sizeof(unsigned long long));
	buf.read((void*)(&_y),sizeof(unsigned long long));
	buf.read((void*)(&_z),sizeof(unsigned long long));


	
	unsigned long long  _verticesNumber =0;
	buf.read((void*)(&_verticesNumber),sizeof(unsigned long long));
	_vertices.resize(_verticesNumber);
	double xyz[3];
	for(size_t i = 0;i <_verticesNumber;i++){
		buf.read((void*)xyz,sizeof(double)*3);
		_vertices[i].x=xyz[0];
		_vertices[i].y=xyz[1];
		_vertices[i].z=xyz[2];
	}

	unsigned long long  _propValuesNumber = 0;
	buf.read((void*)(&_propValuesNumber),sizeof(unsigned long long));
	_propValues.resize(_propValuesNumber);
	for(size_t i = 0;i <_propValuesNumber;i++){
		buf.read((void*)(&(_propValues[i])),sizeof(double));
	}
	return true;
}
/** 将本类的信息写入Buffer中 
    * @param  [in,out] buf Buffer & ，缓冲区对象引用
    * @return 成功返回true，否则返回false
*/
bool Cell::writeBuffer(Buffer& buf) {
	unsigned char v = _valid?1:0;
	buf.write((void*)(&v),sizeof(unsigned char));

	buf.write((void*)(&_x),sizeof(unsigned long long));
	buf.write((void*)(&_y),sizeof(unsigned long long));
	buf.write((void*)(&_z),sizeof(unsigned long long));


	unsigned long long  _verticesNumber =_vertices.size();
	buf.write((void*)(&_verticesNumber),sizeof(unsigned long long));
	double xyz[3];
	for(size_t i = 0;i <_verticesNumber;i++){
		xyz[0]=_vertices[i].x;
		xyz[1]=_vertices[i].y;
		xyz[2]=_vertices[i].z;
		buf.write((void*)xyz,sizeof(double)*3);
	}

	unsigned long long  _propValuesNumber = _propValues.size();
	buf.write((void*)(&_propValuesNumber),sizeof(unsigned long long));
	for(size_t i = 0;i <_propValuesNumber;i++){
		buf.write((void*)(&(_propValues[i])),sizeof(double));
	}
	return true;
	//size_t s = sizeBuffer();
	//size_t len = 0;
	//char *pi = (char *) new unsigned char[s];
	//
	//char *ptr = pi;

	//* (bool *)ptr = _valid;
	//ptr+=sizeof(bool);

	//*(unsigned long long*)ptr = _x;
	//ptr += sizeof(unsigned long long);

	//*(unsigned long long*)ptr = _y;
	//ptr += sizeof(unsigned long long);

	//*(unsigned long long*)ptr = _z;
	//ptr += sizeof(unsigned long long);


	//
	//unsigned long long  _verticesNumber = _vertices.size();
	//*(unsigned long long*)ptr =_verticesNumber;
	//ptr += sizeof(unsigned long long);

	//for(size_t i = 0;i <_verticesNumber;i++){
	//	*(double*)ptr =_vertices[i].x;
	//	ptr += sizeof(double);
	//	*(double*)ptr =_vertices[i].y;
	//	ptr += sizeof(double);
	//	*(double*)ptr =_vertices[i].z;
	//	ptr += sizeof(double);
	//}

	//unsigned long long  _propValuesNumber = _propValues.size();
	//*(unsigned long long*)ptr =_propValuesNumber;
	//ptr += sizeof(unsigned long long);
	//for(size_t i = 0;i <_propValuesNumber;i++){
	//	*(double*)ptr =_propValues[i];
	//	ptr += sizeof(double);
	//}

	//buf.write(pi,s);
	//delete [] pi ;

	return true;
}
/* 将本类的信息写入输出流中，便于文件读写或缓冲区写操作 
    * @param  [in,out] f  std::ostream &，标准输出流对象引用
    * @return 成功返回true，否则返回false
*/
void Cell::write(std::ostream & f){
	Buffer buf;
	writeBuffer(buf);
	f.write((const char*) buf.data(),sizeBuffer());
}
/** 从标准输入流中提取本类的信息  
    * @param  [in,out] f  std::istream &，标准输入流对象引用
    * @return 成功返回true，否则返回false
*/
void Cell::read(std::istream & f){
	size_t s = sizeBuffer();
	char *  p = new char[s];
	Buffer buf(p,s,false);
	f.read(p,s);
	readBuffer(buf);	
}

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
