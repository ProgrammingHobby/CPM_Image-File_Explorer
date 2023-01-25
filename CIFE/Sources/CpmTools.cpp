/*
 * CpmTools.cpp is part of CIFE
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

#include "CpmDefs.h"
#include "CpmTools.hpp"
#include "CpmGuiInterface.hpp"
// --------------------------------------------------------------------------------
#include <fcntl.h>
// --------------------------------------------------------------------------------
CpmTools::CpmTools(CpmDevice *device, CpmFs *fs, CpmGuiInterface *intf,
                   wxString appPath) {
    guiintf = intf;
    diskdefsPath = appPath;
    cpmdevice = device;
    cpmfs = fs;
}

// --------------------------------------------------------------------------------
CpmTools::~CpmTools() {
}

// --------------------------------------------------------------------------------
bool CpmTools::setImageType(wxString typeName) {
    imageTypeName = typeName;

    if (cpmfs->readDiskdefData(typeName.c_str()) == -1) {
        guiintf->printMsg(wxString::Format("cannot read superblock  (%s)\n",
                                           cpmfs->getErrorMsg()));
        return (false);
    }

    return (true);
}

// --------------------------------------------------------------------------------
wxString CpmTools::getActualImageType() {
    return (imageTypeName);
}

// --------------------------------------------------------------------------------
bool CpmTools::openImage(wxString fileName, bool useUppercase) {
    imageFileName = fileName;
    wxString image = fileName.substr(fileName.find_last_of("/\\") + 1);

    if (!cpmdevice->Open(fileName.c_str(), "r+b")) {
        guiintf->printMsg(wxString::Format("cannot open %s  (%s)\n", image,
                                           cpmdevice->getErrorMsg()));
        return (false);
    }
    else {
        if (cpmfs->initDriveData(useUppercase) == -1) {
            guiintf->printMsg(wxString::Format("cannot init filesystem  (%s)\n",
                                               cpmfs->getErrorMsg()));
            return (false);
        }
    }

    return (true);
}

// --------------------------------------------------------------------------------
wxString CpmTools::getActualFileName() {
    return (imageFileName);
}

// --------------------------------------------------------------------------------
bool CpmTools::closeImage() {
    cpmfs->unmount();
    wxString image = imageFileName.substr(imageFileName.find_last_of("/\\") + 1);

    if (!cpmdevice->Close()) {
        guiintf->printMsg(wxString::Format("cannot close %s  (%s)\n", image,
                                           cpmdevice->getErrorMsg()));
        return (false);
    }

    return (true);
}

// --------------------------------------------------------------------------------
void CpmTools::showDirectory() {
    CpmFs::CpmInode_t file;
    CpmFs::CpmStatFS_t buf;
    CpmFs::CpmStat_t statbuf;
    wxString files = "*";
    int gargc, row = 0;
    char **gargv;
    cmd = "cpm.directory";
    cpmfs->glob(files.c_str(), &gargc, &gargv);

    if (gargc) {
        int i, l, user, attrib;
        int totalBytes = 0, totalRecs = 0;
        qsort(gargv, gargc, sizeof(char *), namecmp);
        cpmfs->statFs(&buf);

        for (l = user = 0; user < 32; ++user) {
            for (i = 0; i < gargc; ++i) {
                tm *tmp;

                if (gargv[i][0] == '0' + user / 10 && gargv[i][1] == '0' + user % 10) {
                    cpmfs->namei(gargv[i], &file);
                    cpmfs->stat(&file, &statbuf);
                    cpmfs->attrGet(&file, &attrib);
                    totalBytes += statbuf.size;
                    totalRecs += (statbuf.size + 127) / 128;
                    /*    user: name    */
                    guiintf->printDirEntry(0, row, wxString::Format("%2d: %s", user, (gargv[i] + 2)));
                    /*    bytes    */
                    guiintf->printDirEntry(1, row, wxString::Format("%5.1ldK",
                                           (long)(statbuf.size + buf.f_bsize - 1) / buf.f_bsize * (buf.f_bsize / 1024)));
                    /*    records    */
                    guiintf->printDirEntry(2, row, wxString::Format("%6.1ld", (long)(statbuf.size / 128)));
                    /*    attributes    */
                    std::string attribute;
                    attribute += ((attrib & CPM_ATTR_F1)      ? '1' : '-');
                    attribute += ((attrib & CPM_ATTR_F2)      ? '2' : '-');
                    attribute += ((attrib & CPM_ATTR_F3)      ? '3' : '-');
                    attribute += ((attrib & CPM_ATTR_F4)      ? '4' : '-');
                    attribute += ' ';
                    attribute += ((attrib & CPM_ATTR_RO)      ? 'r' : '-');
                    attribute += ((attrib & CPM_ATTR_SYS)     ? 's' : '-');
                    attribute += ((attrib & CPM_ATTR_ARCV)    ? 'a' : '-');
                    guiintf->printDirEntry(3, row, attribute);
                    /*    protections    */
                    std::string protections;
                    protections += ((attrib & CPM_ATTR_PWREAD)  ? "rd" : "--");
                    protections += ' ';
                    protections += ((attrib & CPM_ATTR_PWWRITE) ? "wr" : "--");
                    protections += ' ';
                    protections += ((attrib & CPM_ATTR_PWDEL)   ? "del" : "---");
                    guiintf->printDirEntry(4, row, protections);

                    /*    updated    */
                    if (statbuf.mtime) {
                        tmp = localtime(&statbuf.mtime);

                        if (tmp) {
                            guiintf->printDirEntry(5, row, wxString::Format("%02d-%s-%04d %02d:%02d", tmp->tm_mday,
                                                   month[tmp->tm_mon], tmp->tm_year + 1900, tmp->tm_hour, tmp->tm_min));
                        }
                    }

                    /*    created    */
                    if (statbuf.ctime) {
                        tmp = localtime(&statbuf.ctime);

                        if (tmp) {
                            guiintf->printDirEntry(6, row, wxString::Format("%02d-%s-%04d %02d:%02d", tmp->tm_mday,
                                                   month[tmp->tm_mon], tmp->tm_year + 1900, tmp->tm_hour, tmp->tm_min));
                        }
                    }

                    /*    last access    */
                    if (statbuf.atime) {
                        tmp = localtime(&statbuf.atime);

                        if (tmp) {
                            guiintf->printDirEntry(7, row, wxString::Format("%02d-%s-%04d %02d:%02d", tmp->tm_mday,
                                                   month[tmp->tm_mon], tmp->tm_year + 1900, tmp->tm_hour, tmp->tm_min));
                        }
                    }

                    ++l;
                    row++;
                }
            }
        }

        guiintf->printDirInfo(
            wxString::Format(" Total Bytes = %5.1dk\t\t\tTotal Records = %5.1d\t\t\t\tFiles Found = %4.1d\n"
                             " Total 1k Blocks = %5.1ld\t\tUsed/Max Dir Entries: %4.1ld/%4.1ld",
                             ((totalBytes + 1023) / 1024), totalRecs, l, ((buf.f_bused * buf.f_bsize) / 1024),
                             (buf.f_files - buf.f_ffree), buf.f_files));
    }
    else {
        guiintf->printDirInfo(" No files.");
    }
}

