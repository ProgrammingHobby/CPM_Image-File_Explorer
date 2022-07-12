/***********************************************************************************
**
**	Copyright (C) 29.12.2021 Uwe Merker Germany
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

#ifndef CPMDEFS_H
#define CPMDEFS_H

// --------------------------------------------------------------------------------
#define INTBITS ((int)(sizeof(int)*8))

#define CPMFS_HI_USER    (0x1<<0) /* has user numbers up to 31    */
#define CPMFS_CPM3_DATES (0x1<<1) /* has CP/M+ style time stamps  */
#define CPMFS_CPM3_OTHER (0x1<<2) /* has passwords and disc label */
#define CPMFS_DS_DATES   (0x1<<3) /* has datestamper timestamps   */
#define CPMFS_EXACT_SIZE (0x1<<4) /* has reverse exact file size  */

#define CPMFS_DR22  (CPMFS_HI_USER)
#define CPMFS_P2DOS (CPMFS_CPM3_DATES|CPMFS_HI_USER)
#define CPMFS_DR3   (CPMFS_CPM3_DATES|CPMFS_CPM3_OTHER|CPMFS_HI_USER)
#define CPMFS_ISX   (CPMFS_EXACT_SIZE)
#define CPMFS_ZSYS  (CPMFS_HI_USER)

/* CP/M file attributes */
#define CPM_ATTR_F1		1
#define CPM_ATTR_F2		2
#define CPM_ATTR_F3		4
#define CPM_ATTR_F4		8
/* F5-F8 are banned in CP/M 2 & 3, F7 is used by ZSDOS */
#define CPM_ATTR_RO         256     /* Read-only */
#define CPM_ATTR_SYS        512     /* System */
#define CPM_ATTR_ARCV       1024    /* Archive */
#define CPM_ATTR_PWDEL      2048    /* Password required to delete */
#define CPM_ATTR_PWWRITE    4096    /* Password required to write */
#define CPM_ATTR_PWREAD     8192    /* Password required to read */

#define PASSWD_RECLEN 24
#define RESERVED_ENTRIES 4
#define RESERVED_INODES 3

#define BCD2BIN(x) ((((x)>>4)&0xf)*10 + ((x)&0xf))
#define BIN2BCD(x) (((((x)/10)&0xf)<<4) + (((x)%10)&0xf))

#define ISFILECHAR(notFirst,c) (((notFirst) || (c)!=' ') && (c)>=' ' && !((c)&~0x7f) && (c)!='<' && (c)!='>' && (c)!='.' && (c)!=',' && (c)!=';' && (c)!=':' && (c)!='=' && (c)!='?' && (c)!='*' && (c)!= '[' && (c)!=']')
#define EXTENT(low,high) (((low)&0x1f)|(((high)&0x3f)<<5))
#define EXTENTL(extent) ((extent)&0x1f)
#define EXTENTH(extent) (((extent>>5))&0x3f)

#define OK 0
#define MODIFIED 1
#define BROKEN 2

#define T0 'G'
#define T1 'E'
#define T2 'H'
#define T3 'E'
#define T4 'I'
#define T5 'M'
#define T6 ' '
#define T7 ' '

#define PB ((char)(T0+T1+T2+T3+T4+T5+T6+T7))
#define P0 ((char)(T7^PB))
#define P1 ((char)(T6^PB))
#define P2 ((char)(T5^PB))
#define P3 ((char)(T4^PB))
#define P4 ((char)(T3^PB))
#define P5 ((char)(T2^PB))
#define P6 ((char)(T1^PB))
#define P7 ((char)(T0^PB))

// --------------------------------------------------------------------------------
#endif // CPMDEFS_H
