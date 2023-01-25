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
#include "wxXmlConfig.hpp"
#include "Version.h"
#include "FileCopySettingsDialog.hpp"
#include "ImagesHistory.hpp"
#include "diskdefs.hpp"
#include "FileDialogImageTypesPanel.hpp"
#include "GeneralSettingsDialog.hpp"
// --------------------------------------------------------------------------------
#include <wx/aboutdlg.h>
#include <wx/filedlg.h>
#include <wx/stdpaths.h>
#include <wx/msgdlg.h>
#include <wx/clipbrd.h>
#include <wx/dir.h>
// --------------------------------------------------------------------------------
BEGIN_EVENT_TABLE(MainWindow, wxFrame)
    EVT_ENTER_WINDOW(MainWindow::onEnterWindow)
    EVT_MENU(wxID_FILE_OPEN, MainWindow::onImageFileOpen)
    EVT_MENU(wxID_FILE_CLOSE, MainWindow::onImageFileClose)
    EVT_MENU(wxID_FILE_NEW, MainWindow::onImageFileNew)
    EVT_MENU(wxID_QUIT, MainWindow::onQuit)
    EVT_MENU(wxID_ABOUT, MainWindow::onAbout)
    EVT_MENU(wxID_REFRESH, MainWindow::onRefresh)
    EVT_MENU(wxID_SELECTALL, MainWindow::onSelectAll)
    EVT_MENU(wxID_DELETE, MainWindow::onDelete)
    EVT_MENU(wxID_EDIT, MainWindow::onRename)
    EVT_MENU(wxID_ATTRIBUTES, MainWindow::onAttributes)
    EVT_MENU(wxID_PROTECTIONS, MainWindow::onProtections)
    EVT_MENU(wxID_CREATE_NEW, MainWindow::onCreateNew)
    EVT_MENU(wxID_CHECK_IMAGE, MainWindow::onCheckImage)
    EVT_MENU(wxID_COPY_SETTINGS, MainWindow::onCopySettings)
    EVT_MENU(wxID_GENERAL_SETTINGS, MainWindow::onGeneralSettings)
    EVT_MENU(wxID_PASTE, MainWindow::onPasteFile)
    EVT_MENU(wxID_CLEAR_HISTORY, MainWindow::onClearHistory)
    EVT_BUTTON(wxID_BUTTON_CLEAR_MESSAGES, MainWindow::onClearMessages)
    EVT_BUTTON(wxID_BUTTON_SAVE_MESSAGES, MainWindow::onSaveMessages)
    EVT_LIST_ITEM_SELECTED(wxID_IMAGE_CONTENTS, MainWindow::onListItemSelected)
    EVT_LIST_ITEM_RIGHT_CLICK(wxID_IMAGE_CONTENTS, MainWindow::onListItemRightClick)
