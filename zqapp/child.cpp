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

#include "child.h"
#include "app.h"
#include "frame.h"
#include "splitter.h"
#include "view.h"

// ===========================================================================
// implementation
// ===========================================================================

// ---------------------------------------------------------------------------
// ChildFrmBase
// ---------------------------------------------------------------------------

// Note that wxID_NEW and wxID_ABOUT commands get passed
// to the parent window for processing, so no need to
// duplicate event handlers here.
wxBEGIN_EVENT_TABLE(ChildFrmBase, wxMDIChildFrame)
	EVT_MENU(wxID_CLOSE, ChildFrmBase::OnClose)
	EVT_MOVE(ChildFrmBase::OnMove)
	EVT_SIZE(ChildFrmBase::OnSize)
	EVT_CLOSE(ChildFrmBase::OnCloseWindow)
wxEND_EVENT_TABLE()

wxBEGIN_EVENT_TABLE(ChildFrmBase::EventHandler, wxEvtHandler)
	EVT_MENU(MDI_REFRESH, ChildFrmBase::EventHandler::OnRefresh)
	EVT_UPDATE_UI(MDI_DISABLED_FROM_CHILD, ChildFrmBase::OnUpdateUIDisable)
wxEND_EVENT_TABLE()

unsigned ChildFrmBase::ms_numChildren = 0;

ChildFrmBase::ChildFrmBase(wxMDIParentFrame *parent)
	: wxMDIChildFrame
	(
		parent,
		wxID_ANY,
		wxString::Format("Child %u", ++ms_numChildren)
	),
	MenuEventLogger("child", this)
{
	SetIcon(wxICON(chart));

	const bool canBeResized = !IsAlwaysMaximized();

	// this should work for MDI frames as well as for normal ones, provided
	// they can be resized at all
	if (canBeResized)
		SetSizeHints(100, 100);

	// test that event handlers pushed on top of MDI children do work (this
	// used to be broken, see #11225)
	PushEventHandler(new EventHandler(ms_numChildren));
}

ChildFrmBase::~ChildFrmBase()
{
	PopEventHandler(true);

	ms_numChildren--;
}

void ChildFrmBase::SetStatusText(const wxString &text, int number)
{
	((wxMDIChildFrame*)GetParent())->SetStatusText(text, number);
}

void ChildFrmBase::OnClose(wxCommandEvent& WXUNUSED(event))
{
	Close(true);
}

void ChildFrmBase::OnMove(wxMoveEvent& event)
{
	// VZ: here everything is totally wrong under MSW, the positions are
	//     different and both wrong (pos2 is off by 2 pixels for me which seems
	//     to be the width of the MDI canvas border)
	wxPoint pos1 = event.GetPosition(),
		pos2 = GetPosition();
	wxLogStatus("position from event: (%d, %d), from frame (%d, %d)",
		pos1.x, pos1.y, pos2.x, pos2.y);

	event.Skip();
}

void ChildFrmBase::OnSize(wxSizeEvent& event)
{
	// VZ: under MSW the size event carries the client size (quite
	//     unexpectedly) *except* for the very first one which has the full
	//     size... what should it really be? TODO: check under wxGTK
	wxSize size1 = event.GetSize(),
		size2 = GetSize(),
		size3 = GetClientSize();
	wxLogStatus("size from event: %dx%d, from frame %dx%d, client %dx%d",
		size1.x, size1.y, size2.x, size2.y, size3.x, size3.y);

	event.Skip();
}

void ChildFrmBase::OnCloseWindow(wxCloseEvent& event)
{
	/*if ( m_canvas && m_canvas->IsDirty() )
	{
	if ( wxMessageBox("Really close?", "Please confirm",
	wxICON_QUESTION | wxYES_NO) != wxYES )
	{
	event.Veto();

	return;
	}
	}*/

	event.Skip();
}

// ===========================================================================
// implementation
// ===========================================================================

// ---------------------------------------------------------------------------
// MyChild
// ---------------------------------------------------------------------------

// Note that wxID_NEW and wxID_ABOUT commands get passed
// to the parent window for processing, so no need to
// duplicate event handlers here.
wxBEGIN_EVENT_TABLE(MyChild, wxMDIChildFrame)
    EVT_MENU(MDI_REFRESH, MyChild::OnRefresh)
    EVT_MENU(MDI_CHANGE_TITLE, MyChild::OnChangeTitle)
    EVT_MENU(MDI_CHANGE_POSITION, MyChild::OnChangePosition)
    EVT_MENU(MDI_CHANGE_SIZE, MyChild::OnChangeSize)

