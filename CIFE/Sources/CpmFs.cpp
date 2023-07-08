/*
 * CpmFs.cpp is part of CIFE
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

#include "CpmFs.hpp"
#include "CpmDefs.h"
#include "CpmDevice.hpp"
// --------------------------------------------------------------------------------
#include <cstring>
#include <climits>
#include <cstdarg>
#include <ctime>
#include <fcntl.h>
#include <memory>
// --------------------------------------------------------------------------------
extern char **environ;
// --------------------------------------------------------------------------------
CpmFs::CpmFs(CpmDevice *device, std::string appPath) {
    cpmdevice = device;
    diskdefsPath = appPath + "diskdefs";
    fserr = "";
}

// --------------------------------------------------------------------------------
CpmFs::~CpmFs() {
}

// --------------------------------------------------------------------------------
//  -- Copy string, leaving 8th bit alone
// --------------------------------------------------------------------------------
void CpmFs::memcpy7(char *dest, char const *src, int count) {
    while (count--) {
        *dest = ((*dest) & 0x80) | ((*src) & 0x7F);
        ++dest;
        ++src;
    }
}

// --------------------------------------------------------------------------------
//  -- is character allowed in a name?
// --------------------------------------------------------------------------------
int CpmFs::isFileChar(char c, int type) {
    if (c & 0x80) {
        return 0;
    }

    if (type == CPMFS_DR3) {
        return (c > ' ' && c != '<' && c != '>' && c != '.' && c != ',' && c != ';' && c != ':'
                && c != '=' && c != '?' && c != '*' && c != '[' && c != ']' && c != '|' && !islower(c));
    }
    else {
        return (c > ' ' && c != '<' && c != '>' && c != '.' && c != ';' && c != ':'
                && c != '=' && c != '?' && c != '*' && c != '_' && !islower(c));
    }
}

// --------------------------------------------------------------------------------
//  -- split file name into name and extension
// --------------------------------------------------------------------------------
int CpmFs::splitFilename(char const *fullname, int type, char *name, char *ext,
                         int *user) {
    int i, j;
    memset(name, ' ', 8);
    memset(ext, ' ', 3);

    if (!isdigit(fullname[0]) || !isdigit(fullname[1])) {
        fserr = "illegal CP/M filename";
        return (-1);
    }

    *user = 10 * (fullname[0] - '0') + (fullname[1] - '0');
    fullname += 2;

    if ((fullname[0] == '\0') || *user >= ((type & CPMFS_HI_USER) ? 32 : 16)) {
        fserr = "illegal CP/M filename";
        return (-1);
    }

    for (i = 0; i < 8 && fullname[i] && fullname[i] != '.'; ++i) {
        if (!isFileChar(toupper(fullname[i]), type)) {
            fserr = "illegal CP/M filename";
            return (-1);
        }
        else {
            name[i] = toupper(fullname[i]);
        }
    }

    if (i == 0) {
        /* no filename after user or extension without filename */
        fserr = "illegal CP/M filename";
        return (-1);
    }

    if (fullname[i] == '.') {
        ++i;

        for (j = 0; j < 3 && fullname[i]; ++i, ++j) {
            if (!isFileChar(toupper(fullname[i]), type)) {
                fserr = "illegal CP/M filename";
                return (-1);
            }
            else {
                ext[j] = toupper(fullname[i]);
            }
        }

        if (fullname[i]) {
            /* too long name */
            fserr = "illegal CP/M filename";
            return (-1);
        }
    }

    return (0);
}

