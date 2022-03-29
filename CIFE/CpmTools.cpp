/***********************************************************************************
**
**	Copyright (C) 31.07.2021 Uwe Merker Germany
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
#include "CpmTools.h"
#include "CpmGuiInterface.h"
#include "CpmDefs.h"
// --------------------------------------------------------------------------------
#include <cstdio>
#include <ctype.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
// --------------------------------------------------------------------------------
CpmTools::CpmTools(CpmGuiInterface *intf, wxString appPath) {
    guiintf = intf;
    diskdefsPath = appPath;
}

// --------------------------------------------------------------------------------
CpmTools::~CpmTools() {
}

// --------------------------------------------------------------------------------
void CpmTools::setImageType(wxString typeName) {
    imageTypeName = typeName;
}

// --------------------------------------------------------------------------------
void CpmTools::setImageFile(wxString fileName) {
    imageFileName = fileName;
}

// --------------------------------------------------------------------------------
bool CpmTools::getBootTracksEnabled() {
    return ((drive.boottrk > 0) ? true : false);
}

// --------------------------------------------------------------------------------
void CpmTools::showDirectory() {
    CpmInode_t file;
    CpmStatFS_t buf;
    CpmStat_t statbuf;
    wxString files = "*";
    int gargc, row = 0;
    char **gargv;
    cmd = "cpm.directory";
    wxString image = imageFileName.substr(imageFileName.find_last_of("/\\") + 1);

    if ((err = deviceOpen(imageFileName.c_str(), "rb"))) {
        guiintf->printMsg(wxString::Format("%s: cannot open %s (%s)\n", cmd, image, err));
        return;
    }

    if (cpmReadSuper(imageTypeName.c_str()) == -1) {
        guiintf->printMsg(wxString::Format("%s: cannot read superblock (%s)\n", cmd, err));
        return;
    }

    cpmglob(files.c_str(), &gargc, &gargv);

    if (gargc) {
        int i, l, user, attrib;
        int totalBytes = 0, totalRecs = 0;
        qsort(gargv, gargc, sizeof(char *), namecmp);
        cpmStatFS(&buf);

        for (l = user = 0; user < 32; ++user) {
            for (i = 0; i < gargc; ++i) {
                tm *tmp;

                if (gargv[i][0] == '0' + user / 10 && gargv[i][1] == '0' + user % 10) {
                    cpmNamei(gargv[i], &file);
                    cpmStat(&file, &statbuf);
                    cpmAttrGet(&file, &attrib);
                    totalBytes += statbuf.size;
                    totalRecs += (statbuf.size + 127) / 128;
                    /*    user: name    */
                    guiintf->printDirEntry(0, row, wxString::Format("%2d: %s", user, (gargv[i] + 2)));
                    /*    bytes    */
                    guiintf->printDirEntry(1, row, wxString::Format("%5.1ldK", (long)(statbuf.size + buf.f_bsize - 1) / buf.f_bsize * (buf.f_bsize / 1024)));
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
                            guiintf->printDirEntry(5, row, wxString::Format("%02d-%s-%04d %02d:%02d", tmp->tm_mday, month[tmp->tm_mon], tmp->tm_year + 1900, tmp->tm_hour, tmp->tm_min));
                        }
                    }

                    /*    created    */
                    if (statbuf.ctime) {
                        tmp = localtime(&statbuf.ctime);

                        if (tmp) {
                            guiintf->printDirEntry(6, row, wxString::Format("%02d-%s-%04d %02d:%02d", tmp->tm_mday, month[tmp->tm_mon], tmp->tm_year + 1900, tmp->tm_hour, tmp->tm_min));
                        }
                    }

                    /*    last access    */
                    if (statbuf.atime) {
                        tmp = localtime(&statbuf.atime);

                        if (tmp) {
                            guiintf->printDirEntry(7, row, wxString::Format("%02d-%s-%04d %02d:%02d", tmp->tm_mday, month[tmp->tm_mon], tmp->tm_year + 1900, tmp->tm_hour, tmp->tm_min));
                        }
                    }

                    ++l;
                    row++;
                }
            }
        }

        guiintf->printDirInfo(wxString::Format(" Total Bytes = %5.1dk\t\t\tTotal Records = %5.1d\t\t\t\tFiles Found = %4.1d\n"
                                               " Total 1k Blocks = %5.1ld\t\tUsed/Max Dir Entries: %4.1ld/%4.1ld",
                                               ((totalBytes + 1023) / 1024), totalRecs, l,
                                               ((buf.f_bused * buf.f_bsize) / 1024), (buf.f_files - buf.f_ffree), buf.f_files));
    }
    else {
        guiintf->printDirInfo(" No files.");
    }

    if ((err = deviceClose())) {
        guiintf->printMsg(wxString::Format("%s: cannot close %s (%s)\n", cmd, image, err));
        return;
    }
}

// --------------------------------------------------------------------------------
void CpmTools::deleteFile(wxArrayString files) {
    char **gargv;
    int gargc;
    cmd = "cpm.remove";
    wxString image = imageFileName.substr(imageFileName.find_last_of("/\\") + 1);

    if ((err = deviceOpen(imageFileName.c_str(), "r+b"))) {
        guiintf->printMsg(wxString::Format("%s: cannot open %s (%s)\n", cmd, image, err), CpmGuiInterface::msgColRed);
        return;
    }

    if (cpmReadSuper(imageTypeName.c_str()) == -1) {
        guiintf->printMsg(wxString::Format("%s: cannot read superblock (%s)\n", cmd, err), CpmGuiInterface::msgColRed);
        return;
    }

    for (size_t count = 0; count < files.GetCount(); count++) {
        wxString fileName = files.Item(count);
        cpmglob(fileName.c_str(), &gargc, &gargv);

        if (cpmUnlink(gargv[0]) == -1) {
            guiintf->printMsg(wxString::Format("%s: can not erase %s: %s\n", cmd, gargv[0], err), CpmGuiInterface::msgColRed);
        }
    }

    cpmUmount();

    if ((err = deviceClose())) {
        guiintf->printMsg(wxString::Format("%s: cannot close %s (%s)\n", cmd, image, err), CpmGuiInterface::msgColRed);
    }
}

// --------------------------------------------------------------------------------
void CpmTools::renameFile(wxString oldName, wxString newName) {
    char nName[15];
    char **gargv;
    int gargc;
    cmd = "cpm.rename";
    wxString image = imageFileName.substr(imageFileName.find_last_of("/\\") + 1);

    if ((err = deviceOpen(imageFileName.c_str(), "r+b"))) {
        guiintf->printMsg(wxString::Format("%s: cannot open %s (%s)\n", cmd, image, err), CpmGuiInterface::msgColRed);
        return;
    }

    if (cpmReadSuper(imageTypeName.c_str()) == -1) {
        guiintf->printMsg(wxString::Format("%s: cannot read superblock (%s)\n", cmd, err), CpmGuiInterface::msgColRed);
        return;
    }

    cpmglob(oldName.c_str(), &gargc, &gargv);
    convertFilename(newName.c_str(), nName);

    if (cpmRename(gargv[0], nName) == -1) {
        guiintf->printMsg(wxString::Format("%s: can not rename %s in %s:  %s\n", cmd, oldName, newName, err), CpmGuiInterface::msgColRed);
    }

    cpmUmount();

    if ((err = deviceClose())) {
        guiintf->printMsg(wxString::Format("%s: cannot close %s (%s)\n", cmd, image, err), CpmGuiInterface::msgColRed);
        return;
    }
}

// --------------------------------------------------------------------------------
void CpmTools::setFileAttributes(wxString name, int attributes) {
    char **gargv;
    int gargc;
    cmd = "cpm.chattr";
    wxString image = imageFileName.substr(imageFileName.find_last_of("/\\") + 1);

    if ((err = deviceOpen(imageFileName.c_str(), "r+b"))) {
        guiintf->printMsg(wxString::Format("%s: cannot open %s (%s)\n", cmd, image, err), CpmGuiInterface::msgColRed);
        return;
    }

    if (cpmReadSuper(imageTypeName.c_str()) == -1) {
        guiintf->printMsg(wxString::Format("%s: cannot read superblock (%s)\n", cmd, err), CpmGuiInterface::msgColRed);
        return;
    }

    cpmglob(name.c_str(), &gargc, &gargv);
    CpmInode_t ino;

    if (cpmNamei(gargv[0], &ino) == -1) {
        guiintf->printMsg(wxString::Format("%s: can not find %s: %s\n", cmd, gargv[0], err), CpmGuiInterface::msgColRed);
    }
    else if (cpmAttrSet(&ino, attributes) == -1) {
        guiintf->printMsg(wxString::Format("%s: can not set attributes %s: %s\n", cmd, gargv[0], err), CpmGuiInterface::msgColRed);
    }

    cpmUmount();

    if ((err = deviceClose())) {
        guiintf->printMsg(wxString::Format("%s: cannot close %s (%s)\n", cmd, image, err), CpmGuiInterface::msgColRed);
    }
}

// --------------------------------------------------------------------------------
void CpmTools::setFileProtections(wxString name, int protections) {
    char **gargv;
    int gargc;
    cmd = "cpm.chprot";
    wxString image = imageFileName.substr(imageFileName.find_last_of("/\\") + 1);

    if ((err = deviceOpen(imageFileName.c_str(), "r+b"))) {
        guiintf->printMsg(wxString::Format("%s: cannot open %s (%s)\n", cmd, image, err), CpmGuiInterface::msgColRed);
        return;
    }

    if (cpmReadSuper(imageTypeName.c_str()) == -1) {
        guiintf->printMsg(wxString::Format("%s: cannot read superblock (%s)\n", cmd, err), CpmGuiInterface::msgColRed);
        return;
    }

    cpmglob(name.c_str(), &gargc, &gargv);
    CpmInode_t ino;

    if (cpmNamei(gargv[0], &ino) == -1) {
        guiintf->printMsg(wxString::Format("%s: can not find %s: %s\n", cmd, gargv[0], err), CpmGuiInterface::msgColRed);
    }
    else if (cpmProtSet(&ino, protections) == -1) {
        guiintf->printMsg(wxString::Format("%s: can not set protections %s: %s\n", cmd, gargv[0], err), CpmGuiInterface::msgColRed);
    }

    cpmUmount();

    if ((err = deviceClose())) {
        guiintf->printMsg(wxString::Format("%s: cannot close %s (%s)\n", cmd, image, err), CpmGuiInterface::msgColRed);
    }
}

// --------------------------------------------------------------------------------
void CpmTools::createNewImage(wxString label, bool useTimeStamps, wxString bootTrackFile) {
    size_t bootTrackSize;
    char *bootTracks;
    cmd = "cpm.mkfs";
    wxString bootImage = bootTrackFile.substr(bootTrackFile.find_last_of("/\\") + 1);
    wxString image = imageFileName.substr(imageFileName.find_last_of("/\\") + 1);
    device.opened = 0;
    cpmReadSuper(imageTypeName.c_str());
    bootTrackSize = drive.boottrk * drive.secLength * drive.sectrk;

    if ((bootTracks = (char *)malloc(bootTrackSize)) == (char *)0) {
        guiintf->printMsg(wxString::Format("%s: can not allocate boot track buffer: %s\n", cmd, strerror(errno)), CpmGuiInterface::msgColRed);
        return;
    }

    memset(bootTracks, 0xe5, bootTrackSize);

    if (!bootTrackFile.IsEmpty() && (bootTrackSize > 0)) {
        FILE *fd;
        size_t filesize, readsize;

        if ((fd = fopen(bootTrackFile.c_str(), "rb")) == (FILE *)0) {
            guiintf->printMsg(wxString::Format("%s: can not open %s: %s\n", cmd, bootImage.c_str(), strerror(errno)), CpmGuiInterface::msgColRed);
            return;
        }

        fseek(fd, 0, SEEK_END);
        filesize = ftell(fd);

        if (filesize > bootTrackSize) {
            guiintf->printMsg(wxString::Format("%s: boottrack file is bigger than boottracks size.\n", cmd), CpmGuiInterface::msgColRed);
            return;
        }

        rewind(fd);
        readsize = fread(bootTracks, sizeof(char), filesize, fd);

        if (readsize < filesize) {
            guiintf->printMsg(wxString::Format("%s: error reading boottrack file.\n", cmd), CpmGuiInterface::msgColRed);
            return;
        }
    }

    if (mkfs(imageTypeName.c_str(), label.c_str(), bootTracks, (useTimeStamps ? 1 : 0)) == -1) {
        guiintf->printMsg(wxString::Format("%s: can not make new file system: %s\n", cmd, err), CpmGuiInterface::msgColRed);
        return;
    }

    guiintf->printMsg(wxString::Format("%s: new Image-File '%s' successful created.\n", cmd, image), CpmGuiInterface::msgColBlack);
}

