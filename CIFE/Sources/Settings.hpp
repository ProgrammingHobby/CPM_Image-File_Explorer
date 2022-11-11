/*
 * Settings.hpp is part of CIFE
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

#ifndef SETTINGS_H
#define SETTINGS_H
// --------------------------------------------------------------------------------
#include <wx/xml/xml.h>
// --------------------------------------------------------------------------------
class Settings {
    public: // Attributes

    public: // Methods
        void writeInteger(wxString section, wxString key, int data);
        void writeString(wxString section, wxString key, wxString data);
        void writeBoolean(wxString section, wxString key, bool data);
        int readInteger(wxString section, wxString key, int defaultData);
        wxString readString(wxString section, wxString key, wxString defaultData);
        bool readBoolean(wxString section, wxString key, bool defaultData);
        void removeSection(wxString section);
        void removeKey(wxString section, wxString key);

    public: // Constructor & Destructor
        Settings(wxString settingsFileName);
        virtual ~Settings();

    protected: // Attributes

    protected: // Methods

    private: // Attributes
        wxString settingsFile;
        wxXmlDocument xmlSettings;

    private: // Methods
        wxXmlNode *findNode(wxXmlNode *rootNode, wxString name);
};

// --------------------------------------------------------------------------------
#endif // SETTINGS_H
