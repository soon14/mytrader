/////////////////////////////////////////////////////////////////////////////
// Name:        mymodels.cpp
// Purpose:     wxDataViewCtrl wxWidgets sample
// Author:      Robert Roebling
// Modified by: Francesco Montorsi, Bo Yang
// Created:     06/01/06
// Copyright:   (c) Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "datamodel.h"
#include "zqstr.hpp"

// ----------------------------------------------------------------------------
// resources
// ----------------------------------------------------------------------------

#include "null.xpm"
#include "wx_small.xpm"

// ----------------------------------------------------------------------------
// MyListModel
// ----------------------------------------------------------------------------

static int my_sort_reverse( int *v1, int *v2 )
{
   return *v2-*v1;
}

static int my_sort( int *v1, int *v2 )
{
   return *v1-*v2;
}

#define INITIAL_NUMBER_OF_ITEMS 10000

MyListModel::MyListModel() :
        wxDataViewVirtualListModel( INITIAL_NUMBER_OF_ITEMS )
{
    // the first 100 items are really stored in this model;
    // all the others are synthesized on request
    static const unsigned NUMBER_REAL_ITEMS = 100;

    m_toggleColValues.reserve(NUMBER_REAL_ITEMS);
    m_textColValues.reserve(NUMBER_REAL_ITEMS);
    m_toggleColValues.push_back(false);
    m_textColValues.push_back("first row with long label to test ellipsization");
    for (unsigned int i = 1; i < NUMBER_REAL_ITEMS; i++)
    {
        m_toggleColValues.push_back(false);
        m_textColValues.push_back(wxString::Format("real row %d", i));
    }

    m_iconColValues.assign(NUMBER_REAL_ITEMS, "test");

    m_icon[0] = wxIcon( null_xpm );
    m_icon[1] = wxIcon( wx_small_xpm );
}

void MyListModel::Prepend( const wxString &text )
{
    m_toggleColValues.insert( m_toggleColValues.begin(), 0 );
    m_textColValues.Insert( text, 0 );
    RowPrepended();
}

void MyListModel::DeleteItem( const wxDataViewItem &item )
{
    unsigned int row = GetRow( item );

    if (row >= m_toggleColValues.size())
        return;

    m_toggleColValues.erase( m_toggleColValues.begin()+row );

    if (row >= m_textColValues.GetCount())
        return;

    m_textColValues.RemoveAt( row );
    RowDeleted( row );
}

void MyListModel::DeleteItems( const wxDataViewItemArray &items )
{
    unsigned i;
    wxArrayInt rows;
    for (i = 0; i < items.GetCount(); i++)
    {
        unsigned int row = GetRow( items[i] );
        if (row < m_textColValues.GetCount())
        {
            wxASSERT(row < m_toggleColValues.size());
            rows.Add( row );
        }
    }

    if (rows.GetCount() == 0)
    {
        // none of the selected items were in the range of the items
        // which we store... for simplicity, don't allow removing them
        wxLogError( "Cannot remove rows with an index greater than %u", unsigned(m_textColValues.GetCount()) );
        return;
    }

    // Sort in descending order so that the last
    // row will be deleted first. Otherwise the
    // remaining indeces would all be wrong.
    rows.Sort( my_sort_reverse );
    for (i = 0; i < rows.GetCount(); i++)
    {
        m_toggleColValues.erase( m_toggleColValues.begin()+rows[i] );
        m_textColValues.RemoveAt( rows[i] );
    }

    // This is just to test if wxDataViewCtrl can
    // cope with removing rows not sorted in
    // descending order
    rows.Sort( my_sort );
    RowsDeleted( rows );
}

void MyListModel::AddMany()
{
    Reset( GetCount()+1000 );
}

