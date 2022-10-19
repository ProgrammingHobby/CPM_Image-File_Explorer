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
class CreateFileDialog : public Ui_CreateFileDialog {

    public:     // Attributes

    public:     // Methods
        wxString getBootTrackFile();
        wxString getFileSystemLabel();
        bool getUseTimestamps();
        void setBootTracksUsed(bool value);

    public:     // Constructor & Destructor
        CreateFileDialog(wxWindow *parent);
        virtual ~CreateFileDialog();

    protected:  // Event Methods
    void onButtonBootFileClicked(wxCommandEvent &event);

    private:    // Attributes

    private:    // Methods

        DECLARE_EVENT_TABLE()
};

// --------------------------------------------------------------------------------
#endif // CREATEFILEDIALOG_H
