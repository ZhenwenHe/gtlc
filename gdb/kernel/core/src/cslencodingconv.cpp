#include "csl.h" 

begin_gtl_namespace
begin_gdb_namespace
#ifdef GTL_RECODE_ICONV

void GTLClearRecodeIconvWarningFlags() {

}
char *GTLRecodeIconv(const char *, const char *, const char *) {
	return 0;
}
char *GTLRecodeFromWCharIconv(const wchar_t *,
	const char *, const char *) {
	return 0;
}
wchar_t *GTLRecodeToWCharIconv(const char *,
	const char *, const char *) {
	return 0;
}

#endif // GTL_RECODE_ICONV
end_gdb_namespace
end_gtl_namespace

