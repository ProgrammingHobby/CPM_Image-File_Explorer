/*
 * CreateFileDialog.cpp is part of CIFE
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

#include "CreateFileDialog.hpp"

#include <wx/filedlg.h>
#include <wx/stdpaths.h>
// --------------------------------------------------------------------------------
BEGIN_EVENT_TABLE(CreateFileDialog, wxDialog)
    EVT_BUTTON(wxID_BUTTON_BOOTFILE, CreateFileDialog::onButtonBootFileClicked)
END_EVENT_TABLE()
// --------------------------------------------------------------------------------
CreateFileDialog::CreateFileDialog(wxWindow *parent) : Ui_CreateFileDialog(parent) {
    int size = editBootTrackFile->GetSize().GetHeight();
    buttonBootTrackFile->SetMinSize(wxSize(size, size));
    buttonBootTrackFile->SetMaxSize(wxSize(size, size));
}

// --------------------------------------------------------------------------------
CreateFileDialog::~CreateFileDialog() {
}

// --------------------------------------------------------------------------------
wxString CreateFileDialog::getBootTrackFile() {
    return (editBootTrackFile->GetValue());
}

// --------------------------------------------------------------------------------
wxString CreateFileDialog::getFileSystemLabel() {
    return (editFileSystemLabel->GetValue());
}

// --------------------------------------------------------------------------------
bool CreateFileDialog::getUseTimestamps() {
    return (checkboxUseTimeStamps->GetValue());
}

// --------------------------------------------------------------------------------
void CreateFileDialog::onButtonBootFileClicked(wxCommandEvent &event) {
    WXUNUSED(event)
    wxFileDialog fileDialog(this, _("Open CP/M Boot-Track Image File"), wxStandardPaths::Get().GetUserDataDir(),
                            wxEmptyString, _("Binary Files (*.bin,*.cpm,*.sys)|*.bin;*.BIN;*.cpm;*.CPM;*.sys;*.SYS|"
                                    "Image Files (*.img,*.fdd,*.dsk)|*.img;*.IMG;*.fdd;*.FDD;*.dsk;*.DSK|"
                                    "all Files (*.*)|*.*"),
                            wxFD_OPEN);

    if (fileDialog.ShowModal() == wxID_OK) {
        wxString filePath = fileDialog.GetPath();
        editBootTrackFile->SetValue(filePath);
        editBootTrackFile->SetInsertionPoint(filePath.length());
    }
}

// --------------------------------------------------------------------------------
void CreateFileDialog::setBootTracksUsed(bool value) {
    panelBootTrackFile->Enable(value);
}

// --------------------------------------------------------------------------------
