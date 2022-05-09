/***********************************************************************************
**
**	Copyright (C) 09.05.2022 Uwe Merker Germany
**
**	Diese Datei ist Teil von CPM_Image-File_Explorer.
**
**	CPM_Image-File_Explorer ist Freie Software: Sie können es unter den Bedingungen
**	der GNU General Public License, wie von der Free Software Foundation,
**	Version 3 der Lizenz oder (nach Ihrer Wahl) jeder späteren
**	veröffentlichten Version, weiterverbreiten und/oder modifizieren.
**
**	CPM_Image-File_Explorer wird in der Hoffnung, dass es nützlich sein wird, aber
**	OHNE JEDE GEWÄHRLEISTUNG, bereitgestellt; sogar ohne die implizite
**	Gewährleistung der MARKTFÄHIGKEIT oder EIGNUNG FÜR EINEN BESTIMMTEN ZWECK.
**	Siehe die GNU General Public License für weitere Details.
**
**	Sie sollten eine Kopie der GNU General Public License zusammen mit diesem
**	Programm erhalten haben. Wenn nicht, siehe <http://www.gnu.org/licenses/>.
**
***********************************************************************************/

#ifndef SETTINGS_H
#define SETTINGS_H
// --------------------------------------------------------------------------------
#include <wx/xml/xml.h>
// --------------------------------------------------------------------------------
class Settings {
    public:
        //////////// Public Attributes ///////////

        ////////////// Public Methods ////////////
        void writeInteger(wxString section, wxString key, int data);
        void writeString(wxString section, wxString key, wxString data);
        void writeBoolean(wxString section, wxString key, bool data);
        int readInteger(wxString section, wxString key, int defaultData);
        wxString readString(wxString section, wxString key, wxString defaultData);
        bool readBoolean(wxString section, wxString key, bool defaultData);
        void removeSection(wxString section);
        void removeKey(wxString section, wxString key);

        ///// Public Constructor & Destructor ////
        Settings(wxString settingsFileName);
        virtual ~Settings();

    protected:
        //////// Protected Event Methods /////////

    private:
        /////////// Private Attributes ///////////
        wxString settingsFile;
        wxXmlDocument xmlSettings;

        ///////////// Private Methods ////////////
        wxXmlNode *findNode(wxXmlNode *rootNode, wxString name);
};

// --------------------------------------------------------------------------------
#endif // SETTINGS_H
