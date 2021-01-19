#pragma once

#include "base.h"
#include "settings.h"
#include "smartkb.h"

// this is typically in a header: it just declares MY_EVENT event type
wxDECLARE_EVENT(MY_EVENT, wxCommandEvent);
wxDECLARE_EVENT(ZQDB_NOTIFY_ENABLE_EVENT, wxCommandEvent);
wxDECLARE_EVENT(ZQDB_NOTIFY_DISABLE_EVENT, wxCommandEvent);
wxDECLARE_EVENT(ZQDB_NOTIFY_APPEND_EVENT, wxCommandEvent);
wxDECLARE_EVENT(ZQDB_NOTIFY_REMOVE_EVENT, wxCommandEvent);
wxDECLARE_EVENT(ZQDB_NOTIFY_UPDATE_EVENT, wxCommandEvent);

class MySmartKBDlg;
class MyFrame;

// ----------------------------------------------------------------------------
// global variables
// ----------------------------------------------------------------------------

extern wxBitmap *gs_bmpNoMask,
*gs_bmpWithColMask,
*gs_bmpMask,
*gs_bmpWithMask,
*gs_bmp4,
*gs_bmp4_mono,
*gs_bmp36;

// -- application --

class MyEventFilter : public wxEventFilter
{
public:
	MyEventFilter();
	~MyEventFilter();
	virtual int FilterEvent(wxEvent& event);
private:
	wxDECLARE_NO_COPY_CLASS(MyEventFilter);
};

class MyApp : public wxApp
	, MyEventFilter
	, public SettingsMap<MyApp>
	, public zqdb::INotifyT<MyApp>
{
	typedef zqdb::INotifyT<MyApp> NotifyBase;
protected:
	wxTaskBarIcon *taskbaricon_ = nullptr; //系统托盘
#if defined(__WXOSX__) && wxOSX_USE_COCOA
	MyTaskBarIcon *dockicon_ = nullptr;
#endif
	MySmartKBDlg *smartkbdlg_ = nullptr; //键盘精灵
	std::vector<MyFrame *> frames_; //视图列表
protected:
	void DeleteBitmaps();
	bool LoadImages();
	void LoadSkinInfo();

public:
    bool OnInit() wxOVERRIDE;
    int OnExit() wxOVERRIDE;

	void Exit();

	inline MySmartKBDlg* GetSmartKBDlg() { return smartkbdlg_; }
	inline void ResetSmartKBDlg() { smartkbdlg_ = nullptr; }

	void ResetFrame(MyFrame * frame);

	void Goto(HZQDB h);
	void OnGoto(wxCommandEvent& event);
	void OnNotify(wxCommandEvent& event);

	void OnNotifyEnable(HZQDB h);
	void OnNotifyDisable(HZQDB h);
	void OnNotifyAppend(HZQDB h);
	void OnNotifyRemove(HZQDB h);
	void OnNotifyUpdate(HZQDB h);
	void OnNotify(HMDB hdb, HMTABLE htb, MDB_NOTIFY_DATA* notify);
};

wxDECLARE_APP(MyApp);