// --------------------------------------------------------------------------------
void CpmTools::deleteFile(wxArrayString files) {
    char **gargv;
    int gargc;
    cmd = "cpm.remove";

    for (size_t count = 0; count < files.GetCount(); count++) {
        wxString fileName = files.Item(count);
        cpmfs->glob(fileName.c_str(), &gargc, &gargv);

        if (cpmfs->unlink(gargv[0]) == -1) {
            guiintf->printMsg(wxString::Format("%s: can not erase %s  (%s)\n", cmd, gargv[0],
                                               cpmfs->getErrorMsg()), CpmGuiInterface::msgColRed);
        }
    }

    cpmfs->sync();
}

// --------------------------------------------------------------------------------
void CpmTools::renameFile(wxString oldName, wxString newName) {
    char nName[15];
    char **gargv;
    int gargc;
    cmd = "cpm.rename";
    cpmfs->glob(oldName.c_str(), &gargc, &gargv);
    convertFilename(newName.c_str(), nName);

    if (cpmfs->rename(gargv[0], nName) == -1) {
        guiintf->printMsg(wxString::Format("%s: can not rename %s in %s  (%s)\n", cmd, oldName,
                                           newName, cpmfs->getErrorMsg()), CpmGuiInterface::msgColRed);
    }

    cpmfs->sync();
}

// --------------------------------------------------------------------------------
void CpmTools::setFileAttributes(wxString name, int attributes) {
    char **gargv;
    int gargc;
    CpmFs::CpmInode_t ino;
    cmd = "cpm.chattr";
    cpmfs->glob(name.c_str(), &gargc, &gargv);

    if (cpmfs->namei(gargv[0], &ino) == -1) {
        guiintf->printMsg(wxString::Format("%s: can not find %s  (%s)\n", cmd, gargv[0],
                                           cpmfs->getErrorMsg()), CpmGuiInterface::msgColRed);
    }
    else if (cpmfs->attrSet(&ino, attributes) == -1) {
        guiintf->printMsg(wxString::Format("%s: can not set attributes %s  (%s)\n", cmd, gargv[0],
                                           cpmfs->getErrorMsg()), CpmGuiInterface::msgColRed);
    }

    cpmfs->sync();
}

// --------------------------------------------------------------------------------
void CpmTools::setFileProtections(wxString name, int protections) {
    char **gargv;
    int gargc;
    CpmFs::CpmInode_t ino;
    cmd = "cpm.chprot";
    cpmfs->glob(name.c_str(), &gargc, &gargv);

    if (cpmfs->namei(gargv[0], &ino) == -1) {
        guiintf->printMsg(wxString::Format("%s: can not find %s  (%s)\n", cmd, gargv[0],
                                           cpmfs->getErrorMsg()), CpmGuiInterface::msgColRed);
    }
    else if (cpmfs->protSet(&ino, protections) == -1) {
        guiintf->printMsg(wxString::Format("%s: can not set protections %s  (%s)\n", cmd,
                                           gargv[0],
                                           cpmfs->getErrorMsg()), CpmGuiInterface::msgColRed);
    }

    cpmfs->sync();
}

