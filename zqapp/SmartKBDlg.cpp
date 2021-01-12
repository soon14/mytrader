#include "SmartKBDlg.h"
#include "app.h"
#include "frame.h"


// ----------------------------------------------------------------------------
// SmartKBDlg implementation
// ----------------------------------------------------------------------------

wxBEGIN_EVENT_TABLE(SmartKBDlg, wxDialog)
	EVT_TEXT(wxID_EDIT, SmartKBDlg::OnSearch)
	EVT_BUTTON(wxID_OK, SmartKBDlg::OnOK)
	EVT_BUTTON(wxID_EXIT, SmartKBDlg::OnExit)
	EVT_CLOSE(SmartKBDlg::OnCloseWindow)
wxEND_EVENT_TABLE()


SmartKBDlg::SmartKBDlg():wxDialog(NULL, wxID_ANY, wxT("SmartKBDlg"))
{
	wxSizer * const sizerTop = new wxBoxSizer(wxVERTICAL);

	wxSizerFlags flags;
	flags.Border(wxALL, 10);

	ctrl_text_ = new wxTextCtrl(this, wxID_EDIT);
	sizerTop->Add(ctrl_text_, flags.Expand());

	auto listctrl = new wxDataViewCtrl(this, wxID_ANY, wxDefaultPosition,
		wxSize(300,600), 0);

	m_list_model = new SmartKBListModel;
	listctrl->AssociateModel(m_list_model.get());

	listctrl->AppendTextColumn("Code",
		SmartKBListModel::Col_Code);
	listctrl->AppendTextColumn("Name",
		SmartKBListModel::Col_Name);
	//listctrl->AppendTextColumn("Desc",
	//	SmartKBListModel::Col_Desc);

	sizerTop->Add(listctrl, flags);

	SetSizerAndFit(sizerTop);
	Centre();

#if wxUSE_HOTKEY
	if (RegisterHotKey(0, wxMOD_ALT, 'V'))
	{
		//m_logText->AppendText("Try pressing Alt-Shift-Home anywhere now.\n");
	}
	else
	{
		//m_logText->AppendText("Failed to register hot key.\n");
	}
	Bind(wxEVT_HOTKEY, &SmartKBDlg::OnHotkey, this);
#endif // wxUSE_HOTKEY

	Bind(SMARTKB_SEARCH_RESULT_EVENT, &SmartKBDlg::OnSearchResult, this);
	Bind(wxEVT_DATAVIEW_ITEM_ACTIVATED, &SmartKBDlg::OnActivated, this);
}

SmartKBDlg::~SmartKBDlg()
{
#if wxUSE_HOTKEY
	if (!UnregisterHotKey(0))
	{
		//m_logText->AppendText("Failed to unregister hot key.\n");
	}
#endif // wxUSE_HOTKEY

	wxGetApp().ResetSmartKBDlg();
}

#if wxUSE_HOTKEY
void SmartKBDlg::OnHotkey(wxKeyEvent& event)
{
	Show(!IsVisible());
	if (IsVisible()) {
		Raise();
	}
}
#endif // wxUSE_HOTKEY

void SmartKBDlg::OnSearch(wxCommandEvent& event)
{
	m_list_model->Search(this, event.GetString().Trim().MakeUpper());
}

void SmartKBDlg::OnSearchResult(wxCommandEvent& event)
{
	m_list_model->ShowResult();
}

void SmartKBDlg::OnActivated(wxDataViewEvent &event)
{
	Show(false);
	SmartKBItem smkbi;
	if (m_list_model->GetResult(event.GetItem(), smkbi)) {
		wxCommandEvent goto_event(MY_CMD_GOTO_EVENT);
		goto_event.SetInt(CMD_TYPE_ZQDB);
		goto_event.SetString(smkbi.Code);
		goto_event.SetClientData(smkbi.Data);
		wxGetApp().GetTechFrm()->GetEventHandler()->AddPendingEvent(goto_event);
	}
}

void SmartKBDlg::OnOK(wxCommandEvent& WXUNUSED(event))
{
	Show(false);
}

void SmartKBDlg::OnExit(wxCommandEvent& WXUNUSED(event))
{
	Close(true);
}

void SmartKBDlg::OnCloseWindow(wxCloseEvent& WXUNUSED(event))
{
	Destroy();
}


