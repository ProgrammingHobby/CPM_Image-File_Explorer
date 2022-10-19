/*
 * FileProtectionsDialog.hpp is part of CIFE
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

#ifndef FILEPROTECTIONSDIALOG_H
#define FILEPROTECTIONSDIALOG_H
// --------------------------------------------------------------------------------
#include "Ui_FileProtectionsDialog.hpp"
// --------------------------------------------------------------------------------
class FileProtectionsDialog : public Ui_FileProtectionsDialog {
    public:     // Attributes

    public:     // Methods
        void setProtections(wxString protections);
        int getProtections();

    public:     // Constructor & Destructor
        FileProtectionsDialog(wxWindow *parent);
        virtual ~FileProtectionsDialog();

    protected:  // Event Methods

    private:    // Attributes
        const int CPM_ATTR_PWDEL = 2048;    /* Password required to delete */
        const int CPM_ATTR_PWWRITE = 4096;  /* Password required to write */
        const int CPM_ATTR_PWREAD = 8192;   /* Password required to read */

    private:    // Methods
};

// --------------------------------------------------------------------------------
#endif // FILEPROTECTIONSDIALOG_H
