#pragma once

class IMFSinkWriter;

enum class ScreenCaptureType
{
	Desktop,
	Application,
	Document
};

// CScreenCaptureThread

class CScreenCaptureThread : public CWinThread
{
	DECLARE_DYNCREATE(CScreenCaptureThread)

protected:
	CScreenCaptureThread();           // protected constructor used by dynamic creation
	virtual ~CScreenCaptureThread();

	void OnStopMessageReceived(WPARAM wparam, LPARAM lparam);

	void AddMessage(const CString& str, AppMessageType type);



public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	void Start(const CRect &rc);

	HRESULT InitializeSinkWriter(IMFSinkWriter** ppWriter, DWORD* pStreamIndex);

	HRESULT WriteFrame(IMFSinkWriter* pWriter, DWORD streamIndex, const LONGLONG& rtStart);

	

	void CaptureScreen(Gdiplus::Graphics &graphics, const CRect &rc);



	void MakeVideo();

protected:
	DECLARE_MESSAGE_MAP()
	
	
	CRect m_mainWndRect;
	CRect m_viewClientRect;
	BOOL m_bRunning;

	CRect m_rect;

	// Format constants
	UINT32 VIDEO_WIDTH;// = 640;
	UINT32 VIDEO_HEIGHT;// = 480;
	UINT32 VIDEO_FPS;// = 30;
	UINT64 VIDEO_FRAME_DURATION;// = 10 * 1000 * 1000 / VIDEO_FPS;
	UINT32 VIDEO_BIT_RATE;// = 800000;
	GUID   m_videoEncodingFormat;// = MFVideoFormat_WMV3;
	GUID   m_videoInputFormat;// = MFVideoFormat_RGB32;


	
	Gdiplus::BitmapData* bitmapData;

};