END_EVENT_TABLE()
// --------------------------------------------------------------------------------
MainWindow::MainWindow(wxWindow *parent, wxString appPath) : Ui_MainWindow(parent) {
    cpmguiinterface = new CpmGuiInterface(listImageContents, textMessages, textContentsInfo);
    cpmdevice = new CpmDevice();
    cpmfs = new CpmFs(cpmdevice, appPath.ToStdString());
    cpmtools = new CpmTools(cpmdevice, cpmfs, cpmguiinterface, appPath);
    wxXmlConfig *xmlconfig = new wxXmlConfig(wxEmptyString, wxEmptyString, "cife.xml",
            wxEmptyString, wxCONFIG_USE_LOCAL_FILE);
    wxConfigBase::Set(xmlconfig);
    config = wxConfigBase::Get();
    imageshistory = new ImagesHistory(menuRecentFiles, config);

    if (!imageshistory->load()) {
        menuRecentFiles->Enable(wxID_CLEAR_HISTORY, false);
    }

    for (int i = 0; i < 10; i++) {
        this->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainWindow::onSelectHistoryEntry, this,
                   wxID_FILE1 + i);
    }

    wxSize fontSize = this->GetFont().GetPixelSize();
    wxFont listFont = wxFont(fontSize, wxFontFamily::wxFONTFAMILY_TELETYPE,
                             wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_NORMAL);
    listImageContents->SetFont(listFont);
    textContentsInfo->SetFont(listFont);
    textMessages->SetFont(listFont);
    listImageContents->Bind(wxEVT_CONTEXT_MENU, &MainWindow::onShowContextMenu, this);
    listImageContents->Connect(wxEVT_DROP_FILES,
                               wxDropFilesEventHandler(MainWindow::onDropFiles), NULL, this);
    listImageContents->DragAcceptFiles(true);
    presetMenues();
    createPopupMenu();
    listImageContents->enableSizing(true);
    correctWindowSize();
    bool loadLastImage;
    config->Read("/General/OpenLastImageOnRestart", &loadLastImage, false);

    if (loadLastImage) {
        imageFile = imageshistory->getHistoryImageFile(0);

        if (imageFile.FileExists()) {
            loadImageFromHistory(0);
        }
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
    size.x = config->ReadLong("/MainWindow/SizeX", width);
    size.y = config->ReadLong("/MainWindow/SizeY", (height * 1.5));
    this->SetSize(size);
    wxPoint point;
    point.x = config->ReadLong("/MainWindow/PosX", 10);
    point.y = config->ReadLong("/MainWindow/PosY", 10);
    this->SetPosition(point);
    splitterImageViews->SetSashPosition(config->ReadLong("/MainWindow/SplitterPos", 466),
                                        true);
}

// --------------------------------------------------------------------------------
MainWindow::~MainWindow() {
    config->Write("/MainWindow/PosX", this->GetPosition().x);
    config->Write("/MainWindow/PosY", this->GetPosition().y);
    config->Write("/MainWindow/SizeX", this->GetSize().x);
    config->Write("/MainWindow/SizeY", this->GetSize().y);
    config->Write("/MainWindow/SplitterPos", splitterImageViews->GetSashPosition());

    imageshistory->save();

    wxDELETE(cpmguiinterface);
    wxDELETE(cpmtools);
    wxDELETE(cpmfs);
    wxDELETE(cpmdevice);
    wxDELETE(popupMenu);
    wxDELETE(imageshistory);
}

// --------------------------------------------------------------------------------
void MainWindow::onImageFileOpen(wxCommandEvent &event) {
    WXUNUSED(event)
    wxString dialogPath;

    do {
        if (imageFile.IsOk()) {
            dialogPath = imageFile.GetPath();
        }
        else {
            dialogPath = wxStandardPaths::Get().GetDocumentsDir();
        }

        wxFileDialog fileDialog(this, _("Open CP/M Disk Image File"),
                                dialogPath, imageFile.GetFullName(),
                                _("Image Files (*.img,*.fdd,*.dsk)|*.img;*.IMG;"
                                  "*.fdd;*.FDD;*.dsk;*.DSK|all Files (*.*)|*.*"),
                                wxFD_OPEN);
#if wxVERSION_NUMBER >= 3100
        ImageTypesHook dialogHook;
        fileDialog.SetCustomizeHook(dialogHook);
#else
        fileDialog.SetExtraControlCreator(&createFileDialogImageTypesPanel);
#endif

        if (fileDialog.ShowModal() == wxID_OK) {
#if wxVERSION_NUMBER >= 3100
            imageType = dialogHook.getImageType();
#else
            imageType = static_cast<FileDialogImageTypesPanel *>
                        (fileDialog.GetExtraControl())->getSelectedImageType();
#endif
            imageFile = fileDialog.GetPath();

            if (!imageType.IsEmpty()) {
                editImageFile->SetValue(imageFile.GetFullPath());
                editImageFile->SetInsertionPoint(imageFile.GetFullPath().length());
                editImageType->SetValue(imageType);
                imageshistory->addItem(imageFile.GetFullPath(), imageType);

                if (!menuRecentFiles->IsEnabled(wxID_CLEAR_HISTORY)) {
                    menuRecentFiles->Enable(wxID_CLEAR_HISTORY, true);
                }

                bool useUppercase;
                config->Read("/CpmFilesystem/UseUppercaseCharacters", &useUppercase, false);

                if (cpmtools->setImageType(imageType)
                        && cpmtools->openImage(imageFile.GetFullPath(), useUppercase)) {
                    isImageLoaded = true;
                    showDirectory();
                }

                break;
            }
            else {
                wxMessageDialog dialog(NULL, "\nNo Image-Type selected."
                                       "\n\nPlease select proper Image-Type"
                                       " to open the Image.",
                                       "Create new Image-File", wxOK | wxICON_QUESTION);
                dialog.ShowModal();
            }

        }
        else {
            break;
        }
    } while (imageType.IsEmpty());
}

