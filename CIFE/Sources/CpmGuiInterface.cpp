/*
 * CpmGuiInterface.cpp is part of CIFE
 * Copyright (c) 2021  Uwe Merker
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "CpmGuiInterface.hpp"
// --------------------------------------------------------------------------------
#include <wx/textctrl.h>
#include <wx/listctrl.h>
#include <wx/colour.h>
#include <wx/msgdlg.h>
// --------------------------------------------------------------------------------
CpmGuiInterface::CpmGuiInterface(wxListView *listView, wxTextCtrl *textCtrl,
                                 wxTextCtrl *statText) {
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
    wxMessageDialog deleteDialog(NULL, Message + " ?", title,
                                 wxYES_NO | wxYES_DEFAULT | wxICON_QUESTION);
#else
    wxMessageDialog deleteDialog(NULL, "\n" + Message + " ?", title,
                                 wxYES_NO | wxYES_DEFAULT | wxICON_QUESTION);
#endif

    if (deleteDialog.ShowModal() == wxID_YES) {
        return (true);
    }

    return (false);
}

// --------------------------------------------------------------------------------
