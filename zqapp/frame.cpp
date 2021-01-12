/////////////////////////////////////////////////////////////////////////////
// Name:        mdi.cpp
// Purpose:     MDI sample
// Author:      Julian Smart
// Modified by: 2008-10-31 Vadim Zeitlin: big clean up
// Created:     04/01/98
// Copyright:   (c) 1997 Julian Smart
//              (c) 2008 Vadim Zeitlin
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#include "frame.h"
#include "app.h"
#include "SmartKBDlg.h"
#include "child.h"
#include "zqstr.hpp"

#ifndef wxHAS_IMAGES_IN_RESOURCES
    #include "../sample.xpm"
    #include "chart.xpm"
#endif

#include "bitmaps/new.xpm"
#include "bitmaps/open.xpm"
#include "bitmaps/save.xpm"
#include "bitmaps/copy.xpm"
#include "bitmaps/cut.xpm"
#include "bitmaps/paste.xpm"
#include "bitmaps/print.xpm"
#include "bitmaps/help.xpm"

//
//键盘精灵
//标题栏
//Ribon菜单栏 开始，行情，交易，策略，帮助
//行情|交易|策略 TAB分析画面
//状态栏
//

// replace this 0 with 1 to build the sample using the generic MDI classes (you
// may also need to add src/generic/mdig.cpp to the build)
#if 0
    #include "wx/generic/mdig.h"
    #define wxMDIParentFrame wxGenericMDIParentFrame
    #define wxMDIChildFrame wxGenericMDIChildFrame
    #define wxMDIClientWindow wxGenericMDIClientWindow
#endif

// ---------------------------------------------------------------------------
// event tables
// ---------------------------------------------------------------------------

wxBEGIN_EVENT_TABLE(MyFrame, wxMDIParentFrame)
    EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
    EVT_MENU(wxID_NEW, MyFrame::OnNewWindow)
    EVT_MENU(MDI_FULLSCREEN, MyFrame::OnFullScreen)
    EVT_MENU(wxID_EXIT, MyFrame::OnQuit)

    EVT_MENU(wxID_CLOSE_ALL, MyFrame::OnCloseAll)

    EVT_MENU_OPEN(MyFrame::OnMenuOpen)
    EVT_MENU_HIGHLIGHT(wxID_ABOUT, MyFrame::OnMenuHighlight)
    EVT_MENU_HIGHLIGHT(MDI_REFRESH, MyFrame::OnMenuHighlight)
    EVT_MENU_CLOSE(MyFrame::OnMenuClose)

    EVT_CLOSE(MyFrame::OnClose)
wxEND_EVENT_TABLE()

// ===========================================================================
// implementation
// ===========================================================================

// ---------------------------------------------------------------------------
// MyFrame
// ---------------------------------------------------------------------------

