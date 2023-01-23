/*
 * GeneralSettingsDialog.cpp is part of CIFE
 * Copyright (c) 2023  Uwe Merker
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

#include "GeneralSettingsDialog.hpp"
// --------------------------------------------------------------------------------
GeneralSettingsDialog::GeneralSettingsDialog(wxWindow *parent,
        wxConfigBase *cnf) : Ui_GeneralSettingsDialog(parent) {
    config = cnf;
    bool loadLastImage;
    config->Read("/General/OpenLastImageOnRestart", &loadLastImage, false);
    checkboxOpenLastImage->SetValue(loadLastImage);
    bool useCpmUppercase;
    config->Read("/CpmFilesystem/UseUppercaseCharacters", &useCpmUppercase, false);
    checkboxUseCpmUppercase->SetValue(useCpmUppercase);
    correctDialogSize();
}

// --------------------------------------------------------------------------------
GeneralSettingsDialog::~GeneralSettingsDialog() {
    config->Write("/GeneralSettingsDialog/PosX", this->GetPosition().x);
    config->Write("/GeneralSettingsDialog/PosY", this->GetPosition().y);
    config->Write("/GeneralSettingsDialog/SizeX", this->GetSize().x);
    config->Write("/GeneralSettingsDialog/SizeY", this->GetSize().y);
    config->Write("/General/OpenLastImageOnRestart", checkboxOpenLastImage->GetValue());
    config->Write("/CpmFilesystem/UseUppercaseCharacters",
                  checkboxUseCpmUppercase->GetValue());
}

// --------------------------------------------------------------------------------
void GeneralSettingsDialog::correctDialogSize() {
    this->SetMinSize(wxSize(0, 0));
    int width = this->GetBestSize().GetWidth();
    int height = this->GetBestSize().GetHeight();
    wxSize size;
    size.x = config->ReadLong("/GeneralSettingsDialog/SizeX", width);
    size.y = config->ReadLong("/GeneralSettingsDialog/SizeY", height);
    this->SetMinSize(size);
    this->SetMaxSize(size);
    wxPoint point;
    point.x = config->ReadLong("/GeneralSettingsDialog/PosX", 10);
    point.y = config->ReadLong("/GeneralSettingsDialog/PosY", 10);
    this->SetPosition(point);
}

// --------------------------------------------------------------------------------
