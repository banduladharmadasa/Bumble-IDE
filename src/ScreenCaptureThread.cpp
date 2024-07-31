// ScreenCaptureThread.cpp : implementation file
//

#include "stdafx.h"
#include "ScreenCaptureThread.h"
#include "BumbleEdit.h"
#include "BumbleEditView.h"

#include <mfapi.h>
#include <mfidl.h>
#include <Mfreadwrite.h>
#include <mferror.h>

#pragma comment(lib, "mfreadwrite")
#pragma comment(lib, "mfplat")
#pragma comment(lib, "mfuuid")

using namespace Gdiplus;
// CScreenCaptureThread

IMPLEMENT_DYNCREATE(CScreenCaptureThread, CWinThread)

CScreenCaptureThread::CScreenCaptureThread() : m_bRunning(FALSE),
											   m_rect(0, 0, 640, 480)
{
	m_bAutoDelete = FALSE;

	VIDEO_BIT_RATE = 800000;
	m_videoEncodingFormat = MFVideoFormat_WMV3;
	m_videoInputFormat = MFVideoFormat_RGB32;

	bitmapData = new Gdiplus::BitmapData;
}

CScreenCaptureThread::~CScreenCaptureThread()
{
	delete bitmapData;
	bitmapData = NULL;
}

void CScreenCaptureThread::OnStopMessageReceived(WPARAM wparam, LPARAM lparam)
{
	m_bRunning = FALSE;
}

void CScreenCaptureThread::AddMessage(const CString &str, AppMessageType type)
{
	CString *pStr = new CString(str);
	if (!theApp.PostThreadMessageW(ID_MSG_FROM_SHELL, type, (LPARAM)pStr))
	{
		delete pStr;
	}
}

BOOL CScreenCaptureThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	TRACE(L"Started\n");

	MakeVideo();

	TRACE(L"Done\n");

	return TRUE;
}

int CScreenCaptureThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

void CScreenCaptureThread::Start(const CRect &rc)
{
	m_bRunning = TRUE;
	AddMessage(L"Screen capturing process started\n", AppMessageType_Info);

	if (m_bRunning)
	{

		// CaptureSingleFrame(rc);
	}

	AddMessage(L"Screen capturing process terminated\n", AppMessageType_Info);
	// AfxEndThread(0);

	m_rect = rc;
	MakeVideo();
}

BEGIN_MESSAGE_MAP(CScreenCaptureThread, CWinThread)

ON_THREAD_MESSAGE(ID_UITHREADTOCAPTURE_SENDSTOPMESSAGE, &CScreenCaptureThread::OnStopMessageReceived)

END_MESSAGE_MAP()

template <class T>
void SafeRelease(T **ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = NULL;
	}
}

