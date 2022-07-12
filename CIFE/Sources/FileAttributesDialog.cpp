/***********************************************************************************
**
**	Copyright (C) 01.12.2021 Uwe Merker Germany
**
**	Diese Datei ist Teil von CPM_Image-File_Explorer.
**
**	CPM_Image-File_Explorer ist Freie Software: Sie können es unter den Bedingungen
**	der GNU General Public License, wie von der Free Software Foundation,
**	Version 3 der Lizenz oder (nach Ihrer Wahl) jeder späteren
**	veröffentlichten Version, weiterverbreiten und/oder modifizieren.
**
**	CPM_Image-File_Explorer wird in der Hoffnung, dass es nützlich sein wird, aber
**	OHNE JEDE GEWÄHRLEISTUNG, bereitgestellt; sogar ohne die implizite
**	Gewährleistung der MARKTFÄHIGKEIT oder EIGNUNG FÜR EINEN BESTIMMTEN ZWECK.
**	Siehe die GNU General Public License für weitere Details.
**
**	Sie sollten eine Kopie der GNU General Public License zusammen mit diesem
**	Programm erhalten haben. Wenn nicht, siehe <http://www.gnu.org/licenses/>.
**
***********************************************************************************/

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
