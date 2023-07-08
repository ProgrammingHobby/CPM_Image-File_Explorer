/*
 * CpmDefs.h is part of CIFE
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

#define EXTENT(low,high) (((low)&0x1f)|(((high)&0x3f)<<5))
#define EXTENTL(extent) ((extent)&0x1f)
#define EXTENTH(extent) (((extent>>5))&0x3f)

#define OK 0
#define MODIFIED 1
#define BROKEN 2

#define C0 'G'
#define C1 'E'
#define C2 'H'
#define C3 'E'
#define C4 'I'
#define C5 'M'
#define C6 ' '
#define C7 ' '

#define PB ((char)(C0+C1+C2+C3+C4+C5+C6+C7))
#define PC0 ((char)(C7^PB))
#define PC1 ((char)(C6^PB))
#define PC2 ((char)(C5^PB))
#define PC3 ((char)(C4^PB))
#define PC4 ((char)(C3^PB))
#define PC5 ((char)(C2^PB))
#define PC6 ((char)(C1^PB))
#define PC7 ((char)(C0^PB))

// --------------------------------------------------------------------------------
#endif // CPMDEFS_H
