///////////////////////////////////////////////////////////////////////////////
// Name:        ribbondemo.cpp
// Purpose:     wxRibbon: Ribbon user interface - sample/test program
// Author:      Peter Cawley
// Modified by:
// Created:     2009-05-25
// Copyright:   (C) Copyright 2009, Peter Cawley
// Licence:     wxWindows Library Licence
///////////////////////////////////////////////////////////////////////////////

#include "app.h"
#include "taskbar.h"
#include "SmartKBDlg.h"
#include "frame.h"

// this is a definition so can't be in a header
wxDEFINE_EVENT(MY_EVENT, wxCommandEvent);
wxDEFINE_EVENT(ZQDB_CODE_FIELD_EVENT, wxCommandEvent);

wxBitmap *gs_bmpNoMask = NULL,
*gs_bmpWithColMask = NULL,
*gs_bmpMask = NULL,
*gs_bmpWithMask = NULL,
*gs_bmp4 = NULL,
*gs_bmp4_mono = NULL,
*gs_bmp36 = NULL;

void CB_LOG(int level, const char* logstr)
{

}

void CB_STATUS(HNNODE h, NET_NODE_STATUS status)
{

}

int CB_MSG(HNMSG hmsg)
{
	return 0;
}

void CB_NOTIFY(HMDB hdb, HMTABLE htb, MDB_NOTIFY_DATA* notify)
{
	wxGetApp().OnNotify(hdb, htb, notify);
}

//ZQDB_OUT_API_TABLE g_api = { 0 };
//
//void ZQDB_LOG(ZQDB_LOG_LEVEL level, const char* logstr)
//{
//    wxCommandEvent event(MY_EVENT, MyFrame::ID_MY_WINDOW);
//    event.SetString(logstr);
//    wxGetApp().GetTopWindow()->ProcessWindowEvent(event);
//}
//
//void ZQDB_NOTIFY(const char* key, ZQDB_NOTIFY notify)
//{
//    HZQDB h = g_api.OpenData(key);
//    if(h) {
//        printf("%s %d h = %p %d %p\n", key, notify, h, g_api.GetDataCount(h,nullptr), g_api.GetData(h,0));
//        g_api.CloseData(h);
//    }
//}

// -- implementations --

wxIMPLEMENT_APP(MyApp);

void MyApp::DeleteBitmaps()
{
	wxDELETE(gs_bmpNoMask);
	wxDELETE(gs_bmpWithColMask);
	wxDELETE(gs_bmpMask);
	wxDELETE(gs_bmpWithMask);
	wxDELETE(gs_bmp4);
	wxDELETE(gs_bmp4_mono);
	wxDELETE(gs_bmp36);
}

bool MyApp::LoadImages()
{
	gs_bmpNoMask = new wxBitmap;
	gs_bmpWithColMask = new wxBitmap;
	gs_bmpMask = new wxBitmap;
	gs_bmpWithMask = new wxBitmap;
	gs_bmp4 = new wxBitmap;
	gs_bmp4_mono = new wxBitmap;
	gs_bmp36 = new wxBitmap;

	wxPathList pathList;
	// special hack for Unix in-tree sample build, don't do this in real
	// programs, use wxStandardPaths instead
	pathList.Add(wxFileName(argv[0]).GetPath());
	pathList.Add(".");
	pathList.Add("..");
	pathList.Add("../drawing");
	pathList.Add("../../../samples/drawing");

	wxString path = pathList.FindValidPath("pat4.bmp");
	if (!path)
		return false;

	/* 4 colour bitmap */
	gs_bmp4->LoadFile(path, wxBITMAP_TYPE_BMP);
	/* turn into mono-bitmap */
	gs_bmp4_mono->LoadFile(path, wxBITMAP_TYPE_BMP);
	wxMask* mask4 = new wxMask(*gs_bmp4_mono, *wxBLACK);
	gs_bmp4_mono->SetMask(mask4);

	path = pathList.FindValidPath("pat36.bmp");
	if (!path)
		return false;
	gs_bmp36->LoadFile(path, wxBITMAP_TYPE_BMP);
	wxMask* mask36 = new wxMask(*gs_bmp36, *wxBLACK);
	gs_bmp36->SetMask(mask36);

	path = pathList.FindValidPath("image.bmp");
	if (!path)
		return false;
	gs_bmpNoMask->LoadFile(path, wxBITMAP_TYPE_BMP);
	gs_bmpWithMask->LoadFile(path, wxBITMAP_TYPE_BMP);
	gs_bmpWithColMask->LoadFile(path, wxBITMAP_TYPE_BMP);

	path = pathList.FindValidPath("mask.bmp");
	if (!path)
		return false;
	gs_bmpMask->LoadFile(path, wxBITMAP_TYPE_BMP);

	wxMask *mask = new wxMask(*gs_bmpMask, *wxBLACK);
	gs_bmpWithMask->SetMask(mask);

	mask = new wxMask(*gs_bmpWithColMask, *wxWHITE);
	gs_bmpWithColMask->SetMask(mask);

	return true;
}

