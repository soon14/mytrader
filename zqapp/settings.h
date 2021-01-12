#pragma once

#include "utility.h"
#include <view.h>

class SkinInfo : public zqdb::SkinInfo
{
public:

	//鼠标
	
	//颜色
	std::shared_ptr<wxRibbonDefaultArtProvider> artProvider;
	wxColour crPrimary;			//主色调（背景色调）
	wxColour crSecondary;		//次色调（高亮色调）
	wxColour crTertiary;		//第三色调（文字色调）

	wxColour crViewBkgnd;		//视图色（二级窗口）
	wxColour crViewForgnd;		//视图文字
	wxColour crCtrlBkgnd;		//控件色（三级窗口）
	wxColour crCtrlForgnd;		//视图文字
	
	//画笔

	//画刷

	//字体

	//字体高宽

	//窗体高宽（相对于水平或者垂直布局）
};

template<class T>
class SettingsMap 
	: public boost::property_tree::ptree
	, public zqdb::SkinMap<T,SkinInfo>
{
	typedef zqdb::SkinMap<T, SkinInfo> SkinBase;
public:

	void Clear()
	{
		SkinBase::Clear();
	}
	inline bool IsDispOk() const {
		return SkinBase::IsDispOk();
	}
};
