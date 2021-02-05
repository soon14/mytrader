#include "myapp.h"
#include "myframe.h"
#include "mysmartkbdlg.h"

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
//Navigate
//EVT_RIBBONBUTTONBAR_CLICKED(ID_NAVIGATE_HOME, MyFrame::OnNavigateHome)
EVT_RIBBONBUTTONBAR_CLICKED(ID_NAVIGATE_BACKWARD, MyFrame::OnNavigateBackward)
EVT_RIBBONBUTTONBAR_DROPDOWN_CLICKED(ID_NAVIGATE_BACKWARD, MyFrame::OnNavigateBackwardDropdown)
EVT_UPDATE_UI(ID_NAVIGATE_BACKWARD, MyFrame::OnNavigateBackwardUpdateUI)
//EVT_RIBBONBUTTONBAR_CLICKED(ID_NAVIGATE_FORWARD, MyFrame::OnNavigateForward)
//EVT_RIBBONBUTTONBAR_DROPDOWN_CLICKED(ID_NAVIGATE_FORWARD, MyFrame::OnNavigateForwardDropdown)
//EVT_RIBBONBUTTONBAR_CLICKED(ID_NAVIGATE_UP, MyFrame::OnNavigateUp)
//EVT_RIBBONBUTTONBAR_CLICKED(ID_NAVIGATE_DOWN, MyFrame::OnNavigateDown)
//EVT_RIBBONBUTTONBAR_CLICKED(ID_NAVIGATE_FIND, MyFrame::OnNavigateFind)
//Market
//EVT_RIBBONBUTTONBAR_CLICKED(ID_MARKET_SELF, MyFrame::OnMarket)
//EVT_RIBBONBUTTONBAR_CLICKED(ID_MARKET_MAIN, MyFrame::OnMarketMain)
//EVT_RIBBONBUTTONBAR_CLICKED(ID_MARKET_ALL, MyFrame::OnMarketAll)
////Trade
//EVT_RIBBONBUTTONBAR_CLICKED(ID_TRADE_QUICK_BUY_FAK, MyFrame::OnTradeBuyFAK)
//EVT_RIBBONBUTTONBAR_CLICKED(ID_TRADE_QUICK_BUY_FOK, MyFrame::OnTradeBuyFOK)
//EVT_RIBBONBUTTONBAR_CLICKED(ID_TRADE_QUICK_SELL_FAK, MyFrame::OnTradeSellFAK)
//EVT_RIBBONBUTTONBAR_CLICKED(ID_TRADE_QUICK_SELL_FOK, MyFrame::OnTradeSellFOK)
//EVT_RIBBONBUTTONBAR_CLICKED(ID_TRADE_QUICK_CLOSE, MyFrame::OnTradeClose)
//EVT_RIBBONBUTTONBAR_CLICKED(ID_TRADE_POSITION, MyFrame::OnTradePosition)
//EVT_RIBBONBUTTONBAR_CLICKED(ID_TRADE_ORDER, MyFrame::OnTradeOrder)
//EVT_RIBBONBUTTONBAR_CLICKED(ID_TRADE_TRADE, MyFrame::OnTradeTrade)
//EVT_RIBBONBUTTONBAR_CLICKED(ID_TRADE_ADDUSER, MyFrame::OnTradeAddUser)
//EVT_RIBBONBUTTONBAR_CLICKED(ID_TRADE_REMOVEUSER, MyFrame::OnTradeRemoveUser)
EVT_SIZE(MyFrame::OnSizeEvent)
//EVT_CLOSE(MyFrame::OnCloseEvent)
EVT_TIMER(wxID_ANY, MyFrame::OnTimer)
wxEND_EVENT_TABLE()
//
//wxBEGIN_EVENT_TABLE(MyFrame::EventHandler, wxEvtHandler)
//EVT_CHAR(MyFrame::EventHandler::OnChar)
//wxEND_EVENT_TABLE()
//
//MyFrame::EventHandler::EventHandler(MyFrame* frame)
//{
//	wxWindowList children = frame->GetChildren();
//	for (wxWindowListNode *i = children.GetFirst(); i; i = i->GetNext()) {
//		SetChildNextHandler(i->GetData());
//	}
//}
//
//void MyFrame::EventHandler::SetChildNextHandler(wxWindow* child)
//{
//	if (child) {
//		child->SetNextHandler(this);
//		wxWindowList children = child->GetChildren();
//		for (wxWindowListNode *i = children.GetFirst(); i; i = i->GetNext()) {
//			SetChildNextHandler(i->GetData());
//		}
//	}
//}
//
//void MyFrame::EventHandler::OnChar(wxKeyEvent &event)
//{
//	event.Skip();
//}

#include "transparent.xpm"
//#include "zoom_in.xpm"
//#include "zoom_out.xpm"