void MyApp::LoadSkinInfo()
{
	wxScreenDC  dc;

	auto skin_info_ptr = std::make_shared<SkinInfo>();

	skin_info_ptr->curDragLeftRight = wxCursor(wxCURSOR_SIZEWE);
	skin_info_ptr->curDragUpDown = wxCursor(wxCURSOR_SIZENS);
	skin_info_ptr->curDrawLine = wxCursor(wxCURSOR_PENCIL);
	
	skin_info_ptr->crBackgnd.Set(7, 7, 7);		//背景
	skin_info_ptr->crTabSelBackgnd.Set(64, 0, 0);	//标签选中背景
	skin_info_ptr->crRptTitleBakcgnd.Set(27, 27, 27);//报表标题背景
	skin_info_ptr->crRptSelBackgnd.Set(0, 0, 128);	//报表选中背景

	skin_info_ptr->crTitle.Set(192, 192, 192);			//标题
	skin_info_ptr->crName.Set(192, 192, 192);			//名称
	skin_info_ptr->crText.Set(255, 255, 255);			//文字
	skin_info_ptr->crRising.Set(255, 62, 62);			//上涨
	skin_info_ptr->crFalling.Set(84, 252, 84);		//下跌
	skin_info_ptr->crCommodityCode.Set(80, 248, 255);	//代码
	skin_info_ptr->crCommodityName.Set(252, 252, 84);	//名称
	skin_info_ptr->crAmount.Set(252, 252, 84);			//价
	skin_info_ptr->crVolume.Set(252, 252, 84);			//量
	skin_info_ptr->crTabSel.Set(84, 252, 252);			//标签选中

	skin_info_ptr->crLine.Set(255, 255, 255);			//线
	skin_info_ptr->crAverageLine.Set(252, 252, 84);	//均线
	skin_info_ptr->crDrawLine.Set(255, 0, 255);		//画线
	skin_info_ptr->crXYLine.Set(60, 60, 60);			//X、Y分隔线
	skin_info_ptr->crXText.Set(125, 125, 125);			//X坐标文字
	skin_info_ptr->crYText.Set(125, 125, 125);//QCOLOR_DEF_YAXIS_VALUE;			//Y坐标文字
	skin_info_ptr->crCrossCursor.Set(122, 122, 122);	//十字游标	
	skin_info_ptr->crRptLine.Set(81, 81, 81);		//报表线
	skin_info_ptr->crRisingLine.Set(255, 52, 52);		//上涨线
	skin_info_ptr->crFallingLine.Set(84, 252, 252);	//下跌线
	skin_info_ptr->crILine[0].Set(252, 252, 252);
	skin_info_ptr->crILine[1].Set(252, 252, 84);
	skin_info_ptr->crILine[2].Set(252, 84, 252);
	skin_info_ptr->crILine[3].Set(84, 232, 61);
	skin_info_ptr->crILine[4].Set(255, 109, 0);
	skin_info_ptr->crILine[5].Set(185, 120, 234);
	skin_info_ptr->crILine[6].Set(107, 142, 212);
	skin_info_ptr->crILine[7].Set(164, 164, 137);
	skin_info_ptr->crRefline.Set(60, 60, 60);
	skin_info_ptr->crOrderLine.Set(60, 60, 60);

	skin_info_ptr->penLine = wxPen(skin_info_ptr->crLine, 1);			//线
	skin_info_ptr->penAverageLine = wxPen(skin_info_ptr->crAverageLine, 1);	//均线
	//skin_info_ptr->penDrawLine = wxPen(skin_info_ptr->crDrawLine, 1);		//画线
	skin_info_ptr->penXYLine = wxPen(skin_info_ptr->crXYLine, 1);		//X、Y分隔线
	skin_info_ptr->penXText = wxPen(skin_info_ptr->crXText, 1);			//X坐标文字
	skin_info_ptr->penYText = wxPen(skin_info_ptr->crYText, 1);			//Y坐标文字
	//skin_info_ptr->penCrossCursor = wxPen(skin_info_ptr->crCrossCursor, 1);	//十字游标	
	skin_info_ptr->penRptLine = wxPen(skin_info_ptr->crRptLine, 1);		//报表线
	skin_info_ptr->penRisingLine = wxPen(skin_info_ptr->crRisingLine, 1);	//上涨线
	skin_info_ptr->penFallingLine = wxPen(skin_info_ptr->crFallingLine, 1);	//下跌线
	skin_info_ptr->penILine[0] = wxPen(skin_info_ptr->crILine[0], 1);		//指标线
	skin_info_ptr->penILine[1] = wxPen(skin_info_ptr->crILine[1], 1);		//指标线
	skin_info_ptr->penILine[2] = wxPen(skin_info_ptr->crILine[2], 1);		//指标线
	skin_info_ptr->penILine[3] = wxPen(skin_info_ptr->crILine[3], 1);		//指标线
	skin_info_ptr->penILine[4] = wxPen(skin_info_ptr->crILine[4], 1);		//指标线
	skin_info_ptr->penILine[5] = wxPen(skin_info_ptr->crILine[5], 1);		//指标线
	skin_info_ptr->penILine[6] = wxPen(skin_info_ptr->crILine[6], 1);		//指标线
	skin_info_ptr->penILine[7] = wxPen(skin_info_ptr->crILine[7], 1);		//指标线
	skin_info_ptr->penRefline = wxPen(skin_info_ptr->crRefline, 0, wxPENSTYLE_DOT);		//参考线
	skin_info_ptr->penOrderLine = wxPen(skin_info_ptr->crOrderLine, 1);		//订单线
	skin_info_ptr->pen = wxPen(skin_info_ptr->crLine, 1);
	skin_info_ptr->penRising = wxPen(skin_info_ptr->crRisingLine, 1);
	skin_info_ptr->penFalling = wxPen(skin_info_ptr->crFallingLine, 1);
	skin_info_ptr->penDrawLine = wxPen(skin_info_ptr->crDrawLine, 1);
	skin_info_ptr->penCrossCursor = wxPen(wxColour(80, 80, 80), 0);
	skin_info_ptr->penYCrossCursor = wxPen(wxColour(60, 60, 60), 0);
	skin_info_ptr->penNow = wxPen(wxColour(168, 168, 168), 1);
	skin_info_ptr->penOrder = wxPen(skin_info_ptr->crOrderLine, 1, wxPENSTYLE_DOT_DASH);

	skin_info_ptr->brush = wxBrush(skin_info_ptr->crLine);
	skin_info_ptr->brushRising = wxBrush(skin_info_ptr->crRisingLine);
	skin_info_ptr->brushFalling = wxBrush(skin_info_ptr->crFallingLine);
	skin_info_ptr->brushDrawLine = wxBrush(skin_info_ptr->crDrawLine);
	skin_info_ptr->brushNull = *wxTRANSPARENT_BRUSH;

	wxFont font = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
	skin_info_ptr->fontName = wxFont(14, font.GetFamily(), font.GetStyle(), font.GetWeight(), font.GetUnderlined()
		, wxT("宋体"));
	skin_info_ptr->fontText = wxFont(14, font.GetFamily(), font.GetStyle(), font.GetWeight(), font.GetUnderlined()
		, wxT("system"));
	skin_info_ptr->fontTabTitle = wxFont(13, font.GetFamily(), font.GetStyle(), font.GetWeight(), font.GetUnderlined()
		, wxT("宋体"));
	skin_info_ptr->fontRptTitle = wxFont(13, font.GetFamily(), font.GetStyle(), font.GetWeight(), font.GetUnderlined()
		, wxT("宋体"));
	skin_info_ptr->fontRptText = wxFont(13, font.GetFamily(), font.GetStyle(), font.GetWeight(), font.GetUnderlined()
		, wxT("宋体"));
	skin_info_ptr->fontTechTitle = wxFont(12, font.GetFamily(), font.GetStyle(), font.GetWeight(), font.GetUnderlined()
		, wxT("宋体"));
	skin_info_ptr->fontXText = wxFont(13, font.GetFamily(), font.GetStyle(), font.GetWeight(), font.GetUnderlined()
		, wxT("宋体"));
	skin_info_ptr->fontYText = wxFont(13, font.GetFamily(), font.GetStyle(), font.GetWeight(), font.GetUnderlined()
		, wxT("宋体"));
	skin_info_ptr->fontOrder = wxFont(13, font.GetFamily(), font.GetStyle(), font.GetWeight(), font.GetUnderlined()
		, wxT("宋体"));

	wxString strText = wxT("高速行情交易系统");
	dc.SetFont(skin_info_ptr->fontName);
	skin_info_ptr->xyName = dc.GetTextExtent(strText);
	skin_info_ptr->xyName.x /= strText.Length();
	dc.SetFont(skin_info_ptr->fontText);
	skin_info_ptr->xyText = dc.GetTextExtent(strText);
	skin_info_ptr->xyText.x /= strText.Length();
	dc.SetFont(skin_info_ptr->fontTabTitle);
	skin_info_ptr->xyTabTitle = dc.GetTextExtent(strText);
	skin_info_ptr->xyTabTitle.x /= strText.Length();
	dc.SetFont(skin_info_ptr->fontRptTitle);
	skin_info_ptr->xyRptTitle = dc.GetTextExtent(strText);
	skin_info_ptr->xyRptTitle.x /= strText.Length();
	dc.SetFont(skin_info_ptr->fontRptText);
	skin_info_ptr->xyRptText = dc.GetTextExtent(strText);
	skin_info_ptr->xyRptText.x /= strText.Length();
	dc.SetFont(skin_info_ptr->fontTechTitle);
	skin_info_ptr->xyTechTitle = dc.GetTextExtent(strText);
	skin_info_ptr->xyTechTitle.x /= strText.Length();
	dc.SetFont(skin_info_ptr->fontXText);
	skin_info_ptr->xyXText = dc.GetTextExtent(strText);
	skin_info_ptr->xyXText.x /= strText.Length();
	dc.SetFont(skin_info_ptr->fontYText);
	skin_info_ptr->xyYText = dc.GetTextExtent(strText);
	skin_info_ptr->xyYText.x /= strText.Length();
	dc.SetFont(skin_info_ptr->fontOrder);
	skin_info_ptr->xyOrder = dc.GetTextExtent(strText);
	skin_info_ptr->xyOrder.x /= strText.Length();
	skin_info_ptr->xySpace.x = 2;
	skin_info_ptr->xySpace.y = 3;

	skin_info_ptr->xyTabCtrl.x = 18;
	skin_info_ptr->xyTabCtrl.y = 18;
	skin_info_ptr->xyScrollBar.x = 18;
	skin_info_ptr->xyScrollBar.y = 18;
	skin_info_ptr->xyWndIndicator.x = 100;
	skin_info_ptr->xyWndIndicator.y = 100;
	skin_info_ptr->xyInfoIndicator.x = skin_info_ptr->xyXText.x * 5;
	skin_info_ptr->xyInfoIndicator.y = 24;
	skin_info_ptr->xyCoordinate.x = 18;
	skin_info_ptr->xyCoordinate.y = 18;


	skin_info_ptr->nBarWidth[0] = 1;
	skin_info_ptr->nBarWidth[1] = 1;
	skin_info_ptr->nBarWidth[2] = 1;
	skin_info_ptr->nBarWidth[3] = 1;
	skin_info_ptr->nBarWidth[4] = 3;
	skin_info_ptr->nBarWidth[5] = 5;
	skin_info_ptr->nBarWidth[6] = 7;
	skin_info_ptr->nBarWidth[7] = 9;
	skin_info_ptr->nBarWidth[8] = 12;
	skin_info_ptr->nBarSpace[0] = 0;
	skin_info_ptr->nBarSpace[1] = 1;
	skin_info_ptr->nBarSpace[2] = 2;
	skin_info_ptr->nBarSpace[3] = 3;
	skin_info_ptr->nBarSpace[4] = 3;
	skin_info_ptr->nBarSpace[5] = 3;
	skin_info_ptr->nBarSpace[6] = 3;
	skin_info_ptr->nBarSpace[7] = 3;
	skin_info_ptr->nBarSpace[8] = 3;
	skin_info_ptr->nBarScale = 5;
	skin_info_ptr->nBarShift = 12;

	SetSkinInfo(skin_info_ptr);
}

