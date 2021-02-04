#include "view.hpp"
#include <math.h>
#include <mdbdf.hpp>
#include "wx/artprov.h"
#include "wx/ribbon/bar.h"
#include "wx/ribbon/buttonbar.h"
#include "wx/ribbon/gallery.h"
#include "wx/ribbon/toolbar.h"
#include <zq.pb.h>

//
//wxDEFINE_EVENT(ZQDB_CODE_PREV_EVENT, wxCommandEvent);
//wxDEFINE_EVENT(ZQDB_CODE_NEXT_EVENT, wxCommandEvent);

namespace zqdb {

	static wxRibbonMSWArtProvider g_ArgProvider;

	Rect::Rect(const wxRect& o):Base(o.x,o.y,o.width,o.height)
	{

	}

	Rect& Rect::Inflate(wxCoord left, wxCoord top, wxCoord right, wxCoord bottom)
	{
		x -= left;
		y -= top;
		width += left + right;
		height += top + bottom;
		return *this;
	}

	Rect& Rect::InflateX(wxCoord left, wxCoord right)
	{
		x -= left;
		width += left + right;
		return *this;
	}

	Rect& Rect::InflateY(wxCoord top, wxCoord bottom)
	{
		y -= top;
		height += top + bottom;
		return *this;
	}

	Rect& Rect::InflateLeft(wxCoord d)
	{
		x -= d;
		width += d;
		return *this;
	}

	Rect& Rect::InflateRight(wxCoord d)
	{
		width += d;
		return *this;
	}

	Rect& Rect::InflateTop(wxCoord d)
	{
		y -= d;
		height += d;
		return *this;
	}

	Rect& Rect::InflateBottom(wxCoord d)
	{
		height += d;
		return *this;
	}

	void SkinInfo::SetFieldName(const mdb::Field& field, const wxString& name)
	{
		field_names_[field] = name;
	}

	const wxString& SkinInfo::GetFieldName(const mdb::Field& field)
	{
		auto it = field_names_.find(field);
		if (it != field_names_.end()) {
			return it->second;
		}
		static wxString _null_name;
		return _null_name;
	}
	
	///

	TitleView::TitleView(wxWindow* parent, const char* xml, size_t xmlflag) : Base(parent, xml, xmlflag)
	{

	}

	TitleView::~TitleView()
	{

	}

	void TitleView::OnSkinInfoChanged()
	{
		Base::OnSkinInfoChanged();
		CalcBestSize();
	}

	void TitleView::CalcBestSize()
	{
		if (!skin_info_ptr_) {
			return;
		}
		if (best_size_ != wxSize(0, 0)) {
			return;
		}

		best_size_.x = skin_info_ptr_->xySpace.x + skin_info_ptr_->xyName.x * 5
			+ skin_info_ptr_->xySpace.x + skin_info_ptr_->xyName.x * 5 + skin_info_ptr_->xySpace.x;
		best_size_.y = skin_info_ptr_->xySpace.y + skin_info_ptr_->xyName.y + skin_info_ptr_->xySpace.y;
		auto min_size = GetMinSize();
		if (min_size != best_size_) {
			SetMinSize(best_size_);
		}
	}

