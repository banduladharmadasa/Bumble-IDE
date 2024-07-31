#pragma once

#include <wmsdk.h>
#include <mmdeviceapi.h>

HRESULT GetCodecNames(IWMCodecInfo3 *pCodecInfo, REFGUID inType, CMFCPropertyGridProperty *pProp);