MyFrame::MyFrame(const char* xml, size_t xmlflag)
    : wxFrame(NULL, wxID_ANY, "wxRibbon Sample Application", wxDefaultPosition, wxSize(800, 600), wxDEFAULT_FRAME_STYLE)
	, timer_(this)
{
	CFG_FROM_XML(cfg, xml, xmlflag);

	wxCSConv utf8cv(wxFONTENCODING_UTF8);

	zqdb::AllUser alluser;
	for (size_t i = 0; i < alluser.size(); i++)
	{
		h_ = alluser[0];
		break;
	}

#if wxUSE_STATUSBAR
	const int widths[] = { -1, 60, 60, 60, 60 };
	CreateStatusBar(5);
	SetStatusWidths(5, widths);
	//SetStatusText("no selection", 0);
#endif // wxUSE_STATUSBAR

	//开始
	//后退 上一个 | 搜索  市场 品种 ... | 模板1 模板2 2图 3图 4图  | 快买 持仓 | 官网
	//后退 下一个 | 主力 市场 ... 品种 ... |   最前 前移 后移 最后 放大 缩小  | 快卖 委托       | 关于
	//后退 前进进 | 自选                  | 周期  ... 划线 ... | 快平 成交 | 帮助
	//导航         行情               | 分析                  | 交易                | 其他
    m_ribbon = new wxRibbonBar(this,-1,wxDefaultPosition, wxDefaultSize, wxRIBBON_BAR_FLOW_HORIZONTAL
                                | wxRIBBON_BAR_SHOW_PAGE_LABELS
                                | wxRIBBON_BAR_SHOW_PANEL_EXT_BUTTONS
                                | wxRIBBON_BAR_SHOW_TOGGLE_BUTTON
                                | wxRIBBON_BAR_SHOW_HELP_BUTTON
                                );

	{
		/*int metric_vals[wxRIBBON_ART_GALLERY_BITMAP_PADDING_BOTTOM_SIZE - wxRIBBON_ART_TAB_SEPARATION_SIZE + 1] = { 0 };
		for (size_t id = wxRIBBON_ART_TAB_SEPARATION_SIZE; id <= wxRIBBON_ART_GALLERY_BITMAP_PADDING_BOTTOM_SIZE; id++)
		{
			metric_vals[id - wxRIBBON_ART_TAB_SEPARATION_SIZE] = m_ribbon->GetArtProvider()->GetMetric(id);
		}
		for (size_t id = wxRIBBON_ART_TAB_SEPARATION_SIZE; id <= wxRIBBON_ART_GALLERY_BITMAP_PADDING_BOTTOM_SIZE; id++)
		{
			if (metric_vals[id - wxRIBBON_ART_TAB_SEPARATION_SIZE] < 10) {
				metric_vals[id - wxRIBBON_ART_TAB_SEPARATION_SIZE] = 10;
			}
			m_ribbon->GetArtProvider()->SetMetric(id, metric_vals[id - wxRIBBON_ART_TAB_SEPARATION_SIZE]);
		}*/
		wxRibbonPage* home = new wxRibbonPage(m_ribbon, wxID_ANY, "开始");
		
		auto navigate_panel = new wxRibbonPanel(home, wxID_ANY, "导航");
		wxRibbonButtonBar *navigate_bar = new wxRibbonButtonBar(navigate_panel);
		navigate_bar->AddHybridButton(ID_NAVIGATE_BACKWARD, "返回"
			, wxArtProvider::GetBitmap(wxART_GO_BACK, wxART_OTHER, wxSize(24, 24)), "回退到前面看过的代码");
		navigate_bar->SetButtonTextMinWidth(ID_NAVIGATE_BACKWARD, wxT("上一个"));
		navigate_bar->SetButtonMaxSizeClass(ID_NAVIGATE_BACKWARD, wxRIBBON_BUTTONBAR_BUTTON_LARGE);
		navigate_bar->SetButtonMinSizeClass(ID_NAVIGATE_BACKWARD, wxRIBBON_BUTTONBAR_BUTTON_LARGE);
		navigate_bar->AddHybridButton(ID_NAVIGATE_FORWARD, "前进"
			, wxArtProvider::GetBitmap(wxART_GO_FORWARD, wxART_OTHER, wxSize(24, 24)), "前进到前面看过的代码");
		navigate_bar->SetButtonTextMinWidth(ID_NAVIGATE_FORWARD, wxT("上一个"));
		navigate_bar->SetButtonMaxSizeClass(ID_NAVIGATE_FORWARD, wxRIBBON_BUTTONBAR_BUTTON_LARGE);
		navigate_bar->SetButtonMinSizeClass(ID_NAVIGATE_FORWARD, wxRIBBON_BUTTONBAR_BUTTON_LARGE);
		/*navigate_bar->AddHybridButton(ID_NAVIGATE_FIND, "搜索…"
			, wxArtProvider::GetBitmap(wxART_FIND, wxART_OTHER, wxSize(16, 16))
			, "键盘精灵搜索…,（ALT+V）快捷键键盘精灵搜索将创建新窗口…");
		navigate_bar->SetButtonTextMinWidth(ID_NAVIGATE_FIND, wxT("上一个"));
		navigate_bar->SetButtonMaxSizeClass(ID_NAVIGATE_FIND, wxRIBBON_BUTTONBAR_BUTTON_MEDIUM);
		navigate_bar->SetButtonMinSizeClass(ID_NAVIGATE_FIND, wxRIBBON_BUTTONBAR_BUTTON_MEDIUM);
		navigate_bar->AddButton(ID_NAVIGATE_UP, "上一个"
			, wxArtProvider::GetBitmap(wxART_GO_UP, wxART_OTHER, wxSize(16, 16))
			, "上一个代码");
		navigate_bar->SetButtonMaxSizeClass(ID_NAVIGATE_UP, wxRIBBON_BUTTONBAR_BUTTON_MEDIUM);
		navigate_bar->SetButtonMinSizeClass(ID_NAVIGATE_UP, wxRIBBON_BUTTONBAR_BUTTON_MEDIUM);
		navigate_bar->AddButton(ID_NAVIGATE_DOWN, "下一个"
			, wxArtProvider::GetBitmap(wxART_GO_DOWN, wxART_OTHER, wxSize(16, 16))
			, "下一个代码");
		navigate_bar->SetButtonMaxSizeClass(ID_NAVIGATE_DOWN, wxRIBBON_BUTTONBAR_BUTTON_MEDIUM);
		navigate_bar->SetButtonMinSizeClass(ID_NAVIGATE_DOWN, wxRIBBON_BUTTONBAR_BUTTON_MEDIUM);*/

		auto market_panel = new wxRibbonPanel(home, wxID_ANY, "行情");
		wxRibbonButtonBar* market_bar = new wxRibbonButtonBar(market_panel);
		market_bar->AddButton(ID_MARKET_SELF, "自选", wxBitmap(transparent_xpm), wxEmptyString);
		market_bar->SetButtonMaxSizeClass(ID_MARKET_SELF, wxRIBBON_BUTTONBAR_BUTTON_MEDIUM);
		market_bar->SetButtonMinSizeClass(ID_MARKET_SELF, wxRIBBON_BUTTONBAR_BUTTON_MEDIUM);
		market_bar->AddButton(ID_MARKET_MAIN, "主力", wxBitmap(transparent_xpm));
		market_bar->SetButtonMaxSizeClass(ID_MARKET_MAIN, wxRIBBON_BUTTONBAR_BUTTON_MEDIUM);
		market_bar->SetButtonMinSizeClass(ID_MARKET_MAIN, wxRIBBON_BUTTONBAR_BUTTON_MEDIUM);
		market_bar->AddButton(ID_MARKET_ALL, "全部", wxBitmap(transparent_xpm));
		market_bar->SetButtonMaxSizeClass(ID_MARKET_ALL, wxRIBBON_BUTTONBAR_BUTTON_MEDIUM);
		market_bar->SetButtonMinSizeClass(ID_MARKET_ALL, wxRIBBON_BUTTONBAR_BUTTON_MEDIUM);
		zqdb::AllExchange allexchange;
		for (size_t i = 0, j = 0; i < allexchange.size(); i++)
		{
			zqdb::Exchange exchange(allexchange[i]);
			auto info = exchange.GetValue();
			//market_bar->AddHybridButton(ID_MARKET_EXCHANGE + i
			market_bar->AddButton(ID_MARKET_EXCHANGE + i
				, utf8cv.cMB2WX(info->Name), wxBitmap(transparent_xpm));
			market_bar->SetButtonMaxSizeClass(ID_MARKET_EXCHANGE + i, wxRIBBON_BUTTONBAR_BUTTON_MEDIUM);
			market_bar->SetButtonMinSizeClass(ID_MARKET_EXCHANGE + i, wxRIBBON_BUTTONBAR_BUTTON_MEDIUM);
		}

		//wxRibbonPanel *analysis_panel = new wxRibbonPanel(home, wxID_ANY, "分析",
		//	wxNullBitmap, wxDefaultPosition, wxDefaultSize,
		//	wxRIBBON_PANEL_NO_AUTO_MINIMISE |
		//	wxRIBBON_PANEL_EXT_BUTTON);
		//wxRibbonToolBar *analysis_bar = new wxRibbonToolBar(analysis_panel, ID_MAIN_TOOLBAR);
		////加减指标，技术模板，移动，缩放
		////周期，划线
		////加减指标
		//analysis_bar->AddDropdownTool(wxID_ADD, wxArtProvider::GetBitmap(wxART_PLUS, wxART_OTHER, wxSize(16, 15)));
		//analysis_bar->AddDropdownTool(wxID_REMOVE, wxArtProvider::GetBitmap(wxART_MINUS, wxART_OTHER, wxSize(16, 15)));
		//analysis_bar->AddSeparator();
		////周期
		//analysis_bar->AddToggleTool(wxID_JUSTIFY_LEFT, align_left_xpm);
		//analysis_bar->AddToggleTool(wxID_JUSTIFY_CENTER, align_center_xpm);
		//analysis_bar->AddToggleTool(wxID_JUSTIFY_RIGHT, align_right_xpm);
		//analysis_bar->AddToggleTool(ID_SMALL_BUTTON_1, align_right_xpm);
		//analysis_bar->AddToggleTool(ID_SMALL_BUTTON_1, align_right_xpm);
		//analysis_bar->AddToggleTool(ID_SMALL_BUTTON_1, align_right_xpm);
		//analysis_bar->AddToggleTool(ID_SMALL_BUTTON_1, align_right_xpm);
		//analysis_bar->AddToggleTool(ID_SMALL_BUTTON_1, align_right_xpm);
		//analysis_bar->AddSeparator();
		////技术模板
		//analysis_bar->AddHybridTool(wxID_NEW, wxArtProvider::GetBitmap(wxART_NEW, wxART_OTHER, wxSize(16, 15)));
		//analysis_bar->AddTool(wxID_OPEN, wxArtProvider::GetBitmap(wxART_FILE_OPEN, wxART_OTHER, wxSize(16, 15)), "Open something");
		//analysis_bar->AddTool(wxID_SAVE, wxArtProvider::GetBitmap(wxART_FILE_SAVE, wxART_OTHER, wxSize(16, 15)), "Save something");
		//analysis_bar->AddTool(wxID_SAVEAS, wxArtProvider::GetBitmap(wxART_FILE_SAVE_AS, wxART_OTHER, wxSize(16, 15)), "Save something as ...");
		//analysis_bar->EnableTool(wxID_OPEN, false);
		//analysis_bar->EnableTool(wxID_SAVE, false);
		//analysis_bar->EnableTool(wxID_SAVEAS, false);
		//analysis_bar->AddSeparator();
		////移动
		//analysis_bar->AddTool(wxID_ANY, wxArtProvider::GetBitmap(wxART_GOTO_FIRST, wxART_OTHER, wxSize(16, 15)));
		//analysis_bar->AddTool(wxID_ANY, wxArtProvider::GetBitmap(wxART_GO_FORWARD, wxART_OTHER, wxSize(16, 15)));
		//analysis_bar->AddTool(wxID_ANY, wxArtProvider::GetBitmap(wxART_GO_BACK, wxART_OTHER, wxSize(16, 15)));
		//analysis_bar->AddTool(wxID_ANY, wxArtProvider::GetBitmap(wxART_GOTO_LAST, wxART_OTHER, wxSize(16, 15)));
		//analysis_bar->AddSeparator();
		////划线
		//analysis_bar->AddToggleTool(wxID_JUSTIFY_LEFT, align_left_xpm);
		//analysis_bar->AddToggleTool(wxID_JUSTIFY_CENTER, align_center_xpm);
		//analysis_bar->AddToggleTool(wxID_JUSTIFY_RIGHT, align_right_xpm);
		//analysis_bar->AddToggleTool(ID_SMALL_BUTTON_1, align_right_xpm);
		//analysis_bar->AddToggleTool(ID_SMALL_BUTTON_1, align_right_xpm);
		//analysis_bar->AddToggleTool(ID_SMALL_BUTTON_1, align_right_xpm);
		//analysis_bar->AddToggleTool(ID_SMALL_BUTTON_1, align_right_xpm);
		//analysis_bar->AddToggleTool(ID_SMALL_BUTTON_1, align_right_xpm);
		//analysis_bar->AddSeparator();
		////缩放
		//analysis_bar->AddTool(wxID_ZOOM_IN, zoom_in_xpm);
		//analysis_bar->AddTool(wxID_ZOOM_OUT, zoom_out_xpm);
		//analysis_bar->AddSeparator();
		///*analysis_bar->AddHybridTool(ID_POSITION_LEFT, position_left_xpm,
		//	"Align ribbonbar vertically\non the left\nfor demonstration purposes");
		//analysis_bar->AddHybridTool(ID_POSITION_TOP, position_top_xpm,
		//	"Align the ribbonbar horizontally\nat the top\nfor demonstration purposes");
		//analysis_bar->AddSeparator();
		//analysis_bar->AddHybridTool(wxID_PRINT, wxArtProvider::GetBitmap(wxART_PRINT, wxART_OTHER, wxSize(16, 15)),
		//	"This is the Print button tooltip\ndemonstrating a tooltip");*/
		//analysis_bar->SetRows(2, 2);

		auto trade_panel = new wxRibbonPanel(home, wxID_ANY, "交易");
		wxRibbonButtonBar* trade_bar = new wxRibbonButtonBar(trade_panel);
		/*trade_bar->AddButton(ID_TRADE_QUICK_BUY_FAK, "快买FAK", wxBitmap(transparent_xpm));
		trade_bar->SetButtonMaxSizeClass(ID_TRADE_QUICK_BUY_FAK, wxRIBBON_BUTTONBAR_BUTTON_MEDIUM);
		trade_bar->SetButtonMinSizeClass(ID_TRADE_QUICK_BUY_FAK, wxRIBBON_BUTTONBAR_BUTTON_MEDIUM);
		trade_bar->AddButton(ID_TRADE_QUICK_SELL_FAK, "快卖FAK", wxBitmap(transparent_xpm));
		trade_bar->SetButtonMaxSizeClass(ID_TRADE_QUICK_SELL_FAK, wxRIBBON_BUTTONBAR_BUTTON_MEDIUM);
		trade_bar->SetButtonMinSizeClass(ID_TRADE_QUICK_SELL_FAK, wxRIBBON_BUTTONBAR_BUTTON_MEDIUM);
		trade_bar->AddButton(ID_TRADE_QUICK_CLOSE, "快平", wxBitmap(transparent_xpm));
		trade_bar->SetButtonTextMinWidth(ID_TRADE_QUICK_CLOSE, wxT("上一个"));
		trade_bar->SetButtonMaxSizeClass(ID_TRADE_QUICK_CLOSE, wxRIBBON_BUTTONBAR_BUTTON_MEDIUM);
		trade_bar->SetButtonMinSizeClass(ID_TRADE_QUICK_CLOSE, wxRIBBON_BUTTONBAR_BUTTON_MEDIUM);
		trade_bar->AddButton(ID_TRADE_QUICK_BUY_FOK, "快买FOK", wxBitmap(transparent_xpm));
		trade_bar->SetButtonMaxSizeClass(ID_TRADE_QUICK_BUY_FOK, wxRIBBON_BUTTONBAR_BUTTON_MEDIUM);
		trade_bar->SetButtonMinSizeClass(ID_TRADE_QUICK_BUY_FOK, wxRIBBON_BUTTONBAR_BUTTON_MEDIUM);
		trade_bar->AddButton(ID_TRADE_QUICK_SELL_FOK, "快卖FOK", wxBitmap(transparent_xpm));
		trade_bar->SetButtonMaxSizeClass(ID_TRADE_QUICK_SELL_FOK, wxRIBBON_BUTTONBAR_BUTTON_MEDIUM);
		trade_bar->SetButtonMinSizeClass(ID_TRADE_QUICK_SELL_FOK, wxRIBBON_BUTTONBAR_BUTTON_MEDIUM);*/
		trade_bar->AddButton(ID_TRADE_POSITION, "持仓", wxBitmap(transparent_xpm));
		trade_bar->SetButtonMaxSizeClass(ID_TRADE_POSITION, wxRIBBON_BUTTONBAR_BUTTON_MEDIUM);
		trade_bar->SetButtonMinSizeClass(ID_TRADE_POSITION, wxRIBBON_BUTTONBAR_BUTTON_MEDIUM);
		trade_bar->AddButton(ID_TRADE_ORDER, "委托", wxBitmap(transparent_xpm));
		trade_bar->SetButtonMaxSizeClass(ID_TRADE_ORDER, wxRIBBON_BUTTONBAR_BUTTON_MEDIUM);
		trade_bar->SetButtonMinSizeClass(ID_TRADE_ORDER, wxRIBBON_BUTTONBAR_BUTTON_MEDIUM);
		trade_bar->AddButton(ID_TRADE_TRADE, "成交", wxBitmap(transparent_xpm));
		trade_bar->SetButtonMaxSizeClass(ID_TRADE_TRADE, wxRIBBON_BUTTONBAR_BUTTON_MEDIUM);
		trade_bar->SetButtonMinSizeClass(ID_TRADE_TRADE, wxRIBBON_BUTTONBAR_BUTTON_MEDIUM);
		
		//wxRibbonPanel *sizer_panel = new wxRibbonPanel(home, wxID_ANY, "Panel with Sizer",
		//	wxNullBitmap, wxDefaultPosition, wxDefaultSize,
		//	wxRIBBON_PANEL_DEFAULT_STYLE);

		//wxArrayString as;
		//as.Add("Item 1 using a box sizer now");
		//as.Add("Item 2 using a box sizer now");
		//wxComboBox* sizer_panelcombo = new wxComboBox(sizer_panel, wxID_ANY,
		//	wxEmptyString,
		//	wxDefaultPosition, wxDefaultSize,
		//	as, wxCB_READONLY);

		//wxComboBox* sizer_panelcombo2 = new wxComboBox(sizer_panel, wxID_ANY,
		//	wxEmptyString,
		//	wxDefaultPosition, wxDefaultSize,
		//	as, wxCB_READONLY);

		//sizer_panelcombo->Select(0);
		//sizer_panelcombo2->Select(1);
		//sizer_panelcombo->SetMinSize(wxSize(150, 24));
		//sizer_panelcombo2->SetMinSize(wxSize(150, 24));

		//wxRibbonButtonBar* bar = new wxRibbonButtonBar(sizer_panel, wxID_ANY);
		//bar->AddButton(ID_BUTTON_XX, "xx", ribbon_xpm);
		//bar->AddButton(ID_BUTTON_XY, "xy", ribbon_xpm);
		//// This prevents ribbon buttons in panels with sizer from collapsing.
		//bar->SetButtonMinSizeClass(ID_BUTTON_XX, wxRIBBON_BUTTONBAR_BUTTON_LARGE);
		//bar->SetButtonMinSizeClass(ID_BUTTON_XY, wxRIBBON_BUTTONBAR_BUTTON_LARGE);

		//wxSizer* sizer_panelsizer_h = new wxBoxSizer(wxHORIZONTAL);
		//wxSizer* sizer_panelsizer_v = new wxBoxSizer(wxVERTICAL);
		//sizer_panelsizer_v->AddStretchSpacer(1);
		//sizer_panelsizer_v->Add(sizer_panelcombo, 0, wxALL | wxEXPAND, 2);
		//sizer_panelsizer_v->Add(sizer_panelcombo2, 0, wxALL | wxEXPAND, 2);
		//sizer_panelsizer_v->AddStretchSpacer(1);
		//sizer_panelsizer_h->Add(bar, 0, wxEXPAND);
		//sizer_panelsizer_h->Add(sizer_panelsizer_v, 0);
		//sizer_panel->SetSizer(sizer_panelsizer_h);

		//wxFont label_font(wxFontInfo(8).Light());
		//m_bitmap_creation_dc.SetFont(label_font);
	}
	{
		wxRibbonPage* market_page = new wxRibbonPage(m_ribbon, wxID_ANY, "行情");
		auto market_panel = new wxRibbonPanel(market_page, wxID_ANY, "行情");
		wxRibbonButtonBar* market_bar = new wxRibbonButtonBar(market_panel);
		market_bar->AddButton(ID_MARKET_SELF, "自选", wxBitmap(transparent_xpm), wxEmptyString);
		market_bar->SetButtonMaxSizeClass(ID_MARKET_SELF, wxRIBBON_BUTTONBAR_BUTTON_MEDIUM);
		market_bar->SetButtonMinSizeClass(ID_MARKET_SELF, wxRIBBON_BUTTONBAR_BUTTON_MEDIUM);
		market_bar->AddHybridButton(ID_MARKET_MAIN, "主力", wxBitmap(transparent_xpm));
		market_bar->SetButtonMaxSizeClass(ID_MARKET_MAIN, wxRIBBON_BUTTONBAR_BUTTON_MEDIUM);
		market_bar->SetButtonMinSizeClass(ID_MARKET_MAIN, wxRIBBON_BUTTONBAR_BUTTON_MEDIUM);
		market_bar->AddHybridButton(ID_MARKET_ALL, "全部", wxBitmap(transparent_xpm));
		market_bar->SetButtonMaxSizeClass(ID_MARKET_ALL, wxRIBBON_BUTTONBAR_BUTTON_MEDIUM);
		market_bar->SetButtonMinSizeClass(ID_MARKET_ALL, wxRIBBON_BUTTONBAR_BUTTON_MEDIUM);
		zqdb::AllExchange allexchange;
		for (size_t i = 0; i < allexchange.size(); i++)
		{
			zqdb::Exchange exchange(allexchange[i]);
			auto info = exchange.GetValue();
			market_bar->AddHybridButton(ID_MARKET_EXCHANGE + i
				, utf8cv.cMB2WX(info->Name), wxBitmap(transparent_xpm));
			market_bar->SetButtonMaxSizeClass(ID_MARKET_EXCHANGE + i, wxRIBBON_BUTTONBAR_BUTTON_MEDIUM);
			market_bar->SetButtonMinSizeClass(ID_MARKET_EXCHANGE + i, wxRIBBON_BUTTONBAR_BUTTON_MEDIUM);
		}
	}
	/*{
		wxRibbonPage* page = new wxRibbonPage(m_ribbon, wxID_ANY, "分析", empty_xpm);
		wxRibbonPanel* panel = new wxRibbonPanel(page, wxID_ANY, "Button bar manipulation", ribbon_xpm);
		m_mutable_button_bar = new wxRibbonButtonBar(panel, wxID_ANY);
		m_mutable_button_bar->AddButton(ID_PLUS_MINUS, "+/-",
			wxArtProvider::GetBitmap(wxART_PLUS, wxART_OTHER, wxSize(24, 24)));
		m_plus_minus_state = false;
		m_mutable_button_bar->AddButton(ID_CHANGE_LABEL, "short", ribbon_xpm);
		m_mutable_button_bar->SetButtonTextMinWidth(ID_CHANGE_LABEL, "some long text");
		m_change_label_state = false;

		panel = new wxRibbonPanel(page, wxID_ANY, "Always medium buttons", ribbon_xpm);
		wxRibbonButtonBar* bar = new wxRibbonButtonBar(panel, wxID_ANY);
		bar->AddButton(ID_SMALL_BUTTON_1, "Button 1", ribbon_xpm);
		bar->SetButtonMaxSizeClass(ID_SMALL_BUTTON_1, wxRIBBON_BUTTONBAR_BUTTON_MEDIUM);
		bar->AddButton(ID_SMALL_BUTTON_2, "Button 2", ribbon_xpm);
		bar->SetButtonMaxSizeClass(ID_SMALL_BUTTON_2, wxRIBBON_BUTTONBAR_BUTTON_MEDIUM);
		bar->AddButton(ID_SMALL_BUTTON_3, "Button 3", ribbon_xpm);
		bar->AddButton(ID_SMALL_BUTTON_4, "Button 4", ribbon_xpm);
		bar->AddButton(ID_SMALL_BUTTON_5, "Button 5", ribbon_xpm);
		bar->SetButtonMaxSizeClass(ID_SMALL_BUTTON_5, wxRIBBON_BUTTONBAR_BUTTON_MEDIUM);
		bar->AddButton(ID_SMALL_BUTTON_6, "Button 6", ribbon_xpm);
		bar->SetButtonMaxSizeClass(ID_SMALL_BUTTON_6, wxRIBBON_BUTTONBAR_BUTTON_MEDIUM);
	}*/
	{
		wxRibbonPage* trade_page = new wxRibbonPage(m_ribbon, wxID_ANY, "交易");
		auto trade_panel = new wxRibbonPanel(trade_page, wxID_ANY, "交易");
		wxRibbonButtonBar* trade_bar = new wxRibbonButtonBar(trade_panel);
		trade_bar->AddButton(ID_TRADE_QUICK_BUY_FAK, "快买FAK", wxBitmap(transparent_xpm));
		trade_bar->SetButtonMaxSizeClass(ID_TRADE_QUICK_BUY_FAK, wxRIBBON_BUTTONBAR_BUTTON_MEDIUM);
		trade_bar->SetButtonMinSizeClass(ID_TRADE_QUICK_BUY_FAK, wxRIBBON_BUTTONBAR_BUTTON_MEDIUM);
		trade_bar->AddButton(ID_TRADE_QUICK_SELL_FAK, "快卖FAK", wxBitmap(transparent_xpm));
		trade_bar->SetButtonMaxSizeClass(ID_TRADE_QUICK_SELL_FAK, wxRIBBON_BUTTONBAR_BUTTON_MEDIUM);
		trade_bar->SetButtonMinSizeClass(ID_TRADE_QUICK_SELL_FAK, wxRIBBON_BUTTONBAR_BUTTON_MEDIUM);
		trade_bar->AddButton(ID_TRADE_QUICK_CLOSE, "快平", wxBitmap(transparent_xpm));
		trade_bar->SetButtonTextMinWidth(ID_TRADE_QUICK_CLOSE, wxT("上一个"));
		trade_bar->SetButtonMaxSizeClass(ID_TRADE_QUICK_CLOSE, wxRIBBON_BUTTONBAR_BUTTON_MEDIUM);
		trade_bar->SetButtonMinSizeClass(ID_TRADE_QUICK_CLOSE, wxRIBBON_BUTTONBAR_BUTTON_MEDIUM);
		trade_bar->AddButton(ID_TRADE_QUICK_BUY_FOK, "快买FOK", wxBitmap(transparent_xpm));
		trade_bar->SetButtonMaxSizeClass(ID_TRADE_QUICK_BUY_FOK, wxRIBBON_BUTTONBAR_BUTTON_MEDIUM);
		trade_bar->SetButtonMinSizeClass(ID_TRADE_QUICK_BUY_FOK, wxRIBBON_BUTTONBAR_BUTTON_MEDIUM);
		trade_bar->AddButton(ID_TRADE_QUICK_SELL_FOK, "快卖FOK", wxBitmap(transparent_xpm));
		trade_bar->SetButtonMaxSizeClass(ID_TRADE_QUICK_SELL_FOK, wxRIBBON_BUTTONBAR_BUTTON_MEDIUM);
		trade_bar->SetButtonMinSizeClass(ID_TRADE_QUICK_SELL_FOK, wxRIBBON_BUTTONBAR_BUTTON_MEDIUM);
		trade_bar->AddButton(ID_TRADE_POSITION, "持仓", wxBitmap(transparent_xpm));
		trade_bar->SetButtonMaxSizeClass(ID_TRADE_POSITION, wxRIBBON_BUTTONBAR_BUTTON_MEDIUM);
		trade_bar->SetButtonMinSizeClass(ID_TRADE_POSITION, wxRIBBON_BUTTONBAR_BUTTON_MEDIUM);
		trade_bar->AddButton(ID_TRADE_ORDER, "委托", wxBitmap(transparent_xpm));
		trade_bar->SetButtonMaxSizeClass(ID_TRADE_ORDER, wxRIBBON_BUTTONBAR_BUTTON_MEDIUM);
		trade_bar->SetButtonMinSizeClass(ID_TRADE_ORDER, wxRIBBON_BUTTONBAR_BUTTON_MEDIUM);
		trade_bar->AddButton(ID_TRADE_TRADE, "成交", wxBitmap(transparent_xpm));
		trade_bar->SetButtonMaxSizeClass(ID_TRADE_TRADE, wxRIBBON_BUTTONBAR_BUTTON_MEDIUM);
		trade_bar->SetButtonMinSizeClass(ID_TRADE_TRADE, wxRIBBON_BUTTONBAR_BUTTON_MEDIUM);
	}
	{
        wxRibbonPage* scheme = new wxRibbonPage(m_ribbon, wxID_ANY, "帮助");
        /*m_ribbon->GetArtProvider()->GetColourScheme(&m_default_primary,
            &m_default_secondary, &m_default_tertiary);
        wxRibbonPanel *provider_panel = new wxRibbonPanel(scheme, wxID_ANY,
            "Art", wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_NO_AUTO_MINIMISE);
        wxRibbonButtonBar *provider_bar = new wxRibbonButtonBar(provider_panel, wxID_ANY);
        provider_bar->AddButton(ID_DEFAULT_PROVIDER, "Default Provider",
            wxArtProvider::GetBitmap(wxART_QUESTION, wxART_OTHER, wxSize(32, 32)));
        provider_bar->AddButton(ID_AUI_PROVIDER, "AUI Provider", aui_style_xpm);
        provider_bar->AddButton(ID_MSW_PROVIDER, "MSW Provider", msw_style_xpm);
        wxRibbonPanel *primary_panel = new wxRibbonPanel(scheme, wxID_ANY,
            "Primary Colour", colours_xpm);
        m_primary_gallery = PopulateColoursPanel(primary_panel,
            m_default_primary, ID_PRIMARY_COLOUR);
        wxRibbonPanel *secondary_panel = new wxRibbonPanel(scheme, wxID_ANY,
            "Secondary Colour", colours_xpm);
        m_secondary_gallery = PopulateColoursPanel(secondary_panel,
            m_default_secondary, ID_SECONDARY_COLOUR);*/
    }
    //new wxRibbonPage(m_ribbon, wxID_ANY, "Empty Page", empty_xpm);
    //new wxRibbonPage(m_ribbon, wxID_ANY, "Highlight Page", empty_xpm);
    //m_ribbon->AddPageHighlight(m_ribbon->GetPageCount()-1);

    m_ribbon->Realize();

    //m_logwindow = new wxTextCtrl(this, wxID_ANY, wxEmptyString,
    //    wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY |
    //    wxTE_LEFT | wxTE_BESTWRAP | wxBORDER_NONE);
	//m_logwindow->Show(false);

	code_list_ = new wxDataViewCtrl(this, wxID_ANY, wxDefaultPosition, wxSize(300,300), wxNO_BORDER | wxDV_NO_HEADER | wxDV_VARIABLE_LINE_HEIGHT);

	code_list_model_ = new MyCodeListModel;
	code_list_->AssociateModel(code_list_model_.get());

	code_list_code_render_ = new MyCodeListCodeRenderer();
	auto code_col = new wxDataViewColumn("Code", code_list_code_render_, MyCodeListModel::Col_Code
	, FromDIP(220), wxALIGN_LEFT,  wxDATAVIEW_COL_RESIZABLE);
	code_list_->AppendColumn(code_col);
	//code_list_->AppendTextColumn("Code",
	//	MyCodeListModel::Col_Code);
	code_list_name_render_ = new MyCodeListNameRenderer();
	/*auto name_col = new wxDataViewColumn("Name", code_list_name_render_, MyCodeListModel::Col_Name);
	code_list_->AppendColumn(name_col);*/
	//code_list_->AppendTextColumn("Name",
	//	MyCodeListModel::Col_Name);
	//code_list_->AppendTextColumn("Desc",
	//	SmartKBListModel::Col_Desc);

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
	else */ {
		auto opt_tv = cfg.get_child_optional("techview");
		ASSERT(opt_tv);
		auto& cfg_tv = opt_tv.get();
		auto tv = new zqdb::TechView(this, (const char*)&cfg_tv, XUtil::XML_FLAG_PTREE);
		tv->SetSkinInfo(wxGetApp().GetSkinInfo());
		m_tv = tv;
		//m_tv->SetScrollbars(10, 10, 100, 240);
	}

	m_togglePanels = new wxToggleButton(this, wxID_ANY, "&Toggle panels");
	m_togglePanels->SetValue(true);
	m_togglePanels->Show(false);

#if wxUSE_NOTEBOOK
	m_type = Type_Notebook;
#elif wxUSE_CHOICEBOOK
	m_type = Type_Choicebook;
#elif wxUSE_LISTBOOK
	m_type = Type_Listbook;
#elif wxUSE_TREEBOOK
	m_type = Type_Treebook;
#elif wxUSE_TOOLBOOK
	m_type = Type_Toolbook;
#elif wxUSE_AUI
	m_type = Type_Aui;
#else
	m_type = Type_Simplebook;
#endif

	m_orient = ID_ORIENT_DEFAULT;
	m_chkShowImages = true;
	m_fixedWidth = false;
	m_multi = false;
	m_noPageTheme = false;
	m_buttonBar = false;
	m_horzLayout = false;

	m_bookCtrl = nullptr;
	RecreateBook();

	ShowView(position_list_);

	wxSizer *s = new wxBoxSizer(wxVERTICAL);

	s->Add(m_ribbon, 0, wxEXPAND);

	wxSizer *sh = new wxBoxSizer(wxHORIZONTAL);

	//sh->Add(m_logwindow, 0, wxEXPAND);
	sh->Add(code_list_, 0, wxEXPAND);
	sh->AddSpacer(1);

	/*auto opt_topview = cfg.get_child_optional("topview");
	if (opt_topview) {
		auto& cfg_topview = opt_topview.get();
		auto *sizer_topview = new wxBoxSizer(wxHORIZONTAL);
		auto opt_tradeview = cfg_topview.get_child_optional("tradeview");
		auto opt_infoview = cfg_topview.get_child_optional("infoview");
		if (opt_tradeview) {
			trade_view_ = new TradeView(this, (const char*)&opt_tradeview.get(), XUtil::XML_FLAG_PTREE);
			sizer_topview->Add(trade_view_, 0, wxEXPAND);
		}
		if (opt_infoview) {
			info_view_ = new InfoView(this, (const char*)&opt_infoview.get(), XUtil::XML_FLAG_PTREE);
			sizer_topview->Add(info_view_, 1, wxEXPAND);
		}
		else {
			info_view_ = new InfoView(this);
			sizer_topview->Add(info_view_, 1, wxEXPAND);
		}
		tech_sizer_->Add(sizer_topview, 0, wxEXPAND);
	}*/

	sh->Add(m_tv, 1, wxEXPAND);

	//右侧视图
	/*sh->AddSplitter(1);
	//sh->AddSpacer(200);
	sh->Add(
	new wxButton(this, wxID_CANCEL, "Cancel"),
	1,
	wxEXPAND);*/
	auto opt_rightview = cfg.get_child_optional("rightview");
	if (opt_rightview) {
		bool add_spacer = false;
		auto& cfg_rightview = opt_rightview.get();
		auto *sizer_rightview = new wxBoxSizer(wxVERTICAL);
		auto opt_titleview = cfg_rightview.get_child_optional("titleview");
		auto opt_mmpview = cfg_rightview.get_child_optional("mmpview");
		auto opt_tradeview = cfg_rightview.get_child_optional("tradeview");
		auto opt_tickview = cfg_rightview.get_child_optional("tickview");
		if (opt_titleview) {
			if (add_spacer) {
				sizer_rightview->AddSpacer(1);
			}
			add_spacer = true;
			title_view_ = new zqdb::TitleView(this, (const char*)&opt_titleview.get(), XUtil::XML_FLAG_PTREE);
			sizer_rightview->Add(title_view_, 0, wxEXPAND);
		}
		if (opt_mmpview) {
			if (add_spacer) {
				sizer_rightview->AddSpacer(1);
			}
			add_spacer = true;
			mmp_view_ = new zqdb::MmpView(this, (const char*)&opt_mmpview.get(), XUtil::XML_FLAG_PTREE);
			sizer_rightview->Add(mmp_view_, 0, wxEXPAND);
		}
		if (opt_tradeview) {
			if (add_spacer) {
				sizer_rightview->AddSpacer(1);
			}
			add_spacer = true;
			trade_view_ = new zqdb::TradeView(this, (const char*)&opt_tradeview.get(), XUtil::XML_FLAG_PTREE);
			sizer_rightview->Add(trade_view_, 0, wxEXPAND);
		}
		if (opt_tickview) {
			if (add_spacer) {
				sizer_rightview->AddSpacer(1);
			}
			add_spacer = true;
			tick_view_ = new zqdb::TickView(this, (const char*)&opt_tickview.get(), XUtil::XML_FLAG_PTREE);
			sizer_rightview->Add(tick_view_, 1, wxEXPAND);
		}
		else {
			if (add_spacer) {
				sizer_rightview->AddSpacer(1);
			}
			add_spacer = true;
			tick_view_ = new zqdb::TickView(this);
			sizer_rightview->Add(tick_view_, 1, wxEXPAND);
		}
		sh->AddSpacer(1);
		sh->Add(sizer_rightview, 0, wxEXPAND);
	}

	s->Add(sh, 1, wxEXPAND);

	s->Add(m_bookCtrl, 0, wxEXPAND);

	SetSizer(s);

	Bind(wxEVT_RIBBONBUTTONBAR_CLICKED, &MyFrame::OnMarket, this, ID_MARKET_BEGIN, ID_MARKET_END);
	Bind(wxEVT_RIBBONBUTTONBAR_DROPDOWN_CLICKED, &MyFrame::OnMarketDropdown, this, ID_MARKET_BEGIN, ID_MARKET_END);

	Bind(MY_CMD_GOTO_EVENT, &MyFrame::OnGoto, this);
	Bind(wxEVT_DATAVIEW_ITEM_ACTIVATED, &MyFrame::OnActivated, this);
	//Bind(ZQDB_CODE_PREV_EVENT, &MyFrame::OnCodePrev, this);
	//Bind(ZQDB_CODE_NEXT_EVENT, &MyFrame::OnCodeNext, this);
	Bind(ZQDB_NOTIFY_ENABLE_EVENT, &MyFrame::OnNotify, this);
	Bind(ZQDB_NOTIFY_DISABLE_EVENT, &MyFrame::OnNotify, this);
	Bind(ZQDB_NOTIFY_APPEND_EVENT, &MyFrame::OnNotify, this);
	Bind(ZQDB_NOTIFY_REMOVE_EVENT, &MyFrame::OnNotify, this);
	Bind(ZQDB_NOTIFY_UPDATE_EVENT, &MyFrame::OnNotify, this);

	// test that event handlers pushed on top of MDI children do work (this
	// used to be broken, see #11225)
	//PushEventHandler(new EventHandler(this));

	timer_.Start(1000);

	UpdateStatus();
}

