#pragma once

#include <calcif.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <array>
#include <map>
#include <set>
#include <iostream>
#include <iomanip>
#include <memory>
#include <mutex>

#include <XUtil/XXml.hpp>
#include <XUtil/XDateTime.hpp>

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
#include "wx/sizer.h"
#include "wx/menu.h"
#include "wx/msgdlg.h"
#include "wx/dcbuffer.h"
#include "wx/colordlg.h"
#include "wx/wrapsizer.h"
#include "wx/image.h"
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

// Standard DC supports drawing with alpha on OSX and GTK3.
#if defined(__WXOSX__) || defined(__WXGTK3__)
#define wxDRAWING_DC_SUPPORTS_ALPHA 1
#else
#define wxDRAWING_DC_SUPPORTS_ALPHA 0
#endif // __WXOSX__ || __WXGTK3__
//
//wxDECLARE_EVENT(ZQDB_CODE_PREV_EVENT, wxCommandEvent);
//wxDECLARE_EVENT(ZQDB_CODE_NEXT_EVENT, wxCommandEvent);

namespace zqdb {

	class TitleView;
	class MmpView;
	class TradeView;
	class InfoView;
	class TickView;

	#define MAX_BAR_SCALE	9

	class Rect : public wxRect
	{
		typedef wxRect Base;
	public:
		//using wxRect::wxRect;
		Rect() : Base() {}
		Rect(int xx, int yy, int ww, int hh) : Base(xx, yy, ww, hh) { }
		Rect(const wxPoint& topLeft, const wxPoint& bottomRight) : Base(topLeft, bottomRight) { }
		Rect(const wxPoint& pt, const wxSize& size) : Base(pt, size) { }
		Rect(const wxSize& size) : Base(size) { }
		Rect(const wxRect& o);

		Rect& Inflate(wxCoord left, wxCoord top, wxCoord right, wxCoord bottom);
		Rect& InflateX(wxCoord left, wxCoord right);
		Rect& InflateY(wxCoord top, wxCoord bottom);
		Rect& InflateLeft(wxCoord d);
		Rect& InflateRight(wxCoord d);
		Rect& InflateTop(wxCoord d);
		Rect& InflateBottom(wxCoord d);

		Rect& Deflate(wxCoord left, wxCoord top, wxCoord right, wxCoord bottom) {
			return Inflate(-left, -top, -right, -bottom);
		}
		Rect& DeflateX(wxCoord left, wxCoord right) {
			return InflateX(-left, -right);
		}
		Rect& DeflateY(wxCoord top, wxCoord bottom) {
			return InflateY(-top, -bottom);
		}
		Rect& DeflateLeft(wxCoord d) {
			return InflateLeft(-d);
		}
		Rect& DeflateRight(wxCoord d) {
			return InflateRight(-d);
		}
		Rect& DeflateTop(wxCoord d) {
			return InflateTop(-d);
		}
		Rect& DeflateBottom(wxCoord d) {
			return InflateBottom(-d);
		}
	};

	//template<class T, class Obj>
	//class MultiObjMap
	//{
	//	typedef MultiObjMap<T, Obj> This;
	//protected:
	//	std::vector<Obj*> objs_;

	//public:

	//	void LoadAllObj(const char* xml = nullptr, size_t xmlflag = XUtil::XML_FLAG_JSON_FILE)
	//	{
	//		//T* pT = static_cast<T*>(this);
	//	}

	//	void SaveAllObj(const char* xml = nullptr, size_t xmlflag = XUtil::XML_FLAG_JSON_FILE)
	//	{
	//		T* pT = static_cast<T*>(this);
	//		//
	//	}

	//	void AddObj(Obj* pObj)
	//	{
	//		T* pT = static_cast<T*>(this);
	//		if (pObj) {
	//			pT->OnAddObj(objs_.size(), pObj);
	//			objs_.push_back(pObj);
	//		}
	//	}

	//	void RemoveObj(size_t Pos, bool bDelete = true)
	//	{
	//		T* pT = static_cast<T*>(this);
	//		ASSERT(Pos < objs_.size());
	//		if (Pos >= objs_.size()) {
	//			return;
	//		}
	//		Obj* pObj = *(objs_.begin() + Pos);
	//		pT->OnRemoveObj(Pos, pObj);
	//		if (bDelete) {
	//			delete pObj;
	//		}
	//		objs_.erase(objs_.begin() + Pos);
	//	}

