// BuiltInServer.cpp : implementation file
//

#include "stdafx.h"

#define NO_CACHING
#include "BumbleEdit.h"
#include "BuiltInServer.h"
#include "CivetServer.h"

#define MAX_OPTIONS (50)
// #define DOCUMENT_ROOT "."
// #define PORT "8085"
// #define CGI_INTERPRETER "C:\\wamp/bin/php/php7.3.1/php-cgi.exe"

struct tuser_data
{
	char *first_message;
};

static struct mg_context *g_ctx;	  /* Set by start_civetweb() */
static struct tuser_data g_user_data; /* Passed to mg_start() by start_civetweb() */

// CBuiltInServer

CBuiltInServer::CBuiltInServer()
{
	if (m_documentRoot.IsEmpty())
	{
		TCHAR my_documents[MAX_PATH];
		HRESULT result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, my_documents);

		if (result == S_OK)
		{

			m_documentRoot = my_documents;
			m_documentRoot.Append(L"\\www");

			// create www folder if not exists
			if (CreateDirectory(m_documentRoot, NULL) ||
				ERROR_ALREADY_EXISTS == GetLastError())
			{
				// CopyFile(...)
			}
			else
			{
				// Failed to create directory.
			}
		}
	}
}

CBuiltInServer::~CBuiltInServer()
{
}

#if defined(__cplusplus) && (__cplusplus >= 201103L)
#define NO_RETURN [[noreturn]]
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)
#define NO_RETURN _Noreturn
#elif defined(__GNUC__)
#define NO_RETURN __attribute((noreturn))
#else
#define NO_RETURN
#endif

#define USE_STACK_SIZE 102400

/* Use same defines as in civetweb.c before including system headers. */
#if !defined(_LARGEFILE_SOURCE)
#define _LARGEFILE_SOURCE /* For fseeko(), ftello() */
#endif
#if !defined(_FILE_OFFSET_BITS)
#define _FILE_OFFSET_BITS 64 /* Use 64-bit file offsets by default */
#endif
#if !defined(__STDC_FORMAT_MACROS)
#define __STDC_FORMAT_MACROS /* <inttypes.h> wants this for C++ */
#endif
#if !defined(__STDC_LIMIT_MACROS)
#define __STDC_LIMIT_MACROS /* C++ wants that for INT64_MAX */
#endif

std::string vformat(const char *fmt, va_list ap)
{
	// Allocate a buffer on the stack that's big enough for us almost
	// all the time.  Be prepared to allocate dynamically if it doesn't fit.
	size_t size = 1024;
	char stackbuf[1024];
	std::vector<char> dynamicbuf;
	char *buf = &stackbuf[0];
	va_list ap_copy;

	while (1)
	{
		// Try to vsnprintf into our buffer.
		va_copy(ap_copy, ap);
		int needed = vsnprintf(buf, size, fmt, ap);
		va_end(ap_copy);

		// NB. C99 (which modern Linux and OS X follow) says vsnprintf
		// failure returns the length it would have needed.  But older
		// glibc and current Windows return -1 for failure, i.e., not
		// telling us how much was needed.

		if (needed <= (int)size && needed >= 0)
		{
			// It fit fine so we're done.
			return std::string(buf, (size_t)needed);
		}

		// vsnprintf reported that it wanted to write more characters
		// than we allotted.  So try again using a dynamic buffer.  This
		// doesn't happen very often if we chose our initial size well.
		size = (needed > 0) ? (needed + 1) : (size * 2);
		dynamicbuf.resize(size);
		buf = &dynamicbuf[0];
	}
}

std::string StdErrToStdString(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	std::string buf = vformat(fmt, ap);
	va_end(ap);

	CString str(buf.c_str());

	theApp.AddMessage(str + L"\n", AppMessageType_Error);

	return buf;
}

static NO_RETURN void die(const char *fmt, ...)
{
	va_list ap;
	char msg[512] = "";

	va_start(ap, fmt);
	(void)vsnprintf(msg, sizeof(msg) - 1, fmt, ap);
	msg[sizeof(msg) - 1] = 0;
	va_end(ap);

#if defined(_WIN32)
	MessageBoxA(NULL, msg, "Error", MB_OK);
#else
	StdErrToStdString("%s\n", msg);
#endif

	exit(EXIT_FAILURE);
}

