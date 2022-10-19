/*
 * RenameFileDialog.hpp is part of CIFE
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

#ifndef RENAMEFILEDIALOG_H
#define RENAMEFILEDIALOG_H
// --------------------------------------------------------------------------------
#include "Ui_RenameFileDialog.hpp"
// --------------------------------------------------------------------------------
class RenameFileDialog : public Ui_RenameFileDialog {

    public:     // Attributes

    public:     // Methods
        void setOldUser(int user);
        void setOldName(wxString name);
        int getNewUser();
        wxString getNewName();

    public:     // Constructor & Destructor
        RenameFileDialog(wxWindow *parent);
        virtual ~RenameFileDialog();

    protected:  // Event Methods
        void onChangeUserClicked(wxCommandEvent &event);

    private:    // Attributes

    private:    // Methods

        DECLARE_EVENT_TABLE()
};

// --------------------------------------------------------------------------------
#endif // RENAMEFILEDIALOG_H
