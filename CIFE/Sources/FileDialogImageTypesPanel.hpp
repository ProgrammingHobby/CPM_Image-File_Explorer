/*
 * FileDialogImageTypesPanel.hpp is part of CIFE
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

#ifndef FILEDIALOGIMAGETYPESPANEL_HPP_
#define FILEDIALOGIMAGETYPESPANEL_HPP_
// --------------------------------------------------------------------------------
#include <wx/panel.h>
#include <wx/stattext.h>
#include <wx/combobox.h>
// --------------------------------------------------------------------------------
class FileDialogImageTypesPanel: public wxPanel {

    public: // Attributes

    public: // Methods
        wxString getSelectedImageType();

    public: // Constructor & Destructor
        FileDialogImageTypesPanel(wxWindow *parent);
        virtual ~FileDialogImageTypesPanel();

    protected: // Attributes

    protected: // Methods

    private: // Attributes
        wxStaticText *labelImageTypes;
        wxComboBox *comboboxImageTypes;

    private: // Methods
};

// --------------------------------------------------------------------------------
static wxWindow *createFileDialogImageTypesPanel(wxWindow *parent) {
    return new FileDialogImageTypesPanel(parent);
}

// --------------------------------------------------------------------------------
#endif /* FILEDIALOGIMAGETYPESPANEL_HPP_ */
