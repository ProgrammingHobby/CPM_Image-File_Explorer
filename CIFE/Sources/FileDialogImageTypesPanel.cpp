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
#include <wx/sizer.h>

// --------------------------------------------------------------------------------
wxString FileDialogImageTypesPanel::getSelectedImageType() {
    return (comboboxImageTypes->GetValue());
}

// --------------------------------------------------------------------------------
FileDialogImageTypesPanel::FileDialogImageTypesPanel(wxWindow *parent) : wxPanel(parent) {
    labelImageTypes = new wxStaticText(this, wxID_ANY, "Image Type : ", wxDefaultPosition,
                                       wxDefaultSize);
    comboboxImageTypes = new wxComboBox(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize,
                                        diskdefs::imageTypes, wxCB_SORT | wxCB_READONLY);
    wxBoxSizer *panelSizer = new wxBoxSizer(wxHORIZONTAL);
    panelSizer->AddStretchSpacer(4);
    panelSizer->Add(labelImageTypes, wxSizerFlags(1).Centre());
    panelSizer->Add(comboboxImageTypes, wxSizerFlags(1).Centre());
    this->SetSizerAndFit(panelSizer);
}

// --------------------------------------------------------------------------------
FileDialogImageTypesPanel::~FileDialogImageTypesPanel() {
    // TODO Auto-generated destructor stub
}

// --------------------------------------------------------------------------------
