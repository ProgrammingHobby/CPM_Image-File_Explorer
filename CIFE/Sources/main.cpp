/*
 * main.cpp is part of CIFE
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
#include "diskdefs.hpp"
// --------------------------------------------------------------------------------
#include <wx/app.h>
#include <wx/stdpaths.h>
#include <wx/msgdlg.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/image.h>
#include <wx/filename.h>
// --------------------------------------------------------------------------------
class wxCifeApp : public wxApp {
    public:
        virtual bool OnInit();
};

// --------------------------------------------------------------------------------
IMPLEMENT_APP(wxCifeApp);

// --------------------------------------------------------------------------------
bool wxCifeApp::OnInit() {
    wxFileName f(wxStandardPaths::Get().GetExecutablePath());
    wxString appPath(f.GetPath() + wxFileName::GetPathSeparator());

    if (wxFileExists(appPath  + "diskdefs")) {
        diskdefs::getImageTypes(appPath);
        wxXmlResource::Get()->InitAllHandlers();
        wxImage::AddHandler(new wxPNGHandler);
        SetTopWindow(new MainWindow(NULL, appPath));
        GetTopWindow()->Show();
        return (true);
    }
    else {
        wxMessageBox(_("Diskdefinitions File not found.\n"
                       "Please copy 'diskdefs' into Application directory."),
                     _("CP/M Image-File Explorer"),
                     wxICON_EXCLAMATION);
        return (false);
    }
}

// --------------------------------------------------------------------------------
