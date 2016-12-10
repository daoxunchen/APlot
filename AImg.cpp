#include "AImg.h"

#include <cassert>

using namespace std;

LRESULT plotImgEvents(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	case WM_CLOSE: CloseWindow(hWnd); break;
	default: return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

const TCHAR * plotImg::s_szClassName = TEXT("APLOT_IMG");
bool plotImg::s_classIsRegistered = false;
bool plotImg::s_exit = false;

bool plotImg::s_registerClass()
{
	WNDCLASS wc = { CS_BYTEALIGNCLIENT, reinterpret_cast<WNDPROC>(plotImgEvents),
		0,0,nullptr,nullptr,nullptr,nullptr,nullptr,s_szClassName };
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.hInstance = GetModuleHandle(nullptr);
	wc.hCursor = LoadCursor(nullptr, IDC_CROSS);
	s_classIsRegistered = RegisterClass(&wc) != 0;
	return s_classIsRegistered;
}

void plotImg::s_processMsg()
{
	MSG msg;
	for (;;) {
		if (!PeekMessage(&msg, nullptr, 0, 0, PM_NOREMOVE))break;
		if (!GetMessage(&msg, nullptr, 0, 0)) break;
		if (msg.message == WM_KEYDOWN && msg.wParam == VK_ESCAPE) {
			s_exit = true;
			break;
		}
		DispatchMessage(&msg);
	}
}

void plotImg::waitKey()
{
	while (!s_exit) {
		s_processMsg();
	}
}

plotImg::plotImg(int w, int h, const TCHAR *title)
{
	if (!s_classIsRegistered && !s_registerClass())	// register WinClass
		throw runtime_error("registerClass Error!\n");
	m_handler = CreateWindow(s_szClassName, title,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		0, 0, 0, 0, nullptr, nullptr, nullptr, nullptr);

	HDC hdc = GetDC(m_handler);
	m_hdc = CreateCompatibleDC(hdc);
	ReleaseDC(m_handler, hdc);

	BITMAPINFO bi = { { sizeof(BITMAPINFOHEADER),w,-h,1,24,BI_RGB,
		w * h * 3,0,0,0,0 } };

	m_hb = CreateDIBSection(m_hdc, &bi, DIB_RGB_COLORS, &m_fb, nullptr, 0);
	m_ob = (HBITMAP)SelectObject(m_hdc, m_hb);
	m_h = h; m_w = w;
	RECT rect = { 0,0,w,h };
	AdjustWindowRect(&rect, GetWindowLong(m_handler, GWL_STYLE), false);

	int wx, wy, sx, sy;
	wx = rect.right - rect.left;
	wy = rect.bottom - rect.top;
	sx = (GetSystemMetrics(SM_CXSCREEN) - wx) / 2;
	sy = (GetSystemMetrics(SM_CYSCREEN) - wy) / 2;
	sy *= sy < 0 ? 0 : 1;
	SetWindowPos(m_handler, nullptr, sx, sy, wx, wy,
		SWP_NOCOPYBITS | SWP_NOZORDER | SWP_SHOWWINDOW);
	SetForegroundWindow(m_handler);
	ShowWindow(m_handler, SW_NORMAL);
}

plotImg::~plotImg()
{
	if (m_hdc) {
		if (m_ob)SelectObject(m_hdc, m_ob);
		DeleteDC(m_hdc);
	}
	if (m_hb)DeleteObject(m_hb);
	if (m_handler)DestroyWindow(m_handler);
}

void plotImg::show()
{
	HDC hdc = GetDC(m_handler);
	BitBlt(hdc, 0, 0, m_w, m_h, m_hdc, 0, 0, SRCCOPY);
	ReleaseDC(m_handler, hdc);
	s_processMsg();
}

Img::Img(size_t w, size_t h, size_t c) :m_width(w), m_height(h), m_channel(c)
{
	assert(w * h * c);
	m_data.resize(w * h * c);
}

void Img::show()
{
	if (!m_win) m_win = make_shared<plotImg>(m_width, m_height);
	switch (m_channel) {
	case 1: {
		auto p = reinterpret_cast<uint8_t *>(m_win->data());
		for (auto it = m_data.begin(); it != m_data.end(); it++) {
			*p++ = *it; *p++ = *it; *p++ = *it;
		}}
			break;
	case 3:
		copy(m_data.begin(), m_data.end(),
			reinterpret_cast<uint8_t *>(m_win->data()));
		break;
	default:
		break;
	}
	m_win->show();
}