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
// --------------------------------------------------------------------------------
#include <wx/filedlg.h>
#include <wx/stdpaths.h>
// --------------------------------------------------------------------------------
BEGIN_EVENT_TABLE(CreateFileDialog, wxDialog)
    EVT_BUTTON(wxID_BUTTON_IMAGEFILE, CreateFileDialog::onButtonImageFileClicked)
    EVT_BUTTON(wxID_BUTTON_BOOTFILE, CreateFileDialog::onButtonBootFileClicked)
END_EVENT_TABLE()
// --------------------------------------------------------------------------------
CreateFileDialog::CreateFileDialog(wxWindow *parent, bool isBoottrackUsed,
                                   bool isNewFile) : Ui_CreateFileDialog(parent) {
    panelImageFile->Enable(isNewFile);
    panelBootTrackFile->Enable(isBoottrackUsed);
    int size = editBootTrackFile->GetSize().GetHeight();
    wxScreenDC dc;
    dc.SetFont(buttonImageFile->GetFont());
    wxString tempString = "#" + buttonImageFile->GetLabel() + "#";
    wxCoord width, height;
    dc.GetTextExtent(tempString, &width, &height);
    buttonBootTrackFile->SetMinSize(wxSize(width, size));
    buttonBootTrackFile->SetMaxSize(wxSize(width, size));
    buttonImageFile->SetMinSize(wxSize(width, size));
    buttonImageFile->SetMaxSize(wxSize(width, size));

    if (isNewFile) {
        textWarning1->SetLabel("Are you sure you want create an new empty Image-File ?");
        textWarning2->SetLabel("Existing Imagefile will be overwritten !");
    }
    else {
        textWarning1->SetLabel("Are you sure you want recreate existing Image-File ?");
        textWarning2->SetLabel("All existing Data will be lost !");
    }

    width = this->GetBestSize().GetWidth();
    height = this->GetBestSize().GetHeight();
    this->SetMinSize(wxSize(width, height));
    this->SetMaxSize(wxSize(width, height));
}

// --------------------------------------------------------------------------------
CreateFileDialog::~CreateFileDialog() {
}

// --------------------------------------------------------------------------------
void CreateFileDialog::setImageFileName(wxString fileName) {
    fileName = fileName.substr(fileName.find_last_of("/\\") + 1);
    editImageFile->SetValue(fileName);
    editImageFile->SetInsertionPoint(fileName.length());
}

// --------------------------------------------------------------------------------
wxString CreateFileDialog::getImageFileName() {
    return (editImageFile->GetValue());
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
bool CreateFileDialog::useTimestamps() {
    return (checkboxUseTimeStamps->GetValue());
}

// --------------------------------------------------------------------------------
void CreateFileDialog::onButtonImageFileClicked(wxCommandEvent &event) {
    WXUNUSED(event)
    wxFileDialog fileDialog(this, _("Open CP/M Boot-Track Image File"),
                            wxStandardPaths::Get().GetUserDataDir(), wxEmptyString,
                            _("Binary Files (*.bin,*.cpm,*.sys)|*.bin;*.BIN;*.cpm;*.CPM;*.sys;*.SYS|"
                              "Image Files (*.img,*.fdd,*.dsk)|*.img;*.IMG;*.fdd;*.FDD;*.dsk;*.DSK|"
                              "all Files (*.*)|*.*"), wxFD_SAVE);

    if (fileDialog.ShowModal() == wxID_OK) {
        wxString filePath = fileDialog.GetPath();
        editImageFile->SetValue(filePath);
        editImageFile->SetInsertionPoint(filePath.length());
    }
}

// --------------------------------------------------------------------------------
void CreateFileDialog::onButtonBootFileClicked(wxCommandEvent &event) {
    WXUNUSED(event)
    wxFileDialog fileDialog(this, _("Open CP/M Boot-Track Image File"),
                            wxStandardPaths::Get().GetUserDataDir(), wxEmptyString,
                            _("Binary Files (*.bin,*.cpm,*.sys)|*.bin;*.BIN;*.cpm;*.CPM;*.sys;*.SYS|"
                              "Image Files (*.img,*.fdd,*.dsk)|*.img;*.IMG;*.fdd;*.FDD;*.dsk;*.DSK|"
                              "all Files (*.*)|*.*"), wxFD_OPEN);

    if (fileDialog.ShowModal() == wxID_OK) {
        wxString filePath = fileDialog.GetPath();
        editBootTrackFile->SetValue(filePath);
        editBootTrackFile->SetInsertionPoint(filePath.length());
    }
}

// --------------------------------------------------------------------------------
