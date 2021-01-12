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

	class Indicator;
	class Windicator;
	class TechView;
	class TitleView;
	class MmpView;
	class TradeView;
	class InfoView;
	class TickView;

	//技术视图TechView，拥有主图Windicator、0或者N个幅图Windicator，每个图里面有多个指标Indicator
	//技术视图更新逻辑：
	//1、SkinInfo变化 -> DispInfo/DispInfoEx变化 ->重新绘制
	//2、CalcData变化 -> DispInfo/DispInfoEx变化 ->重新计算 -> 重新绘制
	//3、CalcData更新 -> DispInfo/DispInfoEx变化 ->重新计算 -> 重新绘制
	//4、CalcInput变化 -> 具体Windicator的DispInfo/DispInfoEx变化 -> 重新计算 -> 重新绘制
	//5、叠加指标 -> 具体Windicator的DispInfo/DispInfoEx变化 -> 重新计算 -> 重新绘制
	//6、增减指标窗口 -> DispInfo/DispInfoEx变化 -> 重新计算 -> 重新绘制
	//7、叠加CODE -> 叠加的CODE拥有独立DispInfo/DispInfoEx信息(即独立坐标) -> 叠加的CODE指标重新计算 -> 重新绘制
	//8、拖动时间轴 -> DispInfo/DispInfoEx变化 ->重新计算 -> 重新绘制
	//9、拖拉指标窗口大小 -> 重新绘制
	//10、TechView窗口大小变化 -> DispInfo/DispInfoEx变化 ->重新绘制

	class DispInfo
	{
	public:
		DispInfo()
		{
			hCalcData = nullptr;

			//HighPrice = 0;
			//LowPrice = 0;
			//HighVolume = 0;
			//LowVolume = 0;
			StartDate = 0;
			StartTime = 0;
			Start = 0;
			Count = 0;
			Shift = 0;
			MaxCount = 0;

			Current = 0;
			//show_cross_cursor;

			InfoHeight = 0;
			InfoWidth = 0;
			TopSpace = 0;
			BottomSpace = 0;

			Scale = 0;

			KLineType = CALC_KLINE_K;
			YCoordType = CALC_COORD_NORMAL;
			XCoordType = CALC_COORD_NORMAL;
		}

		HZQDB hCalcData;				//计算数据
		//size_t HighPrice;				//最高价位置
		//size_t LowPrice;				//最低价位置
		//size_t HighVolume;			//最高量位置	
		//size_t LowVolume;				//最低量位置
		uint32_t StartDate;				//开始日期
		uint32_t StartTime;				//开始时间
		size_t Start;					//开始位置
		size_t Count;					//显示数目
		size_t Shift;					//偏移数目（默认显示时偏移）
		size_t MaxCount;				//最大可显示数目

		size_t Current;					//当前位置
		wxPoint show_cross_cursor;		//是否显示当前位置（屏幕坐标）十字游标，(0,0)表示不显示

		size_t InfoHeight;				//顶部标题信息栏高度
		size_t InfoWidth;				//左/右侧坐标信息栏宽度
		size_t TopSpace;				//标题栏和主图间隙
		size_t BottomSpace;				//底部间隙

		size_t Scale;					//BAR的调整

		CALC_KLINE_TYPE KLineType;		//K线类型
		CALC_STICK_TYPE KVolumeType;	//量额线型
		CALC_COORD_TYPE YCoordType;		//Y轴坐标类型
		CALC_COORD_TYPE XCoordType;		//X轴坐标类型
	};
	
	class DispInfoEx
	{
	public:
		DispInfoEx()
		{
			MaxValue = INVALID_VALUE;
			MinValue = INVALID_VALUE;
		}

		double MaxValue; //最大值
		double MinValue; //最小值
	};
	class IndexInfo
	{
	public:
		IndexInfo()
		{
			index = 0;

			minvalue = INVALID_VALUE;
			maxvalue = INVALID_VALUE;
			mindispvalue = INVALID_VALUE;
			maxdispvalue = INVALID_VALUE;

			pos = 0;
			result = 0.;
			//pt;
		}

		size_t index;

		//CGET_INDICATOR_INDEXINFO

		//获取最小值最大值
		double minvalue;
		double maxvalue;
		double mindispvalue;
		double maxdispvalue;

		//CGET_INDICATOR_INDEXINFOEX

		size_t pos;
		//获得pos位置的buffer线的值和点等信息
		double result;
		wxPoint pt;
	};

	class PointInfo
	{
	public:
		PointInfo()
		{
			//pt;
			pos = 0;
			result = 0.;
		}

		wxPoint pt;
		//获得点pt所在的pos和值等信息
		size_t pos;
		double result;
	};
	class PointInfoEx : public PointInfo
	{
	public:
		PointInfoEx() : PointInfo()
		{
			indicator = nullptr;
			index = 0;
		}

		Indicator* indicator;
		//获得pt是否在indicator的index线上，如果index<index个数则在index线上，如果在整个指标上index==index个数，其他为不在index上
		unsigned long index;
	};

	inline bool IsShowCrossCursor(const wxPoint& pt) { return pt.x != 0 && pt.y != 0; }

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

		void OnNotify(const wxString& name, int type, int pos, HZQDB h)
		{

		}
	};

	template<class T>
	class CalcDataMap
	{
		typedef CalcDataMap<T> This;
	protected:
		std::shared_ptr<zqdb::Calc::Data> calc_data_ptr_;

	public:
		void OnCalcDataChanged()
		{

		}
		void OnCalcDataUpdated()
		{

		}

		void Clear()
		{
			calc_data_ptr_.reset();
		}
		inline bool IsDispOk() const { return calc_data_ptr_ != nullptr; }
		const std::shared_ptr<zqdb::Calc::Data>& GetCalcData() const
		{
			return calc_data_ptr_;
		}
		void SetCalcData(const std::shared_ptr<zqdb::Calc::Data>& calc_data)
		{
			T* pT = static_cast<T*>(this);
			calc_data_ptr_ = calc_data;
			pT->OnCalcDataChanged();
		}
		void UpdateCalcData()
		{
			T* pT = static_cast<T*>(this);
			pT->OnCalcDataUpdated();
		}

		const char* GetCode()
		{
			return calc_data_ptr_->GetCode();
		}

		PERIODTYPE GetCycle(int* pCycleEx)
		{
			if (pCycleEx) {
				*pCycleEx = calc_data_ptr_->GetCycleEx();
			}
			return calc_data_ptr_->GetCycle();
		}

		/*ENUM_DWTYPE GetDWType(unsigned long* pDWDate)
		{
		if (pDWDate) {
		*pDWDate = calc_info_ptr_->DWDate;
		}
		return calc_info_ptr_->DWType;
		}*/

		size_t GetBarCount() const
		{
			return calc_data_ptr_->GetDataCount();
		}

		bool IsBarPosOk(int nPos) const
		{
			if (nPos >= 0 && nPos<GetBarCount()) {
				return true;
			}
			return false;
		}
	};

	template<class T>
	class CalcInputMap
	{
	protected:
		std::shared_ptr<zqdb::Calc::InputAttr> calc_input_ptr_;

	public:
		void Clear()
		{
			calc_input_ptr_.reset();
		}
		const std::shared_ptr<zqdb::Calc::InputAttr>& GetCalcInput() const
		{
			return calc_input_ptr_;
		}
		void SetCalcInput(const std::shared_ptr<zqdb::Calc::InputAttr>& calc_input_ptr)
		{
			T* pT = static_cast<T*>(this);
			calc_input_ptr_ = calc_input_ptr;
			pT->OnCalcInputChanged();
		}
		void OnCalcInputChanged()
		{
		}
	};

