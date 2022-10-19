/*
 * CpmGuiInterface.hpp is part of CIFE
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

#ifndef CPMGUIINTERFACE_HPP
#define CPMGUIINTERFACE_HPP
// --------------------------------------------------------------------------------
class wxListView;
class wxTextCtrl;
// --------------------------------------------------------------------------------
#include <wx/string.h>
// --------------------------------------------------------------------------------
class CpmGuiInterface {
    public:     // Attributes
        enum msgColor {msgColRed, msgColGreen, msgColBlue, msgColBlack};

    public:     // Methods
        void printMsg(wxString msg, msgColor col = msgColBlack);
        void printDirInfo(wxString info);
        void setDirHeader();
        void printDirEntry(int col, int row, wxString data);
        bool askBox(const wxString Message, const wxString title);

    public:     // Constructor & Destructor
        CpmGuiInterface(wxListView *listView, wxTextCtrl *textCtrl, wxTextCtrl *statText);
        ~CpmGuiInterface();

    protected:  // Event Methods

    private:    // Attributes
        wxTextCtrl *textMessages;
        wxListView *listContents;
        wxTextCtrl *textDirInfo;

    private:    // Methods

};

// --------------------------------------------------------------------------------
#endif // CPMGUIINTERFACE_HPP