	void TitleView::Draw(wxDC& dc)
	{
		wxCSConv utf8cv(wxFONTENCODING_UTF8);

		zqdb::Code code(h_);
		auto info = (CODEINFO*)code.GetValue();
		auto rcClient = GetClientRect();

		dc.SetFont(skin_info_ptr_->fontText);
		dc.SetTextForeground(skin_info_ptr_->crCommodityName);
		dc.DrawLabel(utf8cv.cMB2WX(info->Name), rcClient, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
		//UIgdi::DrawTextA(hdc, szTitle, nTitleLen, &rc, DT_RIGHT | DT_VCENTER | DT_SINGLELINE, m_DispInfoPtr->crCommodityName, m_DispInfoPtr->hText);
	
		dc.SetFont(skin_info_ptr_->fontText);
		dc.SetTextForeground(skin_info_ptr_->crCommodityCode);
		dc.DrawLabel(utf8cv.cMB2WX(info->Code), rcClient, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
		//UIgdi::DrawTextA(hdc, szTitle, nTitleLen, &rc, DT_LEFT | DT_VCENTER | DT_SINGLELINE, m_DispInfoPtr->crCommodityCode, m_DispInfoPtr->hText);
	}

	wxBEGIN_EVENT_TABLE(TitleView, Base)
	EVT_ERASE_BACKGROUND(TitleView::OnErase)
	EVT_PAINT(TitleView::OnPaint)
	wxEND_EVENT_TABLE()

	void TitleView::OnErase(wxEraseEvent &event)
	{

	}

	void TitleView::OnPaint(wxPaintEvent &event)
	{
		wxPaintDC dc(this);
		dc.Clear();
		if (IsDispOk()) {
			Draw(dc);
		}
	}

	///

	MmpView::MmpView(wxWindow* parent, const char* xml, size_t xmlflag) : Base(parent, xml, xmlflag)
	{

	}

	MmpView::~MmpView()
	{

	}

	void MmpView::OnSkinInfoChanged()
	{
		Base::OnSkinInfoChanged();
		CalcBestSize();
	}

	void MmpView::CalcBestSize()
	{
		if (!skin_info_ptr_) {
			return;
		}
		if (best_size_ != wxSize(0, 0)) {
			return;
		}

		best_size_.x = skin_info_ptr_->xySpace.x + skin_info_ptr_->xyName.x * 8
			+ skin_info_ptr_->xySpace.x + skin_info_ptr_->xyName.x * 8 + skin_info_ptr_->xySpace.x;
		best_size_.y = skin_info_ptr_->xySpace.y + skin_info_ptr_->xyName.y
			+ skin_info_ptr_->xySpace.y + skin_info_ptr_->xySpace.y
			+ skin_info_ptr_->xySpace.y + skin_info_ptr_->xyName.y;
		auto min_size = GetMinSize();
		if (min_size != best_size_) {
			SetMinSize(best_size_);
		}
	}

	void MmpView::Draw(wxDC& dc)
	{
		wxCSConv utf8cv(wxFONTENCODING_UTF8);

		zqdb::Code code(h_);
		auto info = (CODEINFO*)code.GetValue();
		auto rcClient = GetClientRect();

		auto ptCenter = rcClient.GetLeftTop();
		ptCenter.x += rcClient.width / 2;
		ptCenter.y += rcClient.height / 2;

		auto rcText = rcClient;
		rcText.height = skin_info_ptr_->xyName.y + skin_info_ptr_->xySpace.y;
		for (size_t i = 0; i < level_; i++)
		{
			wxString name;
			double price = INVALID_VALUE;
			double volume = INVALID_VALUE;
			double voldif = INVALID_VALUE;
			if (i == 0) {
				name = wxT("卖一");
				//price = code.GetFieldValueAsDouble(mdb::Field(STR_ZQDB_FIELD_ASK1), INVALID_VALUE);
				//volume = code.GetFieldValueAsDouble(mdb::Field(STR_ZQDB_FIELD_ASKV1), INVALID_VALUE);
				//voldif = code.GetFieldValueAsDouble(mdb::Field(STR_ZQDB_FIELD_ASKX1), INVALID_VALUE);
			}
			Rect rcName, rcPrice, rcVolume, rcVolDif;
			rcName = rcPrice = rcVolume = rcVolDif = rcText;
			rcName.width = rcClient.width / 2;
			rcPrice = rcName;
			rcVolume.x = ptCenter.x; rcVolume.width = rcClient.width / 2;
			rcVolDif = rcVolume;

			dc.SetFont(skin_info_ptr_->fontText);
			dc.SetTextForeground(skin_info_ptr_->crFalling);
			if (IsInvalidValue(price) || IsInvalidValue(volume) || IsInvalidValue(voldif)) {
				dc.DrawLabel(name, rcName, wxALIGN_LEFT | wxALIGN_BOTTOM);
				dc.DrawLabel(wxT("——"), rcPrice, wxALIGN_RIGHT | wxALIGN_BOTTOM);
				dc.DrawLabel(wxT("——"), rcVolume, wxALIGN_LEFT | wxALIGN_BOTTOM);
			}
			else {
				dc.DrawLabel(name, rcName, wxALIGN_LEFT | wxALIGN_BOTTOM);
				dc.DrawLabel(wxString::Format("%.2f", price), rcPrice, wxALIGN_RIGHT | wxALIGN_BOTTOM);
				dc.DrawLabel(wxString::Format("%.0f", volume), rcVolume, wxALIGN_LEFT | wxALIGN_BOTTOM);
				if (!IsZeroFloat(voldif)) {
					dc.SetTextForeground(skin_info_ptr_->GetColor(voldif));
					dc.DrawLabel(wxString::Format("%+.0f", voldif), rcVolDif, wxALIGN_RIGHT | wxALIGN_BOTTOM);
				}
			}
			rcText.y = rcText.GetBottom();
		}
		dc.SetPen(skin_info_ptr_->penXYLine);
		dc.DrawLine(rcText.x, rcText.y, rcText.GetRight(), rcText.y);
		for (size_t i = 0; i < level_; i++)
		{
			wxString name;
			double price = INVALID_VALUE;
			double volume = INVALID_VALUE;
			double voldif = INVALID_VALUE;
			if (i == 0) {
				name = wxT("买一");
				//price = code.GetFieldValueAsDouble(mdb::Field(STR_ZQDB_FIELD_BID1), INVALID_VALUE);
				//volume = code.GetFieldValueAsDouble(mdb::Field(STR_ZQDB_FIELD_BIDV1), INVALID_VALUE);
				//voldif = code.GetFieldValueAsDouble(mdb::Field(STR_ZQDB_FIELD_BIDX1), INVALID_VALUE);
			}

			Rect rcName, rcPrice, rcVolume, rcVolDif;
			rcName = rcPrice = rcVolume = rcVolDif = rcText;
			rcName.width = rcClient.width / 2;
			rcPrice = rcName;
			rcVolume.x = ptCenter.x; rcVolume.width = rcClient.width / 2;
			rcVolDif = rcVolume;

			dc.SetFont(skin_info_ptr_->fontText);
			dc.SetTextForeground(skin_info_ptr_->crRising);
			if (IsInvalidValue(price) || IsInvalidValue(volume) || IsInvalidValue(voldif)) {
				dc.DrawLabel(name, rcName, wxALIGN_LEFT | wxALIGN_BOTTOM);
				dc.DrawLabel(wxT("——"), rcPrice, wxALIGN_RIGHT | wxALIGN_BOTTOM);
				dc.DrawLabel(wxT("——"), rcVolume, wxALIGN_LEFT | wxALIGN_BOTTOM);
			}
			else {
				dc.DrawLabel(name, rcName, wxALIGN_LEFT | wxALIGN_BOTTOM);
				dc.DrawLabel(wxString::Format("%.2f", price), rcPrice, wxALIGN_RIGHT | wxALIGN_BOTTOM);
				dc.DrawLabel(wxString::Format("%.0f", volume), rcVolume, wxALIGN_LEFT | wxALIGN_BOTTOM);
				if (!IsZeroFloat(voldif)) {
					dc.SetTextForeground(skin_info_ptr_->GetColor(voldif));
					dc.DrawLabel(wxString::Format("%+.0f", voldif), rcVolDif, wxALIGN_RIGHT | wxALIGN_BOTTOM);
				}
			}
			rcText.y = rcText.GetBottom();
		}
	}

	wxBEGIN_EVENT_TABLE(MmpView, Base)
	EVT_ERASE_BACKGROUND(MmpView::OnErase)
	EVT_PAINT(MmpView::OnPaint)
	wxEND_EVENT_TABLE()

	void MmpView::OnErase(wxEraseEvent &event)
	{

	}

	void MmpView::OnPaint(wxPaintEvent &event)
	{
		wxPaintDC dc(this);
		dc.Clear();
		if (IsDispOk()) {
			Draw(dc);
		}
	}

	///

	TradeView::TradeView(wxWindow* parent, const char* xml, size_t xmlflag) : Base(parent, xml, xmlflag)
	{
		cmb_user_ = new wxComboBox(this, wxID_ANY,
			wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, nullptr, wxCB_READONLY);
		wxArrayString users;
		zqdb::AllUser alluser;
		for (size_t i = 0; i < alluser.size(); i++)
		{
			zqdb::Object user(alluser[i]);
			auto info = (tagUserInfo*)user.GetValue();
			users.Add(info->User);
		}
		cmb_user_->Set(users, (void**)&alluser[0]);
		cmb_user_->Select(0);
		btn_price_decr_ = new wxButton(this, wxID_ANY, wxT("-"), wxDefaultPosition, wxDefaultSize);
		text_price_ = new wxTextCtrl(this, wxID_ANY, wxEmptyString,
			wxDefaultPosition, wxDefaultSize, wxTE_CENTER | wxTE_BESTWRAP);
		btn_price_incr_ = new wxButton(this, wxID_ANY, wxT("+"), wxDefaultPosition, wxDefaultSize);
		btn_volume_decr_ = new wxButton(this, wxID_ANY, wxT("-"), wxDefaultPosition, wxDefaultSize);
		text_volume_ = new wxTextCtrl(this, wxID_ANY, wxEmptyString,
			wxDefaultPosition, wxDefaultSize, wxTE_CENTER | wxTE_BESTWRAP);
		btn_volume_incr_ = new wxButton(this, wxID_ANY, wxT("+"), wxDefaultPosition, wxDefaultSize);
		//text_price_->Show(false);
		//text_volume_->Show(false);
		btn_buy_ = new wxButton(this, wxID_ANY, wxT("买开"), wxDefaultPosition, wxDefaultSize);
		btn_close_ = new wxButton(this, wxID_ANY, wxT("平仓"), wxDefaultPosition, wxDefaultSize);
		btn_sell_ = new wxButton(this, wxID_ANY, wxT("卖开"), wxDefaultPosition, wxDefaultSize);

		wxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
		topSizer->Add(cmb_user_, 0, wxEXPAND);
		wxSizer* priceSizer = new wxBoxSizer(wxHORIZONTAL);
		priceSizer->Add(btn_price_decr_, 0, wxEXPAND);
		priceSizer->Add(text_price_, 1, wxEXPAND);
		priceSizer->Add(btn_price_incr_, 0, wxEXPAND);
		topSizer->Add(priceSizer, 0, wxEXPAND);
		wxSizer* volumeSizer = new wxBoxSizer(wxHORIZONTAL);
		volumeSizer->Add(btn_volume_decr_, 0, wxEXPAND);
		volumeSizer->Add(text_volume_, 1, wxEXPAND);
		volumeSizer->Add(btn_volume_incr_, 0, wxEXPAND);
		topSizer->Add(volumeSizer, 0, wxEXPAND);
		wxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);
		btnSizer->Add(btn_buy_, 1, wxEXPAND);
		btnSizer->Add(btn_close_, 1, wxEXPAND);
		btnSizer->Add(btn_sell_, 1, wxEXPAND);
		topSizer->Add(btnSizer, 0, wxEXPAND);
		SetSizer(topSizer);
		//topSizer->Fit(this);          // 调用对话框大小
		//topSizer->SetSizeHints(this); // 设置对话框最小大小
		btn_buy_->Bind(wxEVT_BUTTON, &TradeView::OnBtnBuy, this);
	}

	TradeView::~TradeView()
	{
		btn_buy_->Unbind(wxEVT_BUTTON, &TradeView::OnBtnBuy, this);
	}

	void TradeView::OnSkinInfoChanged()
	{
		Base::OnSkinInfoChanged();
		CalcBestSize();
	}
	//
	//void TradeView::OnCalcDataChanged()
	//{
	//	Refresh();
	//	CalcBestSize();
	//}

	void TradeView::OnNotifyUpdate(HZQDB h)
	{
		Base::OnNotifyUpdate(h);
		/*if (name.Find(STR_ZQDB_TABLE_USER) == 0) {
			auto strUser = cmb_user_->GetStringSelection();
			wxArrayString users;
			zqdb::AllUser alluser;
			for (size_t i = 0; i < alluser.size(); i++)
			{
				zqdb::Object user(alluser[i]);
				auto info = (tagUserInfo*)user.GetValue();
				users.Add(info->User);
			}
			cmb_user_->Set(users, (void**)&alluser[0]);
			int pos = cmb_user_->FindString(strUser);
			cmb_user_->Select(pos >= 0 ? pos : 0);
		}*/
	}

	void TradeView::CalcBestSize()
	{
		if (!skin_info_ptr_) {
			return;
		}

		wxSize szPriceDecr = btn_price_decr_->GetSize();
		wxSize szPriceIncr = btn_price_incr_->GetSize();
		wxSize szVolumeDecr = btn_volume_decr_->GetSize();
		wxSize szVolumeIncr = btn_volume_incr_->GetSize();

		szPriceIncr.x = szPriceDecr.x = szPriceIncr.y;
		btn_price_decr_->SetSize(szPriceDecr);
		btn_price_incr_->SetSize(szPriceIncr);
		szVolumeIncr.x = szVolumeDecr.x = szVolumeIncr.y;
		btn_volume_decr_->SetSize(szVolumeDecr);
		btn_volume_incr_->SetSize(szVolumeIncr);

		btn_buy_->SetForegroundColour(skin_info_ptr_->crRising);
		btn_sell_->SetForegroundColour(skin_info_ptr_->crFalling);

		/*if (best_size_ != wxSize(0, 0)) {
			return;
		}

		auto rcClient = GetClientRect();

		//用户   用户	   用户  
		//卖一   价格	   买一
		//买开   量	   卖开
		//	  买平卖平

		//中间用小字
		rcUser = rcClient;
		rcUser.x += skin_info_ptr_->xyName.x * 2;
		rcUser.width = 150;
		rcUser.height = skin_info_ptr_->xySpace.y + skin_info_ptr_->xyTechTitle.y + skin_info_ptr_->xySpace.y;
		rcPrice = rcUser;
		rcPrice.x = rcUser.x + 25;
		rcPrice.width = 100;
		rcPrice.height = skin_info_ptr_->xyTechTitle.y + +skin_info_ptr_->xySpace.y;
		rcVolume = rcPrice;
		rcVolume.y = rcPrice.GetBottom();
		auto rcClose = rcVolume;
		rcClose.y = rcVolume.GetBottom();
		rcBuyClose = rcClose;
		rcBuyClose.width = rcClose.width / 2;
		rcSellClose = rcClose;
		rcSellClose.x = rcBuyClose.GetRight();
		rcSellClose.width = rcClose.width - rcBuyClose.width;
		//左右用大字
		rcAskPrice = rcClient;
		rcAskPrice.width = skin_info_ptr_->xyName.x * 2 + 25;
		rcAskPrice.height = rcUser.height + rcPrice.height;
		rcBuyOpen = rcAskPrice;
		rcBuyOpen.y = rcAskPrice.GetBottom();
		rcBuyOpen.height = rcVolume.height + rcClose.height;
		rcBidPrice = rcClient;
		rcBidPrice.x = rcPrice.GetRight();
		rcBidPrice.width = skin_info_ptr_->xyName.x * 2 + 25;
		rcBidPrice.height = rcAskPrice.height;
		rcSellOpen = rcBidPrice;
		rcSellOpen.y = rcBidPrice.GetBottom();
		rcSellOpen.height = rcBuyOpen.height;

		auto rcAll = rcClient;
		rcAll.width = rcAskPrice.width + rcPrice.width + rcBidPrice.width;
		rcAll.height = rcAskPrice.height + rcBuyOpen.height;
	
		best_size_.x = rcAll.width;
		best_size_.y = rcAll.height;
		auto min_size = GetMinSize();
		if (min_size != best_size_) {
			SetMinSize(best_size_);
		}*/
	}

	void TradeView::Draw(wxDC& dc, int state)
	{
		wxCSConv utf8cv(wxFONTENCODING_UTF8);

		zqdb::Code code(h_);
		auto info = (CODEINFO*)code.GetValue();
		auto rcClient = GetClientRect();

		double askprice = INVALID_VALUE;// calc_data_ptr_->GetFieldAsDouble(mdb::Field(STR_ZQDB_FIELD_ASK1), INVALID_VALUE);
		double price = info->Close;
		double bidprice = INVALID_VALUE;// calc_data_ptr_->GetFieldAsDouble(mdb::Field(STR_ZQDB_FIELD_BID1), INVALID_VALUE);
	
		auto rcAll = rcClient;
		rcAll.width = rcAskPrice.width + rcPrice.width + rcBidPrice.width;
		rcAll.height = rcAskPrice.height + rcBuyOpen.height;

		//g_ArgProvider.DrawButtonBarBackground(dc, this, rcAll);

		auto rcLeft = rcAskPrice;
		rcLeft.height += rcBuyOpen.height;

		auto rcMid = rcPrice;

		auto rcRight = rcBidPrice;
		rcRight.height += rcSellOpen.height;
		if (state) {
			auto pt = wxGetMousePosition();
			pt = ScreenToClient(pt);

			int left_state = wxRIBBON_BUTTONBAR_BUTTON_MEDIUM;
			if (rcLeft.Contains(pt)) {
				if (state == 1) {
					left_state |= wxRIBBON_BUTTONBAR_BUTTON_HOVER_MASK;
				}
				else {
					left_state |= wxRIBBON_BUTTONBAR_BUTTON_ACTIVE_MASK;
				}
			}
			g_ArgProvider.DrawButtonBarButton(dc, this, rcLeft, wxRIBBON_BUTTON_HYBRID, left_state, wxT("买开")
				, skin_info_ptr_->bmpNull, skin_info_ptr_->bmpNull
			);

			int right_state = wxRIBBON_BUTTONBAR_BUTTON_MEDIUM;
			if (rcRight.Contains(pt)) {
				if (state == 1) {
					right_state |= wxRIBBON_BUTTONBAR_BUTTON_HOVER_MASK;
				}
				else {
					right_state |= wxRIBBON_BUTTONBAR_BUTTON_ACTIVE_MASK;
				}
			}
			g_ArgProvider.DrawButtonBarButton(dc, this, rcRight, wxRIBBON_BUTTON_HYBRID, right_state, wxT("卖开")
				, skin_info_ptr_->bmpNull, skin_info_ptr_->bmpNull
			);
		}

		dc.SetPen(skin_info_ptr_->penXYLine);
		dc.SetBrush(skin_info_ptr_->brushNull);
		dc.DrawRectangle(rcAll);
		dc.DrawRectangle(rcAskPrice);
		dc.DrawRectangle(rcBuyOpen);
		dc.DrawRectangle(rcBidPrice);
		dc.DrawRectangle(rcSellOpen);
		dc.DrawRectangle(rcUser);
		dc.DrawRectangle(rcPrice);
		dc.DrawRectangle(rcVolume);
		dc.DrawRectangle(rcBuyClose);
		dc.DrawRectangle(rcSellClose);

		dc.SetFont(skin_info_ptr_->fontName);
		if (IsInvalidValue(askprice)) {
			dc.DrawLabel(wxT("——"), rcAskPrice, wxALIGN_LEFT | wxALIGN_BOTTOM);
		} else {
			dc.DrawLabel(wxString::Format("%.2f", askprice), rcAskPrice, wxALIGN_LEFT | wxALIGN_BOTTOM);
		}

		dc.SetFont(skin_info_ptr_->fontName);
		if (IsInvalidValue(price)) {
			dc.DrawLabel(wxT("——"), rcPrice, wxALIGN_LEFT | wxALIGN_BOTTOM);
		}
		else {
			dc.DrawLabel(wxString::Format("%.2f", price), rcPrice, wxALIGN_LEFT | wxALIGN_BOTTOM);
		}

		dc.SetFont(skin_info_ptr_->fontName);
		if (IsInvalidValue(askprice)) {
			dc.DrawLabel(wxT("——"), rcBidPrice, wxALIGN_LEFT | wxALIGN_BOTTOM);
		}
		else {
			dc.DrawLabel(wxString::Format("%.2f", bidprice), rcBidPrice, wxALIGN_LEFT | wxALIGN_BOTTOM);
		}
	}

	wxBEGIN_EVENT_TABLE(TradeView, Base)
	//EVT_SIZE(TradeView::OnSize)
	//EVT_ERASE_BACKGROUND(TradeView::OnErase)
	//EVT_PAINT(TradeView::OnPaint)
	//EVT_MOUSE_EVENTS(TradeView::OnMouse)
	wxEND_EVENT_TABLE()

	void TradeView::OnSize(wxSizeEvent &event)
	{
		event.Skip();

		/*auto rcClient = GetClientRect();

		wxSize szUser = cmb_user_->GetSize();
		wxSize szPriceDecr = btn_price_decr_->GetBestSize();
		wxSize szPrice = text_price_->GetSize();
		wxSize szPriceIncr = btn_price_incr_->GetBestSize();
		wxSize szVolumeDecr = btn_volume_decr_->GetBestSize();
		wxSize szVolume = text_volume_->GetSize();
		wxSize szVolumeIncr = btn_volume_incr_->GetBestSize();
		wxSize szBuy = btn_buy_->GetBestSize();
		wxSize szClose = btn_close_->GetBestSize();
		wxSize szSell = btn_sell_->GetBestSize();

		szPriceIncr.y = szPriceDecr.y = szPriceIncr.y;
		szVolumeIncr.y = szVolumeDecr.y = szVolume.y;

		szUser.x = szBuy.x + szClose.x + szSell.x;
		cmb_user_->SetSize(szUser);
		btn_price_decr_->SetSize(szPriceDecr);
		btn_price_decr_->Move(0, szUser.y);
		szPrice.x = szUser.x - szPriceDecr.x - szPriceIncr.x;
		text_price_->SetSize(szPrice);
		text_price_->Move(szPriceDecr.x, szUser.y);
		btn_price_incr_->SetSize(szPriceIncr);
		btn_price_incr_->Move(szPriceDecr.x + szPrice.x, szUser.y);
		btn_volume_decr_->SetSize(szVolumeDecr);
		btn_volume_decr_->Move(0, szUser.y + szPrice.y);
		szVolume.x = szUser.x;
		text_volume_->SetSize(szVolume);
		text_volume_->Move(szVolumeDecr.x, szUser.y + szPrice.y);
		btn_volume_incr_->SetSize(szVolumeIncr);
		btn_volume_incr_->Move(szVolumeDecr.x + szVolume.x, szUser.y + szPrice.y);

		btn_buy_->SetSize(szBuy);
		btn_buy_->Move(0, szUser.y + szPrice.y + szVolume.y);
		btn_close_->SetSize(szClose);
		btn_close_->Move(szBuy.x, szUser.y + szPrice.y + szVolume.y);
		btn_sell_->SetSize(szSell);
		btn_sell_->Move(szBuy.x + szClose.x, szUser.y + szPrice.y + szVolume.y);*/
	}

	void TradeView::OnErase(wxEraseEvent &event)
	{

	}

	void TradeView::OnPaint(wxPaintEvent &event)
	{
		wxPaintDC dc(this);
		dc.Clear();
		if (IsDispOk()) {
			Draw(dc);
		}
	}

	void TradeView::OnMouse(wxMouseEvent &event)
	{
		auto evt = event.GetEventType();
		wxPoint pt = event.GetPosition();

		if (!IsDispOk()) {
			return;
		}

		if (evt == wxEVT_ENTER_WINDOW)
		{
		}
		if (evt == wxEVT_LEAVE_WINDOW)
		{
		
		}
		if (evt == wxEVT_LEFT_DOWN)
		{
			wxClientDC dc(this);
			dc.Clear();
			Draw(dc, 2);
		}
		if (evt == wxEVT_LEFT_UP)
		{
			wxClientDC dc(this);
			dc.Clear();
			Draw(dc, 1);
		}
		if (evt == wxEVT_LEFT_DCLICK)
		{
		}
		if (evt == wxEVT_MOTION)
		{
			wxClientDC dc(this);
			dc.Clear();
			Draw(dc, 1);
		}
	}

	void TradeView::OnBtnBuy(wxCommandEvent &event)
	{
		int sel = cmb_user_->GetSelection();
		if (sel < 0 && sel >= cmb_user_->GetCount()) {
			return;
		}
		zqdb::Code code(h_);
		zqdb::ObjectT<tagUserInfo> user((HZQDB)cmb_user_->GetClientData(sel));
		zqdb::Msg msg(NET_MSG_TYPE_REQUEST_DO);
		msg.SetReqID(com::zq::proto::msg::MSG_ID_REQUEST_DO_TD_ORDER_INSERT);
		msg.SetParam("str", "1234567");
		msg.SetParam("i64", "1234567");
		msg.SetParam("f", "1234567.89");
		com::zq::proto::msg::ReqOrderInsert req;
		req.set_broker(user->Broker);
		//req.set_investor(user->)
		req.set_user(user->User);
		req.set_exchange(code->Exchange);
		req.set_code(code->Code);
		req.set_price(code->Close);
		req.set_volume(1);
		req.set_direction(DIRECTION_LONG);
		req.set_offset(OFFSET_OPEN);
		req.set_type(ORDER_LIMIT);
		auto str = req.SerializeAsString();
		msg.SetData(str.data(), str.size());
		ZQDBReqNewOrder(user, msg, nullptr, 0);
#ifdef _DEBUG
		com::zq::proto::msg::TestMessage test_msg;
		test_msg.set_type(NET_MSG_TYPE_REQUEST_DO);
		test_msg.set_id(ZQDBReqID(com::zq::proto::msg::MSG_ID_REQUEST_DO_TD_ORDER_INSERT));
		(*test_msg.mutable_param())["str"] = "1234567";
		(*test_msg.mutable_param())["u32"] = "1234567";
		(*test_msg.mutable_param())["i32"] = "1234567";
		(*test_msg.mutable_param())["u64"] = "1234567";
		(*test_msg.mutable_param())["i64"] = "1234567";
		(*test_msg.mutable_param())["f32"] = "1234567.89";
		(*test_msg.mutable_param())["f64"] = "1234567.89";
		auto test_msg_str = test_msg.SerializeAsString();
		double test_f64 = std::stod((*test_msg.mutable_param())["f64"]);

		com::zq::proto::msg::VarMessage var_msg;
		var_msg.set_type(NET_MSG_TYPE_REQUEST_DO);
		var_msg.set_id(ZQDBReqID(com::zq::proto::msg::MSG_ID_REQUEST_DO_TD_ORDER_INSERT));
		com::zq::proto::msg::Variant var_val;
		var_val.set_str("1234567");
		(*var_msg.mutable_param())["str"] = var_val;
		var_val.set_u32(1234567);
		(*var_msg.mutable_param())["u32"] = var_val;
		var_val.set_i32(1234567);
		(*var_msg.mutable_param())["i32"] = var_val;
		var_val.set_u64(1234567);
		(*var_msg.mutable_param())["u64"] = var_val;
		var_val.set_i64(1234567);
		(*var_msg.mutable_param())["i64"] = var_val;
		var_val.set_f32(1234567.89);
		(*var_msg.mutable_param())["f32"] = var_val;
		var_val.set_f64(1234567.89);
		(*var_msg.mutable_param())["f64"] = var_val;
		auto var_msg_str = var_msg.SerializeAsString();
		double var_f64 = (*var_msg.mutable_param())["f64"].f64();
#endif//
	}

	///

	InfoView::InfoView(wxWindow* parent, const char* xml, size_t xmlflag) : Base(parent, xml, xmlflag)
	{

	}

	InfoView::~InfoView()
	{

	}

	void InfoView::OnSkinInfoChanged()
	{
		Base::OnSkinInfoChanged();
		CalcBestSize();
	}

	//void InfoView::OnCalcDataChanged()
	//{
	//	fields_.clear();
	//	fields_.emplace_back(ZQDB_FIELD_CLOSE);
	//	fields_.emplace_back(ZQDB_FIELD_OPEN);
	//	fields_.emplace_back(2);//ZQDB_FIELD_DELTA;
	//	fields_.emplace_back(ZQDB_FIELD_HIGH);
	//	fields_.emplace_back(2);//ZQDB_FIELD_DELTA_PERCENT;
	//	fields_.emplace_back(ZQDB_FIELD_LOW);
	//	fields_.emplace_back(ZQDB_FIELD_VOLUME);
	//	fields_.emplace_back(2);//ZQDB_FIELD_LAST_VOLUME;
	//	fields_.emplace_back(2);//ZQDB_FIELD_POSITION;
	//	fields_.emplace_back(2);//ZQDB_FIELD_SETTLEMENT;
	//	fields_.emplace_back(0);
	//	fields_.emplace_back(2);//ZQDB_FIELD_PREV_POSITION;
	//	fields_.emplace_back(2);//ZQDB_FIELD_PREV_SETTLEMENT;
	//	fields_.emplace_back(2);//ZQDB_FIELD_OPEN_POSITION;
	//	fields_.emplace_back(2);//ZQDB_FIELD_CLOSE_POSITION;
	//	fields_.emplace_back(2);//ZQDB_FIELD_LAST_OPEN_POSITION;
	//	fields_.emplace_back(2);//ZQDB_FIELD_LAST_CLOSE_POSITION;
	//	fields_.emplace_back(2);//ZQDB_FIELD_AVERAGE_PRICE;
	//	fields_.emplace_back(2);//ZQDB_FIELD_DELTA_POSITION;
	//	fields_.emplace_back(2);//ZQDB_FIELD_BOUGHT_VOLUME;
	//	fields_.emplace_back(2);//ZQDB_FIELD_SOLD_VOLUME;
	//	best_size_ = wxSize(0, 0);
	//	Refresh();
	//	if (IsDispOk()) {
	//		Update();
	//	}
	//}

	void InfoView::CalcBestSize()
	{
		if (!skin_info_ptr_) {
			return;
		}
		if (best_size_ != wxSize(0, 0)) {
			return;
		}

		/*
		best_size_.x = rcAll.width;
		best_size_.y = rcAll.height;
		auto min_size = GetMinSize();
		if (min_size != best_size_) {
			SetMinSize(best_size_);
		}*/
	}

	void InfoView::Draw(wxDC& dc)
	{
		wxCSConv utf8cv(wxFONTENCODING_UTF8);

		zqdb::Code code(h_);
		auto info = (CODEINFO*)code.GetValue();
		auto rcClient = GetClientRect();

		auto rcText = rcClient;
		rcText.DeflateLeft(skin_info_ptr_->xySpace.x);
		rcText.DeflateTop(skin_info_ptr_->xySpace.y);
		for (size_t i = 0, j = fields_.size(); i<j; i++)
		{
			dc.SetFont(skin_info_ptr_->fontName);
			dc.SetTextForeground(skin_info_ptr_->crName);
			dc.DrawLabel(wxT("NAME"), rcText, wxALIGN_LEFT | wxALIGN_BOTTOM);

			/*dc.SetFont(skin_info_ptr_->fontText);
			dc.SetTextForeground(skin_info_ptr_->crText);
			double value = code.GetFieldAsDouble(fields_[i], INVALID_VALUE);
			if (IsInvalidValue(value)) {
			dc.DrawLabel(wxT("——"), rcText, wxALIGN_RIGHT | wxALIGN_BOTTOM);
			}
			else {
			dc.DrawLabel(wxString::Format(wxT("%.2f"), value), rcText, wxALIGN_RIGHT | wxALIGN_BOTTOM);
			}*/
			//rcText.OffsetRect(nFieldWidth, 0);
			//if (m_FieldFlag[i]) {
			//	rcText.bottom += m_DispInfoPtr->xySpace.cy;

			//	rcText.left = rcClient.left;
			//	rcText.right = rcText.left + nFieldWidth;
			//	rcText.top = rcText.bottom;
			//	rcText.bottom = rcText.top + m_DispInfoPtr->xyName.cy;

			//	rcText.left += m_DispInfoPtr->xySpace.cx;

			//	if (m_FieldFlag[i] == FIELD_FLAG_BREAKLINE) {
			//		//rcText.top += 1;
			//		UIgdi::DrawLine(m_dc, rcClient.left, rcText.top, rcClient.right, rcText.top, PS_SOLID, 1, m_DispInfoPtr->crXYLine, R2_COPYPEN);
			//		rcText.top += m_DispInfoPtr->xySpace.cy;
			//		rcText.bottom = rcText.top + m_DispInfoPtr->xyName.cy;
			//	}
			//}
		}

		if (best_size_ == wxSize(0, 0)) {
			best_size_.x = skin_info_ptr_->xyName.x * 2 + 100 + skin_info_ptr_->xyName.x * 2;
			best_size_.y = skin_info_ptr_->xyName.y * 2 + skin_info_ptr_->xySpace.y * 3;
			auto min_size = GetMinSize();
			if (min_size != best_size_) {
				SetMinSize(best_size_);
			}
		}
	}

	wxBEGIN_EVENT_TABLE(InfoView, Base)
	EVT_ERASE_BACKGROUND(InfoView::OnErase)
	EVT_PAINT(InfoView::OnPaint)
	EVT_MOUSE_EVENTS(InfoView::OnMouse)
	wxEND_EVENT_TABLE()

	void InfoView::OnSize(wxSizeEvent &event)
	{
		event.Skip();
	}

	void InfoView::OnErase(wxEraseEvent &event)
	{

	}

	void InfoView::OnPaint(wxPaintEvent &event)
	{
		wxPaintDC dc(this);
		dc.Clear();
		if (IsDispOk()) {
			Draw(dc);
		}
	}

	void InfoView::OnMouse(wxMouseEvent &event)
	{
	
	}

	///

	TickView::TickView(wxWindow* parent, const char* xml, size_t xmlflag) : Base(parent, xml, xmlflag)
	{

	}

	TickView::~TickView()
	{

	}

	void TickView::OnSkinInfoChanged()
	{
		Base::OnSkinInfoChanged();
	}

	void TickView::CalcBestSize()
	{
		if (!skin_info_ptr_) {
			return;
		}
		if (best_size_ != wxSize(0, 0)) {
			return;
		}

		best_size_.x = skin_info_ptr_->xySpace.x + skin_info_ptr_->xyName.x * (10) + skin_info_ptr_->xySpace.x;
		best_size_.y = skin_info_ptr_->xySpace.y + skin_info_ptr_->xyName.y * 20 + skin_info_ptr_->xySpace.y;
		auto min_size = GetMinSize();
		if (min_size != best_size_) {
			SetMinSize(best_size_);
		}
	}

	void TickView::Draw(wxDC& dc)
	{
		wxCSConv utf8cv(wxFONTENCODING_UTF8);

		zqdb::Code code(h_);
		auto info = (CODEINFO*)code.GetValue();
		auto rcClient = GetClientRect();

		/*dc.SetFont(skin_info_ptr_->fontText);
		dc.SetTextForeground(skin_info_ptr_->crCommodityName);
		dc.DrawLabel(utf8cv.cMB2WX(info->Name), rcClient, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
		//UIgdi::DrawTextA(hdc, szTitle, nTitleLen, &rc, DT_RIGHT | DT_VCENTER | DT_SINGLELINE, m_DispInfoPtr->crCommodityName, m_DispInfoPtr->hText);

		dc.SetFont(skin_info_ptr_->fontText);
		dc.SetTextForeground(skin_info_ptr_->crCommodityCode);
		dc.DrawLabel(utf8cv.cMB2WX(info->Code), rcClient, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
		//UIgdi::DrawTextA(hdc, szTitle, nTitleLen, &rc, DT_LEFT | DT_VCENTER | DT_SINGLELINE, m_DispInfoPtr->crCommodityCode, m_DispInfoPtr->hText);*/
	}

	wxBEGIN_EVENT_TABLE(TickView, Base)
	EVT_ERASE_BACKGROUND(TickView::OnErase)
	EVT_PAINT(TickView::OnPaint)
	wxEND_EVENT_TABLE()

	void TickView::OnErase(wxEraseEvent &event)
	{

	}

	void TickView::OnPaint(wxPaintEvent &event)
	{
		wxPaintDC dc(this);
		dc.Clear();
		if (IsDispOk()) {
			Draw(dc);
		}
	}

	wxDEFINE_EVENT(EVT_SPLITTER_CHANGE_EVENT, wxCommandEvent);

	SplitterSizer::SplitterSizer(wxWindow* window, int orient) : Base(orient), window_(window)
	{
		if (window) {
			Bind(wxEVT_DESTROY, &SplitterSizer::OnDestroy, this);
			Bind(wxEVT_SET_CURSOR, &SplitterSizer::OnSetCursor, this);
			Bind(wxEVT_ENTER_WINDOW, &SplitterSizer::OnMouseEnter, this);
			Bind(wxEVT_LEAVE_WINDOW, &SplitterSizer::OnMouseLeave, this);
			Bind(wxEVT_MOTION, &SplitterSizer::OnMouseMove, this);
			Bind(wxEVT_LEFT_DOWN, &SplitterSizer::OnMouseDown, this);
			Bind(wxEVT_LEFT_UP, &SplitterSizer::OnMouseUp, this);
			window->PushEventHandler(this);
		}
	}

	SplitterSizer::~SplitterSizer()
	{
		//splitters_.clear();
	}

	wxSizerItem* SplitterSizer::AddSplitter(int size)
	{
		splitters_.emplace_back(Base::AddSpacer(size));
		return splitters_.back();
	}

	bool SplitterSizer::IsSplitter(wxSizerItem* item)
	{
		return std::find(splitters_.begin(), splitters_.end(), item) != splitters_.end();
	}

	wxSizerItem* SplitterSizer::GetItemByPoint(const wxPoint& pt, size_t* index)
	{
		wxPoint pos = GetPosition();
		wxSize sz = GetSize();
		for (size_t i = 0, j = GetItemCount(); i < j; i++)
		{
			wxSizerItem* item = GetItem(i);
			if (item) {
				wxRect rc_item = item->GetRect();
				if (item->IsSpacer()) {
					if (IsVertical()) {
						rc_item.width = sz.x;
					}
					else {
						rc_item.height = sz.y;
					}
				}
				if (rc_item.Contains(pt)) {
					if (index) {
						*index = i;
					}
					return item;
				}
			}
		}
		return nullptr;
	}

	static void UnBindEvtHandler(wxWindow* window, wxSizer* sizer)
	{
		if (!window || !sizer) {
			return;
		}

		for (size_t i = 0; i < sizer->GetItemCount(); ++i)
		{
			auto sizer_item = sizer->GetItem(i);
			if (sizer_item && sizer_item->IsSizer()) {
				UnBindEvtHandler(window, sizer_item->GetSizer());
			}
		}

		auto splitter = dynamic_cast<SplitterSizer*>(sizer);
		if (splitter) {
			window->RemoveEventHandler(splitter);
		}
	}

	void SplitterSizer::OnDestroy(wxWindowDestroyEvent &event)
	{
		/*Unbind(wxEVT_SET_CURSOR, &SplitterSizer::OnSetCursor, this);
		Unbind(wxEVT_ENTER_WINDOW, &SplitterSizer::OnMouseEnter, this);
		Unbind(wxEVT_LEAVE_WINDOW, &SplitterSizer::OnMouseLeave, this);
		Unbind(wxEVT_MOTION, &SplitterSizer::OnMouseMove, this);
		Unbind(wxEVT_LEFT_DOWN, &SplitterSizer::OnMouseDown, this);
		Unbind(wxEVT_LEFT_UP, &SplitterSizer::OnMouseUp, this);*/
		wxWindow* window = window_;//GetContainingWindow();
		if (window) {
			//window->RemoveEventHandler(this);
			//window->PopEventHandler();
			UnBindEvtHandler(window, window->GetSizer());
		}
		event.Skip();
	}

	void SplitterSizer::OnSetCursor(wxSetCursorEvent& event)
	{
		wxWindow* window = window_;//this->GetContainingWindow();
		if (window) {
			wxPoint pt(event.GetX(), event.GetY());
			wxSizerItem* item = GetItemByPoint(pt, &index_capture_);
			if (item && IsSplitter(item)) {
				item_capture_ = item;
				if (IsVertical()) {
					window->SetCursor(wxCursor(wxCURSOR_SIZENS));
				}
				else {
					window->SetCursor(wxCursor(wxCURSOR_SIZEWE));
				}
				event.Skip();
				return;
			}
		}
		item_capture_ = nullptr;
	}

	void SplitterSizer::OnMouseEnter(wxMouseEvent &event)
	{
		event.Skip();
	}

	void SplitterSizer::OnMouseLeave(wxMouseEvent &event)
	{
		event.Skip();
	}

	void SplitterSizer::OnMouseMove(wxMouseEvent &event)
	{
		wxWindow* window = window_;//this->GetContainingWindow();
		if (window && item_capture_) {
			if (window->HasCapture()) {
				wxPoint pt_sizer = window->ClientToScreen(GetPosition());
				wxSize sz_sizer = GetSize();

				wxScreenDC dc;

				wxDCOverlay overlaydc(overlay_, &dc);
				overlaydc.Clear();

				dc.SetPen(*wxGREY_PEN);
				dc.SetBrush(*wxTRANSPARENT_BRUSH);

				wxPoint pt = window->ClientToScreen(event.GetPosition());
				//dc.DrawLine(pt_capture_, pt);
				//rc.Offset(pt.x - pt_capture_.x, pt.y - pt_capture_.y);
				//dc.DrawRectangle(wxRect(pt_capture_, pt));
				if (IsVertical()) {
					dc.DrawLine(pt_sizer.x, pt.y, pt_sizer.x + sz_sizer.x, pt.y);
				}
				else {
					dc.DrawLine(pt_sizer.y, pt.x, pt_sizer.y + sz_sizer.y, pt.x);
				}
				return;
			}
		}
		event.Skip();
	}

	void SplitterSizer::OnMouseDown(wxMouseEvent &event)
	{
		wxWindow* window = window_;//this->GetContainingWindow();
		if (window && item_capture_) {
			window->CaptureMouse();
			//pt_capture_ = event.GetPosition();
			pt_capture_ = window->ClientToScreen(event.GetPosition());
			return;
		}
		event.Skip();
	}

	void SplitterSizer::OnMouseUp(wxMouseEvent &event)
	{
		wxWindow* window = window_;//this->GetContainingWindow();
		if (window && item_capture_)
		{
			if (window->HasCapture()) {
				window->ReleaseMouse();
				{
					//声明桌面DC
					wxScreenDC dc;
					//设置覆盖DC
					wxDCOverlay overlaydc(overlay_, &dc);
					overlaydc.Clear();
				}
				overlay_.Reset();

				wxPoint pt_sizer = GetPosition();
				wxSize sz_sizer = GetSize();

				wxPoint pt = window->ClientToScreen(event.GetPosition());
				if (IsVertical()) {
					int offset = pt.y - pt_capture_.y;
					/*wxPoint pt_item_sizer = item_sizer_->GetPosition();
					wxSize sz_item_sizer = item_sizer_->GetSize();
					pt_item_sizer.y += offset;
					item_sizer_->SetDimension(pt_item_sizer, sz_item_sizer);*/
					auto x_sizer_index = index_capture_ - 1;
					auto x_sizer = GetItem(x_sizer_index);
					wxSize x_sizer_sz = x_sizer->GetMinSize();
					auto y_sizer_index = index_capture_ + 1;
					auto y_sizer = GetItem(y_sizer_index);
					wxSize y_sizer_sz = y_sizer->GetMinSize();
					x_sizer_sz.y += offset;
					y_sizer_sz.y -= offset;
					SetItemMinSize(x_sizer_index, -1, x_sizer_sz.y);
					SetItemMinSize(y_sizer_index, -1, y_sizer_sz.y);
				}
				else {
					int offset = pt.x - pt_capture_.x;
					auto x_sizer_index = index_capture_ - 1;
					auto x_sizer = GetItem(x_sizer_index);
					wxSize x_sizer_sz = x_sizer->GetMinSize();
					auto y_sizer_index = index_capture_ + 1;
					auto y_sizer = GetItem(y_sizer_index);
					wxSize y_sizer_sz = y_sizer->GetMinSize();
					x_sizer_sz.x += offset;
					y_sizer_sz.x -= offset;
					SetItemMinSize(x_sizer_index, x_sizer_sz.x, -1);
					SetItemMinSize(y_sizer_index, y_sizer_sz.x, -1);
				}
				Layout();
				//sizer->Fit(this);

				//window->GetEventHandler()->AddPendingEvent(wxCommandEvent(EVT_SPLITTER_CHANGE_EVENT));
				return;
			}
		}
		event.Skip();
	}
	
}

