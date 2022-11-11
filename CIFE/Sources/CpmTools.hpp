/*
 * CpmTools.hpp is part of CIFE
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

#ifndef CPMTOOLS_H
#define CPMTOOLS_H
// --------------------------------------------------------------------------------
#include "CpmFs.hpp"
#include "CpmDevice.hpp"
// --------------------------------------------------------------------------------
#include <wx/string.h>
#include <wx/arrstr.h>
// --------------------------------------------------------------------------------
class CpmGuiInterface;
// --------------------------------------------------------------------------------
class CpmTools {

    public:     // Attributes

    public:     // Methods
        bool setImageType(wxString typeName);
        bool openImage(wxString fileName);
        bool closeImage();
        void showDirectory();
        void deleteFile(wxArrayString files);
        void renameFile(wxString oldName, wxString newName);
        void setFileAttributes(wxString name, int attributes);
        void setFileProtections(wxString name, int protections);
        void createNewImage(wxString imageFile, wxString label, bool useTimeStamps,
                            wxString bootTrackFile);
        void checkImage(bool doRepair);
        void writeFileToImage(wxString filename, int userNumber, bool isTextFile,
                              bool preserveTimeStamps);

    public:     // Constructor & Destructor
        CpmTools(CpmDevice *device, CpmFs *fs, CpmGuiInterface *intf, wxString appPath);
        ~CpmTools();

    protected:  // Event Methods

    private:    // Attributes
        CpmGuiInterface *guiintf;
        CpmDevice *cpmdevice;
        CpmFs *cpmfs;
        wxString imageTypeName;
        wxString imageFileName;
        wxString diskdefsPath;
        wxString cmd, toolerr;
        mode_t s_ifdir = 1;
        mode_t s_ifreg = 1;

        const char *month[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                                 "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
                                };

    private:    // Methods
        static int namecmp(const void *a, const void *b);
        int getUserNumber(const char *filename);
        void convertFilename(const char *filename, char *cpmname);
        int bcdCheck(int n, int max, const char *msg, const char *unit, int extent1, int extent2);
        int pwdCheck(int extent, const char *pwd, char decode);
        int ask(const char *msg);
        char *prfile(int extent);
        int fsck(const char *image, bool repair);
        int unix2cpm(const char *unixfilename, const char *cpmfilename, bool text, bool preserve);

};

// --------------------------------------------------------------------------------
#endif // CPMTOOLS_H
