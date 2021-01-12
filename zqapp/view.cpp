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
#include "view.h"
#include "app.h"
#include "frame.h"
#include "child.h"
#include "wx/dcmirror.h"

// ----------------------------------------------------------------------------
// MyCanvas
// ----------------------------------------------------------------------------

MyCanvas::MyCanvas(wxWindow* parent, bool mirror)
	: wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
		wxHSCROLL | wxVSCROLL)
{
	m_mirror = mirror;
	SetScrollbars(20, 20, 5, 5);
}

void MyCanvas::OnDraw(wxDC& dcOrig)
{
	wxMirrorDC dc(dcOrig, m_mirror);

	dc.SetPen(*wxBLACK_PEN);
	dc.DrawLine(0, 0, 100, 200);

	dc.SetBackgroundMode(wxBRUSHSTYLE_TRANSPARENT);
	dc.DrawText("Testing", 50, 50);

	dc.SetPen(*wxRED_PEN);
	dc.SetBrush(*wxGREEN_BRUSH);
	dc.DrawRectangle(120, 120, 100, 80);
}

// ----------------------------------------------------------------------------
// TechCanvas
// ----------------------------------------------------------------------------

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
wxBEGIN_EVENT_TABLE(TechCanvas, wxScrolledWindow)
	EVT_PAINT(TechCanvas::OnPaint)
	EVT_MOTION(TechCanvas::OnMouseMove)
	EVT_LEFT_DOWN(TechCanvas::OnMouseDown)
	EVT_LEFT_UP(TechCanvas::OnMouseUp)
wxEND_EVENT_TABLE()

#include "smile.xpm"

TechCanvas::TechCanvas(TechChild *parent)
	: wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
		wxHSCROLL | wxVSCROLL)
{
	m_owner = parent;
	m_show = File_ShowDefault;
	m_smile_bmp = wxBitmap(smile_xpm);
	m_std_icon = wxArtProvider::GetIcon(wxART_INFORMATION);
	m_clip = false;
	m_rubberBand = false;
#if wxUSE_GRAPHICS_CONTEXT
	m_renderer = NULL;
	m_useAntiAliasing = true;
#endif
	m_useBuffer = false;
	m_showBBox = false;
	m_sizeX = 0;
	m_sizeY = 0;
}

void TechCanvas::DrawTestBrushes(wxDC& dc)
{
	static const wxCoord WIDTH = 200;
	static const wxCoord HEIGHT = 80;

	wxCoord x = 10,
		y = 10;

	dc.SetBrush(*wxGREEN_BRUSH);
	dc.DrawRectangle(x, y, WIDTH, HEIGHT);
	dc.DrawText("Solid green", x + 10, y + 10);

	y += HEIGHT;
	dc.SetBrush(wxBrush(*wxRED, wxBRUSHSTYLE_CROSSDIAG_HATCH));
	dc.DrawRectangle(x, y, WIDTH, HEIGHT);
	dc.DrawText("Diagonally hatched red", x + 10, y + 10);

	y += HEIGHT;
	dc.SetBrush(wxBrush(*wxBLUE, wxBRUSHSTYLE_CROSS_HATCH));
	dc.DrawRectangle(x, y, WIDTH, HEIGHT);
	dc.DrawText("Cross hatched blue", x + 10, y + 10);

	y += HEIGHT;
	dc.SetBrush(wxBrush(*wxCYAN, wxBRUSHSTYLE_VERTICAL_HATCH));
	dc.DrawRectangle(x, y, WIDTH, HEIGHT);
	dc.DrawText("Vertically hatched cyan", x + 10, y + 10);

	y += HEIGHT;
	dc.SetBrush(wxBrush(*wxBLACK, wxBRUSHSTYLE_HORIZONTAL_HATCH));
	dc.DrawRectangle(x, y, WIDTH, HEIGHT);
	dc.DrawText("Horizontally hatched black", x + 10, y + 10);

	y += HEIGHT;
	dc.SetBrush(wxBrush(*gs_bmpMask));
	dc.DrawRectangle(x, y, WIDTH, HEIGHT);
	dc.DrawText("Stipple mono", x + 10, y + 10);

	y += HEIGHT;
	dc.SetBrush(wxBrush(*gs_bmpNoMask));
	dc.DrawRectangle(x, y, WIDTH, HEIGHT);
	dc.DrawText("Stipple colour", x + 10, y + 10);
}

void TechCanvas::DrawTestPoly(wxDC& dc)
{
	wxBrush brushHatch(*wxRED, wxBRUSHSTYLE_FDIAGONAL_HATCH);
	dc.SetBrush(brushHatch);

	wxPoint star[5];
	star[0] = wxPoint(100, 60);
	star[1] = wxPoint(60, 150);
	star[2] = wxPoint(160, 100);
	star[3] = wxPoint(40, 100);
	star[4] = wxPoint(140, 150);

	dc.DrawText("You should see two (irregular) stars below, the left one "
		"hatched", 10, 10);
	dc.DrawText("except for the central region and the right "
		"one entirely hatched", 10, 30);
	dc.DrawText("The third star only has a hatched outline", 10, 50);

	dc.DrawPolygon(WXSIZEOF(star), star, 0, 30);
	dc.DrawPolygon(WXSIZEOF(star), star, 160, 30, wxWINDING_RULE);

	wxBrush brushHatchGreen(*wxGREEN, wxBRUSHSTYLE_FDIAGONAL_HATCH);
	dc.SetBrush(brushHatchGreen);
	wxPoint star2[10];
	star2[0] = wxPoint(0, 100);
	star2[1] = wxPoint(-59, -81);
	star2[2] = wxPoint(95, 31);
	star2[3] = wxPoint(-95, 31);
	star2[4] = wxPoint(59, -81);
	star2[5] = wxPoint(0, 80);
	star2[6] = wxPoint(-47, -64);
	star2[7] = wxPoint(76, 24);
	star2[8] = wxPoint(-76, 24);
	star2[9] = wxPoint(47, -64);
	int count[2] = { 5, 5 };

	dc.DrawPolyPolygon(WXSIZEOF(count), count, star2, 450, 150);
}

void TechCanvas::DrawTestLines(int x, int y, int width, wxDC &dc)
{
	dc.SetPen(wxPen(*wxBLACK, width));
	dc.SetBrush(*wxRED_BRUSH);
	dc.DrawText(wxString::Format("Testing lines of width %d", width), x + 10, y - 10);
	dc.DrawRectangle(x + 10, y + 10, 100, 190);

	dc.DrawText("Solid/dot/short dash/long dash/dot dash", x + 150, y + 10);
	dc.SetPen(wxPen(*wxBLACK, width));
	dc.DrawLine(x + 20, y + 20, 100, y + 20);
	dc.SetPen(wxPen(*wxBLACK, width, wxPENSTYLE_DOT));
	dc.DrawLine(x + 20, y + 30, 100, y + 30);
	dc.SetPen(wxPen(*wxBLACK, width, wxPENSTYLE_SHORT_DASH));
	dc.DrawLine(x + 20, y + 40, 100, y + 40);
	dc.SetPen(wxPen(*wxBLACK, width, wxPENSTYLE_LONG_DASH));
	dc.DrawLine(x + 20, y + 50, 100, y + 50);
	dc.SetPen(wxPen(*wxBLACK, width, wxPENSTYLE_DOT_DASH));
	dc.DrawLine(x + 20, y + 60, 100, y + 60);

	dc.DrawText("Hatches", x + 150, y + 70);
	dc.SetPen(wxPen(*wxBLACK, width, wxPENSTYLE_BDIAGONAL_HATCH));
	dc.DrawLine(x + 20, y + 70, 100, y + 70);
	dc.SetPen(wxPen(*wxBLACK, width, wxPENSTYLE_CROSSDIAG_HATCH));
	dc.DrawLine(x + 20, y + 80, 100, y + 80);
	dc.SetPen(wxPen(*wxBLACK, width, wxPENSTYLE_FDIAGONAL_HATCH));
	dc.DrawLine(x + 20, y + 90, 100, y + 90);
	dc.SetPen(wxPen(*wxBLACK, width, wxPENSTYLE_CROSS_HATCH));
	dc.DrawLine(x + 20, y + 100, 100, y + 100);
	dc.SetPen(wxPen(*wxBLACK, width, wxPENSTYLE_HORIZONTAL_HATCH));
	dc.DrawLine(x + 20, y + 110, 100, y + 110);
	dc.SetPen(wxPen(*wxBLACK, width, wxPENSTYLE_VERTICAL_HATCH));
	dc.DrawLine(x + 20, y + 120, 100, y + 120);

	dc.DrawText("User dash", x + 150, y + 140);
	wxPen ud(*wxBLACK, width, wxPENSTYLE_USER_DASH);
	wxDash dash1[6];
	dash1[0] = 8;  // Long dash  <---------+
	dash1[1] = 2;  // Short gap            |
	dash1[2] = 3;  // Short dash           |
	dash1[3] = 2;  // Short gap            |
	dash1[4] = 3;  // Short dash           |
	dash1[5] = 2;  // Short gap and repeat +
	ud.SetDashes(6, dash1);
	dc.SetPen(ud);
	dc.DrawLine(x + 20, y + 140, 100, y + 140);
	dash1[0] = 5;  // Make first dash shorter
	ud.SetDashes(6, dash1);
	dc.SetPen(ud);
	dc.DrawLine(x + 20, y + 150, 100, y + 150);
	dash1[2] = 5;  // Make second dash longer
	ud.SetDashes(6, dash1);
	dc.SetPen(ud);
	dc.DrawLine(x + 20, y + 160, 100, y + 160);
	dash1[4] = 5;  // Make third dash longer
	ud.SetDashes(6, dash1);
	dc.SetPen(ud);
	dc.DrawLine(x + 20, y + 170, 100, y + 170);

	wxPen penWithCap(*wxBLACK, width);
	dc.SetPen(penWithCap);
	dc.DrawText("Default cap", x + 270, y + 40);
	dc.DrawLine(x + 200, y + 50, x + 250, y + 50);

	penWithCap.SetCap(wxCAP_BUTT);
	dc.SetPen(penWithCap);
	dc.DrawText("Butt ", x + 270, y + 60);
	dc.DrawLine(x + 200, y + 70, x + 250, y + 70);

	penWithCap.SetCap(wxCAP_ROUND);
	dc.SetPen(penWithCap);
	dc.DrawText("Round cap", x + 270, y + 80);
	dc.DrawLine(x + 200, y + 90, x + 250, y + 90);

	penWithCap.SetCap(wxCAP_PROJECTING);
	dc.SetPen(penWithCap);
	dc.DrawText("Projecting cap", x + 270, y + 100);
	dc.DrawLine(x + 200, y + 110, x + 250, y + 110);
}

