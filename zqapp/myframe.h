
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

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

#include <view.h>
#include "mydatamodel.h"
#include "mydataview.h"

// -- frame --

class MyFrame : public wxFrame
	, public zqdb::SkinMap<MyFrame, SkinInfo>
	, public zqdb::TechContainerMap<MyFrame, zqdb::TechContainerInfo>
	, public zqdb::HandleMap<MyFrame>
{
	typedef wxFrame Base;
	typedef zqdb::SkinMap<MyFrame, SkinInfo> SkinBase;
	typedef zqdb::TechContainerMap<MyFrame, zqdb::TechContainerInfo> ContainerMap;
	typedef zqdb::HandleMap<MyFrame> UserBase;
protected:
	wxRibbonBar* m_ribbon;
	//wxTextCtrl* m_logwindow;
	wxToggleButton* m_togglePanels;
	struct MyRecord
	{
		std::shared_ptr<zqdb::TechContainerInfo> info;
		size_t pos;
		MyRecord(std::shared_ptr<zqdb::TechContainerInfo> i):info(i), pos(i->GetCurItemPos()) {}
	};
	std::vector<MyRecord> record_list_;
	size_t record_pos_ = 0;
	wxDataViewCtrl* code_list_;
	MyCodeListCodeRenderer* code_list_code_render_ = nullptr;
	MyCodeListNameRenderer* code_list_name_render_ = nullptr;
	wxObjectDataPtr<MyCodeListModel> code_list_model_;
	zqdb::TechView *m_tv;
	wxString user_; //USER.BROKER
	wxDataViewCtrl* position_list_ = nullptr;
	wxDataViewCtrl* order_list_ = nullptr;
	wxDataViewCtrl* trade_list_ = nullptr;
	wxObjectDataPtr<MyHZQDBListModel> position_list_model_;
	wxObjectDataPtr<MyHZQDBListModel> order_list_model_;
	wxObjectDataPtr<MyHZQDBListModel> trade_list_model_;
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
	void RecreateBook();
	/*class EventHandler : public wxEvtHandler
	{
	public:
		EventHandler(MyFrame* frame);
	private:
		void SetChildNextHandler(wxWindow* child);
		void OnChar(wxKeyEvent& event);
		wxDECLARE_EVENT_TABLE();
		wxDECLARE_NO_COPY_CLASS(EventHandler);
	};*/

	//topview or rightview
	zqdb::TitleView* title_view_ = nullptr;
	zqdb::MmpView* mmp_view_ = nullptr;
	zqdb::TradeView* trade_view_ = nullptr;
	zqdb::InfoView* info_view_ = nullptr;
	zqdb::TickView* tick_view_ = nullptr;

	wxTimer timer_;
public:
    MyFrame(const char* xml, size_t xmlflag);
    ~MyFrame();

	int FilterEvent(wxEvent& event);

	void OnSkinInfoChanged();
	void OnInfoChanged();
	void OnCurItemChanged();
	void OnHandleChanged();

	void Back();
	void Forward();
	void Up();
	void Down();

	void UpdateStatus();

protected:
	//
	void OnGoto(wxCommandEvent& event);

	void ShowView(wxWindow* page);
	void DoViewNotifyEnable(HZQDB h);
	void DoViewNotifyDisable(HZQDB h);
	void DoViewNotifyAppend(HZQDB h);
	void DoViewNotifyRemove(HZQDB h);
	void DoViewNotifyUpdate(HZQDB h);

	void Activate(const wxDataViewItem& item);
	void OnActivated(wxDataViewEvent &event);
	void OnNotify(wxCommandEvent& event);

    enum
    {
		ID_NAVIGATE_BEGIN = wxID_HIGHEST + 1,
		ID_NAVIGATE_HOME,
		ID_NAVIGATE_BACKWARD,
		ID_NAVIGATE_FORWARD,
		ID_NAVIGATE_UP,
		ID_NAVIGATE_DOWN,
		ID_NAVIGATE_FIND,
		ID_NAVIGATE_GOTO,
		ID_NAVIGATE_GOTO_MAX = ID_NAVIGATE_GOTO + 100,
		ID_NAVIGATE_END,
		ID_MARKET_BEGIN,
        ID_MARKET_SELF,
		ID_MARKET_MAIN,
        ID_MARKET_ALL,
		ID_MARKET_EXCHANGE,
		ID_MARKET_EXCHANGE_MAX = ID_MARKET_EXCHANGE + 32,
        ID_MARKET_PRODUCT,
		ID_MARKET_PRODUCT_MAX = ID_MARKET_PRODUCT + 256,
		ID_MARKET_END,
		ID_TRADE_BEGIN,
        ID_TRADE_QUICK_BUY_FAK,
		ID_TRADE_QUICK_BUY_FOK,
		ID_TRADE_QUICK_SELL_FAK,
		ID_TRADE_QUICK_SELL_FOK,
		ID_TRADE_QUICK_CLOSE,
		ID_TRADE_POSITION,
		ID_TRADE_ORDER,
		ID_TRADE_TRADE,
		ID_TRADE_ADDUSER,
		ID_TRADE_REMOVEUSER,
		ID_TRADE_END,
		ID_TECHVIEW_BEGIN,
		ID_TECHVIEW_ADD_INDICATOR,
		ID_TECHVIEW_REMOVE_INDICATOR,
		ID_TECHVIEW_CYCLE,
		ID_TECHVIEW_CYCLE_MAX = ID_TECHVIEW_CYCLE + CYC_MAX,
		ID_TECHVIEW_TEMPLATE,
		ID_TECHVIEW_TEMPLATE_MAX = ID_TECHVIEW_TEMPLATE + 100,
		ID_TECHVIEW_MOVE_LAST,
		ID_TECHVIEW_MOVE_BACK,
		ID_TECHVIEW_MOVE_FORWARD,
		ID_TECHVIEW_MOVE_FIRST,
		ID_TECHVIEW_DRAWLINE,
		ID_TECHVIEW_DRAWLINE_MAX = ID_TECHVIEW_DRAWLINE + CALC_DRAWLINE_TYPE_MAX,
		ID_TECHVIEW_ZOOM_IN,
		ID_TECHVIEW_ZOOM_OUT,
		ID_TECHVIEW_ZOOM_RESET,
		ID_TECHVIEW_END,
		ID_INFO_F10,
		ID_TOOL_CALC,
		ID_HELP_HOME,
		ID_HELP_DISCUSZ,
		ID_HELP_ABOUT,
    };
	//Navigate
    void OnNavigateHome(wxRibbonButtonBarEvent& evt);
    void OnNavigateBackward(wxRibbonButtonBarEvent& evt);
    void OnNavigateBackwardDropdown(wxRibbonButtonBarEvent& evt);
	void OnNavigateBackwardUpdateUI(wxUpdateUIEvent& evt);
    void OnNavigateForward(wxRibbonButtonBarEvent& evt);
    void OnNavigateForwardDropdown(wxRibbonButtonBarEvent& evt);
    void OnNavigateUp(wxRibbonButtonBarEvent& evt);
    void OnNavigateDown(wxRibbonButtonBarEvent& evt);
    void OnNavigateFind(wxRibbonButtonBarEvent& evt);
	//Market
	void OnMarket(wxRibbonButtonBarEvent& evt);
	void OnMarketDropdown(wxRibbonButtonBarEvent& evt);

    void OnSizeEvent(wxSizeEvent& evt);
	void OnCloseEvent(wxCloseEvent& evt);
	void OnTimer(wxTimerEvent& evt);

    wxDECLARE_EVENT_TABLE();
};

