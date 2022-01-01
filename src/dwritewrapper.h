#ifndef DWRITEWRAPPER_H
#define DWRITEWRAPPER_H


#include <dwrite.h>

#ifdef __cplusplus
extern "C"
{
#endif


/**
 * @brief Creates DirectWrite render factory
 * 
 * @param type Value that specifies whether the factory object will be shared or isolated
 * @param factory Address of a pointer to factory object
 * @return HRESULT S_OK on success, otherwise error code
 */
HRESULT dwDWriteCreateFactory(enum DWRITE_FACTORY_TYPE type, IUnknown ** factory);


#ifdef __cplusplus
}
#endif

#endif