	//	void RemoveObj(Obj* pObj, bool bDelete = true)
	//	{
	//		RemoveObj(GetObjPos(pObj), bDelete);
	//	}

	//	void RemoveAllObj(bool bDelete = true)
	//	{
	//		T* pT = static_cast<T*>(this); 
	//		size_t Pos = 0;
	//		for (; Pos < objs_.size(); Pos++)
	//		{
	//			pT->OnRemoveObj(Pos, objs_[Pos]);
	//			if (bDelete) {
	//				delete objs_[Pos];
	//			}
	//		}
	//		objs_.clear();
	//	}

	//	bool IsObjEmpty()
	//	{
	//		return objs_.empty() ? TRUE : FALSE;
	//	}

	//	size_t GetObjCount()
	//	{
	//		return objs_.size();
	//	}

	//	size_t GetObjPos(Obj* pObj)
	//	{
	//		size_t Pos = 0;
	//		for (; Pos < objs_.size(); Pos++)
	//		{
	//			if (pObj == objs_[Pos]) {
	//				break;
	//			}
	//		}
	//		return Pos;
	//	}

	//	Obj* GetObjByPos(size_t Pos)
	//	{
	//		if (Pos >= objs_.size()) {
	//			return nullptr;
	//		}
	//		return objs_[Pos];
	//	}

	//	void OnAddObj(size_t Pos, Obj* pObj)
	//	{

	//	}

	//	void OnRemoveObj(size_t Pos, Obj* pObj)
	//	{

	//	}

	//	void Broadcast(const std::function<bool(size_t pos, Obj* obj)>& f)
	//	{
	//		size_t Pos = 0;
	//		for (; Pos < objs_.size(); Pos++)
	//		{
	//			if (!f(Pos, objs_[Pos])) {
	//				break;
	//			}
	//		}
	//	}
	//};

	//template<class T, class Obj>
	//class MultiNameObjMap
	//{
	//	typedef MultiNameObjMap<T, Obj> This;
	//protected:
	//	std::map<wxString, Obj*> objs_;

	//public:

	//	void LoadAllObj(const char* xml = nullptr, size_t xmlflag = XUtil::XML_FLAG_JSON_STRING)
	//	{
	//		//T* pT = static_cast<T*>(this);
	//	}

	//	void SaveAllObj(const char* xml = nullptr, size_t xmlflag = XUtil::XML_FLAG_JSON_STRING)
	//	{
	//		//T* pT = static_cast<T*>(this);
	//	}

	//	void AddObj(const wxString& name, Obj* obj)
	//	{
	//		T* pT = static_cast<T*>(this);
	//		if (pObj) {
	//			pT->OnAddObj(lpszName, pObj);
	//			ASSERT(!GetObjByName(name));
	//			objs_[name] = pObj;
	//		}
	//	}

	//	void RemoveObj(const wxString& name, bool bDelete = true)
	//	{
	//		T* pT = static_cast<T*>(this);
	//		auto it = objs_.find(name);
	//		if (it != objs_.end()) {
	//			pT->OnRemoveObj(it->first, it->second);
	//			if (bDelete) {
	//				delete it->second;
	//			}
	//			objs_.erase(it);
	//		}
	//	}

	//	void RemoveObj(Obj* pObj, bool bDelete = true)
	//	{
	//		RemoveObj(GetObjName(pObj), bDelete);
	//	}

	//	void RemoveAllObj(bool bDelete = true)
	//	{
	//		T* pT = static_cast<T*>(this);
	//		for (auto& pr : objs_)
	//		{
	//			pT->OnRemoveObj(pr.first, pr.second);
	//			if (bDelete) {
	//				delete pr.second;
	//			}
	//		}
	//		objs_.clear();
	//	}

	//	bool IsObjEmpty()
	//	{
	//		return objs_.empty() ? TRUE : FALSE;
	//	}

	//	size_t GetObjCount()
	//	{
	//		return objs_.size();
	//	}

	//	const wxString& GetObjName(Obj* pObj)
	//	{
	//		if (pObj) {
	//			auto it = objs_.begin();
	//			for (; it != objs_.end(); ++it)
	//			{
	//				if (it->second == pObj) {
	//					return it->first;
	//					break;
	//				}
	//			}
	//		}
	//		return wxNullString;
	//	}

	//	Obj* GetObjByName(const wxString& name)
	//	{
	//		auto it = objs_.find(name);
	//		if (it != objs_.end()) {
	//			return it->second;
	//		}
	//		return nullptr;
	//	}