MyFrame::~MyFrame()
{
	//PopEventHandler(true);

	wxGetApp().ResetFrame(this);
}

// DISPATCH_ON_TYPE() macro is an ugly way to write the "same" code for
// different wxBookCtrlBase-derived classes without duplicating code and
// without using templates, it expands into "before <xxx> after" where "xxx"
// part is control class-specific
#if wxUSE_NOTEBOOK
#define CASE_NOTEBOOK(x) case Type_Notebook: x; break;
#else
#define CASE_NOTEBOOK(x)
#endif

#if wxUSE_LISTBOOK
#define CASE_LISTBOOK(x) case Type_Listbook: x; break;
#else
#define CASE_LISTBOOK(x)
#endif

#if wxUSE_CHOICEBOOK
#define CASE_CHOICEBOOK(x) case Type_Choicebook: x; break;
#else
#define CASE_CHOICEBOOK(x)
#endif

#if wxUSE_TREEBOOK
#define CASE_TREEBOOK(x) case Type_Treebook: x; break;
#else
#define CASE_TREEBOOK(x)
#endif

#if wxUSE_TOOLBOOK
#define CASE_TOOLBOOK(x) case Type_Toolbook: x; break;
#else
#define CASE_TOOLBOOK(x)
#endif

#if wxUSE_AUI
#define CASE_AUINOTEBOOK(x) case Type_AuiNotebook: x; break;
#else
#define CASE_AUINOTEBOOK(x)
#endif

