/*
 * CreateFileDialog.hpp is part of CIFE
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

#ifndef CREATEFILEDIALOG_H
#define CREATEFILEDIALOG_H
// --------------------------------------------------------------------------------
#include "Ui_CreateFileDialog.hpp"
// --------------------------------------------------------------------------------
#include <wx/string.h>
#include <wx/filename.h>
// --------------------------------------------------------------------------------
class CpmFs;
class CpmTools;
// --------------------------------------------------------------------------------
class CreateFileDialog : public Ui_CreateFileDialog {

    public:     // Attributes

    public:     // Methods
        void setDefaultPath(wxString path);
        wxString getImageType();
        wxString getImageFileName();

    public:     // Constructor & Destructor
        CreateFileDialog(wxWindow *parent, wxConfigBase *cnf, CpmFs *fs, CpmTools *tools,
                         bool isNewFile);
        virtual ~CreateFileDialog();

    protected:  // Event Methods
        void onButtonImageFileClicked(wxCommandEvent &event);
        void onButtonBootFileClicked(wxCommandEvent &event);
        void onButtonOkClicked(wxCommandEvent &event);

    private:    // Attributes
        wxConfigBase *config;
        CpmFs *cpmfs;
        CpmTools *cpmtools;
        wxString defaultPath;
        wxString imageType;
        wxFileName imageFile;

    private:    // Methods
        void correctDialogSize();

        DECLARE_EVENT_TABLE()
};

// --------------------------------------------------------------------------------
#endif // CREATEFILEDIALOG_H
