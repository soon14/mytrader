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

#include <view.h>
#include "base.h"
#include "settings.h"

class MyCanvas : public wxScrolledWindow
{
public:
	MyCanvas(wxWindow* parent, bool mirror);
	virtual ~MyCanvas() {}

	virtual void OnDraw(wxDC& dc) wxOVERRIDE;

private:
	bool m_mirror;

	wxDECLARE_NO_COPY_CLASS(MyCanvas);
};

template<class T>
class BaseDataMap
{
protected:
	HZQDB base_;
public:
	void OnBaseDataChange() { }
	void SetBaseData(HZQDB h) {
		T* pT = static_cast<T*>(this);
		base_ = h;
		pT->OnBaseDataChange();
	}
};

class TechChild;

// define a scrollable canvas for drawing onto
class TechCanvas : public wxScrolledWindow
{
public:
	TechCanvas(TechChild *parent);

	void OnPaint(wxPaintEvent &event);
	void OnMouseMove(wxMouseEvent &event);
	void OnMouseDown(wxMouseEvent &event);
	void OnMouseUp(wxMouseEvent &event);

	void ToShow(int show) { m_show = show; Refresh(); }
	int GetPage() { return m_show; }

	// set or remove the clipping region
	void Clip(bool clip) { m_clip = clip; Refresh(); }
#if wxUSE_GRAPHICS_CONTEXT
	bool HasRenderer() const { return m_renderer != NULL; }
	void UseGraphicRenderer(wxGraphicsRenderer* renderer);
	bool IsDefaultRenderer() const
	{
		if (!m_renderer) return false;
		return m_renderer == wxGraphicsRenderer::GetDefaultRenderer();
	}
	wxGraphicsRenderer* GetRenderer() const { return m_renderer; }
	void EnableAntiAliasing(bool use) { m_useAntiAliasing = use; Refresh(); }
#endif // wxUSE_GRAPHICS_CONTEXT
	void UseBuffer(bool use) { m_useBuffer = use; Refresh(); }
	void ShowBoundingBox(bool show) { m_showBBox = show; Refresh(); }
	void GetDrawingSize(int* width, int* height) const;

	void Draw(wxDC& dc);

protected:
	enum DrawMode
	{
		Draw_Normal,
		Draw_Stretch
	};

	void DrawTestLines(int x, int y, int width, wxDC &dc);
	void DrawCrossHair(int x, int y, int width, int heigth, wxDC &dc);
	void DrawTestPoly(wxDC& dc);
	void DrawTestBrushes(wxDC& dc);
	void DrawText(wxDC& dc);
	void DrawImages(wxDC& dc, DrawMode mode);
	void DrawWithLogicalOps(wxDC& dc);
#if wxDRAWING_DC_SUPPORTS_ALPHA || wxUSE_GRAPHICS_CONTEXT
	void DrawAlpha(wxDC& dc);
#endif
#if wxUSE_GRAPHICS_CONTEXT
	void DrawGraphics(wxGraphicsContext* gc);
#endif
	void DrawRegions(wxDC& dc);
	void DrawCircles(wxDC& dc);
	void DrawSplines(wxDC& dc);
	void DrawDefault(wxDC& dc);
	void DrawGradients(wxDC& dc);
	void DrawSystemColours(wxDC& dc);

	void DrawRegionsHelper(wxDC& dc, wxCoord x, bool firstTime);

private:
	TechChild *m_owner;

	int          m_show;
	wxBitmap     m_smile_bmp;
	wxIcon       m_std_icon;
	bool         m_clip;
	wxOverlay    m_overlay;
	bool         m_rubberBand;
	wxPoint      m_anchorpoint;
	wxPoint      m_currentpoint;
#if wxUSE_GRAPHICS_CONTEXT
	wxGraphicsRenderer* m_renderer;
	bool         m_useAntiAliasing;
#endif
	bool         m_useBuffer;
	bool         m_showBBox;
	wxCoord      m_sizeX;
	wxCoord      m_sizeY;

	wxDECLARE_EVENT_TABLE();
};
