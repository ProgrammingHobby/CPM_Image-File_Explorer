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
#include <wx/defs.h>
// --------------------------------------------------------------------------------
#if wxVERSION_NUMBER >= 3100
// --------------------------------------------------------------------------------
#include <wx/event.h>
#include <wx/filedlgcustomize.h>
// --------------------------------------------------------------------------------
class ImageTypesHook: public wxFileDialogCustomizeHook {

    public: // Attributes

    public: // Methods
        void AddCustomControls(wxFileDialogCustomize &customizer) wxOVERRIDE;
        void TransferDataFromCustomControls() wxOVERRIDE;
        wxString getImageType();
        void setImageType(wxString type);

    public: // Constructor & Destructor

    protected: // Attributes

    protected: // Methods

    private: // Attributes
        wxFileDialogChoice *choiceBoxImageType;
        wxFileDialogStaticText *textImageType;
        wxString selectedImageType;

    private: // Methods

};
#else
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
#endif
#endif /* FILEDIALOGIMAGETYPESPANEL_HPP_ */