bool MyApp::OnInit()
{
    if(!wxApp::OnInit())
        return false;

#if wxUSE_LIBPNG
	wxImage::AddHandler(new wxPNGHandler);
#endif

	if (!wxTaskBarIcon::IsAvailable())
	{
		wxMessageBox
		(
			"There appears to be no system tray support in your current environment. This sample may not behave as expected.",
			"Warning",
			wxOK | wxICON_EXCLAMATION
		);
	}

	LoadSkinInfo();

	taskbaricon_ = new MyTaskBarIcon();

	// we should be able to show up to 128 characters on Windows
	if (!taskbaricon_->SetIcon(wxICON(sample),
		"wxTaskBarIcon Sample\n"
		"With a very, very, very, very\n"
		"long tooltip whose length is\n"
		"greater than 64 characters."))
	{
		wxLogError("Could not set icon.");
	}

#if defined(__WXOSX__) && wxOSX_USE_COCOA
	dockicon_ = new MyTaskBarIcon(wxTBI_DOCK);
	if (!dockicon_->SetIcon(wxICON(sample)))
	{
		wxLogError("Could not set icon.");
	}
#endif

	auto path_module_dir = boost::dll::this_line_location().remove_leaf();
	auto path_desc_file = path_module_dir;
	path_desc_file.append("zqapp").append("desc.json");
	auto desc_file = path_desc_file.string();
	CFG_FROM_XML(cfg, desc_file.c_str(), XUtil::XML_FLAG_JSON_FILE);
	ZQDB_INF zqdb_inf = { XUtil::XML_FLAG_PTREE, (const char*)&cfg, &CB_LOG, &CB_STATUS, &CB_MSG, &CB_NOTIFY };
	ZQCalcStart(&zqdb_inf);
    //ZQDB_IN_API_TABLE in = { XUtil::XML_FLAG_JSON_STRING, R"({ "node":{ "name":"zqdbsrv", "type":"SRV" }, "ZQDB": { "name": "ZQDB.ctp.db", "max_mem_size": 1000000000 }, "upstream": { "host": "localhost", "port": 6660, "mode": "IPC" }, "downstream": { "host": "0.0.0.0", "port": 6666 } })" };
    //in.cb = &ZQDB_NOTIFY;
    //ZQDB_API_Init(&in, &g_api);

	//第一次需要调用一下，这样才会创建计算模块
	ZQDBUpdateCalc();

	// Create the main window
	smartkbdlg_ = new SmartKBDlg();
	smartkbdlg_->Show(false);

	techfrm_ = new MyFrame;
	techfrm_->Show(false);

    // Create the main frame window
    //MyFrame *frame = new MyFrame;
    //frame->Show(true);

	Bind(ZQDB_CODE_FIELD_EVENT, &MyApp::OnCodeField, this);

    return true;
}