// Define my frame constructor
MyFrame::MyFrame()
       : wxMDIParentFrame(NULL, wxID_ANY, "wxWidgets MDI Sample",
                          wxDefaultPosition, wxSize(800, 600)),
         MenuEventLogger("parent", this)
{
#if wxUSE_NOTEBOOK
	m_type = Type_Notebook;
#elif wxUSE_CHOICEBOOK
	m_type = Type_Choicebook;
#elif wxUSE_LISTBOOK
	m_type = Type_Listbook;
#elif wxUSE_TREEBOOK
	m_type = Type_Treebook;
#elif wxUSE_TOOLBOOK
	m_type = Type_Toolbook;
#elif wxUSE_AUI
	m_type = Type_Aui;
#else
	m_type = Type_Simplebook;
#endif

	m_orient = ID_ORIENT_DEFAULT;
	m_chkShowImages = true;
	m_fixedWidth = false;
	m_multi = false;
	m_noPageTheme = false;
	m_buttonBar = false;
	m_horzLayout = false;

    SetIcon(wxICON(sample));

    // Make a menubar
#if wxUSE_MENUS
    // Associate the menu bar with the frame
    SetMenuBar(CreateMainMenubar());


    // This shows that the standard window menu may be customized:
    wxMenu * const windowMenu = GetWindowMenu();
    if ( windowMenu )
    {
        // we can change the labels of standard items (which also means we can
        // set up accelerators for them as they're part of the label)
        windowMenu->SetLabel(wxID_MDI_WINDOW_TILE_HORZ,
                             "&Tile horizontally\tCtrl-Shift-H");
        windowMenu->SetLabel(wxID_MDI_WINDOW_TILE_VERT,
                             "&Tile vertically\tCtrl-Shift-V");

        // we can also change the help string
        windowMenu->SetHelpString(wxID_MDI_WINDOW_CASCADE,
                                  "Arrange windows in cascade");

        // we can remove some items
        windowMenu->Delete(wxID_MDI_WINDOW_ARRANGE_ICONS);

        // and we can add completely custom commands -- but then we must handle
        // them ourselves, see OnCloseAll()
        windowMenu->AppendSeparator();
        windowMenu->Append(wxID_CLOSE_ALL, "&Close all windows\tCtrl-Shift-C",
                           "Close all open windows");

        SetWindowMenu(windowMenu);
    }
#endif // wxUSE_MENUS

#if wxUSE_STATUSBAR
    CreateStatusBar();
#endif // wxUSE_STATUSBAR

	// Create the main window

	// don't clutter the text window with time stamps
	wxLog::DisableTimestamp();

	m_bookCtrl = nullptr;

	SetMinSize(wxSize(800, 600));
	RecreateBook();
    //m_bookCtrl = new wxTextCtrl(this, wxID_ANY, "A log window\n",
    //                              wxDefaultPosition, wxDefaultSize,
    //                              wxTE_MULTILINE | wxTE_READONLY);

#if wxUSE_TOOLBAR
    CreateToolBar(wxNO_BORDER | wxTB_FLAT | wxTB_HORIZONTAL);
    InitToolBar(GetToolBar());
#endif // wxUSE_TOOLBAR

#if wxUSE_ACCEL
    // Accelerators
    wxAcceleratorEntry entries[3];
    entries[0].Set(wxACCEL_CTRL, (int) 'N', wxID_NEW);
    entries[1].Set(wxACCEL_CTRL, (int) 'X', wxID_EXIT);
    entries[2].Set(wxACCEL_CTRL, (int) 'A', wxID_ABOUT);
    wxAcceleratorTable accel(3, entries);
    SetAcceleratorTable(accel);
#endif // wxUSE_ACCEL

    // connect it only now, after creating m_bookCtrl
    Bind(wxEVT_SIZE, &MyFrame::OnSize, this);
	Bind(MY_CMD_GOTO_EVENT, &MyFrame::OnGoto, this);
	Bind(wxEVT_DATAVIEW_ITEM_ACTIVATED, &MyFrame::OnActivated, this);
	Bind(ZQDB_CODE_FIELD_EVENT, &MyFrame::OnCodeField, this);
}

MyFrame::~MyFrame()
{
	wxGetApp().ResetTechFrm();

    // and disconnect it to prevent accessing already deleted m_bookCtrl in
    // the size event handler if it's called during destruction
    Unbind(wxEVT_SIZE, &MyFrame::OnSize, this);

    // also prevent its use as log target
    delete wxLog::SetActiveTarget(NULL);
}

#if wxUSE_MENUS
/* static */
wxMenuBar *MyFrame::CreateMainMenubar()
{
    wxMenu *menuFile = new wxMenu;

    menuFile->Append(wxID_NEW, "&New window\tCtrl-N", "Create a new child window");
    menuFile->AppendCheckItem(MDI_FULLSCREEN, "Show &full screen\tCtrl-F");
    menuFile->Append(wxID_EXIT, "&Exit\tAlt-X", "Quit the program");

    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT, "&About\tF1");

    wxMenuBar *mbar = new wxMenuBar;
    mbar->Append(menuFile, "&File");
    mbar->Append(menuHelp, "&Help");

    return mbar;
}
#endif // wxUSE_MENUS

// DISPATCH_ON_TYPE() macro is an ugly way to write the "same" code for
// different wxBookCtrlBase-derived classes without duplicating code and
// without using templates, it expands into "before <xxx> after" where "xxx"
// part is control class-specific
#if wxUSE_NOTEBOOK
#define CASE_NOTEBOOK(x) case Type_Notebook: x; break;
#else
#define CASE_NOTEBOOK(x)
#endif

