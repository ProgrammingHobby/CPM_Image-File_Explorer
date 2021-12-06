/***********************************************************************************
**
**	Copyright (C) 04.12.2021 Uwe Merker Germany
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

#include "FileProtectionsDialog.h"
// --------------------------------------------------------------------------------
#include <wx/tokenzr.h>
// --------------------------------------------------------------------------------
FileProtectionsDialog::FileProtectionsDialog(wxWindow *parent) : Ui_FileProtectionsDialog(parent) {
}

// --------------------------------------------------------------------------------
FileProtectionsDialog::~FileProtectionsDialog() {
}

// --------------------------------------------------------------------------------
void FileProtectionsDialog::setProtections(wxString protections) {
    wxStringTokenizer tokenizer(protections, " ");

    while (tokenizer.HasMoreTokens()) {
        wxString token = tokenizer.GetNextToken();

        if (token == "rd") {
            checkboxReadProtect->SetValue(true);
        }
        else if (token == "wr") {
            checkboxWriteProtect->SetValue(true);
        }
        else if (token == "del") {
            checkboxDeleteProtect->SetValue(true);
        }
    }
}

// --------------------------------------------------------------------------------
int FileProtectionsDialog::getProtections() {
    int protections = 0;

    if (checkboxReadProtect->GetValue()) {
        protections |= CPM_ATTR_PWREAD;
    }

    if (checkboxWriteProtect->GetValue()) {
        protections |= CPM_ATTR_PWWRITE;
    }

    if (checkboxDeleteProtect->GetValue()) {
        protections |= CPM_ATTR_PWDEL;
    }

    return (protections);
}

// --------------------------------------------------------------------------------