#define CASE_SIMPLEBOOK(x) case Type_Simplebook: x; break;

#define DISPATCH_ON_TYPE(before, nb, lb, cb, tb, toolb, aui, sb, after)       \
    switch ( m_type )                                                         \
    {                                                                         \
        CASE_NOTEBOOK(before nb after)                                        \
        CASE_LISTBOOK(before lb after)                                        \
        CASE_CHOICEBOOK(before cb after)                                      \
        CASE_TREEBOOK(before tb after)                                        \
        CASE_TOOLBOOK(before toolb after)                                     \
        CASE_AUINOTEBOOK(before aui after)                                    \
        CASE_SIMPLEBOOK(before sb after)                                      \
                                                                              \
        default:                                                              \
            wxFAIL_MSG( "unknown book control type" );                   \
    }

void MyFrame::RecreateBook()
{
	int flags;
	switch (m_orient)
	{
	case ID_ORIENT_TOP:
		flags = wxBK_TOP;
		break;

	case ID_ORIENT_BOTTOM:
		flags = wxBK_BOTTOM;
		break;

	case ID_ORIENT_LEFT:
		flags = wxBK_LEFT;
		break;

	case ID_ORIENT_RIGHT:
		flags = wxBK_RIGHT;
		break;

	default:
		flags = wxBK_DEFAULT;
	}

#if wxUSE_NOTEBOOK
	if (m_fixedWidth && m_type == Type_Notebook)
		flags |= wxNB_FIXEDWIDTH;
	if (m_multi && m_type == Type_Notebook)
		flags |= wxNB_MULTILINE;
	if (m_noPageTheme && m_type == Type_Notebook)
		flags |= wxNB_NOPAGETHEME;
#endif
#if wxUSE_TOOLBOOK
	if (m_buttonBar && m_type == Type_Toolbook)
		flags |= wxTBK_BUTTONBAR;
	if (m_horzLayout && m_type == Type_Toolbook)
		flags |= wxTBK_HORZ_LAYOUT;
#endif

	wxBookCtrlBase *oldBook = m_bookCtrl;

	DISPATCH_ON_TYPE(m_bookCtrl = new,
		wxNotebook,
		wxListbook,
		wxChoicebook,
		wxTreebook,
		wxToolbook,
		wxAuiNotebook,
		wxSimplebook,
		(this, wxID_ANY, wxDefaultPosition, wxSize(-1, 150), flags));

	if (!m_bookCtrl)
		return;

	m_bookCtrl->Hide();

	// wxToolbook doesn't work without icons so always use them for it.
	/*if (m_chkShowImages || m_type == Type_Toolbook)
	{
	m_bookCtrl->SetImageList(m_imageList);
	}*/

	if (oldBook)
	{
#if wxUSE_TREEBOOK
		// we only need the old treebook if we're recreating another treebook
		wxTreebook *tbkOld = m_type == Type_Treebook
			? wxDynamicCast(oldBook, wxTreebook)
			: NULL;
#endif // wxUSE_TREEBOOK

		const int count = oldBook->GetPageCount();
		for (int n = 0; n < count; n++)
		{
			const int image = -1;//GetIconIndex(m_bookCtrl);
			const wxString str = oldBook->GetPageText(n);

			wxWindow *page = new wxTextCtrl(m_bookCtrl, wxID_ANY, str,
				wxDefaultPosition, wxDefaultSize,
				wxTE_MULTILINE | wxTE_READONLY);

			// treebook complication: need to account for possible parent page
#if wxUSE_TREEBOOK
			if (tbkOld)
			{
				const int parent = tbkOld->GetPageParent(n);
				if (parent != wxNOT_FOUND)
				{
					wxStaticCast(m_bookCtrl, wxTreebook)->
						InsertSubPage(parent, page, str, false, image);

					// skip adding it again below
					continue;
				}
			}
#endif // wxUSE_TREEBOOK

			m_bookCtrl->AddPage(page, str, false, image);
		}

		const int sel = oldBook->GetSelection();
		if (sel != wxNOT_FOUND)
			m_bookCtrl->SetSelection(sel);

		delete oldBook;
	}
	else // no old book
	{
		m_bookCtrl->AddPage(new wxTextCtrl(m_bookCtrl, wxID_ANY, wxEmptyString,
			wxDefaultPosition, wxDefaultSize,
			wxTE_MULTILINE | wxTE_READONLY), wxT("日志"), false);

		position_list_ = new wxDataViewCtrl(m_bookCtrl, wxID_ANY);
		position_list_model_ = new MyHZQDBListModel("./mytrader/positionlist.json", XUtil::XML_FLAG_JSON_FILE);
		position_list_->AssociateModel(position_list_model_.get());
		if (h_) {
			position_list_model_->Select(h_, ZQDB_HANDLE_TYPE_POSITION);
			auto& position_col_info = position_list_model_->GetColInfo();
			for (size_t i = 0, j = position_col_info.size(); i < j; i++)
			{
				auto& col_info = position_col_info[i];
				position_list_->AppendTextColumn(col_info.name, i);
			}
		}
		m_bookCtrl->AddPage(position_list_, wxT("持仓"), false);

		order_list_ = new wxDataViewCtrl(m_bookCtrl, wxID_ANY);
		order_list_model_ = new MyHZQDBListModel("./mytrader/orderlist.json", XUtil::XML_FLAG_JSON_FILE);
		order_list_->AssociateModel(order_list_model_.get());
		if (h_) {
			order_list_model_->Select(h_, ZQDB_HANDLE_TYPE_ORDER);
			auto& order_col_info = order_list_model_->GetColInfo();
			for (size_t i = 0, j = order_col_info.size(); i < j; i++)
			{
				auto& col_info = order_col_info[i];
				order_list_->AppendTextColumn(col_info.name, i);
			}
		}
		m_bookCtrl->AddPage(order_list_, wxT("委托"), true);
		//m_bookCtrl->AddPage(new wxTextCtrl(m_bookCtrl, wxID_ANY, wxEmptyString,
		//	wxDefaultPosition, wxDefaultSize,
		//	wxTE_MULTILINE | wxTE_READONLY), wxT("委托"), false);

		trade_list_ = new wxDataViewCtrl(m_bookCtrl, wxID_ANY);
		trade_list_model_ = new MyHZQDBListModel("./mytrader/tradelist.json", XUtil::XML_FLAG_JSON_FILE);
		trade_list_->AssociateModel(trade_list_model_.get());
		if (h_) {
			trade_list_model_->Select(h_, ZQDB_HANDLE_TYPE_TRADE);
			auto& trade_col_info = trade_list_model_->GetColInfo();
			for (size_t i = 0, j = trade_col_info.size(); i < j; i++)
			{
				auto& col_info = trade_col_info[i];
				trade_list_->AppendTextColumn(col_info.name, i);
			}
		}
		m_bookCtrl->AddPage(trade_list_, wxT("成交"), false);
	}

	m_bookCtrl->Show();

	delete wxLog::SetActiveTarget(new wxLogTextCtrl(wxDynamicCast(m_bookCtrl->GetPage(0), wxTextCtrl)));
}