#if wxUSE_CLIPBOARD
    EVT_MENU(wxID_PASTE, MyChild::OnPaste)
    EVT_UPDATE_UI(wxID_PASTE, MyChild::OnUpdatePaste)
#endif // wxUSE_CLIPBOARD

    EVT_MENU_OPEN(MyChild::OnMenuOpen)
    EVT_MENU_HIGHLIGHT(wxID_ABOUT, MyChild::OnMenuHighlight)
    EVT_MENU_HIGHLIGHT(MDI_REFRESH, MyChild::OnMenuHighlight)
    EVT_MENU_CLOSE(MyChild::OnMenuClose)

wxEND_EVENT_TABLE()

MyChild::MyChild(wxMDIParentFrame *parent)
       : Base(parent)
{
	m_splitter = new MySplitterWindow(this);

	// If you use non-zero gravity you must initialize the splitter with its
	// correct initial size, otherwise it will change the sash position by a
	// huge amount when it's resized from its initial default size to its real
	// size when the frame lays it out. This wouldn't be necessary if default
	// zero gravity were used (although it would do no harm neither).
	m_splitter->SetSize(GetClientSize());
	m_splitter->SetSashGravity(1.0);

#if 1
	m_left = new MyCanvas(m_splitter, true);
	m_left->SetBackgroundColour(*wxRED);
	m_left->SetCursor(wxCursor(wxCURSOR_MAGNIFIER));

	m_right = new MyCanvas(m_splitter, false);
	m_right->SetBackgroundColour(*wxCYAN);
#else // for testing kbd navigation inside the splitter
	m_left = new wxTextCtrl(m_splitter, wxID_ANY, "first text");
	m_right = new wxTextCtrl(m_splitter, wxID_ANY, "second text");
#endif

	// you can also do this to start with a single window
#if 0
	m_right->Show(false);
	m_splitter->Initialize(m_left);
#else
	// you can also try -100
	m_splitter->SplitVertically(m_left, m_right, 100);
#endif

	//m_canvas = new MyCanvas(this, wxPoint(0, 0), GetClientSize());

    const bool canBeResized = !IsAlwaysMaximized();

    // create our menu bar: it will be shown instead of the main frame one when
    // we're active
#if wxUSE_MENUS
    wxMenuBar *mbar = MyFrame::CreateMainMenubar();
    mbar->GetMenu(0)->Insert(1, wxID_CLOSE, "&Close child\tCtrl-W",
                             "Close this window");

    wxMenu *menuChild = new wxMenu;

    menuChild->Append(MDI_REFRESH, "&Refresh picture");
    menuChild->Append(MDI_CHANGE_TITLE, "Change &title...\tCtrl-T");
    if ( canBeResized )
    {
        menuChild->AppendSeparator();
        menuChild->Append(MDI_CHANGE_POSITION, "Move frame\tCtrl-M");
        menuChild->Append(MDI_CHANGE_SIZE, "Resize frame\tCtrl-S");
    }
#if wxUSE_CLIPBOARD
    menuChild->AppendSeparator();
    menuChild->Append(MDI_DISABLED_FROM_CANVAS, "Item not disabled by canvas");
    menuChild->Append(MDI_DISABLED_FROM_CHILD, "Item disabled by child");
    menuChild->AppendSeparator();
    menuChild->Append(wxID_PASTE, "Copy text from clipboard\tCtrl-V");
#endif // wxUSE_CLIPBOARD

    mbar->Insert(1, menuChild, "&Child");

    // Associate the menu bar with the frame
    SetMenuBar(mbar);
#endif // wxUSE_MENUS

    // this should work for MDI frames as well as for normal ones, provided
    // they can be resized at all
    if ( canBeResized )
        SetSizeHints(100, 100);
}

MyChild::~MyChild()
{
}

void MyChild::OnRefresh(wxCommandEvent& WXUNUSED(event))
{
    if (m_splitter)
		m_splitter->Refresh();
}

void MyChild::OnChangePosition(wxCommandEvent& WXUNUSED(event))
{
    Move(10, 10);
}

void MyChild::OnChangeSize(wxCommandEvent& WXUNUSED(event))
{
    SetClientSize(100, 100);
}

void MyChild::OnChangeTitle(wxCommandEvent& WXUNUSED(event))
{
#if wxUSE_TEXTDLG
    static wxString s_title = "Canvas Frame";

    wxString title = wxGetTextFromUser("Enter the new title for MDI child",
                                       "MDI sample question",
                                       s_title,
                                       GetParent()->GetParent());
    if ( !title )
        return;

    s_title = title;
    SetTitle(s_title);
#endif // wxUSE_TEXTDLG
}

