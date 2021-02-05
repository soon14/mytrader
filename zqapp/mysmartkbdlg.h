// aboutdlg.h : interface of the CMySmartKBDlg class
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _H_SMARTKBDLG_H_
#define _H_SMARTKBDLG_H_

#include "mydataview.h"

#include <algorithm>
#include <string>
#include <memory>
#include <thread>         // std::thread
#include <mutex>          // std::mutex
#include <map>

class MySmartKBDlg : public wxDialog
	, public zqdb::SkinMap<MySmartKBDlg, SkinInfo>
{
	typedef wxDialog Base;
	typedef zqdb::SkinMap<MySmartKBDlg, SkinInfo> SkinBase;
private:
	wxWindow* target_ = nullptr;
	wxTextCtrl* ctrl_text_ = nullptr;
	wxDataViewCtrl* ctrl_list_ = nullptr;
    wxObjectDataPtr<SmartKBListModel> ctrl_list_model_;
public:
	MySmartKBDlg();
	virtual ~MySmartKBDlg();

	int FilterEvent(wxEvent& event);
	//
	void OnSkinInfoChanged();
	//
	void ShowFor(wxWindow* target, int key = 0);
	//
protected:
	//
	void DoAction(int key);

#if wxUSE_HOTKEY
	void OnHotkey(wxKeyEvent& event);
#endif // wxUSE_HOTKEY

	void OnSearch(wxCommandEvent& event);
	void OnSearchResult(wxCommandEvent& event);

	void Activate(const wxDataViewItem& item);
	void OnActivated(wxDataViewEvent &event);
	void OnNotify(wxCommandEvent& event);

	void OnActivate(wxActivateEvent& event);
	void OnOK(wxCommandEvent& event);
	void OnExit(wxCommandEvent& event);
	void OnCloseWindow(wxCloseEvent& event);

	void OnShow(wxShowEvent& event);

	wxDECLARE_EVENT_TABLE();
};

#endif//_H_SMARTKBDLG_H_
