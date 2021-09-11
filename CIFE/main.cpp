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

#include <wx/wx.h>
#include <wx/file.h>
#include <wx/msgdlg.h>
// --------------------------------------------------------------------------------
#include "MainWindow.h"
// --------------------------------------------------------------------------------
// application class
class wxCifeApp : public wxApp {
    public:
        virtual bool OnInit();
};

// --------------------------------------------------------------------------------
IMPLEMENT_APP(wxCifeApp);

// --------------------------------------------------------------------------------
bool wxCifeApp::OnInit() {
    if (wxFileExists("diskdefs")) {
        SetTopWindow(new MainWindow(NULL));
        GetTopWindow()->Show();
        return (true);
    }
    else {
        wxMessageBox(_("Diskdefinitions File not found.\n"
                       "Please copy 'diskdefs' into Application directory."), _("CP/M Image-File Explorer"), wxICON_EXCLAMATION);
        return (false);
    }
}

// --------------------------------------------------------------------------------