// --------------------------------------------------------------------------------
void MainWindow::onImageFileClose(wxCommandEvent &event) {
    WXUNUSED(event)
    cpmtools->closeImage();
    editImageFile->Clear();
    editImageType->Clear();
    imageFile.ClearExt();
    listImageContents->DeleteAllItems();
    presetMenues();
    isImageLoaded = false;
}

// --------------------------------------------------------------------------------
void MainWindow::onImageFileNew(wxCommandEvent &event) {
    WXUNUSED(event)
    CreateFileDialog *dialog = new CreateFileDialog(this, config, cpmfs, cpmtools, true);
    dialog->setDefaultPath(imageshistory->getHistoryImageFile(0));

    if (dialog->ShowModal() == wxID_OK) {
        wxString fileName = dialog->getImageFileName();
        bool useUppercase;
        config->Read("/CpmFilesystem/UseUppercaseCharacters", &useUppercase, false);

        if (cpmtools->openImage(fileName, useUppercase)) {
            wxString imageType = dialog->getImageType();
            imageshistory->addItem(fileName, imageType);
            editImageFile->SetValue(fileName);
            editImageFile->SetInsertionPoint(imageFile.GetFullPath().length());
            editImageType->SetValue(imageType);
            isImageLoaded = true;
            showDirectory();
        }
        else {
            isImageLoaded = false;
        }
    }

    wxDELETE(dialog);
}

// --------------------------------------------------------------------------------
void MainWindow::onQuit(wxCommandEvent &event) {
    WXUNUSED(event)
    Close(true);
}

// --------------------------------------------------------------------------------
void MainWindow::onAbout(wxCommandEvent &event) {
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
                               "Source Code Version 2.23 from Michael Haardt."));
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
void MainWindow::onClearMessages(wxCommandEvent &event) {
    WXUNUSED(event)
    textMessages->Clear();
}

// --------------------------------------------------------------------------------
void MainWindow::onSaveMessages(wxCommandEvent &event) {
    WXUNUSED(event)
    wxFileDialog fileDialog(this, _("Save CIFE Messages"),
                            wxStandardPaths::Get().GetUserDataDir(),
                            "CIFE.msg", _("Text Files (*.txt,*.log)|*.txt;*.log|all Files (*.*)|*.*"),
                            wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (fileDialog.ShowModal() == wxID_OK) {
        textMessages->SaveFile(fileDialog.GetPath(), wxTEXT_TYPE_ANY);
    }
}

// --------------------------------------------------------------------------------
void MainWindow::onRefresh(wxCommandEvent &event) {
    WXUNUSED(event)
    showDirectory();
}

// --------------------------------------------------------------------------------
void MainWindow::onShowContextMenu(wxContextMenuEvent &event) {
    WXUNUSED(event)

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
    menuMainWindow->Enable(wxID_FILE_CLOSE, true);
    menuMainWindow->Enable(wxID_FILE_NEW, false);
    menuMainWindow->Enable(wxID_FILE_OPEN, false);

    if (listImageContents->GetItemCount() > 0) {
        menuMainWindow->Enable(wxID_SELECTALL, true);
    }
}

// --------------------------------------------------------------------------------
void MainWindow::onListItemSelected(wxListEvent &event) {
    WXUNUSED(event)

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
    WXUNUSED(event)
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
    WXUNUSED(event)
    listImageContents->SetItemState(-1, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
}

// --------------------------------------------------------------------------------
void MainWindow::onDelete(wxCommandEvent &event) {
    WXUNUSED(event)
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
        showDirectory();
    }
}

// --------------------------------------------------------------------------------
void MainWindow::onRename(wxCommandEvent &event) {
    WXUNUSED(event)
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
        showDirectory();
    }

    wxDELETE(dialog);
}