// --------------------------------------------------------------------------------
void CpmTools::createNewImage(wxString imageFile, wxString label, bool useTimeStamps,
                              bool useUppercase,
                              wxString bootTrackFile) {
    char *bootTracks;
    cmd = "cpm.mkfs";
    wxString bootImage = bootTrackFile.substr(bootTrackFile.find_last_of("/\\") + 1);
    wxString image = imageFile.substr(imageFile.find_last_of("/\\") + 1);
    size_t bootTrackSize = cpmfs->getBootTrackSize();

    if ((bootTracks = (char *)malloc(bootTrackSize)) == (char *)0) {
        guiintf->printMsg(wxString::Format("%s: can not allocate boot track buffer  (%s)\n", cmd,
                                           strerror(errno)), CpmGuiInterface::msgColRed);
        return;
    }

    memset(bootTracks, 0xe5, bootTrackSize);

    if (!bootTrackFile.IsEmpty() && (bootTrackSize > 0)) {
        FILE *fd;
        size_t filesize, readsize;

        if ((fd = fopen(bootTrackFile.c_str(), "rb")) == (FILE *)0) {
            guiintf->printMsg(wxString::Format("%s: can not open %s  (%s)\n", cmd, bootImage.c_str(),
                                               strerror(errno)), CpmGuiInterface::msgColRed);
            return;
        }

        fseek(fd, 0, SEEK_END);
        filesize = ftell(fd);

        if (filesize > bootTrackSize) {
            guiintf->printMsg(wxString::Format("%s: boottrack file is bigger than boottracks size.\n",
                                               cmd), CpmGuiInterface::msgColRed);
            return;
        }

        rewind(fd);
        readsize = fread(bootTracks, sizeof(char), filesize, fd);

        if (readsize < filesize) {
            guiintf->printMsg(wxString::Format("%s: error reading boottrack file.\n", cmd),
                              CpmGuiInterface::msgColRed);
            return;
        }
    }

    if (cpmfs->mkfs(imageFile.c_str(), imageTypeName.c_str(), label.c_str(), bootTracks,
                    (useTimeStamps ? 1 : 0), useUppercase) == -1) {
        guiintf->printMsg(wxString::Format("%s: can not make new file system  (%s)\n", cmd,
                                           cpmfs->getErrorMsg()), CpmGuiInterface::msgColRed);
        return;
    }

    guiintf->printMsg(wxString::Format("%s: new Image-File '%s' successful created.\n", cmd,
                                       image), CpmGuiInterface::msgColBlack);
}

// --------------------------------------------------------------------------------
void CpmTools::checkImage(bool doRepair) {
    int ret;
    cmd = "cpm.fsck";
    wxString image = imageFileName.substr(imageFileName.find_last_of("/\\") + 1);
    ret = fsck(image.c_str(), doRepair);

    if (ret & MODIFIED) {
        if (cpmfs->sync() == -1) {
            guiintf->printMsg(wxString::Format("%s: write error on %s  (%s)\n", cmd, image.c_str(),
                                               strerror(errno)), CpmGuiInterface::msgColRed);
            ret |= BROKEN;
        }

        guiintf->printMsg(wxString::Format("%s: file system on %s modified", cmd, image.c_str()),
                          CpmGuiInterface::msgColBlue);

        if (ret & BROKEN) {
            guiintf->printMsg(wxString::Format(", please check again"), CpmGuiInterface::msgColBlue);
        }

        guiintf->printMsg(wxString::Format("\n"), CpmGuiInterface::msgColBlue);
    }

    cpmfs->sync();
}

// --------------------------------------------------------------------------------
void CpmTools::writeFileToImage(wxString filename, int userNumber, bool isTextFile,
                                bool preserveTimeStamps) {
    char **gargv;
    int gargc;
    cmd = "cpm.cp";
    wxString image = imageFileName.substr(imageFileName.find_last_of("/\\") + 1);
    wxString cpmfile = wxString::Format("%d:",
                                        userNumber) + filename.substr(filename.find_last_of("/\\") + 1);
    cpmfs->glob(cpmfile.c_str(), &gargc, &gargv);
    unix2cpm(filename, cpmfile, isTextFile, preserveTimeStamps);
    cpmfs->sync();
}

// --------------------------------------------------------------------------------
//  -- compare two entries
// --------------------------------------------------------------------------------
int CpmTools::namecmp(const void *a, const void *b) {
    if (**((const char *const *)a) == '[') {
        return -1;
    }

    return strcmp(*((const char *const *)a), *((const char *const *)b));
}

// --------------------------------------------------------------------------------
//  -- get User Number from Filename (UU:NNNNNNNN.EEE)
// --------------------------------------------------------------------------------
int CpmTools::getUserNumber(const char *filename) {
    if (isdigit(*filename) && *(filename + 1) == ':') {
        return (*filename - '0');
    }

    if (isdigit(*filename) && isdigit(*(filename + 1)) && *(filename + 2) == ':') {
        return (10 * (*filename - '0') + (*(filename + 1) - '0'));
    }

    return -1;
}

// --------------------------------------------------------------------------------
//  -- convert Filename from UU:NNNNNNNN.EEE to UUNNNNNNNN.EEE
// --------------------------------------------------------------------------------
void CpmTools::convertFilename(const char *filename, char *cpmname) {
    memset(cpmname, ' ', (2 + 8 + 1 + 3 + 1));
    snprintf(cpmname, 15, "%02d%s", getUserNumber(filename), strchr(filename, ':') + 1);
}

