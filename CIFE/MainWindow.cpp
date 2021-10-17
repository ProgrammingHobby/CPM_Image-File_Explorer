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
#include <wx/msgdlg.h>
// --------------------------------------------------------------------------------
BEGIN_EVENT_TABLE(MainWindow, wxFrame)
    EVT_MENU(wxID_CLOSE, MainWindow::onMenuCloseClicked)
    EVT_MENU(wxID_ABOUT, MainWindow::onMenuAboutClicked)
    EVT_MENU(wxID_REFRESH, MainWindow::onViewRefresh)
    EVT_MENU(wxID_SELECTALL, MainWindow::onSelectAll)
    EVT_MENU(wxID_DELETE, MainWindow::onDelete)
    EVT_BUTTON(wxID_BUTTON_IMAGE_FILE, MainWindow::onButtonImageFileClicked)
    EVT_BUTTON(wxID_BUTTON_CLEAR_MESSAGES, MainWindow::onButtonClearMessagesClicked)
    EVT_BUTTON(wxID_BUTTON_SAVE_MESSAGES, MainWindow::onButtonSaveMessagesClicked)
    EVT_COMBOBOX(wxID_IMAGE_TYPE, MainWindow::onImageTypeChanged)
    EVT_LIST_ITEM_SELECTED(wxID_IMAGE_CONTENTS, MainWindow::onListItemSelected)
    EVT_LIST_ITEM_RIGHT_CLICK(wxID_IMAGE_CONTENTS, MainWindow::onListItemRightClick)
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
    comboboxImageType->SetSelection(3); //========== nur für Debug ==========//
    editImageFile->SetFocus();
    //
    wxSize fontSize = this->GetFont().GetPixelSize();
    wxFont listFont = wxFont(fontSize, wxFontFamily::wxFONTFAMILY_TELETYPE, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_NORMAL);
    listImageContents->SetFont(listFont);
    textContentsInfo->SetFont(listFont);
    //
    listImageContents->Bind(wxEVT_CONTEXT_MENU, &MainWindow::onShowContextMenu, this);
    //
    cpmguiinterface = new CpmGuiInterface(listImageContents, textMessages, textContentsInfo);
    cpmtools = new CpmTools(cpmguiinterface);
    cpmtools->setImageType(comboboxImageType->GetValue());
    //
    isImageLoaded = false;
    correctWindowSize();
    presetMenues();
    //
    //========== nur für Debug ==========//
    wxString filePath = "/home/uwe/Programming/wxWidgets/Projekte/CPM_Image-File_Explorer/Test_Disk-Images/Test_1440kb.fdd";
    editImageFile->SetValue(filePath);
    editImageFile->SetInsertionPoint(filePath.length());
    cpmtools->setImageFile(filePath);
    isImageLoaded = true;
    listImageContents->DeleteAllItems();
    cpmtools->showDirectory();
    listImageContents->SetFocus();
    presetMenues();
    menuMainWindow->Enable(wxID_REFRESH, true);
    menuMainWindow->Enable(wxID_CREATE_NEW, true);
    menuMainWindow->Enable(wxID_CHECK_IMAGE, true);
    menuMainWindow->Enable(wxID_PASTE, true);
    //========== nur für Debug ==========//

    if (listImageContents->GetItemCount() > 0) {
        menuMainWindow->Enable(wxID_SELECTALL, true);
    }
}

// --------------------------------------------------------------------------------
void MainWindow::correctWindowSize() {
    this->SetMinSize(wxSize(0, 0));
    int width = this->GetBestSize().GetWidth();
    width += wxSystemSettings::GetMetric(wxSYS_VSCROLL_X, listImageContents);
    int height = this->GetBestSize().GetHeight();
    this->SetSize(wxSize(width, (height * 1.5)));
    this->SetMinSize(wxSize(width, (height * 1.5)));
}

// --------------------------------------------------------------------------------
MainWindow::~MainWindow() {
    wxDELETE(cpmguiinterface);
    wxDELETE(cpmtools);
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
                               "Source Code Version 2.21 from Michael Haardt."));
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
    wxFileDialog fileDialog(this, _("Open CP/M Disk Image File"), wxStandardPaths::Get().GetUserDataDir(),
                            wxEmptyString, _("Image Files (*.img,*.fdd,*.dsk)|*.img;*.fdd;*.dsk|all Files (*.*)|*.*"),
                            wxFD_OPEN);

    if (fileDialog.ShowModal() == wxID_OK) {
        wxString filePath = fileDialog.GetPath();
        editImageFile->SetValue(filePath);
        editImageFile->SetInsertionPoint(filePath.length());
        cpmtools->setImageFile(filePath);

        if (!wxFileExists(filePath)) {
            // Meldung Image nicht vorhanden, wird neu erstellt
        }

        isImageLoaded = true;
        listImageContents->DeleteAllItems();
        cpmtools->showDirectory();
        listImageContents->SetFocus();
        presetMenues();
        menuMainWindow->Enable(wxID_REFRESH, true);
        menuMainWindow->Enable(wxID_CREATE_NEW, true);
        menuMainWindow->Enable(wxID_CHECK_IMAGE, true);
        menuMainWindow->Enable(wxID_PASTE, true);

        if (listImageContents->GetItemCount() > 0) {
            menuMainWindow->Enable(wxID_SELECTALL, true);
        }
    }
}

// --------------------------------------------------------------------------------
void MainWindow::onImageTypeChanged(wxCommandEvent &event) {
    WXUNUSED(event)
    cpmtools->setImageType(comboboxImageType->GetValue());
}

// --------------------------------------------------------------------------------
wxArrayString MainWindow::getImageTypes() {
    wxArrayString imageTypes;
    wxFileInputStream file(wxT("diskdefs"));
    wxTextInputStream text(file);
    int diskdefsCount = 0;

    while (!file.Eof()) {
        wxString line = text.ReadLine();
        wxStringTokenizer tokenizer(line, " ");

        while (tokenizer.HasMoreTokens()) {
            wxString token = tokenizer.GetNextToken();

            if (token == "diskdef") {
                wxString type = tokenizer.GetNextToken();
                imageTypes.Add(type);
                diskdefsCount++;
            }
        }
    }

    textDiskdefsCount->SetLabel(wxString::Format(_("%d Disk definitions found."), diskdefsCount));
    return (imageTypes);
}

// --------------------------------------------------------------------------------
void MainWindow::onButtonClearMessagesClicked(wxCommandEvent &event) {
    textMessages->Clear();
}

