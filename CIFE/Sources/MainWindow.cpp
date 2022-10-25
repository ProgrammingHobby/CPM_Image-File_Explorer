/*
 * MainWindow.cpp is part of CIFE
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

#include "MainWindow.hpp"
#include "CpmGuiInterface.hpp"
#include "CpmTools.hpp"
#include "CreateFileDialog.hpp"
#include "FileAttributesDialog.hpp"
#include "FileProtectionsDialog.hpp"
#include "RenameFileDialog.hpp"
#include "Settings.hpp"
#include "Version.h"
#include "FileCopySettingsDialog.hpp"
// --------------------------------------------------------------------------------
#include <wx/aboutdlg.h>
#include <wx/versioninfo.h>
#include <wx/filedlg.h>
#include <wx/stdpaths.h>
#include <wx/txtstrm.h>
#include <wx/wfstream.h>
#include <wx/tokenzr.h>
#include <wx/bitmap.h>
#include <wx/icon.h>
#include <wx/msgdlg.h>
#include <wx/datetime.h>
#include <wx/clipbrd.h>
#include <wx/filename.h>
// --------------------------------------------------------------------------------
BEGIN_EVENT_TABLE(MainWindow, wxFrame)
    EVT_ENTER_WINDOW(MainWindow::onEnterWindow)
    EVT_MENU(wxID_FILE_OPEN, MainWindow::onMenuImageFileOpen)
    EVT_MENU(wxID_FILE_CLOSE, MainWindow::onMenuImageFileClose)
    EVT_MENU(wxID_FILE_NEW, MainWindow::onMenuNewImageFile)
    EVT_MENU(wxID_QUIT, MainWindow::onMenuCloseClicked)
    EVT_MENU(wxID_ABOUT, MainWindow::onMenuAboutClicked)
    EVT_MENU(wxID_REFRESH, MainWindow::onViewRefresh)
    EVT_MENU(wxID_SELECTALL, MainWindow::onSelectAll)
    EVT_MENU(wxID_DELETE, MainWindow::onDelete)
    EVT_MENU(wxID_EDIT, MainWindow::onRename)
    EVT_MENU(wxID_ATTRIBUTES, MainWindow::onAttributes)
    EVT_MENU(wxID_PROTECTIONS, MainWindow::onProtections)
    EVT_MENU(wxID_CREATE_NEW, MainWindow::onCreateNew)
    EVT_MENU(wxID_CHECK_IMAGE, MainWindow::onCheckImage)
    EVT_MENU(wxID_COPY_SETTINGS, MainWindow::onCopySettings)
    EVT_MENU(wxID_PASTE, MainWindow::onPasteFile)
    EVT_BUTTON(wxID_BUTTON_CLEAR_MESSAGES, MainWindow::onButtonClearMessagesClicked)
    EVT_BUTTON(wxID_BUTTON_SAVE_MESSAGES, MainWindow::onButtonSaveMessagesClicked)
    EVT_COMBOBOX(wxID_IMAGE_TYPE, MainWindow::onImageTypeChanged)
    EVT_LIST_ITEM_SELECTED(wxID_IMAGE_CONTENTS, MainWindow::onListItemSelected)
    EVT_LIST_ITEM_RIGHT_CLICK(wxID_IMAGE_CONTENTS, MainWindow::onListItemRightClick)
END_EVENT_TABLE()
// --------------------------------------------------------------------------------
MainWindow::MainWindow(wxWindow *parent, wxString appPath) : Ui_MainWindow(parent) {
    cpmguiinterface = new CpmGuiInterface(listImageContents, textMessages, textContentsInfo);
    cpmdevice = new CpmDevice();
    cpmfs = new CpmFs(cpmdevice, appPath.ToStdString());
    cpmtools = new CpmTools(cpmdevice, cpmfs, cpmguiinterface, appPath);
    cifeSettings = new Settings("cife.conf");
    comboboxImageType->Append(getImageTypes(appPath));
    comboboxImageType->SetSelection(0);
    editImageFile->SetFocus();
    wxSize fontSize = this->GetFont().GetPixelSize();
    wxFont listFont = wxFont(fontSize, wxFontFamily::wxFONTFAMILY_TELETYPE,
                             wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_NORMAL);
    listImageContents->SetFont(listFont);
    textContentsInfo->SetFont(listFont);
    textMessages->SetFont(listFont);
    listImageContents->Bind(wxEVT_CONTEXT_MENU, &MainWindow::onShowContextMenu, this);
    comboboxImageType->SetSelection(cifeSettings->readInteger("CpmSettings", "ImageType", 0));
    cpmtools->setImageType(comboboxImageType->GetValue());
    wxString filePath = cifeSettings->readString("CpmSettings", "ImageFile", "");
    presetMenues();
    createPopupMenu();
    listImageContents->enableSizing(true);
    correctWindowSize();

    if (wxFileExists(filePath)) {
        editImageFile->SetValue(filePath);
        editImageFile->SetInsertionPoint(filePath.length());
        cpmtools->setImageType(comboboxImageType->GetValue());
        cpmtools->openImage(filePath);
        isImageLoaded = true;
        showDirectory();
        menuMainWindow->Enable(wxID_FILE_CLOSE, true);
        menuMainWindow->Enable(wxID_FILE_NEW, false);
        menuMainWindow->Enable(wxID_FILE_OPEN, false);
    }
    else {
        isImageLoaded = false;
    }

    if (listImageContents->GetItemCount() > 0) {
        menuMainWindow->Enable(wxID_SELECTALL, true);
    }
}

// --------------------------------------------------------------------------------
void MainWindow::createPopupMenu() {
    popupMenu = new wxMenu();
    wxMenuItem *popupItemRefresh = new wxMenuItem(popupMenu, wxID_REFRESH, _("Refresh\tF5"),
            wxT(""), wxITEM_NORMAL);
    popupItemRefresh->SetBitmap(wxXmlResource::Get()->LoadBitmap(wxT("refresh")));
    popupMenu->Append(popupItemRefresh);
    popupMenu->AppendSeparator();
    wxMenuItem *popupItemCut = new wxMenuItem(popupMenu, wxID_CUT, _("Cut\tCtrl+X"), wxT(""),
            wxITEM_NORMAL);
    popupItemCut->SetBitmap(wxXmlResource::Get()->LoadBitmap(wxT("cut")));
    popupMenu->Append(popupItemCut);
    wxMenuItem *popupItemCopy = new wxMenuItem(popupMenu, wxID_COPY, _("Copy\tCtrl+C"),
            wxT(""), wxITEM_NORMAL);
    popupItemCopy->SetBitmap(wxXmlResource::Get()->LoadBitmap(wxT("copy")));
    popupMenu->Append(popupItemCopy);
    wxMenuItem *popupItemPaste = new wxMenuItem(popupMenu, wxID_PASTE, _("Paste\tCtrl+V"),
            wxT(""), wxITEM_NORMAL);
    popupItemPaste->SetBitmap(wxXmlResource::Get()->LoadBitmap(wxT("paste")));
    popupMenu->Append(popupItemPaste);
    wxMenuItem *popupItemSelectAll = new wxMenuItem(popupMenu, wxID_SELECTALL,
            _("Select all\tCtrl+A"), wxT(""), wxITEM_NORMAL);
    popupItemSelectAll->SetBitmap(wxXmlResource::Get()->LoadBitmap(wxT("select_all")));
    popupMenu->Append(popupItemSelectAll);
    popupMenu->AppendSeparator();
    wxMenuItem *popupItemRename = new wxMenuItem(popupMenu, wxID_EDIT, _("Rename\tF2"),
            wxT(""), wxITEM_NORMAL);
    popupItemRename->SetBitmap(wxXmlResource::Get()->LoadBitmap(wxT("edit")));
    popupMenu->Append(popupItemRename);
    wxMenuItem *popupItemDelete = new wxMenuItem(popupMenu, wxID_DELETE, _("Delete\tDel"),
            wxT(""), wxITEM_NORMAL);
    popupItemDelete->SetBitmap(wxXmlResource::Get()->LoadBitmap(wxT("delete")));
    popupMenu->Append(popupItemDelete);
    popupMenu->AppendSeparator();
    wxMenuItem *popupItemAttributes = new wxMenuItem(popupMenu, wxID_ATTRIBUTES,
            _("Attributes\tF7"), wxT(""), wxITEM_NORMAL);
    popupItemAttributes->SetBitmap(wxXmlResource::Get()->LoadBitmap(wxT("attributes")));
    popupMenu->Append(popupItemAttributes);
    wxMenuItem *popupItemProtections = new wxMenuItem(popupMenu, wxID_PROTECTIONS,
            _("Protections\tF9"), wxT(""), wxITEM_NORMAL);
    popupItemProtections->SetBitmap(wxXmlResource::Get()->LoadBitmap(wxT("permissions")));
    popupMenu->Append(popupItemProtections);
    popupMenu->AppendSeparator();
    wxMenuItem *popupItemCheckImage = new wxMenuItem(popupMenu, wxID_CHECK_IMAGE,
            _("Check Image\tF11"), wxT(""), wxITEM_NORMAL);
    popupItemCheckImage->SetBitmap(wxXmlResource::Get()->LoadBitmap(wxT("check_image")));
    popupMenu->Append(popupItemCheckImage);
}

// --------------------------------------------------------------------------------
void MainWindow::correctWindowSize() {
    this->SetMinSize(wxSize(0, 0));
    int width = this->GetBestSize().GetWidth();
    width += wxSystemSettings::GetMetric(wxSYS_VSCROLL_X, listImageContents);
    int height = this->GetBestSize().GetHeight();
    this->SetMinSize(wxSize(width, (height * 1.5)));
    wxSize size;
    size.x = cifeSettings->readInteger("MainWindow", "SizeX", width);
    size.y = cifeSettings->readInteger("MainWindow", "SizeY", (height * 1.5));
    this->SetSize(size);
    wxPoint point;
    point.x = cifeSettings->readInteger("MainWindow", "PosX", 10);
    point.y = cifeSettings->readInteger("MainWindow", "PosY", 10);
    this->SetPosition(point);
}

// --------------------------------------------------------------------------------
MainWindow::~MainWindow() {
    cifeSettings->writeInteger("MainWindow", "PosX", this->GetPosition().x);
    cifeSettings->writeInteger("MainWindow", "PosY", this->GetPosition().y);
    cifeSettings->writeInteger("MainWindow", "SizeX", this->GetSize().x);
    cifeSettings->writeInteger("MainWindow", "SizeY", this->GetSize().y);

    cifeSettings->writeInteger("CpmSettings", "ImageType", comboboxImageType->GetSelection());
    cifeSettings->writeString("CpmSettings", "ImageFile", editImageFile->GetValue());

    wxDELETE(cpmguiinterface);
    wxDELETE(cpmtools);
    wxDELETE(cpmfs);
    wxDELETE(cpmdevice);
    wxDELETE(cifeSettings);
    wxDELETE(popupMenu);
}

// --------------------------------------------------------------------------------
void MainWindow::onMenuImageFileOpen(wxCommandEvent &event) {
    WXUNUSED(event) wxFileDialog fileDialog(this, _("Open CP/M Disk Image File"),
                                            wxStandardPaths::Get().GetUserDataDir(), wxEmptyString,
                                            _("Image Files (*.img,*.fdd,*.dsk)|*.img;*.IMG;"
                                                    "*.fdd;*.FDD;*.dsk;*.DSK|all Files (*.*)|*.*"),
                                            wxFD_OPEN);

    if (fileDialog.ShowModal() == wxID_OK) {
        wxString filePath = fileDialog.GetPath();
        editImageFile->SetValue(filePath);
        editImageFile->SetInsertionPoint(filePath.length());
        cpmtools->openImage(filePath);
        isImageLoaded = true;
        showDirectory();
        menuMainWindow->Enable(wxID_FILE_CLOSE, true);
        menuMainWindow->Enable(wxID_FILE_NEW, false);
        menuMainWindow->Enable(wxID_FILE_OPEN, false);
    }
}

// --------------------------------------------------------------------------------
void MainWindow::onMenuImageFileClose(wxCommandEvent &event) {
    cpmtools->closeImage();
    editImageFile->SetValue("");
    listImageContents->DeleteAllItems();
    presetMenues();
    isImageLoaded = false;
}

// --------------------------------------------------------------------------------
void MainWindow::onMenuNewImageFile(wxCommandEvent &event) {
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
    wxDateTime datetime;
    wxVersionInfo versionInfo("", wxMAJOR_VERSION, wxMINOR_VERSION, wxRELEASE_NUMBER);
    aboutInfo.SetName("CP/M Image-File Explorer");
    aboutInfo.SetVersion(VERSION_STRING);
    aboutInfo.SetDescription(_("Written in C/C++ with Eclipse CDT.\n"
                               "Using wxWidgets GUI - Framework Version ") + versionInfo.GetVersionString() +
                             wxString::Format(" - %dbit", (int)(sizeof(nullptr) * 8)) +
                             _("\n\nCP/M Images Functionality based on CP/M-Tools\n"
                               "Source Code Version 2.21 from Michael Haardt."));
    aboutInfo.SetCopyright(wxString::Format("Uwe Merker  (C) %d", datetime.GetCurrentYear()));
    aboutInfo.SetWebSite("http://www.moria.de/~michael/cpmtools");
    wxBitmap iconBmp = wxXmlResource::Get()->LoadBitmap(wxT("appiconsmall"));

    if (iconBmp.IsOk()) {
        wxIcon icn;
        icn.CopyFromBitmap(iconBmp);
        aboutInfo.SetIcon(icn);
    }

    wxAboutBox(aboutInfo, this);
}

// --------------------------------------------------------------------------------
void MainWindow::onImageTypeChanged(wxCommandEvent &event) {
    WXUNUSED(event)
    cpmtools->setImageType(comboboxImageType->GetValue());

    if (isImageLoaded) {
        showDirectory();
    }
}

// --------------------------------------------------------------------------------
wxArrayString MainWindow::getImageTypes(wxString appPath) {
    wxArrayString imageTypes;
    wxFileInputStream file(appPath + "diskdefs");
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

    textDiskdefsCount->SetLabel(wxString::Format(_("%d Disk definitions found."),
                                diskdefsCount));
    return (imageTypes);
}

// --------------------------------------------------------------------------------
void MainWindow::onButtonClearMessagesClicked(wxCommandEvent &event) {
    textMessages->Clear();
}

// --------------------------------------------------------------------------------
void MainWindow::onButtonSaveMessagesClicked(wxCommandEvent &event) {
    wxFileDialog fileDialog(this, _("Save CIFE Messages"),
                            wxStandardPaths::Get().GetUserDataDir(),
                            "CIFE.msg", _("Text Files (*.txt,*.log)|*.txt;*.log|all Files (*.*)|*.*"),
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

        if (listImageContents->GetItemCount() == 0) {
            popupMenu->Enable(wxID_SELECTALL, false);
        }
        else {
            popupMenu->Enable(wxID_SELECTALL, true);
        }

        if (listImageContents->GetSelectedItemCount() == 0) {
            popupMenu->Enable(wxID_CUT, false);
            popupMenu->Enable(wxID_COPY, false);
            popupMenu->Enable(wxID_DELETE, false);
            popupMenu->Enable(wxID_ATTRIBUTES, false);
            popupMenu->Enable(wxID_PROTECTIONS, false);
        }
        else {
            popupMenu->Enable(wxID_CUT, true);
            popupMenu->Enable(wxID_COPY, true);
            popupMenu->Enable(wxID_DELETE, true);
            popupMenu->Enable(wxID_ATTRIBUTES, true);
            popupMenu->Enable(wxID_PROTECTIONS, true);
        }

        if (listImageContents->GetSelectedItemCount() != 1) {
            popupMenu->Enable(wxID_EDIT, false);
            popupMenu->Enable(wxID_ATTRIBUTES, false);
            popupMenu->Enable(wxID_PROTECTIONS, false);
        }
        else {
            popupMenu->Enable(wxID_EDIT, true);
            popupMenu->Enable(wxID_ATTRIBUTES, true);
            popupMenu->Enable(wxID_PROTECTIONS, true);
        }

        listImageContents->PopupMenu(popupMenu);
    }
}

// --------------------------------------------------------------------------------
void MainWindow::presetMenues() {
    menuMainWindow->Enable(wxID_FILE_CLOSE, false);
    menuMainWindow->Enable(wxID_FILE_NEW, true);
    menuMainWindow->Enable(wxID_FILE_OPEN, true);
    menuMainWindow->Enable(wxID_CUT, false);
    menuMainWindow->Enable(wxID_COPY, false);
    menuMainWindow->Enable(wxID_PASTE, false);
    menuMainWindow->Enable(wxID_SELECTALL, false);
    menuMainWindow->Enable(wxID_EDIT, false);
    menuMainWindow->Enable(wxID_DELETE, false);
    menuMainWindow->Enable(wxID_ATTRIBUTES, false);
    menuMainWindow->Enable(wxID_PROTECTIONS, false);
    menuMainWindow->Enable(wxID_CREATE_NEW, false);
    menuMainWindow->Enable(wxID_REFRESH, false);
    menuMainWindow->Enable(wxID_CHECK_IMAGE, false);
}

// --------------------------------------------------------------------------------
void MainWindow::showDirectory() {
    listImageContents->DeleteAllItems();
    cpmtools->showDirectory();
    listImageContents->SetFocus();
    presetMenues();
    menuMainWindow->Enable(wxID_REFRESH, true);
    menuMainWindow->Enable(wxID_CREATE_NEW, true);
    menuMainWindow->Enable(wxID_CHECK_IMAGE, true);

    if (listImageContents->GetItemCount() > 0) {
        menuMainWindow->Enable(wxID_SELECTALL, true);
    }
}

// --------------------------------------------------------------------------------
void MainWindow::onListItemSelected(wxListEvent &event) {
    if (listImageContents->GetSelectedItemCount() > 0) {
        menuMainWindow->Enable(wxID_CUT, true);
        menuMainWindow->Enable(wxID_COPY, true);
        menuMainWindow->Enable(wxID_DELETE, true);
    }
    else {
        menuMainWindow->Enable(wxID_CUT, false);
        menuMainWindow->Enable(wxID_COPY, false);
        menuMainWindow->Enable(wxID_DELETE, false);
    }

    if (listImageContents->GetSelectedItemCount() == 1) {
        menuMainWindow->Enable(wxID_ATTRIBUTES, true);
        menuMainWindow->Enable(wxID_PROTECTIONS, true);
        menuMainWindow->Enable(wxID_EDIT, true);
    }
    else {
        menuMainWindow->Enable(wxID_ATTRIBUTES, false);
        menuMainWindow->Enable(wxID_PROTECTIONS, false);
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

    if (((index == firstIndex) && (nextIndex == lastIndex)) || ((index == lastIndex)
            && (nextIndex == -1) && (index == (firstIndex + 1)))) {
        if (nextIndex == -1) {
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
    wxMessageDialog deleteDialog(NULL, _("Are you sure you want delete selected File/s ?"),
                                 _("Delete File/s"), wxYES_NO | wxYES_DEFAULT | wxICON_WARNING);

    if (deleteDialog.ShowModal() == wxID_YES) {
        long index = listImageContents->GetFirstSelected();
        wxArrayString files;

        while (index != -1) {
            wxString file = listImageContents->GetItemText(index, 0);
            file.Replace(" ", "");
            files.Add(file);
            index = listImageContents->GetNextSelected(index);
        }

        cpmtools->deleteFile(files);
        onViewRefresh(event);
    }
}

// --------------------------------------------------------------------------------
void MainWindow::onRename(wxCommandEvent &event) {
    RenameFileDialog *dialog = new RenameFileDialog(this);
    long index = listImageContents->GetFirstSelected();
    wxString oldName = listImageContents->GetItemText(index, 0);
    oldName.Replace(" ", "");
    wxString name = oldName.AfterLast(':');
    wxString user = oldName.BeforeFirst(':');
    dialog->setOldName(name);
    dialog->setOldUser(wxAtoi(user));

    if (dialog->ShowModal() == wxID_OK) {
        wxString newName = dialog->getNewName();
        int newUser = dialog->getNewUser();
        newName = wxString::Format(("%i"), newUser) + ":" + newName;
        cpmtools->renameFile(oldName, newName);
        onViewRefresh(event);
    }

    wxDELETE(dialog);
}

// --------------------------------------------------------------------------------
void MainWindow::onCreateNew(wxCommandEvent &event) {
    CreateFileDialog *dialog = new CreateFileDialog(this, cpmfs->getBootTracksEnabled());
    dialog->setImageFileName(editImageFile->GetValue());

    if (dialog->ShowModal() == wxID_OK) {
        cpmtools->createNewImage(editImageFile->GetValue(), dialog->getFileSystemLabel(),
                                 dialog->useTimestamps(), dialog->getBootTrackFile());
        cpmtools->openImage(editImageFile->GetValue());
        onViewRefresh(event);
    }

    wxDELETE(dialog);
}

// --------------------------------------------------------------------------------
void MainWindow::onCheckImage(wxCommandEvent &event) {
#ifdef _WINDOWS_
    wxMessageDialog deleteDialog(NULL, "Repair Filesystem Errors ?", "Check CP/M Image",
                                 wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
#else
    wxMessageDialog deleteDialog(NULL, "\nRepair Filesystem Errors ?", "Check CP/M Image",
                                 wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
#endif

    if (deleteDialog.ShowModal() == wxID_YES) {
        cpmtools->checkImage(true);
    }
    else {
        cpmtools->checkImage(false);
    }
}

// --------------------------------------------------------------------------------
void MainWindow::onCopySettings(wxCommandEvent &event) {
    FileCopySettingsDialog *dialog = new FileCopySettingsDialog(this, cifeSettings);

    if (dialog->ShowModal() == wxID_OK) {

    }

    wxDELETE(dialog);
}

// --------------------------------------------------------------------------------
void MainWindow::onPasteFile(wxCommandEvent &event) {
    wxClipboard cifeClipboard;

    if (cifeClipboard.Open()) {
        if (cifeClipboard.IsSupported(wxDF_FILENAME)) {
            wxFileDataObject data;
            cifeClipboard.GetData(data);
            int defaultUserNumber = cifeSettings->readInteger("CpmOptions", "DefaultUserNumber", 0);
            wxString textFileEndings = cifeSettings->readString("CpmOptions", "TextfileEndings",
                                       "txt pip pas");
            wxArrayString files = data.GetFilenames();

            for (size_t i = 0; i < files.Count(); i++) {
                wxFileName fileName(files[i]);
                wxString fileExt = fileName.GetExt();
                bool isTextFile = textFileEndings.Matches("*" + fileExt + "*");
                cpmtools->writeFileToImage(files[i], defaultUserNumber, isTextFile, true);
            }

            onViewRefresh(event);
        }

        cifeClipboard.Close();
    }
}

// --------------------------------------------------------------------------------
void MainWindow::onAttributes(wxCommandEvent &event) {
    FileAttributesDialog *dialog = new FileAttributesDialog(this);
    long index = listImageContents->GetFirstSelected();
    wxString name = listImageContents->GetItemText(index, 0);
    name.Replace(" ", "");
    dialog->setAttributes(listImageContents->GetItemText(index, 3));

    if (dialog->ShowModal() == wxID_OK) {
        cpmtools->setFileAttributes(name, dialog->getAttributes());
        onViewRefresh(event);
    }

    wxDELETE(dialog);
}

// --------------------------------------------------------------------------------
void MainWindow::onProtections(wxCommandEvent &event) {
    FileProtectionsDialog *dialog = new FileProtectionsDialog(this);
    long index = listImageContents->GetFirstSelected();
    wxString name = listImageContents->GetItemText(index, 0);
    name.Replace(" ", "");
    dialog->setProtections(listImageContents->GetItemText(index, 4));

    if (dialog->ShowModal() == wxID_OK) {
        cpmtools->setFileProtections(name, dialog->getProtections());
        onViewRefresh(event);
    }

    wxDELETE(dialog);
}

// --------------------------------------------------------------------------------
void MainWindow::onEnterWindow(wxMouseEvent &event) {
    wxClipboard cifeClipboard;

    if (cifeClipboard.Open()) {
        if (cifeClipboard.IsSupported(wxDF_FILENAME)) {
            menuMainWindow->Enable(wxID_PASTE, true);
            popupMenu->Enable(wxID_PASTE, true);
        }
        else {
            menuMainWindow->Enable(wxID_PASTE, false);
            popupMenu->Enable(wxID_PASTE, false);
        }
    }
    else {
        menuMainWindow->Enable(wxID_PASTE, false);
        popupMenu->Enable(wxID_PASTE, false);
    }
}

// --------------------------------------------------------------------------------