void TechCanvas::DrawCrossHair(int x, int y, int width, int heigth, wxDC &dc)
{
	dc.DrawText("Cross hair", x + 10, y + 10);
	dc.SetClippingRegion(x, y, width, heigth);
	dc.SetPen(wxPen(*wxBLUE, 2));
	dc.CrossHair(x + width / 2, y + heigth / 2);
	dc.DestroyClippingRegion();
}

void TechCanvas::DrawDefault(wxDC& dc)
{
	// Draw circle centered at the origin, then flood fill it with a different
	// color. Done with a wxMemoryDC because Blit (used by generic
	// wxDoFloodFill) from a window that is being painted gives unpredictable
	// results on wxGTK
	{
		wxImage img(21, 21, false);
		img.Clear(1);
		wxBitmap bmp(img);
		{
			wxMemoryDC mdc(bmp);
			mdc.SetBrush(dc.GetBrush());
			mdc.SetPen(dc.GetPen());
			mdc.DrawCircle(10, 10, 10);
			wxColour c;
			if (mdc.GetPixel(11, 11, &c))
			{
				mdc.SetBrush(wxColour(128, 128, 0));
				mdc.FloodFill(11, 11, c, wxFLOOD_SURFACE);
			}
		}
		bmp.SetMask(new wxMask(bmp, wxColour(1, 1, 1)));
		dc.DrawBitmap(bmp, -10, -10, true);
	}

	dc.DrawCheckMark(5, 80, 15, 15);
	dc.DrawCheckMark(25, 80, 30, 30);
	dc.DrawCheckMark(60, 80, 60, 60);

	// this is the test for "blitting bitmap into DC damages selected brush" bug
	wxCoord rectSize = m_std_icon.GetWidth() + 10;
	wxCoord x = 100;
	dc.SetPen(*wxTRANSPARENT_PEN);
	dc.SetBrush(*wxGREEN_BRUSH);
	dc.DrawRectangle(x, 10, rectSize, rectSize);
	dc.DrawBitmap(m_std_icon, x + 5, 15, true);
	x += rectSize + 10;
	dc.DrawRectangle(x, 10, rectSize, rectSize);
	dc.DrawIcon(m_std_icon, x + 5, 15);
	x += rectSize + 10;
	dc.DrawRectangle(x, 10, rectSize, rectSize);

	// test for "transparent" bitmap drawing (it intersects with the last
	// rectangle above)
	//dc.SetBrush( *wxTRANSPARENT_BRUSH );

	if (m_smile_bmp.IsOk())
		dc.DrawBitmap(m_smile_bmp, x + rectSize - 20, rectSize - 10, true);

	dc.SetBrush(*wxBLACK_BRUSH);
	dc.DrawRectangle(0, 160, 1000, 300);

	// draw lines
	wxBitmap bitmap(20, 70);
	wxMemoryDC memdc;
	memdc.SelectObject(bitmap);
	memdc.SetBrush(*wxBLACK_BRUSH);
	memdc.SetPen(*wxWHITE_PEN);
	memdc.DrawRectangle(0, 0, 20, 70);
	memdc.DrawLine(10, 0, 10, 70);

	// to the right
	wxPen pen = *wxRED_PEN;
	memdc.SetPen(pen);
	memdc.DrawLine(10, 5, 10, 5);
	memdc.DrawLine(10, 10, 11, 10);
	memdc.DrawLine(10, 15, 12, 15);
	memdc.DrawLine(10, 20, 13, 20);

	/*
	memdc.SetPen(*wxRED_PEN);
	memdc.DrawLine( 12, 5,12, 5 );
	memdc.DrawLine( 12,10,13,10 );
	memdc.DrawLine( 12,15,14,15 );
	memdc.DrawLine( 12,20,15,20 );
	*/

	// same to the left
	memdc.DrawLine(10, 25, 10, 25);
	memdc.DrawLine(10, 30, 9, 30);
	memdc.DrawLine(10, 35, 8, 35);
	memdc.DrawLine(10, 40, 7, 40);

	// XOR draw lines
	dc.SetPen(*wxWHITE_PEN);
	memdc.SetLogicalFunction(wxINVERT);
	memdc.SetPen(*wxWHITE_PEN);
	memdc.DrawLine(10, 50, 10, 50);
	memdc.DrawLine(10, 55, 11, 55);
	memdc.DrawLine(10, 60, 12, 60);
	memdc.DrawLine(10, 65, 13, 65);

	memdc.DrawLine(12, 50, 12, 50);
	memdc.DrawLine(12, 55, 13, 55);
	memdc.DrawLine(12, 60, 14, 60);
	memdc.DrawLine(12, 65, 15, 65);

	memdc.SelectObject(wxNullBitmap);
	dc.DrawBitmap(bitmap, 10, 170);
	wxImage image = bitmap.ConvertToImage();
	image.Rescale(60, 210);
	bitmap = wxBitmap(image);
	dc.DrawBitmap(bitmap, 50, 170);

	// test the rectangle outline drawing - there should be one pixel between
	// the rect and the lines
	dc.SetPen(*wxWHITE_PEN);
	dc.SetBrush(*wxTRANSPARENT_BRUSH);
	dc.DrawRectangle(150, 170, 49, 29);
	dc.DrawRectangle(200, 170, 49, 29);
	dc.SetPen(*wxWHITE_PEN);
	dc.DrawLine(250, 210, 250, 170);
	dc.DrawLine(260, 200, 150, 200);

	// test the rectangle filled drawing - there should be one pixel between
	// the rect and the lines
	dc.SetPen(*wxTRANSPARENT_PEN);
	dc.SetBrush(*wxWHITE_BRUSH);
	dc.DrawRectangle(300, 170, 49, 29);
	dc.DrawRectangle(350, 170, 49, 29);
	dc.SetPen(*wxWHITE_PEN);
	dc.DrawLine(400, 170, 400, 210);
	dc.DrawLine(300, 200, 410, 200);

	// a few more tests of this kind
	dc.SetPen(*wxRED_PEN);
	dc.SetBrush(*wxWHITE_BRUSH);
	dc.DrawRectangle(300, 220, 1, 1);
	dc.DrawRectangle(310, 220, 2, 2);
	dc.DrawRectangle(320, 220, 3, 3);
	dc.DrawRectangle(330, 220, 4, 4);

	dc.SetPen(*wxTRANSPARENT_PEN);
	dc.SetBrush(*wxWHITE_BRUSH);
	dc.DrawRectangle(300, 230, 1, 1);
	dc.DrawRectangle(310, 230, 2, 2);
	dc.DrawRectangle(320, 230, 3, 3);
	dc.DrawRectangle(330, 230, 4, 4);

	// and now for filled rect with outline
	dc.SetPen(*wxRED_PEN);
	dc.SetBrush(*wxWHITE_BRUSH);
	dc.DrawRectangle(500, 170, 49, 29);
	dc.DrawRectangle(550, 170, 49, 29);
	dc.SetPen(*wxWHITE_PEN);
	dc.DrawLine(600, 170, 600, 210);
	dc.DrawLine(500, 200, 610, 200);

	// test the rectangle outline drawing - there should be one pixel between
	// the rect and the lines
	dc.SetPen(*wxWHITE_PEN);
	dc.SetBrush(*wxTRANSPARENT_BRUSH);
	dc.DrawRoundedRectangle(150, 270, 49, 29, 6);
	dc.DrawRoundedRectangle(200, 270, 49, 29, 6);
	dc.SetPen(*wxWHITE_PEN);
	dc.DrawLine(250, 270, 250, 310);
	dc.DrawLine(150, 300, 260, 300);

	// test the rectangle filled drawing - there should be one pixel between
	// the rect and the lines
	dc.SetPen(*wxTRANSPARENT_PEN);
	dc.SetBrush(*wxWHITE_BRUSH);
	dc.DrawRoundedRectangle(300, 270, 49, 29, 6);
	dc.DrawRoundedRectangle(350, 270, 49, 29, 6);
	dc.SetPen(*wxWHITE_PEN);
	dc.DrawLine(400, 270, 400, 310);
	dc.DrawLine(300, 300, 410, 300);

	// Added by JACS to demonstrate bizarre behaviour.
	// With a size of 70, we get a missing red RHS,
	// and the height is too small, so we get yellow
	// showing. With a size of 40, it draws as expected:
	// it just shows a white rectangle with red outline.
	int totalWidth = 70;
	int totalHeight = 70;
	wxBitmap bitmap2(totalWidth, totalHeight);

	wxMemoryDC memdc2;
	memdc2.SelectObject(bitmap2);

	memdc2.SetBackground(*wxYELLOW_BRUSH);
	memdc2.Clear();

	// Now draw a white rectangle with red outline. It should
	// entirely eclipse the yellow background.
	memdc2.SetPen(*wxRED_PEN);
	memdc2.SetBrush(*wxWHITE_BRUSH);

	memdc2.DrawRectangle(0, 0, totalWidth, totalHeight);

	memdc2.SetPen(wxNullPen);
	memdc2.SetBrush(wxNullBrush);
	memdc2.SelectObject(wxNullBitmap);

	dc.DrawBitmap(bitmap2, 500, 270);

	// Repeat, but draw directly on dc
	// Draw a yellow rectangle filling the bitmap

	x = 600; int y = 270;
	dc.SetPen(*wxYELLOW_PEN);
	dc.SetBrush(*wxYELLOW_BRUSH);
	dc.DrawRectangle(x, y, totalWidth, totalHeight);

	// Now draw a white rectangle with red outline. It should
	// entirely eclipse the yellow background.
	dc.SetPen(*wxRED_PEN);
	dc.SetBrush(*wxWHITE_BRUSH);

	dc.DrawRectangle(x, y, totalWidth, totalHeight);
}

