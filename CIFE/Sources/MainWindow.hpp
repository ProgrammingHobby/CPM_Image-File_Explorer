/*
 * MainWindow.hpp is part of CIFE
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H
// --------------------------------------------------------------------------------
#include <wx/arrstr.h>

#include "Ui_MainWindow.hpp"
// --------------------------------------------------------------------------------
class wxMenu;
class CpmTools;
class CpmGuiInterface;
class Settings;
// --------------------------------------------------------------------------------
class MainWindow : public Ui_MainWindow {
    public:     // Attributes

    public:     // Methods

    public:     // Constructor & Destructor
        MainWindow(wxWindow *parent, wxString appPath);
        virtual ~MainWindow();

    protected:  // Event Methods
        void onMenuCloseClicked(wxCommandEvent &event);
        void onMenuAboutClicked(wxCommandEvent &event);
        void onButtonImageFileClicked(wxCommandEvent &event);
        void onButtonClearMessagesClicked(wxCommandEvent &event);
        void onButtonSaveMessagesClicked(wxCommandEvent &event);
        void onImageTypeChanged(wxCommandEvent &event);
        void onViewRefresh(wxCommandEvent &event);
        void onSelectAll(wxCommandEvent &event);
        void onDelete(wxCommandEvent &event);
        void onRename(wxCommandEvent &event);
        void onAttributes(wxCommandEvent &event);
        void onProtections(wxCommandEvent &event);
        void onCreateNew(wxCommandEvent &event);
        void onCheckImage(wxCommandEvent &event);
        void onCopySettings(wxCommandEvent &event);
        void onPasteFile(wxCommandEvent &event);
        void onShowContextMenu(wxContextMenuEvent &event);
        void onListItemSelected(wxListEvent &event);
        void onListItemRightClick(wxListEvent &event);
        void onEnterWindow(wxMouseEvent &event);

    private:    // Attributes
        wxMenu *popupMenu;
        Settings *cifeSettings;
        CpmTools *cpmtools;
        CpmGuiInterface *cpmguiinterface;
        bool isImageLoaded;

    private:    // Methods
        wxArrayString getImageTypes(wxString appPath);
        void createPopupMenu();
        void correctWindowSize();
        void presetMenues();
        void showDirectory();

        DECLARE_EVENT_TABLE()
};

// --------------------------------------------------------------------------------
#endif // MAINWINDOW_H