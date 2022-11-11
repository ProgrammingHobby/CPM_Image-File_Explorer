/*
 * Settings.cpp is part of CIFE
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

#include "Settings.hpp"
// --------------------------------------------------------------------------------
#include <wx/filename.h>
#include <wx/log.h>
#include <wx/stdpaths.h>
// --------------------------------------------------------------------------------
Settings::Settings(wxString settingsFileName) {

    wxString configPath = wxStandardPaths::Get().GetUserLocalDataDir();

    if (!wxDirExists(configPath)) {
        wxMkDir(configPath, wxS_DIR_DEFAULT);
    }

    settingsFile = configPath + wxFileName::GetPathSeparator() + settingsFileName;

    if (!wxFile::Exists(settingsFile)) {
        xmlSettings.Save(settingsFile);
    }

    wxLogNull SilenceExclamationmark;

    if (!xmlSettings.Load(settingsFile)) {
        xmlSettings.SetRoot(new wxXmlNode(wxXML_ELEMENT_NODE, "settings"));
        xmlSettings.Save(settingsFile);
    }
}

// --------------------------------------------------------------------------------
Settings::~Settings() {
    xmlSettings.Save(settingsFile);
}

// --------------------------------------------------------------------------------
wxXmlNode *Settings::findNode(wxXmlNode *rootNode, wxString name) {
    wxXmlNode *node = rootNode->GetChildren();

    while (node) {
        if (node->GetName() == name) {
            break;
        }

        node = node->GetNext();
    }

    return (node);
}

// --------------------------------------------------------------------------------
void Settings::writeInteger(wxString section, wxString key, int data) {
    wxXmlNode *sectionNode = findNode(xmlSettings.GetRoot(), section);

    if (!sectionNode) {
        sectionNode = new wxXmlNode(xmlSettings.GetRoot(), wxXML_ELEMENT_NODE, section);
    }

    wxXmlNode *keyNode = findNode(sectionNode, key);

    if (!keyNode) {
        keyNode = new wxXmlNode(sectionNode, wxXML_ELEMENT_NODE, key);
        keyNode->AddAttribute("int", wxString::Format(wxT("%d"), data));
    }
    else {
        wxXmlAttribute *attr = new wxXmlAttribute("int", wxString::Format(wxT("%d"), data));
        keyNode->SetAttributes(attr);
    }

    xmlSettings.Save(settingsFile);
}

// --------------------------------------------------------------------------------
void Settings::writeString(wxString section, wxString key, wxString data) {
    wxXmlNode *sectionNode = findNode(xmlSettings.GetRoot(), section);

    if (!sectionNode) {
        sectionNode = new wxXmlNode(xmlSettings.GetRoot(), wxXML_ELEMENT_NODE, section);
    }

    wxXmlNode *keyNode = findNode(sectionNode, key);

    if (!keyNode) {
        keyNode = new wxXmlNode(sectionNode, wxXML_ELEMENT_NODE, key);
        keyNode->AddAttribute("data", wxString::Format(wxT("%s"), data));
    }
    else {
        wxXmlAttribute *attr = new wxXmlAttribute("data", wxString::Format(wxT("%s"), data));
        keyNode->SetAttributes(attr);
    }

    xmlSettings.Save(settingsFile);
}

// --------------------------------------------------------------------------------
void Settings::writeBoolean(wxString section, wxString key, bool data) {
    wxXmlNode *sectionNode = findNode(xmlSettings.GetRoot(), section);

    if (!sectionNode) {
        sectionNode = new wxXmlNode(xmlSettings.GetRoot(), wxXML_ELEMENT_NODE, section);
    }

    wxXmlNode *keyNode = findNode(sectionNode, key);

    if (!keyNode) {
        keyNode = new wxXmlNode(sectionNode, wxXML_ELEMENT_NODE, key);
        keyNode->AddAttribute("bool", wxString::Format(wxT("%s"), ((data) ? "true" : "false")));
    }
    else {
        wxXmlAttribute *attr = new wxXmlAttribute("bool", wxString::Format(wxT("%s"),
                ((data) ? "true" : "false")));
        keyNode->SetAttributes(attr);
    }

    xmlSettings.Save(settingsFile);
}

// --------------------------------------------------------------------------------
int Settings::readInteger(wxString section, wxString key, int defaultData) {
    int data = defaultData;
    wxXmlNode *sectionNode = nullptr, *keyNode = nullptr;
    sectionNode = findNode(xmlSettings.GetRoot(), section);

    if (sectionNode) {
        keyNode = findNode(sectionNode, key);
    }

    if (keyNode) {
        wxString nodeData = keyNode->GetAttribute("int", wxString::Format(wxT("%d"),
                            defaultData));

        if (!nodeData.IsEmpty() && nodeData.IsNumber()) {
            data = wxAtoi(nodeData);
        }
    }

    return (data);
}

// --------------------------------------------------------------------------------
wxString Settings::readString(wxString section, wxString key, wxString defaultData) {
    wxString data = defaultData;
    wxXmlNode *sectionNode = nullptr, *keyNode = nullptr;
    sectionNode = findNode(xmlSettings.GetRoot(), section);

    if (sectionNode) {
        keyNode = findNode(sectionNode, key);
    }

    if (keyNode) {
        wxString nodeData = keyNode->GetAttribute("data", wxString::Format(wxT("%s"),
                            defaultData));

        if (!nodeData.IsEmpty() && nodeData.IsAscii()) {
            data = nodeData;
        }
    }

    return (data);
}

// --------------------------------------------------------------------------------
bool Settings::readBoolean(wxString section, wxString key, bool defaultData) {
    bool data = defaultData;
    wxXmlNode *sectionNode = nullptr, *keyNode = nullptr;
    sectionNode = findNode(xmlSettings.GetRoot(), section);

    if (sectionNode) {
        keyNode = findNode(sectionNode, key);
    }

    if (keyNode) {
        wxString nodeData = keyNode->GetAttribute("bool", wxString::Format(wxT("%s"),
                            ((defaultData) ? "true" : "false")));

        if (!nodeData.IsEmpty() && !nodeData.IsNumber()) {
            if (nodeData == "true") {
                data = true;
            }
            else if (nodeData == "false") {
                data = false;
            }
        }
    }

    return (data);
}

// --------------------------------------------------------------------------------
void Settings::removeSection(wxString section) {
    wxXmlNode *sectionNode = nullptr;
    sectionNode = findNode(xmlSettings.GetRoot(), section);

    if (sectionNode) {
        xmlSettings.GetRoot()->RemoveChild(sectionNode);
        xmlSettings.Save(settingsFile);
    }
}

// --------------------------------------------------------------------------------
void Settings::removeKey(wxString section, wxString key) {
    wxXmlNode *sectionNode = nullptr, *keyNode = nullptr;
    sectionNode = findNode(xmlSettings.GetRoot(), section);

    if (sectionNode) {
        keyNode = findNode(sectionNode, key);
    }

    if (keyNode) {
        sectionNode->RemoveChild(keyNode);
        xmlSettings.Save(settingsFile);
    }
}

// --------------------------------------------------------------------------------