void TechCanvas::DrawText(wxDC& dc)
{
	// set underlined font for testing
	dc.SetFont(wxFontInfo(12).Family(wxFONTFAMILY_MODERN).Underlined());
	dc.DrawText("This is text", 110, 10);
	dc.DrawRotatedText("That is text", 20, 10, -45);

	// use wxSWISS_FONT and not wxNORMAL_FONT as the latter can't be rotated
	// under MSW (it is not TrueType)
	dc.SetFont(*wxSWISS_FONT);

	wxString text;
	dc.SetBackgroundMode(wxBRUSHSTYLE_TRANSPARENT);

	for (int n = -180; n < 180; n += 30)
	{
		text.Printf("     %d rotated text", n);
		dc.DrawRotatedText(text, 400, 400, n);
	}

	dc.SetFont(wxFontInfo(18).Family(wxFONTFAMILY_SWISS));

	dc.DrawText("This is Swiss 18pt text.", 110, 40);

	wxCoord length;
	wxCoord height;
	wxCoord descent;
	dc.GetTextExtent("This is Swiss 18pt text.", &length, &height, &descent);
	text.Printf("Dimensions are length %d, height %d, descent %d", length, height, descent);
	dc.DrawText(text, 110, 80);

	text.Printf("CharHeight() returns: %d", dc.GetCharHeight());
	dc.DrawText(text, 110, 120);

	dc.DrawRectangle(100, 40, 4, height);

	// test the logical function effect
	wxCoord y = 150;
	dc.SetLogicalFunction(wxINVERT);
	// text drawing should ignore logical function
	dc.DrawText("There should be a text below", 110, y);
	dc.DrawRectangle(110, y, 100, height);

	y += height;
	dc.DrawText("Visible text", 110, y);
	dc.DrawRectangle(110, y, 100, height);
	dc.DrawText("Visible text", 110, y);
	dc.DrawRectangle(110, y, 100, height);
	dc.SetLogicalFunction(wxCOPY);

	y += height;
	dc.DrawRectangle(110, y, 100, height);
	dc.DrawText("Another visible text", 110, y);

	y += height;
	dc.DrawText("And\nmore\ntext on\nmultiple\nlines", 110, y);
	y += 5 * height;

	dc.SetTextForeground(*wxBLUE);
	dc.DrawRotatedText("Rotated text\ncan have\nmultiple lines\nas well", 110, y, 15);
}

static const struct
{
	wxString name;
	wxRasterOperationMode rop;
} rasterOperations[] =
{
	{ "wxAND",          wxAND },
	{ "wxAND_INVERT",   wxAND_INVERT },
	{ "wxAND_REVERSE",  wxAND_REVERSE },
	{ "wxCLEAR",        wxCLEAR },
	{ "wxCOPY",         wxCOPY },
	{ "wxEQUIV",        wxEQUIV },
	{ "wxINVERT",       wxINVERT },
	{ "wxNAND",         wxNAND },
	{ "wxNO_OP",        wxNO_OP },
	{ "wxOR",           wxOR },
	{ "wxOR_INVERT",    wxOR_INVERT },
	{ "wxOR_REVERSE",   wxOR_REVERSE },
	{ "wxSET",          wxSET },
	{ "wxSRC_INVERT",   wxSRC_INVERT },
	{ "wxXOR",          wxXOR },
};

void TechCanvas::DrawImages(wxDC& dc, DrawMode mode)
{
	dc.DrawText("original image", 0, 0);
	dc.DrawBitmap(*gs_bmpNoMask, 0, 20, 0);
	dc.DrawText("with colour mask", 0, 100);
	dc.DrawBitmap(*gs_bmpWithColMask, 0, 120, true);
	dc.DrawText("the mask image", 0, 200);
	dc.DrawBitmap(*gs_bmpMask, 0, 220, 0);
	dc.DrawText("masked image", 0, 300);
	dc.DrawBitmap(*gs_bmpWithMask, 0, 320, true);

	int cx = gs_bmpWithColMask->GetWidth(),
		cy = gs_bmpWithColMask->GetHeight();

	wxMemoryDC memDC;
	for (size_t n = 0; n < WXSIZEOF(rasterOperations); n++)
	{
		wxCoord x = 120 + 150 * (n % 4),
			y = 20 + 100 * (n / 4);

		dc.DrawText(rasterOperations[n].name, x, y - 20);
		memDC.SelectObject(*gs_bmpWithColMask);
		if (mode == Draw_Stretch)
		{
			dc.StretchBlit(x, y, cx, cy, &memDC, 0, 0, cx / 2, cy / 2,
				rasterOperations[n].rop, true);
		}
		else
		{
			dc.Blit(x, y, cx, cy, &memDC, 0, 0, rasterOperations[n].rop, true);
		}
	}
}

void TechCanvas::DrawWithLogicalOps(wxDC& dc)
{
	static const wxCoord w = 60;
	static const wxCoord h = 60;

	// reuse the text colour here
	dc.SetPen(wxPen(m_owner->m_colourForeground));
	dc.SetBrush(*wxTRANSPARENT_BRUSH);

	size_t n;
	for (n = 0; n < WXSIZEOF(rasterOperations); n++)
	{
		wxCoord x = 20 + 150 * (n % 4),
			y = 20 + 100 * (n / 4);

		dc.DrawText(rasterOperations[n].name, x, y - 20);
		dc.SetLogicalFunction(rasterOperations[n].rop);
		dc.DrawRectangle(x, y, w, h);
		dc.DrawLine(x, y, x + w, y + h);
		dc.DrawLine(x + w, y, x, y + h);
	}

	// now some filled rectangles
	dc.SetBrush(wxBrush(m_owner->m_colourForeground));

	for (n = 0; n < WXSIZEOF(rasterOperations); n++)
	{
		wxCoord x = 20 + 150 * (n % 4),
			y = 500 + 100 * (n / 4);

		dc.DrawText(rasterOperations[n].name, x, y - 20);
		dc.SetLogicalFunction(rasterOperations[n].rop);
		dc.DrawRectangle(x, y, w, h);
	}
}