#if wxUSE_LISTBOOK
#define CASE_LISTBOOK(x) case Type_Listbook: x; break;
#else
#define CASE_LISTBOOK(x)
#endif

#if wxUSE_CHOICEBOOK
#define CASE_CHOICEBOOK(x) case Type_Choicebook: x; break;
#else
#define CASE_CHOICEBOOK(x)
#endif

#if wxUSE_TREEBOOK
#define CASE_TREEBOOK(x) case Type_Treebook: x; break;
#else
#define CASE_TREEBOOK(x)
#endif

#if wxUSE_TOOLBOOK
#define CASE_TOOLBOOK(x) case Type_Toolbook: x; break;
#else
#define CASE_TOOLBOOK(x)
#endif

#if wxUSE_AUI
#define CASE_AUINOTEBOOK(x) case Type_AuiNotebook: x; break;
#else
#define CASE_AUINOTEBOOK(x)
#endif

#define CASE_SIMPLEBOOK(x) case Type_Simplebook: x; break;

#define DISPATCH_ON_TYPE(before, nb, lb, cb, tb, toolb, aui, sb, after)       \
    switch ( m_type )                                                         \
    {                                                                         \
        CASE_NOTEBOOK(before nb after)                                        \
        CASE_LISTBOOK(before lb after)                                        \
        CASE_CHOICEBOOK(before cb after)                                      \
        CASE_TREEBOOK(before tb after)                                        \
        CASE_TOOLBOOK(before toolb after)                                     \
        CASE_AUINOTEBOOK(before aui after)                                    \
        CASE_SIMPLEBOOK(before sb after)                                      \
                                                                              \
        default:                                                              \
            wxFAIL_MSG( "unknown book control type" );                   \
    }

