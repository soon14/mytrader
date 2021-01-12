/////////////////////////////////////////////////////////////////////////////
// Name:        mdi.cpp
// Purpose:     MDI sample
// Author:      Julian Smart
// Modified by:
// Created:     04/01/98
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#pragma once

#include "wx/toolbar.h"

#include "base.h"
#include "datamodel.h"

enum ID_COMMANDS
{
	// these should be in the same order as Type_XXX elements above
	ID_BOOK_NOTEBOOK = wxID_HIGHEST,
	ID_BOOK_LISTBOOK,
	ID_BOOK_CHOICEBOOK,
	ID_BOOK_TREEBOOK,
	ID_BOOK_TOOLBOOK,
	ID_BOOK_AUINOTEBOOK,
	ID_BOOK_SIMPLEBOOK,
	ID_BOOK_MAX,

	ID_ORIENT_DEFAULT,
	ID_ORIENT_TOP,
	ID_ORIENT_BOTTOM,
	ID_ORIENT_LEFT,
	ID_ORIENT_RIGHT,
	ID_ORIENT_MAX,
	ID_SHOW_IMAGES,
	ID_FIXEDWIDTH,
	ID_MULTI,
	ID_NOPAGETHEME,
	ID_BUTTONBAR,
	ID_HORZ_LAYOUT,
	ID_ADD_PAGE,
	ID_ADD_PAGE_NO_SELECT,
	ID_INSERT_PAGE,
	ID_DELETE_CUR_PAGE,
	ID_DELETE_LAST_PAGE,
	ID_NEXT_PAGE,
	ID_ADD_PAGE_BEFORE,
	ID_ADD_SUB_PAGE,
	ID_CHANGE_SELECTION,
	ID_SET_SELECTION,
	ID_GET_PAGE_SIZE,
	ID_SET_PAGE_SIZE,

#if wxUSE_HELP
	ID_CONTEXT_HELP,
#endif // wxUSE_HELP
	ID_HITTEST
};

// Define a new frame
class MyFrame : public wxMDIParentFrame,
                private MenuEventLogger
{
public:
    MyFrame();
    virtual ~MyFrame();

    static wxMenuBar *CreateMainMenubar();

private:
    void InitToolBar(wxToolBar* toolBar);

	void RecreateBook();

	void FindOrCreateTech(HZQDB h);

	void OnGoto(wxCommandEvent& event);
	void OnActivated(wxDataViewEvent &event);

	void OnCodeField(wxCommandEvent& event);

    void OnSize(wxSizeEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnNewWindow(wxCommandEvent& event);
    void OnFullScreen(wxCommandEvent& event);
    void OnQuit(wxCommandEvent& event);
    void OnCloseAll(wxCommandEvent& event);

    void OnMenuOpen(wxMenuEvent& event) { LogMenuOpenClose(event, "opened"); }
    void OnMenuHighlight(wxMenuEvent& event) { LogMenuHighlight(event); }
    void OnMenuClose(wxMenuEvent& event) { LogMenuOpenClose(event, "closed"); }

    void OnClose(wxCloseEvent& event);

	// Sample setup
	enum BookType
	{
		Type_Notebook,
		Type_Listbook,
		Type_Choicebook,
		Type_Treebook,
		Type_Toolbook,
		Type_AuiNotebook,
		Type_Simplebook,
		Type_Max
	} m_type;
	int m_orient;
	bool m_chkShowImages;
	bool m_fixedWidth;
	bool m_multi;
	bool m_noPageTheme;
	bool m_buttonBar;
	bool m_horzLayout;
	wxBookCtrlBase *m_bookCtrl;

	wxObjectDataPtr<MyCodeListModel> code_list_model_;

    wxTextCtrl *m_textWindow;

    wxDECLARE_EVENT_TABLE();
};
