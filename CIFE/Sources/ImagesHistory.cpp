
/*
 * imageshistory.cpp is part of CIFE
 * Copyright (c) 2022  Uwe Merker
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

#include "ImagesHistory.hpp"
#include "wxXmlConfig.hpp"
// --------------------------------------------------------------------------------
#include <wx/menu.h>
#include <wx/filename.h>
// --------------------------------------------------------------------------------
void ImagesHistory::clearHistory() {
    wxString oldPath = m_Config->GetPath();
    int itemsCount = m_Config->ReadLong("/ImagesHistory/EntrysCount", 0);

    if (itemsCount > 0) {
        m_ImageFiles.Clear();
        m_ImageTypes.Clear();
    }

    for (int i = 0; i < itemsCount; i++) {
        m_Config->SetPath(wxString::Format("/ImagesHistory/Image%01d", i + 1));
        m_Config->DeleteEntry("File", false);
        m_Config->DeleteEntry("Type", true);
        m_RecentMenu->Delete(m_MenuItemId + i);
    }

    m_Config->SetPath(oldPath);
}

// --------------------------------------------------------------------------------
void ImagesHistory::addItem(wxString file, wxString type) {
    int itemsCount = m_ImageFiles.GetCount();

    // check if the given Image is already in History
    for (int i = 0; i < itemsCount; i++) {
        if (file == m_ImageFiles[i]) {
            removeHistoryItem(i);
            itemsCount--;
            break;
        }
    }

    // if History full, delete last item
    if (itemsCount == MAXITEMS) {
        removeHistoryItem(--itemsCount);
    }

    m_RecentMenu->Insert(itemsCount, (m_MenuItemId + itemsCount), " ");
    m_ImageFiles.insert(m_ImageFiles.begin(), file);
    m_ImageTypes.insert(m_ImageTypes.begin(), type);
    refreshMenuLabels();
}

// --------------------------------------------------------------------------------
wxString ImagesHistory::getActualImageFile() {
    return (getHistoryImageFile(0));
}

// --------------------------------------------------------------------------------
wxString ImagesHistory::getActualImageType() {
    return (getHistoryImageType(0));
}

// --------------------------------------------------------------------------------
wxString ImagesHistory::getHistoryImageFile(int item) {
    return (m_ImageFiles.GetCount() ?  m_ImageFiles[item] : "");
}

// --------------------------------------------------------------------------------
wxString ImagesHistory::getHistoryImageType(int item) {
    return (m_ImageTypes.GetCount() ?  m_ImageTypes[item] : "");
}

// --------------------------------------------------------------------------------
bool ImagesHistory::load() {
    wxString oldPath = m_Config->GetPath();
    int itemsCount = m_Config->ReadLong("/ImagesHistory/EntrysCount", 0);
    bool historyLoaded = false;

    if (itemsCount == 0) {
        return (historyLoaded);
    }

    for (int i = 0; i < itemsCount; i++) {
        m_Config->SetPath(wxString::Format("/ImagesHistory/Image%01d", i + 1));
        wxString file, type;

        if ((m_Config->Read("File", &file, wxEmptyString))
                && (m_Config->Read("Type", &type, wxEmptyString))) {
            m_ImageFiles.Add(file);
            m_ImageTypes.Add(type);
            wxFileName fileName(file);
            wxString menuLabel = fileName.GetFullName();
            m_RecentMenu->Insert(i, (m_MenuItemId + i), recentMenuLabel(i, menuLabel));;
            m_RecentMenu->SetHelpString(m_MenuItemId + i, file);
            historyLoaded = true;
        }
    }

    m_Config->SetPath(oldPath);
    return (historyLoaded);
}

// --------------------------------------------------------------------------------
void ImagesHistory::save() {
    wxString oldPath = m_Config->GetPath();
    int itemsCount = m_ImageFiles.GetCount();
    m_Config->Write("/ImagesHistory/EntrysCount", itemsCount);

    for (int i = 0; i < MAXITEMS; i++) {
        m_Config->SetPath(wxString::Format("/ImagesHistory/Image%01d", i + 1));

        if (i < itemsCount) {
            m_Config->Write("File", m_ImageFiles[i]);
            m_Config->Write("Type", m_ImageTypes[i]);
        }
    }

    for (int i = itemsCount; i < MAXITEMS; i++) {
        m_Config->SetPath(wxString::Format("/ImagesHistory/Image%01d", i + 1));
        m_Config->DeleteEntry("File", false);
        m_Config->DeleteEntry("Type", true);
    }

    m_Config->SetPath(oldPath);
}

// --------------------------------------------------------------------------------
ImagesHistory::ImagesHistory(wxMenu *menu, wxConfigBase *config) {
    m_RecentMenu = menu;
    m_Config = config;
}

// --------------------------------------------------------------------------------
ImagesHistory::~ImagesHistory() {
    // TODO Auto-generated destructor stub
}

// --------------------------------------------------------------------------------
wxString ImagesHistory::recentMenuLabel(int num, wxString file) {
    wxString pathInMenu(file);
    pathInMenu.Replace("&", "&&");
    return (wxString::Format("&%d %s", num + 1, pathInMenu));
}

// --------------------------------------------------------------------------------
void ImagesHistory::removeHistoryItem(int item) {
    int itemsCount = m_ImageFiles.GetCount();

    // delete Items
    m_ImageFiles.RemoveAt(item);
    m_ImageTypes.RemoveAt(item);
    itemsCount--;

    // shift filenames up
    for (int j = item; j < itemsCount; j++) {
        m_RecentMenu->SetLabel(m_MenuItemId + j, recentMenuLabel(j, m_ImageFiles[j]));
        m_RecentMenu->SetHelpString(m_MenuItemId + j, m_ImageFiles[j]);
    }

    // delete the last menu item which is unused now
    const wxWindowID lastItemId = m_MenuItemId + itemsCount;

    if (m_RecentMenu->FindItem(lastItemId)) {
        m_RecentMenu->Delete(lastItemId);
    }

}

// --------------------------------------------------------------------------------
void ImagesHistory::refreshMenuLabels() {
    int itemsCount = m_ImageFiles.GetCount();

    // If no files, then no need to refresh the menu
    if (itemsCount == 0) {
        return;
    }

    // Update the labels in all menus
    for (int i = 0; i < itemsCount; i++) {
        wxFileName fileName(m_ImageFiles[i]);
        wxString menuLabel = fileName.GetFullName();
        m_RecentMenu->SetLabel(m_MenuItemId + i, recentMenuLabel(i, menuLabel));
        m_RecentMenu->SetHelpString(m_MenuItemId + i, m_ImageFiles[i]);
    }
}

// --------------------------------------------------------------------------------
