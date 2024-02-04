#include "stdafx.h"
#include "ComType.h"

#pragma comment(lib, "wmvcore.lib")

HRESULT GetCodecNames(IWMCodecInfo3* pCodecInfo, REFGUID inType, CMFCPropertyGridProperty* pProp)
{

	DWORD   cCodecs = 0;
	WCHAR* pwszCodecName = NULL;
	DWORD   cchCodecName = 0;

	HRESULT hr = pCodecInfo->GetCodecInfoCount(inType, &cCodecs);
	if (SUCCEEDED(hr))
	{
		printf("Count: %d ", cCodecs);
	}
	else
	{
		printf("Could not get the count of codecs. ");
		return hr;
	}

	for (DWORD dwCodecIndex = 0; dwCodecIndex < cCodecs; dwCodecIndex++)
	{
	
	
		hr = pCodecInfo->GetCodecName(inType, dwCodecIndex,
			NULL, &cchCodecName);
		if (FAILED(hr))
		{
			printf("Could not get the size of the codec name. ");
			return hr;
		}

	
		pwszCodecName = new WCHAR[cchCodecName];
		if (pwszCodecName == NULL)
		{
			printf("Could not allocate memory. ");
			return E_OUTOFMEMORY;
		}

		hr = pCodecInfo->GetCodecName(inType, dwCodecIndex,
			pwszCodecName, &cchCodecName);
		if (FAILED(hr))
		{
			delete[] pwszCodecName;
			printf("Could not get the codec name. ");
			return hr;
		}

          
								

		pProp->AddOption(pwszCodecName);



		delete[] pwszCodecName;
		pwszCodecName = NULL;
		cchCodecName = 0;
	}

	return FALSE;
}
