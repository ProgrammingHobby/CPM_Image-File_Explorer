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
// --------------------------------------------------------------------------------
BEGIN_EVENT_TABLE(MainWindow, wxFrame)
    EVT_MENU(wxID_CLOSE, MainWindow::onMenuCloseClicked)
    EVT_MENU(wxID_ABOUT, MainWindow::onMenuAboutClicked)
END_EVENT_TABLE()
// --------------------------------------------------------------------------------
MainWindow::MainWindow(wxWindow *parent) : Ui_MainWindow(parent) {

    int size = editImageFile->GetSize().GetHeight();
    buttonImageFile->SetMinSize(wxSize(size, size));
    buttonImageFile->SetMaxSize(wxSize(size, size));

    this->Fit();
    this->SetMinSize(this->GetSize());
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
