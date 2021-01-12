// aboutdlg.h : interface of the CSmartKBDlg class
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _H_SMARTKBDLG_H_
#define _H_SMARTKBDLG_H_

#include "app.h"
#include <algorithm>
#include <string>
#include <memory>
#include <thread>         // std::thread
#include <mutex>          // std::mutex
#include <map>

#include "dataview.h"

class SmartKBDlg : public wxDialog
{
private:
	wxTextCtrl* ctrl_text_;
    wxObjectDataPtr<SmartKBListModel> m_list_model;
public:
	SmartKBDlg();
	virtual ~SmartKBDlg();

protected:
#if wxUSE_HOTKEY
	void OnHotkey(wxKeyEvent& event);
#endif // wxUSE_HOTKEY

	void OnSearch(wxCommandEvent& event);
	void OnSearchResult(wxCommandEvent& event);

	void OnActivated(wxDataViewEvent &event);

	void OnOK(wxCommandEvent& event);
	void OnExit(wxCommandEvent& event);
	void OnCloseWindow(wxCloseEvent& event);

	wxDECLARE_EVENT_TABLE();
};

#endif//_H_SMARTKBDLG_H_
