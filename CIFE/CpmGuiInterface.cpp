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

#include "CpmGuiInterface.h"
// --------------------------------------------------------------------------------
#include <wx/textctrl.h>
#include <wx/listctrl.h>
#include <wx/stattext.h>
#include <wx/string.h>
#include <wx/colour.h>
#include <wx/tokenzr.h>
// --------------------------------------------------------------------------------
CpmGuiInterface::CpmGuiInterface(wxListView *listView, wxTextCtrl *textCtrl, wxStaticText *statText) {
    textMessages = textCtrl;
    listContents = listView;
    textDirInfo = statText;
}

// --------------------------------------------------------------------------------
CpmGuiInterface::~CpmGuiInterface() {
}

// --------------------------------------------------------------------------------
void CpmGuiInterface::printMsg(wxString msg, msgColor col) {
    switch (col) {
        case msgColBlack: {
                textMessages->SetForegroundColour(*wxBLACK);
                break;
            }

        case msgColRed: {
                textMessages->SetForegroundColour(*wxRED);
                break;
            }

        case msgColGreen: {
                textMessages->SetForegroundColour(*wxGREEN);
                break;
            }

        case msgColBlue: {
                textMessages->SetForegroundColour(*wxBLUE);
                break;
            }
    }

    textMessages->AppendText(msg);
}

// --------------------------------------------------------------------------------
void CpmGuiInterface::printDirInfo(wxString info) {
    textDirInfo->SetLabel(info);
}

// --------------------------------------------------------------------------------
void CpmGuiInterface::setDirHeader(wxString header) {
    listContents->ClearAll();
    wxStringTokenizer tokenizer(header, ";");
    int charWidth = listContents->GetFont().GetPointSize();

    while (tokenizer.HasMoreTokens()) {
        wxString token = tokenizer.GetNextToken();
        int width = wxAtoi(tokenizer.GetNextToken());
        wxString align = tokenizer.GetNextToken().Upper();
        wxListColumnFormat format = wxLIST_FORMAT_CENTER;

        if (align == "L") {
            format = wxLIST_FORMAT_LEFT;
        }
        else if (align == "R") {
            format = wxLIST_FORMAT_RIGHT;
        }
        else if (align == "C") {
            format = wxLIST_FORMAT_CENTER;
        }

        listContents->AppendColumn(token, format, (width * charWidth));
    }
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
