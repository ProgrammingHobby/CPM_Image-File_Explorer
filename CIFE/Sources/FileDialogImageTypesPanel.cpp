/*
 * FileDialogImageTypesPanel.cpp is part of CIFE
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

#include "FileDialogImageTypesPanel.hpp"
#include "diskdefs.hpp"
// --------------------------------------------------------------------------------
#if wxVERSION_NUMBER >= 3100
// --------------------------------------------------------------------------------
void ImageTypesHook::AddCustomControls(wxFileDialogCustomize &customizer) {
    textImageType = customizer.AddStaticText("Image Type :");
    int count = diskdefs::imageTypes.GetCount();
    diskdefs::imageTypes.Sort();
    wxCArrayString cArray(diskdefs::imageTypes);
    wxString *choiceStrings = cArray.GetStrings();
    choiceBoxImageType = customizer.AddChoice(count, choiceStrings);
    int selection = diskdefs::imageTypes.Index(selectedImageType);
    choiceBoxImageType->SetSelection(selection);
}

// --------------------------------------------------------------------------------
void ImageTypesHook::TransferDataFromCustomControls() {
    int selection = choiceBoxImageType->GetSelection();

    if (selection > -1) {
        selectedImageType = diskdefs::imageTypes[selection];
    }
    else {
        selectedImageType = wxEmptyString;
    }
}

// --------------------------------------------------------------------------------
wxString ImageTypesHook::getImageType() {
    return (selectedImageType);
}

// --------------------------------------------------------------------------------
void ImageTypesHook::setImageType(wxString type) {
    selectedImageType = type;
}

// --------------------------------------------------------------------------------
#else
// --------------------------------------------------------------------------------
#include <wx/sizer.h>
// --------------------------------------------------------------------------------
wxString FileDialogImageTypesPanel::getSelectedImageType() {
    return (comboboxImageTypes->GetValue());
}

// --------------------------------------------------------------------------------
FileDialogImageTypesPanel::FileDialogImageTypesPanel(wxWindow *parent) : wxPanel(parent) {
    labelImageTypes = new wxStaticText(this, wxID_ANY, "Image Type : ", wxDefaultPosition,
                                       wxDefaultSize);
    comboboxImageTypes = new wxComboBox(this, wxID_ANY, wxEmptyString, wxDefaultPosition,
                                        wxDefaultSize,
                                        diskdefs::imageTypes, wxCB_SORT | wxCB_READONLY);
    wxBoxSizer *panelSizer = new wxBoxSizer(wxHORIZONTAL);
    panelSizer->AddStretchSpacer(1);
    panelSizer->Add(labelImageTypes, wxSizerFlags(1).Centre());
    panelSizer->Add(comboboxImageTypes, wxSizerFlags(1).Centre());
    this->SetSizerAndFit(panelSizer);
}

// --------------------------------------------------------------------------------
FileDialogImageTypesPanel::~FileDialogImageTypesPanel() {
}

//---------------------------------------------------------------------------------
#endif
