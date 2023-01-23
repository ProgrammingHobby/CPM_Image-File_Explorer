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
CreateFileDialog::CreateFileDialog(wxWindow *parent, wxConfigBase *cnf, CpmFs *fs,
                                   CpmTools *tools, bool isNewFile) : Ui_CreateFileDialog(parent) {
    config = cnf;
    cpmfs = fs;
    cpmtools = tools;
    wxScreenDC dc;
    dc.SetFont(buttonImageFile->GetFont());
    wxString tempString = "#" + buttonImageFile->GetLabel() + "#";
    wxCoord width, height;
    dc.GetTextExtent(tempString, &width, &height);
    height = editImageFile->GetSize().GetY();
    buttonBootTrackFile->SetMinSize(wxSize(width, height));
    buttonBootTrackFile->SetMaxSize(wxSize(width, height));
    buttonImageFile->SetMinSize(wxSize(width, height));
    buttonImageFile->SetMaxSize(wxSize(width, height));
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

    correctDialogSize();
}

// --------------------------------------------------------------------------------
CreateFileDialog::~CreateFileDialog() {
    config->Write("/CreateFileDialog/PosX", this->GetPosition().x);
    config->Write("/CreateFileDialog/PosY", this->GetPosition().y);
    config->Write("/CreateFileDialog/SizeX", this->GetSize().x);
    config->Write("/CreateFileDialog/SizeY", this->GetSize().y);
}

// --------------------------------------------------------------------------------
void CreateFileDialog::correctDialogSize()  {
    int width = this->GetBestSize().GetWidth();
    int height = this->GetBestSize().GetHeight();
    wxSize size;
    size.x = config->ReadLong("/CreateFileDialog/SizeX", width);
    size.y = config->ReadLong("/CreateFileDialog/SizeY", height);
    this->SetMinSize(size);
    this->SetMaxSize(size);
    wxPoint point;
    point.x = config->ReadLong("/CreateFileDialog/PosX", 10);
    point.y = config->ReadLong("/CreateFileDialog/PosY", 10);
    this->SetPosition(point);
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

    do {
        wxFileDialog fileDialog(this, _("Open CP/M Image-File"), defaultPath,
                                imageFile.GetFullName(),
                                _("Image Files (*.img,*.fdd,*.dsk)|*.img;*.IMG;*.fdd;*.FDD;*.dsk;*.DSK|"
                                  "Binary Files (*.bin,*.cpm,*.sys)|*.bin;*.BIN;*.cpm;*.CPM;*.sys;*.SYS|"
                                  "all Files (*.*)|*.*"), wxFD_SAVE);
#if wxVERSION_NUMBER >= 3100
        ImageTypesHook dialogHook;
        fileDialog.SetCustomizeHook(dialogHook);
#else
        fileDialog.SetExtraControlCreator(&createFileDialogImageTypesPanel);
#endif

        if (fileDialog.ShowModal() == wxID_OK) {
#if wxVERSION_NUMBER >= 3100
            imageType = dialogHook.getImageType();
#else
            imageType = static_cast<FileDialogImageTypesPanel *>
                        (fileDialog.GetExtraControl())->getSelectedImageType();
#endif
            imageFile = fileDialog.GetPath();
            defaultPath = imageFile.GetPath();

            if (!imageType.IsEmpty()) {
                editImageFile->SetValue(imageFile.GetFullName());
                editImageType->SetValue(imageType);
                cpmtools->setImageType(imageType);
                panelBootTrackFile->Enable(cpmfs->getBootTracksEnabled());
                buttonOk->Enable(!imageType.IsEmpty() && imageFile.IsOk());
                break;
            }
            else {
                wxMessageDialog dialog(NULL,
                                       "\nNo Image-Type selected."
                                       "\n\nPlease select proper Image-Type"
                                       " to create the new Image.",
                                       "Create new Image-File", wxOK | wxICON_QUESTION);
                dialog.ShowModal();
            }
        }
        else {
            break;
        }
    } while (imageType.IsEmpty());

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
