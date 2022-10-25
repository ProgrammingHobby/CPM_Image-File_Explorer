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
#include "Settings.hpp"
// --------------------------------------------------------------------------------
FileCopySettingsDialog::FileCopySettingsDialog(wxWindow *parent,
        Settings *settings) : Ui_FileCopySettingsDialog(parent) {
    dialogSettings = settings;

    spinCtrlDefaultUserNumber->SetValue(dialogSettings->readInteger("CpmOptions",
                                        "DefaultUserNumber", 0));
    editTextFileEndings->SetValue(dialogSettings->readString("CpmOptions", "TextfileEndings",
                                  "txt pip pas"));
    checkboxKeepLastUpdated->SetValue(dialogSettings->readBoolean("CpmOptions",
                                      "KeepLastUpdatedTimestamp", false));

    correctDialogSize();

}

// --------------------------------------------------------------------------------
FileCopySettingsDialog::~FileCopySettingsDialog() {
    dialogSettings->writeInteger("CopySettingsDialog", "PosX", this->GetPosition().x);
    dialogSettings->writeInteger("CopySettingsDialog", "PosY", this->GetPosition().y);
    dialogSettings->writeInteger("CopySettingsDialog", "SizeX", this->GetSize().x);
    dialogSettings->writeInteger("CopySettingsDialog", "SizeY", this->GetSize().y);

    dialogSettings->writeInteger("CpmOptions", "DefaultUserNumber",
                                 spinCtrlDefaultUserNumber->GetValue());
    dialogSettings->writeString("CpmOptions", "TextfileEndings",
                                editTextFileEndings->GetValue());
}

// --------------------------------------------------------------------------------
void FileCopySettingsDialog::correctDialogSize() {
    this->SetMinSize(wxSize(0, 0));
    int width = this->GetBestSize().GetWidth();
    int height = this->GetBestSize().GetHeight();
    wxSize size;
    size.x = dialogSettings->readInteger("CopySettingsDialog", "SizeX", width);
    size.y = dialogSettings->readInteger("CopySettingsDialog", "SizeY", (height * 1.5));
    this->SetSize(size);
    this->SetMinSize(wxSize(width, (height * 1.5)));
    wxPoint point;
    point.x = dialogSettings->readInteger("CopySettingsDialog", "PosX", 10);
    point.y = dialogSettings->readInteger("CopySettingsDialog", "PosY", 10);
    this->SetPosition(point);
}

// --------------------------------------------------------------------------------
