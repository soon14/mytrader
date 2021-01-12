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
#include "view.h"
#include "wx/splitter.h"
#include "smartkb.h"

class ChildFrmBase : public wxMDIChildFrame,
	protected MenuEventLogger
{
public:
	ChildFrmBase(wxMDIParentFrame *parent);
	virtual ~ChildFrmBase();

	virtual void SetStatusText(const wxString &text, int number = 0);

	static unsigned GetChildrenCount() { return ms_numChildren; }

private:
	void OnClose(wxCommandEvent& event);
	void OnSize(wxSizeEvent& event);
	void OnMove(wxMoveEvent& event);
	void OnCloseWindow(wxCloseEvent& event);
	void OnUpdateUIDisable(wxUpdateUIEvent& event) { event.Enable(false); }

	static unsigned ms_numChildren;

	// simple test event handler class
	class EventHandler : public wxEvtHandler
	{
	public:
		EventHandler(unsigned numChild) : m_numChild(numChild) { }

	private:
		void OnRefresh(wxCommandEvent& event)
		{
			wxLogMessage("Child #%u refreshed.", m_numChild);
			event.Skip();
		}

		const unsigned m_numChild;

		wxDECLARE_EVENT_TABLE();

		wxDECLARE_NO_COPY_CLASS(EventHandler);
	};

	wxDECLARE_EVENT_TABLE();
};

class MyCanvas;

class MyChild : public ChildFrmBase
{
	typedef ChildFrmBase Base;
public:
    MyChild(wxMDIParentFrame *parent);
    virtual ~MyChild();

private:
    void OnActivate(wxActivateEvent& event);
    void OnRefresh(wxCommandEvent& event);
    void OnUpdateRefresh(wxUpdateUIEvent& event);
    void OnChangeTitle(wxCommandEvent& event);
    void OnChangePosition(wxCommandEvent& event);
    void OnChangeSize(wxCommandEvent& event);
    void OnMove(wxMoveEvent& event);
    void OnMenuOpen(wxMenuEvent& event) { LogMenuOpenClose(event, "opened"); }
    void OnMenuHighlight(wxMenuEvent& event) { LogMenuHighlight(event); }
    void OnMenuClose(wxMenuEvent& event) { LogMenuOpenClose(event, "closed"); }

#if wxUSE_CLIPBOARD
    void OnPaste(wxCommandEvent& event);
    void OnUpdatePaste(wxUpdateUIEvent& event);
#endif // wxUSE_CLIPBOARD

	wxWindow *m_left, *m_right;

	wxSplitterWindow* m_splitter;

    //MyCanvas *m_canvas;

    wxDECLARE_EVENT_TABLE();
};

class TechCanvas;

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// IDs for the controls and the menu commands
enum
{
	// menu items
	File_Quit = wxID_EXIT,
	File_About = wxID_ABOUT,

	MenuShow_First = wxID_HIGHEST,
	File_ShowDefault = MenuShow_First,
	File_ShowText,
	File_ShowLines,
	File_ShowBrushes,
	File_ShowPolygons,
	File_ShowMask,
	File_ShowMaskStretch,
	File_ShowOps,
	File_ShowRegions,
	File_ShowCircles,
	File_ShowSplines,
#if wxDRAWING_DC_SUPPORTS_ALPHA || wxUSE_GRAPHICS_CONTEXT
	File_ShowAlpha,
#endif // wxDRAWING_DC_SUPPORTS_ALPHA || wxUSE_GRAPHICS_CONTEXT
#if wxUSE_GRAPHICS_CONTEXT
	File_ShowGraphics,
#endif
	File_ShowSystemColours,
	File_ShowGradients,
	MenuShow_Last = File_ShowGradients,

#if wxUSE_GRAPHICS_CONTEXT
	File_DC,
	File_GC_Default,
#if wxUSE_CAIRO
	File_GC_Cairo,
#endif // wxUSE_CAIRO
#ifdef __WXMSW__
#if wxUSE_GRAPHICS_GDIPLUS
	File_GC_GDIPlus,
#endif
#if wxUSE_GRAPHICS_DIRECT2D
	File_GC_Direct2D,
#endif
#endif // __WXMSW__
#endif // wxUSE_GRAPHICS_CONTEXT
	File_BBox,
	File_Clip,
	File_Buffer,
#if wxUSE_GRAPHICS_CONTEXT
	File_AntiAliasing,
#endif
	File_Copy,
	File_Save,

