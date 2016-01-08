#include "image.h"
#include "factory.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

ImageSharedPtr Image::create(){
	return ((ImageFactory*)Factory::getFactory(Factory::FACTORYTYPE_IMAGE))->newImage();
}
ImageSharedPtr Image::create(unsigned w, unsigned h, unsigned bpp, void * pdata){
	return ((ImageFactory*)Factory::getFactory(Factory::FACTORYTYPE_IMAGE))->newImage(w, h, bpp,pdata);
}
/**创建一个新图像,，并拷贝p中的数据
	* @param [in] p        源图像
	* @return 图像共享指针
	*/
ImageSharedPtr Image::create(const ImageSharedPtr & p){
	return p->copy(0,0,p->getWidth(),p->getHeight());
}
end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace