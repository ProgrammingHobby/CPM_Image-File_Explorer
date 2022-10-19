/*
 * FileAttributesDialog.cpp is part of CIFE
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

#include "FileAttributesDialog.hpp"
// --------------------------------------------------------------------------------
FileAttributesDialog::FileAttributesDialog(wxWindow *parent) : Ui_FileAttributesDialog(parent) {
    checkBoxes[0] = checkboxF1;
    checkBoxes[1] = checkboxF2;
    checkBoxes[2] = checkboxF3;
    checkBoxes[3] = checkboxF4;
    checkBoxes[4] = checkboxReadOnly;
    checkBoxes[5] = checkboxSystem;
    checkBoxes[6] = checkboxArchived;
}

// --------------------------------------------------------------------------------
FileAttributesDialog::~FileAttributesDialog() {
}

// --------------------------------------------------------------------------------
void FileAttributesDialog::setAttributes(wxString attributes) {
    attributes.Replace(" ", "");

    for (int i = 0; i < 7; i++) {
        if (attributes[i] != '-') {
            checkBoxes[i]->SetValue(true);
        }
    }
}

// --------------------------------------------------------------------------------
int FileAttributesDialog::getAttributes() {
    int attr = 0;

    for (int i = 0; i < 7; i++) {
        if (checkBoxes[i]->GetValue()) {
            switch (i) {
                case 0 :
                    attr |= CPM_ATTR_F1;
                    break;

                case 1 :
                    attr |= CPM_ATTR_F2;
                    break;

                case 2 :
                    attr |= CPM_ATTR_F3;
                    break;

                case 3 :
                    attr |= CPM_ATTR_F4;
                    break;

                case 4 :
                    attr |= CPM_ATTR_RO;
                    break;

                case 5 :
                    attr |= CPM_ATTR_SYS;
                    break;

                case 6 :
                    attr |= CPM_ATTR_ARCV;
                    break;
            }
        }
    }

    return (attr);
}

// --------------------------------------------------------------------------------