HRESULT CScreenCaptureThread::InitializeSinkWriter(IMFSinkWriter **ppWriter, DWORD *pStreamIndex)
{
	*ppWriter = NULL;
	*pStreamIndex = NULL;

	IMFSinkWriter *pSinkWriter = NULL;
	IMFMediaType *pMediaTypeOut = NULL;
	IMFMediaType *pMediaTypeIn = NULL;
	DWORD streamIndex;

	HRESULT hr = MFCreateSinkWriterFromURL(L"output.wmv", NULL, NULL, &pSinkWriter);

	// Set the output media type.
	if (SUCCEEDED(hr))
	{
		hr = MFCreateMediaType(&pMediaTypeOut);
	}
	if (SUCCEEDED(hr))
	{
		hr = pMediaTypeOut->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video);
	}
	if (SUCCEEDED(hr))
	{
		hr = pMediaTypeOut->SetGUID(MF_MT_SUBTYPE, m_videoEncodingFormat);
	}
	if (SUCCEEDED(hr))
	{
		hr = pMediaTypeOut->SetUINT32(MF_MT_AVG_BITRATE, VIDEO_BIT_RATE);
	}
	if (SUCCEEDED(hr))
	{
		hr = pMediaTypeOut->SetUINT32(MF_MT_INTERLACE_MODE, MFVideoInterlace_Progressive);
	}
	if (SUCCEEDED(hr))
	{
		hr = MFSetAttributeSize(pMediaTypeOut, MF_MT_FRAME_SIZE, VIDEO_WIDTH, VIDEO_HEIGHT);
	}
	if (SUCCEEDED(hr))
	{
		hr = MFSetAttributeRatio(pMediaTypeOut, MF_MT_FRAME_RATE, VIDEO_FPS, 1);
	}
	if (SUCCEEDED(hr))
	{
		hr = MFSetAttributeRatio(pMediaTypeOut, MF_MT_PIXEL_ASPECT_RATIO, 1, 1);
	}
	if (SUCCEEDED(hr))
	{
		hr = pSinkWriter->AddStream(pMediaTypeOut, &streamIndex);
	}

	// Set the input media type.
	if (SUCCEEDED(hr))
	{
		hr = MFCreateMediaType(&pMediaTypeIn);
	}
	if (SUCCEEDED(hr))
	{
		hr = pMediaTypeIn->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video);
	}
	if (SUCCEEDED(hr))
	{
		hr = pMediaTypeIn->SetGUID(MF_MT_SUBTYPE, m_videoInputFormat);
	}
	if (SUCCEEDED(hr))
	{
		hr = pMediaTypeIn->SetUINT32(MF_MT_INTERLACE_MODE, MFVideoInterlace_Progressive);
	}
	if (SUCCEEDED(hr))
	{
		hr = MFSetAttributeSize(pMediaTypeIn, MF_MT_FRAME_SIZE, VIDEO_WIDTH, VIDEO_HEIGHT);
	}
	if (SUCCEEDED(hr))
	{
		hr = MFSetAttributeRatio(pMediaTypeIn, MF_MT_FRAME_RATE, VIDEO_FPS, 1);
	}
	if (SUCCEEDED(hr))
	{
		hr = MFSetAttributeRatio(pMediaTypeIn, MF_MT_PIXEL_ASPECT_RATIO, 1, 1);
	}
	if (SUCCEEDED(hr))
	{
		hr = pSinkWriter->SetInputMediaType(streamIndex, pMediaTypeIn, NULL);
	}

	// Tell the sink writer to start accepting data.
	if (SUCCEEDED(hr))
	{
		hr = pSinkWriter->BeginWriting();
	}

	// Return the pointer to the caller.
	if (SUCCEEDED(hr))
	{
		*ppWriter = pSinkWriter;
		(*ppWriter)->AddRef();
		*pStreamIndex = streamIndex;
	}

	SafeRelease(&pSinkWriter);
	SafeRelease(&pMediaTypeOut);
	SafeRelease(&pMediaTypeIn);
	return hr;
}

HRESULT CScreenCaptureThread::WriteFrame(IMFSinkWriter *pWriter, DWORD streamIndex, const LONGLONG &rtStart)
{

	int height = GetSystemMetrics(SM_CYSCREEN);
	int width = GetSystemMetrics(SM_CXSCREEN);
	CRect rc(0, 0, width, height);
	Gdiplus::Bitmap bitmap(rc.Width(), rc.Height(), PixelFormat32bppARGB);

	Gdiplus::Graphics graphics(&bitmap);

	CaptureScreen(graphics, rc);

	// Lock the whole bitmap so we can read pixel data easily.
	Gdiplus::Rect rect(0, 0, width, height);
	bitmap.LockBits(&rect, Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, bitmapData);
	auto *pixels = static_cast<unsigned *>(bitmapData->Scan0);
	bitmap.UnlockBits(bitmapData);

	IMFSample *pSample = NULL;
	IMFMediaBuffer *pBuffer = NULL;

	const LONG cbWidth = 4 * VIDEO_WIDTH;
	const DWORD cbBuffer = cbWidth * VIDEO_HEIGHT;

	BYTE *pData = NULL;

	// Create a new memory buffer.
	HRESULT hr = MFCreateMemoryBuffer(cbBuffer, &pBuffer);

	// Lock the buffer and copy the video frame to the buffer.
	if (SUCCEEDED(hr))
	{
		hr = pBuffer->Lock(&pData, NULL, NULL);
	}
	if (SUCCEEDED(hr))
	{
		hr = MFCopyImage(
			pData,
			cbWidth,
			(BYTE *)(DWORD *)pixels,
			cbWidth,
			cbWidth,
			VIDEO_HEIGHT);
	}

	if (pBuffer)
	{
		pBuffer->Unlock();
	}

	if (SUCCEEDED(hr))
	{
		hr = pBuffer->SetCurrentLength(cbBuffer);
	}

	if (SUCCEEDED(hr))
	{
		hr = MFCreateSample(&pSample);
	}
	if (SUCCEEDED(hr))
	{
		hr = pSample->AddBuffer(pBuffer);
	}

	if (SUCCEEDED(hr))
	{
		hr = pSample->SetSampleTime(rtStart);
	}
	if (SUCCEEDED(hr))
	{
		hr = pSample->SetSampleDuration(VIDEO_FRAME_DURATION);
	}

	if (SUCCEEDED(hr))
	{
		hr = pWriter->WriteSample(streamIndex, pSample);
	}

	SafeRelease(&pSample);
	SafeRelease(&pBuffer);
	return hr;
}

