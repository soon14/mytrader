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

#include "base.h"
#include "wx/splitter.h"
#include "wx/dcmirror.h"

class MySplitterWindow : public wxSplitterWindow
{
public:
	MySplitterWindow(wxFrame *parent);

	// event handlers
	void OnPositionChanged(wxSplitterEvent& event);
	void OnPositionChanging(wxSplitterEvent& event);
	void OnDClick(wxSplitterEvent& event);
	void OnUnsplitEvent(wxSplitterEvent& event);

private:
	wxFrame *m_frame;

	wxDECLARE_EVENT_TABLE();
	wxDECLARE_NO_COPY_CLASS(MySplitterWindow);
};