void MyChild::OnActivate(wxActivateEvent& event)
{
    if ( event.GetActive() && m_splitter)
		m_splitter->SetFocus();
}

#if wxUSE_CLIPBOARD

#include "wx/clipbrd.h"

void MyChild::OnPaste(wxCommandEvent& WXUNUSED(event))
{
	/*wxClipboardLocker lock;
    wxTextDataObject data;
    m_canvas->SetText(wxTheClipboard->GetData(data)
                        ? data.GetText()
                        : wxString("No text on clipboard"));*/
}

void MyChild::OnUpdatePaste(wxUpdateUIEvent& event)
{
    wxClipboardLocker lock;
    event.Enable( wxTheClipboard->IsSupported(wxDF_TEXT) );
}

#endif // wxUSE_CLIPBOARD



// ----------------------------------------------------------------------------
// TechChild
// ----------------------------------------------------------------------------

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them. It can be also done at run-time, but for the
// simple menu events like this the static method is much simpler.
wxBEGIN_EVENT_TABLE(TechChild, Base)
	//EVT_PAINT(TechChild::OnPaint)
	EVT_MENU(File_Clip, TechChild::OnClip)

#if wxUSE_GRAPHICS_CONTEXT
	EVT_MENU(File_GC_Default, TechChild::OnGraphicContextDefault)
	EVT_UPDATE_UI(File_GC_Default, TechChild::OnGraphicContextDefaultUpdateUI)
	EVT_MENU(File_DC, TechChild::OnGraphicContextNone)
	EVT_UPDATE_UI(File_DC, TechChild::OnGraphicContextNoneUpdateUI)
#if wxUSE_CAIRO
	EVT_MENU(File_GC_Cairo, TechChild::OnGraphicContextCairo)
	EVT_UPDATE_UI(File_GC_Cairo, TechChild::OnGraphicContextCairoUpdateUI)
#endif // wxUSE_CAIRO
#ifdef __WXMSW__
#if wxUSE_GRAPHICS_GDIPLUS
	EVT_MENU(File_GC_GDIPlus, TechChild::OnGraphicContextGDIPlus)
	EVT_UPDATE_UI(File_GC_GDIPlus, TechChild::OnGraphicContextGDIPlusUpdateUI)
#endif
#if wxUSE_GRAPHICS_DIRECT2D
	EVT_MENU(File_GC_Direct2D, TechChild::OnGraphicContextDirect2D)
	EVT_UPDATE_UI(File_GC_Direct2D, TechChild::OnGraphicContextDirect2DUpdateUI)
#endif
#endif // __WXMSW__
	EVT_MENU(File_AntiAliasing, TechChild::OnAntiAliasing)
	EVT_UPDATE_UI(File_AntiAliasing, TechChild::OnAntiAliasingUpdateUI)
#endif // wxUSE_GRAPHICS_CONTEXT

	EVT_MENU(File_Buffer, TechChild::OnBuffer)
	EVT_MENU(File_Copy, TechChild::OnCopy)
	EVT_MENU(File_Save, TechChild::OnSave)
	EVT_MENU(File_BBox, TechChild::OnBoundingBox)
	EVT_UPDATE_UI(File_BBox, TechChild::OnBoundingBoxUpdateUI)

	EVT_MENU_RANGE(MenuShow_First, MenuShow_Last, TechChild::OnShow)

	EVT_MENU_RANGE(MenuOption_First, MenuOption_Last, TechChild::OnOption)
wxEND_EVENT_TABLE()

