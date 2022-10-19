/*
 * CifeListView.hpp is part of CIFE
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

#ifndef CIFELISTVIEW_H
#define CIFELISTVIEW_H
// --------------------------------------------------------------------------------
#include <wx/listctrl.h>
// --------------------------------------------------------------------------------
class CifeListView : public wxListView {

    public:     // Attributes

    public:     // Methods
        void enableSizing(bool enable = false);

    public:     // Constructor & Destructor
        CifeListView(wxWindow *parent, wxWindowID id = wxID_ANY, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize, long style = wxTAB_TRAVERSAL);
        virtual ~CifeListView();

    protected:  // Event Methods
        void OnSize(wxSizeEvent &event);

    private:    // Attributes
        bool sizingEnabled;

    private:    // Methods

        DECLARE_EVENT_TABLE()

};

// --------------------------------------------------------------------------------

#endif // CIFELISTVIEW_H
