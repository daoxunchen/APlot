#ifndef AIMG_H
#define AIMG_H

#include <memory>
#include <vector>
#include <Windows.h>

LRESULT plotImgEvents(HWND, UINT, WPARAM, LPARAM);

class plotImg
{
	friend LRESULT plotImgEvents(HWND, UINT, WPARAM, LPARAM);;
public:
	plotImg(int w, int h, const TCHAR *title = TEXT("Img"));
	~plotImg();

	void show();
	void *data() const { return m_fb; }

	static void waitKey();
private:
	static const TCHAR *s_szClassName;
	static bool s_classIsRegistered;
	static bool s_registerClass();
	static void s_processMsg();
	static bool s_exit;

	HWND m_handler = nullptr;
	HDC m_hdc = nullptr;
	HBITMAP m_hb = nullptr;
	HBITMAP m_ob = nullptr;
	int m_w, m_h;
	VOID *m_fb;
};

class Img {
public:
	Img(size_t w, size_t h, size_t c = 3);
	uint8_t* data() { return m_data.data(); }
	void show();
private:
	std::shared_ptr<plotImg> m_win = nullptr;
	int m_width, m_height, m_channel;
	std::vector<uint8_t> m_data;
};

#endif // !AIMG_H