#define INDICATOR_POINT_RECT_WIDTH	2
#define INDICATOR_POINT_RECT_HEIGHT	2

#define INDICATOR_DISP_CHANGED_CURRENT	0X01
#define INDICATOR_DISP_CHANGED_CROSSCURSOR	0X02

	template<class T>
	class DispMap
	{
	protected:
		std::shared_ptr<DispInfo> disp_info_ptr_;

	public:
		void OnDispInfoChanged(size_t Changed)
		{

		}

		void Clear()
		{
			disp_info_ptr_.reset();
		}
		inline bool IsDispOk() const { return disp_info_ptr_ != nullptr; }
		const std::shared_ptr<DispInfo>& GetDispInfo() const
		{
			return disp_info_ptr_;
		}
		void SetDispInfo(const std::shared_ptr<DispInfo>& disp_info_ptr)
		{
			ASSERT(disp_info_ptr);
			T* pT = static_cast<T*>(this);
			size_t Changed = 0;
			std::shared_ptr<DispInfo> old_disp_info_ptr = disp_info_ptr_;
			disp_info_ptr_ = disp_info_ptr;
			ASSERT(old_disp_info_ptr != disp_info_ptr);
			if (old_disp_info_ptr && disp_info_ptr
				&& old_disp_info_ptr->hCalcData == disp_info_ptr->hCalcData
				&& old_disp_info_ptr->Start == disp_info_ptr->Start
				&& old_disp_info_ptr->Count == disp_info_ptr->Count
				&& old_disp_info_ptr->Shift == disp_info_ptr->Shift
				&& old_disp_info_ptr->MaxCount == disp_info_ptr->MaxCount
				&& old_disp_info_ptr->InfoHeight == disp_info_ptr->InfoHeight
				&& old_disp_info_ptr->InfoWidth == disp_info_ptr->InfoWidth
				&& old_disp_info_ptr->TopSpace == disp_info_ptr->TopSpace
				&& old_disp_info_ptr->BottomSpace == disp_info_ptr->BottomSpace
				&& old_disp_info_ptr->Scale == disp_info_ptr->Scale
				&& old_disp_info_ptr->KLineType == disp_info_ptr->KLineType
				&& old_disp_info_ptr->KVolumeType == disp_info_ptr->KVolumeType
				&& old_disp_info_ptr->YCoordType == disp_info_ptr->YCoordType
				&& old_disp_info_ptr->XCoordType == disp_info_ptr->XCoordType) {
				if (old_disp_info_ptr->Current != disp_info_ptr->Current) {
					Changed |= INDICATOR_DISP_CHANGED_CURRENT;
				}
				if (old_disp_info_ptr->show_cross_cursor != disp_info_ptr->show_cross_cursor) {
					Changed |= INDICATOR_DISP_CHANGED_CROSSCURSOR;
				}
			}
			else {
				Changed = (size_t)-1;
			}
			pT->OnDispInfoChanged(Changed);
		}

		//DISP RECT
		//|--|----------------------|--|
		//R -|        TOP           |- L
		//I -|----------------------|- E
		//G M----A -  TECH  - I------N F 
		//H -|----------------------|- T
		//T -|       BOTTOM         |- |   
		//|-------------------------|--|
		Rect GetClientRect() const
		{
			ASSERT(0);
			return Rect();
		}

		Rect GetLeftRect() const
		{
			auto pT = static_cast<const T*>(this);
			Rect rc = pT->GetClientRect();
			if (disp_info_ptr_->InfoWidth >= 0) {
				rc.width = disp_info_ptr_->InfoWidth;
			}
			else {
				rc.width = 0;
			}
			return rc;
		}

		Rect GetRightRect() const
		{
			auto pT = static_cast<const T*>(this);
			Rect rc = pT->GetClientRect();
			if (disp_info_ptr_->InfoWidth >= 0) {
				rc.DeflateLeft(rc.width);
			}
			else {
				rc.DeflateLeft(rc.width + disp_info_ptr_->InfoWidth);
			}
			return rc;
		}

		Rect GetTopRect() const
		{
			auto pT = static_cast<const T*>(this);
			Rect rc = pT->GetClientRect();
			rc.height = disp_info_ptr_->InfoHeight + disp_info_ptr_->TopSpace;
			return rc;
		}

		Rect GetBottomRect() const
		{
			auto pT = static_cast<const T*>(this);
			Rect rc = pT->GetClientRect();
			rc.DeflateTop(rc.height - disp_info_ptr_->BottomSpace);
			return rc;
		}

		Rect GetTitleRect() const
		{
			Rect rc = GetTopRect();
			if (IsInfoBarLeft()) {
				Rect rcLeft = GetLeftRect();
				rc.DeflateLeft(rcLeft.width);
			}
			else {
				Rect rcRight = GetRightRect();
				rc.DeflateRight(rcRight.width);
			}
			rc.height = disp_info_ptr_->InfoHeight;
			return rc;
		}

		Rect GetMainRect() const
		{
			auto pT = static_cast<const T*>(this);
			Rect rc = pT->GetClientRect();
			Rect rcTop = GetTopRect();
			Rect rcBottom = GetBottomRect();
			rc.DeflateTop(rcTop.height);
			rc.DeflateBottom(rcBottom.height);
			return rc;
		}

		Rect GetTechRect() const
		{
			auto pT = static_cast<const T*>(this);
			Rect rc = pT->GetMainRect();
			Rect rcLeft = GetLeftRect();
			Rect rcRight = GetRightRect();
			rc.DeflateLeft(rcLeft.width);
			rc.DeflateRight(rcRight.width);
			return rc;
		}

		Rect GetInfoBarRect() const
		{
			if (IsInfoBarLeft()) {
				return GetLeftRect();
			}
			else {
				return GetRightRect();
			}
		}

		bool IsInfoBarLeft() const
		{
			if (disp_info_ptr_->InfoWidth < 0) {
				return false;
			}
			return true;
		}

		size_t GetBarWidth() const
		{
			auto pT = static_cast<const T*>(this);
			return pT->GetSkinInfo()->nBarWidth[disp_info_ptr_->Scale];
		}

		size_t GetBarPitch() const
		{
			auto pT = static_cast<const T*>(this);
			return pT->GetSkinInfo()->nBarSpace[disp_info_ptr_->Scale];
		}

		bool IsBarDispOk() const
		{
			if (disp_info_ptr_->Start >= 0 && disp_info_ptr_->Count > 0) {
				return true;
			}
			return false;
		}

		int GetBarDispStart() const
		{
			return disp_info_ptr_->Start;
		}

		int GetBarDispEnd() const
		{
			return disp_info_ptr_->Start + disp_info_ptr_->Count;
		}

		size_t GetBarDispCount() const
		{
			return disp_info_ptr_->Count;
		}

		size_t GetBarDispMaxCount() const
		{
			return disp_info_ptr_->MaxCount;
		}

		bool IsBarDispPosOk(int nPos) const
		{
			if (nPos >= disp_info_ptr_->Start&&nPos<(disp_info_ptr_->Start + disp_info_ptr_->Count)) {
				return true;
			}
			return false;
		}
	};

	template<class T>
	class IndicatorMap 
		: public DispMap<T>
		, public CalcDataMap<T>
	{
	public:
		typedef DispMap<T> DispBase;
		typedef CalcDataMap<T> CalcDataBase;
	protected:
		std::shared_ptr<DispInfoEx> disp_info_ex_ptr_;

	public:
		void OnDispInfoExChanged()
		{

		}

		void Clear()
		{
			DispBase::Clear();
			CalcDataBase::Clear();
			disp_info_ex_ptr_.reset();
		}
		inline bool IsDispOk() const {
			return DispBase::IsDispOk() && CalcDataBase::IsDispOk() && disp_info_ex_ptr_;
		}
		/*inline bool IsBarDispOk() const {
			if (IsDispOk()) {
				if (DispBase::IsBarDispOk()) {
					auto nBarCount = GetBarCount();
					if ((disp_info_ptr_->Start + disp_info_ptr_->Count) <= nBarCount) {
						return true;
					}
				}
			}
			return false;
		}*/
		const std::shared_ptr<DispInfoEx>& GetDispInfoEx() const
		{
			return disp_info_ex_ptr_;
		}
		void SetDispInfoEx(const std::shared_ptr<DispInfoEx>& disp_info_ex_ptr)
		{
			T* pT = static_cast<T*>(this);
			disp_info_ex_ptr_ = disp_info_ex_ptr;
			pT->OnDispInfoExChanged();
		}
	};

	//////////////////////////////////////////////////////////////////////////

	template<class T, class TBase = wxPanel>
	class BaseViewT
		: public TBase
		, public SkinMap<T>
		, public HandleMap<T>
	{
		typedef BaseViewT<T, TBase> This;
	public:
		typedef TBase Base;
		typedef SkinMap<T> SkinBase;
		typedef HandleMap<T> HandleBase;
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

		void OnNotify(const wxString& name, int type, int pos, HZQDB h)
		{
			if (h == h_) {
				Refresh();
			}
		}

		bool IsDispOk() const { return skin_info_ptr_ && h_; }

		Rect GetClientRect() const { return Base::GetClientRect(); }
	};

	class Indicator 
		: public zqdb::Calc::Calculator
		, public SkinMap<Indicator>
		, public HandleMap<Indicator>
		, public CalcInputMap<Indicator>
		, public IndicatorMap<Indicator>
	{
		typedef Indicator This;
		typedef zqdb::Calc::Calculator Base;
		typedef SkinMap<Indicator> SkinBase;
		typedef HandleMap<Indicator> HandleBase;
		typedef CalcInputMap<Indicator> CalcInputBase;
		typedef IndicatorMap<Indicator> IndicatorBase;
	public:
		enum Type
		{
			CALC,
			K,
			AMOUNT,
			VOLUME,
		};
		static Type IsType(const char* name) {
			if (stricmp(name, "K") == 0) {
				return K;
			}
			else if (stricmp(name, "AMOUNT") == 0) {
				return AMOUNT;
			}
			else if (stricmp(name, "VOLUME") == 0) {
				return VOLUME;
			}
			return CALC;
		}
		//REFLINE
		typedef struct tagRefline
		{
			char name[MAX_NAME_LENGTH + 1];
			uint32_t type : 4;				//ENUM_REFLINE_TYPE
			uint32_t width : 4;				//线宽
			uint32_t style : 4;				//CALC_LINE_STYLE
			uint32_t digits : 4;			//指标精度,如果指定了精度就不使用商品的digits
			uint32_t color;					//指标颜色
			double dvalue;					//参考值
		}REFLINE, *PREFLINE;
		typedef struct tagResultInfo
		{
			char name[MAX_NAME_LENGTH + 1];
			uint32_t type : 4;				//CALC_RESULT_TYPE
			uint32_t shift : 8;			//指标偏移
			uint32_t begin : 8;			//指标开始
			uint32_t draw : 4;				//CALC_DRAW_TYPE
			uint32_t line : 4;				//CALC_LINE_TYPE
			uint32_t next : 4;				//画线关联指标线
			uint32_t : 0;
			uint32_t digits : 4;			//指标精度,如果指定了精度就不使用商品的digits
			uint32_t width : 4;			//线宽
			uint32_t color;			//指标颜色
			//double min_value;		//指标最大值
			//double max_value;		//指标最小值
			//double base_value;	//基准值
			//int level_count:8;	//水平线数目
			//int level_style:24;	//水平线风格
			//COLORREF level_color;	//水平线颜色
		}RESULTINFO, *PRESULTINFO;
	protected:
		Windicator* parent_ = nullptr;
		//显示信息,所有显示的RECT POINT等信息都是相对于父窗口的位置
		Rect rect_;
		wxString name_;
		Type type_ = CALC;
		CALC_TYPE calc_type_ = CALC_MAJOR;
		std::vector<REFLINE> reflines_;
		std::vector<RESULTINFO> results_;

		//这里记录了指标对应Line的MMX值的位置
		std::vector<std::array<size_t, CALC_MMX_TYPE_MAX>> mmxposs_;
		//这里记录了所有Line里的MMX值所在Line
		std::array<size_t, CALC_MMX_TYPE_MAX> mmxindex_;

		//这里记录了指标对应Line的显示的MMX值的位置
		std::vector<std::array<size_t, CALC_MMX_TYPE_MAX>> mmxdispposs_;
		//这里记录了所有Line里的显示的MMX值所在Line
		std::array<size_t, CALC_MMX_TYPE_MAX> mmxdispindex_;

	public:
		Indicator(Windicator* parent, const char* xml = nullptr, size_t xmlflag = XUtil::XML_FLAG_JSON_STRING);
		virtual ~Indicator();

		inline const wxString& GetName() { return name_; }
		inline bool IsK() { return type_ == K; }
		inline bool IsVolume() { return type_ == VOLUME; }
		inline bool IsAmount() { return type_ == AMOUNT; }
		inline bool IsMajor() { return calc_type_ == CALC_MAJOR; }
		inline const std::vector<RESULTINFO>& GetResultInfo() { return results_; }

		bool IsOpen();
		void Open();
		void Close();

		inline size_t GetResultCount();
		inline size_t GetResultAttrCount(size_t index);
		inline ssize_t GetResultAttrAsInt(size_t index, MDB_FIELD& field, const ssize_t def = 0);
		inline double GetResultAttrAsDouble(size_t index, MDB_FIELD& field, const double def = 0);
		inline char* GetResultAttrAsStr(size_t index, MDB_FIELD& field, char* val, size_t vallen);

		size_t GetResultValueMaxCount();
		size_t GetResultValueCount();
		void* GetResultValue(size_t index, MDB_FIELD* field = nullptr);
		void* GetResultValue(size_t index, size_t pos, MDB_FIELD* field = nullptr);
		ssize_t GetResultValueAsInt(size_t index, size_t pos, ssize_t def = 0);
		double GetResultValueAsDouble(size_t index, size_t pos, double def = 0.);
		char* GetResultValueAsStr(size_t index, size_t pos, char* val, size_t vallen);

		//bool IsUpdateUI();
		void Calc();

		Rect GetClientRect() const { return rect_; }
		void Move(const wxPoint& pt);
		void SetSize(const wxSize& sz);
		void SetRect(const Rect& rc);

		void OnMove();
		void OnSize();

		void OnSkinInfoChanged();
		void OnCalcDataChanged();
		void OnCalcDataUpdated();
		void OnCalcInputChanged();
		void OnDispInfoChanged(size_t Changed);
		void OnDispInfoExChanged();

		void Refresh();

		void PaintBkgnd(wxDC& dc);
		void Paint(wxDC& dc);

		//实际上这些函数可以返回非显示部分的数据,所以有可能有负值，或者Pos不在显示区域
		bool GetDispIndexPoint(size_t index, size_t pos, wxPoint& pt);
		bool GetDispValuePoint(double dvalue, size_t pos, wxPoint& pt);
		bool GetDispPointValue(const wxPoint& pt, size_t& pos, double* dvalue = nullptr);

		bool GetDispIndexInfo(std::shared_ptr<IndexInfo> infoPtr);
		bool GetDispPointInfo(std::shared_ptr<PointInfo> infoPtr);
		virtual bool GetDispPointInfoEx(std::shared_ptr<PointInfoEx> infoPtr);

		bool GetIndexInfo(std::shared_ptr<IndexInfo> info_ptr);
		bool GetIndexInfoEx(std::shared_ptr<IndexInfo> info_ptr);

	protected:
		//
		inline void ResetResultValueMMXPos();
		inline size_t GetResultValueMMXPos(size_t index, CALC_MMX_TYPE mmx) {
			if (index >= mmxposs_.size()) {
				return INVALID_POS;
			}
			return mmxposs_[index][mmx];
		}
		inline size_t GetResultValueMMXIndex(CALC_MMX_TYPE mmx) {
			return mmxindex_[mmx];
		}
		inline ssize_t GetResultValueMMXAsInt(size_t index, CALC_MMX_TYPE mmx, ssize_t def = 0) {
			return GetResultValueAsInt(index, GetResultValueMMXPos(index, mmx), def);
		}
		inline double GetResultValueMMXAsDouble(size_t index, CALC_MMX_TYPE mmx, double def = 0.) {
			return GetResultValueAsDouble(index, GetResultValueMMXPos(index, mmx), def);
		}
		inline char* GetResultValueMMXAsStr(size_t index, CALC_MMX_TYPE mmx, char* val, size_t vallen) {
			return GetResultValueAsStr(index, GetResultValueMMXPos(index, mmx), val, vallen);
		}
		inline ssize_t GetResultValueMMXAsInt(CALC_MMX_TYPE mmx, ssize_t def = 0) {
			auto index = GetResultValueMMXIndex(mmx);
			return GetResultValueAsInt(index, GetResultValueMMXPos(index, mmx), def);
		}
		inline double GetResultValueMMXAsDouble(CALC_MMX_TYPE mmx, double def = 0.) {
			auto index = GetResultValueMMXIndex(mmx);
			return GetResultValueAsDouble(index, GetResultValueMMXPos(index, mmx), def);
		}
		inline char* GetResultValueMMXAsStr(CALC_MMX_TYPE mmx, char* val, size_t vallen) {
			auto index = GetResultValueMMXIndex(mmx);
			return GetResultValueAsStr(index, GetResultValueMMXPos(index, mmx), val, vallen);
		}

		inline void ResetResultValueDispMMXPos();
		inline size_t GetResultValueDispMMXPos(size_t index, CALC_MMX_TYPE mmx) {
			if (index >= mmxdispposs_.size()) {
				return INVALID_POS;
			}
			return mmxdispposs_[index][mmx];
		}
		inline size_t GetResultValueDispMMXIndex(CALC_MMX_TYPE mmx) {
			return mmxdispindex_[mmx];
		}
		inline ssize_t GetResultValueDispMMXAsInt(size_t index, CALC_MMX_TYPE mmx, ssize_t def = 0) {
			return GetResultValueAsInt(index, GetResultValueDispMMXPos(index, mmx), def);
		}
		inline double GetResultValueDispMMXAsDouble(size_t index, CALC_MMX_TYPE mmx, double def = 0.) {
			return GetResultValueAsDouble(index, GetResultValueDispMMXPos(index, mmx), def);
		}
		inline char* GetResultValueDispMMXAsStr(size_t index, CALC_MMX_TYPE mmx, char* val, size_t vallen) {
			return GetResultValueAsStr(index, GetResultValueDispMMXPos(index, mmx), val, vallen);
		}
		inline ssize_t GetResultValueDispMMXAsInt(CALC_MMX_TYPE mmx, ssize_t def = 0) {
			auto index = GetResultValueDispMMXIndex(mmx);
			return GetResultValueAsInt(index, GetResultValueDispMMXPos(index, mmx), def);
		}
		inline double GetResultValueDispMMXAsDouble(CALC_MMX_TYPE mmx, double def = 0.) {
			auto index = GetResultValueDispMMXIndex(mmx);
			return GetResultValueAsDouble(index, GetResultValueDispMMXPos(index, mmx), def);
		}
		inline char* GetResultValueDispMMXAsStr(CALC_MMX_TYPE mmx, char* val, size_t vallen) {
			auto index = GetResultValueDispMMXIndex(mmx);
			return GetResultValueAsStr(index, GetResultValueDispMMXPos(index, mmx), val, vallen);
		}

		inline double GetDispMMXAsDouble(CALC_MMX_TYPE mmx, double def = 0.) {
			switch(mmx) 
			{
			case CALC_MMX_MIN:
				return disp_info_ex_ptr_->MinValue;
			case CALC_MMX_MAX:
				return disp_info_ex_ptr_->MaxValue;
			}
			return def;
		}

	protected:
		int GetBarDispStart(int index = -1);
		int GetBarDispEnd(int index = -1);
		size_t GetBarDispCount(int index = -1);
		bool IsBarDispPosOk(int nPos, int index = -1);

		virtual void DrawOneRefline(wxDC& dc, const wxString& text, int x1, int y1, int x2);
		virtual void DrawRefline(wxDC& dc, const Rect& rc, double maxvalue, double minvalue, double yscale, int ybase);

	};

	class Windicator
		: public BaseViewT<Windicator>
		, public IndicatorMap<Windicator>
	{
		typedef Windicator This;
		typedef BaseViewT<Windicator> Base;
		typedef IndicatorMap<Windicator> IndicatorBase;
	protected:
		std::vector<Indicator*> objs_;
		size_t GetObjPos(Indicator* pObj);
		void Broadcast(const std::function<bool(size_t pos, Indicator* obj)>& f);
		
		std::vector<std::vector<std::shared_ptr<IndexInfo>>> index_info_ptrs_; //指标信息和指定位置信息（值，坐标）
		std::shared_ptr<PointInfoEx> point_info_ptr_; //坐标信息（位置，值）
		std::vector<Rect> info_rects_; //指标信息栏位置

		wxMemoryDC dc_;
		wxMemoryDC dc_info_;
		wxMemoryDC dc_cross_cursor_;

		bool major_flag_ = false; //是否主图指标
		bool drag_xcoord_flag_ = false; //拖拽时间轴
		wxPoint drag_xcoord_pt_; //拖拽时开始位置
		bool draw_line_flag_ = false; //是否画线中
		CALC_DRAWLINE_TYPE draw_line_type_ = CALC_DRAWLINE_NONE; //画线索引，即画什么线，0表示不画线
	public:
		Windicator(TechView* parent, const char* xml = nullptr, size_t xmlflag = XUtil::XML_FLAG_JSON_STRING);
		virtual ~Windicator();

		void Clear();

		Indicator* AddIndicator(const char* name, const char* xml = nullptr, size_t xmlflag = XUtil::XML_FLAG_JSON_FILE);
		void RemoveIndicator(size_t Pos);
		void RemoveAllIndicator();

		void OnSkinInfoChanged();
		void OnCalcDataChanged(); 
		void OnCalcDataUpdated();
		void OnDispInfoChanged(size_t Changed);

		bool IsDispOk() const;

		Rect GetClientRect() const { return Base::GetClientRect(); }

		bool GetIndexInfo(std::shared_ptr<IndexInfo> info_ptr);
		bool GetIndexInfoEx(std::shared_ptr<IndexInfo> info_ptr);
		//bool GetDispIndexInfo(std::shared_ptr<IndexInfo> info_ptr);
		bool GetPointInfo(std::shared_ptr<PointInfo> info_ptr);
		bool GetPointInfoEx(std::shared_ptr<PointInfoEx> info_ptr);

		bool IsAnyOperator() {
			return drag_xcoord_flag_ || draw_line_flag_;
		}

	protected:
		//
		void UpdateUI(size_t Changed);
		void UpdateIndexInfos();
		bool UpdateDispInfoEx(); //更新显示最大最小值
		void UpdateIndexInfosByPos(size_t pos);
		void UpdateIndexInfosByPoint(wxPoint pt);

		void Draw(wxDC& dc);
		void DrawInfo(wxDC& dc);
		void DrawCrossCursor(wxDC& dc);

		void OnSize(wxSizeEvent &event);
		void OnErase(wxEraseEvent &event);
		void OnPaint(wxPaintEvent &event);
		void OnSetCursor(wxSetCursorEvent &event);
		void OnMouse(wxMouseEvent &event); 
		void OnCaptureLost(wxMouseCaptureLostEvent& event);
		void OnChar(wxKeyEvent &event);

		wxDECLARE_EVENT_TABLE();
	};

	class Windxcoord : public Windicator
	{
		typedef Windicator Base;
	public:
		using Windicator::Windicator;

	private:
		//
		void DrawTick(wxDC &dc, Rect &rc, const wxString& text);
		void DrawCoordinate(wxDC& dc);
		void DrawCrossCursor(wxDC& dc);

		void OnPaint(wxPaintEvent &event);

		wxDECLARE_EVENT_TABLE();
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

		void OnNotify(const wxString& name, int type, int pos, HZQDB h);

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

	//技术视图，拥有主图、1或者N个幅图(至少有个时间轴)，每个图里面有多个指标Indicator
	class TechView 
		: public BaseViewT<TechView, wxScrolledWindow>
		, public DispMap<TechView>
		, public CalcDataMap<TechView>
	{
	public:
		typedef BaseViewT<TechView, wxScrolledWindow> Base;
		typedef DispMap<TechView> DispBase;
		typedef CalcDataMap<TechView> CalcDataBase;
	protected:
		SplitterSizer  *tech_sizer_ = nullptr;

		//windicator
		std::vector<Windicator*> objs_;
		void Broadcast(const std::function<bool(size_t pos, Windicator* obj)>& f);

		std::shared_ptr<DispInfo> old_disp_info_ptr_;
		std::shared_ptr<DispInfo> disp_info_ptr_;

		wxPoint show_cross_cursor_; //是否显示当前位置（屏幕坐标）十字游标，(0, 0)表示不显示
		std::shared_ptr<PointInfo> point_info_ptr_;	//坐标信息（位置，值）
		std::shared_ptr<IndexInfo> index_info_ptr_;	//K线信息（值，坐标）

		//topview or rightview
		TitleView* title_view_ = nullptr;
		MmpView* mmp_view_ = nullptr;
		TradeView* trade_view_ = nullptr;
		InfoView* info_view_ = nullptr;
		TickView* tick_view_ = nullptr;

		/*wxSizerItem* item_sizer_ = nullptr;
		size_t item_sizer_index_ = 0;
		wxOverlay overlay_;
		wxPoint pt_capture_;*/
	public:
		TechView(wxWindow *parent, const char* xml = nullptr, size_t xmlflag = XUtil::XML_FLAG_JSON_STRING);
		~TechView();

		void OnSkinInfoChanged();
		void OnCalcDataChanged();
		void OnHandleChanged();
		void OnNotify(const wxString& name, int type, int pos, HZQDB h);
		void OnCalcDataUpdated();

		inline bool IsDispOk() const {
			return Base::IsDispOk() && DispBase::IsDispOk() && CalcDataBase::IsDispOk();
		}
		inline bool IsBarDispOk() const {
			if (IsDispOk()) {
				if (DispBase::IsBarDispOk()) {
					auto nBarCount = GetBarCount();
					if ((disp_info_ptr_->Start + disp_info_ptr_->Count) <= nBarCount) {
						return true;
					}
				}
			}
			return false;
		}

		bool IsAnyOperator();

		void Scale(int nOffset);
		void Move(int nOffset);
		void MoveTo(int nPos);

		//bool IsCrossCursorVisible() { return IsShowCrossCursor(show_cross_cursor_); }
		//void HideCrossCursor();
		void InnerMoveCurrentTo(int nPos, const wxPoint& show_cross_cursor);
		void MoveCurrentTo(int nPos);
		void MoveCurrentTo(const wxPoint& pt);
		void MoveCurrent(int nOffset);
		void UpdateCurrent();
		void HideCurrent();

	protected:
		//
		void UpdateUI();

		int GetBarPosBy(const wxPoint& pt);
		//POINT GetBarPointBy(int nPos);

		Rect GetClientRect() const { return Base::GetClientRect(); }

		void SwapDispInfo();
		enum { UD_NONE = 0, UD_BARSCHANGE, UD_SIZECHANGE, UD_DISPCHANGE, };
		void UpdateDispInfo(int udType = UD_NONE);

		//wxSizerItem* GetItemByPoint(const wxPoint& pt, size_t* index = nullptr);

		void OnSplitterChanged(wxCommandEvent &event);
		void OnSize(wxSizeEvent &event);
		void OnErase(wxEraseEvent &event);
		void OnPaint(wxPaintEvent &event);
		/*void OnSetCursor(wxSetCursorEvent &event);
		void OnMouseEnter(wxMouseEvent &event);
		void OnMouseLeave(wxMouseEvent &event);
		void OnMouseMove(wxMouseEvent &event);
		void OnMouseDClick(wxMouseEvent &event);
		void OnMouseDown(wxMouseEvent &event);
		void OnMouseUp(wxMouseEvent &event);*/

		wxDECLARE_EVENT_TABLE();
	};
}