	//	void OnAddObj(const wxString& name, Obj* pObj)
	//	{

	//	}

	//	void OnRemoveObj(const wxString& name, Obj* pObj)
	//	{
	//		ASSERT(pObj);
	//	}

	//	void Broadcast(const std::function<bool(const wxString& name, Obj* obj)>& f)
	//	{
	//		for (auto& pr : objs_)
	//		{
	//			if (!f(pr.first, pr.second)) {
	//				break;
	//			}
	//		}
	//	}
	//};

	class SkinInfo
	{
	public:
		SkinInfo()
		{
			memset(&nBarWidth, 0, sizeof(nBarWidth));
			memset(&nBarSpace, 0, sizeof(nBarSpace));
			nBarScale = 0;
			nBarShift = 0;
		}
		~SkinInfo()
		{
		}

		// const wxColour& GetUpDownColor(int nUpDown);
		// const wxColour& GetUpDownLineColor(int nUpDown);
		// const wxPen& GetUpDownPen(int nUpDown);
		// const wxPen& GetUpDownLinePen(int nUpDown);
		// const wxBrush& GetUpDownBrush(int nUpDown);
		// const wxBrush& GetUpDownLineBrush(int nUpDown);

		//默认显示配置

		//鼠标
		wxCursor curDragLeftRight; //左右/西东
		wxCursor curDragUpDown; //上下/北南
		wxCursor curDrawLine; //划线

		//颜色
		wxColour crBackgnd;			//背景
		wxColour crTabSelBackgnd;	//标签选中背景
		wxColour crRptTitleBakcgnd;	//报表标题背景
		wxColour crRptSelBackgnd;	//报表选中背景

		wxColour crTitle;			//标题
		wxColour crName;			//名称
		wxColour crText;			//文字
		wxColour crRising;			//上涨
		wxColour crFalling;			//下跌
		wxColour crCommodityCode;	//代码
		wxColour crCommodityName;	//名称
		wxColour crAmount;			//额
		wxColour crVolume;			//量
		wxColour crTabSel;			//标签选中

		wxColour crLine;			//线
		wxColour crAverageLine;		//均线
		wxColour crDrawLine;		//画线
		wxColour crXYLine;			//X、Y分隔线
		wxColour crXText;			//X坐标文字
		wxColour crYText;			//Y坐标文字
		wxColour crCrossCursor;		//十字游标	
		wxColour crRptLine;			//报表线
		wxColour crRisingLine;		//上涨线
		wxColour crFallingLine;		//下跌线
		wxColour crILine[8];		//指标线
		wxColour crRefline;			//参考线
		wxColour crOrderLine;		//订单线

		inline wxColour GetColor(double val) {
			if (val > 0.) {
				return crRising;
			}
			else if (val < 0.) {
				return crFalling;
			}
			return crText;
		}

		//画笔
		wxPen penLine;			//线
		wxPen penAverageLine;	//均线
		//wxPen penDrawLine;		//画线
		wxPen penXYLine;		//X、Y分隔线
		wxPen penXText;			//X坐标文字
		wxPen penYText;			//Y坐标文字
		//wxPen penCrossCursor;	//十字游标	
		wxPen penRptLine;		//报表线
		wxPen penRisingLine;	//上涨线
		wxPen penFallingLine;	//下跌线
		wxPen penILine[8];		//指标线
		wxPen penRefline;		//参考线
		wxPen penOrderLine;		//订单线
		//
		wxPen pen;
		wxPen penRising;
		wxPen penFalling;
		wxPen penDrawLine;
		wxPen penCrossCursor;
		wxPen penYCrossCursor;
		wxPen penNow;
		wxPen penOrder;

		inline wxPen GetPen(double val) {
			if (val > 0.) {
				return penRising;
			}
			else if (val < 0.) {
				return penFalling;
			}
			return pen;
		}

		//画刷
		wxBrush brush;
		wxBrush brushRising;
		wxBrush brushFalling;
		wxBrush brushDrawLine;
		wxBrush brushNull;

		inline wxBrush GetBursh(double val) {
			if (val > 0.) {
				return brushRising;
			}
			else if (val < 0.) {
				return brushFalling;
			}
			return brush;
		}

		//图片
		wxBitmap bmpNull;