// --------------------------------------------------------------------------------
void CpmTools::checkImage(bool doRepair) {
    int ret;
    cmd = "cpm.fsck";
    wxString image = imageFileName.substr(imageFileName.find_last_of("/\\") + 1);

    if ((err = deviceOpen(imageFileName.c_str(), (doRepair ? "r+b" : "rb")))) {
        if ((err = deviceOpen(imageFileName.c_str(), "rb"))) {
            guiintf->printMsg(wxString::Format("%s: cannot open %s: %s\n", cmd, image.c_str(), err), CpmGuiInterface::msgColRed);
            return;
        }
        else {
            guiintf->printMsg(wxString::Format("%s: cannot open %s for writing, no repair possible\n", cmd, image.c_str()), CpmGuiInterface::msgColRed);
        }
    }

    if (cpmReadSuper(imageTypeName.c_str()) == -1) {
        guiintf->printMsg(wxString::Format("%s: cannot read superblock (%s)\n", cmd, err), CpmGuiInterface::msgColRed);
        return;
    }

    ret = fsck(image.c_str(), doRepair);

    if (ret & MODIFIED) {
        if (cpmSync() == -1) {
            guiintf->printMsg(wxString::Format("%s: write error on %s: %s\n", cmd, image.c_str(), strerror(errno)), CpmGuiInterface::msgColRed);
            ret |= BROKEN;
        }

        guiintf->printMsg(wxString::Format("%s: FILE SYSTEM ON %s MODIFIED", cmd, image.c_str()), CpmGuiInterface::msgColBlue);

        if (ret & BROKEN) {
            guiintf->printMsg(wxString::Format(", PLEASE CHECK AGAIN"), CpmGuiInterface::msgColBlue);
        }

        guiintf->printMsg(wxString::Format("\n"), CpmGuiInterface::msgColBlue);
    }

    cpmUmount();
}

// --------------------------------------------------------------------------------
// Basic File Input/Output
// --------------------------------------------------------------------------------
//
// --------------------------------------------------------------------------------
const char *CpmTools::deviceOpen(const char *filename, const char *mode) {
    device.file = fopen(filename, mode);
    device.opened = ((device.file == nullptr) ? false : true);
    return ((device.opened) ? (const char *)0 : strerror(errno));
}

// --------------------------------------------------------------------------------
void CpmTools::deviceSetGeometry(int secLength, int sectrk, int tracks, long offset) {
    device.secLength = secLength;
    device.sectrk = sectrk;
    device.tracks = tracks;
    device.offset = offset;
}

//--------------------------------------------------------------------------------
const char *CpmTools::deviceClose() {
    device.opened = false;
    return ((fclose(device.file) == 0) ? (const char *)0 : strerror(errno));
}

// --------------------------------------------------------------------------------
const char *CpmTools::deviceReadSector(int track, int sector, char *buffer) {
    if (fseek(device.file, (((sector + (track * device.sectrk)) * device.secLength) + device.offset), SEEK_SET) != 0) {
        return (strerror(errno));
    }

    int readCount = fread(buffer, sizeof(char), device.secLength, device.file);

    if (readCount != device.secLength) {
        guiintf->printMsg(wxString::Format("Read Sector: %d bytes lost.\n", (device.secLength - readCount)), CpmGuiInterface::msgColRed);
        memset((buffer + readCount), 0, device.secLength - readCount);
        return (strerror(errno));
    }

    return ((const char *)0);
}

// --------------------------------------------------------------------------------
const char *CpmTools::deviceWriteSector(int track, int sector, const char *buffer) {
    if (fseek(device.file, (((sector + (track * device.sectrk)) * device.secLength) + device.offset), SEEK_SET) != 0) {
        return (strerror(errno));
    }

    int writeCount = fwrite(buffer, sizeof(char), device.secLength, device.file);

    if (writeCount != device.secLength) {
        guiintf->printMsg(wxString::Format("Write Sector: %d bytes lost.\n", (device.secLength - writeCount)), CpmGuiInterface::msgColRed);
        return (strerror(errno));
    }

    return ((const char *)0);
}

// --------------------------------------------------------------------------------
// CP/M File-System
// --------------------------------------------------------------------------------
//
// --------------------------------------------------------------------------------
//  -- Copy string, leaving 8th bit alone
// --------------------------------------------------------------------------------
void CpmTools::memcpy7(char *dest, const char *src, int count) {
    while (count--) {
        *dest = ((*dest) & 0x80) | ((*src) & 0x7F);
        ++dest;
        ++src;
    }
}

// --------------------------------------------------------------------------------
//  -- split file name into name and extension
// --------------------------------------------------------------------------------
int CpmTools::splitFilename(const char *fullname, int type, char *name, char *ext, int *user) {
    int i, j;
    memset(name, ' ', 8);
    memset(ext, ' ', 3);

    if (!isdigit(fullname[0]) || !isdigit(fullname[1])) {
        err = "illegal CP/M filename";
        return (-1);
    }

    *user = 10 * (fullname[0] - '0') + (fullname[1] - '0');
    fullname += 2;

    if ((fullname[0] == '\0') || *user >= ((type & CPMFS_HI_USER) ? 32 : 16)) {
        err = "illegal CP/M filename";
        return (-1);
    }

    for (i = 0; i < 8 && fullname[i] && fullname[i] != '.'; ++i) {
        if (!ISFILECHAR(i, fullname[i])) {
            err = "illegal CP/M filename";
            return (-1);
        }
        else {
            name[i] = toupper(fullname[i]);
        }
    }

    if (fullname[i] == '.') {
        ++i;

        for (j = 0; j < 3 && fullname[i]; ++i, ++j) {
            if (!ISFILECHAR(1, fullname[i])) {
                err = "illegal CP/M filename";
                return (-1);
            }
            else {
                ext[j] = toupper(fullname[i]);
            }
        }

        if (i == 1 && j == 0) {
            err = "illegal CP/M filename";
            return (-1);
        }
    }

    return (0);
}

// --------------------------------------------------------------------------------
//  -- do two file names match?
// --------------------------------------------------------------------------------
int CpmTools::isMatching(int user1, const char *name1, const char *ext1, int user2, const char *name2, const char *ext2) {
    int i;

    if (user1 != user2) {
        return (0);
    }

    for (i = 0; i < 8; ++i) {
        if ((name1[i] & 0x7f) != (name2[i] & 0x7f)) {
            return (0);
        }
    }

    for (i = 0; i < 3; ++i) {
        if ((ext1[i] & 0x7f) != (ext2[i] & 0x7f)) {
            return (0);
        }
    }

    return (1);
}