// --------------------------------------------------------------------------------
//  -- do two file names match?
// --------------------------------------------------------------------------------
int CpmFs::isMatching(int user1, char const *name1, char const *ext1, int user2,
                      char const *name2, char const *ext2) {
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
time_t CpmFs::cpm2unix_time(int days, int hour, int min) {
    /* CP/M stores timestamps in local time.  We don't know which     */
    /* timezone was used and if DST was in effect.  Assuming it was   */
    /* the current offset from UTC is most sensible, but not perfect. */
    int year, days_per_year;
    static int days_per_month[] = {31, 0, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    char **old_environ;
    static char gmt0[] = "TZ=GMT0";
    static char *gmt_env[] = { gmt0, (char *)0 };
    tm tms;
    time_t lt, t;

    time(&lt);
    t = lt;
    tms = *localtime(&lt);
    old_environ = environ;
    environ = gmt_env;
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
    environ = old_environ;
    t -= lt;
    return (t);
}

// --------------------------------------------------------------------------------
//  -- convert UTC to CP/M time
// --------------------------------------------------------------------------------
void CpmFs::unix2cpm_time(time_t now, int *days, int *hour, int *min) {
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
time_t CpmFs::ds2unix_time(const DsEntry_t *entry) {
    tm tms;
    int yr;

    if (entry->minute == 0 && entry->hour == 0 &&
            entry->day == 0 && entry->month == 0 && entry->year == 0) {
        return (0);
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
    return (mktime(&tms));
}

// --------------------------------------------------------------------------------
//  -- convert Unix to DS time
// --------------------------------------------------------------------------------
void CpmFs::unix2ds_time(time_t now, DsEntry_t *entry) {
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
void CpmFs::alvInit() {
    int i, j, offset, block;
    /* clean bitmap */
    memset(drive.alv, 0, drive.alvSize * sizeof(int));

    /* mark directory blocks as used */
    /* A directory may cover more blocks than an int may hold bits, */
    /* so a loop is needed. */
    for (block = 0; block < drive.dirblks; ++block) {
        offset = block / INTBITS;
        drive.alv[offset] |= (1 << (block % INTBITS));
    }

    for (i = 0; i < drive.maxdir; ++i) { /* mark file blocks as used */
        if (drive.dir[i].status >= 0 && drive.dir[i].status <=
                (drive.type & CPMFS_HI_USER ? 31 : 15)) {
            for (j = 0; j < 16; ++j) {
                block = (unsigned char) drive.dir[i].pointers[j];

                if (drive.size > 256) {
                    block += (((unsigned char) drive.dir[i].pointers[++j]) << 8);
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
int CpmFs::allocBlock() {
    int i, j, bits, block;

    for (i = 0; i < drive.alvSize; ++i) {
        for (j = 0, bits = drive.alv[i]; j < INTBITS; ++j) {
            if ((bits & 1) == 0) {
                block = i * INTBITS + j;

                if (block >= drive.size) {
                    fserr = "device full";
                    return (-1);
                }

                drive.alv[i] |= (1 << j);
                return (block);
            }

            bits >>= 1;
        }
    }

    fserr = "device full";
    return (-1);
}

// --------------------------------------------------------------------------------
//  -- find the logical sector number of the CP/M directory
// --------------------------------------------------------------------------------
int CpmFs::bootOffset() {
    if (drive.bootsec >= 0) {
        return (drive.bootsec);
    }

    return (drive.boottrk * drive.sectrk);
}

// --------------------------------------------------------------------------------
//  -- read a (partial) block
// --------------------------------------------------------------------------------
int CpmFs::readBlock(int blockno, char *buffer, int start, int end) {
    int sect, track, counter;

    if (blockno >= drive.size) {
        fserr = "Attempting to access block beyond end of disk";
        return (-1);
    }

    if (end < 0) {
        end = drive.blksiz / drive.secLength - 1;
    }

    sect = (blockno * (drive.blksiz / drive.secLength) + bootOffset()) % drive.sectrk;
    track = (blockno * (drive.blksiz / drive.secLength) + bootOffset()) / drive.sectrk;

    for (counter = 0; counter <= end; ++counter) {
        if (counter >= start) {
            if (!cpmdevice->ReadSector(track, drive.skewtab[sect],
                                       buffer + (drive.secLength * counter))) {
                fserr = cpmdevice->getErrorMsg();
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
int CpmFs::writeBlock(int blockno, char const *buffer, int start, int end) {
    int sect, track, counter;

    if (end < 0) {
        end = drive.blksiz / drive.secLength - 1;
    }

    sect = (blockno * (drive.blksiz / drive.secLength) + bootOffset()) % drive.sectrk;
    track = (blockno * (drive.blksiz / drive.secLength) + bootOffset()) / drive.sectrk;

    for (counter = 0; counter <= end; ++counter) {
        if (counter >= start && (!cpmdevice->WriteSector(track, drive.skewtab[sect],
                                 buffer + (drive.secLength * counter)))) {
            fserr = cpmdevice->getErrorMsg();
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
int CpmFs::findFileExtent(int user, char const *name, char const *ext, int start,
                          int extno) {
    fserr = "file already exists";

    for (; start < drive.maxdir; ++start) {
        if (((unsigned char) drive.dir[start].status) <= (drive.type & CPMFS_HI_USER ? 31 : 15) &&
                (extno == -1 || (EXTENT(drive.dir[start].extnol, drive.dir[start].extnoh) /
                                 drive.extents) == (extno / drive.extents)) &&
                isMatching(user, name, ext, drive.dir[start].status,
                           drive.dir[start].name, drive.dir[start].ext)) {
            return (start);
        }
    }

    fserr = "file not found";
    return (-1);
}

// --------------------------------------------------------------------------------
//  -- find first free extent
// --------------------------------------------------------------------------------
int CpmFs::findFreeExtent() {
    int i;

    for (i = 0; i < drive.maxdir; ++i)
        if (drive.dir[i].status == (char) 0xe5) {
            return (i);
        }

    fserr = "directory full";
    return (-1);
}

// --------------------------------------------------------------------------------
//  -- convert time stamps to CP/M format
// --------------------------------------------------------------------------------
void CpmFs::updateTimeStamps(const CpmInode_t *ino, int extent) {
    PhysDirectoryEntry_t *date;
    int ca_min, ca_hour, ca_days, u_min, u_hour, u_days;

    if (!S_ISREG(ino->mode)) {
        return;
    }

    unix2cpm_time(drive.cnotatime ? ino->ctime : ino->atime, &ca_days, &ca_hour, &ca_min);
    unix2cpm_time(ino->mtime, &u_days, &u_hour, &u_min);

    if ((drive.type & CPMFS_CPM3_DATES)
            && (date = drive.dir + (extent | 3))->status == 0x21) {
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
void CpmFs::updateDsStamps(const CpmInode_t *ino, int extent) {
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
int CpmFs::readTimeStamps(CpmInode_t *i, int lowestExt) {
    PhysDirectoryEntry_t *date;
    int u_days = 0, u_hour = 0, u_min = 0;
    int ca_days = 0, ca_hour = 0, ca_min = 0;
    int protectMode = 0;

    if ((drive.type & CPMFS_CPM3_DATES)
            && (date = drive.dir + (lowestExt | 3))->status == 0x21) {
        switch (lowestExt & 3) {
            case 0: { /* first entry of the four */
                    ca_days = ((unsigned char) date->name[0]) + (((unsigned char) date->name[1]) << 8);
                    ca_hour = (unsigned char) date->name[2];
                    ca_min = (unsigned char) date->name[3];
                    u_days = ((unsigned char) date->name[4]) + (((unsigned char) date->name[5]) << 8);
                    u_hour = (unsigned char) date->name[6];
                    u_min = (unsigned char) date->name[7];
                    protectMode = (unsigned char) date->ext[0];
                    break;
                }

            case 1: { /* second entry */
                    ca_days = ((unsigned char) date->ext[2]) +
                              (((unsigned char) date->extnol) << 8);

                    ca_hour = (unsigned char) date->lrc;
                    ca_min = (unsigned char) date->extnoh;
                    u_days = ((unsigned char) date->blkcnt) +
                             (((unsigned char) date->pointers[0]) << 8);

                    u_hour = (unsigned char) date->pointers[1];
                    u_min = (unsigned char) date->pointers[2];
                    protectMode = (unsigned char) date->pointers[3];
                    break;
                }

            case 2: { /* third one */
                    ca_days = ((unsigned char) date->pointers[5]) +
                              (((unsigned char) date->pointers[6]) << 8);

                    ca_hour = (unsigned char) date->pointers[7];
                    ca_min = (unsigned char) date->pointers[8];
                    u_days = ((unsigned char) date->pointers[9]) +
                             (((unsigned char) date->pointers[10]) << 8);

                    u_hour = (unsigned char) date->pointers[11];
                    u_min = (unsigned char) date->pointers[12];
                    protectMode = (unsigned char) date->pointers[13];
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
void CpmFs::readDsStamps(CpmInode_t *i, int lowestExt) {
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
int CpmFs::recmatch(char const *a, char const *pattern) {
    int first = 1;

    while (*pattern) {
        switch (*pattern) {
            case '*': {
                    if (*a == '.' && first) {
                        return (1);
                    }

                    ++pattern;

                    while (*a)
                        if (recmatch(a, pattern)) {
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
int CpmFs::match(char const *a, char const *pattern) {
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

    if (strcmp(a, ".") == 0 || strcmp(a, "..") == 0) {
        return (0);
    }

    return recmatch(a, pat);
}

// --------------------------------------------------------------------------------
//  -- expand CP/M style wildcards
// --------------------------------------------------------------------------------
void CpmFs::glob(const char *argv, int *gargc, char ***gargv) {
    CpmFile_t dir;
    int entries, dirsize = 0;
    CpmDirent_t *dirent = (CpmDirent_t *) 0;
    int gargcap = 0, j;
    *gargv = (char **) 0;
    *gargc = 0;
    cpmOpendir(&dir);
    entries = 0;
    dirsize = 8;
    dirent = (CpmDirent_t *) malloc(sizeof(CpmDirent_t) * dirsize);

    while (cpmReaddir(&dir, &dirent[entries])) {
        ++entries;

        if (entries == dirsize) {
            dirent = (CpmDirent_t *) realloc(dirent, sizeof(CpmDirent_t) * (dirsize *= 2));
        }
    }

    int found;

    for (j = 0, found = 0; j < entries; ++j) {
        if (match(dirent[j].name, argv)) {
            if (*gargc == gargcap) {
                *gargv = (char **) realloc(*gargv, sizeof(char *) * (
                                               gargcap ? (gargcap *= 2) : (gargcap = 16)));
            }

            (*gargv)[*gargc] = strcpy((char *) malloc(strlen(dirent[j].name) + 1), dirent[j].name);
            ++*gargc;
            ++found;
        }
    }

    free(dirent);
}

// --------------------------------------------------------------------------------
//  -- free expanded wildcards
// --------------------------------------------------------------------------------
void CpmFs::globfree(char **dirent, int entries) {
    for (int d = 0; d < entries; ++d) {
        free(dirent[d]);
    }

    free(dirent);
}

// --------------------------------------------------------------------------------
//  -- read super block from diskdefs file
// --------------------------------------------------------------------------------
int CpmFs::diskdefReadSuper(char const *format) {
    char line[256];
    int ln;
    FILE *fp;
    int insideDef = 0, found = 0;
    drive.type = 0;

    if ((fp = fopen(diskdefsPath.c_str(), "r")) == (FILE *) 0) {
        fserr = "'diskdefs' couldn't be opened";
        return (1);
    }

    ln = 1;

    while (fgets(line, sizeof(line), fp) != (char *) 0) {
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

        for (argc = 0; argc < 1
                && (argv[argc] = strtok(argc ? (char *) 0 : line, " \t\n")); ++argc);

        if ((argv[argc] = strtok((char *) 0, "\n")) != (char *) 0) {
            ++argc;
        }

        if (insideDef) {
            if (argc == 1 && strcmp(argv[0], "end") == 0) {
                insideDef = 0;
                drive.size = (drive.sectrk * drive.tracks - bootOffset()) * drive.secLength /
                             drive.blksiz;

                if (drive.extents == 0) {
                    drive.extents = ((drive.size > 256 ? 8 : 16) * drive.blksiz) / drive.extentsize;
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
                    drive.secLength = strtol(argv[1], (char **) 0, 0);
                }
                else if (strcmp(argv[0], "tracks") == 0) {
                    drive.tracks = strtol(argv[1], (char **) 0, 0);
                }
                else if (strcmp(argv[0], "sectrk") == 0) {
                    drive.sectrk = strtol(argv[1], (char **) 0, 0);
                }
                else if (strcmp(argv[0], "blocksize") == 0) {
                    drive.blksiz = strtol(argv[1], (char **) 0, 0);

                    if (drive.blksiz <= 0) {
                        fserr = msgFormat("invalid blocksize '%s' in line %d", argv[1], ln);
                        return (1);
                    }
                }
                else if (strcmp(argv[0], "maxdir") == 0) {
                    drive.maxdir = strtol(argv[1], (char **) 0, 0);
                }
                else if (strcmp(argv[0], "dirblks") == 0) {
                    drive.dirblks = strtol(argv[1], (char **) 0, 0);
                }
                else if (strcmp(argv[0], "skew") == 0) {
                    drive.skew = strtol(argv[1], (char **) 0, 0);
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
                                fserr = msgFormat("invalid skewtab `%s' at `%s' in line %d",
                                                  argv[1], s, ln);
                                return (1);
                            }

                            s = end;
                            ++sectors;

                            if (*s == ',') {
                                ++s;
                            }
                        }

                        if (pass == 0) {
                            drive.skewtab = (int *) malloc(sizeof(int) * sectors);
                        }
                    }
                }
                else if (strcmp(argv[0], "boottrk") == 0) {
                    drive.boottrk = strtol(argv[1], (char **) 0, 0);
                }
                else if (strcmp(argv[0], "bootsec") == 0) {
                    drive.bootsec = strtol(argv[1], (char **) 0, 0);
                }
                else if (strcmp(argv[0], "offset") == 0) {
                    off_t val;
                    unsigned int multiplier;
                    char *endptr;
                    errno = 0;
                    multiplier = 1;
                    val = strtol(argv[1], &endptr, 10);

                    if ((errno == ERANGE && val == LONG_MAX) || (errno != 0 && val <= 0)) {
                        fserr = msgFormat("invalid offset value `%s' (%s) in line %d",
                                          argv[1], strerror(errno), ln);
                        return (1);
                    }

                    if (endptr == argv[1]) {
                        fserr = msgFormat("offset value `%s' is not a number in line %d",
                                          argv[1], ln);
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
                                    fserr = msgFormat("offset must be specified after sectrk, "
                                                      "tracks and secLength in line %d", ln);
                                    return (1);
                                }

                                multiplier = drive.sectrk * drive.secLength;
                                break;

                            case 'S':
                                if (drive.sectrk < 0 || drive.tracks < 0 || drive.secLength < 0) {
                                    fserr = msgFormat("offset must be specified after sectrk, "
                                                      "tracks and secLength in line %d", ln);
                                    return (1);
                                }

                                multiplier = drive.secLength;
                                break;

                            default:
                                fserr = msgFormat("unknown unit specifier `%c' in line %d",
                                                  *endptr, ln);
                                return (1);
                        }
                    }

                    if (val * multiplier > INT_MAX) {
                        fserr = msgFormat("effective offset is out of range in line %d", ln);
                        return (1);
                    }

                    drive.offset = val * multiplier;
                }
                else if (strcmp(argv[0], "logicalextents") == 0) {
                    drive.extents = strtol(argv[1], (char **) 0, 0);
                }
                else if (strcmp(argv[0], "extentsize") == 0) {
                    drive.extentsize = strtol(argv[1], (char **)0, 0);

                    if (drive.extentsize > 16384) {
                        fserr = msgFormat("extentsize > 16384 in line %d\n", ln);
                        return (1);
                    }
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
                        fserr = msgFormat("invalid OS type `%s' in line %d", argv[1], ln);
                        return (1);
                    }
                }
            }
            else if (argc > 0 && argv[0][0] != '#' && argv[0][0] != ';') {
                fserr = msgFormat("invalid keyword `%s' in line %d", argv[0], ln);
                return (1);
            }
        }
        else if (argc == 2 && strcmp(argv[0], "diskdef") == 0) {
            insideDef = 1;
            drive.skew = 1;
            drive.extents = 0;
            drive.extentsize = 16384;
            drive.type = CPMFS_DR22;
            drive.skewtab = (int *) 0;
            drive.offset = 0;
            drive.blksiz = drive.boottrk = drive.bootsec = drive.secLength = drive.sectrk =
                                               drive.tracks = drive.maxdir = -1;
            drive.dirblks = 0;

            if (strcmp(argv[1], format) == 0) {
                found = 1;
            }
        }

        ++ln;
    }

    fclose(fp);

    if (!found) {
        fserr = msgFormat("unknown format %s", format);
        return (1);
    }

    if (drive.boottrk < 0 && drive.bootsec < 0) {
        fserr = "boottrk/bootsec parameter invalid or missing from diskdef";
        return (1);
    }

    if (drive.secLength < 0) {
        fserr = "secLength parameter invalid or missing from diskdef";
        return (1);
    }

    if (drive.sectrk < 0) {
        fserr = "sectrk parameter invalid or missing from diskdef";
        return (1);
    }

    if (drive.tracks < 0) {
        fserr = "tracks parameter invalid or missing from diskdef";
        return (1);
    }

    if (drive.blksiz < 0) {
        fserr = "blocksize parameter invalid or missing from diskdef";
        return (1);
    }

    if (drive.maxdir < 0) {
        fserr = "maxdir parameter invalid or missing from diskdef";
        return (1);
    }

    if ((drive.size <= 256 ? 16 : 8)*drive.blksiz < drive.extentsize) {
        fserr = msgFormat("extent size less than %d\n", drive.extentsize);
        return (1);
    }

    return (0);
}

// --------------------------------------------------------------------------------
//  -- read super block from amstrad disk
// --------------------------------------------------------------------------------
int CpmFs::amsReadSuper(char const *format) {
    unsigned char boot_sector[512], *boot_spec;
    cpmdevice->SetGeometry(512, 9, 40, 0);

    if (!cpmdevice->ReadSector(0, 0, (char *) boot_sector)) {
        fserr = msgFormat("Failed to read Amstrad superblock  (%s)",
                          cpmdevice->getErrorMsg().c_str());
        return (1);
    }

    boot_spec = (boot_sector[0] == 0
                 || boot_sector[0] == 3) ? boot_sector : (unsigned char *) 0;

    /* Check for JCE's extension to allow Amstrad and MSDOS superblocks
     * in the same sector (for the PCW16)
     */
    if ((boot_sector[0] == 0xE9 || boot_sector[0] == 0xEB) &&
            !memcmp(boot_sector + 0x2B, "CP/M", 4) && !memcmp(boot_sector + 0x33, "DSK", 3) &&
            !memcmp(boot_sector + 0x7C, "CP/M", 4)) {
        boot_spec = boot_sector + 128;
    }

    if (boot_spec == (unsigned char *) 0) {
        fserr = "Amstrad superblock not present";
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
    drive.type |= CPMFS_DR3; /* Amstrads are CP/M 3 systems */
    drive.secLength = 128 << boot_spec[4];
    drive.tracks = boot_spec[2];

    if (boot_spec[1] & 3) {
        drive.tracks *= 2;
    }

    drive.sectrk = boot_spec[3];
    drive.blksiz = 128 << boot_spec[6];
    drive.maxdir = (drive.blksiz / 32) * boot_spec[7];
    drive.dirblks = 0;
    drive.skew = 1; /* Amstrads skew at the controller level */
    drive.skewtab = (int *) 0;
    drive.boottrk = boot_spec[5];
    drive.bootsec = -1;
    drive.offset = 0;
    drive.size = (drive.secLength * drive.sectrk * (drive.tracks - drive.boottrk)) /
                 drive.blksiz;
    drive.extents = ((drive.size > 256 ? 8 : 16) * drive.blksiz) / 16384;
    drive.extentsize = 16384;
    return (0);
}

// --------------------------------------------------------------------------------
//  -- read all datestamper timestamps
// --------------------------------------------------------------------------------
int CpmFs::cpmCheckDs() {
    int dsoffset, dsblks, dsrecs, off, i;
    unsigned char *buf;

    if (!isMatching(0, "!!!TIME&", "DAT", drive.dir->status, drive.dir->name,
                    drive.dir->ext)) {
        return (-1);
    }

    /* Offset to ds file in alloc blocks */
    dsoffset = (drive.maxdir * 32 + (drive.blksiz - 1)) / drive.blksiz;
    dsrecs = (drive.maxdir + 7) / 8;
    dsblks = (dsrecs * 128 + (drive.blksiz - 1)) / drive.blksiz;
    /* Allocate buffer */
    drive.ds = (DsDate_t *) malloc(dsblks * drive.blksiz);
    /* Read ds file in its entirety */
    off = 0;

    for (i = dsoffset; i < dsoffset + dsblks; i++) {
        if (readBlock(i, ((char *) drive.ds) + off, 0, -1) == -1) {
            return (-1);
        }

        off += drive.blksiz;
    }

    /* Verify checksums */
    buf = (unsigned char *) drive.ds;

    for (i = 0; i < dsrecs; i++) {
        unsigned cksum, j;
        cksum = 0;

        for (j = 0; j < 127; j++) {
            cksum += buf[j];
        }

        if (buf[j] != (cksum & 0xff)) {
            free(drive.ds);
            drive.ds = (DsDate_t *) 0;
            return (-1);
        }

        buf += 128;
    }

    return (0);
}

// --------------------------------------------------------------------------------
//  -- get DPB
// --------------------------------------------------------------------------------
int CpmFs::readDiskdefData(const char *format) {
    while (s_ifdir && !S_ISDIR(s_ifdir)) {
        s_ifdir <<= 1;
    }

    while (s_ifreg && !S_ISREG(s_ifreg)) {
        s_ifreg <<= 1;
    }

    if (strcmp(format, "Amstrad") == 0) {
        return (amsReadSuper(format));
    }

    return (diskdefReadSuper(format));
}

// --------------------------------------------------------------------------------
//  -- init in-core data for drive
// --------------------------------------------------------------------------------
int CpmFs::initDriveData(int uppercase) {

    drive.uppercase = uppercase;

    if (drive.dirblks == 0) {
        /* optional field, compute based on directory size */
        drive.dirblks = (drive.maxdir * 32 + (drive.blksiz - 1)) / drive.blksiz;
    }

    cpmdevice->SetGeometry(drive.secLength, drive.sectrk, drive.tracks, drive.offset);

    if (drive.skewtab == (int *) 0) { /* generate skew table */
        int i, j, k;

        if ((drive.skewtab = (int *) malloc(drive.sectrk * sizeof(int))) == (int *) 0) {
            fserr = strerror(errno);
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
        drive.alvSize = ((((drive.sectrk * drive.tracks) - bootOffset()) * drive.secLength) /
                         drive.blksiz + INTBITS - 1) / INTBITS;

        if ((drive.alv = (int *) malloc(drive.alvSize * sizeof(int))) == (int *) 0) {
            fserr = strerror(errno);
            return (-1);
        }
    }

    /* allocate directory buffer */
    if ((drive.dir = (PhysDirectoryEntry_t *)
                     malloc(((drive.maxdir * 32 + drive.blksiz - 1) / drive.blksiz) *
                            drive.blksiz)) == (PhysDirectoryEntry_t *) 0) {
        fserr = strerror(errno);
        return (-1);
    }

    if (cpmdevice->isOpen() == false) { /* create empty directory in core */
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

            for (i = 0; i < drive.maxdir; ++i)
                if (drive.dir[i].status >= 16 && drive.dir[i].status <= 31) {
                    ++passwords;
                }

            if ((drive.passwdLength = passwords * PASSWD_RECLEN)) {
                if ((drive.passwd = (char *) malloc(drive.passwdLength)) == (char *) 0) {
                    fserr = "out of memory";
                    return (-1);
                }

                for (i = 0, passwords = 0; i < drive.maxdir; ++i)
                    if (drive.dir[i].status >= 16 && drive.dir[i].status <= 31) {
                        int j, pb;
                        char *p = drive.passwd + (passwords++ * PASSWD_RECLEN);
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
                        pb = (unsigned char) drive.dir[i].lrc;

                        for (j = 0; j < 8; ++j) {
                            p[15 + j] = ((unsigned char) drive.dir[i].pointers[7 - j]) ^ pb;
                        }

                        p[23] = '\n';
                    }
            }
        }

        /* disc label */
        for (i = 0; i < drive.maxdir; ++i)
            if (drive.dir[i].status == (char) 0x20) {
                int j;
                drive.cnotatime = drive.dir[i].extnol & 0x10;

                if (drive.dir[i].extnol & 0x1) {
                    drive.labelLength = 12;

                    if ((drive.label = (char *) malloc(drive.labelLength)) == (char *) 0) {
                        fserr = "out of memory";
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
        drive.ds = (DsDate_t *) 0;
    }

    return (0);
}

// --------------------------------------------------------------------------------
//  -- write all datestamper timestamps
// --------------------------------------------------------------------------------
int CpmFs::syncDs() {
    if (drive.dirtyDs) {
        int dsoffset, dsblks, dsrecs, off, i;
        unsigned char *buf;
        dsrecs = (drive.maxdir + 7) / 8;
        /* Re-calculate checksums */
        buf = (unsigned char *) drive.ds;

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
int CpmFs::sync() {
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
int CpmFs::unmount() {

    int errSync = sync();

    if (drive.type & CPMFS_DS_DATES) {
        free(drive.ds);
        drive.ds = nullptr;
    }

    free(drive.alv);
    drive.alv = nullptr;
    free(drive.skewtab);
    drive.skewtab = nullptr;
    free(drive.dir);
    drive.dir = nullptr;

    if (drive.passwdLength) {
        free(drive.passwd);
        drive.passwd = nullptr;
    }

    if (drive.labelLength) {
        free(drive.label);
        drive.label = nullptr;
    }

    if (errSync == -1) {
        return (errSync);
    }

    return (0);
}

// --------------------------------------------------------------------------------
//  -- map name to inode
// --------------------------------------------------------------------------------
int CpmFs::namei(char const *filename, CpmInode_t *i) {
    int user;
    char name[8], extension[3];
    int highestExtno, highestExt = -1, lowestExtno, lowestExt = -1;
    int protectMode = 0;

    if (!S_ISDIR(root.mode)) {
        fserr = "No such file";
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
        i->size = highestExtno * drive.extentsize;

        if (drive.size <= 256) for (block = 15; block >= 0; --block) {
                if (drive.dir[highestExt].pointers[block]) {
                    break;
                }
            }
        else for (block = 7; block >= 0; --block) {
                if (drive.dir[highestExt].pointers[2 * block] ||
                        drive.dir[highestExt].pointers[2 * block + 1]) {
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

    if (drive.dir[lowestExt].ext[0] & 0x80) {
        i->attr |= CPM_ATTR_RO;
    }

    if (drive.dir[lowestExt].ext[1] & 0x80) {
        i->attr |= CPM_ATTR_SYS;
    }

    if (drive.dir[lowestExt].ext[2] & 0x80) {
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
void CpmFs::statFs(CpmStatFS_t *buf) {
    int i;
    buf->f_bsize = drive.blksiz;
    buf->f_blocks = drive.size;
    buf->f_bfree = 0;
    buf->f_bused = -(drive.dirblks);

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
        if (drive.dir[i].status == (char) 0xe5) {
            ++buf->f_ffree;
        }
    }

    buf->f_namelen = 11;
}

// --------------------------------------------------------------------------------
//  -- unlink
// --------------------------------------------------------------------------------
int CpmFs::unlink(char const *fname) {
    int user;
    char name[8], extension[3];
    int extent;

    if (!S_ISDIR(root.mode)) {
        fserr = "No such file";
        return (-1);
    }

    if (splitFilename(fname, drive.type, name, extension, &user) == -1) {
        return (-1);
    }

    if ((extent = findFileExtent(user, name, extension, 0, -1)) == -1) {
        return (-1);
    }

    drive.dirtyDirectory = 1;
    drive.dir[extent].status = (char) 0xe5;

    do {
        drive.dir[extent].status = (char) 0xe5;
    } while ((extent = findFileExtent(user, name, extension, extent + 1, -1)) >= 0);

    alvInit();
    return (0);
}

// --------------------------------------------------------------------------------
//  -- rename
// --------------------------------------------------------------------------------
int CpmFs::rename(char const *oldn, char const *newn) {
    int extent;
    int olduser;
    char oldname[8], oldext[3];
    int newuser;
    char newname[8], newext[3];

    if (!S_ISDIR(root.mode)) {
        fserr = "No such file";
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
        fserr = "file already exists";
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
int CpmFs::cpmOpendir(CpmFile_t *dirp) {
    if (!S_ISDIR(root.mode)) {
        fserr = "No such file";
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
int CpmFs::cpmReaddir(CpmFile_t *dir, CpmDirent_t *ent) {
    PhysDirectoryEntry_t *cur = (PhysDirectoryEntry_t *) 0;
    char buf[2 + 8 + 1 + 3 + 1]; /* 00foobarxy.zzy\0 */
    char *bufp;
    int hasext;

    if (!(S_ISDIR(dir->ino->mode))) { /* error: not a directory */
        fserr = "not a directory";
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
        else if (dir->pos >= RESERVED_ENTRIES
                 && dir->pos < (int) drive.maxdir + RESERVED_ENTRIES) {
            int first = dir->pos - RESERVED_ENTRIES;

            if ((cur = drive.dir + (dir->pos - RESERVED_ENTRIES))->status >= 0 &&
                    cur->status <= (drive.type & CPMFS_HI_USER ? 31 : 15)) {
                /* determine first extent for the current file */
                for (i = 0; i < drive.maxdir; ++i)
                    if (i != (dir->pos - RESERVED_ENTRIES)) {
                        if (isMatching(cur->status, cur->name, cur->ext, drive.dir[i].status,
                                       drive.dir[i].name, drive.dir[i].ext) &&
                                EXTENT(cur->extnol, cur->extnoh) > EXTENT(drive.dir[i].extnol, drive.dir[i].extnoh)) {
                            first = i;
                        }
                    }

                if (first == (dir->pos - RESERVED_ENTRIES)) {
                    ent->ino = dir->pos - RESERVED_INODES;
                    /* convert file name to UNIX style */
                    buf[0] = '0' + cur->status / 10;
                    buf[1] = '0' + cur->status % 10;

                    for (bufp = buf + 2, i = 0; i < 8 && (cur->name[i] & 0x7f) != ' '; ++i) {
                        *bufp++ = (drive.uppercase ? (cur->name[i] & 0x7f) : (tolower(cur->name[i] & 0x7f)));
                    }

                    for (hasext = 0, i = 0; i < 3 && (cur->ext[i] & 0x7f) != ' '; ++i) {
                        if (!hasext) {
                            *bufp++ = '.';
                            hasext = 1;
                        }

                        *bufp++ = (drive.uppercase ? (cur->ext[i] & 0x7f) : (tolower(cur->ext[i] & 0x7f)));
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
void CpmFs::stat(const CpmInode_t *ino, CpmStat_t *buf) {
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
int CpmFs::open(CpmInode_t *ino, CpmFile_t *file, mode_t mode) {
    if (S_ISREG(ino->mode)) {
        if ((mode & O_WRONLY) && (ino->mode & 0222) == 0) {
            fserr = "permission denied";
            return (-1);
        }

        file->pos = 0;
        file->ino = ino;
        file->mode = mode;
        return (0);
    }
    else {
        fserr = "not a regular file";
        return (-1);
    }
}

// --------------------------------------------------------------------------------
//  -- read a file from CP/M filesystem
// --------------------------------------------------------------------------------
ssize_t CpmFs::read(CpmFile_t *file, char *buf, size_t count) {
    int findext = 1, findblock = 1, extent = -1, block = -1,
        extentno = -1, got = 0, nextblockpos = -1, nextextpos = -1;
    int blocksize = drive.blksiz;
    int extcap;
    extcap = (drive.size <= 256 ? 16 : 8) * blocksize;

    if (extcap > 16384) {		// drive.extentsize ???
        extcap = 16384 * drive.extents;	// drive.extentsize ???
    }

    if (file->ino->ino == (ino_t) drive.maxdir + 1) { /* [passwd] */
        if ((file->pos + (off_t)count) > file->ino->size) {
            count = file->ino->size - file->pos;
        }

        if (count) {
            memcpy(buf, drive.passwd + file->pos, count);
        }

        file->pos += count;
        return (count);
    }
    else if (file->ino->ino == (ino_t) drive.maxdir + 2) { /* [label] */
        if ((file->pos + (off_t)count) > file->ino->size) {
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
                extentno = file->pos / drive.extentsize;
                extent = findFileExtent(drive.dir[file->ino->ino].status,
                                        drive.dir[file->ino->ino].name, drive.dir[file->ino->ino].ext,
                                        0, extentno);
                nextextpos = (file->pos / extcap) * extcap + extcap;
                findext = 0;
                findblock = 1;
            }

            if (findblock) {
                if (extent != -1) {
                    int ptr;
                    ptr = (file->pos % extcap) / blocksize;

                    if (drive.size > 256) {
                        ptr *= 2;
                    }

                    block = (unsigned char) drive.dir[extent].pointers[ptr];

                    if (drive.size > 256) {
                        block += ((unsigned char) drive.dir[extent].pointers[ptr + 1]) << 8;
                    }

                    if (block == 0) {
                        memset(buffer, 0, blocksize);
                    }
                    else {
                        int start, end;
                        start = (file->pos % blocksize) / drive.secLength;
                        end = ((file->pos % blocksize + (off_t)count) > blocksize ? blocksize - 1 : (int)(
                                   file->pos % blocksize + count - 1)) / drive.secLength;

                        if (block < drive.dirblks) {
                            fserr = "Attempting to access block before beginning of data";

                            if (got == 0) {
                                got = -1;
                            }

                            break;
                        }

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
//  -- write a file to CP/M filesystem
// --------------------------------------------------------------------------------
ssize_t CpmFs::write(CpmFile_t *file, char const *buf, size_t count) {
    int findext = 1, findblock = -1, extent = -1, extentno = -1, got = 0, nextblockpos = -1,
        nextextpos = -1;
    int blocksize = drive.blksiz;
    int block = -1, start = -1, end = -1, ptr = -1, last = -1;
    char buffer[16384];
    int extcap = (drive.size <= 256 ? 16 : 8) * blocksize;

    if (extcap > 16384) {			// drive.extentsize ???
        extcap = (16384 * drive.extents);	// drive.extentsize ???
    }

    while (count > 0) {
        if (findext) {
            extentno = file->pos / drive.extentsize;
            extent = findFileExtent(drive.dir[file->ino->ino].status,
                                    drive.dir[file->ino->ino].name, drive.dir[file->ino->ino].ext,
                                    0, extentno);
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

            if (drive.size > 256) {
                ptr *= 2;
            }

            block = (unsigned char) drive.dir[extent].pointers[ptr];

            if (drive.size > 256) {
                block += ((unsigned char) drive.dir[extent].pointers[ptr + 1]) << 8;
            }

            if (block == 0) { /* allocate new block, set start/end to cover it */
                if ((block = allocBlock()) == -1) {
                    return (got == 0 ? -1 : got);
                }

                drive.dir[extent].pointers[ptr] = block & 0xff;

                if (drive.size > 256) {
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
            else { /* read existing block and set start/end to cover modified parts */
                start = (file->pos % blocksize) / drive.secLength;
                end = ((int)(file->pos % blocksize + count) >= blocksize ? blocksize - 1 : (int)(
                           file->pos % blocksize + count - 1)) / drive.secLength;

                if (file->pos % drive.secLength) {
                    if (readBlock(block, buffer, start, start) == -1) {
                        if (got == 0) {
                            got = -1;
                        }

                        break;
                    }
                }

                if (end != start && (int)(file->pos % blocksize + count) < blocksize
                        && (file->pos + count) % drive.secLength) {

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

        if (drive.size <= 256) for (last = 15; last >= 0; --last) {
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
        drive.dir[extent].blkcnt = ((file->pos - 1) % drive.extentsize) / 128 + 1;

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
int CpmFs::close(CpmFile_t *file) {
    return (0);
}

// --------------------------------------------------------------------------------
//  -- creat new CP/M file
// --------------------------------------------------------------------------------
int CpmFs::create(CpmInode_t *dir, char const *fname, CpmInode_t *ino, mode_t mode) {
    int user;
    char name[8], extension[3];
    int extent;
    PhysDirectoryEntry_t *ent;

    if (!S_ISDIR(dir->mode)) {
        fserr = "No such file or directory";
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
int CpmFs::attrGet(CpmInode_t *ino, cpm_attr_t *attrib) {
    *attrib = ino->attr;
    return (0);
}

// --------------------------------------------------------------------------------
//  -- set CP/M attributes
// --------------------------------------------------------------------------------
int CpmFs::attrSet(CpmInode_t *ino, cpm_attr_t attrib) {
    int extent;
    int user;
    char name[8], extension[3];
    memset(name, 0, sizeof(name));
    memset(extension, 0, sizeof(extension));
    extent = ino->ino;
    drive.dirtyDirectory = 1;
    /* Strip off existing attribute bits */
    memcpy7(name, drive.dir[extent].name, 8);
    memcpy7(extension, drive.dir[extent].ext, 3);
    user = drive.dir[extent].status;

    /* And set new ones */
    if (attrib & CPM_ATTR_F1) {
        name[0] |= 0x80;
    }

    if (attrib & CPM_ATTR_F2) {
        name[1] |= 0x80;
    }

    if (attrib & CPM_ATTR_F3) {
        name[2] |= 0x80;
    }

    if (attrib & CPM_ATTR_F4) {
        name[3] |= 0x80;
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
int CpmFs::protSet(CpmInode_t *ino, mode_t pmode) {
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

    if ((drive.type & CPMFS_CPM3_DATES)
            && (date = drive.dir + (lowestExt | 3))->status == 0x21) {
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
void CpmFs::utime(CpmInode_t *ino, utimbuf *times) {
    ino->atime = times->actime;
    ino->mtime = times->modtime;
    time(&ino->ctime);
    updateTimeStamps(ino, ino->ino);
    updateDsStamps(ino, ino->ino);
}

// --------------------------------------------------------------------------------
//  -- create new empty binary Image-File
// --------------------------------------------------------------------------------
int CpmFs::mkfs(char const *filename, char const *format, char const *label,
                char *bootTracks, int timeStamps, int uppercase) {
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
    if ((fd = fopen(filename, "w+b")) == (FILE *) 0) {
        fserr = strerror(errno);
        return -1;
    }

    /* write system tracks */
    /* this initialises only whole tracks, so it skew is not an issue */
    trkbytes = drive.secLength * drive.sectrk;

    for (i = 0; i < trkbytes * drive.boottrk; i += drive.secLength) {
        if (fwrite(bootTracks + i, sizeof(char), drive.secLength,
                   fd) != (size_t) drive.secLength) {
            fserr = strerror(errno);
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

        firstbuf[12] = timeStamps ? 0x11 :
                       0x01; /* label set and first time stamp is creation date */
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
            fserr = strerror(errno);
            fclose(fd);
            return -1;
        }
    }

    /* fill remaining size */
    memset(buf, 0xe5, 128);
    int imagesize = (drive.secLength * drive.sectrk * drive.tracks);

    for (i = 0; i < (imagesize - (bytes + (trkbytes * drive.boottrk))); i += 128) {
        if (fwrite(buf, sizeof(char), 128, fd) != 128) {
            fserr = strerror(errno);
            fclose(fd);
            return -1;
        }
    }

    /* close image file */
    if (fclose(fd) == -1) {
        fserr = strerror(errno);
        return -1;
    }

    if (timeStamps && !(drive.type == CPMFS_P2DOS || drive.type == CPMFS_DR3)) {
        int offset, j;
        CpmInode_t ino;
        static const char sig[] = "!!!TIME";
        unsigned int records;
        DsDate_t *ds;

        if (!cpmdevice->Open(filename, "r+b")) {
            fserr = msgFormat("Cannot open %s  (%s)", filename, cpmdevice->getErrorMsg().c_str());
            return (-1);
        }

        initDriveData(uppercase);
        records = (drive.maxdir + 7) / 8;

        if (!(ds = (DsDate_t *) malloc(records * 128))) {
            sync();
            return -1;
        }

        memset(ds, 0, records * 128);
        offset = 15;

        for (i = 0; i < records; i++) {
            unsigned int cksum;

            for (j = 0; j < 7; j++, offset += 16) {
                *((char *)ds + offset) = sig[j];
            }

            /* skip checksum byte */
            offset += 16;

            for (cksum = 0, j = 0; j < 127; ++j) {
                cksum += *((unsigned char *)ds + i * 128 + j);
            }

            *((char *)ds + i * 128 + j) = cksum;
        }

        /* The filesystem does not know about datestamper yet, because it
             was not there when it was mounted. */
        if (create(&root, "00!!!TIME&.DAT", &ino, 0222) == -1) {
            fserr = msgFormat("Unable to create DateStamper file.  (%s)", fserr.c_str());
            return -1;
        }

        CpmFile_t dsfile;

        if ((open(&ino, &dsfile, O_WRONLY) == -1)
                || (write(&dsfile, (char *)ds, records * 128) != (records * 128))
                || (close(&dsfile) == -1)) {
            fserr = msgFormat("Unable to write DateStamper file.  (%s)", fserr.c_str());
            return -1;
        }

        protSet(&ino, 0);

        utimbuf ut;
        namei("00!!!TIME&.DAT", &ino);
        time(&ut.actime);
        time(&ut.modtime);
        utime(&ino, &ut);

        drive.ds = ds;
        drive.dirtyDs = 1;
        sync();
    }

    return 0;
}

// --------------------------------------------------------------------------------
std::string CpmFs::getErrorMsg() {
    return (fserr);
}

// --------------------------------------------------------------------------------
std::string CpmFs::msgFormat(const std::string fmt_str, ...) {
    int final_n, n = ((int) fmt_str.size()) * 2;
    std::unique_ptr<char[]> formatted;
    va_list ap;

    while (1) {
        formatted.reset(new char[n]);
        strcpy(&formatted[0], fmt_str.c_str());
        va_start(ap, fmt_str);
        final_n = vsnprintf(&formatted[0], n, fmt_str.c_str(), ap);
        va_end(ap);

        if (final_n < 0 || final_n >= n) {
            n += abs(final_n - n + 1);
        }
        else {
            break;
        }
    }

    return std::string(formatted.get());
}

// --------------------------------------------------------------------------------
CpmFs::CpmSuperBlock_t &CpmFs::getDriveData() {
    return (drive);
}

// --------------------------------------------------------------------------------
CpmFs::CpmInode_t &CpmFs::getDirectoryRoot() {
    return (root);
}

// --------------------------------------------------------------------------------
bool CpmFs::getBootTracksEnabled() {
    return ((drive.boottrk > 0) ? true : false);
}

// --------------------------------------------------------------------------------
size_t CpmFs::getBootTrackSize() {
    return (drive.boottrk * drive.secLength * drive.sectrk);
}
// --------------------------------------------------------------------------------