void MyListModel::GetValueByRow( wxVariant &variant,
                                 unsigned int row, unsigned int col ) const
{
    switch ( col )
    {
        case Col_Toggle:
            if (row >= m_toggleColValues.size())
                variant = false;
            else
                variant = m_toggleColValues[row];
            break;

        case Col_EditableText:
            if (row >= m_textColValues.GetCount())
                variant = wxString::Format( "virtual row %d", row );
            else
                variant = m_textColValues[ row ];
            break;

        case Col_IconText:
            {
                wxString text;
                if ( row >= m_iconColValues.GetCount() )
                    text = "virtual icon";
                else
                    text = m_iconColValues[row];

                variant << wxDataViewIconText(text, m_icon[row % 2]);
            }
            break;

        case Col_Date:
            variant = wxDateTime(1, wxDateTime::Jan, 2000).Add(wxTimeSpan(row));
            break;

        case Col_TextWithAttr:
            {
                static const char *labels[5] =
                {
                    // These strings will look wrong without wxUSE_MARKUP, but
                    // it's just a sample, so we don't care.
                    "<span color=\"#87ceeb\">light</span> and "
                        "<span color=\"#000080\">dark</span> blue",
                    "<big>growing green</big>",
                    "<i>emphatic &amp; red</i>",
                    "<b>bold &amp;&amp; cyan</b>",
                    "<small><tt>dull default</tt></small>",
                };

                variant = labels[row % 5];
            }
            break;

        case Col_Custom:
            {
                IntToStringMap::const_iterator it = m_customColValues.find(row);
                if ( it != m_customColValues.end() )
                    variant = it->second;
                else
                    variant = wxString::Format("%d", row % 100);
            }
            break;

        case Col_Max:
            wxFAIL_MSG( "invalid column" );
    }
}

bool MyListModel::GetAttrByRow( unsigned int row, unsigned int col,
                                wxDataViewItemAttr &attr ) const
{
    switch ( col )
    {
        case Col_Toggle:
            return false;

        case Col_EditableText:
        case Col_Date:
            if (row < m_toggleColValues.size())
            {
                if (m_toggleColValues[row])
                {
                    attr.SetColour( wxColour( *wxLIGHT_GREY ) );
                    attr.SetStrikethrough( true );
                    return true;
                }
            }
            return false;

        case Col_IconText:
            if ( !(row % 2) )
                return false;
            attr.SetColour(*wxYELLOW);
            attr.SetBackgroundColour(*wxLIGHT_GREY);
            break;

        case Col_TextWithAttr:
            if (row < m_toggleColValues.size())
            {
                if (m_toggleColValues[row])
                {
                    attr.SetColour( wxColour( *wxLIGHT_GREY ) );
                    attr.SetStrikethrough( true );
                    return true;
                }
            }
            wxFALLTHROUGH;

        case Col_Custom:
            // do what the labels defined in GetValueByRow() hint at
            switch ( row % 5 )
            {
                case 0:
                    attr.SetColour(*wxBLUE);
                    break;

                case 1:
                    attr.SetColour(*wxGREEN);
                    break;

                case 2:
                    attr.SetColour(*wxRED);
                    break;

                case 3:
                    attr.SetColour(*wxCYAN);
                    attr.SetBold(true);
                    break;

                case 4:
                    return false;
            }
            break;

        case Col_Max:
            wxFAIL_MSG( "invalid column" );
    }

    return true;
}

bool MyListModel::SetValueByRow( const wxVariant &variant,
                                 unsigned int row, unsigned int col )
{
    switch ( col )
    {
        case Col_Toggle:
            if (row >= m_toggleColValues.size())
                return false;

            m_toggleColValues[row] = variant.GetBool();
            return true;

        case Col_EditableText:
        case Col_IconText:
            if (row >= m_textColValues.GetCount())
            {
                // the item is not in the range of the items
                // which we store... for simplicity, don't allow editing it
                wxLogError( "Cannot edit rows with an index greater than %d",
                            m_textColValues.GetCount() );
                return false;
            }

            if ( col == Col_EditableText )
            {
                m_textColValues[row] = variant.GetString();
            }
            else // col == Col_IconText
            {
                wxDataViewIconText iconText;
                iconText << variant;
                m_iconColValues[row] = iconText.GetText();
            }
            return true;

        case Col_Date:
        case Col_TextWithAttr:
            wxLogError("Cannot edit the column %d", col);
            break;

        case Col_Custom:
            m_customColValues[row] = variant.GetString();
            break;

        case Col_Max:
            wxFAIL_MSG( "invalid column" );
    }

    return false;
}