TechChild::TechChild(wxMDIParentFrame *parent, const char* xml, size_t xmlflag) :Base(parent)
{
	CFG_FROM_XML(cfg, xml, xmlflag);

	wxMenu *menuScreen = new wxMenu;
	menuScreen->Append(File_ShowDefault, "&Default screen\tF1");
	menuScreen->Append(File_ShowText, "&Text screen\tF2");
	menuScreen->Append(File_ShowLines, "&Lines screen\tF3");
	menuScreen->Append(File_ShowBrushes, "&Brushes screen\tF4");
	menuScreen->Append(File_ShowPolygons, "&Polygons screen\tF5");
	menuScreen->Append(File_ShowMask, "&Mask screen\tF6");
	menuScreen->Append(File_ShowMaskStretch, "1/&2 scaled mask\tShift-F6");
	menuScreen->Append(File_ShowOps, "&Raster operations screen\tF7");
	menuScreen->Append(File_ShowRegions, "Re&gions screen\tF8");
	menuScreen->Append(File_ShowCircles, "&Circles screen\tF9");
#if wxDRAWING_DC_SUPPORTS_ALPHA || wxUSE_GRAPHICS_CONTEXT
	menuScreen->Append(File_ShowAlpha, "&Alpha screen\tF10");
#endif // wxDRAWING_DC_SUPPORTS_ALPHA || wxUSE_GRAPHICS_CONTEXT
	menuScreen->Append(File_ShowSplines, "Spl&ines screen\tF11");
	menuScreen->Append(File_ShowGradients, "&Gradients screen\tF12");
#if wxUSE_GRAPHICS_CONTEXT
	menuScreen->Append(File_ShowGraphics, "&Graphics screen");
#endif
	menuScreen->Append(File_ShowSystemColours, "System &colours");

	wxMenu *menuFile = new wxMenu;
#if wxUSE_GRAPHICS_CONTEXT
	menuFile->AppendCheckItem(File_GC_Default, "Use default wx&GraphicContext\t1");
	m_menuItemUseDC = menuFile->AppendRadioItem(File_DC, "Use wx&DC\t0");
#if wxUSE_CAIRO
	menuFile->AppendRadioItem(File_GC_Cairo, "Use &Cairo\t2");
#endif // wxUSE_CAIRO
#ifdef __WXMSW__
#if wxUSE_GRAPHICS_GDIPLUS
	menuFile->AppendRadioItem(File_GC_GDIPlus, "Use &GDI+\t3");
#endif
#if wxUSE_GRAPHICS_DIRECT2D
	menuFile->AppendRadioItem(File_GC_Direct2D, "Use &Direct2D\t4");
#endif
#endif // __WXMSW__
#endif // wxUSE_GRAPHICS_CONTEXT
	menuFile->AppendSeparator();
	menuFile->AppendCheckItem(File_BBox, "Show bounding box\tCtrl-E",
		"Show extents used in drawing operations");
	menuFile->AppendCheckItem(File_Clip, "&Clip\tCtrl-C", "Clip/unclip drawing");
	menuFile->AppendCheckItem(File_Buffer, "&Use wx&BufferedPaintDC\tCtrl-Z", "Buffer painting");
#if wxUSE_GRAPHICS_CONTEXT
	menuFile->AppendCheckItem(File_AntiAliasing,
		"&Anti-Aliasing in wxGraphicContext\tCtrl-Shift-A",
		"Enable Anti-Aliasing in wxGraphicContext")
		->Check();
#endif
	menuFile->AppendSeparator();
#if wxUSE_METAFILE && defined(wxMETAFILE_IS_ENH)
	menuFile->Append(File_Copy, "Copy to clipboard");
#endif
	menuFile->Append(File_Save, "&Save...\tCtrl-S", "Save drawing to file");
	menuFile->AppendSeparator();
	menuFile->Append(File_About, "&About\tCtrl-A", "Show about dialog");
	menuFile->AppendSeparator();
	menuFile->Append(File_Quit, "E&xit\tAlt-X", "Quit this program");

	wxMenu *menuMapMode = new wxMenu;
	menuMapMode->Append(MapMode_Text, "&TEXT map mode");
	menuMapMode->Append(MapMode_Lometric, "&LOMETRIC map mode");
	menuMapMode->Append(MapMode_Twips, "T&WIPS map mode");
	menuMapMode->Append(MapMode_Points, "&POINTS map mode");
	menuMapMode->Append(MapMode_Metric, "&METRIC map mode");

	wxMenu *menuUserScale = new wxMenu;
	menuUserScale->Append(UserScale_StretchHoriz, "Stretch &horizontally\tCtrl-H");
	menuUserScale->Append(UserScale_ShrinkHoriz, "Shrin&k horizontally\tCtrl-G");
	menuUserScale->Append(UserScale_StretchVertic, "Stretch &vertically\tCtrl-V");
	menuUserScale->Append(UserScale_ShrinkVertic, "&Shrink vertically\tCtrl-W");
	menuUserScale->AppendSeparator();
	menuUserScale->Append(UserScale_Restore, "&Restore to normal\tCtrl-0");

	wxMenu *menuAxis = new wxMenu;
	menuAxis->AppendCheckItem(AxisMirror_Horiz, "Mirror horizontally\tCtrl-M");
	menuAxis->AppendCheckItem(AxisMirror_Vertic, "Mirror vertically\tCtrl-N");

	wxMenu *menuLogical = new wxMenu;
	menuLogical->Append(LogicalOrigin_MoveDown, "Move &down\tCtrl-D");
	menuLogical->Append(LogicalOrigin_MoveUp, "Move &up\tCtrl-U");
	menuLogical->Append(LogicalOrigin_MoveLeft, "Move &right\tCtrl-L");
	menuLogical->Append(LogicalOrigin_MoveRight, "Move &left\tCtrl-R");
	menuLogical->AppendSeparator();
	menuLogical->Append(LogicalOrigin_Set, "Set to (&100, 100)\tShift-Ctrl-1");
	menuLogical->Append(LogicalOrigin_Restore, "&Restore to normal\tShift-Ctrl-0");

	wxMenu *menuColour = new wxMenu;
#if wxUSE_COLOURDLG
	menuColour->Append(Colour_TextForeground, "Text &foreground...");
	menuColour->Append(Colour_TextBackground, "Text &background...");
	menuColour->Append(Colour_Background, "Background &colour...");
#endif // wxUSE_COLOURDLG
	menuColour->AppendCheckItem(Colour_BackgroundMode, "&Opaque/transparent\tCtrl-B");
	menuColour->AppendCheckItem(Colour_TextureBackgound, "Draw textured back&ground\tCtrl-T");

	// now append the freshly created menu to the menu bar...
	wxMenuBar *menuBar = new wxMenuBar;
	menuBar->Append(menuFile, "&Drawing");
	menuBar->Append(menuScreen, "Scree&n");
	menuBar->Append(menuMapMode, "&Mode");
	menuBar->Append(menuUserScale, "&Scale");
	menuBar->Append(menuAxis, "&Axis");
	menuBar->Append(menuLogical, "&Origin");
	menuBar->Append(menuColour, "&Colours");

	// ... and attach this menu bar to the frame
	SetMenuBar(menuBar);

	m_mapMode = wxMM_TEXT;
	m_xUserScale = 1.0;
	m_yUserScale = 1.0;
	m_xLogicalOrigin = 0;
	m_yLogicalOrigin = 0;
	m_xAxisReversed =
		m_yAxisReversed = false;
	m_backgroundMode = wxBRUSHSTYLE_SOLID;
	m_colourForeground = *wxBLACK;
	m_colourBackground = *wxLIGHT_GREY;
	m_textureBackground = false;

	/*auto opt_htv = cfg.get_child_optional("htmlview");
	if (opt_htv) {
		auto& cfg_htv = opt_htv.get();
		auto htv = new wxHtmlWindow(this);
		//htv->SetRelatedFrame(this, _("wxHTML Demo: '%s'"));
#if wxUSE_STATUSBAR
		//htv->SetRelatedStatusBar(1);
#endif // wxUSE_STATUSBAR
		htv->LoadPage(cfg_htv.get<std::string>("url"));
	}
	else */{
		auto opt_tv = cfg.get_child_optional("techview");
		ASSERT(opt_tv);
		auto& cfg_tv = opt_tv.get();
		auto tv = new zqdb::TechView(this, (const char*)&cfg_tv, XUtil::XML_FLAG_PTREE);
		tv->SetSkinInfo(wxGetApp().GetSkinInfo());
		m_tv = tv;
		//m_tv->SetScrollbars(10, 10, 100, 240);
	}
	//Bind(ZQDB_CODE_PREV_EVENT, &TechChild::OnCodePrev, this);
	//Bind(ZQDB_CODE_NEXT_EVENT, &TechChild::OnCodeNext, this);
	Bind(ZQDB_CODE_FIELD_EVENT, &TechChild::OnCodeField, this);
}

