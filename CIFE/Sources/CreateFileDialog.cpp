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
#include "CpmTools.hpp"
#include "FileDialogImageTypesPanel.hpp"
// --------------------------------------------------------------------------------
#include <wx/dcscreen.h>
#include <wx/filedlg.h>
#include <wx/stdpaths.h>
#include <wx/msgdlg.h>
// --------------------------------------------------------------------------------
BEGIN_EVENT_TABLE(CreateFileDialog, wxDialog)
    EVT_BUTTON(wxID_BUTTON_IMAGEFILE, CreateFileDialog::onButtonImageFileClicked)
    EVT_BUTTON(wxID_BUTTON_BOOTFILE, CreateFileDialog::onButtonBootFileClicked)
    EVT_BUTTON(wxID_OK, CreateFileDialog::onButtonOkClicked)
END_EVENT_TABLE()
// --------------------------------------------------------------------------------
CreateFileDialog::CreateFileDialog(wxWindow *parent, CpmFs *fs, CpmTools *tools,
                                   bool isNewFile) : Ui_CreateFileDialog(parent) {
    cpmfs = fs;
    cpmtools = tools;
    int size = editBootTrackFile->GetSize().GetHeight();
    wxScreenDC dc;
    dc.SetFont(buttonImageFile->GetFont());
    wxString tempString = " " + buttonImageFile->GetLabel() + " ";
    wxCoord width, height;
    dc.GetTextExtent(tempString, &width, &height);
    buttonBootTrackFile->SetMinSize(wxSize(width, size));
    buttonBootTrackFile->SetMaxSize(wxSize(width, size));
    buttonImageFile->SetMinSize(wxSize(width, size));
    buttonImageFile->SetMaxSize(wxSize(width, size));
    buttonOk->Enable(false);
    imageFile.Clear();
    imageType.Clear();
    panelImageFile->Enable(isNewFile);
    panelImageType->Enable(isNewFile);

    if (isNewFile) {
        textWarning1->SetLabel("Are you sure you want create an new empty Image-File ?");
        textWarning2->SetLabel("Existing Imagefile will be overwritten !");
        this->SetTitle("Create new empty Image");
        panelBootTrackFile->Enable(false);
    }
    else {
        textWarning1->SetLabel("Are you sure you want recreate existing Image-File ?");
        textWarning2->SetLabel("All existing Data will be lost !");
        this->SetTitle("Reformat current Image");
        panelBootTrackFile->Enable(cpmfs->getBootTracksEnabled());
        imageType = cpmtools->getActualImageType();
        editImageType->SetValue(imageType);
        imageFile = cpmtools->getActualFileName();
        editImageFile->SetValue(imageFile.GetFullName());
        buttonOk->Enable(!editImageType->IsEmpty() && imageFile.IsOk());
    }

    width = this->GetBestSize().GetWidth();
    height = this->GetBestSize().GetHeight();
    this->SetMinSize(wxSize(width, height));
    this->SetMaxSize(wxSize(width, height));
    this->SetFocus();
}

// --------------------------------------------------------------------------------
CreateFileDialog::~CreateFileDialog() {
}

// --------------------------------------------------------------------------------
void CreateFileDialog::setDefaultPath(wxString path) {
    defaultPath = path;
    wxFileName filename(path);

    if (filename.IsOk()) {
        defaultPath = filename.GetPath();
    }
    else {
        defaultPath = wxStandardPaths::Get().GetDocumentsDir();
    }
}

// --------------------------------------------------------------------------------
wxString CreateFileDialog::getImageType() {
    return (imageType);
}

// --------------------------------------------------------------------------------
wxString CreateFileDialog::getImageFileName() {
    return (imageFile.GetFullPath());
}

// --------------------------------------------------------------------------------
void CreateFileDialog::onButtonImageFileClicked(wxCommandEvent &event) {
    WXUNUSED(event)
    wxFileDialog fileDialog(this, _("Open CP/M Image-File"), defaultPath,
                            imageFile.GetFullName(),
                            _("Image Files (*.img,*.fdd,*.dsk)|*.img;*.IMG;*.fdd;*.FDD;*.dsk;*.DSK|"
                              "Binary Files (*.bin,*.cpm,*.sys)|*.bin;*.BIN;*.cpm;*.CPM;*.sys;*.SYS|"
                              "all Files (*.*)|*.*"), wxFD_SAVE);

    fileDialog.SetExtraControlCreator(&createFileDialogImageTypesPanel);

    if (fileDialog.ShowModal() == wxID_OK) {
        imageFile = fileDialog.GetPath();
        defaultPath = imageFile.GetPath();
        editImageFile->SetValue(imageFile.GetFullName());
        imageType = static_cast<FileDialogImageTypesPanel *>
                    (fileDialog.GetExtraControl())->getSelectedImageType();

        if (imageType.IsEmpty()) {
            wxMessageDialog dialog(NULL,
                                   "\nNo Image-Type selected.""\n\nPlease select proper Image-Type"
                                   " to create the new Image.", "Create new Image-File", wxOK | wxICON_QUESTION);
            dialog.ShowModal();
            return;
        }

        editImageType->SetValue(imageType);
        cpmtools->setImageType(imageType);
        panelBootTrackFile->Enable(cpmfs->getBootTracksEnabled());
        buttonOk->Enable(!imageType.IsEmpty() && imageFile.IsOk());
    }
}

// --------------------------------------------------------------------------------
void CreateFileDialog::onButtonBootFileClicked(wxCommandEvent &event) {
    WXUNUSED(event)
    wxFileDialog fileDialog(this, _("Open CP/M Boot-Track Image File"), defaultPath,
                            wxEmptyString,
                            _("Image Files (*.img,*.fdd,*.dsk)|*.img;*.IMG;*.fdd;*.FDD;*.dsk;*.DSK|"
                              "Binary Files (*.bin,*.cpm,*.sys)|*.bin;*.BIN;*.cpm;*.CPM;*.sys;*.SYS|"
                              "all Files (*.*)|*.*"), wxFD_OPEN);

    if (fileDialog.ShowModal() == wxID_OK) {
        wxString filePath = fileDialog.GetPath();
        editBootTrackFile->SetValue(filePath);
        editBootTrackFile->SetInsertionPoint(filePath.length());
    }
}

// --------------------------------------------------------------------------------
void CreateFileDialog::onButtonOkClicked(wxCommandEvent &event) {
    cpmtools->createNewImage(imageFile.GetFullPath(), editFileSystemLabel->GetValue(),
                             checkboxUseTimeStamps->GetValue(), editBootTrackFile->GetValue());
    wxDialog::EndModal(wxID_OK);
}
// --------------------------------------------------------------------------------
