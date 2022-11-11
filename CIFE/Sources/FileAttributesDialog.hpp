/*
 * FileAttributesDialog.hpp is part of CIFE
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

#ifndef FILEATTRIBUTESDIALOG_H
#define FILEATTRIBUTESDIALOG_H
// --------------------------------------------------------------------------------
#include "Ui_FileAttributesDialog.hpp"
// --------------------------------------------------------------------------------
#include <wx/string.h>
// --------------------------------------------------------------------------------
class FileAttributesDialog : public Ui_FileAttributesDialog {
    public:     // Attributes

    public:     // Methods
        void setAttributes(wxString attributes);
        int getAttributes();

    public:     // Constructor & Destructor
        FileAttributesDialog(wxWindow *parent);
        virtual ~FileAttributesDialog();

    protected:  // Event Methods

    private:    // Attributes
        /* CP/M file attributes */
        const int CPM_ATTR_F1 = 1;
        const int CPM_ATTR_F2 = 2;
        const int CPM_ATTR_F3 = 4;
        const int CPM_ATTR_F4 = 8;
        const int CPM_ATTR_RO = 256;   /* Read-only */
        const int CPM_ATTR_SYS = 512;   /* System */
        const int CPM_ATTR_ARCV = 1024;  /* Archive */
        wxCheckBox *checkBoxes[7];

    private:    // Methods
};

// --------------------------------------------------------------------------------
#endif // FILEATTRIBUTESDIALOG_H
