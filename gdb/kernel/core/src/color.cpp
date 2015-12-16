#include "color.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

void Color::convert(long c, Color4b & c4b){
	c4b.a = (c>>24)&0xff;
	c4b.b = (c>>16)&0xff;
	c4b.g = (c>>8)&0xff;
	c4b.r = c&0xff;
}
void Color::convert(const Color4b &  c4b, Color4f & c4f){
	c4f.r = c4b.r/255.0f;
	c4f.g = c4b.g/255.0f;
	c4f.b = c4b.b/255.0f;
	c4f.a = c4b.a/255.0f;
}
void Color::convert(long c, Color4f & c4f){
	Color4b  c4b;
	convert(c,c4b);
	convert(c4b,c4f);
}
void Color::convert(long c, Color3b & c3b){
	c3b.b = (c>>16)&0xff;
	c3b.g = (c>>8)&0xff;
	c3b.r = c&0xff;
}
void Color::convert(const Color4f &  c4f, Color4b & c4b){
	c4b.r =(unsigned char) c4f.r * 255;
	c4b.g =(unsigned char) c4f.g * 255;
	c4b.b =(unsigned char) c4f.b * 255;
	c4b.a =(unsigned char) c4f.a * 255;
}
void Color::convert(const Color4f &  c4f, long & c){
	Color4b   c4b;
	convert(c4f,c4b);
	c=RGB(c4b.r,c4b.g,c4b.b);
}

Color4f Color::create(float r, float g, float b, float a){
	Color4f c4f ;
	c4f.r=r;
	c4f.g=g;
	c4f.b=b;
	c4f.a=a;
	return c4f;
}
Color3f Color::create(float r, float g, float b ){
	Color3f c3f ;
	c3f.r=r;
	c3f.g=g;
	c3f.b=b;
	return c3f;
}
end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
