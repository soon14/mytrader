#pragma once

#include "base.h"
#include "settings.h"
#include "smartkb.h"

// this is typically in a header: it just declares MY_EVENT event type
wxDECLARE_EVENT(MY_EVENT, wxCommandEvent);
wxDECLARE_EVENT(ZQDB_CODE_FIELD_EVENT, wxCommandEvent);

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

class MyApp : public wxApp, public SettingsMap<MyApp>
{
protected:
	wxTaskBarIcon *taskbaricon_ = nullptr; //系统托盘
#if defined(__WXOSX__) && wxOSX_USE_COCOA
	MyTaskBarIcon *dockicon_ = nullptr;
#endif
	wxDialog *smartkbdlg_ = nullptr; //键盘精灵
	wxFrame *techfrm_ = nullptr; //技术框架，面向技术派的UI框架，技术版（ToB/C都会带上）
	//wxFrame *stdfrm_ = nullptr; //标准框架，标准客户UI框架，标准版（ToC）
	//wxFrame *profrm_ = nullptr; //标准框架，专业客户UI框架，专业版（ToB）
protected:
	void DeleteBitmaps();
	bool LoadImages();
	void LoadSkinInfo();

public:
    bool OnInit() wxOVERRIDE;
    int OnExit() wxOVERRIDE;

	void Exit();

	inline wxDialog* GetSmartKBDlg() { return smartkbdlg_; }
	inline void ResetSmartKBDlg() { smartkbdlg_ = nullptr; }

	inline wxFrame* GetTechFrm() { return techfrm_; }
	inline void ResetTechFrm() { techfrm_ = nullptr; }

	void OnCodeField(wxCommandEvent& event);

	void OnNotify(HMDB hdb, HMTABLE htb, MDB_NOTIFY_DATA* notify);
};

wxDECLARE_APP(MyApp);