// --------------------------------------------------------------------------------
void MainWindow::onCreateNew(wxCommandEvent &event) {
    WXUNUSED(event)
    CreateFileDialog *dialog = new CreateFileDialog(this, config, cpmfs, cpmtools, false);
    dialog->setDefaultPath(imageFile.GetFullPath());

    if (dialog->ShowModal() == wxID_OK) {
        bool useUppercase;
        config->Read("/CpmFilesystem/UseUppercaseCharacters", &useUppercase, false);
        cpmtools->openImage(editImageFile->GetValue(), useUppercase);
        showDirectory();
    }

    wxDELETE(dialog);
}

// --------------------------------------------------------------------------------
void MainWindow::onCheckImage(wxCommandEvent &event) {
    WXUNUSED(event)
#ifdef __WINDOWS__
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
    WXUNUSED(event)
    FileCopySettingsDialog *dialog = new FileCopySettingsDialog(this, config);
    dialog->ShowModal();
    wxDELETE(dialog);
}

// --------------------------------------------------------------------------------
void MainWindow::onGeneralSettings(wxCommandEvent &event) {
    WXUNUSED(event)
    bool oldUppercase, newUppercase;
    config->Read("/CpmFilesystem/UseUppercaseCharacters", &oldUppercase, false);
    GeneralSettingsDialog *dialog = new GeneralSettingsDialog(this, config);
    dialog->ShowModal();
    wxDELETE(dialog);
    config->Read("/CpmFilesystem/UseUppercaseCharacters", &newUppercase, false);

    if (oldUppercase != newUppercase) {
        cpmfs->initDriveData(newUppercase);
        showDirectory();
    }
}


// --------------------------------------------------------------------------------
void MainWindow::onPasteFile(wxCommandEvent &event) {
    WXUNUSED(event)
    wxClipboard cifeClipboard;

    if (cifeClipboard.Open()) {
        if (cifeClipboard.IsSupported(wxDF_FILENAME)) {
            wxFileDataObject data;
            cifeClipboard.GetData(data);
            int defaultUserNumber = config->ReadLong("/CpmOptions/DefaultUserNumber", 0);
            wxString textFileEndings;
            config->Read("/CpmOptions/TextfileEndings", &textFileEndings, "txt pip pas");
            bool keepLastUpdatedTimestamp;
            config->Read("/CpmOptions/KeepLastUpdatedTimestamp", &keepLastUpdatedTimestamp,
                         false);
            wxArrayString pasted, files;
            pasted = data.GetFilenames();

            for (size_t i = 0; i < pasted.Count(); i++) {
                if (wxFileExists(pasted[i])) {
                    files.push_back(pasted[i]);
                }
                else if (wxDirExists(pasted[i])) {
                    wxDir::GetAllFiles(pasted[i], &files);
                }
            }

            for (size_t i = 0; i < files.Count(); i++) {
                wxFileName fileName(files[i]);
                wxString fileExt = fileName.GetExt();
                bool isTextFile = textFileEndings.Matches("*" + fileExt + "*");
                cpmtools->writeFileToImage(files[i], defaultUserNumber, isTextFile,
                                           keepLastUpdatedTimestamp);
            }

            showDirectory();
        }

        cifeClipboard.Close();
    }
}

// --------------------------------------------------------------------------------
void MainWindow::onClearHistory(wxCommandEvent &event) {
    WXUNUSED(event)
    imageshistory->clearHistory();
    menuRecentFiles->Enable(wxID_CLEAR_HISTORY, false);
}

// --------------------------------------------------------------------------------
void MainWindow::onSelectHistoryEntry(wxCommandEvent &event) {
    WXUNUSED(event)
    loadImageFromHistory(event.GetId() - wxID_FILE1);
}

