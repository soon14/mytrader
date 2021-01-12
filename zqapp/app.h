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
	wxTaskBarIcon *taskbaricon_ = nullptr; //ϵͳ����
#if defined(__WXOSX__) && wxOSX_USE_COCOA
	MyTaskBarIcon *dockicon_ = nullptr;
#endif
	wxDialog *smartkbdlg_ = nullptr; //���̾���
	wxFrame *techfrm_ = nullptr; //������ܣ��������ɵ�UI��ܣ������棨ToB/C������ϣ�
	//wxFrame *stdfrm_ = nullptr; //��׼��ܣ���׼�ͻ�UI��ܣ���׼�棨ToC��
	//wxFrame *profrm_ = nullptr; //��׼��ܣ�רҵ�ͻ�UI��ܣ�רҵ�棨ToB��
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