void TechChild::OnInfoChanged()
{
	OnCurItemChanged();
}

void TechChild::OnCurItemChanged()
{
	auto tv = (zqdb::TechView*)m_tv;
	auto h = GetItem(GetCurItemPos());
	auto data = std::make_shared<zqdb::Calc::Data>(h, CYC_1MIN);
	tagCodeInfo* code = (tagCodeInfo*)data->GetValue();
	wxCSConv utf8cv(wxFONTENCODING_UTF8);
	wxString title = wxString::Format(wxT("%s %s")
		, utf8cv.cMB2WX(code->Name).data()
		, utf8cv.cMB2WX(code->Code).data()
	);
	SetTitle(title);
	tv->SetCalcData(data);
}

void TechChild::OnCodePrev(wxCommandEvent& event)
{
	auto pos = GetCurItemPos();
	if (pos <= 0) {
		pos = GetItemCount() - 1;
	}
	else {
		pos = (pos - 1) % GetItemCount();
	}
	SetCurItemPos(pos);
}

void TechChild::OnCodeNext(wxCommandEvent& event)
{
	auto pos = GetCurItemPos();
	if (pos >= (GetItemCount() - 1)) {
		pos = 0;
	}
	else {
		pos = (pos + 1) % GetItemCount();
	}
	SetCurItemPos(pos);
}