int MyApp::OnExit()
{
	DeleteBitmaps();

	ZQCalcStop();

    return wxApp::OnExit();
}

void MyApp::Exit()
{
	if (techfrm_) {
		delete techfrm_;
		wxASSERT(techfrm_ == nullptr);
	}
	if (smartkbdlg_) {
		delete smartkbdlg_;
		wxASSERT(smartkbdlg_ == nullptr);
	}
	if (taskbaricon_) {
		taskbaricon_->Destroy();
		taskbaricon_ = nullptr;
	}
}

void MyApp::OnCodeField(wxCommandEvent& event)
{
	ZQDBUpdateCalc();

	if (techfrm_) {
		techfrm_->GetEventHandler()->ProcessEvent(event);
	}
}

void MyApp::OnNotify(HMDB hdb, HMTABLE htb, MDB_NOTIFY_DATA* notify)
{
	/*switch (notify->notify)
	{
	case NOTIFY_UNKNOWN:
		break;
	case NOTIFY_DELETE:
		break;
	case NOTIFY_INIT:
		break;
	case NOTIFY_META:
		break;
	case NOTIFY_ATTR:
		break;
	case NOTIFY_INSERT:
		break;
	case NOTIFY_UPDATE:
		break;
	case NOTIFY_APPEND:
		break;
	case NOTIFY_REMOVE:
		break;
	case NOTIFY_UPDATE_FIELD:
	{
		auto notify_data = (MDB_NOTIFY_UPDATE_FIELD*)notify;
		auto product = ZQDBNotifyIsProduct(hdb, htb, notify);
		if (product) {
			for (size_t i = 0; i < notify_data->elem_num; i++)
			{
				auto code = ZQDBNotifyGetCode(product, notify_data->pos + i);
				if (code) {
					auto event = new wxCommandEvent(ZQDB_CODE_FIELD_EVENT);
					event->SetClientData(code);
					wxGetApp().QueueEvent(event);
				}
			}
		}
	}
	break;
	default:
		break;
	}*/
}