// --------------------------------------------------------------------------------
void MainWindow::onButtonSaveMessagesClicked(wxCommandEvent &event) {
    wxFileDialog fileDialog(this, _("Save CIFE Messages"), wxStandardPaths::Get().GetUserDataDir(),
                            wxEmptyString, _("Text Files (*.txt,*.log)|*.txt;*.log|all Files (*.*)|*.*"),
                            wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (fileDialog.ShowModal() == wxID_OK) {
        textMessages->SaveFile(fileDialog.GetPath(), wxTEXT_TYPE_ANY);
    }
}

// --------------------------------------------------------------------------------
void MainWindow::onViewRefresh(wxCommandEvent &event) {
    listImageContents->DeleteAllItems();
    cpmtools->showDirectory();
    listImageContents->SetFocus();
}

// --------------------------------------------------------------------------------
void MainWindow::onShowContextMenu(wxContextMenuEvent &event) {
    if (isImageLoaded) {
        wxMenu popupMenu;
        popupMenu.Append(wxID_REFRESH, _("Refresh\tF5"), wxT(""), wxITEM_NORMAL);
        popupMenu.AppendSeparator();
        popupMenu.Append(wxID_CUT, _("Cut\tCtrl+X"), wxT(""), wxITEM_NORMAL)->Enable(false);
        popupMenu.Append(wxID_COPY, _("Copy\tCtrl+C"), wxT(""), wxITEM_NORMAL)->Enable(false);
        popupMenu.Append(wxID_PASTE, _("Paste\tCtrl+V"), wxT(""), wxITEM_NORMAL);
        popupMenu.Append(wxID_SELECTALL, _("Select all\tCtrl+A"), wxT(""), wxITEM_NORMAL)->Enable(false);
        popupMenu.AppendSeparator();
        popupMenu.Append(wxID_EDIT, _("Rename\tF2"), wxT(""), wxITEM_NORMAL)->Enable(false);
        popupMenu.Append(wxID_DELETE, _("Delete\tDel"), wxT(""), wxITEM_NORMAL)->Enable(false);
        popupMenu.AppendSeparator();
        popupMenu.Append(wxID_PERMISSIONS, _("Permissions\tF7"), wxT(""), wxITEM_NORMAL)->Enable(false);
        popupMenu.Append(wxID_ATTRIBUTES, _("Attributes\tF9"), wxT(""), wxITEM_NORMAL)->Enable(false);
        popupMenu.AppendSeparator();
        popupMenu.Append(wxID_CHECK_IMAGE, _("Check Image\tF11"), wxT(""), wxITEM_NORMAL);

        if (listImageContents->GetItemCount() > 0) {
            popupMenu.Enable(wxID_SELECTALL, true);
        }

        if (listImageContents->GetSelectedItemCount() > 0) {
            popupMenu.Enable(wxID_CUT, true);
            popupMenu.Enable(wxID_COPY, true);
            popupMenu.Enable(wxID_DELETE, true);
            popupMenu.Enable(wxID_PERMISSIONS, true);
            popupMenu.Enable(wxID_ATTRIBUTES, true);
        }

        if (listImageContents->GetSelectedItemCount() == 1) {
            popupMenu.Enable(wxID_EDIT, true);
        }

        listImageContents->PopupMenu(&popupMenu);
    }
}

// --------------------------------------------------------------------------------
void MainWindow::presetMenues() {
    menuMainWindow->Enable(wxID_CUT, false);
    menuMainWindow->Enable(wxID_COPY, false);
    menuMainWindow->Enable(wxID_PASTE, false);
    menuMainWindow->Enable(wxID_SELECTALL, false);
    menuMainWindow->Enable(wxID_EDIT, false);
    menuMainWindow->Enable(wxID_DELETE, false);
    menuMainWindow->Enable(wxID_PERMISSIONS, false);
    menuMainWindow->Enable(wxID_ATTRIBUTES, false);
    menuMainWindow->Enable(wxID_CREATE_NEW, false);
    menuMainWindow->Enable(wxID_REFRESH, false);
    menuMainWindow->Enable(wxID_CHECK_IMAGE, false);
}

// --------------------------------------------------------------------------------
void MainWindow::onListItemSelected(wxListEvent &event) {
    if (listImageContents->GetSelectedItemCount() > 0) {
        menuMainWindow->Enable(wxID_CUT, true);
        menuMainWindow->Enable(wxID_COPY, true);
        menuMainWindow->Enable(wxID_DELETE, true);
        menuMainWindow->Enable(wxID_PERMISSIONS, true);
        menuMainWindow->Enable(wxID_ATTRIBUTES, true);
    }
    else {
        menuMainWindow->Enable(wxID_CUT, false);
        menuMainWindow->Enable(wxID_COPY, false);
        menuMainWindow->Enable(wxID_DELETE, false);
        menuMainWindow->Enable(wxID_PERMISSIONS, false);
        menuMainWindow->Enable(wxID_ATTRIBUTES, false);
    }

    if (listImageContents->GetSelectedItemCount() == 1) {
        menuMainWindow->Enable(wxID_EDIT, true);
    }
    else {
        menuMainWindow->Enable(wxID_EDIT, false);
    }
}

// --------------------------------------------------------------------------------
void MainWindow::onListItemRightClick(wxListEvent &event) {
    long index = event.GetIndex();
    long nextIndex = listImageContents->GetNextSelected(index);
    long firstIndex = listImageContents->GetFirstSelected();
    long chkIndex = firstIndex;
    long lastIndex = firstIndex;

    while (chkIndex != -1) {
        lastIndex = chkIndex;
        chkIndex = listImageContents->GetNextSelected(chkIndex);
    }

    if (((index == firstIndex) && (nextIndex == lastIndex)) || ((index == lastIndex) && (nextIndex == -1) && (index == (firstIndex + 1)))) {
        if (nextIndex == -1)  {
            listImageContents->SetItemState(firstIndex, 0, wxLIST_STATE_SELECTED);
        }
        else {
            listImageContents->SetItemState(nextIndex, 0, wxLIST_STATE_SELECTED);
        }

        if (index != -1) {
            listImageContents->SetItemState(index, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
        }
    }
}

// --------------------------------------------------------------------------------
void MainWindow::onSelectAll(wxCommandEvent &event) {
    listImageContents->SetItemState(-1, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
}

// --------------------------------------------------------------------------------
void MainWindow::onDelete(wxCommandEvent &event) {
    wxMessageDialog deleteDialog(this, _("Are you sure you want\ndelete selected File/s ?"),
                                 _("Delete File/s"), wxYES_NO | wxYES_DEFAULT | wxICON_QUESTION);

    if (deleteDialog.ShowModal() == wxID_YES) {
        long index = listImageContents->GetFirstSelected();
        wxArrayString files;

        while (index != -1) {
            wxString file = listImageContents->GetItemText(index);
            file.Replace(" ", "");
            files.Add(file);
            index = listImageContents->GetNextSelected(index);
        }

        cpmtools->deleteFile(files);
        onViewRefresh(event);
    }
}
// --------------------------------------------------------------------------------
