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
 * @param factory Address of a pointer to factory object down-casted to a double-pointer of IUnknown
 * @return HRESULT S_OK on success, otherwise error code
 */
HRESULT dwDWriteCreateFactory(enum DWRITE_FACTORY_TYPE type, IUnknown ** factory);

/**
 * @brief Creates a text format object used for text layout
 * 
 * @param factory Pointer of DWrite factory object down-casted to IUnknown pointer
 * @param fontFamilyName Array of characters that contains the name of the font family
 * @param fontCollection A pointer to a font collection object down-casted to a pointer
 * of IUnknown. When NULL, indicates system font collection
 * @param fontWeight Font weight
 * @param fontStyle Font style
 * @param fontStretch Font stretch
 * @param fontSize Font size in logical DIPs. 1 DIP = 1/96 inch
 * @param localeName An array of characters that contains the locale name
 * @param textFormat Address of a pointer to DWrite text format object down-casted
 * to a double-pointer of IUnknown
 * @return HRESULT S_OK on success, otherwise error code
 */
HRESULT dwFactoryCreateTextFormat(
	IUnknown * factory,
	const WCHAR * fontFamilyName,
	IUnknown * fontCollection,
	enum DWRITE_FONT_WEIGHT fontWeight,
	enum DWRITE_FONT_STYLE fontStyle,
	enum DWRITE_FONT_STRETCH fontStretch,
	FLOAT fontSize,
	const WCHAR * localeName,
	IUnknown ** textFormat
);

/**
 * @brief 
 * 
 * @param txtFmt Pointer of IDWriteTextFormat down-casted to IUnknown pointer
 * @param txtAlignment value that specifies text alignment
 * @return HRESULT S_OK on success, otherwise error code
 */
HRESULT dwTFmtSetTextAlignment(
	IUnknown * txtFmt,
	enum DWRITE_TEXT_ALIGNMENT txtAlignment
);


#ifdef __cplusplus
}
#endif

#endif
