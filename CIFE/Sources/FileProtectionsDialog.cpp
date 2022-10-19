/*
 * FileProtectionsDialog.cpp is part of CIFE
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

#include "FileProtectionsDialog.hpp"

#include <wx/tokenzr.h>
// --------------------------------------------------------------------------------
FileProtectionsDialog::FileProtectionsDialog(wxWindow *parent) : Ui_FileProtectionsDialog(parent) {
}

// --------------------------------------------------------------------------------
FileProtectionsDialog::~FileProtectionsDialog() {
}

// --------------------------------------------------------------------------------
void FileProtectionsDialog::setProtections(wxString protections) {
    wxStringTokenizer tokenizer(protections, " ");

    while (tokenizer.HasMoreTokens()) {
        wxString token = tokenizer.GetNextToken();

        if (token == "rd") {
            checkboxReadProtect->SetValue(true);
        }
        else if (token == "wr") {
            checkboxWriteProtect->SetValue(true);
        }
        else if (token == "del") {
            checkboxDeleteProtect->SetValue(true);
        }
    }
}

// --------------------------------------------------------------------------------
int FileProtectionsDialog::getProtections() {
    int protections = 0;

    if (checkboxReadProtect->GetValue()) {
        protections |= CPM_ATTR_PWREAD;
    }

    if (checkboxWriteProtect->GetValue()) {
        protections |= CPM_ATTR_PWWRITE;
    }

    if (checkboxDeleteProtect->GetValue()) {
        protections |= CPM_ATTR_PWDEL;
    }

    return (protections);
}

// --------------------------------------------------------------------------------
