#include "config.h"
#include "terrain.h" 
#include "factory.h"
#include "internal_DOMGridImpl.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

DOMGridSharedPtr DOMGrid::create(string szfiledom){
	char path[MAX_PATH];
	char sz[MAX_PATH];
	string str;
	double dr[4];
	//double west,east,south,north;

	ifstream iFstreamDOM;
	iFstreamDOM.open(szfiledom.c_str(), std::ios_base::in | std::ios_base::_Nocreate);
	//DataMark: 
	iFstreamDOM >> str;
	iFstreamDOM >> str;
	//Version:
	iFstreamDOM >> str;
	iFstreamDOM >> str;
	//Unit:
	iFstreamDOM >> str;
	iFstreamDOM >> str;
	//Type:
	iFstreamDOM >> str;
	iFstreamDOM >> str;
	//SaveWay:
	iFstreamDOM >> str;
	iFstreamDOM >> str;
	//Band:
	iFstreamDOM >> str;
	iFstreamDOM >> str;
	//Row:
	iFstreamDOM >> str;
	iFstreamDOM >> str;
	//Col:
	iFstreamDOM >> str;
	iFstreamDOM >> str;
	//Xr:
	iFstreamDOM >> str;
	iFstreamDOM >> str;
	//Yc:
	iFstreamDOM >> str;
	iFstreamDOM >> str;
	//StoreLocation:
	iFstreamDOM >> str;
	iFstreamDOM >> path;
	//West:
	iFstreamDOM >> str;
	iFstreamDOM.getline(sz, MAX_PATH);
	dr[0] = stringToNumber<double, char>(sz);
	//East:
	iFstreamDOM >> str;
	iFstreamDOM.getline(sz, MAX_PATH);
	dr[1] = stringToNumber<double, char>(sz);
	//South:
	iFstreamDOM >> str;
	iFstreamDOM.getline(sz, MAX_PATH);
	dr[2] = stringToNumber<double, char>(sz);
	//North:
	iFstreamDOM >> str;
	iFstreamDOM.getline(sz, MAX_PATH);
	dr[3] = stringToNumber<double, char>(sz);
	//Width:
	//Height:
	iFstreamDOM.close();

	DOMGridSharedPtr _domgrid = DOMGrid::create();
	ImageSharedPtr image = Image::create();
	image->load(path);
	if (image->isValid()){
		//unsigned bpp_div_8=image->getBitsPerPixel()/8;
		//Buffer dbf(image->getHeight()*image->getWidth()*bpp_div_8);
		//dbf.setPos(0);
		//if(image->getWidth()*bpp_div_8 != image->getScanWidth()){
		//	unsigned char * p = 0;
		//	for(unsigned int i =0;i<image->getHeight();i++){
		//		p=image->getScanLine(i);
		//		dbf.write(p,image->getWidth()*bpp_div_8);
		//	}
		//}
		//else {
		//	dbf.write(0,image->getPixels(),image->getHeight()*image->getWidth()*bpp_div_8);
		//}

		//_domgrid->setParameters(image->getHorizontalResolution(),image->getVerticalResolution(),
		//	dr[1],dr[0],dr[3],dr[2],image->getWidth(),image->getHeight(),1,(unsigned char*)dbf.data(),
		//	dbf.size(),true);

		//////////////////////////////////////////////////////////////
		_domgrid->setParameters(dr[1], dr[0], dr[3], dr[2], image);
	}
	return _domgrid;
}



DOMGridSharedPtr DOMGrid::create(){
#if(USING_DEFAULT_3DGEOMETRY==1)
	return DOMGridSharedPtr(new DOMGridImpl());
#else
	return ((Geometry3dFactory*)Factory::getFactory(Factory::FACTORYTYPE_3D_GEOMETRY))->newDOMGrid();
#endif
}



end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace