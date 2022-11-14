/*
 * FileCopySettingsDialog.cpp is part of CIFE
 * Copyright (c) 2022  Uwe Merker
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

#include "FileCopySettingsDialog.hpp"
// --------------------------------------------------------------------------------
FileCopySettingsDialog::FileCopySettingsDialog(wxWindow *parent,
        wxConfigBase *cnf) : Ui_FileCopySettingsDialog(parent) {
    config = cnf;
    spinCtrlDefaultUserNumber->SetValue(
        config->ReadLong("/CpmOptions/DefaultUserNumber", 0));
    wxString textFileEndings;
    config->Read("/CpmOptions/TextfileEndings", &textFileEndings, "txt pip pas");
    editTextFileEndings->SetValue(textFileEndings);
    bool keepLastUpdated;
    config->Read("/CpmOptions/KeepLastUpdatedTimestamp", &keepLastUpdated, false);
    checkboxKeepLastUpdated->SetValue(keepLastUpdated);
    correctDialogSize();
}

// --------------------------------------------------------------------------------
FileCopySettingsDialog::~FileCopySettingsDialog() {
    config->Write("/CopySettingsDialog/PosX", this->GetPosition().x);
    config->Write("/CopySettingsDialog/PosY", this->GetPosition().y);
    config->Write("/CopySettingsDialog/SizeX", this->GetSize().x);
    config->Write("/CopySettingsDialog/SizeY", this->GetSize().y);
    config->Write("/CpmOptions/DefaultUserNumber",
                  spinCtrlDefaultUserNumber->GetValue());

    if (!editTextFileEndings->IsEmpty()) {
        config->Write("/CpmOptions/TextfileEndings", editTextFileEndings->GetValue());
    }

    config->Write("/CpmOptions/KeepLastUpdatedTimestamp",
                  checkboxKeepLastUpdated->GetValue());
}

// --------------------------------------------------------------------------------
void FileCopySettingsDialog::correctDialogSize() {
    this->SetMinSize(wxSize(0, 0));
    int width = this->GetBestSize().GetWidth();
    int height = this->GetBestSize().GetHeight();
    wxSize size;
    size.x = config->ReadLong("/CopySettingsDialog/SizeX", width);
    size.y = config->ReadLong("/CopySettingsDialog/SizeY", (height * 1.5));
    this->SetSize(size);
    this->SetMinSize(wxSize(width, (height * 1.5)));
    wxPoint point;
    point.x = config->ReadLong("/CopySettingsDialog/PosX", 10);
    point.y = config->ReadLong("/CopySettingsDialog/PosY", 10);
    this->SetPosition(point);
}

// --------------------------------------------------------------------------------