		//字体
		wxFont fontName;
		wxFont fontText;
		wxFont fontTabTitle;
		wxFont fontRptTitle;
		wxFont fontRptText;
		wxFont fontTechTitle;
		wxFont fontXText;
		wxFont fontYText;
		wxFont fontOrder;

		//字体高宽
		wxSize xyName;
		wxSize xyText;
		wxSize xyTabTitle;
		wxSize xyRptTitle;
		wxSize xyRptText;
		wxSize xyTechTitle;
		wxSize xyXText;
		wxSize xyYText;
		wxSize xyOrder;
		wxSize xySpace;

		//窗体高宽（相对于水平或者垂直布局）
		wxSize xyTabCtrl;								//默认标签水平高度，或者垂直时宽度
		wxSize xyScrollBar;							//默认滚动条水平高度，或者垂直时宽度
		wxSize xyWndIndicator;						//默认指标窗口水平高度，或者垂直时宽度
		wxSize xyInfoIndicator;						//默认指标信息栏水平右侧信息栏宽度和顶部标题栏高度，或者垂直时（忽略）
		wxSize xyCoordinate;							//默认时间轴坐标水平高度，或者垂直时（忽略）

		size_t nBarWidth[MAX_BAR_SCALE];				//K线柱宽度调整
		size_t nBarSpace[MAX_BAR_SCALE];				//K线柱间隙调整
		size_t nBarScale;
		size_t nBarShift;								//平移图表,0表示不平移

		std::map<mdb::Field, wxString> field_names_;	//多语言
		void SetFieldName(const mdb::Field& field, const wxString& name);
		virtual const wxString& GetFieldName(const mdb::Field& field);
	};

	template<class T, class TSkinInfo = SkinInfo>
	class SkinMap
	{
	protected:
		std::shared_ptr<TSkinInfo> skin_info_ptr_;

	public:
		SkinMap()
		{
			
		}

		void Clear()
		{
			skin_info_ptr_.reset();
		}
		inline bool IsDispOk() const {
			return skin_info_ptr_ ? true : false;
		}
		const std::shared_ptr<TSkinInfo>& GetSkinInfo() const
		{
			return skin_info_ptr_;
		}
		void SetSkinInfo(const std::shared_ptr<TSkinInfo>& skin_info_ptr)
		{
			T* pT = static_cast<T*>(this);
			skin_info_ptr_ = skin_info_ptr;
			pT->OnSkinInfoChanged();
		}
		void OnSkinInfoChanged()
		{
		}
	};

	class ContainerInfo
	{
	public:
		int type_;
		wxString Key_;
		long Extra_;
		void* data_;
	};

	class TechContainerInfo : public ContainerInfo
	{
	public:
		bool read_only_ = false;
		std::vector<HZQDB> h_list_;
		size_t pos_ = 0;
	public:
		TechContainerInfo(bool read_only = false) : read_only_(read_only)
		{
		}

		bool IsReadOnly() { return read_only_; }
		bool IsSame(HZQDB h) {
			if (h_list_[pos_] == h) {
				return true;
			}
			return false;
		}
		void Assign(const HZQDB* h_list, size_t h_count) { h_list_.assign(h_list, h_list + h_count); }
		size_t GetItemCount() { return h_list_.size(); }
		HZQDB GetItem(size_t pos)
		{
			if (pos < h_list_.size()) {
				return h_list_[pos];
			}
			ASSERT(0);
			return nullptr;
		}
		size_t GetCurItemPos()
		{
			return pos_;
		}
		void SetCurItemPos(size_t Pos)
		{
			ASSERT(Pos < h_list_.size());
			pos_ = Pos;
		}
	};

	template<class T, class TInfo = ContainerInfo>
	class ContainerMap
	{
	protected:
		//
		std::shared_ptr<TInfo> info_ptr_;

	public:
		void OnInfoChanged()
		{
		}

		const std::shared_ptr<TInfo>& GetInfo() const { return info_ptr_; }
		void SetInfo(const std::shared_ptr<TInfo>& info_ptr) {
			T* pT = static_cast<T*>(this);
			info_ptr_ = info_ptr;
			pT->OnInfoChanged();
		}

		bool IsOk() { return info_ptr_ != nullptr; }
	};

	template<class T, class TInfo = TechContainerInfo>
	class TechContainerMap : public ContainerMap<T, TInfo>
	{
	public:

		bool IsReadOnly() { return info_ptr_->IsReadOnly(); }
		bool IsSame(HZQDB h) { return info_ptr_->IsSame(h); }
		void Assign(const HZQDB* h_list, size_t h_count) { info_ptr_->Assign(h_list, h_count); }
		size_t GetItemCount() { return info_ptr_->GetItemCount(); }
		HZQDB GetItem(size_t pos) { return info_ptr_->GetItem(pos);  }
		size_t GetCurItemPos() { return info_ptr_->GetCurItemPos(); }
		void OnCurItemChanged() { }
		void SetCurItemPos(size_t Pos) {
			T* pT = static_cast<T*>(this);
			if (Pos != info_ptr_->GetCurItemPos()) {
				info_ptr_->SetCurItemPos(Pos);
				pT->OnCurItemChanged();
			}
		}
	};

	///

	template<class T>
	class HandleMap
	{
		typedef HandleMap<T> This;
	protected:
		HZQDB h_ = nullptr;

	public:
		void OnHandleChanged()
		{

		}

		void Clear()
		{
			h_ = nullptr;
		}
		inline bool IsDispOk() const { return h_ != nullptr; }
		HZQDB GetHandle() const
		{
			return h_;
		}
		void SetHandle(HZQDB h)
		{
			T* pT = static_cast<T*>(this);
			h_ = h;
			pT->OnHandleChanged();
		}
	};

	//////////////////////////////////////////////////////////////////////////

	template<class T, class TBase = wxPanel>
	class BaseViewT
		: public TBase
		, public SkinMap<T>
		, public HandleMap<T>
		, public INotifyT<T>
	{
		typedef BaseViewT<T, TBase> This;
	public:
		typedef TBase Base;
		typedef SkinMap<T> SkinBase;
		typedef HandleMap<T> HandleBase;
		typedef INotifyT<T> NotifyBase;
	protected:
		int orient_ = 0;
	public:
		BaseViewT(wxWindow* parent, const char* xml = nullptr, size_t xmlflag = XUtil::XML_FLAG_JSON_STRING) : Base(parent)
		{
			//T* pT = static_cast<T*>(this);
			//Bind(wxEVT_ERASE_BACKGROUND, &This::OnErase, this);
			//Bind(wxEVT_PAINT, &This::OnPaint, this);
		}

		void OnOrientChanged()
		{
			Refresh();
		}

		void SetOrient(int orient)
		{
			T* pT = static_cast<T*>(this);
			if (orient != orient_) {
				orient_ = orient;
				pT->OnOrientChanged();
			}
		}
		int GetOrient() { return orient_; }

		void OnSkinInfoChanged()
		{
			SetBackgroundColour(skin_info_ptr_->crBackgnd);
			Refresh();
		}

		void OnNotifyUpdate(HZQDB h)
		{
			if (h == h_) {
				Refresh();
			}
		}

		bool IsDispOk() const { return skin_info_ptr_ && h_; }

		Rect GetClientRect() const { return Base::GetClientRect(); }
	};

	class TitleView 
		: public BaseViewT<TitleView>
	{
		typedef TitleView This;
	public:
		typedef BaseViewT<TitleView> Base;
	protected:
		wxSize best_size_;
	public:
		TitleView(wxWindow* parent, const char* xml = nullptr, size_t xmlflag = XUtil::XML_FLAG_JSON_STRING);
		virtual ~TitleView();

		void OnSkinInfoChanged();

	protected:
		void CalcBestSize();

		void Draw(wxDC& dc);

		void OnErase(wxEraseEvent &event);
		void OnPaint(wxPaintEvent &event);

		wxDECLARE_EVENT_TABLE();
	};

	class MmpView
		: public BaseViewT<MmpView>
	{
		typedef MmpView This;
	public:
		typedef BaseViewT<MmpView> Base;
	protected:
		wxSize best_size_;
		size_t level_ = 1;
	public:
		MmpView(wxWindow* parent, const char* xml = nullptr, size_t xmlflag = XUtil::XML_FLAG_JSON_STRING);
		virtual ~MmpView();

		void OnSkinInfoChanged();

	protected:
		void CalcBestSize();

		void Draw(wxDC& dc);

		void OnErase(wxEraseEvent &event);
		void OnPaint(wxPaintEvent &event);

		wxDECLARE_EVENT_TABLE();
	};

