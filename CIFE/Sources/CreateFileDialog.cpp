/***********************************************************************************
**
**	Copyright (C) 11.12.2021 Uwe Merker Germany
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

#include "CreateFileDialog.hpp"

#include <wx/filedlg.h>
#include <wx/stdpaths.h>
// --------------------------------------------------------------------------------
BEGIN_EVENT_TABLE(CreateFileDialog, wxDialog)
    EVT_BUTTON(wxID_BUTTON_BOOTFILE, CreateFileDialog::onButtonBootFileClicked)
END_EVENT_TABLE()
// --------------------------------------------------------------------------------
CreateFileDialog::CreateFileDialog(wxWindow *parent) : Ui_CreateFileDialog(parent) {
    int size = editBootTrackFile->GetSize().GetHeight();
    buttonBootTrackFile->SetMinSize(wxSize(size, size));
    buttonBootTrackFile->SetMaxSize(wxSize(size, size));
}

// --------------------------------------------------------------------------------
CreateFileDialog::~CreateFileDialog() {
}

// --------------------------------------------------------------------------------
wxString CreateFileDialog::getBootTrackFile() {
    return (editBootTrackFile->GetValue());
}

// --------------------------------------------------------------------------------
wxString CreateFileDialog::getFileSystemLabel() {
    return (editFileSystemLabel->GetValue());
}

// --------------------------------------------------------------------------------
bool CreateFileDialog::getUseTimestamps() {
    return (checkboxUseTimeStamps->GetValue());
}

// --------------------------------------------------------------------------------
void CreateFileDialog::onButtonBootFileClicked(wxCommandEvent &event) {
    WXUNUSED(event)
    wxFileDialog fileDialog(this, _("Open CP/M Boot-Track Image File"), wxStandardPaths::Get().GetUserDataDir(),
                            wxEmptyString, _("Binary Files (*.bin,*.cpm,*.sys)|*.bin;*.BIN;*.cpm;*.CPM;*.sys;*.SYS|"
                                    "Image Files (*.img,*.fdd,*.dsk)|*.img;*.IMG;*.fdd;*.FDD;*.dsk;*.DSK|"
                                    "all Files (*.*)|*.*"),
                            wxFD_OPEN);

    if (fileDialog.ShowModal() == wxID_OK) {
        wxString filePath = fileDialog.GetPath();
        editBootTrackFile->SetValue(filePath);
        editBootTrackFile->SetInsertionPoint(filePath.length());
    }
}

// --------------------------------------------------------------------------------
void CreateFileDialog::setBootTracksUsed(bool value) {
    panelBootTrackFile->Enable(value);
}

// --------------------------------------------------------------------------------