// --------------------------------------------------------------------------------
void MainWindow::loadImageFromHistory(int historyId) {
    imageFile = imageshistory->getHistoryImageFile(historyId);
    imageType = imageshistory->getHistoryImageType(historyId);

    if (diskdefs::imageTypes.Index(imageType) == wxNOT_FOUND) {
        wxMessageDialog deleteDialog(NULL, "\nDo you want to delete the History Entry?",
                                     "Not a valid Image-Type !",
                                     wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);

        if (deleteDialog.ShowModal() == wxID_YES) {
            imageshistory->deleteItem(historyId);
        }

        return;
    }

    imageshistory->addItem(imageFile.GetFullPath(), imageType);

    if (isImageLoaded) {
        cpmtools->closeImage();
    }

    presetMenues();
    listImageContents->DeleteAllItems();

    if (imageFile.FileExists()) {
        editImageFile->SetValue(imageFile.GetFullPath());
        editImageFile->SetInsertionPoint(imageFile.GetFullPath().length());
        editImageType->SetValue(imageType);
        cpmtools->setImageType(imageType);
        bool useUppercase;
        config->Read("/CpmFilesystem/UseUppercaseCharacters", &useUppercase, false);
        cpmtools->openImage(imageFile.GetFullPath(), useUppercase);
        isImageLoaded = true;
        showDirectory();
    }
    else {
        isImageLoaded = false;
    }
}

// --------------------------------------------------------------------------------
void MainWindow::onAttributes(wxCommandEvent &event) {
    WXUNUSED(event)
    FileAttributesDialog *dialog = new FileAttributesDialog(this);
    long index = listImageContents->GetFirstSelected();
    wxString name = listImageContents->GetItemText(index, 0);
    name.Replace(" ", "");
    dialog->setAttributes(listImageContents->GetItemText(index, 3));

    if (dialog->ShowModal() == wxID_OK) {
        cpmtools->setFileAttributes(name, dialog->getAttributes());
        showDirectory();
    }

    wxDELETE(dialog);
}

// --------------------------------------------------------------------------------
void MainWindow::onProtections(wxCommandEvent &event) {
    WXUNUSED(event)
    FileProtectionsDialog *dialog = new FileProtectionsDialog(this);
    long index = listImageContents->GetFirstSelected();
    wxString name = listImageContents->GetItemText(index, 0);
    name.Replace(" ", "");
    dialog->setProtections(listImageContents->GetItemText(index, 4));

    if (dialog->ShowModal() == wxID_OK) {
        cpmtools->setFileProtections(name, dialog->getProtections());
        showDirectory();
    }

    wxDELETE(dialog);
}

// --------------------------------------------------------------------------------
void MainWindow::onEnterWindow(wxMouseEvent &event) {
    WXUNUSED(event)
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
void MainWindow::onDropFiles(wxDropFilesEvent &event) {
    WXUNUSED(event)

    if (event.GetNumberOfFiles() > 0) {
        wxString *dropped = event.GetFiles();
        wxASSERT(dropped);
        wxBusyCursor busyCursor;
        wxWindowDisabler disabler;
        wxArrayString files;

        for (int i = 0; i < event.GetNumberOfFiles(); i++) {
            if (wxFileExists(dropped[i])) {
                files.push_back(dropped[i]);
            }
            else if (wxDirExists(dropped[i])) {
                wxDir::GetAllFiles(dropped[i], &files);
            }
        }

        int defaultUserNumber = config->ReadLong("/CpmOptions/DefaultUserNumber", 0);
        wxString textFileEndings;
        config->Read("/CpmOptions/TextfileEndings", &textFileEndings, "txt pip pas");
        bool keepLastUpdatedTimestamp;
        config->Read("/CpmOptions/KeepLastUpdatedTimestamp", &keepLastUpdatedTimestamp,
                     false);

        for (size_t i = 0; i < files.Count(); i++) {
            wxFileName fileName(files[i]);
            wxString fileExt = fileName.GetExt();
            bool isTextFile = textFileEndings.Matches("*" + fileExt + "*");
            cpmtools->writeFileToImage(files[i], defaultUserNumber, isTextFile,
                                       keepLastUpdatedTimestamp);
        }

        showDirectory();
    }
}

// --------------------------------------------------------------------------------