static char *sdup(const char *str)
{
	size_t len;
	char *p;

	len = strlen(str) + 1;
	p = (char *)malloc(len);

	if (p == NULL)
	{
		die("Cannot allocate %u bytes", (unsigned)len);
	}

	memcpy(p, str, len);
	return p;
}

static int log_message(const struct mg_connection *conn, const char *message)
{
	const struct mg_context *ctx = mg_get_context(conn);
	struct tuser_data *ud = (struct tuser_data *)mg_get_user_data(ctx);

	StdErrToStdString("%s\n", message);

	if (ud->first_message == NULL)
	{
		ud->first_message = sdup(message);
	}

	return 0;
}

int CBuiltInServer::ExampleHandler(struct mg_connection *conn, void *cbdata)
{
	// mg_printf(conn, "Cache-Control: no-cache; Cache-Control: no-store;");
	return 0;
}

BOOL CBuiltInServer::Start(const CString &path)
{
	CConfig *config = theApp.GetConfig();

	CString configPath = config->GetCgiBinPath();

	if (configPath.Trim().IsEmpty())
	{
		BumbleMessageBox(L"Bumble needs a CGI interpreter to show the document on the browser!\nPlease add the CGI interpreter path in Options panel");
		return FALSE;
	}

	m_documentRoot = config->GetRootDirectory();
	if (m_documentRoot.IsEmpty())
	{
		m_documentRoot = path;
	}

	if (!PathIsDirectory(m_documentRoot))
	{

		BumbleMessageBox(L"Root Directory is not a valid path!");
		return FALSE;
	}

	CW2A strCgiInterpreter(configPath);

	if (!PathFileExistsA(strCgiInterpreter))
	{

		BumbleMessageBox(L"Invalid CGI interpreter path!");

		return FALSE;
	}

	if (PathIsDirectoryA(strCgiInterpreter))
	{
		BumbleMessageBox(L"CGI interpreter executable name is required.");
		return FALSE;
	}

	int port = config->GetListeningPort();

	CStringA strPort;
	strPort.Format("%d", port);
	if (strPort.GetLength() != 4)
	{
		BumbleMessageBox(L"Invalid listening port!");
		return FALSE;
	}

	struct mg_callbacks callbacks;

	std::wstring rawPath(m_documentRoot);
	std::wstring uncPath = ConvertToUNC(rawPath);
	const wchar_t *str = uncPath.c_str();
	size_t size = wcslen(str) * 2 + 2;
	char *_lpszDocRoot = new char[size];
	size_t c_size;
	wcstombs_s(&c_size, _lpszDocRoot, size, str, size);

	const char *options[] = {
		"document_root", _lpszDocRoot,
		"listening_ports", strPort,
		"cgi_interpreter", strCgiInterpreter /*CGI_INTERPRETER*/,
		"keep_alive_timeout_ms", "500",
		"enable_keep_alive", "yes",
		"static_file_max_age", "0",
		NULL};

	memset(&callbacks, 0, sizeof(callbacks));
	callbacks.log_message = &log_message;

	g_ctx = mg_start(&callbacks, &g_user_data, (const char **)options);

	if (g_ctx == NULL)
	{

		BumbleMessageBox(L"Unable to start the web server");
	}
	else
	{

		theApp.AddMessage(L"Server started at 127.0.0.1:" + CString(strPort) + L"\n", AppMessageType_Info);
		theApp.AddMessage(L"The root document location is '" + m_documentRoot + L"'\n", AppMessageType_Info);

		/*mg_printf(g_ctx, "ache-Control: no-cache");*/
		mg_set_request_handler(g_ctx, "**.html$", &CBuiltInServer::ExampleHandler, 0);

		theApp.m_bServerRunning = TRUE;
	}

	return TRUE;
}

static void stop_civetweb(void)
{
	mg_stop(g_ctx);
	free(g_user_data.first_message);
	g_user_data.first_message = NULL;
	theApp.AddMessage(L"Server stopped\n", AppMessageType_Info);
	theApp.m_bServerRunning = FALSE;
}

void CBuiltInServer::Stop()
{
	stop_civetweb();
}