#if wxDRAWING_DC_SUPPORTS_ALPHA || wxUSE_GRAPHICS_CONTEXT
void TechCanvas::DrawAlpha(wxDC& dc)
{
	wxDouble margin = 20;
	wxDouble width = 180;
	wxDouble radius = 30;

	dc.SetPen(wxPen(wxColour(128, 0, 0), 12));
	dc.SetBrush(*wxRED_BRUSH);

	wxRect r(margin, margin + width*0.66, width, width);

	dc.DrawRoundedRectangle(r.x, r.y, r.width, r.width, radius);

	dc.SetPen(wxPen(wxColour(0, 0, 128), 12));
	dc.SetBrush(wxColour(0, 0, 255, 192));

	r.Offset(width * 0.8, -width * 0.66);

	dc.DrawRoundedRectangle(r.x, r.y, r.width, r.width, radius);

	dc.SetPen(wxPen(wxColour(128, 128, 0), 12));
	dc.SetBrush(wxBrush(wxColour(192, 192, 0, 192)));

	r.Offset(width * 0.8, width *0.5);

	dc.DrawRoundedRectangle(r.x, r.y, r.width, r.width, radius);

	dc.SetPen(*wxTRANSPARENT_PEN);
	dc.SetBrush(wxBrush(wxColour(255, 255, 128, 128)));
	dc.DrawRoundedRectangle(0, margin + width / 2, width * 3, 100, radius);

	dc.SetTextBackground(wxColour(160, 192, 160, 160));
	dc.SetTextForeground(wxColour(255, 128, 128, 128));
	dc.SetFont(wxFontInfo(40).Family(wxFONTFAMILY_SWISS).Italic());
	dc.DrawText("Hello!", 120, 80);
}
#endif // wxDRAWING_DC_SUPPORTS_ALPHA || wxUSE_GRAPHICS_CONTEXT

#if wxUSE_GRAPHICS_CONTEXT

const int BASE = 80.0;
const int BASE2 = BASE / 2;
const int BASE4 = BASE / 4;


// modeled along Robin Dunn's GraphicsContext.py sample

void TechCanvas::DrawGraphics(wxGraphicsContext* gc)
{
	wxFont font = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
	gc->SetFont(font, *wxBLACK);

	// make a path that contains a circle and some lines, centered at 0,0
	wxGraphicsPath path = gc->CreatePath();
	path.AddCircle(0, 0, BASE2);
	path.MoveToPoint(0, -BASE2);
	path.AddLineToPoint(0, BASE2);
	path.MoveToPoint(-BASE2, 0);
	path.AddLineToPoint(BASE2, 0);
	path.CloseSubpath();
	path.AddRectangle(-BASE4, -BASE4 / 2, BASE2, BASE4);

	// Now use that path to demonstrate various capabilities of the graphics context
	gc->PushState(); // save current translation/scale/other state
	gc->Translate(60, 75); // reposition the context origin

	gc->SetPen(wxPen("navy"));
	gc->SetBrush(wxBrush("pink"));

	for (int i = 0; i < 3; ++i)
	{
		wxString label;
		switch (i)
		{
		case 0:
			label = "StrokePath";
			break;
		case 1:
			label = "FillPath";
			break;
		case 2:
			label = "DrawPath";
			break;
		}
		wxDouble w, h;
		gc->GetTextExtent(label, &w, &h, NULL, NULL);
		gc->DrawText(label, -w / 2, -BASE2 - h - 4);
		switch (i)
		{
		case 0:
			gc->StrokePath(path);
			break;
		case 1:
			gc->FillPath(path);
			break;
		case 2:
			gc->DrawPath(path);
			break;
		}
		gc->Translate(2 * BASE, 0);
	}

	gc->PopState(); // restore saved state
	gc->PushState(); // save it again
	gc->Translate(60, 200); // offset to the lower part of the window

	gc->DrawText("Scale", 0, -BASE2);
	gc->Translate(0, 20);

	gc->SetBrush(wxBrush(wxColour(178, 34, 34, 128)));// 128 == half transparent
	for (int i = 0; i < 8; ++i)
	{
		gc->Scale(1.08, 1.08); // increase scale by 8%
		gc->Translate(5, 5);
		gc->DrawPath(path);
	}

	gc->PopState(); // restore saved state
	gc->PushState(); // save it again
	gc->Translate(400, 200);

	gc->DrawText("Rotate", 0, -BASE2);

	// Move the origin over to the next location
	gc->Translate(0, 75);

	// draw our path again, rotating it about the central point,
	// and changing colors as we go
	for (int angle = 0; angle < 360; angle += 30)
	{
		gc->PushState(); // save this new current state so we can
						 //  pop back to it at the end of the loop
		wxImage::RGBValue val = wxImage::HSVtoRGB(wxImage::HSVValue(float(angle) / 360, 1, 1));
		gc->SetBrush(wxBrush(wxColour(val.red, val.green, val.blue, 64)));
		gc->SetPen(wxPen(wxColour(val.red, val.green, val.blue, 128)));

		// use translate to artfully reposition each drawn path
		gc->Translate(1.5 * BASE2 * cos(wxDegToRad(angle)),
			1.5 * BASE2 * sin(wxDegToRad(angle)));

		// use Rotate to rotate the path
		gc->Rotate(wxDegToRad(angle));

		// now draw it
		gc->DrawPath(path);
		gc->PopState();
	}
	gc->PopState();

	gc->PushState();
	gc->Translate(60, 400);
	const wxString labelText("Scaled smiley inside a square");
	gc->DrawText(labelText, 0, 0);
	// Center a bitmap horizontally
	wxDouble textWidth;
	gc->GetTextExtent(labelText, &textWidth, NULL);
	const wxDouble rectWidth = 100;
	wxDouble x0 = (textWidth - rectWidth) / 2;
	gc->DrawRectangle(x0, BASE2, rectWidth, 100);
	gc->DrawBitmap(m_smile_bmp, x0, BASE2, rectWidth, 100);
	gc->PopState();

	// Draw graphics bitmap and its subbitmap
	gc->PushState();
	gc->Translate(300, 400);
	gc->DrawText("Smiley as a graphics bitmap", 0, 0);

	wxGraphicsBitmap gbmp1 = gc->CreateBitmap(m_smile_bmp);
	gc->DrawBitmap(gbmp1, 0, BASE2, 50, 50);
	int bmpw = m_smile_bmp.GetWidth();
	int bmph = m_smile_bmp.GetHeight();
	wxGraphicsBitmap gbmp2 = gc->CreateSubBitmap(gbmp1, 0, bmph / 5, bmpw / 2, bmph / 2);
	gc->DrawBitmap(gbmp2, 80, BASE2, 50, 50 * (bmph / 2) / (bmpw / 2));
	gc->PopState();
}
#endif // wxUSE_GRAPHICS_CONTEXT

void TechCanvas::DrawCircles(wxDC& dc)
{
	int x = 100,
		y = 100,
		r = 20;

	dc.SetPen(*wxRED_PEN);
	dc.SetBrush(*wxGREEN_BRUSH);

	dc.DrawText("Some circles", 0, y);
	dc.DrawCircle(x, y, r);
	dc.DrawCircle(x + 2 * r, y, r);
	dc.DrawCircle(x + 4 * r, y, r);

	y += 2 * r;
	dc.DrawText("And ellipses", 0, y);
	dc.DrawEllipse(x - r, y, 2 * r, r);
	dc.DrawEllipse(x + r, y, 2 * r, r);
	dc.DrawEllipse(x + 3 * r, y, 2 * r, r);

	y += 2 * r;
	dc.DrawText("And arcs", 0, y);
	dc.DrawArc(x - r, y, x + r, y, x, y);
	dc.DrawArc(x + 4 * r, y, x + 2 * r, y, x + 3 * r, y);
	dc.DrawArc(x + 5 * r, y, x + 5 * r, y, x + 6 * r, y);

	y += 2 * r;
	dc.DrawEllipticArc(x - r, y, 2 * r, r, 0, 90);
	dc.DrawEllipticArc(x + r, y, 2 * r, r, 90, 180);
	dc.DrawEllipticArc(x + 3 * r, y, 2 * r, r, 180, 270);
	dc.DrawEllipticArc(x + 5 * r, y, 2 * r, r, 270, 360);

	// same as above, just transparent brush

	dc.SetPen(*wxRED_PEN);
	dc.SetBrush(*wxTRANSPARENT_BRUSH);

	y += 2 * r;
	dc.DrawText("Some circles", 0, y);
	dc.DrawCircle(x, y, r);
	dc.DrawCircle(x + 2 * r, y, r);
	dc.DrawCircle(x + 4 * r, y, r);

	y += 2 * r;
	dc.DrawText("And ellipses", 0, y);
	dc.DrawEllipse(x - r, y, 2 * r, r);
	dc.DrawEllipse(x + r, y, 2 * r, r);
	dc.DrawEllipse(x + 3 * r, y, 2 * r, r);

	y += 2 * r;
	dc.DrawText("And arcs", 0, y);
	dc.DrawArc(x - r, y, x + r, y, x, y);
	dc.DrawArc(x + 4 * r, y, x + 2 * r, y, x + 3 * r, y);
	dc.DrawArc(x + 5 * r, y, x + 5 * r, y, x + 6 * r, y);

	y += 2 * r;
	dc.DrawEllipticArc(x - r, y, 2 * r, r, 0, 90);
	dc.DrawEllipticArc(x + r, y, 2 * r, r, 90, 180);
	dc.DrawEllipticArc(x + 3 * r, y, 2 * r, r, 180, 270);
	dc.DrawEllipticArc(x + 5 * r, y, 2 * r, r, 270, 360);

}