// --------------------------------------------------------------------------------
//  -- check format and range of BCD digit
// --------------------------------------------------------------------------------
int CpmTools::bcdCheck(int n, int max, const char *msg, const char *unit, int extent1,
                       int extent2) {
    if (((n >> 4) & 0xf) > 10 || (n & 0xf) > 10
            || (((n >> 4) & 0xf) * 10 + (n & 0xf)) >= max) {
        guiintf->printMsg(wxString::Format("    Bad %s %s (extent=%d/%d, %s=%02x)\n", msg, unit,
                                           extent1, extent2, unit, (n & 0xff)), CpmGuiInterface::msgColRed);
        return -1;
    }
    else {
        return 0;
    }
}

// --------------------------------------------------------------------------------
//  -- check password
// --------------------------------------------------------------------------------
int CpmTools::pwdCheck(int extent, const char *pwd, char decode) {
    char c;
    int i;

    for (i = 0; i < 8; ++i) {
        if ((c = ((char)(pwd[7 - i] ^ decode))) < ' ' || (c & 0x80)) {
            wxString passwd;

            for (i = 0; i < 8; ++i) {
                c = pwd[7 - i] ^ decode;

                if (c < ' ' || (c & 0x80)) {
                    passwd += "\\";
                    passwd += ('0' + ((c >> 6) & 0x03));
                    passwd += ('0' + ((c >> 3) & 0x03));
                    passwd += ('0' + (c & 0x03));
                }
                else {
                    passwd += (c);
                }
            }

            guiintf->printMsg(
                wxString::Format("    Non-printable character in password (extent=%d, password=%s)\n",
                                 extent, passwd), CpmGuiInterface::msgColRed);
            return -1;
        }
    }

    return 0;
}

// --------------------------------------------------------------------------------
//  -- ask user and return answer
// --------------------------------------------------------------------------------
int CpmTools::ask(const char *msg) {
    if (guiintf->askBox(msg, cmd)) {
        guiintf->printMsg(wxString::Format("    %s [Y]? Y\n", msg), CpmGuiInterface::msgColGreen);
        return 1;
    }

    guiintf->printMsg(wxString::Format("    %s [Y]? N\n", msg), CpmGuiInterface::msgColGreen);
    return 0;
}

// --------------------------------------------------------------------------------
//  -- print file name
// --------------------------------------------------------------------------------
char *CpmTools::prfile(int extent) {
    CpmFs::CpmSuperBlock_t drive = cpmfs->getDriveData();
    CpmFs::PhysDirectoryEntry_t *dir;
    static char name[80];
    char *s = name;
    int i;
    char c;
    dir = drive.dir + extent;

    for (i = 0; i < 8; ++i) {
        c = dir->name[i];

        if ((c & 0x7f) < ' ') {
            *s++ = '\\';
            *s++ = ('0' + ((c >> 6) & 0x01));
            *s++ = ('0' + ((c >> 3) & 0x03));
            *s++ = ('0' + (c & 0x03));
        }
        else {
            *s++ = (c & 0x7f);
        }
    }

    *s++ = '.';

    for (i = 0; i < 3; ++i) {
        c = dir->ext[i];

        if ((c & 0x7f) < ' ') {
            *s++ = '\\';
            *s++ = ('0' + ((c >> 6) & 0x01));
            *s++ = ('0' + ((c >> 3) & 0x03));
            *s++ = ('0' + (c & 0x03));
        }
        else {
            *s++ = (c & 0x7f);
        }
    }

    *s = '\0';
    return name;
}