void TechChild::OnCodeField(wxCommandEvent& event)
{
	HZQDB code = (HZQDB)event.GetClientData();
	if (code) {
		auto tv = (zqdb::TechView*)m_tv;
		auto calc_data_ptr = tv->GetCalcData();
		if (calc_data_ptr && calc_data_ptr->GetBase() == code) {
			tv->UpdateCalcData();
		}
	}
}

#if wxUSE_GRAPHICS_CONTEXT
void TechChild::OnGraphicContextNone(wxCommandEvent& WXUNUSED(event))
{
	m_canvas->UseGraphicRenderer(NULL);
}

void TechChild::OnGraphicContextNoneUpdateUI(wxUpdateUIEvent& event)
{
	event.Check(m_canvas->GetRenderer() == NULL);
}

void TechChild::OnGraphicContextDefault(wxCommandEvent& WXUNUSED(event))
{
	m_canvas->UseGraphicRenderer(wxGraphicsRenderer::GetDefaultRenderer());
}

void TechChild::OnGraphicContextDefaultUpdateUI(wxUpdateUIEvent& event)
{
	event.Check(m_canvas->IsDefaultRenderer());
}

#if wxUSE_CAIRO
void TechChild::OnGraphicContextCairo(wxCommandEvent& WXUNUSED(event))
{
	m_canvas->UseGraphicRenderer(wxGraphicsRenderer::GetCairoRenderer());
}

void TechChild::OnGraphicContextCairoUpdateUI(wxUpdateUIEvent& event)
{
	event.Check(m_canvas->GetRenderer() == wxGraphicsRenderer::GetCairoRenderer());
}
#endif // wxUSE_CAIRO
#ifdef __WXMSW__
#if wxUSE_GRAPHICS_GDIPLUS
void TechChild::OnGraphicContextGDIPlus(wxCommandEvent& WXUNUSED(event))
{
	m_canvas->UseGraphicRenderer(wxGraphicsRenderer::GetGDIPlusRenderer());
}

void TechChild::OnGraphicContextGDIPlusUpdateUI(wxUpdateUIEvent& event)
{
	event.Check(m_canvas->GetRenderer() == wxGraphicsRenderer::GetGDIPlusRenderer());
}
#endif
#if wxUSE_GRAPHICS_DIRECT2D
void TechChild::OnGraphicContextDirect2D(wxCommandEvent& WXUNUSED(event))
{
	m_canvas->UseGraphicRenderer(wxGraphicsRenderer::GetDirect2DRenderer());
}

void TechChild::OnGraphicContextDirect2DUpdateUI(wxUpdateUIEvent& event)
{
	event.Check(m_canvas->GetRenderer() == wxGraphicsRenderer::GetDirect2DRenderer());
}
#endif
#endif // __WXMSW__
void TechChild::OnAntiAliasing(wxCommandEvent& event)
{
	m_canvas->EnableAntiAliasing(event.IsChecked());
}

void TechChild::OnAntiAliasingUpdateUI(wxUpdateUIEvent& event)
{
	event.Enable(m_canvas->GetRenderer() != NULL);
}
#endif // wxUSE_GRAPHICS_CONTEXT

void TechChild::OnClip(wxCommandEvent& event)
{
	m_canvas->Clip(event.IsChecked());
}

void TechChild::OnBuffer(wxCommandEvent& event)
{
	m_canvas->UseBuffer(event.IsChecked());
}

void TechChild::OnCopy(wxCommandEvent& WXUNUSED(event))
{
#if wxUSE_METAFILE && defined(wxMETAFILE_IS_ENH)
	wxMetafileDC dc;
	if (!dc.IsOk())
		return;
	m_canvas->Draw(dc);
	wxMetafile *mf = dc.Close();
	if (!mf)
		return;
	mf->SetClipboard();
	delete mf;
#endif
}

