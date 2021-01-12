#include "utility.h"

wxDEFINE_EVENT(MY_CMD_GOTO_EVENT, wxCommandEvent);


void Broadcast(wxWindow* parent, const std::function<void(wxWindow* child)>& f, bool deep)
{
	if (parent) {
		wxWindowList children = parent->GetChildren();
		for (wxWindowListNode *i = children.GetFirst(); i; i = i->GetNext()) {
			auto child = i->GetData();
			if (child) {
				if (deep) {
					Broadcast(child, f, deep);
				}
				f(child);
			}
		}
	}
}


#include "wx/html/htmlwin.h"


// ----------------------------------------------------------------------------
// TAG HANDER FOR 'MYBIND' TAG
// ----------------------------------------------------------------------------

#include "wx/html/m_templ.h"

TAG_HANDLER_BEGIN(MYBIND, "MYBIND")

TAG_HANDLER_PROC(tag)
{
	wxWindow *wnd;
	int ax, ay;
	int fl = 0;

	tag.ScanParam("X", "%i", &ax);
	tag.ScanParam("Y", "%i", &ay);

	if (tag.HasParam("FLOAT")) fl = ax;

	wnd = new wxTextCtrl
	(
		m_WParser->GetWindowInterface()->GetHTMLWindow(),
		wxID_ANY,
		tag.GetParam("NAME"),
		wxPoint(0, 0),
		wxSize(ax, ay),
		wxTE_MULTILINE
	);

	wnd->Show(true);

	m_WParser->GetContainer()->InsertCell(new wxHtmlWidgetCell(wnd, fl));

	return false;
}

TAG_HANDLER_END(MYBIND)



TAGS_MODULE_BEGIN(MyBind)

TAGS_MODULE_ADD(MYBIND)

TAGS_MODULE_END(MyBind)



