#pragma once
#include "base.h"
#include <XUtil/XXml.hpp>
#include <XUtil/XDateTime.hpp>

	/*inline string wxString2gbk(const wxString& src)
	{
		wxCSConv cvGB2312(wxT("GB2312"));
		return string(cvGB2312.cWX2MB(src.c_str()));
	}
	inline wxString gbk2wxString(const string& src)
	{
		wxCSConv cvGB2312(wxT("GB2312"));
		return wxString(cvGB2312.cMB2WX(src.c_str()));
	}

	inline string wxString2utf(const wxString& src)
	{
		wxCSConv cvutf(wxFONTENCODING_UTF8);
		return string(cvutf.cWX2MB(src.c_str()));
	}
	inline wxString utf2wxString(const string& src)
	{
		wxCSConv cvutf(wxFONTENCODING_UTF8);
		return wxString(cvutf.cMB2WX(src.c_str()));
	}*/

//自定义命令事件说明
//wxCommandEvent
//String -> Key
//Int -> Type
//ExtraLong -> Long
//ClientData -> Ptr
enum CMD_TYPE 
{
	CMD_TYPE_ZQDB, //Key是Code,ClientData是HZQDB
};

wxDECLARE_EVENT(MY_CMD_GOTO_EVENT, wxCommandEvent);

class ItemBase
{
public:
	virtual const char* Key() { return ""; }
	virtual const char* Name() { return ""; }
	virtual const char* Desc() { return ""; }
};

void Broadcast(wxWindow* parent, const std::function<void(wxWindow* child)>& f, bool deep = false);

