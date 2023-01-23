/*
 * GeneralSettingsDialog.hpp is part of CIFE
 * Copyright (c) 2023  Uwe Merker
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

#ifndef GENERALSETTINGSDIALOG_HPP
#define GENERALSETTINGSDIALOG_HPP
// --------------------------------------------------------------------------------
#include "Ui_GeneralSettingsDialog.hpp"
// --------------------------------------------------------------------------------
class GeneralSettingsDialog : public Ui_GeneralSettingsDialog {
    public:     // Attributes

    public:     // Methods

    public:     // Constructor & Destructor
        GeneralSettingsDialog(wxWindow *parent, wxConfigBase *cnf);
        virtual ~GeneralSettingsDialog();

    protected:  // Event Methods

    private:    // Attributes
        wxConfigBase *config;

    private:    // Methods
        void correctDialogSize();
};

// --------------------------------------------------------------------------------
#endif // GENERALSETTINGSDIALOG_HPP