void TechCanvas::DrawSplines(wxDC& dc)
{
#if wxUSE_SPLINES
	dc.DrawText("Some splines", 10, 5);

	// values are hardcoded rather than randomly generated
	// so the output can be compared between native
	// implementations on platforms with different random
	// generators

	const int R = 300;
	const wxPoint center(R + 20, R + 20);
	const int angles[7] = { 0, 10, 33, 77, 13, 145, 90 };
	const int radii[5] = { 100 , 59, 85, 33, 90 };
	const int numPoints = 200;
	wxPoint pts[numPoints];

	// background spline calculation
	unsigned int radius_pos = 0;
	unsigned int angle_pos = 0;
	int angle = 0;
	for (int i = 0; i < numPoints; i++)
	{
		angle += angles[angle_pos];
		int r = R * radii[radius_pos] / 100;
		pts[i].x = center.x + (wxCoord)(r * cos(M_PI * angle / 180.0));
		pts[i].y = center.y + (wxCoord)(r * sin(M_PI * angle / 180.0));

		angle_pos++;
		if (angle_pos >= WXSIZEOF(angles)) angle_pos = 0;

		radius_pos++;
		if (radius_pos >= WXSIZEOF(radii)) radius_pos = 0;
	}

	// background spline drawing
	dc.SetPen(*wxRED_PEN);
	dc.DrawSpline(WXSIZEOF(pts), pts);

	// less detailed spline calculation
	wxPoint letters[4][5];
	// w
	letters[0][0] = wxPoint(0, 1); //  O           O
	letters[0][1] = wxPoint(1, 3); //   *         *
	letters[0][2] = wxPoint(2, 2); //    *   O   *
	letters[0][3] = wxPoint(3, 3); //     * * * *
	letters[0][4] = wxPoint(4, 1); //      O   O
								   // x1
	letters[1][0] = wxPoint(5, 1); //  O*O
	letters[1][1] = wxPoint(6, 1); //     *
	letters[1][2] = wxPoint(7, 2); //      O
	letters[1][3] = wxPoint(8, 3); //       *
	letters[1][4] = wxPoint(9, 3); //        O*O
								   // x2
	letters[2][0] = wxPoint(5, 3); //        O*O
	letters[2][1] = wxPoint(6, 3); //       *
	letters[2][2] = wxPoint(7, 2); //      O
	letters[2][3] = wxPoint(8, 1); //     *
	letters[2][4] = wxPoint(9, 1); //  O*O
								   // W
	letters[3][0] = wxPoint(10, 0); //  O           O
	letters[3][1] = wxPoint(11, 3); //   *         *
	letters[3][2] = wxPoint(12, 1); //    *   O   *
	letters[3][3] = wxPoint(13, 3); //     * * * *
	letters[3][4] = wxPoint(14, 0); //      O   O

	const int dx = 2 * R / letters[3][4].x;
	const int h[4] = { -R / 2, 0, R / 4, R / 2 };

	for (int m = 0; m < 4; m++)
	{
		for (int n = 0; n < 5; n++)
		{
			letters[m][n].x = center.x - R + letters[m][n].x * dx;
			letters[m][n].y = center.y + h[letters[m][n].y];
		}

		dc.SetPen(wxPen(*wxBLUE, 1, wxPENSTYLE_DOT));
		dc.DrawLines(5, letters[m]);
		dc.SetPen(wxPen(*wxBLACK, 4));
		dc.DrawSpline(5, letters[m]);
	}

#else
	dc.DrawText("Splines not supported.", 10, 5);
#endif
}