void MyFrame::RecreateBook()
{
	int flags;
	switch (m_orient)
	{
	case ID_ORIENT_TOP:
		flags = wxBK_TOP;
		break;

	case ID_ORIENT_BOTTOM:
		flags = wxBK_BOTTOM;
		break;

	case ID_ORIENT_LEFT:
		flags = wxBK_LEFT;
		break;

	case ID_ORIENT_RIGHT:
		flags = wxBK_RIGHT;
		break;

	default:
		flags = wxBK_DEFAULT;
	}

#if wxUSE_NOTEBOOK
	if (m_fixedWidth && m_type == Type_Notebook)
		flags |= wxNB_FIXEDWIDTH;
	if (m_multi && m_type == Type_Notebook)
		flags |= wxNB_MULTILINE;
	if (m_noPageTheme && m_type == Type_Notebook)
		flags |= wxNB_NOPAGETHEME;
#endif
#if wxUSE_TOOLBOOK
	if (m_buttonBar && m_type == Type_Toolbook)
		flags |= wxTBK_BUTTONBAR;
	if (m_horzLayout && m_type == Type_Toolbook)
		flags |= wxTBK_HORZ_LAYOUT;
#endif

	wxBookCtrlBase *oldBook = m_bookCtrl;

	DISPATCH_ON_TYPE(m_bookCtrl = new,
		wxNotebook,
		wxListbook,
		wxChoicebook,
		wxTreebook,
		wxToolbook,
		wxAuiNotebook,
		wxSimplebook,
		(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, flags));

	if (!m_bookCtrl)
		return;

	m_bookCtrl->Hide();

	// wxToolbook doesn't work without icons so always use them for it.
	/*if (m_chkShowImages || m_type == Type_Toolbook)
	{
		m_bookCtrl->SetImageList(m_imageList);
	}*/

	if (oldBook)
	{
#if wxUSE_TREEBOOK
		// we only need the old treebook if we're recreating another treebook
		wxTreebook *tbkOld = m_type == Type_Treebook
			? wxDynamicCast(oldBook, wxTreebook)
			: NULL;
#endif // wxUSE_TREEBOOK

		const int count = oldBook->GetPageCount();
		for (int n = 0; n < count; n++)
		{
			const int image = -1;//GetIconIndex(m_bookCtrl);
			const wxString str = oldBook->GetPageText(n);

			wxWindow *page = new wxTextCtrl(m_bookCtrl, wxID_ANY, str,
				wxDefaultPosition, wxDefaultSize,
				wxTE_MULTILINE | wxTE_READONLY);

			// treebook complication: need to account for possible parent page
#if wxUSE_TREEBOOK
			if (tbkOld)
			{
				const int parent = tbkOld->GetPageParent(n);
				if (parent != wxNOT_FOUND)
				{
					wxStaticCast(m_bookCtrl, wxTreebook)->
						InsertSubPage(parent, page, str, false, image);

					// skip adding it again below
					continue;
				}
			}
#endif // wxUSE_TREEBOOK

			m_bookCtrl->AddPage(page, str, false, image);
		}

		const int sel = oldBook->GetSelection();
		if (sel != wxNOT_FOUND)
			m_bookCtrl->SetSelection(sel);

		delete oldBook;
	}
	else // no old book
	{
		m_bookCtrl->AddPage(new wxTextCtrl(m_bookCtrl, wxID_ANY, wxEmptyString,
			wxDefaultPosition, wxDefaultSize,
			wxTE_MULTILINE | wxTE_READONLY), wxT("日志"), false);

		auto listctrl = new wxDataViewCtrl(m_bookCtrl, wxID_ANY);

		code_list_model_ = new MyCodeListModel;
		listctrl->AssociateModel(code_list_model_.get());

		listctrl->AppendTextColumn("Code",
			SmartKBListModel::Col_Code);
		listctrl->AppendTextColumn("Name",
			SmartKBListModel::Col_Name);
		//listctrl->AppendTextColumn("Desc",
		//	SmartKBListModel::Col_Desc);

		code_list_model_->Show();

		m_bookCtrl->AddPage(listctrl, wxT("代码"), true);
		
		m_bookCtrl->AddPage(new wxTextCtrl(m_bookCtrl, wxID_ANY, wxEmptyString,
			wxDefaultPosition, wxDefaultSize,
			wxTE_MULTILINE | wxTE_READONLY), wxT("自选"), false);

		m_bookCtrl->AddPage(new wxTextCtrl(m_bookCtrl, wxID_ANY, wxEmptyString,
			wxDefaultPosition, wxDefaultSize,
			wxTE_MULTILINE | wxTE_READONLY), wxT("持仓"), false);
		m_bookCtrl->AddPage(new wxTextCtrl(m_bookCtrl, wxID_ANY, wxEmptyString,
			wxDefaultPosition, wxDefaultSize,
			wxTE_MULTILINE | wxTE_READONLY), wxT("委托"), false);
		m_bookCtrl->AddPage(new wxTextCtrl(m_bookCtrl, wxID_ANY, wxEmptyString,
			wxDefaultPosition, wxDefaultSize,
			wxTE_MULTILINE | wxTE_READONLY), wxT("成交"), false);
	}

	m_bookCtrl->Show();

	delete wxLog::SetActiveTarget(new wxLogTextCtrl(wxDynamicCast(m_bookCtrl->GetPage(0), wxTextCtrl)));
}

void MyFrame::FindOrCreateTech(HZQDB h)
{
	//this->SetActiveChild();
	for (wxWindowList::const_iterator i = GetChildren().begin();
		i != GetChildren().end();
		++i)
	{
		auto techchild = wxDynamicCast(*i, TechChild);
		if (techchild) {
			if (techchild->IsSame(h)) {
				techchild->Raise();
				return;
			}
		}
	}
	TechChild* techframe = new TechChild(this, "./zqapp/techfrm.json", XUtil::XML_FLAG_JSON_FILE);
	auto info_ptr = std::make_shared<zqdb::TechContainerInfo>(techframe);
	info_ptr->Assign(&h, 1);
	techframe->SetInfo(info_ptr);
	techframe->Show(true);

}

void MyFrame::OnGoto(wxCommandEvent& event)
{
	FindOrCreateTech((HZQDB)event.GetClientData());
	if (!IsVisible()) {
		Show();
	}
	Raise();
}

void MyFrame::OnActivated(wxDataViewEvent &event)
{
	FindOrCreateTech((HZQDB)code_list_model_->GetData(event.GetItem()));
}

