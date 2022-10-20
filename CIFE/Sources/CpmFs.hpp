/*
 * CpmFs.hpp is part of CIFE
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

#ifndef CPMFS_HPP_
#define CPMFS_HPP_
// --------------------------------------------------------------------------------
#include "CpmDevice.hpp"
// --------------------------------------------------------------------------------
#include <string>
#include <utime.h>
// --------------------------------------------------------------------------------
class CpmFs {
    public:    // Attributes

        typedef int cpm_attr_t;

        typedef struct {
            ino_t ino;
            mode_t mode;
            off_t size;
            cpm_attr_t attr;
            time_t atime;
            time_t mtime;
            time_t ctime;
        } CpmInode_t;

        typedef struct {
            char status;
            char name[8];
            char ext[3];
            char extnol;
            char lrc;
            char extnoh;
            char blkcnt;
            char pointers[16];
        } PhysDirectoryEntry_t;

        typedef struct {
            char year;
            char month;
            char day;
            char hour;
            char minute;
        } DsEntry_t;

        typedef struct {
            DsEntry_t create;
            DsEntry_t access;
            DsEntry_t modify;
            char checksum;
        } DsDate_t;

        typedef struct {
            int secLength;
            int tracks;
            int sectrk;
            int blksiz;
            int maxdir;
            int skew;
            int boottrk;
            off_t offset;
            int type;
            int size;
            int extents; /* logical extents per physical extent */
            int *skewtab;
            PhysDirectoryEntry_t *dir;
            int alvSize;
            int *alv;
            int cnotatime;
            char *label;
            size_t labelLength;
            char *passwd;
            size_t passwdLength;
            int dirtyDirectory;
            DsDate_t *ds;
            int dirtyDs;
        } CpmSuperBlock_t;

        typedef struct {
            ino_t ino;
            mode_t mode;
            off_t size;
            time_t atime;
            time_t mtime;
            time_t ctime;
        } CpmStat_t;

        typedef struct {
            long f_bsize;
            long f_blocks;
            long f_bfree;
            long f_bused;
            long f_bavail;
            long f_files;
            long f_ffree;
            long f_namelen;
        } CpmStatFS_t;

        typedef struct {
            mode_t mode;
            off_t pos;
            CpmInode_t *ino;
        } CpmFile_t;

        typedef struct {
            ino_t ino;
            off_t off;
            size_t reclen;
            char name[2 + 8 + 1 + 3 + 1]; /* 00foobarxy.zzy\0 */
        } CpmDirent_t;

    public:    // Methods
        int cpmReadDiskdefData(const char *format);
        int cpmInitDriveData();
        void cpmglob(const char *argv, int *gargc, char ***gargv);
        void cpmStatFS(CpmStatFS_t *buf);
        int cpmNamei(const char *filename, CpmInode_t *i);
        void cpmStat(const CpmInode_t *ino, CpmStat_t *buf);
        int cpmAttrGet(CpmInode_t *ino, cpm_attr_t *attrib);
        void cpmUmount();
        int cpmUnlink(const char *fname);
        int cpmRename(const char *oldname, const char *newname);
        int cpmAttrSet(CpmInode_t *ino, cpm_attr_t attrib);
        int cpmProtSet(CpmInode_t *ino, mode_t pmode);
        int cpmSync();
        int cpmCreat(CpmInode_t *dir, const char *fname, CpmInode_t *ino, mode_t mode);
        int cpmOpen(CpmInode_t *ino, CpmFile_t *file, mode_t mode);
        int cpmRead(CpmFile_t *file, char *buf, int count);
        int cpmWrite(CpmFile_t *file, const char *buf, int count);
        int cpmClose(CpmFile_t *file);
        void cpmUtime(CpmInode_t *ino, utimbuf *times);
        int mkfs(const char *filename, const char *format, const char *label, char *bootTracks,
                 int timeStamps);
        bool getBootTracksEnabled();
        size_t getBootTrackSize();
        CpmSuperBlock_t &getDriveData();
        CpmInode_t &getDirectoryRoot();
        std::string getErrorMsg();

    public:    // Constructor & Destructor
        CpmFs(CpmDevice *device, std::string appPath);
        virtual ~CpmFs();

    protected:    // Attributes

    protected:    // Methods

    private:    // Attributes
        std::string diskdefsPath;
        CpmDevice *cpmdevice;
        CpmSuperBlock_t drive;
        CpmInode_t root;
        std::string fserr;
        mode_t s_ifdir = 1;
        mode_t s_ifreg = 1;

    private:    // Methods
        void memcpy7(char *dest, const char *src, int count);
        int splitFilename(const char *fullname, int type, char *name, char *ext, int *user);
        int isMatching(int user1, const char *name1, const char *ext1, int user2,
                       const char *name2, const char *ext2);
        time_t cpm2unix_time(int days, int hour, int min);
        void unix2cpm_time(time_t now, int *days, int *hour, int *min);
        time_t ds2unix_time(const DsEntry_t *entry);
        void unix2ds_time(time_t now, DsEntry_t *entry);
        void alvInit();
        int allocBlock();
        int readBlock(int blockno, char *buffer, int start, int end);
        int writeBlock(int blockno, const char *buffer, int start, int end);
        int findFileExtent(int user, const char *name, const char *ext, int start, int extno);
        int findFreeExtent();
        void updateTimeStamps(const CpmInode_t *ino, int extent);
        void updateDsStamps(const CpmInode_t *ino, int extent);
        int readTimeStamps(CpmInode_t *i, int lowestExt);
        void readDsStamps(CpmInode_t *i, int lowestExt);
        int recmatch(const char *a, const char *pattern);
        int match(const char *a, const char *pattern);
        int diskdefReadSuper(const char *format);
        int amsReadSuper(const char *format);
        int cpmCheckDs();
        int syncDs();
        int cpmOpendir(CpmFile_t *dirp);
        int cpmReaddir(CpmFile_t *dir, CpmDirent_t *ent);
        std::string msgFormat(const std::string fmt_str, ...);
};

// --------------------------------------------------------------------------------
#endif /* CPMFS_HPP_ */