	class TradeView
		: public BaseViewT<TradeView>
	{
		typedef TradeView This;
	public:
		typedef BaseViewT<TradeView> Base;
	protected:
		wxComboBox* cmb_user_ = nullptr;
		wxButton* btn_price_decr_ = nullptr;
		wxTextCtrl* text_price_ = nullptr; //限价时需要输入价格
		wxButton* btn_price_incr_ = nullptr;
		wxButton* btn_volume_decr_ = nullptr;
		wxTextCtrl* text_volume_ = nullptr; //需要输入量
		wxButton* btn_volume_incr_ = nullptr;
		wxButton* btn_buy_ = nullptr;
		wxButton* btn_close_ = nullptr;
		wxButton* btn_sell_ = nullptr;
		Rect rcUser; //用户
		Rect rcAskPrice; //卖一
		Rect rcPrice; //价
		Rect rcVolume; //量
		Rect rcBidPrice; //买一
		Rect rcBuyOpen; //买开
		Rect rcSellOpen; //卖开
		Rect rcBuyClose; //买平
		Rect rcSellClose; //卖平
		wxSize best_size_;
	public:
		TradeView(wxWindow* parent, const char* xml = nullptr, size_t xmlflag = XUtil::XML_FLAG_JSON_STRING);
		virtual ~TradeView();

		void OnSkinInfoChanged();

		void OnNotifyUpdate(HZQDB h);

	protected:
		void CalcBestSize();
		//virtual wxSize DoGetBestSize() const;

		//0:normal,1:hover,2:click
		void Draw(wxDC& dc, int state = 0);

		void OnSize(wxSizeEvent &event);
		void OnErase(wxEraseEvent &event);
		void OnPaint(wxPaintEvent &event);
		void OnMouse(wxMouseEvent &event);
		void OnBtnBuy(wxCommandEvent &event);

		wxDECLARE_EVENT_TABLE();
	};

	class InfoView
		: public BaseViewT<InfoView>
	{
		typedef InfoView This;
	public:
		typedef BaseViewT<InfoView> Base;
	protected:
		wxSize best_size_;
		std::vector<mdb::Field> fields_; //0表示换行, 1表示要换行且显示换行线
	public:
		InfoView(wxWindow* parent, const char* xml = nullptr, size_t xmlflag = XUtil::XML_FLAG_JSON_STRING);
		virtual ~InfoView();

		void OnSkinInfoChanged();

	protected:
		void CalcBestSize();

		void Draw(wxDC& dc);

		void OnSize(wxSizeEvent &event);
		void OnErase(wxEraseEvent &event);
		void OnPaint(wxPaintEvent &event);
		void OnMouse(wxMouseEvent &event);

		wxDECLARE_EVENT_TABLE();
	};

	class TickView
		: public BaseViewT<InfoView>
	{
		typedef TickView This;
	public:
		typedef BaseViewT<InfoView> Base;
	protected:
		wxSize best_size_;
	public:
		TickView(wxWindow* parent, const char* xml = nullptr, size_t xmlflag = XUtil::XML_FLAG_JSON_STRING);
		virtual ~TickView();

		void OnSkinInfoChanged();

	protected:
		void CalcBestSize();

		void Draw(wxDC& dc);

		void OnErase(wxEraseEvent &event);
		void OnPaint(wxPaintEvent &event);

		wxDECLARE_EVENT_TABLE();
	};

	wxDECLARE_EVENT(EVT_SPLITTER_CHANGE_EVENT, wxCommandEvent);

	class SplitterSizer : public wxBoxSizer, public wxEvtHandler
	{
		typedef wxBoxSizer Base;
	public:
		//using Base::Base;
		SplitterSizer(wxWindow* window, int orient);
		~SplitterSizer();

		wxSizerItem* AddSplitter(int size = 1);

		wxSizerItem* GetItemByPoint(const wxPoint& pt, size_t* index = nullptr);
		bool IsSplitter(wxSizerItem* item);

	protected:
		void OnDestroy(wxWindowDestroyEvent &event);
		void OnSetCursor(wxSetCursorEvent &event);
		void OnMouseEnter(wxMouseEvent &event);
		void OnMouseLeave(wxMouseEvent &event);
		void OnMouseMove(wxMouseEvent &event);
		void OnMouseDown(wxMouseEvent &event);
		void OnMouseUp(wxMouseEvent &event);

		wxWindow* window_ = nullptr;
		std::vector<wxSizerItem*> splitters_;
		wxOverlay overlay_;
		wxPoint pt_capture_;
		wxSizerItem* item_capture_ = nullptr;
		size_t index_capture_ = 0;
	};

}
