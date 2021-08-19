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
#include "Version.h"
#include "CpmTools.h"
#include "CpmGuiInterface.h"
// --------------------------------------------------------------------------------
#include <wx/aboutdlg.h>
#include <wx/platinfo.h>
#include <wx/versioninfo.h>
#include <wx/filedlg.h>
#include <wx/stdpaths.h>
#include <wx/txtstrm.h>
#include <wx/wfstream.h>
#include <wx/tokenzr.h>
#include <wx/bitmap.h>
#include <wx/icon.h>
#include <wx/settings.h>
// --------------------------------------------------------------------------------
BEGIN_EVENT_TABLE(MainWindow, wxFrame)
    EVT_MENU(wxID_CLOSE, MainWindow::onMenuCloseClicked)
    EVT_MENU(wxID_ABOUT, MainWindow::onMenuAboutClicked)
    EVT_BUTTON(wxID_BUTTON_IMAGE_FILE, MainWindow::onButtonImageFileClicked)
    EVT_BUTTON(wxID_BUTTON_UPDATE_DIR, MainWindow::onButtonUpdateDirClicked)
    EVT_BUTTON(wxID_BUTTON_CLEAR_MESSAGES, MainWindow::onButtonClearMessagesClicked)
    EVT_BUTTON(wxID_BUTTON_SAVE_MESSAGES, MainWindow::onButtonSaveMessagesClicked)
    EVT_COMBOBOX_DROPDOWN(wxID_IMAGE_TYPE, MainWindow::onComboBoxImageTypeDropDown)
    EVT_COMBOBOX(wxID_IMAGE_TYPE, MainWindow::onImageTypeChanged)
END_EVENT_TABLE()
// --------------------------------------------------------------------------------
#include "Bitmaps/appiconsmall.xpm"
// --------------------------------------------------------------------------------
MainWindow::MainWindow(wxWindow *parent) : Ui_MainWindow(parent) {
    int size = editImageFile->GetSize().GetHeight();
    buttonImageFile->SetMinSize(wxSize(size, size));
    buttonImageFile->SetMaxSize(wxSize(size, size));
    wxBitmap iconBmp(appiconsmall_xpm);

    if (iconBmp.IsOk()) {
        wxIcon icn;
        icn.CopyFromBitmap(iconBmp);
        SetIcon(icn);
    }

    comboboxImageType->Append(getImageTypes());
    comboboxImageType->SetSelection(3);     // nur für Test, ansonsten 0
    comboboxDirViewType->SetSelection(0);   // nur für Test, entfällt ansonsten
    editImageFile->SetFocus();
    buttonUpdateDir->Enable(false);
    wxSize fontSize = this->GetFont().GetPixelSize();
    wxFont listFont = wxFont(fontSize, wxFontFamily::wxFONTFAMILY_TELETYPE, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_NORMAL);
    listImageContents->SetFont(listFont);
    cpmguiinterface = new CpmGuiInterface(listImageContents, textMessages, textContentsInfo);
    cpmtools = new CpmTools(cpmguiinterface);
    correctWindowSize();
}

// --------------------------------------------------------------------------------
void MainWindow::correctWindowSize() {
    this->SetMinSize(wxSize(0, 0));
    int width = this->GetBestSize().GetWidth();
    width += wxSystemSettings::GetMetric(wxSYS_VSCROLL_X, listImageContents);
    int height = this->GetBestSize().GetHeight();
    this->SetSize(wxSize(width, height));
    this->SetMinSize(wxSize(width, height));
}

// --------------------------------------------------------------------------------
MainWindow::~MainWindow() {
    if (cpmguiinterface != nullptr) {
        delete cpmguiinterface;
        cpmguiinterface = nullptr;
    }

    if (cpmtools != nullptr) {
        delete cpmtools;
        cpmtools = nullptr;
    }
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
    aboutInfo.SetVersion(VERSION_STRING);
    aboutInfo.SetIcon(this->GetIcon());
    aboutInfo.SetDescription(_("Written in C/C++ with CodeLite-IDE\n"
                               "Using wxWidgets GUI - Framework Version ") + versionInfo.GetVersionString() +
                             _("\n\nCP/M Images Functionality based on the CP/M-Tools\n"
                               "Source Code Version 2.21 from Michael Haardt.\n"
                               "http://www.moria.de/~michael/cpmtools"));
    aboutInfo.SetCopyright("Uwe Merker  (C) 2021");
    aboutInfo.SetWebSite("http://www.moria.de/~michael/cpmtools\n"
                         "https://github.com/ProgrammingHobby/CPM_Image-File_Explorer.git");
    wxBitmap iconBmp(appiconsmall_xpm);

    if (iconBmp.IsOk()) {
        wxIcon icn;
        icn.CopyFromBitmap(iconBmp);
        aboutInfo.SetIcon(icn);
    }

    wxAboutBox(aboutInfo, this);
}

// --------------------------------------------------------------------------------
void MainWindow::onButtonImageFileClicked(wxCommandEvent &event) {
    WXUNUSED(event)
//    wxFileDialog fileDialog(this, _T("Open CP/M Disk Image File"), wxStandardPaths::Get().GetUserDataDir(),
//                            wxEmptyString, _T("Image Files (*.img,*.fdd,*.dsk)|*.img;*.fdd;*.dsk|all Files (*.*)|*.*"),
//                            wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    wxFileDialog fileDialog(this, _T("Open CP/M Disk Image File"), "/home/uwe/Programming/wxWidgets/Projekte/CPM_Image-File_Explorer/",
                            wxEmptyString, _T("Image Files (*.img,*.fdd,*.dsk)|*.img;*.fdd;*.dsk|all Files (*.*)|*.*"),
                            wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (fileDialog.ShowModal() == wxID_OK) {
        editImageFile->SetValue(fileDialog.GetPath());
        editImageFile->SetInsertionPoint(editImageFile->GetValue().length());
//        cpmtools->setImageFile(fileDialog.GetPath());
//        cpmtools->showDirectory();
        correctWindowSize();
        buttonUpdateDir->Enable(true);
    }
    else {
        buttonUpdateDir->Enable(false);
    }
}

// --------------------------------------------------------------------------------
void MainWindow::onComboBoxImageTypeDropDown(wxCommandEvent &event) {
    WXUNUSED(event)
    comboboxImageType->Clear();
    comboboxImageType->Append(getImageTypes());
}

// --------------------------------------------------------------------------------
void MainWindow::onImageTypeChanged(wxCommandEvent &event) {
    WXUNUSED(event)
//    cpmtools->setImageType(comboboxImageType->GetValue());
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
void MainWindow::onButtonUpdateDirClicked(wxCommandEvent &event) {
    WXUNUSED(event)
    listImageContents->ClearAll();
//    cpmtools->showDirectory();
    correctWindowSize();
}

// --------------------------------------------------------------------------------
void MainWindow::onButtonClearMessagesClicked(wxCommandEvent &event) {
    textMessages->Clear();
}

// --------------------------------------------------------------------------------
void MainWindow::onButtonSaveMessagesClicked(wxCommandEvent &event) {
    wxFileDialog fileDialog(this, _T("Save CIFE Messages"), wxStandardPaths::Get().GetUserDataDir(),
                            wxEmptyString, _T("Text Files (*.txt,*.log)|*.txt;*.log|all Files (*.*)|*.*"),
                            wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (fileDialog.ShowModal() == wxID_OK) {
        textMessages->SaveFile(fileDialog.GetPath(), wxTEXT_TYPE_ANY);
    }
}

// --------------------------------------------------------------------------------
