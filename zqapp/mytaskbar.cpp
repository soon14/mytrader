/////////////////////////////////////////////////////////////////////////////
// Name:        tbtest.cpp
// Purpose:     wxTaskBarIcon demo
// Author:      Julian Smart
// Modified by:
// Created:     01/02/97
// Copyright:   (c)
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#include "mytaskbar.h"
#include "myapp.h"
#include "myframe.h"

// ============================================================================
// implementation
// ============================================================================
//
//MyDialog::~MyDialog()
//{
//    delete m_taskBarIcon;
//}
//
//void MyDialog::OnAbout(wxCommandEvent& WXUNUSED(event))
//{
//    static const char * const title = "About wxWidgets Taskbar Sample";
//    static const char * const message
//        = "wxWidgets sample showing wxTaskBarIcon class\n"
//          "\n"
//          "(C) 1997 Julian Smart\n"
//          "(C) 2007 Vadim Zeitlin";
//
//#if defined(__WXMSW__) && wxUSE_TASKBARICON_BALLOONS
//    m_taskBarIcon->ShowBalloon(title, message, 15000, wxICON_INFORMATION);
//#else // !__WXMSW__
//    wxMessageBox(message, title, wxICON_INFORMATION|wxOK, this);
//#endif // __WXMSW__/!__WXMSW__
//}
//
//void MyDialog::OnOK(wxCommandEvent& WXUNUSED(event))
//{
//    Show(false);
//}
//
//void MyDialog::OnExit(wxCommandEvent& WXUNUSED(event))
//{
//    Close(true);
//}
//
//void MyDialog::OnCloseWindow(wxCloseEvent& WXUNUSED(event))
//{
//    Destroy();
//}


// ----------------------------------------------------------------------------
// MyTaskBarIcon implementation
// ----------------------------------------------------------------------------

enum
{
    PU_RESTORE = 10001,
    PU_NEW_ICON,
    PU_EXIT,
    PU_CHECKMARK,
    PU_SUB1,
    PU_SUB2,
    PU_SUBMAIN
};


wxBEGIN_EVENT_TABLE(MyTaskBarIcon, wxTaskBarIcon)
    EVT_MENU(PU_RESTORE, MyTaskBarIcon::OnMenuRestore)
    EVT_MENU(PU_EXIT,    MyTaskBarIcon::OnMenuExit)
    EVT_MENU(PU_NEW_ICON,MyTaskBarIcon::OnMenuSetNewIcon)
    EVT_MENU(PU_CHECKMARK,MyTaskBarIcon::OnMenuCheckmark)
    EVT_UPDATE_UI(PU_CHECKMARK,MyTaskBarIcon::OnMenuUICheckmark)
    EVT_TASKBAR_LEFT_DCLICK  (MyTaskBarIcon::OnLeftButtonDClick)
    EVT_MENU(PU_SUB1, MyTaskBarIcon::OnMenuSub)
    EVT_MENU(PU_SUB2, MyTaskBarIcon::OnMenuSub)
wxEND_EVENT_TABLE()

void MyTaskBarIcon::OnMenuRestore(wxCommandEvent& )
{
    //gs_dialog->Show(true);
}

void MyTaskBarIcon::OnMenuExit(wxCommandEvent& )
{
    wxGetApp().Exit();
}

static bool check = true;

void MyTaskBarIcon::OnMenuCheckmark(wxCommandEvent& )
{
    check = !check;
}

void MyTaskBarIcon::OnMenuUICheckmark(wxUpdateUIEvent &event)
{
    event.Check(check);
}

void MyTaskBarIcon::OnMenuSetNewIcon(wxCommandEvent&)
{
    /*wxIcon icon(smile_xpm);

    if (!SetIcon(icon, "wxTaskBarIcon Sample - a different icon"))
        wxMessageBox("Could not set new icon.");*/
}

void MyTaskBarIcon::OnMenuSub(wxCommandEvent&)
{
    wxMessageBox("You clicked on a submenu!");
}

// Overridables
wxMenu *MyTaskBarIcon::CreatePopupMenu()
{
    wxMenu *menu = new wxMenu;
    menu->Append(PU_RESTORE, "&Restore main window");
    menu->AppendSeparator();
    menu->Append(PU_NEW_ICON, "&Set New Icon");
    menu->AppendSeparator();
    menu->AppendCheckItem(PU_CHECKMARK, "Test &check mark");
    menu->AppendSeparator();
    wxMenu *submenu = new wxMenu;
    submenu->Append(PU_SUB1, "One submenu");
    submenu->AppendSeparator();
    submenu->Append(PU_SUB2, "Another submenu");
    menu->Append(PU_SUBMAIN, "Submenu", submenu);
    /* OSX has built-in quit menu for the dock menu, but not for the status item */
#ifdef __WXOSX__
    if ( OSXIsStatusItem() )
#endif
    {
        menu->AppendSeparator();
        menu->Append(PU_EXIT,    "E&xit");
    }
    return menu;
}

void MyTaskBarIcon::OnLeftButtonDClick(wxTaskBarIconEvent&)
{
    //gs_dialog->Show(true);
}
