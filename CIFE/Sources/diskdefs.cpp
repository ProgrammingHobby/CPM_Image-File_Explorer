/*
 * diskdefs.cpp is part of CIFE
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

#include "diskdefs.hpp"
// --------------------------------------------------------------------------------
#include <wx/txtstrm.h>
#include <wx/wfstream.h>
#include <wx/tokenzr.h>
// --------------------------------------------------------------------------------
namespace diskdefs {
    wxArrayString imageTypes;

    void getImageTypes(wxString appPath) {
        imageTypes.Clear();
        imageTypes.Add("Amstrad");
        wxFileInputStream file(appPath + "diskdefs");
        wxTextInputStream text(file);
        int diskdefsCount = 0;

        while (!file.Eof()) {
            wxString line = text.ReadLine();
            wxStringTokenizer tokenizer(line, " ");

            while (tokenizer.HasMoreTokens()) {
                wxString token = tokenizer.GetNextToken();

                if (token == "diskdef") {
                    wxString type = tokenizer.GetNextToken();
                    imageTypes.Add(type);
                    diskdefsCount++;
                }
            }
        }
    }
    // --------------------------------------------------------------------------------
} /* namespace diskdefs */


