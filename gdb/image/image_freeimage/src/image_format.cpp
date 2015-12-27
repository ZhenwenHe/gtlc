#include "imageimpl.h"
 

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace
bool ImageImpl::write(int imageFormat, Buffer & fout, int flags)
{
	if(imageFormat==IMAGE_FORMAT_DDS)
	{
		return writeDDSBuffer(fout);
	}

	FREE_IMAGE_FORMAT fif =(FREE_IMAGE_FORMAT) imageFormat;
	BOOL bSuccess = FALSE;
	if(fif != FIF_UNKNOWN ) {
		// Check that the dib can be saved in this format
		BOOL bCanSave;

		FREE_IMAGE_TYPE image_type = FreeImage_GetImageType(_dib);
		if(image_type == FIT_BITMAP) {
			// standard bitmap type
			WORD bpp = FreeImage_GetBPP(_dib);
			bCanSave = (FreeImage_FIFSupportsWriting(fif) && FreeImage_FIFSupportsExportBPP(fif, bpp));
		} else {
			// special bitmap type
			bCanSave = FreeImage_FIFSupportsExportType(fif, image_type);
		}
		if(bCanSave) {
			FIMEMORY * hmem = FreeImage_OpenMemory(0,0);
			bSuccess=FreeImage_SaveToMemory(fif, _dib, hmem, flags);
			BYTE * p=0;
			DWORD size_in_bytes=0;
			FreeImage_AcquireMemory(hmem,&p,&size_in_bytes);
			fout.write((void*)p,size_in_bytes);
			p=0;
			FreeImage_CloseMemory(hmem);
			return bSuccess==TRUE;
		}
	}
	return bSuccess==TRUE;
}

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace

