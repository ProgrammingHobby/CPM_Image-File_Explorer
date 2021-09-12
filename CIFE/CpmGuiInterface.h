/***********************************************************************************
**
**	Copyright (C) 19.08.2021 Uwe Merker Germany
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

#ifndef CPMGUIINTERFACE_HPP
#define CPMGUIINTERFACE_HPP
// --------------------------------------------------------------------------------
class wxListView;
class wxTextCtrl;
// --------------------------------------------------------------------------------
#include <wx/string.h>
// --------------------------------------------------------------------------------
class CpmGuiInterface {
    public:     // Attributes
        enum msgColor {msgColRed, msgColGreen, msgColBlue, msgColBlack};

    public:     // Methods
        void printMsg(wxString msg, msgColor col = msgColBlack);
        void printDirInfo(wxString info);
        void setDirHeader();
        void printDirEntry(int col, int row, wxString data);

    public:     // Constructor & Destructor
        CpmGuiInterface(wxListView *listView, wxTextCtrl *textCtrl, wxTextCtrl *statText);
        ~CpmGuiInterface();

    protected:  // Event Methods

    private:    // Attributes
        wxTextCtrl *textMessages;
        wxListView *listContents;
        wxTextCtrl *textDirInfo;

    private:    // Methods

};

// --------------------------------------------------------------------------------
#endif // CPMGUIINTERFACE_HPP
