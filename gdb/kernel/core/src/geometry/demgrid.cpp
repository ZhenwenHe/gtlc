#include "config.h"
#include "terrain.h"
#include "factory.h"
#include "internal_demgridimpl.h"


begin_gtl_namespace
begin_gdb_namespace

double DEMGrid::getDefaultInvalidValue()
{
	return -9999.0;
}

DEMGridSharedPtr DEMGrid::create(){
#if(USING_3DGEOMETRY_TYPE==1)
	return  DEMGridSharedPtr(new DEMGridImpl());
#else
	return ((Geometry3dFactory*)Factory::getFactory(Factory::FACTORYTYPE_3D_GEOMETRY))->newDEMGrid();
#endif
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
void DEMGrid::fillDEMGrid(DEMGrid::DEM & _dem, DEMGridSharedPtr & grid){

	grid->setOrigin(_dem._Xo, _dem._Yo);
	grid->setHeightRange(_dem._MinV, _dem._MaxV);
	grid->setAlpha(_dem._Alpha);
	grid->setBelta(0);
	grid->setCols(_dem._Col);
	grid->setRows(_dem._Row);
	//设置坐标系表示
	grid->setCoordinateSystem(1);
	//设置坐标单位
	grid->setUnit(1);
	grid->setCellSize(max(_dem._DX, _dem._DY));
	if (_dem._Nodata != std::numeric_limits<double>::max())//DBL_MAX
		grid->setInvalidValue(_dem._Nodata);
	else
		grid->setInvalidValue(DEMGrid::getDefaultInvalidValue());
	grid->setScale(_dem._HZoom);
	size_t s = _dem._Row*_dem._Col;
	float * data = new float[s];
	std::copy(_dem._Data, _dem._Data + s, data);
	grid->setHeightData(data, true);
}

void DEMGrid::initial(DEMGrid::DEM & _dem){
	_dem._Alpha = 0.0;
	_dem._Compress = 0.0;
	_dem._DataMark = "GTL-DEM";
	_dem._DX = 1;
	_dem._DY = 1;
	_dem._HZoom = 0;
	_dem._MaxV = - std::numeric_limits<double>::max();//-DBL_MAX;
	_dem._MinV = std::numeric_limits<double>::max();//DBL_MAX;
	_dem._Nodata = std::numeric_limits<double>::max();//DBL_MAX;
	_dem._Unit = "M";
	_dem._ValueType = "Double";
	_dem._Version = "1.0";
	_dem._Xo = 0;
	_dem._Yo = 0;

	_dem._Data = 0;
	_dem._Row = 0;
	_dem._Col = 0;
}
void DEMGrid::initial(DEMGrid::DEM & _dem, long rows, long cols){

	_dem._Alpha = 0.0;
	_dem._Compress = 0.0;
	_dem._DataMark = "GTL-DEM";
	_dem._DX = 1;
	_dem._DY = 1;
	_dem._HZoom = 0;
	_dem._MaxV = -std::numeric_limits<double>::max();//-DBL_MAX;
	_dem._MinV = std::numeric_limits<double>::max();//DBL_MAX;
	_dem._Nodata = std::numeric_limits<double>::max();//DBL_MAX;
	_dem._Unit = "M";
	_dem._ValueType = "Double";
	_dem._Version = "1.0";
	_dem._Xo = 0;
	_dem._Yo = 0;

	_dem._Data = new double[rows*cols];
	_dem._Row = rows;
	_dem._Col = cols;
}
void DEMGrid::copy(DEMGrid::DEM & _dem, const DEMGrid::DEM & _d){
	_dem._Alpha = _d._Alpha;
	_dem._Compress = _d._Compress;
	_dem._DataMark = _d._DataMark;
	_dem._DX = _d._DX;
	_dem._DY = _d._DY;
	_dem._HZoom = _d._HZoom;
	_dem._MaxV = _d._MaxV;
	_dem._MinV = _d._MinV;
	_dem._Nodata = _d._Nodata;
	_dem._Unit = _d._Unit;
	_dem._ValueType = _d._ValueType;
	_dem._Version = _d._Version;
	_dem._Xo = _d._Xo;
	_dem._Yo = _d._Yo;
	_dem._Row = _d._Row;
	_dem._Col = _d._Col;
	_dem._Data = new double[_d._Row*_d._Col];
	memcpy(_dem._Data, _d._Data, sizeof(double)*_d._Row*_d._Col);
}
void DEMGrid::getXYZ(const DEMGrid::DEM & _dem, long r, long c, double & x, double & y, double & z){
	if (_dem._HZoom == 0 || _dem._HZoom == 1){
		x = _dem._Xo + c*_dem._DX;
		y = _dem._Yo + r*_dem._DY;
		z = _dem._Data[r*_dem._Col + c];
	}
	else{
		x = _dem._Xo + c*_dem._DX;
		y = _dem._Yo + r*_dem._DY;
		z = _dem._Data[r*_dem._Col + c];

		x /= _dem._HZoom;
		y /= _dem._HZoom;
		z /= _dem._HZoom;
	}
}
void DEMGrid::getXYZ(const DEMGrid::DEM & _dem, long r, long c, double *xyz){
	if (_dem._HZoom == 0 || _dem._HZoom == 1){
		xyz[0] = _dem._Xo + c*_dem._DX;
		xyz[1] = _dem._Yo + r*_dem._DY;
		xyz[2] = _dem._Data[r*_dem._Col + c];
	}
	else{
		xyz[0] = _dem._Xo + c*_dem._DX;
		xyz[1] = _dem._Yo + r*_dem._DY;
		xyz[2] = _dem._Data[r*_dem._Col + c];
		xyz[0] /= _dem._HZoom;
		xyz[1] /= _dem._HZoom;
		xyz[2] /= _dem._HZoom;
	}
}
void DEMGrid::getXYZ(const DEMGrid::DEM & _dem, double *xyz){
	size_t k = 0;
	if (_dem._HZoom == 0 || _dem._HZoom == 1){
		for (int i = 0; i < _dem._Row; i++){
			for (int j = 0; j < _dem._Col; j++){
				xyz[k++] = _dem._Xo + j*_dem._DX;
				xyz[k++] = _dem._Yo + i*_dem._DY;
				xyz[k++] = _dem._Data[i*_dem._Col + j];
			}
		}
	}
	else{
		for (int i = 0; i < _dem._Row; i++){
			for (int j = 0; j < _dem._Col; j++){
				xyz[k++] = (_dem._Xo + j*_dem._DX) / _dem._HZoom;
				xyz[k++] = (_dem._Yo + i*_dem._DY) / _dem._HZoom;
				xyz[k++] = (_dem._Data[i*_dem._Col + j]) / _dem._HZoom;
			}
		}
	}
}
void DEMGrid::uninitial(DEMGrid::DEM & _dem){
	if (_dem._Data)
		delete[] _dem._Data;
	_dem._Data = 0;
}
/*
NSDTF-DEM格式说明：
DataMark--------中国地球空间数据交换格式-格网数据交换格式(CNSDTF-RAS或CNSDTF-DEM)的标志。基本部分，不可缺省。
Version--------该空间数据交换格式的版本号,如1.0。基本部分，不可缺省。
Alpha--------方向角。基本部分，不可缺省。
Unit--------坐标单位,K表示公里,M表示米,D表示以度为单位的经纬度,S表示以度分秒表示的经纬度(此时坐标格式为DDDMMSS.SSSS, DDD为度, MM为分, SS.SSSS为秒)。基本部分，不可缺省。
Compress--------压缩方法。0表示不压缩,1表示游程编码。基本部分，不可缺省。
Xo--------左上角原点X坐标。基本部分，不可缺省。
Yo--------左上角原点Y坐标。基本部分，不可缺省。
DX--------X方向的间距。基本部分，不可缺省。
DY--------Y方向的间距。基本部分，不可缺省。
Row--------行数。基本部分，不可缺省。
Col--------列数。基本部分，不可缺省。
HZoom--------高程放大倍率。基本部分，不可缺省。设置高程的放大倍率,使高程数据可以整数存贮,如高程精度精确到厘米,高程的放大倍率为100。如果不是DEM则HZoom为1。
MinV--------可选部分，最小值。
MaxV--------可选部分，最大值。
*/
bool DEMGrid::readCNSDTF(std::string filename, DEMGrid::DEM & _dem){
	std::locale::global(std::locale(""));

	std::ifstream _demfile;		 //DEM文件流
	_demfile.open(filename.c_str()); //读取文件
	std::string strtmp;

	if (_demfile){
		initial(_dem);
		_demfile >> strtmp >> _dem._DataMark;
		_demfile >> strtmp >> _dem._Version;
		_demfile >> strtmp >> _dem._Alpha;
		_demfile >> strtmp >> _dem._Unit;
		_demfile >> strtmp >> _dem._Compress;
		_demfile >> strtmp >> _dem._Xo;
		_demfile >> strtmp >> _dem._Yo;
		_demfile >> strtmp >> _dem._DX;
		_demfile >> strtmp >> _dem._DY;
		_demfile >> strtmp >> _dem._Row;
		_demfile >> strtmp >> _dem._Col;
		_demfile >> strtmp >> _dem._ValueType;
		_demfile >> strtmp >> _dem._HZoom;
		_demfile >> strtmp;//>> _dem._MinV;
		_demfile >> strtmp;//>> _dem._MaxV;

		trim(strtmp, ' ');
		if (strtmp.compare("MaxV:") != 0)	{//说明MinV和MaxV写入了相应的值，读取MaxV行的两列字符串
			_demfile >> strtmp;
			_demfile >> strtmp;
		}
	}
	else
		return false;

	_dem._Data = new double[_dem._Row*_dem._Col];
	double _val;
	int i, j;
	for (j = _dem._Row - 1; j >= 0; j--)	{		//由于此文件记录Y数值的方法与其他不同，需要翻转数据
		for (i = 0; i<_dem._Col; i++){

			_demfile >> _val;
			if (_dem._HZoom != 0)
				_val = _val / _dem._HZoom;	//原始的缩放因子

			if (_val != _dem._Nodata){		//不等于 dNodata
				if (_dem._MinV > _val) _dem._MinV = _val;//记录高程上的最小数值
				if (_dem._MaxV < _val) _dem._MaxV = _val;//记录高程上的最大数值
			}
			_dem._Data[j*_dem._Col + i] = _val;
		}
	}
	return true;
}
bool DEMGrid::writeCNSDTF(const DEMGrid::DEM & _dem, std::string filename){
	std::locale::global(std::locale(""));

	std::ofstream _demfile;		 //DEM文件流
	_demfile.open(filename.c_str()); //读取文件
	std::string strtmp;

	if (_demfile){

		_demfile << "DataMark: CNSDTF-DEM" << std::endl;
		_demfile << "Version: 1.0" << std::endl;
		_demfile << "Alpha: " << _dem._Alpha << std::endl;
		_demfile << "Unit: " << _dem._Unit << std::endl;
		_demfile << "Compress: " << _dem._Compress << std::endl;
		_demfile << "X0: " << _dem._Xo << std::endl;
		_demfile << "Y0: " << _dem._Yo << std::endl;
		_demfile << "DX: " << _dem._DX << std::endl;
		_demfile << "DY: " << _dem._DY << std::endl;
		_demfile << "Row: " << _dem._Row << std::endl;
		_demfile << "Column: " << _dem._Col << std::endl;
		_demfile << "ValueType: " << _dem._ValueType << std::endl;
		_demfile << "Hzoom: " << _dem._HZoom << std::endl;
		_demfile << "MinV: " << _dem._MinV << std::endl;
		_demfile << "MaxV: " << _dem._MaxV << std::endl;
	}
	else
		return false;


	int i, j, k = 0;
	for (j = _dem._Row - 1; j >= 0; j--)	{		//由于此文件记录Y数值的方法与其他不同，需要翻转数据
		for (i = 0; i < _dem._Col; i++){
			k = j*_dem._Col + i;
			_demfile << _dem._Data[k] << " ";
		}
		_demfile << std::endl;
	}
	return true;
}
/*ArcGIS中GRID转换文本文件格式说明： 字串5

ncols         319----------------数据列数
nrows         198----------------数据行数
xllcorner     131985----------------数据左下角的X值
yllcorner     19020----------------数据左下角的Y值
cellsize      5----------------数据分辨率（栅格单元的宽高）
NODATA_value -99999----------------无值数据标志*/
bool DEMGrid::readASC(std::string filename, DEMGrid::DEM & _dem){
	std::locale::global(std::locale(""));

	std::ifstream _demfile;		 //DEM文件流
	_demfile.open(filename.c_str()); //读取文件
	std::string strtmp;

	if (_demfile){
		initial(_dem);

		_demfile >> strtmp >> _dem._Col;
		_demfile >> strtmp >> _dem._Row;
		_demfile >> strtmp >> _dem._Xo;
		_demfile >> strtmp >> _dem._Yo;
		_demfile >> strtmp >> _dem._DX;
		_dem._DY = _dem._DX;
		_demfile >> strtmp >> _dem._Nodata;
	}
	else
		return false;

	_dem._Data = new double[_dem._Row*_dem._Col];

	int i, j, k = 0;
	for (j = 0; j < _dem._Row; j++)	{
		for (i = 0; i<_dem._Col; i++){
			_demfile >> _dem._Data[k];

			if (_dem._Data[k] != _dem._Nodata){		//不等于 dNodata
				if (_dem._MinV > _dem._Data[k]) _dem._MinV = _dem._Data[k];//记录高程上的最小数值
				if (_dem._MaxV < _dem._Data[k]) _dem._MaxV = _dem._Data[k];//记录高程上的最大数值
			}
			k++;
		}
	}

	return true;
}
bool DEMGrid::writeASC(const DEMGrid::DEM & _dem, std::string filename){
	std::locale::global(std::locale(""));

	std::ofstream _demfile;		 //DEM文件流
	_demfile.open(filename.c_str()); //读取文件

	if (_demfile){

		_demfile << "ncols      " << _dem._Col << std::endl;
		_demfile << "nrows      " << _dem._Row << std::endl;
		_demfile << "xllcorner      " << _dem._Xo << std::endl;
		_demfile << "yllcorner      " << _dem._Yo << std::endl;
		_demfile << "cellsize      " << _dem._DX << std::endl;
		_demfile << "NODATA_value  " << _dem._Nodata << std::endl;
	}
	else
		return false;


	int i, j, k = 0;
	for (j = 0; j < _dem._Row; j++)	{
		for (i = 0; i < _dem._Col; i++){
			_demfile << _dem._Data[k] << " ";
			k++;
		}
		_demfile << std::endl;
	}

	return true;
}
end_gdb_namespace
end_gtl_namespace