void MyFrame::OnCodeField(wxCommandEvent& event)
{
	auto active = this->GetActiveChild();
	if (active) {
		active->GetEventHandler()->ProcessEvent(event);
	}
}

void MyFrame::OnClose(wxCloseEvent& event)
{
	Show(false);
    /*unsigned numChildren = MyChild::GetChildrenCount();
    if ( event.CanVeto() && (numChildren > 1) )
    {
        wxString msg;
        msg.Printf("%d windows still open, close anyhow?", numChildren);
        if ( wxMessageBox(msg, "Please confirm",
                          wxICON_QUESTION | wxYES_NO) != wxYES )
        {
            event.Veto();

            return;
        }
    }

    event.Skip();*/
}

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close();
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event) )
{
    (void)wxMessageBox("wxWidgets 2.0 MDI Demo\n"
                       "Author: Julian Smart (c) 1997\n"
                       "Usage: mdi.exe", "About MDI Demo");
}

void MyFrame::OnNewWindow(wxCommandEvent& WXUNUSED(event) )
{
    // create and show another child frame
    MyChild *subframe = new MyChild(this);
    subframe->Show(true);
	/*TechChild* techframe = new TechChild(this);
	auto info_ptr = std::make_shared<TechContainerInfo>(techframe);
	zqdb::AllCode allcode;
	info_ptr->item_list_.reserve(allcode.size());
	for (auto h : allcode)
	{
		zqdb::Code code(h);
		auto info = code.GetValue();
		SmartKBItem one;
		one.Code = zqdb::utf8_to_unicode(info->Code);
		one.Name = zqdb::utf8_to_unicode(info->Name);
		one.Type = 0;
		one.Data = h;
		info_ptr->item_list_.push_back(one);
	}
	techframe->SetInfo(info_ptr);
	techframe->Show(true);*/
}

void MyFrame::OnFullScreen(wxCommandEvent& event)
{
    ShowFullScreen(event.IsChecked());
}

void MyFrame::OnCloseAll(wxCommandEvent& WXUNUSED(event))
{
    for ( wxWindowList::const_iterator i = GetChildren().begin();
          i != GetChildren().end();
          ++i )
    {
        if ( wxDynamicCast(*i, wxMDIChildFrame) )
            (*i)->Close();
    }
}

void MyFrame::OnSize(wxSizeEvent& event)
{
    int w, h;
    GetClientSize(&w, &h);
	const int wleft = 350;
    m_bookCtrl->SetSize(0, 0, wleft, h);
    GetClientWindow()->SetSize(wleft, 0, w - wleft, h);

    // FIXME: On wxX11, we need the MDI frame to process this
    // event, but on other platforms this should not
    // be done.
#ifdef __WXUNIVERSAL__
    event.Skip();
#else
    wxUnusedVar(event);
#endif
}

#if wxUSE_TOOLBAR
void MyFrame::InitToolBar(wxToolBar* toolBar)
{
    wxBitmap bitmaps[8];

    bitmaps[0] = wxBitmap( new_xpm );
    bitmaps[1] = wxBitmap( open_xpm );
    bitmaps[2] = wxBitmap( save_xpm );
    bitmaps[3] = wxBitmap( copy_xpm );
    bitmaps[4] = wxBitmap( cut_xpm );
    bitmaps[5] = wxBitmap( paste_xpm );
    bitmaps[6] = wxBitmap( print_xpm );
    bitmaps[7] = wxBitmap( help_xpm );

    toolBar->AddTool(wxID_NEW, "New", bitmaps[0], "New file");
    toolBar->AddTool(1, "Open", bitmaps[1], "Open file");
    toolBar->AddTool(2, "Save", bitmaps[2], "Save file");
    toolBar->AddSeparator();
    toolBar->AddTool(3, "Copy", bitmaps[3], "Copy");
    toolBar->AddTool(4, "Cut", bitmaps[4], "Cut");
    toolBar->AddTool(5, "Paste", bitmaps[5], "Paste");
    toolBar->AddSeparator();
    toolBar->AddTool(6, "Print", bitmaps[6], "Print");
    toolBar->AddSeparator();
    toolBar->AddTool(wxID_ABOUT, "About", bitmaps[7], "Help");

    toolBar->Realize();
}
#endif // wxUSE_TOOLBAR
