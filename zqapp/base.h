#pragma once

#include <calcif.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <iomanip>

#include <XUtil/XXml.hpp>

#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#include "wx/mdi.h"
#endif

#include "wx/toolbar.h"

#include "wx/app.h"
#include "wx/frame.h"
#include "wx/textctrl.h"
#include "wx/ribbon/bar.h"
#include "wx/ribbon/buttonbar.h"
#include "wx/ribbon/gallery.h"
#include "wx/ribbon/toolbar.h"
#include "wx/sizer.h"
#include "wx/menu.h"
#include "wx/msgdlg.h"
#include "wx/dcbuffer.h"
#include "wx/colordlg.h"
#include "wx/artprov.h"
#include "wx/combobox.h"
#include "wx/tglbtn.h"
#include "wx/wrapsizer.h"
#include <wx/taskbar.h>
#include "wx/image.h"
#include "wx/artprov.h"
#include "wx/dcbuffer.h"
#include "wx/dcgraph.h"
#include "wx/overlay.h"
#include "wx/graphics.h"
#include "wx/filename.h"
#include "wx/metafile.h"
#include "wx/settings.h"
#if wxUSE_SVG
#include "wx/dcsvg.h"
#endif
#if wxUSE_POSTSCRIPT
#include "wx/dcps.h"
#endif

#include "wx/html/htmlwin.h"

#include "wx/choicebk.h"
#include "wx/listbook.h"
#include "wx/treebook.h"
#include "wx/notebook.h"
#include "wx/simplebook.h"
#include "wx/toolbook.h"
#include "wx/aui/auibook.h"

// Standard DC supports drawing with alpha on OSX and GTK3.
#if defined(__WXOSX__) || defined(__WXGTK3__)
#define wxDRAWING_DC_SUPPORTS_ALPHA 1
#else
#define wxDRAWING_DC_SUPPORTS_ALPHA 0
#endif // __WXOSX__ || __WXGTK3__

// Helper class logging menu open/close events.
class MenuEventLogger
{
public:
	MenuEventLogger(const char* label, wxFrame* frame)
		: m_label(label),
		m_frame(frame)
	{
	}

protected:
	void LogMenuOpenClose(wxMenuEvent& event, const char *action)
	{
		event.Skip();

		wxString what;

		wxMenu* const menu = event.GetMenu();
		if (menu)
			what.Printf("Menu \"%s\"", menu->GetTitle());
		else
			what = "Unknown menu";

		wxLogMessage(m_frame, "%s %s in %s", what, action, m_label);
	}

	void LogMenuHighlight(wxMenuEvent& event)
	{
		event.Skip();

		wxLogMessage(m_frame, "Item %d selected in %s",
			event.GetMenuId(), m_label);
	}

	const wxString m_label;
	wxFrame* const m_frame;

	wxDECLARE_NO_COPY_CLASS(MenuEventLogger);
};

// menu items ids
enum
{
	MDI_FULLSCREEN = 100,
	MDI_REFRESH,
	MDI_DISABLED_FROM_CANVAS,
	MDI_DISABLED_FROM_CHILD,
	MDI_CHANGE_TITLE,
	MDI_CHANGE_POSITION,
	MDI_CHANGE_SIZE
};
