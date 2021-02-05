/////////////////////////////////////////////////////////////////////////////
// Name:        mymodels.h
// Purpose:     wxDataViewCtrl wxWidgets sample
// Author:      Robert Roebling
// Modified by: Francesco Montorsi, Bo Yang
// Created:     06/01/06
// Copyright:   (c) Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#pragma once

#include "myapp.h"
#include "smartkb.h"

#include "wx/dataview.h"
#include "wx/hashmap.h"
#include <wx/vector.h>

WX_DECLARE_HASH_MAP(unsigned, wxString, wxIntegerHash, wxIntegerEqual,
                    IntToStringMap);

// ----------------------------------------------------------------------------
// MyListModel
// ----------------------------------------------------------------------------

class MyListModel: public wxDataViewVirtualListModel
{
public:
    enum
    {
        Col_Toggle,
        Col_EditableText,
        Col_IconText,
        Col_Date,
        Col_TextWithAttr,
        Col_Custom,
        Col_Max
    };

    MyListModel();

    // helper methods to change the model

    void Prepend( const wxString &text );
    void DeleteItem( const wxDataViewItem &item );
    void DeleteItems( const wxDataViewItemArray &items );
    void AddMany();


    // implementation of base class virtuals to define model

    virtual unsigned int GetColumnCount() const wxOVERRIDE
    {
        return Col_Max;
    }

    virtual wxString GetColumnType( unsigned int col ) const wxOVERRIDE
    {
        if (col == Col_Toggle)
            return "bool";

        if (col == Col_IconText)
            return "wxDataViewIconText";

        return "string";
    }

    virtual void GetValueByRow( wxVariant &variant,
                                unsigned int row, unsigned int col ) const wxOVERRIDE;
    virtual bool GetAttrByRow( unsigned int row, unsigned int col,
                               wxDataViewItemAttr &attr ) const wxOVERRIDE;
    virtual bool SetValueByRow( const wxVariant &variant,
                                unsigned int row, unsigned int col ) wxOVERRIDE;

private:
    wxVector<bool>   m_toggleColValues;
    wxArrayString    m_textColValues;
    wxArrayString    m_iconColValues;
    IntToStringMap   m_customColValues;
    wxIcon           m_icon[2];
};

// ----------------------------------------------------------------------------
// SmartKBListModel
// ----------------------------------------------------------------------------

wxDECLARE_EVENT(SMARTKB_SEARCH_RESULT_EVENT, wxCommandEvent);

class SmartKBListModel : public wxDataViewVirtualListModel, public SmartKB
{
private:
	std::string type_ = ""; //默认全局SmartKB
	wxEvtHandler* notify_ = nullptr;
	std::vector<SmartKBItem> results_;
public:
	enum
	{
		Col_Code,
		Col_Name,
		Col_Desc,
		Col_Max
	};

	SmartKBListModel();

	//更新所有数据列表
	void ClearAll();
	void UpdateAll();
	//搜索
	bool Search(wxEvtHandler* notify, const wxString& strKey, int flag = 0);
	//通知搜索结束
	virtual void NotifyResult();
	//显示结果
	virtual void ShowResult();
	//获取数据
	bool GetResult(const wxDataViewItem& item, SmartKBItem& smkbi);

	// implementation of base class virtuals to define model

	virtual unsigned int GetColumnCount() const wxOVERRIDE
	{
		return Col_Max;
	}

	virtual wxString GetColumnType(unsigned int col) const wxOVERRIDE
	{
		/*if (col == Col_Toggle)
			return "bool";

		if (col == Col_IconText)
			return "wxDataViewIconText";*/

		return "string";
	}

	virtual void GetValueByRow(wxVariant &variant,
		unsigned int row, unsigned int col) const wxOVERRIDE;
	virtual bool GetAttrByRow(unsigned int row, unsigned int col,
		wxDataViewItemAttr &attr) const wxOVERRIDE;
	virtual bool SetValueByRow(const wxVariant &variant,
		unsigned int row, unsigned int col) wxOVERRIDE;
};

///
//可以显示代码行情信息
///

class MyCodeListModel : public wxDataViewVirtualListModel
{
public:
	enum
	{
		Col_Code,
		Col_Name,
		Col_Desc,
		Col_Max
	};
private:
	std::vector<HZQDB> code_items_;
public:
	MyCodeListModel();

	void Show(const std::vector<HZQDB>& h_list);
	HZQDB GetData(const wxDataViewItem& item);

	// implementation of base class virtuals to define model

	virtual unsigned int GetColumnCount() const wxOVERRIDE
	{
		return Col_Max;
	}

	virtual wxString GetColumnType(unsigned int col) const wxOVERRIDE
	{
		if (col == Col_Code)
		return "void*";

		if (col == Col_Name)
		return "void*";

		return "string";
	}

	virtual void GetValueByRow(wxVariant &variant,
		unsigned int row, unsigned int col) const wxOVERRIDE;
	virtual bool GetAttrByRow(unsigned int row, unsigned int col,
		wxDataViewItemAttr &attr) const wxOVERRIDE;
	virtual bool SetValueByRow(const wxVariant &variant,
		unsigned int row, unsigned int col) wxOVERRIDE;
};

///
//可以根据XML配置显示信息
///

class MyHZQDBListModel : public wxDataViewVirtualListModel
{
public:
	struct ColInfo {
		MDB_FIELD field;
		int precision;
		wxString name;
		ColInfo() :field{ 0 }, precision(0), name(){}
	};
private:
	std::map<std::string,std::vector<ColInfo>> all_col_items_;
	std::vector<ColInfo> col_items_;
	std::vector<HZQDB> val_items_;
public:
	MyHZQDBListModel(const char* xml = nullptr, size_t xmlflag = XUtil::XML_FLAG_JSON_STRING);

	void Select(HZQDB user, ZQDB_HANDLE_TYPE type);
	const std::vector<ColInfo>& GetColInfo() { return col_items_; }
	void Clear();
	void Show(const std::vector<HZQDB>& h_list);
	HZQDB GetData(const wxDataViewItem& item);

	// implementation of base class virtuals to define model

	virtual unsigned int GetColumnCount() const wxOVERRIDE
	{
		return col_items_.size();
	}

	virtual wxString GetColumnType(unsigned int col) const wxOVERRIDE
	{
		return "string";
	}

	virtual void GetValueByRow(wxVariant &variant,
		unsigned int row, unsigned int col) const wxOVERRIDE;
	virtual bool GetAttrByRow(unsigned int row, unsigned int col,
		wxDataViewItemAttr &attr) const wxOVERRIDE;
	virtual bool SetValueByRow(const wxVariant &variant,
		unsigned int row, unsigned int col) wxOVERRIDE;
};
