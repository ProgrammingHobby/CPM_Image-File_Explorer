/***********************************************************************************
**
**	Copyright (C) 14.07.2021 Uwe Merker Germany
**
**	Diese Datei ist Teil von CPM_Image_File_Explorer.
**
**	CPM_Image_File_Explorer ist Freie Software: Sie können es unter den Bedingungen
**	der GNU General Public License, wie von der Free Software Foundation,
**	Version 3 der Lizenz oder (nach Ihrer Wahl) jeder späteren
**	veröffentlichten Version, weiterverbreiten und/oder modifizieren.
**
**	CPM_Image_File_Explorer wird in der Hoffnung, dass es nützlich sein wird, aber
**	OHNE JEDE GEWÄHRLEISTUNG, bereitgestellt; sogar ohne die implizite
**	Gewährleistung der MARKTFÄHIGKEIT oder EIGNUNG FÜR EINEN BESTIMMTEN ZWECK.
**	Siehe die GNU General Public License für weitere Details.
**
**	Sie sollten eine Kopie der GNU General Public License zusammen mit diesem
**	Programm erhalten haben. Wenn nicht, siehe <http://www.gnu.org/licenses/>.
**
***********************************************************************************/

#include "MainWindow.h"
// --------------------------------------------------------------------------------
#include <wx/aboutdlg.h>
#include <wx/platinfo.h>
#include <wx/versioninfo.h>
#include <wx/filedlg.h>
#include <wx/stdpaths.h>
#include <wx/txtstrm.h>
#include <wx/wfstream.h>
#include <wx/tokenzr.h>
// --------------------------------------------------------------------------------
BEGIN_EVENT_TABLE(MainWindow, wxFrame)
    EVT_MENU(wxID_CLOSE, MainWindow::onMenuCloseClicked)
    EVT_MENU(wxID_ABOUT, MainWindow::onMenuAboutClicked)
    EVT_BUTTON(wxID_BUTTON_IMAGE_FILE, MainWindow::onButtonImageFileClicked)
    EVT_COMBOBOX_DROPDOWN(wxID_IMAGE_TYPE, MainWindow::onComboBoxDropDown)
END_EVENT_TABLE()
// --------------------------------------------------------------------------------
MainWindow::MainWindow(wxWindow *parent) : Ui_MainWindow(parent) {

    int size = editImageFile->GetSize().GetHeight();
    buttonImageFile->SetMinSize(wxSize(size, size));
    buttonImageFile->SetMaxSize(wxSize(size, size));

    this->Fit();
    this->SetMinSize(this->GetSize());

    comboboxImageType->Append(getImageTypes());
}

// --------------------------------------------------------------------------------
MainWindow::~MainWindow() {
}

// --------------------------------------------------------------------------------
void MainWindow::onMenuCloseClicked(wxCommandEvent &event) {
    WXUNUSED(event)
    Close(true);
}

// --------------------------------------------------------------------------------
void MainWindow::onMenuAboutClicked(wxCommandEvent &event) {
    WXUNUSED(event)
    wxAboutDialogInfo aboutInfo;
    wxVersionInfo versionInfo("", wxMAJOR_VERSION, wxMINOR_VERSION, wxRELEASE_NUMBER);

    aboutInfo.SetName("CP/M Image-File Explorer");
    aboutInfo.SetVersion("0.0.1");
    aboutInfo.SetIcon(this->GetIcon());
    aboutInfo.SetDescription(_("Written in C/C++ with CodeLite-IDE\n"
                               "Using wxWidgets GUI - Framework Version ") + versionInfo.GetVersionString());
    aboutInfo.SetCopyright("Uwe Merker  (C) 2021");
    aboutInfo.SetWebSite("https://github.com/ProgrammingHobby/CPM_Image-File_Explorer.git");

    wxAboutBox(aboutInfo);
}

// --------------------------------------------------------------------------------
void MainWindow::onButtonImageFileClicked(wxCommandEvent &event) {
    wxFileDialog fileDialog(this, _T("Open CP/M Disk Image File"), wxStandardPaths::Get().GetUserDataDir(),
                            wxEmptyString, _T("Image Files (*.img,*.fdd,*.dsk)|*.img;*.fdd;*.dsk|all Files (*.*)|*.*"),
                            wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (fileDialog.ShowModal() == wxID_OK) {
        editImageFile->SetValue(fileDialog.GetPath());
        editImageFile->SetInsertionPoint(editImageFile->GetValue().length());
    }
}

// --------------------------------------------------------------------------------
void MainWindow::onComboBoxDropDown(wxCommandEvent &event) {
    comboboxImageType->Clear();
    comboboxImageType->Append(getImageTypes());
}

// --------------------------------------------------------------------------------
wxArrayString MainWindow::getImageTypes() {
    wxArrayString imageTypes;
    wxFileInputStream file(wxT("diskdefs"));
    wxTextInputStream text(file);

    while (!file.Eof()) {
        wxString line = text.ReadLine();
        wxStringTokenizer tokenizer(line, " ");

        while (tokenizer.HasMoreTokens()) {
            wxString token = tokenizer.GetNextToken();

            if (token == "diskdef") {
                wxString type = tokenizer.GetNextToken();
                imageTypes.Add(type);
            }
        }
    }

    return (imageTypes);
}

// --------------------------------------------------------------------------------
