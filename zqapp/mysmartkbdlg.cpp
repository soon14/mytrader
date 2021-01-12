#include "mysmartkbdlg.h"
#include "myapp.h"
#include "myframe.h"
#include "wx/sysopt.h"
#include "wx/display.h"


// ----------------------------------------------------------------------------
// MySmartKBDlg implementation
// ----------------------------------------------------------------------------

wxBEGIN_EVENT_TABLE(MySmartKBDlg, Base)
	EVT_TEXT(wxID_EDIT, MySmartKBDlg::OnSearch)
	EVT_ACTIVATE(MySmartKBDlg::OnActivate)
	EVT_BUTTON(wxID_OK, MySmartKBDlg::OnOK)
	EVT_BUTTON(wxID_EXIT, MySmartKBDlg::OnExit)
	EVT_CLOSE(MySmartKBDlg::OnCloseWindow)
	EVT_SHOW(MySmartKBDlg::OnShow)
wxEND_EVENT_TABLE()


MySmartKBDlg::MySmartKBDlg()
	:Base(nullptr, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0)
{
	wxSizer * const sizerTop = new wxBoxSizer(wxVERTICAL);

	wxSizerFlags flags;
	//flags.Border(wxALL, 10);

	ctrl_text_ = new wxTextCtrl(this, wxID_EDIT, wxEmptyString, wxDefaultPosition, wxDefaultSize
		, wxNO_BORDER | wxTE_PROCESS_ENTER| wxTE_PROCESS_TAB);
	sizerTop->Add(ctrl_text_, flags.Expand());

	sizerTop->AddSpacer(2);

	ctrl_list_ = new wxDataViewCtrl(this, wxID_ANY, wxDefaultPosition,
		wxSize(200,200), wxNO_BORDER | wxDV_NO_HEADER);
	ctrl_list_->ShowScrollbars(wxSHOW_SB_NEVER, wxSHOW_SB_NEVER);

	ctrl_list_model_ = new SmartKBListModel;
	ctrl_list_->AssociateModel(ctrl_list_model_.get());

	//auto *code_render = new SmartKBListRenderer();
	//wxDataViewColumn *code_col =
	//	new wxDataViewColumn("Code", code_render, SmartKBListModel::Col_Code);
	//ctrl_list_->AppendColumn(code_col);
	ctrl_list_->AppendTextColumn("Code",
		SmartKBListModel::Col_Code);

	//auto *name_render = new SmartKBListRenderer();
	//wxDataViewColumn *name_col =
	//	new wxDataViewColumn("Name", name_render, SmartKBListModel::Col_Name);
	//ctrl_list_->AppendColumn(name_col);
	ctrl_list_->AppendTextColumn("Name",
		SmartKBListModel::Col_Name);
	//ctrl_list_->AppendTextColumn("Desc",
	//	SmartKBListModel::Col_Desc);

	sizerTop->Add(ctrl_list_, flags);

	SetSizerAndFit(sizerTop);

	//Centre();

#if wxUSE_HOTKEY
	if (RegisterHotKey(0, wxMOD_ALT, 'V'))
	{
		//m_logText->AppendText("Try pressing Alt-Shift-Home anywhere now.\n");
	}
	else
	{
		//m_logText->AppendText("Failed to register hot key.\n");
	}
	Bind(wxEVT_HOTKEY, &MySmartKBDlg::OnHotkey, this);
#endif // wxUSE_HOTKEY

	Bind(SMARTKB_SEARCH_RESULT_EVENT, &MySmartKBDlg::OnSearchResult, this);
	Bind(wxEVT_DATAVIEW_ITEM_ACTIVATED, &MySmartKBDlg::OnActivated, this);
}

MySmartKBDlg::~MySmartKBDlg()
{
#if wxUSE_HOTKEY
	if (!UnregisterHotKey(0))
	{
		//m_logText->AppendText("Failed to unregister hot key.\n");
	}
#endif // wxUSE_HOTKEY

	wxGetApp().ResetSmartKBDlg();
}

int MySmartKBDlg::FilterEvent(wxEvent& event)
{
	const wxEventType t = event.GetEventType();
	auto obj = event.GetEventObject();
	if (obj) {
		if (t == wxEVT_KEY_DOWN) {
			auto key_event = wxDynamicCast(&event, wxKeyEvent);
			auto key_code = key_event->GetKeyCode();
			switch (key_code)
			{
			case WXK_ESCAPE: {
				Show(false);
			} break;
			default: {
				if (obj->IsKindOf(wxCLASSINFO(wxTextCtrl))) {
					switch (key_code)
					{
					case WXK_UP:
					case WXK_DOWN:
					case WXK_RETURN: {
						DoAction(key_code);
					} break;
					default:
						break;
					}
				}
			} break;
			}
		}
	}
	// Continue processing the event normally as well.
	return wxEventFilter::Event_Skip;
}

