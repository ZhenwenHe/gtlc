#include "csl.h"
begin_gtl_namespace
begin_gdb_namespace
StringList::StringList() {
	_data = 0;
}
StringList::StringList(const StringList & sl) {
	_data = 0;
	cslDuplicate(sl._data);
}
StringList::~StringList() {
	if (_data)
		cslDestroy(_data);
	_data = 0;
}
StringList StringList::operator=(const StringList & sl) {
	_data = 0;
	cslDuplicate(sl._data);
	return *this;
}
const char * StringList::operator[](size_t i) {
	return (const char *)_data[i];
}
end_gdb_namespace
end_gtl_namespace