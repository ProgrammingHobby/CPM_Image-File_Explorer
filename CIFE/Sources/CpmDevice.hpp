/*
 * CpmDevice.hpp is part of CIFE
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

#ifndef CPMDEVICE_HPP_
#define CPMDEVICE_HPP_
// --------------------------------------------------------------------------------
#include <cstdio>
#include <string>
// --------------------------------------------------------------------------------
class CpmDevice {
    public:    // Attributes

    public:    // Methods
        bool Open(const char *filename, const char *mode);
        void SetGeometry(int secLength, int sectrk, int tracks, long offset);
        bool Close();
        bool ReadSector(int track, int sector, char *buffer);
        bool WriteSector(int track, int sector, const char *buffer);
        bool isOpen();
        std::string getErrorMsg();

    public:    // Constructor & Destructor
        CpmDevice();
        virtual ~CpmDevice();

    protected:    // Attributes

    protected:    // Methods

    private:    // Attributes
        typedef struct {
            bool opened;
            int secLength;
            int tracks;
            int sectrk;
            off_t offset;
            FILE *file;
        } Device_t;

        Device_t device;
        std::string deverr;

    private:    // Methods
        std::string msgFormat(const std::string fmt_str, ...);
};

// --------------------------------------------------------------------------------
#endif /* CPMDEVICE_HPP_ */