void TechCanvas::DrawGradients(wxDC& dc)
{
	static const int TEXT_HEIGHT = 15;

	// LHS: linear
	wxRect r(10, 10, 50, 50);
	dc.DrawText("wxRIGHT", r.x, r.y);
	r.Offset(0, TEXT_HEIGHT);
	dc.GradientFillLinear(r, *wxWHITE, *wxBLUE, wxRIGHT);

	r.Offset(0, r.height + 10);
	dc.DrawText("wxLEFT", r.x, r.y);
	r.Offset(0, TEXT_HEIGHT);
	dc.GradientFillLinear(r, *wxWHITE, *wxBLUE, wxLEFT);

	r.Offset(0, r.height + 10);
	dc.DrawText("wxDOWN", r.x, r.y);
	r.Offset(0, TEXT_HEIGHT);
	dc.GradientFillLinear(r, *wxWHITE, *wxBLUE, wxDOWN);

	r.Offset(0, r.height + 10);
	dc.DrawText("wxUP", r.x, r.y);
	r.Offset(0, TEXT_HEIGHT);
	dc.GradientFillLinear(r, *wxWHITE, *wxBLUE, wxUP);

	wxRect  gfr = wxRect(r);

	// RHS: concentric
	r = wxRect(200, 10, 50, 50);
	dc.DrawText("Blue inside", r.x, r.y);
	r.Offset(0, TEXT_HEIGHT);
	dc.GradientFillConcentric(r, *wxBLUE, *wxWHITE);

	r.Offset(0, r.height + 10);
	dc.DrawText("White inside", r.x, r.y);
	r.Offset(0, TEXT_HEIGHT);
	dc.GradientFillConcentric(r, *wxWHITE, *wxBLUE);

	r.Offset(0, r.height + 10);
	dc.DrawText("Blue in top left corner", r.x, r.y);
	r.Offset(0, TEXT_HEIGHT);
	dc.GradientFillConcentric(r, *wxBLUE, *wxWHITE, wxPoint(0, 0));

	r.Offset(0, r.height + 10);
	dc.DrawText("Blue in bottom right corner", r.x, r.y);
	r.Offset(0, TEXT_HEIGHT);
	dc.GradientFillConcentric(r, *wxBLUE, *wxWHITE, wxPoint(r.width, r.height));

	// check that the area filled by the gradient is exactly the interior of
	// the rectangle
	r.x = 350;
	r.y = 30;
	dc.DrawText("The interior should be filled but", r.x, r.y);
	r.y += 15;
	dc.DrawText(" the red border should remain visible:", r.x, r.y);
	r.y += 15;

	r.width =
		r.height = 50;
	wxRect r2 = r;
	r2.x += 60;
	wxRect r3 = r;
	r3.y += 60;
	wxRect r4 = r2;
	r4.y += 60;
	dc.SetPen(*wxRED_PEN);
	dc.DrawRectangle(r);
	r.Deflate(1);
	dc.GradientFillLinear(r, *wxGREEN, *wxBLACK, wxNORTH);
	dc.DrawRectangle(r2);
	r2.Deflate(1);
	dc.GradientFillLinear(r2, *wxBLACK, *wxGREEN, wxSOUTH);
	dc.DrawRectangle(r3);
	r3.Deflate(1);
	dc.GradientFillLinear(r3, *wxGREEN, *wxBLACK, wxEAST);
	dc.DrawRectangle(r4);
	r4.Deflate(1);
	dc.GradientFillLinear(r4, *wxBLACK, *wxGREEN, wxWEST);

#if wxUSE_GRAPHICS_CONTEXT
	if (m_renderer)
	{
		wxGCDC                      &gdc = (wxGCDC&)dc;
		wxGraphicsContext           *gc = gdc.GetGraphicsContext();
		wxGraphicsPath              pth;
		wxGraphicsGradientStops     stops;
		double boxX, boxY, boxWidth, boxHeight;

		gfr.Offset(0, gfr.height + 10);
		dc.DrawText("Linear Gradient with Stops", gfr.x, gfr.y);
		gfr.Offset(0, TEXT_HEIGHT);

		stops = wxGraphicsGradientStops(*wxRED, *wxBLUE);
		stops.Add(wxColour(255, 255, 0), 0.33f);
		stops.Add(*wxGREEN, 0.67f);

		gc->SetBrush(gc->CreateLinearGradientBrush(gfr.x, gfr.y,
			gfr.x + gfr.width, gfr.y + gfr.height,
			stops));
		pth = gc->CreatePath();
		pth.MoveToPoint(gfr.x, gfr.y);
		pth.AddLineToPoint(gfr.x + gfr.width, gfr.y);
		pth.AddLineToPoint(gfr.x + gfr.width, gfr.y + gfr.height);
		pth.AddLineToPoint(gfr.x, gfr.y + gfr.height);
		pth.CloseSubpath();
		gc->FillPath(pth);
		pth.GetBox(&boxX, &boxY, &boxWidth, &boxHeight);
		dc.CalcBoundingBox(wxRound(boxX), wxRound(boxY));
		dc.CalcBoundingBox(wxRound(boxX + boxWidth), wxRound(boxY + boxHeight));

		wxGraphicsGradientStops simpleStops(*wxRED, *wxBLUE);

		gfr.Offset(0, gfr.height + 10);
		dc.DrawText("Radial Gradient from Red to Blue without intermediary Stops",
			gfr.x, gfr.y);
		gfr.Offset(0, TEXT_HEIGHT);

		gc->SetBrush(gc->CreateRadialGradientBrush(gfr.x + gfr.width / 2,
			gfr.y + gfr.height / 2,
			gfr.x + gfr.width / 2,
			gfr.y + gfr.height / 2,
			gfr.width / 2,
			simpleStops));

		pth = gc->CreatePath();
		pth.MoveToPoint(gfr.x, gfr.y);
		pth.AddLineToPoint(gfr.x + gfr.width, gfr.y);
		pth.AddLineToPoint(gfr.x + gfr.width, gfr.y + gfr.height);
		pth.AddLineToPoint(gfr.x, gfr.y + gfr.height);
		pth.CloseSubpath();
		gc->FillPath(pth);
		pth.GetBox(&boxX, &boxY, &boxWidth, &boxHeight);
		dc.CalcBoundingBox(wxRound(boxX), wxRound(boxY));
		dc.CalcBoundingBox(wxRound(boxX + boxWidth), wxRound(boxY + boxHeight));

		gfr.Offset(0, gfr.height + 10);
		dc.DrawText("Radial Gradient from Red to Blue with Yellow and Green Stops",
			gfr.x, gfr.y);
		gfr.Offset(0, TEXT_HEIGHT);

		gc->SetBrush(gc->CreateRadialGradientBrush(gfr.x + gfr.width / 2,
			gfr.y + gfr.height / 2,
			gfr.x + gfr.width / 2,
			gfr.y + gfr.height / 2,
			gfr.width / 2,
			stops));
		pth = gc->CreatePath();
		pth.MoveToPoint(gfr.x, gfr.y);
		pth.AddLineToPoint(gfr.x + gfr.width, gfr.y);
		pth.AddLineToPoint(gfr.x + gfr.width, gfr.y + gfr.height);
		pth.AddLineToPoint(gfr.x, gfr.y + gfr.height);
		pth.CloseSubpath();
		gc->FillPath(pth);
		pth.GetBox(&boxX, &boxY, &boxWidth, &boxHeight);
		dc.CalcBoundingBox(wxRound(boxX), wxRound(boxY));
		dc.CalcBoundingBox(wxRound(boxX + boxWidth), wxRound(boxY + boxHeight));

		gfr.Offset(0, gfr.height + 10);
		dc.DrawText("Linear Gradient with Stops and Gaps", gfr.x, gfr.y);
		gfr.Offset(0, TEXT_HEIGHT);

		stops = wxGraphicsGradientStops(*wxRED, *wxBLUE);
		stops.Add(wxColour(255, 255, 0), 0.33f);
		stops.Add(wxTransparentColour, 0.33f);
		stops.Add(wxTransparentColour, 0.67f);
		stops.Add(*wxGREEN, 0.67f);

		gc->SetBrush(gc->CreateLinearGradientBrush(gfr.x, gfr.y + gfr.height,
			gfr.x + gfr.width, gfr.y,
			stops));
		pth = gc->CreatePath();
		pth.MoveToPoint(gfr.x, gfr.y);
		pth.AddLineToPoint(gfr.x + gfr.width, gfr.y);
		pth.AddLineToPoint(gfr.x + gfr.width, gfr.y + gfr.height);
		pth.AddLineToPoint(gfr.x, gfr.y + gfr.height);
		pth.CloseSubpath();
		gc->FillPath(pth);
		pth.GetBox(&boxX, &boxY, &boxWidth, &boxHeight);
		dc.CalcBoundingBox(wxRound(boxX), wxRound(boxY));
		dc.CalcBoundingBox(wxRound(boxX + boxWidth), wxRound(boxY + boxHeight));

		gfr.Offset(0, gfr.height + 10);
		dc.DrawText("Radial Gradient with Stops and Gaps", gfr.x, gfr.y);
		gfr.Offset(0, TEXT_HEIGHT);

		gc->SetBrush(gc->CreateRadialGradientBrush(gfr.x + gfr.width / 2,
			gfr.y + gfr.height / 2,
			gfr.x + gfr.width / 2,
			gfr.y + gfr.height / 2,
			gfr.width / 2,
			stops));
		pth = gc->CreatePath();
		pth.MoveToPoint(gfr.x, gfr.y);
		pth.AddLineToPoint(gfr.x + gfr.width, gfr.y);
		pth.AddLineToPoint(gfr.x + gfr.width, gfr.y + gfr.height);
		pth.AddLineToPoint(gfr.x, gfr.y + gfr.height);
		pth.CloseSubpath();
		gc->FillPath(pth);
		pth.GetBox(&boxX, &boxY, &boxWidth, &boxHeight);
		dc.CalcBoundingBox(wxRound(boxX), wxRound(boxY));
		dc.CalcBoundingBox(wxRound(boxX + boxWidth), wxRound(boxY + boxHeight));

		gfr.Offset(0, gfr.height + 10);
		dc.DrawText("Gradients with Stops and Transparency", gfr.x, gfr.y);
		gfr.Offset(0, TEXT_HEIGHT);

		stops = wxGraphicsGradientStops(*wxRED, wxTransparentColour);
		stops.Add(*wxRED, 0.33f);
		stops.Add(wxTransparentColour, 0.33f);
		stops.Add(wxTransparentColour, 0.67f);
		stops.Add(*wxBLUE, 0.67f);
		stops.Add(*wxBLUE, 1.0f);

		pth = gc->CreatePath();
		pth.MoveToPoint(gfr.x, gfr.y);
		pth.AddLineToPoint(gfr.x + gfr.width, gfr.y);
		pth.AddLineToPoint(gfr.x + gfr.width, gfr.y + gfr.height);
		pth.AddLineToPoint(gfr.x, gfr.y + gfr.height);
		pth.CloseSubpath();

		gc->SetBrush(gc->CreateRadialGradientBrush(gfr.x + gfr.width / 2,
			gfr.y + gfr.height / 2,
			gfr.x + gfr.width / 2,
			gfr.y + gfr.height / 2,
			gfr.width / 2,
			stops));
		gc->FillPath(pth);

		stops = wxGraphicsGradientStops(wxColour(255, 0, 0, 128), wxColour(0, 0, 255, 128));
		stops.Add(wxColour(255, 255, 0, 128), 0.33f);
		stops.Add(wxColour(0, 255, 0, 128), 0.67f);

		gc->SetBrush(gc->CreateLinearGradientBrush(gfr.x, gfr.y,
			gfr.x + gfr.width, gfr.y,
			stops));
		gc->FillPath(pth);
		pth.GetBox(&boxX, &boxY, &boxWidth, &boxHeight);
		dc.CalcBoundingBox(wxRound(boxX), wxRound(boxY));
		dc.CalcBoundingBox(wxRound(boxX + boxWidth), wxRound(boxY + boxHeight));

		gfr.Offset(0, gfr.height + 10);
		dc.DrawText("Stroked path with a gradient pen", gfr.x, gfr.y);
		gfr.Offset(0, TEXT_HEIGHT);

		pth = gc->CreatePath();
		pth.MoveToPoint(gfr.x + gfr.width / 2, gfr.y);
		pth.AddLineToPoint(gfr.x + gfr.width, gfr.y + gfr.height / 2);
		pth.AddLineToPoint(gfr.x + gfr.width / 2, gfr.y + gfr.height);
		pth.AddLineToPoint(gfr.x, gfr.y + gfr.height / 2);
		pth.CloseSubpath();

		stops = wxGraphicsGradientStops(*wxRED, *wxBLUE);
		stops.Add(wxColour(255, 255, 0), 0.33f);
		stops.Add(*wxGREEN, 0.67f);

		wxGraphicsPen pen = gc->CreatePen(
			wxGraphicsPenInfo(wxColour(0, 0, 0)).Width(6).Join(wxJOIN_BEVEL).LinearGradient(
				gfr.x + gfr.width / 2, gfr.y,
				gfr.x + gfr.width / 2, gfr.y + gfr.height,
				stops));
		gc->SetPen(pen);
		gc->StrokePath(pth);
	}
#endif // wxUSE_GRAPHICS_CONTEXT
}

