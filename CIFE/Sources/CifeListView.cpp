/***********************************************************************************
**
**	Copyright (C) 08.06.2022 Uwe Merker Germany
**
**	Diese Datei ist Teil von CPM_Image-File_Explorer.
**
**	CPM_Image-File_Explorer ist Freie Software: Sie können es unter den Bedingungen
**	der GNU General Public License, wie von der Free Software Foundation,
**	Version 3 der Lizenz oder (nach Ihrer Wahl) jeder späteren
**	veröffentlichten Version, weiterverbreiten und/oder modifizieren.
**
**	CPM_Image-File_Explorer wird in der Hoffnung, dass es nützlich sein wird, aber
**	OHNE JEDE GEWÄHRLEISTUNG, bereitgestellt; sogar ohne die implizite
**	Gewährleistung der MARKTFÄHIGKEIT oder EIGNUNG FÜR EINEN BESTIMMTEN ZWECK.
**	Siehe die GNU General Public License für weitere Details.
**
**	Sie sollten eine Kopie der GNU General Public License zusammen mit diesem
**	Programm erhalten haben. Wenn nicht, siehe <http://www.gnu.org/licenses/>.
**
***********************************************************************************/

#include "CifeListView.hpp"
// --------------------------------------------------------------------------------
BEGIN_EVENT_TABLE(CifeListView, wxListView)
    EVT_SIZE(CifeListView::OnSize)
END_EVENT_TABLE()
// --------------------------------------------------------------------------------
CifeListView::CifeListView(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, long style) : wxListView(parent, id, pos, size, style) {
    this->SetBackgroundColour(wxColour(wxT("rgb(203,230,162)")));
    sizingEnabled = false;
}

// --------------------------------------------------------------------------------
CifeListView::~CifeListView() {
}

// --------------------------------------------------------------------------------
void CifeListView::OnSize(wxSizeEvent &event) {
    if (sizingEnabled) {
        int newWidth, colwidths = 0;
        int viewWidth = this->GetClientSize().GetWidth();
        colwidths += newWidth = (viewWidth * 0.151);
        this->SetColumnWidth(0, newWidth);
        colwidths += newWidth = (viewWidth * 0.076);
        this->SetColumnWidth(1, newWidth);
        colwidths += newWidth = (viewWidth * 0.076);
        this->SetColumnWidth(2, newWidth);
        colwidths += newWidth = (viewWidth * 0.107);
        this->SetColumnWidth(3, newWidth);
        colwidths += newWidth = (viewWidth * 0.107);
        this->SetColumnWidth(4, newWidth);
        colwidths += newWidth = (viewWidth * 0.161);
        this->SetColumnWidth(5, newWidth);
        colwidths += newWidth = (viewWidth * 0.161);
        this->SetColumnWidth(6, newWidth);
        colwidths += newWidth = (viewWidth * 0.161);
        this->SetColumnWidth(7, (newWidth + (viewWidth - colwidths)));
        event.Skip();
    }
}

// --------------------------------------------------------------------------------
void CifeListView::enableSizing(bool enable) {
    sizingEnabled = enable;
}

// --------------------------------------------------------------------------------
