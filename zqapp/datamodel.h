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

#include "app.h"
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
	class CodeItem
	{
	public:
		HZQDB h;
		wxString Code;
		wxString Name;
	};
private:
	std::vector<CodeItem> code_items_;
public:
	MyCodeListModel();

	void Show();
	HZQDB GetData(const wxDataViewItem& item);

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