void TechCanvas::DrawSystemColours(wxDC& dc)
{
	wxFont mono(wxFontInfo().Family(wxFONTFAMILY_TELETYPE));
	wxSize textSize;
	{
		wxDCFontChanger setMono(dc, mono);
		textSize = dc.GetTextExtent("#01234567");
	}

	int lineHeight = textSize.GetHeight();
	wxRect r(textSize.GetWidth() + 10, 10, 100, lineHeight);

	wxString title = "System colours";

	const wxSystemAppearance appearance = wxSystemSettings::GetAppearance();
	const wxString appearanceName = appearance.GetName();
	if (!appearanceName.empty())
		title += wxString::Format(" for \"%s\"", appearanceName);
	if (appearance.IsDark())
		title += " (using dark system theme)";
	dc.DrawText(title, 10, r.y);
	r.y += 2 * lineHeight;
	dc.DrawText(wxString::Format("Window background is %s",
		appearance.IsUsingDarkBackground() ? "dark"
		: "light"),
		10, r.y);
	r.y += 3 * lineHeight;

	dc.SetPen(*wxTRANSPARENT_PEN);

	static const struct {
		wxSystemColour index;
		const char* name;
	} sysColours[] =
	{
		{ wxSYS_COLOUR_3DDKSHADOW, "wxSYS_COLOUR_3DDKSHADOW" },
		{ wxSYS_COLOUR_3DLIGHT, "wxSYS_COLOUR_3DLIGHT" },
		{ wxSYS_COLOUR_ACTIVEBORDER, "wxSYS_COLOUR_ACTIVEBORDER" },
		{ wxSYS_COLOUR_ACTIVECAPTION, "wxSYS_COLOUR_ACTIVECAPTION" },
		{ wxSYS_COLOUR_APPWORKSPACE, "wxSYS_COLOUR_APPWORKSPACE" },
		{ wxSYS_COLOUR_BTNFACE, "wxSYS_COLOUR_BTNFACE" },
		{ wxSYS_COLOUR_BTNHIGHLIGHT, "wxSYS_COLOUR_BTNHIGHLIGHT" },
		{ wxSYS_COLOUR_BTNSHADOW, "wxSYS_COLOUR_BTNSHADOW" },
		{ wxSYS_COLOUR_BTNTEXT, "wxSYS_COLOUR_BTNTEXT" },
		{ wxSYS_COLOUR_CAPTIONTEXT, "wxSYS_COLOUR_CAPTIONTEXT" },
		{ wxSYS_COLOUR_DESKTOP, "wxSYS_COLOUR_DESKTOP" },
		{ wxSYS_COLOUR_GRADIENTACTIVECAPTION, "wxSYS_COLOUR_GRADIENTACTIVECAPTION" },
		{ wxSYS_COLOUR_GRADIENTINACTIVECAPTION, "wxSYS_COLOUR_GRADIENTINACTIVECAPTION" },
		{ wxSYS_COLOUR_GRAYTEXT, "wxSYS_COLOUR_GRAYTEXT" },
		{ wxSYS_COLOUR_HIGHLIGHTTEXT, "wxSYS_COLOUR_HIGHLIGHTTEXT" },
		{ wxSYS_COLOUR_HIGHLIGHT, "wxSYS_COLOUR_HIGHLIGHT" },
		{ wxSYS_COLOUR_HOTLIGHT, "wxSYS_COLOUR_HOTLIGHT" },
		{ wxSYS_COLOUR_INACTIVEBORDER, "wxSYS_COLOUR_INACTIVEBORDER" },
		{ wxSYS_COLOUR_INACTIVECAPTIONTEXT, "wxSYS_COLOUR_INACTIVECAPTIONTEXT" },
		{ wxSYS_COLOUR_INACTIVECAPTION, "wxSYS_COLOUR_INACTIVECAPTION" },
		{ wxSYS_COLOUR_INFOBK, "wxSYS_COLOUR_INFOBK" },
		{ wxSYS_COLOUR_INFOTEXT, "wxSYS_COLOUR_INFOTEXT" },
		{ wxSYS_COLOUR_LISTBOXHIGHLIGHTTEXT, "wxSYS_COLOUR_LISTBOXHIGHLIGHTTEXT" },
		{ wxSYS_COLOUR_LISTBOXTEXT, "wxSYS_COLOUR_LISTBOXTEXT" },
		{ wxSYS_COLOUR_LISTBOX, "wxSYS_COLOUR_LISTBOX" },
		{ wxSYS_COLOUR_MENUBAR, "wxSYS_COLOUR_MENUBAR" },
		{ wxSYS_COLOUR_MENUHILIGHT, "wxSYS_COLOUR_MENUHILIGHT" },
		{ wxSYS_COLOUR_MENUTEXT, "wxSYS_COLOUR_MENUTEXT" },
		{ wxSYS_COLOUR_MENU, "wxSYS_COLOUR_MENU" },
		{ wxSYS_COLOUR_SCROLLBAR, "wxSYS_COLOUR_SCROLLBAR" },
		{ wxSYS_COLOUR_WINDOWFRAME, "wxSYS_COLOUR_WINDOWFRAME" },
		{ wxSYS_COLOUR_WINDOWTEXT, "wxSYS_COLOUR_WINDOWTEXT" },
		{ wxSYS_COLOUR_WINDOW, "wxSYS_COLOUR_WINDOW" }
	};

	for (int i = 0; i < wxSYS_COLOUR_MAX; i++)
	{
		wxString colourName(sysColours[i].name);
		wxColour c(wxSystemSettings::GetColour(sysColours[i].index));

		{
			wxDCFontChanger setMono(dc, mono);
			dc.DrawText(c.GetAsString(wxC2S_HTML_SYNTAX), 10, r.y);
		}

		dc.SetBrush(wxBrush(c));
		dc.DrawRectangle(r);

		dc.DrawText(colourName, r.GetRight() + 10, r.y);

		r.y += lineHeight;
	}
}

void TechCanvas::DrawRegions(wxDC& dc)
{
	dc.DrawText("You should see a red rect partly covered by a cyan one "
		"on the left", 10, 5);
	dc.DrawText("and 5 smileys from which 4 are partially clipped on the right",
		10, 5 + dc.GetCharHeight());
	dc.DrawText("The second copy should be identical but right part of it "
		"should be offset by 10 pixels.",
		10, 5 + 2 * dc.GetCharHeight());

	DrawRegionsHelper(dc, 10, true);
	DrawRegionsHelper(dc, 350, false);
}

void TechCanvas::DrawRegionsHelper(wxDC& dc, wxCoord x, bool firstTime)
{
	wxCoord y = 100;

	dc.DestroyClippingRegion();
	dc.SetBrush(*wxWHITE_BRUSH);
	dc.SetPen(*wxTRANSPARENT_PEN);
	dc.DrawRectangle(x, y, 310, 310);

	dc.SetClippingRegion(x + 10, y + 10, 100, 270);

	dc.SetBrush(*wxRED_BRUSH);
	dc.DrawRectangle(x, y, 310, 310);

	dc.SetClippingRegion(x + 10, y + 10, 100, 100);

	dc.SetBrush(*wxCYAN_BRUSH);
	dc.DrawRectangle(x, y, 310, 310);

	dc.DestroyClippingRegion();

	wxRegion region(x + 110, y + 20, 100, 270);
#if !defined(__WXMOTIF__)
	if (!firstTime)
		region.Offset(10, 10);
#endif
	dc.SetDeviceClippingRegion(region);

	dc.SetBrush(*wxGREY_BRUSH);
	dc.DrawRectangle(x, y, 310, 310);

	if (m_smile_bmp.IsOk())
	{
		dc.DrawBitmap(m_smile_bmp, x + 150, y + 150, true);
		dc.DrawBitmap(m_smile_bmp, x + 130, y + 10, true);
		dc.DrawBitmap(m_smile_bmp, x + 130, y + 280, true);
		dc.DrawBitmap(m_smile_bmp, x + 100, y + 70, true);
		dc.DrawBitmap(m_smile_bmp, x + 200, y + 70, true);
	}
}

void TechCanvas::GetDrawingSize(int* width, int* height) const
{
	if (width)
		*width = m_sizeX;

	if (height)
		*height = m_sizeY;
}

void TechCanvas::OnPaint(wxPaintEvent &WXUNUSED(event))
{
	if (m_useBuffer)
	{
		wxBufferedPaintDC bpdc(this);
		Draw(bpdc);
	}
	else
	{
		wxPaintDC pdc(this);
		Draw(pdc);
	}
}

