/***********************************************************************************
**
**	Copyright (C) 19.08.2021 Uwe Merker Germany
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

#include "CpmGuiInterface.hpp"

#include <wx/textctrl.h>
#include <wx/listctrl.h>
#include <wx/string.h>
#include <wx/colour.h>
#include <wx/msgdlg.h>
// --------------------------------------------------------------------------------
CpmGuiInterface::CpmGuiInterface(wxListView *listView, wxTextCtrl *textCtrl, wxTextCtrl *statText) {
    textMessages = textCtrl;
    listContents = listView;
    textDirInfo = statText;
    setDirHeader();
}

// --------------------------------------------------------------------------------
CpmGuiInterface::~CpmGuiInterface() {
}

// --------------------------------------------------------------------------------
void CpmGuiInterface::printMsg(wxString msg, msgColor col) {
    switch (col) {
        case msgColBlack: {
                textMessages->SetDefaultStyle(wxTextAttr(*wxBLACK));
                break;
            }

        case msgColRed: {
                textMessages->SetDefaultStyle(wxTextAttr(*wxRED));
                break;
            }

        case msgColGreen: {
                textMessages->SetDefaultStyle(wxTextAttr(0x00B000));
                break;
            }

        case msgColBlue: {
                textMessages->SetDefaultStyle(wxTextAttr(*wxBLUE));
                break;
            }
    }

    textMessages->AppendText(msg);
}

// --------------------------------------------------------------------------------
void CpmGuiInterface::printDirInfo(wxString info) {
    textDirInfo->Clear();
    textDirInfo->AppendText(info);
}

// --------------------------------------------------------------------------------
void CpmGuiInterface::setDirHeader() {
    listContents->ClearAll();
    int charWidth = listContents->GetFont().GetPointSize();
    listContents->AppendColumn("User: Name", wxLIST_FORMAT_LEFT, (14 * charWidth));
    listContents->AppendColumn("Bytes", wxLIST_FORMAT_RIGHT, (7 * charWidth));
    listContents->AppendColumn("Recs", wxLIST_FORMAT_RIGHT, (7 * charWidth));
    listContents->AppendColumn("Attributes", wxLIST_FORMAT_CENTER, (10 * charWidth));
    listContents->AppendColumn("Protections", wxLIST_FORMAT_CENTER, (10 * charWidth));
    listContents->AppendColumn("Updated", wxLIST_FORMAT_CENTER, (15 * charWidth));
    listContents->AppendColumn("Created", wxLIST_FORMAT_CENTER, (15 * charWidth));
    listContents->AppendColumn("Last Access", wxLIST_FORMAT_CENTER, (15 * charWidth));
}

// --------------------------------------------------------------------------------
void CpmGuiInterface::printDirEntry(int col, int row, wxString data) {
    if (col == 0) {
        listContents->InsertItem(row, data);
    }
    else {
        listContents->SetItem(row, col, data);
    }
}

// --------------------------------------------------------------------------------
bool CpmGuiInterface::askBox(const wxString Message, const wxString title) {
#ifdef _WINDOWS_
    wxMessageDialog deleteDialog(NULL, Message + " ?", title, wxYES_NO | wxYES_DEFAULT | wxICON_QUESTION);
#else
    wxMessageDialog deleteDialog(NULL, "\n" + Message + " ?", title, wxYES_NO | wxYES_DEFAULT | wxICON_QUESTION);
#endif

    if (deleteDialog.ShowModal() == wxID_YES) {
        return (true);
    }

    return (false);
}

// --------------------------------------------------------------------------------
