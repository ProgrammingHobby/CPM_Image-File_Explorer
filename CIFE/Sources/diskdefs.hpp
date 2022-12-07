/*
 * diskdefs.hpp is part of CIFE
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

#ifndef DISKDEFS_HPP_
#define DISKDEFS_HPP_
// --------------------------------------------------------------------------------
#include <wx/string.h>
#include <wx/arrstr.h>
// --------------------------------------------------------------------------------
namespace diskdefs {
    void getImageTypes(wxString appPath);
    extern wxArrayString imageTypes;
} /* namespace diskdefs */

// --------------------------------------------------------------------------------
#endif /* DISKDEFS_HPP_ */