// --------------------------------------------------------------------------------
//  -- file system check
// --------------------------------------------------------------------------------
int CpmTools::fsck(const char *image, bool repair) {
    CpmFs::CpmSuperBlock_t drive = cpmfs->getDriveData();
    int ret = OK;
    int extent, extent2;
    CpmFs::PhysDirectoryEntry_t *dir, *dir2;
    /* Phase 1: check extent fields */
    guiintf->printMsg("====================================================================================================\n",
                      CpmGuiInterface::msgColGreen);
    guiintf->printMsg("  Phase 1: check extent fields\n", CpmGuiInterface::msgColGreen);

    for (extent = 0; extent < drive.maxdir; ++extent) {
        char *status;
        int usedBlocks = 0;
        dir = drive.dir + extent;
        status = &dir->status;

        if (*status >= 0
                && *status <= (drive.type == CPMFS_P2DOS ? 31 : 15)) { /* directory entry */
            /* check name and extension */
            {
                int i;
                char *c;

                for (i = 0; i < 8; ++i) {
                    c = &(dir->name[i]);

                    if (!ISFILECHAR(i, *c & 0x7f) || islower(*c & 0x7f)) {
                        guiintf->printMsg(
                            wxString::Format("    Error: Bad name (extent=%d, name=\"%s\", position=%d)\n", extent,
                                             prfile(extent), i), CpmGuiInterface::msgColGreen);

                        if (repair && ask("Remove file")) {
                            *status = (char)0xE5;
                            ret |= MODIFIED;
                            break;
                        }
                        else {
                            ret |= BROKEN;
                        }
                    }
                }

                if (*status == (char)0xe5) {
                    continue;
                }

                for (i = 0; i < 3; ++i) {
                    c = &(dir->ext[i]);

                    if (!ISFILECHAR(1, *c & 0x7f) || islower(*c & 0x7f)) {
                        guiintf->printMsg(
                            wxString::Format("    Error: Bad name (extent=%d, name=\"%s\", position=%d)\n", extent,
                                             prfile(extent), i), CpmGuiInterface::msgColGreen);

                        if (repair && ask("Remove file")) {
                            *status = (char)0xE5;
                            ret |= MODIFIED;
                            break;
                        }
                        else {
                            ret |= BROKEN;
                        }
                    }
                }

                if (*status == (char)0xe5) {
                    continue;
                }
            }

            /* check extent number */
            if ((dir->extnol & 0xff) > 0x1f) {
                guiintf->printMsg(
                    wxString::Format("    Error: Bad lower bits of extent number (extent=%d, name=\"%s\", low bits=%d)\n",
                                     extent, prfile(extent), dir->extnol & 0xff), CpmGuiInterface::msgColGreen);

                if (repair && ask("Remove file")) {
                    *status = (char)0xE5;
                    ret |= MODIFIED;
                }
                else {
                    ret |= BROKEN;
                }
            }

            if (*status == (char)0xe5) {
                continue;
            }

            if ((dir->extnoh & 0xff) > 0x3f) {
                guiintf->printMsg(
                    wxString::Format("    Error: Bad higher bits of extent number (extent=%d, name=\"%s\", high bits=%d)\n",
                                     extent, prfile(extent), dir->extnoh & 0xff), CpmGuiInterface::msgColGreen);

                if (repair && ask("Remove file")) {
                    *status = (char)0xE5;
                    ret |= MODIFIED;
                }
                else {
                    ret |= BROKEN;
                }
            }

            if (*status == (char)0xe5) {
                continue;
            }

            /* check last record byte count */
            if ((dir->lrc & 0xff) > 128) {
                guiintf->printMsg(
                    wxString::Format("    Error: Bad last record byte count (extent=%d, name=\"%s\", lrc=%d)\n",
                                     extent, prfile(extent), dir->lrc & 0xff), CpmGuiInterface::msgColGreen);

                if (repair && ask("Clear last record byte count")) {
                    dir->lrc = (char)0;
                    ret |= MODIFIED;
                }
                else {
                    ret |= BROKEN;
                }
            }

            if (*status == (char)0xe5) {
                continue;
            }

            /* check block number range */
            {
                int block, min, max, i;
                min = drive.dirblks;
                max = drive.size;

                for (i = 0; i < 16; ++i) {
                    block = dir->pointers[i] & 0xff;

                    if (drive.size > 256) {
                        block += (dir->pointers[++i] & 0xff) << 8;
                    }

                    if (block > 0) {
                        ++usedBlocks;

                        if (block < min || block >= max) {
                            guiintf->printMsg(
                                wxString::Format("    Error: Bad block number (extent=%d, name=\"%s\", block=%d)\n",
                                                 extent, prfile(extent), block), CpmGuiInterface::msgColGreen);

                            if (repair && ask("Remove file")) {
                                *status = (char)0xE5;
                                ret |= MODIFIED;
                                break;
                            }
                            else {
                                ret |= BROKEN;
                            }
                        }
                    }
                }

                if (*status == (char)0xe5) {
                    continue;
                }
            }
            /* check number of used blocks ? */
            /* check record count */
            {
                int i, min, max, recordsInBlocks, used = 0;
                min = (dir->extnol % drive.extents) * 16 / drive.extents;
                max = ((dir->extnol % drive.extents) + 1) * 16 / drive.extents;

                for (i = min; i < max; ++i) {
                    if (dir->pointers[i] || (drive.size > 256 && dir->pointers[i + 1])) {
                        ++used;
                    }

                    if (drive.size > 256) {
                        ++i;
                    }
                }

                recordsInBlocks = (((unsigned char)dir->blkcnt) * 128 + drive.blksiz - 1) / drive.blksiz;

                if (recordsInBlocks != used) {
                    guiintf->printMsg(
                        wxString::Format("    Error: Bad record count (extent=%d, name=\"%s\", record count=%d)\n",
                                         extent, prfile(extent), dir->blkcnt & 0xff), CpmGuiInterface::msgColGreen);

                    if (repair && ask("Remove file")) {
                        *status = (char)0xE5;
                        ret |= MODIFIED;
                    }
                    else {
                        ret |= BROKEN;
                    }
                }

                if (*status == (char)0xe5) {
                    continue;
                }
            }

            /* check for too large .com files */
            if (((EXTENT(dir->extnol, dir->extnoh) == 3 && dir->blkcnt >= 126)
                    || EXTENT(dir->extnol, dir->extnoh) >= 4) && (dir->ext[0] & 0x7f) == 'C'
                    && (dir->ext[1] & 0x7f) == 'O' && (dir->ext[2] & 0x7f) == 'M') {
                guiintf->printMsg(
                    wxString::Format("    Warning: Oversized .COM file (extent=%d, name=\"%s\")\n", extent,
                                     prfile(extent)), CpmGuiInterface::msgColGreen);
            }
        }
        else if ((drive.type == CPMFS_P2DOS || drive.type == CPMFS_DR3)
                 && *status == 33) { /* check time stamps ? */
            unsigned long created, modified;
            char s;

            if ((s = drive.dir[extent2 = (extent & ~3)].status) >= 0
                    && s <= (drive.type == CPMFS_P2DOS ? 31 :
                             15)) { /* time stamps for first of the three extents */
                bcdCheck(dir->name[2], 24, drive.cnotatime ? "creation date" : "access date", "hour",
                         extent, extent2);
                bcdCheck(dir->name[3], 60, drive.cnotatime ? "creation date" : "access date", "minute",
                         extent, extent2);
                bcdCheck(dir->name[6], 24, "modification date", "hour", extent, extent2);
                bcdCheck(dir->name[7], 60, "modification date", "minute", extent, extent2);
                created = (dir->name[4] + (dir->name[1] << 8)) * (0x60 * 0x60) + dir->name[2] * 0x60 +
                          dir->name[3];
                modified = (dir->name[0] + (dir->name[5] << 8)) * (0x60 * 0x60) + dir->name[6] * 0x60 +
                           dir->name[7];

                if (drive.cnotatime && modified < created) {
                    guiintf->printMsg(
                        wxString::Format("    Warning: Modification date earlier than creation date (extent=%d/%d)\n",
                                         extent, extent2), CpmGuiInterface::msgColGreen);
                }
            }

            if ((s = drive.dir[extent2 = (extent & ~3) + 1].status) >= 0
                    && s <= (drive.type == CPMFS_P2DOS ? 31 : 15)) { /* time stamps for second */
                bcdCheck(dir->lrc, 24, drive.cnotatime ? "creation date" : "access date", "hour", extent,
                         extent2);
                bcdCheck(dir->extnoh, 60, drive.cnotatime ? "creation date" : "access date", "minute",
                         extent, extent2);
                bcdCheck(dir->pointers[1], 24, "modification date", "hour", extent, extent2);
                bcdCheck(dir->pointers[2], 60, "modification date", "minute", extent, extent2);
                created = (dir->ext[2] + (dir->extnol << 8)) * (0x60 * 0x60) + dir->lrc * 0x60 +
                          dir->extnoh;
                modified = (dir->blkcnt + (dir->pointers[0] << 8)) * (0x60 * 0x60) + dir->pointers[1] *
                           0x60 + dir->pointers[2];

                if (drive.cnotatime && modified < created) {
                    guiintf->printMsg(
                        wxString::Format("    Warning: Modification date earlier than creation date (extent=%d/%d)\n",
                                         extent, extent2), CpmGuiInterface::msgColGreen);
                }
            }

            if ((s = drive.dir[extent2 = (extent & ~3) + 2].status) >= 0
                    && s <= (drive.type == CPMFS_P2DOS ? 31 : 15)) { /* time stamps for third */
                bcdCheck(dir->pointers[7], 24, drive.cnotatime ? "creation date" : "access date", "hour",
                         extent, extent2);
                bcdCheck(dir->pointers[8], 60, drive.cnotatime ? "creation date" : "access date",
                         "minute", extent, extent2);
                bcdCheck(dir->pointers[11], 24, "modification date", "hour", extent, extent2);
                bcdCheck(dir->pointers[12], 60, "modification date", "minute", extent, extent2);
                created = (dir->pointers[5] + (dir->pointers[6] << 8)) * (0x60 * 0x60) + dir->pointers[7]
                          * 0x60 + dir->pointers[8];
                modified = (dir->pointers[9] + (dir->pointers[10] << 8)) * (0x60 * 0x60) +
                           dir->pointers[11] * 0x60 + dir->pointers[12];

                if (drive.cnotatime && modified < created) {
                    guiintf->printMsg(
                        wxString::Format("    Warning: Modification date earlier than creation date (extent=%d/%d)\n",
                                         extent, extent2), CpmGuiInterface::msgColGreen);
                }
            }
        }
        else if (drive.type == CPMFS_DR3 && *status == 32) { /* disc label */
            unsigned long created, modified;
            bcdCheck(dir->pointers[10], 24, drive.cnotatime ? "creation date" : "access date", "hour",
                     extent, extent);
            bcdCheck(dir->pointers[11], 60, drive.cnotatime ? "creation date" : "access date",
                     "minute", extent, extent);
            bcdCheck(dir->pointers[14], 24, "modification date", "hour", extent, extent);
            bcdCheck(dir->pointers[15], 60, "modification date", "minute", extent, extent);
            created = (dir->pointers[8] + (dir->pointers[9] << 8)) * (0x60 * 0x60) + dir->pointers[10]
                      * 0x60 + dir->pointers[11];
            modified = (dir->pointers[12] + (dir->pointers[13] << 8)) * (0x60 * 0x60) +
                       dir->pointers[14] * 0x60 + dir->pointers[15];

            if (drive.cnotatime && modified < created) {
                guiintf->printMsg(
                    wxString::Format("    Warning: Label modification date earlier than creation date (extent=%d)\n",
                                     extent), CpmGuiInterface::msgColGreen);
            }

            if ((dir->extnol & 0x40) && (dir->extnol & 0x10)) {
                guiintf->printMsg(
                    wxString::Format("    Error: Bit 4 and 6 can only be exclusively be set (extent=%d, label byte=0x%02x)\n",
                                     extent, (unsigned char)dir->extnol), CpmGuiInterface::msgColGreen);

                if (repair && ask("Time stamp on creation")) {
                    dir->extnol &= ~0x40;
                    ret |= MODIFIED;
                }
                else if (repair && ask("Time stamp on access")) {
                    dir->extnol &= ~0x10;
                    ret |= MODIFIED;
                }
                else {
                    ret |= BROKEN;
                }
            }

            if ((dir->extnol & 0x80) && pwdCheck(extent, dir->pointers, dir->lrc)) {
                char msg[80];
                sprintf(msg, "Set password to %c%c%c%c%c%c%c%c", C0, C1, C2, C3, C4, C5, C6, C7);

                if (repair && ask(msg)) {
                    dir->pointers[0] = PC0;
                    dir->pointers[1] = PC1;
                    dir->pointers[2] = PC2;
                    dir->pointers[3] = PC3;
                    dir->pointers[4] = PC4;
                    dir->pointers[5] = PC5;
                    dir->pointers[6] = PC6;
                    dir->pointers[7] = PC7;
                    dir->lrc = PB;
                    ret |= MODIFIED;
                }
                else {
                    ret |= BROKEN;
                }
            }
        }
        else if (drive.type == CPMFS_DR3 && *status >= 16 && *status <= 31) { /* password */
            /* check name and extension */
            {
                int i;
                char *c;

                for (i = 0; i < 8; ++i) {
                    c = &(dir->name[i]);

                    if (!ISFILECHAR(i, *c & 0x7f) || islower(*c & 0x7f)) {
                        guiintf->printMsg(
                            wxString::Format("    Error: Bad name (extent=%d, name=\"%s\", position=%d)\n", extent,
                                             prfile(extent), i), CpmGuiInterface::msgColGreen);

                        if (repair && ask("Clear password entry")) {
                            *status = (char)0xE5;
                            ret |= MODIFIED;
                            break;
                        }
                        else {
                            ret |= BROKEN;
                        }
                    }
                }

                if (*status == (char)0xe5) {
                    continue;
                }

                for (i = 0; i < 3; ++i) {
                    c = &(dir->ext[i]);

                    if (!ISFILECHAR(1, *c & 0x7f) || islower(*c & 0x7f)) {
                        guiintf->printMsg(
                            wxString::Format("    Error: Bad name (extent=%d, name=\"%s\", position=%d)\n", extent,
                                             prfile(extent), i), CpmGuiInterface::msgColGreen);

                        if (repair && ask("Clear password entry")) {
                            *status = (char)0xE5;
                            ret |= MODIFIED;
                            break;
                        }
                        else {
                            ret |= BROKEN;
                        }
                    }
                }

                if (*status == (char)0xe5) {
                    continue;
                }
            }

            /* check password */
            if ((dir->extnol & (0x80 | 0x40 | 0x20)) && pwdCheck(extent, dir->pointers, dir->lrc)) {
                char msg[80];
                sprintf(msg, "Set password to %c%c%c%c%c%c%c%c", C0, C1, C2, C3, C4, C5, C6, C7);

                if (repair && ask(msg)) {
                    dir->pointers[0] = PC0;
                    dir->pointers[1] = PC1;
                    dir->pointers[2] = PC2;
                    dir->pointers[3] = PC3;
                    dir->pointers[4] = PC4;
                    dir->pointers[5] = PC5;
                    dir->pointers[6] = PC6;
                    dir->pointers[7] = PC7;
                    dir->lrc = PB;
                    ret |= MODIFIED;
                }
                else {
                    ret |= BROKEN;
                }
            }
        }
        else if (*status != (char)0xe5) { /* bad status */
            guiintf->printMsg(
                wxString::Format("    Error: Bad status (extent=%d, name=\"%s\", status=0x%02x)\n",
                                 extent, prfile(extent), *status & 0xff), CpmGuiInterface::msgColGreen);

            if (repair && ask("Clear entry")) {
                *status = (char)0xE5;
                ret |= MODIFIED;
            }
            else {
                ret |= BROKEN;
            }

            continue;
        }
    }

    /* Phase 2: check extent connectivity */
    guiintf->printMsg("  Phase 2: check extent connectivity\n", CpmGuiInterface::msgColGreen);

    /* check multiple allocated blocks */
    for (extent = 0; extent < drive.maxdir; ++extent) {
        if ((dir = drive.dir + extent)->status >= 0
                && dir->status <= (drive.type == CPMFS_P2DOS ? 31 : 15)) {
            int i, j, block, block2;

            for (i = 0; i < 16; ++i) {
                block = dir->pointers[i] & 0xff;

                if (drive.size > 256) {
                    block += (dir->pointers[++i] & 0xff) << 8;
                }

                for (extent2 = 0; extent2 < drive.maxdir; ++extent2) {
                    if ((dir2 = drive.dir + extent2)->status >= 0
                            && dir2->status <= (drive.type == CPMFS_P2DOS ? 31 : 15)) {
                        for (j = 0; j < 16; ++j) {
                            block2 = dir2->pointers[j] & 0xff;

                            if (drive.size > 256) {
                                block2 += (dir2->pointers[++j] & 0xff) << 8;
                            }

                            if (block != 0 && block2 != 0 && block == block2 && !(extent == extent2 && i == j)) {
                                guiintf->printMsg(
                                    wxString::Format("    Error: Multiple allocated block (extent=%d,%d, name=\"%s\"", extent,
                                                     extent2, prfile(extent)), CpmGuiInterface::msgColGreen);
                                guiintf->printMsg(wxString::Format(",\"%s\" block=%d)\n", prfile(extent2), block),
                                                  CpmGuiInterface::msgColGreen);
                                ret |= BROKEN;
                            }
                        }
                    }
                }
            }
        }
    }

    /* check multiple extents */
    for (extent = 0; extent < drive.maxdir; ++extent) {
        if ((dir = drive.dir + extent)->status >= 0
                && dir->status <= (drive.type == CPMFS_P2DOS ? 31 : 15)) {
            for (extent2 = 0; extent2 < drive.maxdir; ++extent2) {
                if ((dir2 = drive.dir + extent2)->status >= 0
                        && dir2->status <= (drive.type == CPMFS_P2DOS ? 31 : 15)) {
                    if (extent != extent2
                            && EXTENT(dir->extnol, dir->extnoh) == EXTENT(dir2->extnol, dir2->extnoh)
                            && dir->status == dir2->status) {
                        int i;

                        for (i = 0; i < 8 && (dir->name[i] & 0x7f) == (dir2->name[i] & 0x7f); ++i);

                        if (i == 8) {
                            for (i = 0; i < 3 && (dir->ext[i] & 0x7f) == (dir2->ext[i] & 0x7f); ++i);

                            if (i == 3) {
                                guiintf->printMsg(wxString::Format("    Error: Duplicate extent (extent=%d,%d)\n", extent,
                                                                   extent2), CpmGuiInterface::msgColGreen);
                                ret |= BROKEN;
                            }
                        }
                    }
                }
            }
        }
    }

    if (ret == 0) { /* print statistics */
        CpmFs::CpmStatFS_t statfsbuf;
        int fragmented = 0, borders = 0;
        cpmfs->statFs(&statfsbuf);

        for (extent = 0; extent < drive.maxdir; ++extent) {
            if ((dir = drive.dir + extent)->status >= 0
                    && dir->status <= (drive.type == CPMFS_P2DOS ? 31 : 15)) {
                int i, block, previous = -1;

                for (i = 0; i < 16; ++i) {
                    block = dir->pointers[i] & 0xff;

                    if (drive.size > 256) {
                        block += (dir->pointers[++i] & 0xff) << 8;
                    }

                    if (previous != -1) {
                        if (block != 0 && block != (previous + 1)) {
                            ++fragmented;
                        }

                        ++borders;
                    }

                    previous = block;
                }
            }
        }

        fragmented = (borders ? (1000 * fragmented) / borders : 0);
        guiintf->printMsg(
            wxString::Format("  %s: %ld/%ld files (%d.%d%% non-contigous), %ld/%ld blocks\n", image,
                             statfsbuf.f_files - statfsbuf.f_ffree, statfsbuf.f_files, fragmented / 10,
                             fragmented % 10, statfsbuf.f_blocks - statfsbuf.f_bfree, statfsbuf.f_blocks),
            CpmGuiInterface::msgColGreen);
    }

    guiintf->printMsg("====================================================================================================\n",
                      CpmGuiInterface::msgColGreen);
    return ret;
}

