/***********************************************************************************
**
**	Copyright (C) 18.10.2021 Uwe Merker Germany
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

#include "RenameFileDialog.h"
// --------------------------------------------------------------------------------
#include <wx/dcscreen.h>
#include <wx/gdicmn.h>
// --------------------------------------------------------------------------------
BEGIN_EVENT_TABLE(RenameFileDialog, wxDialog)
    EVT_CHECKBOX(wxID_CHANGE_USER, RenameFileDialog::onChangeUserClicked)
END_EVENT_TABLE()
// --------------------------------------------------------------------------------
RenameFileDialog::RenameFileDialog(wxWindow *parent) : Ui_RenameFileDialog(parent) {
    wxScreenDC dc;
    dc.SetFont(this->GetFont());
    int height = editOldUser->GetSize().GetHeight();
    int userWidth = dc.GetTextExtent("####").GetWidth();
    editOldUser->SetMinSize(wxSize(userWidth, height));
    editOldUser->SetMaxSize(wxSize(userWidth, height));
    editNewUser->SetMinSize(wxSize(userWidth, height));
    editNewUser->SetMaxSize(wxSize(userWidth, height));
    int nameWidth = dc.GetTextExtent("##############").GetWidth();
    editOldFileName->SetMinSize(wxSize(nameWidth, height));
    editOldFileName->SetMaxSize(wxSize(nameWidth, height));
    editNewFileName->SetMinSize(wxSize(nameWidth, height));
    editNewFileName->SetMaxSize(wxSize(nameWidth, height));
    editNewFileName->SetFocus();
    this->SetMinSize(wxSize(0, 0));
    this->SetSize(this->GetBestSize());
    this->SetMinSize(this->GetBestSize());
}

// --------------------------------------------------------------------------------
RenameFileDialog::~RenameFileDialog() {
}

// --------------------------------------------------------------------------------
void RenameFileDialog::setOldUser(int user) {
    editOldUser->SetValue(wxString::Format("%i", user));
    editNewUser->SetValue(wxString::Format("%i", user));
}

// --------------------------------------------------------------------------------
void RenameFileDialog::setOldName(wxString name) {
    editOldFileName->SetValue(name);
    editNewFileName->SetValue(name);
}

// --------------------------------------------------------------------------------
wxString RenameFileDialog::getNewName() {
    return (editNewFileName->GetValue());
}

// --------------------------------------------------------------------------------
int RenameFileDialog::getNewUser() {
    return (wxAtoi(editNewUser->GetValue()));
}

// --------------------------------------------------------------------------------
void RenameFileDialog::onChangeUserClicked(wxCommandEvent &event) {
    bool value = checkboxChangeUser->GetValue();
    editNewUser->Enable(value);

    if (!value) {
        editNewUser->SetValue(editOldUser->GetValue());
        editNewFileName->SetFocus();
    }
    else {
        editNewUser->SetFocus();
    }
}

// --------------------------------------------------------------------------------