int MyFrame::FilterEvent(wxEvent& event)
{
	const wxEventType t = event.GetEventType();
	auto obj = event.GetEventObject();
	if (obj) {
		if (t == wxEVT_MOUSEWHEEL) {
			if (!m_tv->IsAnyOperator()) {
				auto mouse_event = wxDynamicCast(&event, wxMouseEvent);
				auto rotation = mouse_event->GetWheelRotation();
				auto delta = mouse_event->GetWheelDelta();
				auto inverted = mouse_event->IsWheelInverted();
				auto axis = mouse_event->GetWheelAxis();
				if (axis == wxMOUSE_WHEEL_VERTICAL) {
					if (rotation < 0) {
						Up();
						return wxEventFilter::Event_Ignore;
					}
					else if (rotation > 0) {
						Down();
						return wxEventFilter::Event_Ignore;
					}
				}
			}
		}
		else if (t == wxEVT_CHAR) {
			if (obj->IsKindOf(wxCLASSINFO(wxTextCtrlBase))) {
				
			}
			else {
				auto key_event = wxDynamicCast(&event, wxKeyEvent);
				auto key_code = key_event->GetKeyCode();
				switch (key_code)
				{
				/*case WXK_ESCAPE: {
					auto tv = (TechView*)GetParent();
					if (IsShowCrossCursor(disp_info_ptr_->show_cross_cursor)) {
						tv->HideCurrent();
						event.Skip();
					}
				} break;
				case WXK_HOME: {
					auto tv = (TechView*)GetParent();
					auto nBarCount = GetBarCount();
					tv->MoveTo(std::max<int>(0, nBarCount - (disp_info_ptr_->MaxCount - disp_info_ptr_->Shift)));
					if (IsShowCrossCursor(disp_info_ptr_->show_cross_cursor)) {
						tv->MoveCurrentTo(nBarCount - 1);
						event.Skip();
					}
				} break;
				case WXK_END: {
					auto tv = (TechView*)GetParent();
					tv->MoveTo(0);
					if (IsShowCrossCursor(disp_info_ptr_->show_cross_cursor)) {
						tv->MoveCurrentTo(0);
						event.Skip();
					}
				} break;*/
				case WXK_PAGEUP: {
					Up();
					return wxEventFilter::Event_Ignore;
				} break;
				case WXK_PAGEDOWN: {
					Down();
					return wxEventFilter::Event_Ignore;
				} break;
				//case WXK_LEFT: {
				//	auto tv = (TechView*)GetParent();
				//	if (!IsShowCrossCursor(disp_info_ptr_->show_cross_cursor)) {
				//		tv->MoveCurrentTo(disp_info_ptr_->Start);
				//	}
				//	else {
				//		/*if (disp_info_ptr_->Start <= 0 && disp_info_ptr_->Current <= 0) {
				//		RequestMore();
				//		}*/
				//		tv->MoveCurrent(-1);
				//	}
				//	event.Skip();
				//} break;
				//case WXK_RIGHT: {
				//	auto tv = (TechView*)GetParent();
				//	if (!IsShowCrossCursor(disp_info_ptr_->show_cross_cursor)) {
				//		tv->MoveCurrentTo(disp_info_ptr_->Start + disp_info_ptr_->Count - 1);
				//	}
				//	else {
				//		tv->MoveCurrent(1);
				//	}
				//	event.Skip();
				//} break;
				//case WXK_UP: {
				//	auto tv = (TechView*)GetParent();
				//	tv->Scale(1);
				//	tv->UpdateCurrent();
				//	event.Skip();
				//} break;
				//case WXK_DOWN: {
				//	auto tv = (TechView*)GetParent();
				//	tv->Scale(-1);
				//	tv->UpdateCurrent();
				//	event.Skip();
				//} break;
				default: {
					if (std::isalnum(key_code)) {
						auto dlg = wxGetApp().GetSmartKBDlg();
						if (dlg) {
							dlg->ShowFor(this, key_code);
							return wxEventFilter::Event_Ignore;
						}
					}
				} break;
				}
			}
		}
	}
	// Continue processing the event normally as well.
	return wxEventFilter::Event_Skip;
}

