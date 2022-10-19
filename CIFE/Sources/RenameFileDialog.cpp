/*
 * RenameFileDialog.cpp is part of CIFE
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

#include "RenameFileDialog.hpp"

#include <wx/dcscreen.h>
#include <wx/gdicmn.h>
// --------------------------------------------------------------------------------
BEGIN_EVENT_TABLE(RenameFileDialog, wxDialog)
    EVT_CHECKBOX(wxID_CHANGE_USER, RenameFileDialog::onChangeUserClicked)
END_EVENT_TABLE()
// --------------------------------------------------------------------------------
RenameFileDialog::RenameFileDialog(wxWindow *parent) : Ui_RenameFileDialog(parent) {
    wxScreenDC dc;
    dc.SetFont(this->GetFont());
    int height = editOldUser->GetSize().GetHeight();
    int userWidth = dc.GetTextExtent("####").GetWidth();
    editOldUser->SetMinSize(wxSize(userWidth, height));
    editOldUser->SetMaxSize(wxSize(userWidth, height));
    editNewUser->SetMinSize(wxSize(userWidth, height));
    editNewUser->SetMaxSize(wxSize(userWidth, height));
    int nameWidth = dc.GetTextExtent("##############").GetWidth();
    editOldFileName->SetMinSize(wxSize(nameWidth, height));
    editOldFileName->SetMaxSize(wxSize(nameWidth, height));
    editNewFileName->SetMinSize(wxSize(nameWidth, height));
    editNewFileName->SetMaxSize(wxSize(nameWidth, height));
    editNewFileName->SetFocus();
    this->SetMinSize(wxSize(0, 0));
    this->SetSize(this->GetBestSize());
    this->SetMinSize(this->GetBestSize());
}

// --------------------------------------------------------------------------------
RenameFileDialog::~RenameFileDialog() {
}

// --------------------------------------------------------------------------------
void RenameFileDialog::setOldUser(int user) {
    editOldUser->SetValue(wxString::Format("%i", user));
    editNewUser->SetValue(wxString::Format("%i", user));
}

// --------------------------------------------------------------------------------
void RenameFileDialog::setOldName(wxString name) {
    editOldFileName->SetValue(name);
    editNewFileName->SetValue(name);
}

// --------------------------------------------------------------------------------
wxString RenameFileDialog::getNewName() {
    return (editNewFileName->GetValue());
}

// --------------------------------------------------------------------------------
int RenameFileDialog::getNewUser() {
    return (wxAtoi(editNewUser->GetValue()));
}

// --------------------------------------------------------------------------------
void RenameFileDialog::onChangeUserClicked(wxCommandEvent &event) {
    bool value = checkboxChangeUser->GetValue();
    editNewUser->Enable(value);

    if (!value) {
        editNewUser->SetValue(editOldUser->GetValue());
        editNewFileName->SetFocus();
    }
    else {
        editNewUser->SetFocus();
    }
}

// --------------------------------------------------------------------------------
