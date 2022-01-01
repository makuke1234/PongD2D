#include "dwritewrapper.h"

extern "C"
{

HRESULT dwDWriteCreateFactory(enum DWRITE_FACTORY_TYPE type, IUnknown ** factory)
{
	return ::DWriteCreateFactory(type, __uuidof(IDWriteFactory), factory);
}


}
