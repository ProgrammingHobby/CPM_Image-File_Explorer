/*
 * imageshistory.hpp is part of CIFE
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

#ifndef IMAGESHISTORY_HPP_
#define IMAGESHISTORY_HPP_
// --------------------------------------------------------------------------------
#include <wx/object.h>
#include <wx/arrstr.h>
// --------------------------------------------------------------------------------
class wxConfigBase;
class wxMenu;
// --------------------------------------------------------------------------------
class ImagesHistory: public wxObject {
    public: // Attributes

    public: // Methods
        void clearHistory();
        void addItem(wxString file, int type);
        wxString getActualImageFile();
        int getActualImageType();
        wxString getHistoryImageFile(int item);
        int getHistoryImageType(int item);
        bool load();
        void save();

    public: // Constructor & Destructor
        ImagesHistory(wxMenu *menu, wxConfigBase *config);
        virtual ~ImagesHistory();

    protected: // Attributes

    protected: // Methods

    private: // Attributes
        const int MAXITEMS = 10;
        wxWindowID m_MenuItemId = wxID_FILE1;
        wxArrayString m_ImageFiles;
        wxArrayString m_ImageTypes;
        wxMenu *m_RecentMenu;
        wxConfigBase *m_Config;

    private: // Methods
        wxString recentMenuLabel(int num, wxString file);
        void removeHistoryItem(int item);
        void refreshMenuLabels();
};

// --------------------------------------------------------------------------------
#endif /* IMAGESHISTORY_HPP_ */