// ----------------------------------------------------------------------------
// SmartKBListModel
// ----------------------------------------------------------------------------

wxDEFINE_EVENT(SMARTKB_SEARCH_RESULT_EVENT, wxCommandEvent);

SmartKBListModel::SmartKBListModel() :
	wxDataViewVirtualListModel()
{
	zqdb::AllExchange allexchange;
	for (auto h : allexchange)
	{
		zqdb::Exchange exchange(h);
		auto info = exchange.GetValue();
		SmartKBItem one;
		one.Code = zqdb::utf8_to_unicode(info->Exchange);
		one.Name = zqdb::utf8_to_unicode(info->Name);
		one.Type = 0;
		one.Data = h;
		Add("", &one, 1);
	}
	zqdb::AllProduct allproduct;
	for (auto h : allproduct)
	{
		zqdb::Product product(h);
		auto info = product.GetValue();
		SmartKBItem one;
		one.Code = zqdb::utf8_to_unicode(info->Product);
		one.Name = zqdb::utf8_to_unicode(info->Name);
		one.Type = 0;
		one.Data = h;
		Add("", &one, 1);
	}
	zqdb::AllCode allcode;
	for (auto h : allcode)
	{
		zqdb::Code code(h);
		auto info = code.GetValue();
		SmartKBItem one;
		one.Code = zqdb::utf8_to_unicode(info->Code);
		one.Name = zqdb::utf8_to_unicode(info->Name);
		one.Type = 0;
		one.Data = h;
		Add("", &one, 1);
	}
	Select("");
}

bool SmartKBListModel::Search(wxEvtHandler* notify, const wxString& strKey, int flag)
{
	notify_ = notify;
	return Input(strKey.c_str(), flag);
}

void SmartKBListModel::NotifyResult()
{
	notify_->QueueEvent(new wxCommandEvent(SMARTKB_SEARCH_RESULT_EVENT));
}

void SmartKBListModel::ShowResult()
{
	results_.clear();
	GetInputResults(results_);
	Reset(results_.size());
}

bool SmartKBListModel::SmartKBListModel::GetResult(const wxDataViewItem& item, SmartKBItem& smkbi)
{
	auto row = GetRow(item);
	if (row >= 0 && row < results_.size()) {
		smkbi = results_[row];
		return true;
	}
	return false;
}

void SmartKBListModel::GetValueByRow(wxVariant &variant,
	unsigned int row, unsigned int col) const
{
	switch (col)
	{
	case Col_Code:
	{
		variant = results_[row].Code;
	}
	break;

	case Col_Name:
		variant = results_[row].Name;
		break;

	case Col_Desc:
	{
		static const char *labels[5] =
		{
			// These strings will look wrong without wxUSE_MARKUP, but
			// it's just a sample, so we don't care.
			"<span color=\"#87ceeb\">light</span> and "
			"<span color=\"#000080\">dark</span> blue",
			"<big>growing green</big>",
			"<i>emphatic &amp; red</i>",
			"<b>bold &amp;&amp; cyan</b>",
			"<small><tt>dull default</tt></small>",
		};

		variant = labels[row % 5];
	}
	break;
	case Col_Max:
		wxFAIL_MSG("invalid column");
	}
}