void TechCanvas::Draw(wxDC& pdc)
{
#if wxUSE_GRAPHICS_CONTEXT
	wxGCDC gdc;

	if (m_renderer)
	{
		wxGraphicsContext* context;
		if (wxPaintDC *paintdc = wxDynamicCast(&pdc, wxPaintDC))
		{
			context = m_renderer->CreateContext(*paintdc);
		}
		else if (wxMemoryDC *memdc = wxDynamicCast(&pdc, wxMemoryDC))
		{
			context = m_renderer->CreateContext(*memdc);
		}
#if wxUSE_METAFILE && defined(wxMETAFILE_IS_ENH)
		else if (wxMetafileDC *metadc = wxDynamicCast(&pdc, wxMetafileDC))
		{
			context = m_renderer->CreateContext(*metadc);
		}
#endif
		else
		{
			wxFAIL_MSG("Unknown wxDC kind");
			return;
		}

		context->SetAntialiasMode(m_useAntiAliasing ? wxANTIALIAS_DEFAULT : wxANTIALIAS_NONE);

		gdc.SetBackground(GetBackgroundColour());
		gdc.SetGraphicsContext(context);
	}

	wxDC &dc = m_renderer ? static_cast<wxDC&>(gdc) : pdc;
#else
	wxDC &dc = pdc;
#endif

	// Adjust scrolled contents for screen drawing operations only.
	if (wxDynamicCast(&pdc, wxBufferedPaintDC) ||
		wxDynamicCast(&pdc, wxPaintDC))
	{
		PrepareDC(dc);
	}

	m_owner->PrepareDC(dc);

	dc.SetBackgroundMode(m_owner->m_backgroundMode);
	if (m_owner->m_backgroundBrush.IsOk())
		dc.SetBackground(m_owner->m_backgroundBrush);
	if (m_owner->m_colourForeground.IsOk())
		dc.SetTextForeground(m_owner->m_colourForeground);
	if (m_owner->m_colourBackground.IsOk())
		dc.SetTextBackground(m_owner->m_colourBackground);

	if (m_owner->m_textureBackground) {
		if (!m_owner->m_backgroundBrush.IsOk()) {
			dc.SetBackground(wxBrush(wxColour(0, 128, 0)));
		}
	}

	if (m_clip)
		dc.SetClippingRegion(100, 100, 100, 100);

	dc.Clear();

	if (m_owner->m_textureBackground)
	{
		dc.SetPen(*wxMEDIUM_GREY_PEN);
		for (int i = 0; i < 200; i++)
			dc.DrawLine(0, i * 10, i * 10, 0);
	}

	switch (m_show)
	{
	case File_ShowDefault:
		DrawDefault(dc);
		break;

	case File_ShowCircles:
		DrawCircles(dc);
		break;

	case File_ShowSplines:
		DrawSplines(dc);
		break;

	case File_ShowRegions:
		DrawRegions(dc);
		break;

	case File_ShowText:
		DrawText(dc);
		break;

	case File_ShowLines:
		DrawTestLines(0, 100, 0, dc);
		DrawTestLines(0, 320, 1, dc);
		DrawTestLines(0, 540, 2, dc);
		DrawTestLines(0, 760, 6, dc);
		DrawCrossHair(0, 0, 400, 90, dc);
		break;

	case File_ShowBrushes:
		DrawTestBrushes(dc);
		break;

	case File_ShowPolygons:
		DrawTestPoly(dc);
		break;

	case File_ShowMask:
		DrawImages(dc, Draw_Normal);
		break;

	case File_ShowMaskStretch:
		DrawImages(dc, Draw_Stretch);
		break;

	case File_ShowOps:
		DrawWithLogicalOps(dc);
		break;

#if wxDRAWING_DC_SUPPORTS_ALPHA || wxUSE_GRAPHICS_CONTEXT
	case File_ShowAlpha:
		DrawAlpha(dc);
		break;
#endif // wxDRAWING_DC_SUPPORTS_ALPHA || wxUSE_GRAPHICS_CONTEXT
#if wxUSE_GRAPHICS_CONTEXT
	case File_ShowGraphics:
		DrawGraphics(gdc.GetGraphicsContext());
		break;
#endif

	case File_ShowGradients:
		DrawGradients(dc);
		break;

	case File_ShowSystemColours:
		DrawSystemColours(dc);
		break;

	default:
		break;
	}

	// For drawing with raw wxGraphicsContext
	// there is no bounding box to obtain.
	if (m_showBBox
#if wxUSE_GRAPHICS_CONTEXT
		&& m_show != File_ShowGraphics
#endif // wxUSE_GRAPHICS_CONTEXT
		)
	{
		dc.SetPen(wxPen(wxColor(0, 128, 0), 1, wxPENSTYLE_DOT));
		dc.SetBrush(*wxTRANSPARENT_BRUSH);
		dc.DrawRectangle(dc.MinX(), dc.MinY(), dc.MaxX() - dc.MinX() + 1, dc.MaxY() - dc.MinY() + 1);
	}

	// Adjust drawing area dimensions only if screen drawing is invoked.
	if (wxDynamicCast(&pdc, wxBufferedPaintDC) ||
		wxDynamicCast(&pdc, wxPaintDC))
	{
		wxCoord x0, y0;
		dc.GetDeviceOrigin(&x0, &y0);
		m_sizeX = dc.LogicalToDeviceX(dc.MaxX()) - x0 + 1;
		m_sizeY = dc.LogicalToDeviceY(dc.MaxY()) - y0 + 1;
	}
}

void TechCanvas::OnMouseMove(wxMouseEvent &event)
{
#if wxUSE_STATUSBAR
	{
		wxClientDC dc(this);
		PrepareDC(dc);
		m_owner->PrepareDC(dc);

		wxPoint pos = event.GetPosition();
		long x = dc.DeviceToLogicalX(pos.x);
		long y = dc.DeviceToLogicalY(pos.y);
		wxString str;
		str.Printf("Current mouse position: %d,%d", (int)x, (int)y);
		m_owner->SetStatusText(str);
	}

	if (m_rubberBand)
	{
		int x, y, xx, yy;
		event.GetPosition(&x, &y);
		CalcUnscrolledPosition(x, y, &xx, &yy);
		m_currentpoint = wxPoint(xx, yy);
		wxRect newrect(m_anchorpoint, m_currentpoint);

		wxClientDC dc(this);
		PrepareDC(dc);

		wxDCOverlay overlaydc(m_overlay, &dc);
		overlaydc.Clear();
#ifdef __WXMAC__
		dc.SetPen(*wxGREY_PEN);
		dc.SetBrush(wxColour(192, 192, 192, 64));
#else
		dc.SetPen(wxPen(*wxLIGHT_GREY, 2));
		dc.SetBrush(*wxTRANSPARENT_BRUSH);
#endif
		dc.DrawRectangle(newrect);
	}
#else
	wxUnusedVar(event);
#endif // wxUSE_STATUSBAR
}

void TechCanvas::OnMouseDown(wxMouseEvent &event)
{
	int x, y, xx, yy;
	event.GetPosition(&x, &y);
	CalcUnscrolledPosition(x, y, &xx, &yy);
	m_anchorpoint = wxPoint(xx, yy);
	m_currentpoint = m_anchorpoint;
	m_rubberBand = true;
	CaptureMouse();
}

void TechCanvas::OnMouseUp(wxMouseEvent &event)
{
	if (m_rubberBand)
	{
		ReleaseMouse();
		{
			wxClientDC dc(this);
			PrepareDC(dc);
			wxDCOverlay overlaydc(m_overlay, &dc);
			overlaydc.Clear();
		}
		m_overlay.Reset();
		m_rubberBand = false;

		wxPoint endpoint = CalcUnscrolledPosition(event.GetPosition());

		// Don't pop up the message box if nothing was actually selected.
		if (endpoint != m_anchorpoint)
		{
			wxLogMessage("Selected rectangle from (%d, %d) to (%d, %d)",
				m_anchorpoint.x, m_anchorpoint.y,
				endpoint.x, endpoint.y);
		}
	}
}

#if wxUSE_GRAPHICS_CONTEXT
void TechCanvas::UseGraphicRenderer(wxGraphicsRenderer* renderer)
{
	m_renderer = renderer;
	if (renderer)
	{
		int major, minor, micro;
		renderer->GetVersion(&major, &minor, &micro);
		wxString str = wxString::Format("Graphics renderer: %s %i.%i.%i",
			renderer->GetName(), major, minor, micro);
		m_owner->SetStatusText(str, 1);
	}
	else
	{
		m_owner->SetStatusText(wxEmptyString, 1);
	}

	Refresh();
}
#endif // wxUSE_GRAPHICS_CONTEXT

