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

#ifndef CPMTOOLS_H
#define CPMTOOLS_H
// --------------------------------------------------------------------------------
#include <string>
#include <utime.h>
#include <time.h>
#include <wx/arrstr.h>
// --------------------------------------------------------------------------------
class CpmGuiInterface;
class wxString;
// --------------------------------------------------------------------------------
class CpmTools {

    public:     // Attributes

    public:     // Methods
        void setImageType(wxString typeName);
        void setImageFile(wxString fileName);
        void showDirectory();
        void deleteFile(wxArrayString files);
        void renameFile(wxString oldName, wxString newName);
        void setFileAttributes(wxString name, int attributes);
        void setFileProtections(wxString name, int protections);
        bool getBootTracksEnabled();
        void createNewImage(wxString label, bool useTimeStamps, wxString bootTrackFile);
        void checkImage(bool doRepair);

    public:     // Constructor & Destructor
        CpmTools(CpmGuiInterface *intf);
        ~CpmTools();

    protected:  // Event Methods

    private:    // Attributes
        typedef int cpm_attr_t;

        typedef struct {
            bool opened;
            int secLength;
            int tracks;
            int sectrk;
            off_t offset;
            FILE *file;
        } Device_t;

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

        CpmGuiInterface *guiintf;
        Device_t device;
        CpmSuperBlock_t drive;
        CpmInode_t root;
        std::string imageTypeName;
        std::string imageFileName;
        const char *boo;
        std::string cmd;
        mode_t s_ifdir = 1;
        mode_t s_ifreg = 1;
        int norepair;

        const char *month[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

    private:    // Methods
        // --------------------------------------------------------------------------------
        // Basic File Input/Output
        // --------------------------------------------------------------------------------
        const char *deviceOpen(const char *filename, const char *mode);
        const char *deviceSetGeometry(int secLength, int sectrk, int tracks, long offset);
        const char *deviceClose();
        const char *deviceReadSector(int track, int sector, char *buffer);
        const char *deviceWriteSector(int track, int sector, const char *buffer);
        // --------------------------------------------------------------------------------
        // CP/M File-System
        // --------------------------------------------------------------------------------
        void memcpy7(char *dest, const char *src, int count);
        int splitFilename(const char *fullname, int type, char *name, char *ext, int *user);
        int isMatching(int user1, const char *name1, const char *ext1, int user2, const char *name2, const char *ext2);
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
        void cpmglob(const char *argv, int *gargc, char ***gargv);
        int diskdefReadSuper(const char *format);
        int amsReadSuper(const char *format);
        int cpmCheckDs();
        int cpmReadSuper(const char *format);
        int syncDs();
        int cpmSync();
        void cpmUmount();
        int cpmNamei(const char *filename, CpmInode_t *i);
        void cpmStatFS(CpmStatFS_t *buf);
        int cpmUnlink(const char *fname);
        int cpmRename(const char *oldname, const char *newname);
        int cpmOpendir(CpmFile_t *dirp);
        int cpmReaddir(CpmFile_t *dir, CpmDirent_t *ent);
        void cpmStat(const CpmInode_t *ino, CpmStat_t *buf);
        int cpmOpen(CpmInode_t *ino, CpmFile_t *file, mode_t mode);
        int cpmRead(CpmFile_t *file, char *buf, int count);
        int cpmWrite(CpmFile_t *file, const char *buf, int count);
        int cpmClose(CpmFile_t *file);
        int cpmCreat(CpmInode_t *dir, const char *fname, CpmInode_t *ino, mode_t mode);
        int cpmAttrGet(CpmInode_t *ino, cpm_attr_t *attrib);
        int cpmAttrSet(CpmInode_t *ino, cpm_attr_t attrib);
        int cpmProtSet(CpmInode_t *ino, mode_t pmode);
        void cpmUtime(CpmInode_t *ino, utimbuf *times);
        // --------------------------------------------------------------------------------
        // CP/M Tool-Functions
        // --------------------------------------------------------------------------------
        static int namecmp(const void *a, const void *b);
        int getUserNumber(const char *filename);
        void convertFilename(const char *filename, char *cpmname);
        int bcdCheck(int n, int max, const char *msg, const char *unit, int extent1, int extent2);
        int pwdCheck(int extent, const char *pwd, char decode);
        int ask(const char *msg);
        char *prfile(int extent);
        int mkfs(const char *format, const char *label, char *bootTracks, int timeStamps);
        int fsck(const char *image, bool repair);

};

// --------------------------------------------------------------------------------
#endif // CPMTOOLS_H