#include <memory>
Gdiplus::Status HBitmapToBitmap(HBITMAP source, Gdiplus::PixelFormat pixel_format, Gdiplus::Bitmap **result_out)
{
	BITMAP source_info = {0};
	if (!::GetObject(source, sizeof(source_info), &source_info))
		return Gdiplus::GenericError;

	Gdiplus::Status s;

	std::unique_ptr<Gdiplus::Bitmap> target(new Gdiplus::Bitmap(source_info.bmWidth, source_info.bmHeight, pixel_format));
	if (!target.get())
		return Gdiplus::OutOfMemory;
	if ((s = target->GetLastStatus()) != Gdiplus::Ok)
		return s;

	Gdiplus::BitmapData target_info;
	Gdiplus::Rect rect(0, 0, source_info.bmWidth, source_info.bmHeight);

	s = target->LockBits(&rect, Gdiplus::ImageLockModeWrite, pixel_format, &target_info);
	if (s != Gdiplus::Ok)
		return s;

	if (target_info.Stride != source_info.bmWidthBytes)
		return Gdiplus::InvalidParameter; // pixel_format is wrong!

	CopyMemory(target_info.Scan0, source_info.bmBits, source_info.bmWidthBytes * source_info.bmHeight);

	s = target->UnlockBits(&target_info);
	if (s != Gdiplus::Ok)
		return s;

	*result_out = target.release();

	return Gdiplus::Ok;
}

void CScreenCaptureThread::CaptureScreen(Gdiplus::Graphics &graphics, const CRect &rc)
{
	HDC desktopDC = ::GetDC(0);
	HDC memdc = 0;
	HBITMAP hBitmap;

	if (!desktopDC)
	{
		return;
	}

	memdc = CreateCompatibleDC(desktopDC);
	hBitmap = CreateCompatibleBitmap(desktopDC, rc.Width(), rc.Height());
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(memdc, hBitmap);
	BitBlt(memdc, 0, 0, rc.Width(), rc.Height(), desktopDC, rc.left, rc.top, SRCCOPY);

	Gdiplus::Bitmap _bitmap(hBitmap, NULL);

	graphics.DrawImage(&_bitmap, Rect(0, 0, rc.Width(), rc.Height()));

	SelectObject(memdc, hOldBitmap);
	DeleteObject(memdc);
	DeleteObject(hBitmap);
	ReleaseDC(0, desktopDC);
}

void CScreenCaptureThread::MakeVideo()
{
	AddMessage(L"Capture process startted.", AppMessageType_Info);

	m_bRunning = TRUE;

	int height = GetSystemMetrics(SM_CYSCREEN);
	int width = GetSystemMetrics(SM_CXSCREEN);
	CRect rc(0, 0, width, height);
	Gdiplus::Bitmap bitmap(rc.Width(), rc.Height(), PixelFormat32bppARGB);

	Gdiplus::Graphics graphics(&bitmap);

	CaptureScreen(graphics, rc);

	VIDEO_WIDTH = width;
	VIDEO_HEIGHT = height;
	VIDEO_FPS = 20;
	VIDEO_FRAME_DURATION = 50 * 1000 * 1000 / VIDEO_FPS;

	HRESULT hr = MFStartup(MF_VERSION);
	if (SUCCEEDED(hr))
	{
		IMFSinkWriter *pSinkWriter = NULL;
		DWORD stream;

		hr = InitializeSinkWriter(&pSinkWriter, &stream);
		if (SUCCEEDED(hr))
		{
			// Send frames to the sink writer.
			LONGLONG rtStart = 0;

			while (m_bRunning)
			{
				MSG msg;

				if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
				{
					if ((msg.message == WM_CHAR))
					{
					}
					else if ((msg.message == WM_COMMAND) && (msg.wParam == ID_ACTION_STOP))
					{
						m_bRunning = FALSE;
						AddMessage(L"Capture Process is stopping..\n", AppMessageType_Info);
					}
				}

				Sleep(50);

				hr = WriteFrame(pSinkWriter, stream, rtStart);
				if (FAILED(hr))
				{
					break;
				}
				rtStart += VIDEO_FRAME_DURATION;
			}
		}
		if (SUCCEEDED(hr))
		{
			hr = pSinkWriter->Finalize();
		}
		SafeRelease(&pSinkWriter);
		MFShutdown();
	}

	ExitInstance();
}