bool SmartKBListModel::GetAttrByRow(unsigned int row, unsigned int col,
	wxDataViewItemAttr &attr) const
{
	switch (col)
	{
	case Col_Code:
		if (!(row % 2))
			return false;
		attr.SetColour(wxColour(*wxLIGHT_GREY));
		attr.SetStrikethrough(true);
		return true;
	case Col_Name:
		if (!(row % 2))
			return false;
		attr.SetColour(*wxYELLOW);
		attr.SetBackgroundColour(*wxLIGHT_GREY);
		return true;
		break;
	case Col_Desc:
		if (!(row % 2))
			return false;
		attr.SetColour(wxColour(*wxLIGHT_GREY));
		attr.SetStrikethrough(true);
		return true;
		break;
	case Col_Max:
		wxFAIL_MSG("invalid column");
	}
	return true;
}

bool SmartKBListModel::SetValueByRow(const wxVariant &variant,
	unsigned int row, unsigned int col)
{
	switch (col)
	{
	case Col_Code:
		return true;
	case Col_Name:
		return true;
		break;
	case Col_Desc:
		return true;
		break;
	case Col_Max:
		break;
	}

	return false;
}


// ----------------------------------------------------------------------------
// MyCodeListModel
// ----------------------------------------------------------------------------

MyCodeListModel::MyCodeListModel() :
	wxDataViewVirtualListModel()
{
	wxCSConv utf8cv(wxFONTENCODING_UTF8);
	zqdb::AllCode allcode;
	code_items_.reserve(allcode.size());
	for (auto h : allcode)
	{
		zqdb::Code code(h);
		auto info = code.GetValue();
		CodeItem one;
		one.h = h;
		one.Code = utf8cv.cMB2WX(info->Code).data();
		one.Name = utf8cv.cMB2WX(info->Name).data();
		code_items_.push_back(one);
	}
}

void MyCodeListModel::Show()
{
	Reset(code_items_.size());
}

HZQDB MyCodeListModel::GetData(const wxDataViewItem& item)
{
	auto row = GetRow(item);
	if (row >= 0 && row < code_items_.size()) {
		return code_items_[row].h;
	}
	return nullptr;
}

void MyCodeListModel::GetValueByRow(wxVariant &variant,
	unsigned int row, unsigned int col) const
{
	switch (col)
	{
	case Col_Code:
	{
		variant = code_items_[row].Code;
	}
	break;

	case Col_Name:
		variant = code_items_[row].Name;
		break;

	case Col_Desc:
	{
		static const char *labels[5] =
		{
			// These strings will look wrong without wxUSE_MARKUP, but
			// it's just a sample, so we don't care.
			"<span color=\"#87ceeb\">light</span> and "
			"<span color=\"#000080\">dark</span> blue",
			"<big>growing green</big>",
			"<i>emphatic &amp; red</i>",
			"<b>bold &amp;&amp; cyan</b>",
			"<small><tt>dull default</tt></small>",
		};

		variant = labels[row % 5];
	}
	break;
	case Col_Max:
		wxFAIL_MSG("invalid column");
	}
}

bool MyCodeListModel::GetAttrByRow(unsigned int row, unsigned int col,
	wxDataViewItemAttr &attr) const
{
	switch (col)
	{
	case Col_Code:
		if (!(row % 2))
			return false;
		attr.SetColour(wxColour(*wxLIGHT_GREY));
		attr.SetStrikethrough(true);
		return true;
	case Col_Name:
		if (!(row % 2))
			return false;
		attr.SetColour(*wxYELLOW);
		attr.SetBackgroundColour(*wxLIGHT_GREY);
		return true;
		break;
	case Col_Desc:
		if (!(row % 2))
			return false;
		attr.SetColour(wxColour(*wxLIGHT_GREY));
		attr.SetStrikethrough(true);
		return true;
		break;
	case Col_Max:
		wxFAIL_MSG("invalid column");
	}
	return true;
}

bool MyCodeListModel::SetValueByRow(const wxVariant &variant,
	unsigned int row, unsigned int col)
{
	switch (col)
	{
	case Col_Code:
		return true;
	case Col_Name:
		return true;
		break;
	case Col_Desc:
		return true;
		break;
	case Col_Max:
		break;
	}

	return false;
}