void MyFrame::OnSkinInfoChanged()
{
	Freeze();
	//普通显示信息变化
	SetBackgroundColour(skin_info_ptr_->crPrimary);
	SetForegroundColour(skin_info_ptr_->crTertiary);
	//wxColour primary, secondary, tertiary;
	//m_ribbon->SetBackgroundColour();
	/*m_ribbon->GetArtProvider()->GetColourScheme(&primary, &secondary, &tertiary);
#ifdef _DEBUG
	int r, g, b;
	r = primary.Red();
	g = primary.Green();
	b = primary.Blue();
	r = secondary.Red();
	g = secondary.Green();
	b = secondary.Blue();
	r = tertiary.Red();
	g = tertiary.Green();
	b = tertiary.Blue();
#endif*/
	if (skin_info_ptr_->artProvider) {
		m_ribbon->SetArtProvider(skin_info_ptr_->artProvider->Clone());
	}
	else {
		m_ribbon->GetArtProvider()->SetColourScheme(skin_info_ptr_->crPrimary, skin_info_ptr_->crSecondary, skin_info_ptr_->crTertiary);
	}
	code_list_->SetBackgroundColour(skin_info_ptr_->crCtrlBkgnd);
	code_list_->SetForegroundColour(skin_info_ptr_->crCtrlForgnd);
	code_list_code_render_->SetSkinInfo(skin_info_ptr_);
	code_list_name_render_->SetSkinInfo(skin_info_ptr_);
	m_tv->SetSkinInfo(skin_info_ptr_);
	if (title_view_) {
		title_view_->SetSkinInfo(skin_info_ptr_);
	}
	if (mmp_view_) {
		mmp_view_->SetSkinInfo(skin_info_ptr_);
	}
	if (trade_view_) {
		trade_view_->SetSkinInfo(skin_info_ptr_);
	}
	if (info_view_) {
		info_view_->SetSkinInfo(skin_info_ptr_);
	}
	if (tick_view_) {
		tick_view_->SetSkinInfo(skin_info_ptr_);
	}
	//m_togglePanels->SetBackgroundColour(skin_info_ptr_->crXYLine);
	/*Broadcast(this, [this](wxWindow* child) {
		if (child != m_tv && child != m_ribbon) {
			Broadcast(child, [this](wxWindow* child) {
				child->SetBackgroundColour(skin_info_ptr_->crBackgnd);
			}, true);
			child->SetBackgroundColour(skin_info_ptr_->crXYLine);
		}
	});*/
	auto statusbar = GetStatusBar();
	if (statusbar) {
		statusbar->SetBackgroundColour(skin_info_ptr_->crPrimary);
		statusbar->SetForegroundColour(skin_info_ptr_->crTertiary);
	}
	Layout();
	Thaw();
}