void TechChild::OnSave(wxCommandEvent& WXUNUSED(event))
{
	wxString wildCard = "Bitmap image (*.bmp)|*.bmp;*.BMP";
#if wxUSE_LIBPNG
	wildCard.Append("|PNG image (*.png)|*.png;*.PNG");
#endif
#if wxUSE_SVG
	wildCard.Append("|SVG image (*.svg)|*.svg;*.SVG");
#endif
#if wxUSE_POSTSCRIPT
	wildCard.Append("|PostScript file (*.ps)|*.ps;*.PS");
#endif

	wxFileDialog dlg(this, "Save as bitmap", wxEmptyString, wxEmptyString,
		wildCard,
		wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	if (dlg.ShowModal() == wxID_OK)
	{
		int width, height;
		m_canvas->GetDrawingSize(&width, &height);
		wxFileName fn(dlg.GetPath());
		wxString ext = fn.GetExt().Lower();
#if wxUSE_SVG
		if (ext == "svg")
		{
#if wxUSE_GRAPHICS_CONTEXT
			// Graphics screen can only be drawn using GraphicsContext
			if (m_canvas->GetPage() == File_ShowGraphics) {
				wxLogMessage("Graphics screen can not be saved as SVG.");
				return;
			}
			wxGraphicsRenderer* tempRenderer = m_canvas->GetRenderer();
			m_canvas->UseGraphicRenderer(NULL);
#endif
			wxSVGFileDC svgdc(dlg.GetPath(), width, height, 72, "Drawing sample");
			svgdc.SetBitmapHandler(new wxSVGBitmapEmbedHandler());
			m_canvas->Draw(svgdc);
#if wxUSE_GRAPHICS_CONTEXT
			m_canvas->UseGraphicRenderer(tempRenderer);
#endif
		}
		else
#endif
#if wxUSE_POSTSCRIPT
			if (ext == "ps")
			{
#if wxUSE_GRAPHICS_CONTEXT
				// Graphics screen can only be drawn using wxGraphicsContext
				if (m_canvas->GetPage() == File_ShowGraphics)
				{
					wxLogMessage("Graphics screen can not be saved as PostScript file.");
					return;
				}
				wxGraphicsRenderer* curRenderer = m_canvas->GetRenderer();
				m_canvas->UseGraphicRenderer(NULL);
#endif // wxUSE_GRAPHICS_CONTEXT
				wxPrintData printData;
				printData.SetPrintMode(wxPRINT_MODE_FILE);
				printData.SetFilename(dlg.GetPath());
				printData.SetOrientation(wxPORTRAIT);
				printData.SetPaperId(wxPAPER_A4);
				wxPostScriptDC psdc(printData);
				// Save current scale factor
				const double curUserScaleX = m_xUserScale;
				const double curUserScaleY = m_yUserScale;
				// Change the scale temporarily to fit the drawing into the page.
				int w, h;
				psdc.GetSize(&w, &h);
				double sc = wxMin((double)w / width, (double)h / height);
				m_xUserScale *= sc;
				m_yUserScale *= sc;
				psdc.StartDoc("Drawing sample");
				// Define default font.
				psdc.SetFont(wxFontInfo(10).Family(wxFONTFAMILY_MODERN));
				psdc.StartPage();
				m_canvas->Draw(psdc);
				psdc.EndPage();
				psdc.EndDoc();
				// Restore original scale facor
				m_xUserScale = curUserScaleX;
				m_yUserScale = curUserScaleY;
#if wxUSE_GRAPHICS_CONTEXT
				m_canvas->UseGraphicRenderer(curRenderer);
#endif // wxUSE_GRAPHICS_CONTEXT
			}
			else
#endif // wxUSE_POSTSCRIPT
			{
				wxBitmap bmp(width, height);
				wxMemoryDC mdc(bmp);
				mdc.SetBackground(*wxWHITE_BRUSH);
				mdc.Clear();
				m_canvas->Draw(mdc);
				bmp.ConvertToImage().SaveFile(dlg.GetPath());
			}
	}
}

void TechChild::OnShow(wxCommandEvent& event)
{
	const int show = event.GetId();

#if wxDRAWING_DC_SUPPORTS_ALPHA || wxUSE_GRAPHICS_CONTEXT
	// Make sure we do use a graphics context when selecting one of the screens
	// requiring it.
#if wxDRAWING_DC_SUPPORTS_ALPHA
	// If DC supports drawing with alpha
	// then GC is necessary only for graphics screen.
	if (show == File_ShowGraphics)
#else // wxUSE_GRAPHICS_CONTEXT
	// DC doesn't support drawing with alpha
	// so GC is necessary both for alpha and graphics screen.
	if (show == File_ShowAlpha || show == File_ShowGraphics)
#endif // wxDRAWING_DC_SUPPORTS_ALPHA, wxUSE_GRAPHICS_CONTEXT
	{
		if (!m_canvas->HasRenderer())
			m_canvas->UseGraphicRenderer(wxGraphicsRenderer::GetDefaultRenderer());
		// Disable selecting wxDC, if necessary.
		m_menuItemUseDC->Enable(!m_canvas->HasRenderer());
	}
	else
	{
		m_menuItemUseDC->Enable(true);
	}
#endif // wxDRAWING_DC_SUPPORTS_ALPHA || wxUSE_GRAPHICS_CONTEXT
	m_canvas->ToShow(show);
}

void TechChild::OnOption(wxCommandEvent& event)
{
	switch (event.GetId())
	{
	case MapMode_Text:
		m_mapMode = wxMM_TEXT;
		break;
	case MapMode_Lometric:
		m_mapMode = wxMM_LOMETRIC;
		break;
	case MapMode_Twips:
		m_mapMode = wxMM_TWIPS;
		break;
	case MapMode_Points:
		m_mapMode = wxMM_POINTS;
		break;
	case MapMode_Metric:
		m_mapMode = wxMM_METRIC;
		break;

	case LogicalOrigin_MoveDown:
		m_yLogicalOrigin += 10;
		break;
	case LogicalOrigin_MoveUp:
		m_yLogicalOrigin -= 10;
		break;
	case LogicalOrigin_MoveLeft:
		m_xLogicalOrigin += 10;
		break;
	case LogicalOrigin_MoveRight:
		m_xLogicalOrigin -= 10;
		break;
	case LogicalOrigin_Set:
		m_xLogicalOrigin =
			m_yLogicalOrigin = -100;
		break;
	case LogicalOrigin_Restore:
		m_xLogicalOrigin =
			m_yLogicalOrigin = 0;
		break;

	case UserScale_StretchHoriz:
		m_xUserScale *= 1.10;
		break;
	case UserScale_ShrinkHoriz:
		m_xUserScale /= 1.10;
		break;
	case UserScale_StretchVertic:
		m_yUserScale *= 1.10;
		break;
	case UserScale_ShrinkVertic:
		m_yUserScale /= 1.10;
		break;
	case UserScale_Restore:
		m_xUserScale =
			m_yUserScale = 1.0;
		break;

	case AxisMirror_Vertic:
		m_yAxisReversed = !m_yAxisReversed;
		break;
	case AxisMirror_Horiz:
		m_xAxisReversed = !m_xAxisReversed;
		break;

#if wxUSE_COLOURDLG
	case Colour_TextForeground:
		m_colourForeground = SelectColour();
		break;
	case Colour_TextBackground:
		m_colourBackground = SelectColour();
		break;
	case Colour_Background:
	{
		wxColour col = SelectColour();
		if (col.IsOk())
		{
			m_backgroundBrush.SetColour(col);
		}
	}
	break;
#endif // wxUSE_COLOURDLG

	case Colour_BackgroundMode:
		m_backgroundMode = m_backgroundMode == wxBRUSHSTYLE_SOLID
			? wxBRUSHSTYLE_TRANSPARENT
			: wxBRUSHSTYLE_SOLID;
		break;

	case Colour_TextureBackgound:
		m_textureBackground = !m_textureBackground;
		break;

	default:
		// skip Refresh()
		return;
	}

	m_canvas->Refresh();
}

void TechChild::OnBoundingBox(wxCommandEvent& evt)
{
	m_canvas->ShowBoundingBox(evt.IsChecked());
}

void TechChild::OnBoundingBoxUpdateUI(wxUpdateUIEvent& evt)
{
#if wxUSE_GRAPHICS_CONTEXT
	evt.Enable(m_canvas->GetPage() != File_ShowGraphics);
#else
	wxUnusedVar(evt);
#endif // wxUSE_GRAPHICS_CONTEXT / !wxUSE_GRAPHICS_CONTEXT
}

void TechChild::PrepareDC(wxDC& dc)
{
	dc.SetLogicalOrigin(m_xLogicalOrigin, m_yLogicalOrigin);
	dc.SetAxisOrientation(!m_xAxisReversed, m_yAxisReversed);
	dc.SetUserScale(m_xUserScale, m_yUserScale);
	dc.SetMapMode(m_mapMode);
}

#if wxUSE_COLOURDLG
wxColour TechChild::SelectColour()
{
	wxColour col;
	wxColourData data;
	wxColourDialog dialog(this, &data);

	if (dialog.ShowModal() == wxID_OK)
	{
		col = dialog.GetColourData().GetColour();
	}

	return col;
}
#endif // wxUSE_COLOURDLG
