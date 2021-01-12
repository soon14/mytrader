/////////////////////////////////////////////////////////////////////////////
// Name:        dataview.cpp
// Purpose:     wxDataViewCtrl wxWidgets sample
// Author:      Robert Roebling
// Modified by: Francesco Montorsi, Bo Yang
// Created:     06/01/06
// Copyright:   (c) Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "mydataview.h"

// a simple renderer that wraps each word on a new line
MyCodeListCodeRenderer::MyCodeListCodeRenderer()
	: wxDataViewCustomRenderer("void*", wxDATAVIEW_CELL_INERT, 0)
{ 
}

bool MyCodeListCodeRenderer::Render(wxRect rect, wxDC *dc, int state)
{
	//Name
	dc->SetFont(skin_info_ptr_->fontName);
	dc->SetTextForeground(skin_info_ptr_->crCtrlForgnd);
	dc->DrawLabel(code_, rect, wxALIGN_LEFT | wxALIGN_TOP);
	//Code
	dc->SetFont(skin_info_ptr_->fontTechTitle);
	dc->SetTextForeground(skin_info_ptr_->crCtrlForgnd);
	dc->DrawLabel(name_, rect, wxALIGN_LEFT | wxALIGN_BOTTOM);
	//RenderText(m_value, 0, rect, dc, state);
	return true;
}

wxSize MyCodeListCodeRenderer::GetSize() const
{
	wxSize txtSize;
	txtSize.x = name_.size() * skin_info_ptr_->xyName.x;
	txtSize.y = skin_info_ptr_->xyName.y + skin_info_ptr_->xyTechTitle.y;
	return txtSize;
}

bool MyCodeListCodeRenderer::SetValue(const wxVariant &value)
{
	wxCSConv utf8cv(wxFONTENCODING_UTF8);

	h_ = (HZQDB)value.GetVoidPtr();
	auto info = (CODEINFO*)ZQDBGetValue(h_);
	name_ = utf8cv.cMB2WX(info->Name);//value.GetString();
	code_ = utf8cv.cMB2WX(info->Code);
	return true;
}

bool MyCodeListCodeRenderer::GetValue(wxVariant &WXUNUSED(value)) const 
{ 
	return true; 
}

#if wxUSE_ACCESSIBILITY
wxString MyCodeListCodeRenderer::GetAccessibleDescription() const
{
	return name_;
}
#endif // wxUSE_ACCESSIBILITY

///

MyCodeListNameRenderer::MyCodeListNameRenderer()
	: wxDataViewCustomRenderer("void*", wxDATAVIEW_CELL_INERT, 0)
{ 

}

bool MyCodeListNameRenderer::Render(wxRect rect, wxDC *dc, int state)
{
	if (!info_ || IsInvalidValue(info_->Close) || IsInvalidValue(info_->YClose)) {
		return false;
	}
	//
	//Price
	//change zdf
	//
	double price = info_->Close;
	double change = info_->Close - info_->YClose;
	double zdf = change / info_->YClose * 100;
	//Price
	wxString strPrice = wxString::Format("%.2f", price);
	dc->SetFont(skin_info_ptr_->fontName);
	dc->SetTextForeground(skin_info_ptr_->GetColor(change));
	dc->DrawLabel(strPrice, rect, wxALIGN_LEFT | wxALIGN_TOP);
	//change
	wxString strChange = wxString::Format("%.2f", change);
	dc->SetFont(skin_info_ptr_->fontTechTitle);
	dc->SetTextForeground(skin_info_ptr_->GetColor(change));
	dc->DrawLabel(strChange, rect, wxALIGN_LEFT | wxALIGN_BOTTOM);
	//zdf
	wxString strZDF = wxString::Format("%.2f", change);
	dc->SetFont(skin_info_ptr_->fontTechTitle);
	dc->SetTextForeground(skin_info_ptr_->GetColor(change));
	dc->DrawLabel(strZDF, rect, wxALIGN_RIGHT | wxALIGN_BOTTOM);
	//RenderText(m_value, 0, rect, dc, state);
	return true;
}

wxSize MyCodeListNameRenderer::GetSize() const
{
	wxSize txtSize;
	txtSize.x = -1;
	txtSize.y = skin_info_ptr_->xyName.y + skin_info_ptr_->xyTechTitle.y;
	return txtSize;
}

bool MyCodeListNameRenderer::SetValue(const wxVariant &value)
{
	wxCSConv utf8cv(wxFONTENCODING_UTF8);

	h_ = (HZQDB)value.GetVoidPtr();
	info_ = (CODEINFO*)ZQDBGetValue(h_);
	return true;
}

bool MyCodeListNameRenderer::GetValue(wxVariant &WXUNUSED(value)) const 
{ 
	return true;
}

#if wxUSE_ACCESSIBILITY
wxString MyCodeListNameRenderer::GetAccessibleDescription() const
{
	return "";
}
#endif // wxUSE_ACCESSIBILITY
