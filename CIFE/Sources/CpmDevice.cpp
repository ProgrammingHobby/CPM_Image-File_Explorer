/*
 * CpmDevice.cpp is part of CIFE
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

#include "CpmDevice.hpp"
// --------------------------------------------------------------------------------
#include <cstdio>
#include <cerrno>
#include <cstring>
#include <memory>
#include <cstdarg>
// --------------------------------------------------------------------------------
CpmDevice::CpmDevice() {
}

// --------------------------------------------------------------------------------
CpmDevice::~CpmDevice() {
}

// --------------------------------------------------------------------------------
bool CpmDevice::Open(const char *filename, const char *mode) {
    device.file = fopen(filename, mode);
    device.opened = ((device.file == nullptr) ? false : true);
    deverr = strerror(errno);
    return (device.opened);
}

// --------------------------------------------------------------------------------
void CpmDevice::SetGeometry(int secLength, int sectrk, int tracks, long offset) {
    device.secLength = secLength;
    device.sectrk = sectrk;
    device.tracks = tracks;
    device.offset = offset;
}

// --------------------------------------------------------------------------------
bool CpmDevice::Close() {
    device.opened = ((fclose(device.file) == 0) ? true : false);
    deverr = strerror(errno);
    return (device.opened);
}

// --------------------------------------------------------------------------------
bool CpmDevice::ReadSector(int track, int sector, char *buffer) {
    if (fseek(device.file, (((sector + (track * device.sectrk)) * device.secLength) +
                            device.offset), SEEK_SET) != 0) {
        deverr = strerror(errno);
        return (false);
    }

    int readCount = fread(buffer, sizeof(char), device.secLength, device.file);

    if (readCount != device.secLength) {
        deverr = msgFormat("Read Sector: %d bytes lost. Error: %s",
                           (device.secLength - readCount), strerror(errno));
        memset((buffer + readCount), 0, device.secLength - readCount);
        return (false);
    }

    return (true);
}

// --------------------------------------------------------------------------------
bool CpmDevice::WriteSector(int track, int sector, const char *buffer) {
    if (fseek(device.file, (((sector + (track * device.sectrk)) * device.secLength) +
                            device.offset), SEEK_SET) != 0) {
        deverr = strerror(errno);
        return (false);
    }

    int writeCount = fwrite(buffer, sizeof(char), device.secLength, device.file);

    if (writeCount != device.secLength) {
        deverr = msgFormat("Write Sector: %d bytes lost. Error: %s",
                           (device.secLength - writeCount), strerror(errno));
        return (false);
    }

    return (true);
}

// --------------------------------------------------------------------------------
bool CpmDevice::isOpen() {
    return (device.opened);
}

// --------------------------------------------------------------------------------
std::string CpmDevice::getErrorMsg() {
    return (deverr);
}

// --------------------------------------------------------------------------------
std::string CpmDevice::msgFormat(const std::string fmt_str, ...) {
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