// --------------------------------------------------------------------------------
//  -- convert CP/M time to UTC
// --------------------------------------------------------------------------------
time_t CpmTools::cpm2unix_time(int days, int hour, int min) {
    /* CP/M stores timestamps in local time.  We don't know which     */
    /* timezone was used and if DST was in effect.  Assuming it was   */
    /* the current offset from UTC is most sensible, but not perfect. */
    int year, days_per_year;
    static int days_per_month[] = {31, 0, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    tm tms;
    time_t lt, t;
    time(&lt);
    t = lt;
    tms = *localtime(&lt);
    tms.tm_isdst = 0;
    lt = mktime(&tms);
    lt -= t;
    tms.tm_sec = 0;
    tms.tm_min = ((min >> 4) & 0xf) * 10 + (min & 0xf);
    tms.tm_hour = ((hour >> 4) & 0xf) * 10 + (hour & 0xf);
    tms.tm_mday = 1;
    tms.tm_mon = 0;
    tms.tm_year = 78;
    tms.tm_isdst = -1;

    for (;;) {
        year = tms.tm_year + 1900;
        days_per_year = ((year % 4) == 0 && ((year % 100) || (year % 400) == 0)) ? 366 : 365;

        if (days > days_per_year) {
            days -= days_per_year;
            ++tms.tm_year;
        }
        else {
            break;
        }
    }

    for (;;) {
        days_per_month[1] = (days_per_year == 366) ? 29 : 28;

        if (days > days_per_month[tms.tm_mon]) {
            days -= days_per_month[tms.tm_mon];
            ++tms.tm_mon;
        }
        else {
            break;
        }
    }

    t = mktime(&tms) + (days - 1) * 24 * 3600;
    t -= lt;
    return (t);
}

// --------------------------------------------------------------------------------
//  -- convert UTC to CP/M time
// --------------------------------------------------------------------------------
void CpmTools::unix2cpm_time(time_t now, int *days, int *hour, int *min) {
    tm *tms;
    int i;
    tms = localtime(&now);
    *min = ((tms->tm_min / 10) << 4) | (tms->tm_min % 10);
    *hour = ((tms->tm_hour / 10) << 4) | (tms->tm_hour % 10);

    for (i = 1978, *days = 0; i < 1900 + tms->tm_year; ++i) {
        *days += 365;

        if (i % 4 == 0 && (i % 100 != 0 || i % 400 == 0)) {
            ++*days;
        }
    }

    *days += tms->tm_yday + 1;
}

// --------------------------------------------------------------------------------
//  -- convert DS to Unix time
// --------------------------------------------------------------------------------
time_t CpmTools::ds2unix_time(const DsEntry_t *entry) {
    tm tms;
    int yr;

    if (entry->minute == 0 &&
            entry->hour == 0 &&
            entry->day == 0 &&
            entry->month == 0 &&
            entry->year == 0) {
        return 0;
    }

    tms.tm_isdst = -1;
    tms.tm_sec = 0;
    tms.tm_min = BCD2BIN(entry->minute);
    tms.tm_hour = BCD2BIN(entry->hour);
    tms.tm_mday = BCD2BIN(entry->day);
    tms.tm_mon = BCD2BIN(entry->month) - 1;
    yr = BCD2BIN(entry->year);

    if (yr < 70) {
        yr += 100;
    }

    tms.tm_year = yr;
    return mktime(&tms);
}

// --------------------------------------------------------------------------------
//  -- convert Unix to DS time
// --------------------------------------------------------------------------------
void CpmTools::unix2ds_time(time_t now, DsEntry_t *entry) {
    tm *tms;
    int yr;

    if (now == 0) {
        entry->minute = entry->hour = entry->day = entry->month = entry->year = 0;
    }
    else {
        tms = localtime(&now);
        entry->minute = BIN2BCD(tms->tm_min);
        entry->hour = BIN2BCD(tms->tm_hour);
        entry->day = BIN2BCD(tms->tm_mday);
        entry->month = BIN2BCD(tms->tm_mon + 1);
        yr = tms->tm_year;

        if (yr > 100) {
            yr -= 100;
        }

        entry->year = BIN2BCD(yr);
    }
}

// --------------------------------------------------------------------------------
//  -- init allocation vector
// --------------------------------------------------------------------------------
void CpmTools::alvInit() {
    int i, j, offset, block;
    /* clean bitmap */
    memset(drive.alv, 0, drive.alvSize * sizeof(int));

    /* mark directory blocks as used */
    /* A directory may cover more blocks than an int may hold bits, */
    /* so a loop is needed. */
    for (block = 0; block < (drive.maxdir * 32 + drive.blksiz - 1) / drive.blksiz; ++block) {
        offset = block / INTBITS;
        drive.alv[offset] |= (1 << (block % INTBITS));
    }

    for (i = 0; i < drive.maxdir; ++i) { /* mark file blocks as used */
        if (drive.dir[i].status >= 0 && drive.dir[i].status <= (drive.type & CPMFS_HI_USER ? 31 : 15)) {
            for (j = 0; j < 16; ++j) {
                block = (unsigned char)drive.dir[i].pointers[j];

                if (drive.size >= 256) {
                    block += (((unsigned char)drive.dir[i].pointers[++j]) << 8);
                }

                if (block && block < drive.size) {
                    offset = block / INTBITS;
                    drive.alv[offset] |= (1 << block % INTBITS);
                }
            }
        }
    }
}

// --------------------------------------------------------------------------------
//  -- allocate a new disk block
// --------------------------------------------------------------------------------
int CpmTools::allocBlock() {
    int i, j, bits, block;

    for (i = 0; i < drive.alvSize; ++i) {
        for (j = 0, bits = drive.alv[i]; j < INTBITS; ++j) {
            if ((bits & 1) == 0) {
                block = i * INTBITS + j;

                if (block >= drive.size) {
                    err = "device full";
                    return (-1);
                }

                drive.alv[i] |= (1 << j);
                return (block);
            }

            bits >>= 1;
        }
    }

    err = "device full";
    return (-1);
}

// --------------------------------------------------------------------------------
//  -- read a (partial) block
// --------------------------------------------------------------------------------
int CpmTools::readBlock(int blockno, char *buffer, int start, int end) {
    int sect, track, counter;

    if (blockno >= drive.size) {
        err = "Attempting to access block beyond end of disk";
        return (-1);
    }

    if (end < 0) {
        end = drive.blksiz / drive.secLength - 1;
    }

    sect = (blockno * (drive.blksiz / drive.secLength) + drive.sectrk * drive.boottrk) % drive.sectrk;
    track = (blockno * (drive.blksiz / drive.secLength) + drive.sectrk * drive.boottrk) / drive.sectrk;

    for (counter = 0; counter <= end; ++counter) {
        if (counter >= start) {
            if ((err = deviceReadSector(track, drive.skewtab[sect], buffer + (drive.secLength * counter)))) {
                return (-1);
            }
        }

        ++sect;

        if (sect >= drive.sectrk) {
            sect = 0;
            ++track;
        }
    }

    return (0);
}

// --------------------------------------------------------------------------------
//  -- write a (partial) block
// --------------------------------------------------------------------------------
int CpmTools::writeBlock(int blockno, const char *buffer, int start, int end) {
    int sect, track, counter;

    if (end < 0) {
        end = drive.blksiz / drive.secLength - 1;
    }

    sect = (blockno * (drive.blksiz / drive.secLength)) % drive.sectrk;
    track = (blockno * (drive.blksiz / drive.secLength)) / drive.sectrk + drive.boottrk;

    for (counter = 0; counter <= end; ++counter) {
        if (counter >= start && (err = deviceWriteSector(track, drive.skewtab[sect], buffer + (drive.secLength * counter)))) {
            return (-1);
        }

        ++sect;

        if (sect >= drive.sectrk) {
            sect = 0;
            ++track;
        }
    }

    return (0);
}

// --------------------------------------------------------------------------------
//  -- find first/next extent for a file
// --------------------------------------------------------------------------------
int CpmTools::findFileExtent(int user, const char *name, const char *ext, int start, int extno) {
    err = "file already exists";

    for (; start < drive.maxdir; ++start) {
        if (((unsigned char)drive.dir[start].status) <= (drive.type & CPMFS_HI_USER ? 31 : 15)
                && (extno == -1 || (EXTENT(drive.dir[start].extnol, drive.dir[start].extnoh) / drive.extents) == (extno / drive.extents))
                && isMatching(user, name, ext, drive.dir[start].status, drive.dir[start].name, drive.dir[start].ext)) {
            return (start);
        }
    }

    err = "file not found";
    return (-1);
}

// --------------------------------------------------------------------------------
//  -- find first free extent
// --------------------------------------------------------------------------------
int CpmTools::findFreeExtent() {
    int i;

    for (i = 0; i < drive.maxdir; ++i) if (drive.dir[i].status == (char)0xe5) {
            return (i);
        }

    err = "directory full";
    return (-1);
}

// --------------------------------------------------------------------------------
//  -- convert time stamps to CP/M format
// --------------------------------------------------------------------------------
void CpmTools::updateTimeStamps(const CpmInode_t *ino, int extent) {
    PhysDirectoryEntry_t *date;
    int ca_min, ca_hour, ca_days, u_min, u_hour, u_days;

    if (!S_ISREG(ino->mode)) {
        return;
    }

    unix2cpm_time(drive.cnotatime ? ino->ctime : ino->atime, &ca_days, &ca_hour, &ca_min);
    unix2cpm_time(ino->mtime, &u_days, &u_hour, &u_min);

    if ((drive.type & CPMFS_CPM3_DATES) && (date = drive.dir + (extent | 3))->status == 0x21) {
        drive.dirtyDirectory = 1;

        switch (extent & 3) {
            case 0: { /* first entry */
                    date->name[0] = ca_days & 0xff;
                    date->name[1] = ca_days >> 8;
                    date->name[2] = ca_hour;
                    date->name[3] = ca_min;
                    date->name[4] = u_days & 0xff;
                    date->name[5] = u_days >> 8;
                    date->name[6] = u_hour;
                    date->name[7] = u_min;
                    break;
                }

            case 1: { /* second entry */
                    date->ext[2] = ca_days & 0xff;
                    date->extnol = ca_days >> 8;
                    date->lrc = ca_hour;
                    date->extnoh = ca_min;
                    date->blkcnt = u_days & 0xff;
                    date->pointers[0] = u_days >> 8;
                    date->pointers[1] = u_hour;
                    date->pointers[2] = u_min;
                    break;
                }

            case 2: { /* third entry */
                    date->pointers[5] = ca_days & 0xff;
                    date->pointers[6] = ca_days >> 8;
                    date->pointers[7] = ca_hour;
                    date->pointers[8] = ca_min;
                    date->pointers[9] = u_days & 0xff;
                    date->pointers[10] = u_days >> 8;
                    date->pointers[11] = u_hour;
                    date->pointers[12] = u_min;
                    break;
                }
        }
    }
}

// --------------------------------------------------------------------------------
//  -- set time in datestamper file
// --------------------------------------------------------------------------------
void CpmTools::updateDsStamps(const CpmInode_t *ino, int extent) {
    DsDate_t *stamp;

    if (!S_ISREG(ino->mode)) {
        return;
    }

    if (!(drive.type & CPMFS_DS_DATES)) {
        return;
    }

    /* Get datestamp struct */
    stamp = drive.ds + extent;
    unix2ds_time(ino->mtime, &stamp->modify);
    unix2ds_time(ino->ctime, &stamp->create);
    unix2ds_time(ino->atime, &stamp->access);
    drive.dirtyDs = 1;
}

// --------------------------------------------------------------------------------
//  -- read CP/M time stamp
// --------------------------------------------------------------------------------
int CpmTools::readTimeStamps(CpmInode_t *i, int lowestExt) {
    /* variables */
    PhysDirectoryEntry_t *date;
    int u_days = 0, u_hour = 0, u_min = 0;
    int ca_days = 0, ca_hour = 0, ca_min = 0;
    int protectMode = 0;

    if ((drive.type & CPMFS_CPM3_DATES) && (date = drive.dir + (lowestExt | 3))->status == 0x21) {
        switch (lowestExt & 3) {
            case 0: { /* first entry of the four */
                    ca_days = ((unsigned char)date->name[0]) + (((unsigned char)date->name[1]) << 8);
                    ca_hour = (unsigned char)date->name[2];
                    ca_min = (unsigned char)date->name[3];
                    u_days = ((unsigned char)date->name[4]) + (((unsigned char)date->name[5]) << 8);
                    u_hour = (unsigned char)date->name[6];
                    u_min = (unsigned char)date->name[7];
                    protectMode = (unsigned char)date->ext[0];
                    break;
                }

            case 1: { /* second entry */
                    ca_days = ((unsigned char)date->ext[2]) + (((unsigned char)date->extnol) << 8);
                    ca_hour = (unsigned char)date->lrc;
                    ca_min = (unsigned char)date->extnoh;
                    u_days = ((unsigned char)date->blkcnt) + (((unsigned char)date->pointers[0]) << 8);
                    u_hour = (unsigned char)date->pointers[1];
                    u_min = (unsigned char)date->pointers[2];
                    protectMode = (unsigned char)date->pointers[3];
                    break;
                }

            case 2: { /* third one */
                    ca_days = ((unsigned char)date->pointers[5]) + (((unsigned char)date->pointers[6]) << 8);
                    ca_hour = (unsigned char)date->pointers[7];
                    ca_min = (unsigned char)date->pointers[8];
                    u_days = ((unsigned char)date->pointers[9]) + (((unsigned char)date->pointers[10]) << 8);
                    u_hour = (unsigned char)date->pointers[11];
                    u_min = (unsigned char)date->pointers[12];
                    protectMode = (unsigned char)date->pointers[13];
                    break;
                }
        }

        if (drive.cnotatime) {
            i->ctime = cpm2unix_time(ca_days, ca_hour, ca_min);
            i->atime = 0;
        }
        else {
            i->ctime = 0;
            i->atime = cpm2unix_time(ca_days, ca_hour, ca_min);
        }

        i->mtime = cpm2unix_time(u_days, u_hour, u_min);
    }
    else {
        i->atime = i->mtime = i->ctime = 0;
        protectMode = 0;
    }

    return (protectMode);
}

// --------------------------------------------------------------------------------
//  -- read datestamper time stamp
// --------------------------------------------------------------------------------
void CpmTools::readDsStamps(CpmInode_t *i, int lowestExt) {
    DsDate_t *stamp;

    if (!(drive.type & CPMFS_DS_DATES)) {
        return;
    }

    /* Get datestamp */
    stamp = drive.ds + lowestExt;
    i->mtime = ds2unix_time(&stamp->modify);
    i->ctime = ds2unix_time(&stamp->create);
    i->atime = ds2unix_time(&stamp->access);
}

// --------------------------------------------------------------------------------
//  -- match filename against a pattern
// --------------------------------------------------------------------------------
int CpmTools::recmatch(const char *a, const char *pattern) {
    int first = 1;

    while (*pattern) {
        switch (*pattern) {
            case '*': {
                    if (*a == '.' && first) {
                        return (1);
                    }

                    ++pattern;

                    while (*a) if (recmatch(a, pattern)) {
                            return (1);
                        }
                        else {
                            ++a;
                        }

                    break;
                }

            case '?': {
                    if (*a) {
                        ++a;
                        ++pattern;
                    }
                    else {
                        return (0);
                    }

                    break;
                }

            default:
                if (tolower(*a) == tolower(*pattern)) {
                    ++a;
                    ++pattern;
                }
                else {
                    return (0);
                }
        }

        first = 0;
    }

    return (*pattern == '\0' && *a == '\0');
}

// --------------------------------------------------------------------------------
//  -- match filename against a pattern
// --------------------------------------------------------------------------------
int CpmTools::match(const char *a, const char *pattern) {
    int user;
    char pat[257];

    if (isdigit(*pattern) && *(pattern + 1) == ':') {
        user = (*pattern - '0');
        pattern += 2;
    }
    else if (isdigit(*pattern) && isdigit(*(pattern + 1)) && *(pattern + 2) == ':') {
        user = (10 * (*pattern - '0') + (*(pattern + 1) - '0'));
        pattern += 3;
    }
    else {
        user = -1;
    }

    if (user == -1) {
        sprintf(pat, "??%s", pattern);
    }
    else {
        sprintf(pat, "%02d%s", user, pattern);
    }

    return recmatch(a, pat);
}

// --------------------------------------------------------------------------------
//  -- expand CP/M style wildcards
// --------------------------------------------------------------------------------
void CpmTools::cpmglob(const char *argv, int *gargc, char ***gargv) {
    CpmFile_t dir;
    int entries, dirsize = 0;
    CpmDirent_t *dirent = (CpmDirent_t *)0;
    int gargcap = 0, j;
    *gargv = (char **)0;
    *gargc = 0;
    cpmOpendir(&dir);
    entries = 0;
    dirsize = 8;
    dirent = (CpmDirent_t *)malloc(sizeof(CpmDirent_t) * dirsize);

    while (cpmReaddir(&dir, &dirent[entries])) {
        ++entries;

        if (entries == dirsize) {
            dirent = (CpmDirent_t *)realloc(dirent, sizeof(CpmDirent_t) * (dirsize *= 2));
        }
    }

    int found;

    for (j = 0, found = 0; j < entries; ++j) {
        if (match(dirent[j].name, argv)) {
            if (*gargc == gargcap) {
                *gargv = (char **)realloc(*gargv, sizeof(char *) * (gargcap ? (gargcap *= 2) : (gargcap = 16)));
            }

            (*gargv)[*gargc] = strcpy((char *)malloc(strlen(dirent[j].name) + 1), dirent[j].name);
            ++*gargc;
            ++found;
        }
    }

    free(dirent);
}

// --------------------------------------------------------------------------------
//  -- read super block from diskdefs file
// --------------------------------------------------------------------------------
int CpmTools::diskdefReadSuper(const char *format) {
    char line[256];
    int ln;
    FILE *fp;
    int insideDef = 0, found = 0;
    drive.type = 0;

    if ((fp = fopen(diskdefsPath + "diskdefs", "r")) == (FILE *)0) {
        guiintf->printMsg(wxString::Format("%s: `diskdefs' couldn't be opened.\n", cmd));
        return (1);
    }

    ln = 1;

    while (fgets(line, sizeof(line), fp) != (char *)0) {
        int argc;
        char *argv[2];
        char *s;
        /* Allow inline comments preceded by ; or # */
        s = strchr(line, '#');

        if (s) {
            strcpy(s, "\n");
        }

        s = strchr(line, ';');

        if (s) {
            strcpy(s, "\n");
        }

        for (argc = 0; argc < 1 && (argv[argc] = strtok(argc ? (char *)0 : line, " \t\n")); ++argc);

        if ((argv[argc] = strtok((char *)0, "\n")) != (char *)0) {
            ++argc;
        }

        if (insideDef) {
            if (argc == 1 && strcmp(argv[0], "end") == 0) {
                insideDef = 0;
                drive.size = (drive.secLength * drive.sectrk * (drive.tracks - drive.boottrk)) / drive.blksiz;

                if (drive.extents == 0) {
                    drive.extents = ((drive.size >= 256 ? 8 : 16) * drive.blksiz) / 16384;
                }

                if (drive.extents == 0) {
                    drive.extents = 1;
                }

                if (found) {
                    break;
                }
            }
            else if (argc == 2) {
                if (strcmp(argv[0], "seclen") == 0) {
                    drive.secLength = strtol(argv[1], (char **)0, 0);
                }
                else if (strcmp(argv[0], "tracks") == 0) {
                    drive.tracks = strtol(argv[1], (char **)0, 0);
                }
                else if (strcmp(argv[0], "sectrk") == 0) {
                    drive.sectrk = strtol(argv[1], (char **)0, 0);
                }
                else if (strcmp(argv[0], "blocksize") == 0) {
                    drive.blksiz = strtol(argv[1], (char **)0, 0);

                    if (drive.blksiz <= 0) {
                        guiintf->printMsg(wxString::Format("%s: invalid blocksize `%s' in line %d\n", cmd, argv[1], ln));
                        return (1);
                    }
                }
                else if (strcmp(argv[0], "maxdir") == 0) {
                    drive.maxdir = strtol(argv[1], (char **)0, 0);
                }
                else if (strcmp(argv[0], "skew") == 0) {
                    drive.skew = strtol(argv[1], (char **)0, 0);
                }
                else if (strcmp(argv[0], "skewtab") == 0) {
                    int pass, sectors;

                    for (pass = 0; pass < 2; ++pass) {
                        sectors = 0;

                        for (s = argv[1]; *s;) {
                            int phys;
                            char *end;
                            phys = strtol(s, &end, 10);

                            if (pass == 1) {
                                drive.skewtab[sectors] = phys;
                            }

                            if (end == s) {
                                guiintf->printMsg(wxString::Format("%s: invalid skewtab `%s' at `%s' in line %d\n", cmd, argv[1], s, ln));
                                return (1);
                            }

                            s = end;
                            ++sectors;

                            if (*s == ',') {
                                ++s;
                            }
                        }

                        if (pass == 0) {
                            drive.skewtab = (int *)malloc(sizeof(int) * sectors);
                        }
                    }
                }
                else if (strcmp(argv[0], "boottrk") == 0) {
                    drive.boottrk = strtol(argv[1], (char **)0, 0);
                }
                else if (strcmp(argv[0], "offset") == 0) {
                    off_t val;
                    unsigned int multiplier;
                    char *endptr;
                    errno = 0;
                    multiplier = 1;
                    val = strtol(argv[1], &endptr, 10);

                    if ((errno == ERANGE && val == LONG_MAX) || (errno != 0 && val <= 0)) {
                        guiintf->printMsg(wxString::Format("%s: invalid offset value `%s' (%s) in line %d\n", cmd, argv[1], strerror(errno), ln));
                        return (1);
                    }

                    if (endptr == argv[1]) {
                        guiintf->printMsg(wxString::Format("%s: offset value `%s' is not a number in line %d\n", cmd, argv[1], ln));
                        return (1);
                    }

                    if (*endptr != '\0') {
                        /* Have a unit specifier */
                        switch (toupper(*endptr)) {
                            case 'K':
                                multiplier = 1024;
                                break;

                            case 'M':
                                multiplier = 1024 * 1024;
                                break;

                            case 'T':
                                if (drive.sectrk < 0 || drive.tracks < 0 || drive.secLength < 0) {
                                    guiintf->printMsg(wxString::Format("%s: offset must be specified after sectrk, tracks and secLength in line %d\n", cmd, ln));
                                    return (1);
                                }

                                multiplier = drive.sectrk * drive.secLength;
                                break;

                            case 'S':
                                if (drive.sectrk < 0 || drive.tracks < 0 || drive.secLength < 0) {
                                    guiintf->printMsg(wxString::Format("%s: offset must be specified after sectrk, tracks and secLength in line %d\n", cmd, ln));
                                    return (1);
                                }

                                multiplier = drive.secLength;
                                break;

                            default:
                                guiintf->printMsg(wxString::Format("%s: unknown unit specifier `%c' in line %d\n", cmd, *endptr, ln));
                                return (1);
                        }
                    }

                    if (val * multiplier > INT_MAX) {
                        guiintf->printMsg(wxString::Format("%s: effective offset is out of range in line %d\n", cmd, ln));
                        return (1);
                    }

                    drive.offset = val * multiplier;
                }
                else if (strcmp(argv[0], "logicalextents") == 0) {
                    drive.extents = strtol(argv[1], (char **)0, 0);
                }
                else if (strcmp(argv[0], "os") == 0) {
                    if (strcmp(argv[1], "2.2") == 0) {
                        drive.type |= CPMFS_DR22;
                    }
                    else if (strcmp(argv[1], "3") == 0) {
                        drive.type |= CPMFS_DR3;
                    }
                    else if (strcmp(argv[1], "isx") == 0) {
                        drive.type |= CPMFS_ISX;
                    }
                    else if (strcmp(argv[1], "p2dos") == 0) {
                        drive.type |= CPMFS_P2DOS;
                    }
                    else if (strcmp(argv[1], "zsys") == 0) {
                        drive.type |= CPMFS_ZSYS;
                    }
                    else {
                        guiintf->printMsg(wxString::Format("%s: invalid OS type `%s' in line %d\n", cmd, argv[1], ln));
                        return (1);
                    }
                }
            }
            else if (argc > 0 && argv[0][0] != '#' && argv[0][0] != ';') {
                guiintf->printMsg(wxString::Format("%s: invalid keyword `%s' in line %d\n", cmd, argv[0], ln));
                return (1);
            }
        }
        else if (argc == 2 && strcmp(argv[0], "diskdef") == 0) {
            insideDef = 1;
            drive.skew = 1;
            drive.extents = 0;
            drive.type = CPMFS_DR22;
            drive.skewtab = (int *)0;
            drive.offset = 0;
            drive.blksiz = drive.boottrk = drive.secLength = drive.sectrk = drive.tracks = -1;

            if (strcmp(argv[1], format) == 0) {
                found = 1;
            }
        }

        ++ln;
    }

    fclose(fp);

    if (!found) {
        guiintf->printMsg(wxString::Format("%s: unknown format %s\n", cmd, format));
        return (1);
    }

    if (drive.boottrk < 0) {
        guiintf->printMsg(wxString::Format("%s: boottrk parameter invalid or missing from diskdef\n", cmd));
        return (1);
    }

    if (drive.secLength < 0) {
        guiintf->printMsg(wxString::Format("%s: secLength parameter invalid or missing from diskdef\n", cmd));
        return (1);
    }

    if (drive.sectrk < 0) {
        guiintf->printMsg(wxString::Format("%s: sectrk parameter invalid or missing from diskdef\n", cmd));
        return (1);
    }

    if (drive.tracks < 0) {
        guiintf->printMsg(wxString::Format("%s: tracks parameter invalid or missing from diskdef\n", cmd));
        return (1);
    }

    if (drive.blksiz < 0) {
        guiintf->printMsg(wxString::Format("%s: blocksize parameter invalid or missing from diskdef\n", cmd));
        return (1);
    }

    return (0);
}

// --------------------------------------------------------------------------------
//  -- read super block from amstrad disk
// --------------------------------------------------------------------------------
int CpmTools::amsReadSuper(const char *format) {
    unsigned char boot_sector[512], *boot_spec;
    deviceSetGeometry(512, 9, 40, 0);

    if ((err = deviceReadSector(0, 0, (char *)boot_sector))) {
        guiintf->printMsg(wxString::Format("%s: Failed to read Amstrad superblock (%s)\n", cmd, err));
        return (1);
    }

    boot_spec = (boot_sector[0] == 0 || boot_sector[0] == 3) ? boot_sector : (unsigned char *)0;

    /* Check for JCE's extension to allow Amstrad and MSDOS superblocks
     * in the same sector (for the PCW16)
     */
    if
    (
        (boot_sector[0] == 0xE9 || boot_sector[0] == 0xEB)
        && !memcmp(boot_sector + 0x2B, "CP/M", 4)
        && !memcmp(boot_sector + 0x33, "DSK",  3)
        && !memcmp(boot_sector + 0x7C, "CP/M", 4)
    ) {
        boot_spec = boot_sector + 128;
    }

    if (boot_spec == (unsigned char *)0) {
        guiintf->printMsg(wxString::Format("%s: Amstrad superblock not present\n", cmd));
        return (1);
    }

    /* boot_spec[0] = format number: 0 for SS SD, 3 for DS DD
                [1] = single/double sided and density flags
                [2] = cylinders per side
                [3] = sectors per cylinder
                [4] = Physical sector shift, 2 => 512
                [5] = Reserved track count
                [6] = Block shift
                [7] = No. of directory blocks
     */
    drive.type = 0;
    drive.type |= CPMFS_DR3;	/* Amstrads are CP/M 3 systems */
    drive.secLength = 128 << boot_spec[4];
    drive.tracks = boot_spec[2];

    if (boot_spec[1] & 3) {
        drive.tracks *= 2;
    }

    drive.sectrk = boot_spec[3];
    drive.blksiz = 128 << boot_spec[6];
    drive.maxdir = (drive.blksiz / 32) * boot_spec[7];
    drive.skew = 1; /* Amstrads skew at the controller level */
    drive.skewtab = (int *)0;
    drive.boottrk = boot_spec[5];
    drive.offset = 0;
    drive.size = (drive.secLength * drive.sectrk * (drive.tracks - drive.boottrk)) / drive.blksiz;
    drive.extents = ((drive.size >= 256 ? 8 : 16) * drive.blksiz) / 16384;
    return (0);
}

// --------------------------------------------------------------------------------
//  -- read all datestamper timestamps
// --------------------------------------------------------------------------------
int CpmTools::cpmCheckDs() {
    int dsoffset, dsblks, dsrecs, off, i;
    unsigned char *buf;

    if (!isMatching(0, "!!!TIME&", "DAT", drive.dir->status, drive.dir->name, drive.dir->ext)) {
        return (-1);
    }

    /* Offset to ds file in alloc blocks */
    dsoffset = (drive.maxdir * 32 + (drive.blksiz - 1)) / drive.blksiz;
    dsrecs = (drive.maxdir + 7) / 8;
    dsblks = (dsrecs * 128 + (drive.blksiz - 1)) / drive.blksiz;
    /* Allocate buffer */
    drive.ds = (DsDate_t *)malloc(dsblks * drive.blksiz);
    /* Read ds file in its entirety */
    off = 0;

    for (i = dsoffset; i < dsoffset + dsblks; i++) {
        if (readBlock(i, ((char *)drive.ds) + off, 0, -1) == -1) {
            return (-1);
        }

        off += drive.blksiz;
    }

    /* Verify checksums */
    buf = (unsigned char *)drive.ds;

    for (i = 0; i < dsrecs; i++) {
        unsigned cksum, j;
        cksum = 0;

        for (j = 0; j < 127; j++) {
            cksum += buf[j];
        }

        if (buf[j] != (cksum & 0xff)) {
            free(drive.ds);
            drive.ds = (DsDate_t *)0;
            return (-1);
        }

        buf += 128;
    }

    return (0);
}

// --------------------------------------------------------------------------------
//  -- get DPB and init in-core data for drive
// --------------------------------------------------------------------------------
int CpmTools::cpmReadSuper(const char *format) {
    while (s_ifdir && !S_ISDIR(s_ifdir)) {
        s_ifdir <<= 1;
    }

    while (s_ifreg && !S_ISREG(s_ifreg)) {
        s_ifreg <<= 1;
    }

    if (strcmp(format, "amstrad") == 0) {
        amsReadSuper(format);
    }
    else {
        diskdefReadSuper(format);
    }

    deviceSetGeometry(drive.secLength, drive.sectrk, drive.tracks, drive.offset);

    if (drive.skewtab == (int *)0) { /* generate skew table */
        int	i, j, k;

        if ((drive.skewtab = (int *)malloc(drive.sectrk * sizeof(int))) == (int *)0) {
            err = strerror(errno);
            return (-1);
        }

        memset(drive.skewtab, 0, drive.sectrk * sizeof(int));

        for (i = j = 0; i < drive.sectrk; ++i, j = (j + drive.skew) % drive.sectrk) {
            while (1) {
                for (k = 0; k < i && drive.skewtab[k] != j; ++k);

                if (k < i) {
                    j = (j + 1) % drive.sectrk;
                }
                else {
                    break;
                }
            }

            drive.skewtab[i] = j;
        }
    }

    /* initialise allocation vector bitmap */
    {
        drive.alvSize = ((drive.secLength * drive.sectrk * (drive.tracks - drive.boottrk)) / drive.blksiz + INTBITS - 1) / INTBITS;

        if ((drive.alv = (int *)malloc(drive.alvSize * sizeof(int))) == (int *)0) {
            err = strerror(errno);
            return (-1);
        }
    }

    /* allocate directory buffer */
    if ((drive.dir = (PhysDirectoryEntry_t *) malloc(((drive.maxdir * 32 + drive.blksiz - 1) / drive.blksiz) * drive.blksiz)) == (PhysDirectoryEntry_t *)0) {
        err = strerror(errno);
        return (-1);
    }

    if (device.opened == false) { /* create empty directory in core */
        memset(drive.dir, 0xe5, drive.maxdir * 32);
    }
    else { /* read directory in core */
        int i, blocks, entry;
        blocks = (drive.maxdir * 32 + drive.blksiz - 1) / drive.blksiz;
        entry = 0;

        for (i = 0; i < blocks; ++i) {
            if (readBlock(i, (char *)(drive.dir + entry), 0, -1) == -1) {
                return (-1);
            }

            entry += (drive.blksiz / 32);
        }
    }

    alvInit();

    if (drive.type & CPMFS_CPM3_OTHER) { /* read additional superblock information */
        int i;
        /* passwords */
        {
            int passwords = 0;

            for (i = 0; i < drive.maxdir; ++i) if (drive.dir[i].status >= 16 && drive.dir[i].status <= 31) {
                    ++passwords;
                }

            if ((drive.passwdLength = passwords * PASSWD_RECLEN)) {
                if ((drive.passwd = (char *)malloc(drive.passwdLength)) == (char *)0) {
                    err = "out of memory";
                    return (-1);
                }

                for (i = 0, passwords = 0; i < drive.maxdir; ++i) if (drive.dir[i].status >= 16 && drive.dir[i].status <= 31) {
                        int j, pb;
                        char *p = drive.passwd + (passwords++*PASSWD_RECLEN);
                        p[0] = '0' + (drive.dir[i].status - 16) / 10;
                        p[1] = '0' + (drive.dir[i].status - 16) % 10;

                        for (j = 0; j < 8; ++j) {
                            p[2 + j] = drive.dir[i].name[j] & 0x7f;
                        }

                        p[10] = (drive.dir[i].ext[0] & 0x7f) == ' ' ? ' ' : '.';

                        for (j = 0; j < 3; ++j) {
                            p[11 + j] = drive.dir[i].ext[j] & 0x7f;
                        }

                        p[14] = ' ';
                        pb = (unsigned char)drive.dir[i].lrc;

                        for (j = 0; j < 8; ++j) {
                            p[15 + j] = ((unsigned char)drive.dir[i].pointers[7 - j])^pb;
                        }

                        p[23] = '\n';
                    }
            }
        }

        /* disc label */
        for (i = 0; i < drive.maxdir; ++i) if (drive.dir[i].status == (char)0x20) {
                int j;
                drive.cnotatime = drive.dir[i].extnol & 0x10;

                if (drive.dir[i].extnol & 0x1) {
                    drive.labelLength = 12;

                    if ((drive.label = (char *)malloc(drive.labelLength)) == (char *)0) {
                        err = "out of memory";
                        return -1;
                    }

                    for (j = 0; j < 8; ++j) {
                        drive.label[j] = drive.dir[i].name[j] & 0x7f;
                    }

                    for (j = 0; j < 3; ++j) {
                        drive.label[8 + j] = drive.dir[i].ext[j] & 0x7f;
                    }

                    drive.label[11] = '\n';
                }
                else {
                    drive.labelLength = 0;
                }

                break;
            }

        if (i == drive.maxdir) {
            drive.cnotatime = 1;
            drive.labelLength = 0;
        }
    }
    else {
        drive.passwdLength = 0;
        drive.cnotatime = 1;
        drive.labelLength = 0;
    }

    drive.dirtyDirectory = 0;
    root.ino = drive.maxdir;
    root.mode = (s_ifdir | 0777);
    root.size = 0;
    root.atime = root.mtime = root.ctime = 0;
    drive.dirtyDs = 0;

    if (cpmCheckDs() == 0) {
        drive.type |= CPMFS_DS_DATES;
    }
    else {
        drive.ds = (DsDate_t *)0;
    }

    return (0);
}

// --------------------------------------------------------------------------------
//  -- write all datestamper timestamps
// --------------------------------------------------------------------------------
int CpmTools::syncDs() {
    if (drive.dirtyDs) {
        int dsoffset, dsblks, dsrecs, off, i;
        unsigned char *buf;
        dsrecs = (drive.maxdir + 7) / 8;
        /* Re-calculate checksums */
        buf = (unsigned char *)drive.ds;

        for (i = 0; i < dsrecs; i++) {
            unsigned cksum, j;
            cksum = 0;

            for (j = 0; j < 127; j++) {
                cksum += buf[j];
            }

            buf[j] = cksum & 0xff;
            buf += 128;
        }

        dsoffset = (drive.maxdir * 32 + (drive.blksiz - 1)) / drive.blksiz;
        dsblks = (dsrecs * 128 + (drive.blksiz - 1)) / drive.blksiz;
        off = 0;

        for (i = dsoffset; i < dsoffset + dsblks; i++) {
            if (writeBlock(i, ((char *)(drive.ds)) + off, 0, -1) == -1) {
                return (-1);
            }

            off += drive.blksiz;
        }
    }

    return (0);
}

// --------------------------------------------------------------------------------
//  -- write directory back
// --------------------------------------------------------------------------------
int CpmTools::cpmSync() {
    if (drive.dirtyDirectory) {
        int i, blocks, entry;
        blocks = (drive.maxdir * 32 + drive.blksiz - 1) / drive.blksiz;
        entry = 0;

        for (i = 0; i < blocks; ++i) {
            if (writeBlock(i, (char *)(drive.dir + entry), 0, -1) == -1) {
                return (-1);
            }

            entry += (drive.blksiz / 32);
        }

        drive.dirtyDirectory = 0;
    }

    if (drive.type & CPMFS_DS_DATES) {
        syncDs();
    }

    return (0);
}

// --------------------------------------------------------------------------------
//  -- free actual drive
// --------------------------------------------------------------------------------
void CpmTools::cpmUmount() {
    cpmSync();

    if (drive.type & CPMFS_DS_DATES) {
        free(drive.ds);
    }

    free(drive.alv);
    free(drive.skewtab);
    free(drive.dir);

    if (drive.passwdLength) {
        free(drive.passwd);
    }

    if (drive.labelLength) {
        free(drive.label);
    }
}

// --------------------------------------------------------------------------------
//  -- map name to inode
// --------------------------------------------------------------------------------
int CpmTools::cpmNamei(const char *filename, CpmInode_t *i) {
    int user;
    char name[8], extension[3];
    int highestExtno, highestExt = -1, lowestExtno, lowestExt = -1;
    int protectMode = 0;

    if (!S_ISDIR(root.mode)) {
        err = "No such file";
        return (-1);
    }

    if (strcmp(filename, ".") == 0 || strcmp(filename, "..") == 0) { /* root directory */
        *i = root;
        return (0);
    }
    else if (strcmp(filename, "[passwd]") == 0 && drive.passwdLength) { /* access passwords */
        i->attr = 0;
        i->ino = drive.maxdir + 1;
        i->mode = s_ifreg | 0444;
        i->atime = i->mtime = i->ctime = 0;
        i->size = drive.passwdLength;
        return (0);
    }
    else if (strcmp(filename, "[label]") == 0 && drive.labelLength) { /* access label */
        i->attr = 0;
        i->ino = drive.maxdir + 2;
        i->mode = s_ifreg | 0444;
        i->atime = i->mtime = i->ctime = 0;
        i->size = drive.labelLength;
        return (0);
    }

    if (splitFilename(filename, drive.type, name, extension, &user) == -1) {
        return (-1);
    }

    /* find highest and lowest extent */
    {
        int extent;
        i->size = 0;
        extent = -1;
        highestExtno = -1;
        lowestExtno = 2049;

        while ((extent = findFileExtent(user, name, extension, extent + 1, -1)) != -1) {
            int extno = EXTENT(drive.dir[extent].extnol, drive.dir[extent].extnoh);

            if (extno > highestExtno) {
                highestExtno = extno;
                highestExt = extent;
            }

            if (extno < lowestExtno) {
                lowestExtno = extno;
                lowestExt = extent;
            }
        }
    }

    if (highestExtno == -1) {
        return (-1);
    }

    /* calculate size */
    {
        int block;
        i->size = highestExtno * 16384;

        if (drive.size < 256) for (block = 15; block >= 0; --block) {
                if (drive.dir[highestExt].pointers[block]) {
                    break;
                }
            }
        else for (block = 7; block >= 0; --block) {
                if (drive.dir[highestExt].pointers[2 * block] || drive.dir[highestExt].pointers[2 * block + 1]) {
                    break;
                }
            }

        if (drive.dir[highestExt].blkcnt) {
            i->size += ((drive.dir[highestExt].blkcnt & 0xff) - 1) * 128;

            if (drive.type & CPMFS_ISX) {
                i->size += (128 - drive.dir[highestExt].lrc);
            }
            else {
                i->size += drive.dir[highestExt].lrc ? (drive.dir[highestExt].lrc & 0xff) : 128;
            }
        }
    }
    i->ino = lowestExt;
    i->mode = s_ifreg;
    /* read timestamps */
    protectMode = readTimeStamps(i, lowestExt);
    /* Determine the inode attributes */
    i->attr = 0;

    if (drive.dir[lowestExt].name[0] & 0x80) {
        i->attr |= CPM_ATTR_F1;
    }

    if (drive.dir[lowestExt].name[1] & 0x80) {
        i->attr |= CPM_ATTR_F2;
    }

    if (drive.dir[lowestExt].name[2] & 0x80) {
        i->attr |= CPM_ATTR_F3;
    }

    if (drive.dir[lowestExt].name[3] & 0x80) {
        i->attr |= CPM_ATTR_F4;
    }

    if (drive.dir[lowestExt].ext [0] & 0x80) {
        i->attr |= CPM_ATTR_RO;
    }

    if (drive.dir[lowestExt].ext [1] & 0x80) {
        i->attr |= CPM_ATTR_SYS;
    }

    if (drive.dir[lowestExt].ext [2] & 0x80) {
        i->attr |= CPM_ATTR_ARCV;
    }

    if (protectMode & 0x20) {
        i->attr |= CPM_ATTR_PWDEL;
    }

    if (protectMode & 0x40) {
        i->attr |= CPM_ATTR_PWWRITE;
    }

    if (protectMode & 0x80) {
        i->attr |= CPM_ATTR_PWREAD;
    }

    if (drive.dir[lowestExt].ext[1] & 0x80) {
        i->mode |= 01000;
    }

    i->mode |= 0444;

    if (!(drive.dir[lowestExt].ext[0] & 0x80)) {
        i->mode |= 0222;
    }

    if (extension[0] == 'C' && extension[1] == 'O' && extension[2] == 'M') {
        i->mode |= 0111;
    }

    readDsStamps(i, lowestExt);
    return (0);
}

// --------------------------------------------------------------------------------
//  -- statfs
// --------------------------------------------------------------------------------
void CpmTools::cpmStatFS(CpmStatFS_t *buf) {
    int i;
    buf->f_bsize = drive.blksiz;
    buf->f_blocks = drive.size;
    buf->f_bfree = 0;
    buf->f_bused = -(drive.maxdir * 32 + drive.blksiz - 1) / drive.blksiz;

    for (i = 0; i < drive.alvSize; ++i) {
        int temp, j;
        temp = *(drive.alv + i);

        for (j = 0; j < INTBITS; ++j) {
            if (i * INTBITS + j < drive.size) {
                if (1 & temp) {
                    ++buf->f_bused;
                }
                else {
                    ++buf->f_bfree;
                }
            }

            temp >>= 1;
        }
    }

    buf->f_bavail = buf->f_bfree;
    buf->f_files = drive.maxdir;
    buf->f_ffree = 0;

    for (i = 0; i < drive.maxdir; ++i) {
        if (drive.dir[i].status == (char)0xe5) {
            ++buf->f_ffree;
        }
    }

    buf->f_namelen = 11;
}

// --------------------------------------------------------------------------------
//  -- unlink
// --------------------------------------------------------------------------------
int CpmTools::cpmUnlink(const char *fname) {
    int user;
    char name[8], extension[3];
    int extent;

    if (!S_ISDIR(root.mode)) {
        err = "No such file";
        return (-1);
    }

    if (splitFilename(fname, drive.type, name, extension, &user) == -1) {
        return (-1);
    }

    if ((extent = findFileExtent(user, name, extension, 0, -1)) == -1) {
        return (-1);
    }

    drive.dirtyDirectory = 1;
    drive.dir[extent].status = (char)0xe5;

    do {
        drive.dir[extent].status = (char)0xe5;
    } while ((extent = findFileExtent(user, name, extension, extent + 1, -1)) >= 0);

    alvInit();
    return (0);
}

// --------------------------------------------------------------------------------
//  -- rename
// --------------------------------------------------------------------------------
int CpmTools::cpmRename(const char *oldn, const char *newn) {
    int extent;
    int olduser;
    char oldname[8], oldext[3];
    int newuser;
    char newname[8], newext[3];

    if (!S_ISDIR(root.mode)) {
        err = "No such file";
        return (-1);
    }

    if (splitFilename(oldn, drive.type, oldname, oldext, &olduser) == -1) {
        return (-1);
    }

    if (splitFilename(newn, drive.type, newname, newext, &newuser) == -1) {
        return (-1);
    }

    if ((extent = findFileExtent(olduser, oldname, oldext, 0, -1)) == -1) {
        return (-1);
    }

    if (findFileExtent(newuser, newname, newext, 0, -1) != -1) {
        err = "file already exists";
        return (-1);
    }

    do {
        drive.dirtyDirectory = 1;
        drive.dir[extent].status = newuser;
        memcpy7(drive.dir[extent].name, newname, 8);
        memcpy7(drive.dir[extent].ext, newext, 3);
    } while ((extent = findFileExtent(olduser, oldname, oldext, extent + 1, -1)) != -1);

    return (0);
}

// --------------------------------------------------------------------------------
//  -- opendir
// --------------------------------------------------------------------------------
int CpmTools::cpmOpendir(CpmFile_t *dirp) {
    if (!S_ISDIR(root.mode)) {
        err = "No such file";
        return (-1);
    }

    dirp->ino = &root;
    dirp->pos = 0;
    dirp->mode = O_RDONLY;
    return (0);
}

// --------------------------------------------------------------------------------
//  -- readdir
// --------------------------------------------------------------------------------
int CpmTools::cpmReaddir(CpmFile_t *dir, CpmDirent_t *ent) {
    PhysDirectoryEntry_t *cur = (PhysDirectoryEntry_t *)0;
    char buf[2 + 8 + 1 + 3 + 1]; /* 00foobarxy.zzy\0 */
    char *bufp;
    int hasext;

    if (!(S_ISDIR(dir->ino->mode))) { /* error: not a directory */
        err = "not a directory";
        return (-1);
    }

    while (1) {
        int i;

        if (dir->pos == 0) { /* first entry is . */
            ent->ino = drive.maxdir;
            ent->reclen = 1;
            strcpy(ent->name, ".");
            ent->off = dir->pos;
            ++dir->pos;
            return (1);
        }
        else if (dir->pos == 1) { /* next entry is .. */
            ent->ino = drive.maxdir;
            ent->reclen = 2;
            strcpy(ent->name, "..");
            ent->off = dir->pos;
            ++dir->pos;
            return (1);
        }
        else if (dir->pos == 2) {
            if (drive.passwdLength) { /* next entry is [passwd] */
                ent->ino = drive.maxdir + 1;
                ent->reclen = 8;
                strcpy(ent->name, "[passwd]");
                ent->off = dir->pos;
                ++dir->pos;
                return (1);
            }
        }
        else if (dir->pos == 3) {
            if (drive.labelLength) { /* next entry is [label] */
                ent->ino = drive.maxdir + 2;
                ent->reclen = 7;
                strcpy(ent->name, "[label]");
                ent->off = dir->pos;
                ++dir->pos;
                return (1);
            }
        }
        else if (dir->pos >= RESERVED_ENTRIES && dir->pos < (int)drive.maxdir + RESERVED_ENTRIES) {
            int first = dir->pos - RESERVED_ENTRIES;

            if ((cur = drive.dir + (dir->pos - RESERVED_ENTRIES))->status >= 0 && cur->status <= (drive.type & CPMFS_HI_USER ? 31 : 15)) {
                /* determine first extent for the current file */
                for (i = 0; i < drive.maxdir; ++i) if (i != (dir->pos - RESERVED_ENTRIES)) {
                        if (isMatching(cur->status, cur->name, cur->ext, drive.dir[i].status, drive.dir[i].name, drive.dir[i].ext) && EXTENT(cur->extnol, cur->extnoh) > EXTENT(drive.dir[i].extnol, drive.dir[i].extnoh)) {
                            first = i;
                        }
                    }

                if (first == (dir->pos - RESERVED_ENTRIES)) {
                    ent->ino = dir->pos - RESERVED_INODES;
                    /* convert file name to UNIX style */
                    buf[0] = '0' + cur->status / 10;
                    buf[1] = '0' + cur->status % 10;

                    for (bufp = buf + 2, i = 0; i < 8 && (cur->name[i] & 0x7f) != ' '; ++i) {
                        *bufp++ = tolower(cur->name[i] & 0x7f);
                    }

                    for (hasext = 0, i = 0; i < 3 && (cur->ext[i] & 0x7f) != ' '; ++i) {
                        if (!hasext) {
                            *bufp++ = '.';
                            hasext = 1;
                        }

                        *bufp++ = tolower(cur->ext[i] & 0x7f);
                    }

                    *bufp = '\0';
                    ent->reclen = bufp - buf;
                    strcpy(ent->name, buf);
                    ent->off = dir->pos;
                    ++dir->pos;
                    return (1);
                }
            }
        }
        else {
            return (0);
        }

        ++dir->pos;
    }
}

// --------------------------------------------------------------------------------
//  -- stat
// --------------------------------------------------------------------------------
void CpmTools::cpmStat(const CpmInode_t *ino, CpmStat_t *buf) {
    buf->ino = ino->ino;
    buf->mode = ino->mode;
    buf->size = ino->size;
    buf->atime = ino->atime;
    buf->mtime = ino->mtime;
    buf->ctime = ino->ctime;
}

// --------------------------------------------------------------------------------
//  -- open
// --------------------------------------------------------------------------------
int CpmTools::cpmOpen(CpmInode_t *ino, CpmFile_t *file, mode_t mode) {
    if (S_ISREG(ino->mode)) {
        if ((mode & O_WRONLY) && (ino->mode & 0222) == 0) {
            err = "permission denied";
            return (-1);
        }

        file->pos = 0;
        file->ino = ino;
        file->mode = mode;
        return (0);
    }
    else {
        err = "not a regular file";
        return (-1);
    }
}

// --------------------------------------------------------------------------------
//  -- read
// --------------------------------------------------------------------------------
int CpmTools::cpmRead(CpmFile_t *file, char *buf, int count) {
    int findext = 1, findblock = 1, extent = -1, block = -1, extentno = -1, got = 0, nextblockpos = -1, nextextpos = -1;
    int blocksize = drive.blksiz;
    int extcap;
    extcap = (drive.size < 256 ? 16 : 8) * blocksize;

    if (extcap > 16384) {
        extcap = 16384 * drive.extents;
    }

    if (file->ino->ino == (ino_t)drive.maxdir + 1) {   /* [passwd] */
        if ((file->pos + count) > file->ino->size) {
            count = file->ino->size - file->pos;
        }

        if (count) {
            memcpy(buf, drive.passwd + file->pos, count);
        }

        file->pos += count;
        return (count);
    }
    else if (file->ino->ino == (ino_t)drive.maxdir + 2) {  /* [label] */
        if ((file->pos + count) > file->ino->size) {
            count = file->ino->size - file->pos;
        }

        if (count) {
            memcpy(buf, drive.label + file->pos, count);
        }

        file->pos += count;
        return (count);
    }
    else while (count > 0 && file->pos < file->ino->size) {
            char buffer[16384];

            if (findext) {
                extentno = file->pos / 16384;
                extent = findFileExtent(drive.dir[file->ino->ino].status, drive.dir[file->ino->ino].name, drive.dir[file->ino->ino].ext, 0, extentno);
                nextextpos = (file->pos / extcap) * extcap + extcap;
                findext = 0;
                findblock = 1;
            }

            if (findblock) {
                if (extent != -1) {
                    int start, end, ptr;
                    ptr = (file->pos % extcap) / blocksize;

                    if (drive.size >= 256) {
                        ptr *= 2;
                    }

                    block = (unsigned char)drive.dir[extent].pointers[ptr];

                    if (drive.size >= 256) {
                        block += ((unsigned char)drive.dir[extent].pointers[ptr + 1]) << 8;
                    }

                    if (block == 0) {
                        memset(buffer, 0, blocksize);
                    }
                    else {
                        start = (file->pos % blocksize) / drive.secLength;
                        end = ((file->pos % blocksize + count) > blocksize ? blocksize - 1 : (file->pos % blocksize + count - 1)) / drive.secLength;

                        if (readBlock(block, buffer, start, end) == -1) {
                            if (got == 0) {
                                got = -1;
                            }

                            break;
                        }
                    }
                }

                nextblockpos = (file->pos / blocksize) * blocksize + blocksize;
                findblock = 0;
            }

            if (file->pos < nextblockpos) {
                if (extent == -1) {
                    *buf++ = '\0';
                }
                else {
                    *buf++ = buffer[file->pos % blocksize];
                }

                ++file->pos;
                ++got;
                --count;
            }
            else if (file->pos == nextextpos) {
                findext = 1;
            }
            else {
                findblock = 1;
            }
        }

    return (got);
}

// --------------------------------------------------------------------------------
//  -- write
// --------------------------------------------------------------------------------
int CpmTools::cpmWrite(CpmFile_t *file, const char *buf, int count) {
    int findext = 1, findblock = -1, extent = -1, extentno = -1, got = 0, nextblockpos = -1, nextextpos = -1;
    int blocksize = drive.blksiz;
    int extcap = (drive.size < 256 ? 16 : 8) * blocksize;
    int block = -1, start = -1, end = -1, ptr = -1, last = -1;
    char buffer[16384];

    while (count > 0) {
        if (findext) {
            extentno = file->pos / 16384;
            extent = findFileExtent(drive.dir[file->ino->ino].status, drive.dir[file->ino->ino].name, drive.dir[file->ino->ino].ext, 0, extentno);
            nextextpos = (file->pos / extcap) * extcap + extcap;

            if (extent == -1) {
                if ((extent = findFreeExtent()) == -1) {
                    return (got == 0 ? -1 : got);
                }

                drive.dir[extent] = drive.dir[file->ino->ino];
                memset(drive.dir[extent].pointers, 0, 16);
                drive.dir[extent].extnol = EXTENTL(extentno);
                drive.dir[extent].extnoh = EXTENTH(extentno);
                drive.dir[extent].blkcnt = 0;
                drive.dir[extent].lrc = 0;
                time(&file->ino->ctime);
                updateTimeStamps(file->ino, extent);
                updateDsStamps(file->ino, extent);
            }

            findext = 0;
            findblock = 1;
        }

        if (findblock) {
            ptr = (file->pos % extcap) / blocksize;

            if (drive.size >= 256) {
                ptr *= 2;
            }

            block = (unsigned char)drive.dir[extent].pointers[ptr];

            if (drive.size >= 256) {
                block += ((unsigned char)drive.dir[extent].pointers[ptr + 1]) << 8;
            }

            if (block == 0) {   /* allocate new block, set start/end to cover it */
                if ((block = allocBlock()) == -1) {
                    return (got == 0 ? -1 : got);
                }

                drive.dir[extent].pointers[ptr] = block & 0xff;

                if (drive.size >= 256) {
                    drive.dir[extent].pointers[ptr + 1] = (block >> 8) & 0xff;
                }

                start = 0;
                /* By setting end to the end of the block and not the end
                 * of the currently written data, the whole block gets
                 * wiped from the disk, which is slow, but convenient in
                 * case of sparse files.
                 */
                end = (blocksize - 1) / drive.secLength;
                memset(buffer, 0, blocksize);
                time(&file->ino->ctime);
                updateTimeStamps(file->ino, extent);
                updateDsStamps(file->ino, extent);
            }
            else  { /* read existing block and set start/end to cover modified parts */
                start = (file->pos % blocksize) / drive.secLength;
                end = ((file->pos % blocksize + count) >= blocksize ? blocksize - 1 : (file->pos % blocksize + count - 1)) / drive.secLength;

                if (file->pos % drive.secLength) {
                    if (readBlock(block, buffer, start, start) == -1) {
                        if (got == 0) {
                            got = -1;
                        }

                        break;
                    }
                }

                if (end != start && file->pos % blocksize + count < blocksize && (file->pos + count) % drive.secLength) {
                    if (readBlock(block, buffer, end, end) == -1) {
                        if (got == 0) {
                            got = -1;
                        }

                        break;
                    }
                }
            }

            nextblockpos = (file->pos / blocksize) * blocksize + blocksize;
            findblock = 0;
        }

        /* fill block and write it */
        drive.dirtyDirectory = 1;

        while (file->pos != nextblockpos && count) {
            buffer[file->pos % blocksize] = *buf++;
            ++file->pos;

            if (file->ino->size < file->pos) {
                file->ino->size = file->pos;
            }

            ++got;
            --count;
        }

        /* In case the data only fills part of a sector, the rest is
         * still initialized: A new block was cleared and the boundaries
         * of an existing block were read.
         */
        writeBlock(block, buffer, start, end);
        time(&file->ino->mtime);

        if (drive.size < 256) for (last = 15; last >= 0; --last) {
                if (drive.dir[extent].pointers[last]) {
                    break;
                }
            }
        else for (last = 14; last > 0; last -= 2) {
                if (drive.dir[extent].pointers[last] || drive.dir[extent].pointers[last + 1]) {
                    last /= 2;
                    break;
                }
            }

        if (last > 0) {
            extentno += (last * blocksize) / extcap;
        }

        drive.dir[extent].extnol = EXTENTL(extentno);
        drive.dir[extent].extnoh = EXTENTH(extentno);
        drive.dir[extent].blkcnt = ((file->pos - 1) % 16384) / 128 + 1;

        if (drive.type & CPMFS_EXACT_SIZE) {
            drive.dir[extent].lrc = (128 - (file->pos % 128)) & 0x7F;
        }
        else {
            drive.dir[extent].lrc = file->pos % 128;
        }

        updateTimeStamps(file->ino, extent);
        updateDsStamps(file->ino, extent);

        if (file->pos == nextextpos) {
            findext = 1;
        }
        else if (file->pos == nextblockpos) {
            findblock = 1;
        }
    }

    return (got);
}

// --------------------------------------------------------------------------------
//  -- close
// --------------------------------------------------------------------------------
int CpmTools::cpmClose(CpmFile_t *file) {
    return (0);
}

// --------------------------------------------------------------------------------
//  -- creat new cpm file
// --------------------------------------------------------------------------------
int CpmTools::cpmCreat(CpmInode_t *dir, const char *fname, CpmInode_t *ino, mode_t mode) {
    int user;
    char name[8], extension[3];
    int extent;
    PhysDirectoryEntry_t *ent;

    if (!S_ISDIR(dir->mode)) {
        err = "No such file or directory";
        return (-1);
    }

    if (splitFilename(fname, drive.type, name, extension, &user) == -1) {
        return (-1);
    }

    if (findFileExtent(user, name, extension, 0, -1) != -1) {
        return (-1);
    }

    if ((extent = findFreeExtent()) == -1) {
        return (-1);
    }

    ent = drive.dir + extent;
    drive.dirtyDirectory = 1;
    memset(ent, 0, 32);
    ent->status = user;
    memcpy(ent->name, name, 8);
    memcpy(ent->ext, extension, 3);
    ino->ino = extent;
    ino->mode = s_ifreg | mode;
    ino->size = 0;
    time(&ino->atime);
    time(&ino->mtime);
    time(&ino->ctime);
    updateTimeStamps(ino, extent);
    updateDsStamps(ino, extent);
    return (0);
}

// --------------------------------------------------------------------------------
//  -- get CP/M attributes
// --------------------------------------------------------------------------------
int CpmTools::cpmAttrGet(CpmInode_t *ino, cpm_attr_t *attrib) {
    *attrib = ino->attr;
    return (0);
}

// --------------------------------------------------------------------------------
//  -- set CP/M attributes
// --------------------------------------------------------------------------------
int CpmTools::cpmAttrSet(CpmInode_t *ino, cpm_attr_t attrib) {
    int extent;
    int user;
    char name[8], extension[3];
    memset(name,      0, sizeof(name));
    memset(extension, 0, sizeof(extension));
    extent = ino->ino;
    drive.dirtyDirectory = 1;
    /* Strip off existing attribute bits */
    memcpy7(name, drive.dir[extent].name, 8);
    memcpy7(extension, drive.dir[extent].ext,  3);
    user = drive.dir[extent].status;

    /* And set new ones */
    if (attrib & CPM_ATTR_F1) {
        name[0]      |= 0x80;
    }

    if (attrib & CPM_ATTR_F2) {
        name[1]      |= 0x80;
    }

    if (attrib & CPM_ATTR_F3) {
        name[2]      |= 0x80;
    }

    if (attrib & CPM_ATTR_F4) {
        name[3]      |= 0x80;
    }

    if (attrib & CPM_ATTR_RO) {
        extension[0] |= 0x80;
    }

    if (attrib & CPM_ATTR_SYS) {
        extension[1] |= 0x80;
    }

    if (attrib & CPM_ATTR_ARCV) {
        extension[2] |= 0x80;
    }

    do {
        memcpy(drive.dir[extent].name, name, 8);
        memcpy(drive.dir[extent].ext, extension, 3);
    } while ((extent = findFileExtent(user, name, extension, extent + 1, -1)) != -1);

    /* Update the stored (inode) copies of the file attributes and mode */
    ino->attr = attrib;

    if (attrib & CPM_ATTR_RO) {
        ino->mode &= ~(S_IWUSR | S_IWGRP | S_IWOTH);
    }
    else {
        ino->mode |= (S_IWUSR | S_IWGRP | S_IWOTH);
    }

    return (0);
}

// --------------------------------------------------------------------------------
//  -- set CP/M file protection mode
// --------------------------------------------------------------------------------
int CpmTools::cpmProtSet(CpmInode_t *ino, mode_t pmode) {
    PhysDirectoryEntry_t *date;
    int protectMode = 0;
    int lowestExt = ino->ino;

    if (pmode & CPM_ATTR_PWDEL) {
        protectMode |= 0x20;
    }

    if (pmode & CPM_ATTR_PWWRITE) {
        protectMode |= 0x40;
    }

    if (pmode & CPM_ATTR_PWREAD) {
        protectMode |= 0x80;
    }

    if ((drive.type & CPMFS_CPM3_DATES) && (date = drive.dir + (lowestExt | 3))->status == 0x21) {
        drive.dirtyDirectory = 1;

        switch (lowestExt & 3) {
            case 0: {
                    date->ext[0] = protectMode;
                    break;
                }

            case 1: {
                    date->pointers[3] = protectMode;
                    break;
                }

            case 2: {
                    date->pointers[13] = protectMode;
                    break;
                }
        }
    }

    return (0);
}

// --------------------------------------------------------------------------------
//  -- set timestamps
// --------------------------------------------------------------------------------
void CpmTools::cpmUtime(CpmInode_t *ino, utimbuf *times) {
    ino->atime = times->actime;
    ino->mtime = times->modtime;
    time(&ino->ctime);
    updateTimeStamps(ino, ino->ino);
    updateDsStamps(ino, ino->ino);
}

// --------------------------------------------------------------------------------
// CP/M Tool-Functions
// --------------------------------------------------------------------------------
//
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
//  -- create new empty binary Image-File
// --------------------------------------------------------------------------------
int CpmTools::mkfs(const char *format, const char *label, char *bootTracks, int timeStamps) {
    /* variables */
    unsigned int i;
    char buf[128];
    char firstbuf[128];
    FILE *fd;
    unsigned int bytes;
    unsigned int trkbytes;

    /* check for empty label */
    if (strcmp(label, "") == 0) {
        label = "UNLABELED";
    }

    /* open image file */
    if ((fd = fopen(imageFileName.c_str(), "w+b")) == (FILE *)0) {
        err = strerror(errno);
        return -1;
    }

    /* write system tracks */
    /* this initialises only whole tracks, so it skew is not an issue */
    trkbytes = drive.secLength * drive.sectrk;

    for (i = 0; i < trkbytes * drive.boottrk; i += drive.secLength) {
        if (fwrite(bootTracks + i, sizeof(char), drive.secLength, fd) != (size_t)drive.secLength) {
            err = strerror(errno);
            fclose(fd);
            return -1;
        }
    }

    /* write directory */
    memset(buf, 0xe5, 128);
    bytes = drive.maxdir * 32;

    if (bytes % trkbytes) {
        bytes = ((bytes + trkbytes) / trkbytes) * trkbytes;
    }

    if (timeStamps && (drive.type == CPMFS_P2DOS || drive.type == CPMFS_DR3)) {
        buf[3 * 32] = 0x21;
    }

    memcpy(firstbuf, buf, 128);

    if (drive.type == CPMFS_DR3) {
        time_t now;
        struct tm *t;
        int min, hour, days;
        firstbuf[0] = 0x20;

        for (i = 0; i < 11 && *label; ++i, ++label) {
            firstbuf[1 + i] = toupper(*label & 0x7f);
        }

        while (i < 11) {
            firstbuf[1 + i++] = ' ';
        }

        firstbuf[12] = timeStamps ? 0x11 : 0x01; /* label set and first time stamp is creation date */
        memset(&firstbuf[13], 0, 1 + 2 + 8);

        if (timeStamps) {
            int year;
            /* Stamp label. */
            time(&now);
            t = localtime(&now);
            min = ((t->tm_min / 10) << 4) | (t->tm_min % 10);
            hour = ((t->tm_hour / 10) << 4) | (t->tm_hour % 10);

            for (year = 1978, days = 0; year < 1900 + t->tm_year; ++year) {
                days += 365;

                if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) {
                    ++days;
                }
            }

            days += t->tm_yday + 1;
            firstbuf[24] = firstbuf[28] = days & 0xff;
            firstbuf[25] = firstbuf[29] = days >> 8;
            firstbuf[26] = firstbuf[30] = hour;
            firstbuf[27] = firstbuf[31] = min;
        }
    }

    for (i = 0; i < bytes; i += 128) {
        if (fwrite((i == 0 ? firstbuf : buf), sizeof(char), 128, fd) != 128) {
            err = strerror(errno);
            fclose(fd);
            return -1;
        }
    }

    /* fill remaining size */
    memset(buf, 0xe5, 128);
    int imagesize = (drive.secLength * drive.sectrk * drive.tracks);

    for (i = 0; i < (imagesize - (bytes + (trkbytes * drive.boottrk))); i += 128) {
        if (fwrite(buf, sizeof(char), 128, fd) != 128) {
            err = strerror(errno);
            fclose(fd);
            return -1;
        }
    }

    /* close image file */
    if (fclose(fd) == -1) {
        err = strerror(errno);
        return -1;
    }

    if (timeStamps && !(drive.type == CPMFS_P2DOS || drive.type == CPMFS_DR3)) { /*{{{*/
        int offset, j;
        CpmInode_t ino;
        static const char sig[] = "!!!TIME";
        unsigned int records;
        DsDate_t *ds;

        if ((err = deviceOpen(imageFileName.c_str(), "r+b"))) {
            guiintf->printMsg(wxString::Format("%s: cannot open %s (%s)\n", cmd, imageFileName, err), CpmGuiInterface::msgColRed);
            exit(1);
        }

        cpmReadSuper(format);
        records = drive.maxdir / 8;

        if (!(ds = (DsDate_t *)malloc(records * 128))) {
            cpmUmount();
            return -1;
        }

        memset(ds, 0, records * 128);
        offset = 15;

        for (i = 0; i < records; i++) {
            for (j = 0; j < 7; j++, offset += 16) {
                *((char *)ds + offset) = sig[j];
            }

            /* skip checksum byte */
            offset += 16;
        }

        /* Set things up so cpmSync will generate checksums and write the * file. */
        if (cpmCreat(&root, "00!!!TIME&.DAT", &ino, 0) == -1) {
            guiintf->printMsg(wxString::Format("%s: Unable to create DateStamper file: %s\n", cmd, err), CpmGuiInterface::msgColRed);
            return -1;
        }

        drive.dirtyDs = 1;
        cpmUmount();
    }

    return 0;
}

