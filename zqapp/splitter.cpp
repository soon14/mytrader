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
#include "splitter.h"
#include "app.h"
#include "frame.h"

// ----------------------------------------------------------------------------
// MySplitterWindow
// ----------------------------------------------------------------------------

wxBEGIN_EVENT_TABLE(MySplitterWindow, wxSplitterWindow)
	EVT_SPLITTER_SASH_POS_CHANGED(wxID_ANY, MySplitterWindow::OnPositionChanged)
	EVT_SPLITTER_SASH_POS_CHANGING(wxID_ANY, MySplitterWindow::OnPositionChanging)

	EVT_SPLITTER_DCLICK(wxID_ANY, MySplitterWindow::OnDClick)

	EVT_SPLITTER_UNSPLIT(wxID_ANY, MySplitterWindow::OnUnsplitEvent)
wxEND_EVENT_TABLE()

MySplitterWindow::MySplitterWindow(wxFrame *parent)
	: wxSplitterWindow(parent, wxID_ANY,
		wxDefaultPosition, wxDefaultSize,
		wxSP_3D | wxSP_LIVE_UPDATE |
		wxCLIP_CHILDREN /* | wxSP_NO_XP_THEME */)
{
	m_frame = parent;
}

void MySplitterWindow::OnPositionChanged(wxSplitterEvent& event)
{
	wxLogStatus(m_frame, "Position has changed, now = %d (or %d)",
		event.GetSashPosition(), GetSashPosition());

	event.Skip();
}

void MySplitterWindow::OnPositionChanging(wxSplitterEvent& event)
{
	wxLogStatus(m_frame, "Position is changing, now = %d (or %d)",
		event.GetSashPosition(), GetSashPosition());

	event.Skip();
}

void MySplitterWindow::OnDClick(wxSplitterEvent& event)
{
#if wxUSE_STATUSBAR
	m_frame->SetStatusText("Splitter double clicked", 1);
#endif // wxUSE_STATUSBAR

	event.Skip();
}

void MySplitterWindow::OnUnsplitEvent(wxSplitterEvent& event)
{
#if wxUSE_STATUSBAR
	m_frame->SetStatusText("Splitter unsplit", 1);
#endif // wxUSE_STATUSBAR

	event.Skip();
}