// --------------------------------------------------------------------------------
int CpmTools::unix2cpm(const char *unixfilename, const char *cpmfilename, bool text,
                       bool preserve) {
    int c, exitcode = 0;
    FILE *ufp;
    wxString unixfile = unixfilename;
    unixfile = unixfile.substr(unixfile.find_last_of("/\\") + 1);

    if ((ufp = fopen(unixfilename, "rb")) == (FILE *)0) {
        guiintf->printMsg(wxString::Format("%s: can not open %s  (%s)\n", cmd, unixfile,
                                           strerror(errno)));
        exitcode = 1;
    }
    else {
        CpmFs::CpmInode_t ino;
        char cpmname[2 + 8 + 1 + 3 + 1];
        char *translate;
        struct stat st;
        stat(unixfilename, &st);
        snprintf(cpmname, sizeof(cpmname), "%02d%s", getUserNumber(cpmfilename),
                 strchr(cpmfilename, ':') + 1);
        translate = cpmname;

        while ((translate = strchr(translate, ','))) {
            *translate = '/';
        }

        if (cpmfs->create(&cpmfs->getDirectoryRoot(), cpmname, &ino, 0666) == -1) {
            guiintf->printMsg(wxString::Format("%s: can not create %s  (%s)\n", cmd, cpmfilename,
                                               cpmfs->getErrorMsg()));
            exitcode = 1;
        }
        else {
            CpmFs::CpmFile_t file;
            int ohno = 0;
            char buf[4096 + 1];
            cpmfs->open(&ino, &file, O_WRONLY);

            do {
                ssize_t j;

                for (j = 0; j < ((ssize_t)sizeof(buf) / 2) && (c = getc(ufp)) != EOF; ++j) {
                    if (text && c == '\n') {
                        buf[j++] = '\r';
                    }

                    buf[j] = c;
                }

                if (text && c == EOF) {
                    buf[j++] = '\032';
                }

                if (cpmfs->write(&file, buf, j) != j) {
                    guiintf->printMsg(wxString::Format("%s: can not write %s  (%s)\n", cmd, cpmfilename,
                                                       cpmfs->getErrorMsg()));
                    ohno = 1;
                    exitcode = 1;
                    break;
                }
            } while (c != EOF);

            if (cpmfs->close(&file) == EOF && !ohno) {
                guiintf->printMsg(wxString::Format("%s: can not close %s  (%s)\n", cmd, cpmfilename,
                                                   cpmfs->getErrorMsg()));
                exitcode = 1;
            }

            if (preserve && !ohno) {
                struct utimbuf times;
                times.actime = st.st_atime;
                times.modtime = st.st_mtime;
                cpmfs->utime(&ino, &times);
            }
        }

        if (fclose(ufp) == EOF) {
            guiintf->printMsg(wxString::Format("%s: can not close %s  (%s)\n", cmd, cpmfilename,
                                               strerror(errno)));
            exitcode = 1;
        }
    }

    return (exitcode);
}

// --------------------------------------------------------------------------------