void MyFrame::OnInfoChanged()
{
	code_list_model_->Show(info_ptr_->h_list_);
	auto nCurItemPos = info_ptr_->GetCurItemPos();
	if (nCurItemPos < code_list_model_->GetCount()) {
		code_list_->Select(code_list_model_->GetItem(nCurItemPos));
	}
	OnCurItemChanged();
}

void MyFrame::OnCurItemChanged()
{
	Freeze();
	auto tv = (zqdb::TechView*)m_tv;
	auto pos = GetCurItemPos();
	auto h = GetItem(pos);
	auto data = std::make_shared<zqdb::Calc::Data>(h, CYC_1MIN, 0, 0);
	tagCodeInfo* code = (tagCodeInfo*)data->GetValue();
	wxCSConv utf8cv(wxFONTENCODING_UTF8);
	wxString title = wxString::Format(wxT("%s %s")
		, utf8cv.cMB2WX(code->Name).data()
		, utf8cv.cMB2WX(code->Code).data()
	);
	SetTitle(title);
	code_list_->Select(code_list_model_->GetItem(pos));
	tv->SetCalcData(data);
	if (title_view_) {
		title_view_->SetHandle(h);
	}
	if (mmp_view_) {
		mmp_view_->SetHandle(h);
	}
	if (trade_view_) {
		trade_view_->SetHandle(h);
	}
	if (info_view_) {
		info_view_->SetHandle(h);
	}
	if (tick_view_) {
		tick_view_->SetHandle(h);
	}

	auto info = GetInfo();
	size_t i = 0, j = record_list_.size();
	for (; i < j; ++i)
	{
		if (record_list_[i].info == info && record_list_[i].pos == info->GetCurItemPos()) {
			auto record = record_list_[i];
			size_t k = i;
			for (; (k + 1) < j; ++k) {
				record_list_[k] = record_list_[k + 1];
			}
			record_pos_ = k;
			record_list_[k] = record;
			break;
		}
	}
	if (i >= j) {
		record_pos_ = record_list_.size();
		record_list_.push_back(info);
	}
	Layout();
	Thaw();
}

void MyFrame::OnHandleChanged()
{

}

void MyFrame::Back()
{
	if (record_pos_ > 0 && !record_list_.empty()) {
		--record_pos_;
		auto record = record_list_[record_pos_];
		record.info->SetCurItemPos(record.pos);
		SetInfo(record.info);
	}
}

void MyFrame::Forward()
{
	if ((record_pos_ + 1) >= record_list_.size()) {
		++record_pos_;
		auto record = record_list_[record_pos_];
		record.info->SetCurItemPos(record.pos);
		SetInfo(record.info);
	}
}

void MyFrame::Up()
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

void MyFrame::Down()
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

void MyFrame::UpdateStatus()
{
	wxCSConv utf8cv(wxFONTENCODING_UTF8);

	zqdb::AllExchange all_exchange;
	zqdb::AllStatus all_status;

#if wxUSE_STATUSBAR
	auto statusbar = GetStatusBar();
	if (statusbar) {
		std::vector<int> widths;
		widths.resize(1 + all_exchange.size() + all_status.size());
		widths[0] = -1;
		for (size_t i = 1, j = 1 + all_exchange.size(); i < j; ++i)
		{
			widths[i] = 80;
		}
		for (size_t i = 1 + all_exchange.size(), j = widths.size(); i < j; ++i)
		{
			widths[i] = 40;
		}
		statusbar->SetFieldsCount(widths.size(), widths.data());
		for (size_t i = 1, j = 1 + all_exchange.size(); i < j; ++i)
		{
			zqdb::Exchange exchange(all_exchange[i - 1]);
			auto info = exchange.GetValue();
			statusbar->SetStatusText(utf8cv.cMB2WX(info->Name), i);
		}
		for (size_t i = 1 + all_exchange.size(), j = widths.size(); i < j; ++i)
		{
			auto info = all_status[i - (1 + all_exchange.size())];
			statusbar->SetStatusText(utf8cv.cMB2WX(info.Name), i);
		}
	}
#endif // wxUSE_STATUSBAR
}

void MyFrame::OnGoto(wxCommandEvent& event)
{
	auto techfrm = this;
	auto info_ptr = std::make_shared<zqdb::TechContainerInfo>(techfrm);
	auto h = (HZQDB)event.GetClientData();
	switch (h->type)
	{
	case ZQDB_HANDLE_TYPE_EXCHANGE:
	case ZQDB_HANDLE_TYPE_PRODUCT: {
		zqdb::AllCode all(h);
		info_ptr->Assign(all.data(), all.size());
	} break;
	case ZQDB_HANDLE_TYPE_CODE: {
		info_ptr->Assign(&h, 1);
	} break;
	default:
		ASSERT(0);
		return;
		break;
	}
	techfrm->SetInfo(info_ptr);
}