// --------------------------------------------------------------------------------
//  -- check format and range of BCD digit
// --------------------------------------------------------------------------------
int CpmTools::bcdCheck(int n, int max, const char *msg, const char *unit, int extent1, int extent2) {
    if (((n >> 4) & 0xf) > 10 || (n & 0xf) > 10 || (((n >> 4) & 0xf) * 10 + (n & 0xf)) >= max) {
        guiintf->printMsg(wxString::Format("    Bad %s %s (extent=%d/%d, %s=%02x)\n", msg, unit, extent1, extent2, unit, (n & 0xff)), CpmGuiInterface::msgColRed);
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

    for (i = 0; i < 8; ++i) if ((c = ((char)(pwd[7 - i] ^ decode))) < ' ' || c & 0x80) {
            wxString passwd;

            for (i = 0; i < 8; ++i) {
                c = pwd[7 - i] ^ decode;

                if (c < ' ' || c & 0x80) {
                    passwd += "\\";
                    passwd += ('0' + ((c >> 6) & 0x03));
                    passwd += ('0' + ((c >> 3) & 0x03));
                    passwd += ('0' + (c & 0x03));
                }
                else {
                    passwd += (c);
                }
            }

            guiintf->printMsg(wxString::Format("    Non-printable character in password (extent=%d, password=%s)\n", extent, passwd), CpmGuiInterface::msgColRed);
            return -1;
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
    PhysDirectoryEntry_t *dir;
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
    /* variables */
    int ret = OK;
    int extent, extent2;
    PhysDirectoryEntry_t *dir, *dir2;
    /* Phase 1: check extent fields */
    guiintf->printMsg("====================================================================================================\n", CpmGuiInterface::msgColGreen);
    guiintf->printMsg("  Phase 1: check extent fields\n", CpmGuiInterface::msgColGreen);

    for (extent = 0; extent < drive.maxdir; ++extent) {
        char *status;
        int usedBlocks = 0;
        dir = drive.dir + extent;
        status = &dir->status;

        if (*status >= 0 && *status <= (drive.type == CPMFS_P2DOS ? 31 : 15)) { /* directory entry */
            /* check name and extension */
            {
                int i;
                char *c;

                for (i = 0; i < 8; ++i) {
                    c = &(dir->name[i]);

                    if (!ISFILECHAR(i, *c & 0x7f) || islower(*c & 0x7f)) {
                        guiintf->printMsg(wxString::Format("    Error: Bad name (extent=%d, name=\"%s\", position=%d)\n", extent, prfile(extent), i), CpmGuiInterface::msgColGreen);

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
                        guiintf->printMsg(wxString::Format("    Error: Bad name (extent=%d, name=\"%s\", position=%d)\n", extent, prfile(extent), i), CpmGuiInterface::msgColGreen);

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
                guiintf->printMsg(wxString::Format("    Error: Bad lower bits of extent number (extent=%d, name=\"%s\", low bits=%d)\n", extent, prfile(extent), dir->extnol & 0xff), CpmGuiInterface::msgColGreen);

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
                guiintf->printMsg(wxString::Format("    Error: Bad higher bits of extent number (extent=%d, name=\"%s\", high bits=%d)\n", extent, prfile(extent), dir->extnoh & 0xff), CpmGuiInterface::msgColGreen);

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
                guiintf->printMsg(wxString::Format("    Error: Bad last record byte count (extent=%d, name=\"%s\", lrc=%d)\n", extent, prfile(extent), dir->lrc & 0xff), CpmGuiInterface::msgColGreen);

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
                min = (drive.maxdir * 32 + drive.blksiz - 1) / drive.blksiz;
                max = drive.size;

                for (i = 0; i < 16; ++i) {
                    block = dir->pointers[i] & 0xff;

                    if (drive.size >= 256) {
                        block += (dir->pointers[++i] & 0xff) << 8;
                    }

                    if (block > 0) {
                        ++usedBlocks;

                        if (block < min || block >= max) {
                            guiintf->printMsg(wxString::Format("    Error: Bad block number (extent=%d, name=\"%s\", block=%d)\n", extent, prfile(extent), block), CpmGuiInterface::msgColGreen);

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
                assert(min < max);

                for (i = min; i < max; ++i) {
                    /* [JCE] Rewritten because the previous implementation didn't work
                     *       properly with Visual C++ */
                    if (dir->pointers[i] || (drive.size >= 256 && dir->pointers[i + 1])) {
                        ++used;
                    }

                    if (drive.size >= 256) {
                        ++i;
                    }
                }

                recordsInBlocks = (((unsigned char)dir->blkcnt) * 128 + drive.blksiz - 1) / drive.blksiz;

                if (recordsInBlocks != used) {
                    guiintf->printMsg(wxString::Format("    Error: Bad record count (extent=%d, name=\"%s\", record count=%d)\n", extent, prfile(extent), dir->blkcnt & 0xff), CpmGuiInterface::msgColGreen);

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
            if (((EXTENT(dir->extnol, dir->extnoh) == 3 && dir->blkcnt >= 126) || EXTENT(dir->extnol, dir->extnoh) >= 4) && (dir->ext[0] & 0x7f) == 'C' && (dir->ext[1] & 0x7f) == 'O' && (dir->ext[2] & 0x7f) == 'M') {
                guiintf->printMsg(wxString::Format("    Warning: Oversized .COM file (extent=%d, name=\"%s\")\n", extent, prfile(extent)), CpmGuiInterface::msgColGreen);
            }
        }
        else if ((drive.type == CPMFS_P2DOS || drive.type == CPMFS_DR3) && *status == 33) { /* check time stamps ? */
            unsigned long created, modified;
            char s;

            if ((s = drive.dir[extent2 = (extent & ~3)].status) >= 0 && s <= (drive.type == CPMFS_P2DOS ? 31 : 15)) { /* time stamps for first of the three extents */
                bcdCheck(dir->name[2], 24, drive.cnotatime ? "creation date" : "access date", "hour", extent, extent2);
                bcdCheck(dir->name[3], 60, drive.cnotatime ? "creation date" : "access date", "minute", extent, extent2);
                bcdCheck(dir->name[6], 24, "modification date", "hour", extent, extent2);
                bcdCheck(dir->name[7], 60, "modification date", "minute", extent, extent2);
                created = (dir->name[4] + (dir->name[1] << 8)) * (0x60 * 0x60) + dir->name[2] * 0x60 + dir->name[3];
                modified = (dir->name[0] + (dir->name[5] << 8)) * (0x60 * 0x60) + dir->name[6] * 0x60 + dir->name[7];

                if (drive.cnotatime && modified < created) {
                    guiintf->printMsg(wxString::Format("    Warning: Modification date earlier than creation date (extent=%d/%d)\n", extent, extent2), CpmGuiInterface::msgColGreen);
                }
            }

            if ((s = drive.dir[extent2 = (extent & ~3) + 1].status) >= 0 && s <= (drive.type == CPMFS_P2DOS ? 31 : 15)) { /* time stamps for second */
                bcdCheck(dir->lrc, 24, drive.cnotatime ? "creation date" : "access date", "hour", extent, extent2);
                bcdCheck(dir->extnoh, 60, drive.cnotatime ? "creation date" : "access date", "minute", extent, extent2);
                bcdCheck(dir->pointers[1], 24, "modification date", "hour", extent, extent2);
                bcdCheck(dir->pointers[2], 60, "modification date", "minute", extent, extent2);
                created = (dir->ext[2] + (dir->extnol << 8)) * (0x60 * 0x60) + dir->lrc * 0x60 + dir->extnoh;
                modified = (dir->blkcnt + (dir->pointers[0] << 8)) * (0x60 * 0x60) + dir->pointers[1] * 0x60 + dir->pointers[2];

                if (drive.cnotatime && modified < created) {
                    guiintf->printMsg(wxString::Format("    Warning: Modification date earlier than creation date (extent=%d/%d)\n", extent, extent2), CpmGuiInterface::msgColGreen);
                }
            }

            if ((s = drive.dir[extent2 = (extent & ~3) + 2].status) >= 0 && s <= (drive.type == CPMFS_P2DOS ? 31 : 15)) { /* time stamps for third */
                bcdCheck(dir->pointers[7], 24, drive.cnotatime ? "creation date" : "access date", "hour", extent, extent2);
                bcdCheck(dir->pointers[8], 60, drive.cnotatime ? "creation date" : "access date", "minute", extent, extent2);
                bcdCheck(dir->pointers[11], 24, "modification date", "hour", extent, extent2);
                bcdCheck(dir->pointers[12], 60, "modification date", "minute", extent, extent2);
                created = (dir->pointers[5] + (dir->pointers[6] << 8)) * (0x60 * 0x60) + dir->pointers[7] * 0x60 + dir->pointers[8];
                modified = (dir->pointers[9] + (dir->pointers[10] << 8)) * (0x60 * 0x60) + dir->pointers[11] * 0x60 + dir->pointers[12];

                if (drive.cnotatime && modified < created) {
                    guiintf->printMsg(wxString::Format("    Warning: Modification date earlier than creation date (extent=%d/%d)\n", extent, extent2), CpmGuiInterface::msgColGreen);
                }
            }
        }
        else if (drive.type == CPMFS_DR3 && *status == 32) { /* disc label */
            unsigned long created, modified;
            bcdCheck(dir->pointers[10], 24, drive.cnotatime ? "creation date" : "access date", "hour", extent, extent);
            bcdCheck(dir->pointers[11], 60, drive.cnotatime ? "creation date" : "access date", "minute", extent, extent);
            bcdCheck(dir->pointers[14], 24, "modification date", "hour", extent, extent);
            bcdCheck(dir->pointers[15], 60, "modification date", "minute", extent, extent);
            created = (dir->pointers[8] + (dir->pointers[9] << 8)) * (0x60 * 0x60) + dir->pointers[10] * 0x60 + dir->pointers[11];
            modified = (dir->pointers[12] + (dir->pointers[13] << 8)) * (0x60 * 0x60) + dir->pointers[14] * 0x60 + dir->pointers[15];

            if (drive.cnotatime && modified < created) {
                guiintf->printMsg(wxString::Format("    Warning: Label modification date earlier than creation date (extent=%d)\n", extent), CpmGuiInterface::msgColGreen);
            }

            if (dir->extnol & 0x40 && dir->extnol & 0x10) {
                guiintf->printMsg(wxString::Format("    Error: Bit 4 and 6 can only be exclusively be set (extent=%d, label byte=0x%02x)\n", extent, (unsigned char)dir->extnol), CpmGuiInterface::msgColGreen);

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

            if (dir->extnol & 0x80 && pwdCheck(extent, dir->pointers, dir->lrc)) {
                char msg[80];
                sprintf(msg, "Set password to %c%c%c%c%c%c%c%c", T0, T1, T2, T3, T4, T5, T6, T7);

                if (repair && ask(msg)) {
                    dir->pointers[0] = P0;
                    dir->pointers[1] = P1;
                    dir->pointers[2] = P2;
                    dir->pointers[3] = P3;
                    dir->pointers[4] = P4;
                    dir->pointers[5] = P5;
                    dir->pointers[6] = P6;
                    dir->pointers[7] = P7;
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
                        guiintf->printMsg(wxString::Format("    Error: Bad name (extent=%d, name=\"%s\", position=%d)\n", extent, prfile(extent), i), CpmGuiInterface::msgColGreen);

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
                        guiintf->printMsg(wxString::Format("    Error: Bad name (extent=%d, name=\"%s\", position=%d)\n", extent, prfile(extent), i), CpmGuiInterface::msgColGreen);

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
            if (dir->extnol & (0x80 | 0x40 | 0x20) && pwdCheck(extent, dir->pointers, dir->lrc)) {
                char msg[80];
                sprintf(msg, "Set password to %c%c%c%c%c%c%c%c", T0, T1, T2, T3, T4, T5, T6, T7);

                if (repair && ask(msg)) {
                    dir->pointers[0] = P0;
                    dir->pointers[1] = P1;
                    dir->pointers[2] = P2;
                    dir->pointers[3] = P3;
                    dir->pointers[4] = P4;
                    dir->pointers[5] = P5;
                    dir->pointers[6] = P6;
                    dir->pointers[7] = P7;
                    dir->lrc = PB;
                    ret |= MODIFIED;
                }
                else {
                    ret |= BROKEN;
                }
            }
        }
        else if (*status != (char)0xe5) { /* bad status */
            guiintf->printMsg(wxString::Format("    Error: Bad status (extent=%d, name=\"%s\", status=0x%02x)\n", extent, prfile(extent), *status & 0xff), CpmGuiInterface::msgColGreen);

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
    for (extent = 0; extent < drive.maxdir; ++extent) if ((dir = drive.dir + extent)->status >= 0 && dir->status <= (drive.type == CPMFS_P2DOS ? 31 : 15)) {
            int i, j, block, block2;

            for (i = 0; i < 16; ++i) {
                block = dir->pointers[i] & 0xff;

                if (drive.size >= 256) {
                    block += (dir->pointers[++i] & 0xff) << 8;
                }

                for (extent2 = 0; extent2 < drive.maxdir; ++extent2) if ((dir2 = drive.dir + extent2)->status >= 0 && dir2->status <= (drive.type == CPMFS_P2DOS ? 31 : 15)) {
                        for (j = 0; j < 16; ++j) {
                            block2 = dir2->pointers[j] & 0xff;

                            if (drive.size >= 256) {
                                block2 += (dir2->pointers[++j] & 0xff) << 8;
                            }

                            if (block != 0 && block2 != 0 && block == block2 && !(extent == extent2 && i == j)) {
                                guiintf->printMsg(wxString::Format("    Error: Multiple allocated block (extent=%d,%d, name=\"%s\"", extent, extent2, prfile(extent)), CpmGuiInterface::msgColGreen);
                                guiintf->printMsg(wxString::Format(",\"%s\" block=%d)\n", prfile(extent2), block), CpmGuiInterface::msgColGreen);
                                ret |= BROKEN;
                            }
                        }
                    }
            }
        }

    /* check multiple extents */
    for (extent = 0; extent < drive.maxdir; ++extent) if ((dir = drive.dir + extent)->status >= 0 && dir->status <= (drive.type == CPMFS_P2DOS ? 31 : 15)) {
            for (extent2 = 0; extent2 < drive.maxdir; ++extent2) if ((dir2 = drive.dir + extent2)->status >= 0 && dir2->status <= (drive.type == CPMFS_P2DOS ? 31 : 15)) {
                    if (extent != extent2 && EXTENT(dir->extnol, dir->extnoh) == EXTENT(dir2->extnol, dir2->extnoh) && dir->status == dir2->status) {
                        int i;

                        for (i = 0; i < 8 && (dir->name[i] & 0x7f) == (dir2->name[i] & 0x7f); ++i);

                        if (i == 8) {
                            for (i = 0; i < 3 && (dir->ext[i] & 0x7f) == (dir2->ext[i] & 0x7f); ++i);

                            if (i == 3) {
                                guiintf->printMsg(wxString::Format("    Error: Duplicate extent (extent=%d,%d)\n", extent, extent2), CpmGuiInterface::msgColGreen);
                                ret |= BROKEN;
                            }
                        }
                    }
                }
        }

    if (ret == 0) { /* print statistics */
        CpmStatFS_t statfsbuf;
        int fragmented = 0, borders = 0;
        cpmStatFS(&statfsbuf);

        for (extent = 0; extent < drive.maxdir; ++extent) if ((dir = drive.dir + extent)->status >= 0 && dir->status <= (drive.type == CPMFS_P2DOS ? 31 : 15)) {
                int i, block, previous = -1;

                for (i = 0; i < 16; ++i) {
                    block = dir->pointers[i] & 0xff;

                    if (drive.size >= 256) {
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

        fragmented = (borders ? (1000 * fragmented) / borders : 0);
        guiintf->printMsg(wxString::Format("  %s: %ld/%ld files (%d.%d%% non-contigous), %ld/%ld blocks\n", image, statfsbuf.f_files - statfsbuf.f_ffree, statfsbuf.f_files, fragmented / 10, fragmented % 10, statfsbuf.f_blocks - statfsbuf.f_bfree, statfsbuf.f_blocks), CpmGuiInterface::msgColGreen);
    }

    guiintf->printMsg("====================================================================================================\n", CpmGuiInterface::msgColGreen);
    return ret;
}

// --------------------------------------------------------------------------------
