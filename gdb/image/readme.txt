这里采用的是freeImage317的静态库。
编译freeimage出现错误：Severity	Code	Description	Project	File	Line	Suppression State
Error	C1189	#error:  Macro definition of snprintf conflicts with Standard Library function declaration	
LibTIFF4	C:\Program Files (x86)\Windows Kits\10\Include\10.0.10240.0\ucrt\stdio.h	1927	

这个错误主要是由于VS2015采用了C99标准

找到LibTIFF4工程中的tif_config.h中的下列代码：
#ifdef _WIN32

#define snprintf _snprintf
#define lfind _lfind

#endif // _WIN32
替换为：
#ifdef _WIN32

#if _MSC_VER<1900
#define snprintf _snprintf
#endif
#define lfind _lfind

#endif // _WIN32