void MySmartKBDlg::OnSkinInfoChanged()
{
	//普通显示信息变化
	SetBackgroundColour(skin_info_ptr_->crPrimary);
	ctrl_text_->SetBackgroundColour(skin_info_ptr_->crCtrlBkgnd);
	ctrl_text_->SetForegroundColour(skin_info_ptr_->crCtrlForgnd);
	//
	ctrl_list_->SetBackgroundColour(skin_info_ptr_->crCtrlBkgnd);
	//ctrl_list_->SetAlternateRowColour(skin_info_ptr_->crXYLine);
	ctrl_list_->SetForegroundColour(skin_info_ptr_->crCtrlForgnd);
}

void MySmartKBDlg::ShowFor(wxWindow* target, int key)
{
	target_ = target;
	if (key) {
		ctrl_text_->SetLabel(wxString::Format(wxT("%c"), key));
	}
	else {
		ctrl_text_->SetSelection(-1, -1);
	}
	ctrl_text_->SetFocus();
	//ctrl_list_model_->SetMaxResult(5);
	wxRect rcWnd = GetRect();
	if (target_) {
		wxRect rcDisplay(target_->GetRect());
		SetPosition(rcWnd.CenterIn(rcDisplay).GetLeftTop());
	}
	else {
		wxDisplay display(wxDisplay::GetFromWindow(target_ ? target_ : this));
		wxRect rcDisplay(display.GetClientArea());
		rcDisplay.Offset(-rcWnd.width, -rcWnd.height);
		SetPosition(rcDisplay.GetRightBottom());
	}
	//SetPosition(wxGetMousePosition());
	Show(!IsVisible());
	if (IsVisible()) {
		Raise();
	}

	//GetCaret()
	//wxGetMousePosition();
	//WarpPointer()
}

void MySmartKBDlg::DoAction(int key)
{
	switch (key)
	{
	case WXK_UP: {
		auto sel = ctrl_list_->GetSelection(); 
		if (sel) {
			auto sel_row = ctrl_list_model_->GetRow(sel);
			if (sel_row <= 0) {
				return;
			}
			auto item = ctrl_list_model_->GetItem(sel_row - 1);
			ctrl_list_->Select(item);
			ctrl_list_->EnsureVisible(item);
		}
		else {
			if (ctrl_list_model_->GetCount()) {
				ctrl_list_->Select(ctrl_list_model_->GetItem(0));
			}
		}
	} break;
	case WXK_DOWN: {
		auto sel = ctrl_list_->GetSelection();
		if (sel) {
			auto sel_row = ctrl_list_model_->GetRow(sel);
			if (sel_row >= (ctrl_list_model_->GetCount() - 1)) {
				return;
			}
			auto item = ctrl_list_model_->GetItem(sel_row + 1);
			ctrl_list_->Select(item);
			ctrl_list_->EnsureVisible(item);
		}
		else {
			if (ctrl_list_model_->GetCount()) {
				ctrl_list_->Select(ctrl_list_model_->GetItem(0));
			}
		}
	} break;
	case WXK_RETURN: {
		auto sel = ctrl_list_->GetSelection();
		Activate(sel);
	} break;
	default:
		break;
	}
}

#if wxUSE_HOTKEY
void MySmartKBDlg::OnHotkey(wxKeyEvent& event)
{
	ShowFor(nullptr);
}
#endif // wxUSE_HOTKEY

void MySmartKBDlg::OnSearch(wxCommandEvent& event)
{
	ctrl_list_model_->Search(this, event.GetString().Trim().MakeUpper());
}

void MySmartKBDlg::OnSearchResult(wxCommandEvent& event)
{
	ctrl_list_model_->ShowResult();
	if (ctrl_list_model_->GetCount()) {
		ctrl_list_->Select(ctrl_list_model_->GetItem(0));
	}
	//ctrl_list_->AdjustScrollbars();
}

void MySmartKBDlg::Activate(const wxDataViewItem& item)
{
	Show(false);
	SmartKBItem smkbi;
	if (ctrl_list_model_->GetResult(item, smkbi)) {
		wxCommandEvent goto_event(MY_CMD_GOTO_EVENT);
		goto_event.SetInt(CMD_TYPE_ZQDB);
		goto_event.SetString(smkbi.Code);
		goto_event.SetClientData(smkbi.Data);
		if (target_) {
			target_->GetEventHandler()->AddPendingEvent(goto_event);
		}
		else {
			wxGetApp().AddPendingEvent(goto_event);
		}
	}
}

void MySmartKBDlg::OnActivated(wxDataViewEvent &event)
{
	Activate(event.GetItem());
}

void MySmartKBDlg::OnActivate(wxActivateEvent &event)
{
	if (event.GetActive()) {
		//
	} else {
		Show(false);
	}
}

void MySmartKBDlg::OnOK(wxCommandEvent& WXUNUSED(event))
{
	Show(false);
}

void MySmartKBDlg::OnExit(wxCommandEvent& WXUNUSED(event))
{
	Close(true);
}

void MySmartKBDlg::OnCloseWindow(wxCloseEvent& WXUNUSED(event))
{
	//Destroy();
	Show(false);
}

void MySmartKBDlg::OnShow(wxShowEvent& event)
{
	if (event.IsShown()) {

	}
	else {
		//ctrl_text_->SetLabel(wxEmptyString);
	}
}


