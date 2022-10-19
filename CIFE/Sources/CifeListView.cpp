/*
 * CifeListView.cpp is part of CIFE
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

#include "CifeListView.hpp"
// --------------------------------------------------------------------------------
BEGIN_EVENT_TABLE(CifeListView, wxListView)
    EVT_SIZE(CifeListView::OnSize)
END_EVENT_TABLE()
// --------------------------------------------------------------------------------
CifeListView::CifeListView(wxWindow *parent, wxWindowID id, const wxPoint &pos,
                           const wxSize &size, long style) : wxListView(parent, id, pos, size, style) {
    this->SetBackgroundColour(wxColour(wxT("rgb(203,230,162)")));
    sizingEnabled = false;
}

// --------------------------------------------------------------------------------
CifeListView::~CifeListView() {
}

// --------------------------------------------------------------------------------
void CifeListView::OnSize(wxSizeEvent &event) {
    if (sizingEnabled) {
        int newWidth, colwidths = 0;
        int viewWidth = this->GetClientSize().GetWidth();
        colwidths += newWidth = (viewWidth * 0.151);
        this->SetColumnWidth(0, newWidth);
        colwidths += newWidth = (viewWidth * 0.076);
        this->SetColumnWidth(1, newWidth);
        colwidths += newWidth = (viewWidth * 0.076);
        this->SetColumnWidth(2, newWidth);
        colwidths += newWidth = (viewWidth * 0.107);
        this->SetColumnWidth(3, newWidth);
        colwidths += newWidth = (viewWidth * 0.107);
        this->SetColumnWidth(4, newWidth);
        colwidths += newWidth = (viewWidth * 0.161);
        this->SetColumnWidth(5, newWidth);
        colwidths += newWidth = (viewWidth * 0.161);
        this->SetColumnWidth(6, newWidth);
        colwidths += newWidth = (viewWidth * 0.161);
        this->SetColumnWidth(7, (newWidth + (viewWidth - colwidths)));
        event.Skip();
    }
}

// --------------------------------------------------------------------------------
void CifeListView::enableSizing(bool enable) {
    sizingEnabled = enable;
}

// --------------------------------------------------------------------------------
