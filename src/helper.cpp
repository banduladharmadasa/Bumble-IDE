#include "stdafx.h"
#include <sstream>
#include <iomanip>
using namespace std;



CString GetHTMLColorString(COLORREF color) 
{
	CString str;
	str.Format(L"#%02x%02x%02x", GetRValue(color), GetGValue(color), GetBValue(color));
	return str;
}

CString GetRGBColorString(COLORREF color)
{
	CString str;
	str.Format(L"rgb(%d, %d, %d)", GetRValue(color), GetGValue(color), GetBValue(color));
	return str;
}

CString GetHSVColorString(COLORREF color)
{
	double h, s, v;
	h = s = v = 0.0;
	CDrawingManager::RGBtoHSV(color, &h, &s, &v);
	CString str;
	str.Format(L"hsv(%d, %.1f, %.1f)", (int)h,  s*100.0,  v*100.0);
	return str;
}

CString GetHSLColorString(COLORREF color)
{
	double h, s, l;
	h = s = l = 0.0;
	CDrawingManager::RGBtoHSL(color, &h, &s, &l);
	CString str;
	str.Format(L"hsl(%d, %.1f, %.1f)", (int)(h*360.0), s * 100.0, l * 100.0);
	return str;
}

CString GetCMYKColorString(COLORREF color)
{
	CString str;
	double c = (double)GetRValue(color) / 255.0;
	double m = (double)GetGValue(color) / 255.0;
	double y = (double)GetBValue(color) / 255.0;

	c = 1.0f - c;
	y = 1.0f - y;
	m = 1.0f - m;

	double min_cmy = std::fmin(m, std::fmin(c, y));
	c = (c - min_cmy) / (1.0f - min_cmy);
	m = (m - min_cmy) / (1.0f - min_cmy);
	y = (y - min_cmy) / (1.0f - min_cmy);
	double k = min_cmy;
	double cmyk_scale = 100.0;

	str.Format(L"cmyk(%.4f, %.4f, %.4f, %.4f)", c * cmyk_scale, m * cmyk_scale, y * cmyk_scale, k * cmyk_scale);

	return str;
}