void MyFrame::ShowView(wxWindow* page)
{
	//切换显示视图
	auto old_page = m_bookCtrl->GetCurrentPage();
	if (old_page != page) {
		for (size_t i = 0, j = m_bookCtrl->GetPageCount(); i < j; i++)
		{
			if (m_bookCtrl->GetPage(i) == page)
			{
				m_bookCtrl->SetSelection(i);
				break;
			}
		}
	}
	//初始化数据
	if (page == position_list_) {
		zqdb::AllPosition positions(h_);
		position_list_model_->Show(positions);
	}
	else if (page == order_list_) {
		zqdb::AllOrder orders(h_);
		order_list_model_->Show(orders);
	}
	else if (page == trade_list_) {
		zqdb::AllTrade trades(h_);
		trade_list_model_->Show(trades);
	}
}

void MyFrame::DoViewNotifyEnable(HZQDB h)
{
	auto tv = (zqdb::TechView*)m_tv; 
	tv->OnNotifyEnable(h);
	if (title_view_) {
		title_view_->OnNotifyEnable(h);
	}
	if (mmp_view_) {
		mmp_view_->OnNotifyEnable(h);
	}
	if (trade_view_) {
		trade_view_->OnNotifyEnable(h);
	}
	if (info_view_) {
		info_view_->OnNotifyEnable(h);
	}
	if (tick_view_) {
		tick_view_->OnNotifyEnable(h);
	}
	auto book_page = m_bookCtrl->GetCurrentPage();
	if (book_page == order_list_ && ZQDBGetParent(h_) == h) {
		zqdb::AllOrder orders(h_);
		order_list_model_->Show(orders);
	}
}

void MyFrame::DoViewNotifyDisable(HZQDB h)
{
	auto tv = (zqdb::TechView*)m_tv;
	tv->OnNotifyDisable(h);
	if (title_view_) {
		title_view_->OnNotifyDisable(h);
	}
	if (mmp_view_) {
		mmp_view_->OnNotifyDisable(h);
	}
	if (trade_view_) {
		trade_view_->OnNotifyDisable(h);
	}
	if (info_view_) {
		info_view_->OnNotifyDisable(h);
	}
	if (tick_view_) {
		tick_view_->OnNotifyDisable(h);
	}
	auto book_page = m_bookCtrl->GetCurrentPage();
	if (book_page == order_list_ && ZQDBGetParent(h_) == h) {
		order_list_model_->Clear();
	}
}

void MyFrame::DoViewNotifyAppend(HZQDB h)
{
	auto tv = (zqdb::TechView*)m_tv;
	tv->OnNotifyAppend(h);
	if (title_view_) {
		title_view_->OnNotifyAppend(h);
	}
	if (mmp_view_) {
		mmp_view_->OnNotifyAppend(h);
	}
	if (trade_view_) {
		trade_view_->OnNotifyAppend(h);
	}
	if (info_view_) {
		info_view_->OnNotifyAppend(h);
	}
	if (tick_view_) {
		tick_view_->OnNotifyAppend(h);
	}
}

void MyFrame::DoViewNotifyRemove(HZQDB h)
{
	auto tv = (zqdb::TechView*)m_tv;
	tv->OnNotifyRemove(h);
	if (title_view_) {
		title_view_->OnNotifyRemove(h);
	}
	if (mmp_view_) {
		mmp_view_->OnNotifyRemove(h);
	}
	if (trade_view_) {
		trade_view_->OnNotifyRemove(h);
	}
	if (info_view_) {
		info_view_->OnNotifyRemove(h);
	}
	if (tick_view_) {
		tick_view_->OnNotifyRemove(h);
	}
}

void MyFrame::DoViewNotifyUpdate(HZQDB h)
{
	auto tv = (zqdb::TechView*)m_tv;
	tv->OnNotifyUpdate(h);
	if (title_view_) {
		title_view_->OnNotifyUpdate(h);
	}
	if (mmp_view_) {
		mmp_view_->OnNotifyUpdate(h);
	}
	if (trade_view_) {
		trade_view_->OnNotifyUpdate(h);
	}
	if (info_view_) {
		info_view_->OnNotifyUpdate(h);
	}
	if (tick_view_) {
		tick_view_->OnNotifyUpdate(h);
	}
	auto book_page = m_bookCtrl->GetCurrentPage();
	if (book_page == order_list_ && h->type == ZQDB_HANDLE_TYPE_ORDER) {
		order_list_->Refresh();
	}
}

void MyFrame::Activate(const wxDataViewItem& item)
{
	if (item) {
		auto index = code_list_model_->GetRow(item);
		SetCurItemPos(index);
	}
}

void MyFrame::OnActivated(wxDataViewEvent &event)
{
	Activate(event.GetItem());
}

void MyFrame::OnNotify(wxCommandEvent& event)
{
	HZQDB h = (HZQDB)event.GetClientData();
	if (h) {
		if (event.GetEventType() == ZQDB_NOTIFY_ENABLE_EVENT)
		{
			DoViewNotifyEnable(h);
		}
		else if (event.GetEventType() == ZQDB_NOTIFY_DISABLE_EVENT)
		{
			DoViewNotifyDisable(h);
		}
		else if (event.GetEventType() == ZQDB_NOTIFY_APPEND_EVENT)
		{
			DoViewNotifyAppend(h);
		}
		else if (event.GetEventType() == ZQDB_NOTIFY_REMOVE_EVENT)
		{
			DoViewNotifyRemove(h);
		}
		else if (event.GetEventType() == ZQDB_NOTIFY_UPDATE_EVENT)
		{
			DoViewNotifyUpdate(h);
		}
		//tv->OnNotify(event.GetString(), event.GetInt(), event.GetExtraLong(), h);
		/*auto calc_data_ptr = tv->GetCalcData();
		if (calc_data_ptr && calc_data_ptr->GetBase() == code) {
			tv->UpdateCalcData();
		}*/
	}
}

void MyFrame::OnNavigateHome(wxRibbonButtonBarEvent& WXUNUSED(evt))
{
}

void MyFrame::OnNavigateBackward(wxRibbonButtonBarEvent& WXUNUSED(evt))
{
	Back();
}

void MyFrame::OnNavigateBackwardDropdown(wxRibbonButtonBarEvent& evt)
{
	if (record_pos_ > 0) {
		wxCSConv utf8cv(wxFONTENCODING_UTF8);

		wxMenu menu;
		for (size_t i = 0; i < record_pos_; i++)
		{
			zqdb::Code code(record_list_[i].info->GetItem(record_list_[i].pos));
			auto info = code.GetValue();
			menu.Append(ID_NAVIGATE_GOTO + i, utf8cv.cMB2WX(info->Name));
		}
		evt.PopupMenu(&menu);
	}
}

void MyFrame::OnNavigateBackwardUpdateUI(wxUpdateUIEvent& evt)
{
	evt.Enable(record_pos_ > 0);
}

void MyFrame::OnNavigateForward(wxRibbonButtonBarEvent& WXUNUSED(evt))
{
	Forward();
}

void MyFrame::OnNavigateForwardDropdown(wxRibbonButtonBarEvent& evt)
{
	wxMenu menu;
	menu.Append(wxID_ANY, "Equilateral");
	menu.Append(wxID_ANY, "Isosceles");
	menu.Append(wxID_ANY, "Scalene");

	evt.PopupMenu(&menu);
}

void MyFrame::OnNavigateUp(wxRibbonButtonBarEvent& WXUNUSED(evt))
{
	Up();
}

void MyFrame::OnNavigateDown(wxRibbonButtonBarEvent& WXUNUSED(evt))
{
	Down();
}

void MyFrame::OnNavigateFind(wxRibbonButtonBarEvent& WXUNUSED(evt))
{
	auto dlg = wxGetApp().GetSmartKBDlg();
	if (dlg) {
		dlg->ShowFor(this);
	}
}

void MyFrame::OnMarket(wxRibbonButtonBarEvent& WXUNUSED(evt))
{
}

void MyFrame::OnMarketDropdown(wxRibbonButtonBarEvent& evt)
{
	wxMenu menu;
	menu.Append(wxID_ANY, "Equilateral");
	menu.Append(wxID_ANY, "Isosceles");
	menu.Append(wxID_ANY, "Scalene");

	evt.PopupMenu(&menu);
}

//
//void MyFrame::OnNew(wxRibbonToolBarEvent& WXUNUSED(evt))
//{
//    AddText("New button clicked.");
//}
//
//void MyFrame::OnNewDropdown(wxRibbonToolBarEvent& evt)
//{
//    wxMenu menu;
//    menu.Append(wxID_ANY, "New Document");
//    menu.Append(wxID_ANY, "New Template");
//    menu.Append(wxID_ANY, "New Mail");
//
//    evt.PopupMenu(&menu);
//}

// This shows how to hide ribbon dynamically if there is not enough space.
void MyFrame::OnSizeEvent(wxSizeEvent& evt)
{
    if ( evt.GetSize().GetWidth() < 200 )
        m_ribbon->Hide();
    else
        m_ribbon->Show();

    evt.Skip();
}

void MyFrame::OnCloseEvent(wxCloseEvent& evt)
{
	evt.Skip();
}

void MyFrame::OnTimer(wxTimerEvent& evt)
{

}