	MenuOption_First,

	MapMode_Text = MenuOption_First,
	MapMode_Lometric,
	MapMode_Twips,
	MapMode_Points,
	MapMode_Metric,

	UserScale_StretchHoriz,
	UserScale_ShrinkHoriz,
	UserScale_StretchVertic,
	UserScale_ShrinkVertic,
	UserScale_Restore,

	AxisMirror_Horiz,
	AxisMirror_Vertic,

	LogicalOrigin_MoveDown,
	LogicalOrigin_MoveUp,
	LogicalOrigin_MoveLeft,
	LogicalOrigin_MoveRight,
	LogicalOrigin_Set,
	LogicalOrigin_Restore,

#if wxUSE_COLOURDLG
	Colour_TextForeground,
	Colour_TextBackground,
	Colour_Background,
#endif // wxUSE_COLOURDLG
	Colour_BackgroundMode,
	Colour_TextureBackgound,

	MenuOption_Last = Colour_TextureBackgound
};

class TechChild : public ChildFrmBase
	, public zqdb::TechContainerMap<TechChild, zqdb::TechContainerInfo>
{
	typedef ChildFrmBase Base;
	typedef zqdb::TechContainerMap<TechChild, zqdb::TechContainerInfo> ContainerMap;
public:
	TechChild(wxMDIParentFrame *parent, const char* xml, size_t xmlflag);
	//virtual ~TechChild();

	void OnInfoChanged();
	void OnCurItemChanged();

	void OnCodePrev(wxCommandEvent& event);
	void OnCodeNext(wxCommandEvent& event);
	void OnCodeField(wxCommandEvent& event);

#if wxUSE_GRAPHICS_CONTEXT
	void OnGraphicContextNone(wxCommandEvent& WXUNUSED(event));
	void OnGraphicContextNoneUpdateUI(wxUpdateUIEvent& event);
	void OnGraphicContextDefault(wxCommandEvent& WXUNUSED(event));
	void OnGraphicContextDefaultUpdateUI(wxUpdateUIEvent& event);
#if wxUSE_CAIRO
	void OnGraphicContextCairo(wxCommandEvent& WXUNUSED(event));
	void OnGraphicContextCairoUpdateUI(wxUpdateUIEvent& event);
#endif // wxUSE_CAIRO
#ifdef __WXMSW__
#if wxUSE_GRAPHICS_GDIPLUS
	void OnGraphicContextGDIPlus(wxCommandEvent& WXUNUSED(event));
	void OnGraphicContextGDIPlusUpdateUI(wxUpdateUIEvent& event);
#endif
#if wxUSE_GRAPHICS_DIRECT2D
	void OnGraphicContextDirect2D(wxCommandEvent& WXUNUSED(event));
	void OnGraphicContextDirect2DUpdateUI(wxUpdateUIEvent& event);
#endif
#endif // __WXMSW__
	void OnAntiAliasing(wxCommandEvent& event);
	void OnAntiAliasingUpdateUI(wxUpdateUIEvent& event);
#endif // wxUSE_GRAPHICS_CONTEXT

	void OnClip(wxCommandEvent& event);
	void OnBuffer(wxCommandEvent& event);
	void OnCopy(wxCommandEvent& event);
	void OnSave(wxCommandEvent& event);
	void OnShow(wxCommandEvent &event);
	void OnOption(wxCommandEvent &event);
	void OnBoundingBox(wxCommandEvent& evt);
	void OnBoundingBoxUpdateUI(wxUpdateUIEvent& evt);

#if wxUSE_COLOURDLG
	wxColour SelectColour();
#endif // wxUSE_COLOURDLG
	void PrepareDC(wxDC& dc) wxOVERRIDE;

	int         m_backgroundMode;
	int         m_textureBackground;
	wxMappingMode m_mapMode;
	double      m_xUserScale;
	double      m_yUserScale;
	int         m_xLogicalOrigin;
	int         m_yLogicalOrigin;
	bool        m_xAxisReversed,
		m_yAxisReversed;
	wxColour    m_colourForeground,    // these are _text_ colours
		m_colourBackground;
	wxBrush     m_backgroundBrush;
	zqdb::TechView *m_tv;
	TechCanvas   *m_canvas;
	wxMenuItem *m_menuItemUseDC;
private:
	wxDECLARE_EVENT_TABLE();
};