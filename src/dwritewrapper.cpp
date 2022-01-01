#include "dwritewrapper.h"

extern "C"
{

HRESULT dwDWriteCreateFactory(enum DWRITE_FACTORY_TYPE type, IUnknown ** factory)
{
	return ::DWriteCreateFactory(type, __uuidof(IDWriteFactory), factory);
}

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
)
{
	return static_cast<IDWriteFactory *>(factory)->CreateTextFormat(
		fontFamilyName,
		static_cast<IDWriteFontCollection *>(fontCollection),
		fontWeight,
		fontStyle,
		fontStretch,
		fontSize,
		localeName,
		reinterpret_cast<IDWriteTextFormat **>(textFormat)
	);
}

HRESULT dwTFmtSetTextAlignment(
	IUnknown * txtFmt,
	enum DWRITE_TEXT_ALIGNMENT txtAlignment
)
{
	return static_cast<IDWriteTextFormat *>(txtFmt)->SetTextAlignment(txtAlignment);
}



}
