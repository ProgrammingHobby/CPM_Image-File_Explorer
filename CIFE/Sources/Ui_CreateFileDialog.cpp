//////////////////////////////////////////////////////////////////////
// This file was auto-generated by codelite's wxCrafter Plugin
// wxCrafter project file: Ui_CreateFileDialog.wxcp
// Do not modify this file by hand!
//////////////////////////////////////////////////////////////////////

#include "Ui_CreateFileDialog.hpp"


// Declare the bitmap loading function
extern void wxCraftertMBioxInitBitmapResources();

static bool bBitmapLoaded = false;


Ui_CreateFileDialog::Ui_CreateFileDialog(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
    : wxDialog(parent, id, title, pos, size, style)
{
    if ( !bBitmapLoaded ) {
        // We need to initialise the default bitmap handler
        wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
        wxCraftertMBioxInitBitmapResources();
        bBitmapLoaded = true;
    }
    
    wxBoxSizer* sizerCreateFileDialog = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(sizerCreateFileDialog);
    
    wxGridBagSizer* sizerImageSettings = new wxGridBagSizer(0, 0);
    
    sizerCreateFileDialog->Add(sizerImageSettings, 1, wxLEFT|wxBOTTOM|wxEXPAND, WXC_FROM_DIP(4));
    
    textImageFile = new wxStaticText(this, wxID_ANY, _("Image File:"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    
    sizerImageSettings->Add(textImageFile, wxGBPosition(0,0), wxGBSpan(1,1), wxTOP|wxALIGN_CENTER_VERTICAL, WXC_FROM_DIP(4));
    
    textImageType = new wxStaticText(this, wxID_ANY, _("ImageType:"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    
    sizerImageSettings->Add(textImageType, wxGBPosition(1,0), wxGBSpan(1,1), wxTOP|wxALIGN_CENTER_VERTICAL, WXC_FROM_DIP(5));
    
    textBootTrackFile = new wxStaticText(this, wxID_ANY, _("Boottrack File:"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    
    sizerImageSettings->Add(textBootTrackFile, wxGBPosition(2,0), wxGBSpan(1,1), wxTOP|wxALIGN_CENTER_VERTICAL, WXC_FROM_DIP(4));
    
    textFileSystemLabel = new wxStaticText(this, wxID_ANY, _("File-System Label:"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    
    sizerImageSettings->Add(textFileSystemLabel, wxGBPosition(3,0), wxGBSpan(1,1), wxTOP|wxALIGN_CENTER_VERTICAL, WXC_FROM_DIP(4));
    
    editFileSystemLabel = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    #if wxVERSION_NUMBER >= 3000
    editFileSystemLabel->SetHint(wxT(""));
    #endif
    
    sizerImageSettings->Add(editFileSystemLabel, wxGBPosition(3,1), wxGBSpan(1,1), wxLEFT|wxTOP|wxBOTTOM|wxALIGN_CENTER_VERTICAL, WXC_FROM_DIP(4));
    
    checkboxUseTimeStamps = new wxCheckBox(this, wxID_ANY, _("use Timestamps"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), wxCHK_2STATE);
    checkboxUseTimeStamps->SetValue(true);
    
    sizerImageSettings->Add(checkboxUseTimeStamps, wxGBPosition(4,1), wxGBSpan(1,1), wxTOP|wxBOTTOM, WXC_FROM_DIP(4));
    
    panelImageType = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), wxTAB_TRAVERSAL);
    
    sizerImageSettings->Add(panelImageType, wxGBPosition(1,1), wxGBSpan(1,2), wxALL|wxEXPAND, WXC_FROM_DIP(4));
    
    wxBoxSizer* sizerImageType = new wxBoxSizer(wxHORIZONTAL);
    panelImageType->SetSizer(sizerImageType);
    
    editImageType = new wxTextCtrl(panelImageType, wxID_ANY, wxT(""), wxDefaultPosition, wxDLG_UNIT(panelImageType, wxSize(-1,-1)), wxTE_READONLY);
    #if wxVERSION_NUMBER >= 3000
    editImageType->SetHint(wxT(""));
    #endif
    
    sizerImageType->Add(editImageType, 3, wxTOP, WXC_FROM_DIP(4));
    
    sizerImageType->Add(-1, -1, 1, 0, WXC_FROM_DIP(4));
    
    panelImageFile = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), wxTAB_TRAVERSAL);
    
    sizerImageSettings->Add(panelImageFile, wxGBPosition(0,1), wxGBSpan(1,2), wxLEFT|wxRIGHT|wxTOP|wxEXPAND, WXC_FROM_DIP(4));
    
    wxBoxSizer* sizerImageFile = new wxBoxSizer(wxHORIZONTAL);
    panelImageFile->SetSizer(sizerImageFile);
    
    editImageFile = new wxTextCtrl(panelImageFile, wxID_ANY, wxT(""), wxDefaultPosition, wxDLG_UNIT(panelImageFile, wxSize(-1,-1)), wxTE_READONLY);
    #if wxVERSION_NUMBER >= 3000
    editImageFile->SetHint(wxT(""));
    #endif
    
    sizerImageFile->Add(editImageFile, 1, wxTOP|wxEXPAND, WXC_FROM_DIP(4));
    
    buttonImageFile = new wxButton(panelImageFile, wxID_BUTTON_IMAGEFILE, _("Browse"), wxDefaultPosition, wxDLG_UNIT(panelImageFile, wxSize(-1,-1)), 0);
    
    sizerImageFile->Add(buttonImageFile, 0, wxLEFT|wxTOP|wxALIGN_CENTER_VERTICAL, WXC_FROM_DIP(4));
    
    panelBootTrackFile = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), wxTAB_TRAVERSAL);
    
    sizerImageSettings->Add(panelBootTrackFile, wxGBPosition(2,1), wxGBSpan(1,2), wxALL|wxEXPAND, WXC_FROM_DIP(4));
    
    wxBoxSizer* sizerBootTrackFile = new wxBoxSizer(wxHORIZONTAL);
    panelBootTrackFile->SetSizer(sizerBootTrackFile);
    
    editBootTrackFile = new wxTextCtrl(panelBootTrackFile, wxID_ANY, wxT(""), wxDefaultPosition, wxDLG_UNIT(panelBootTrackFile, wxSize(-1,-1)), wxTE_READONLY);
    #if wxVERSION_NUMBER >= 3000
    editBootTrackFile->SetHint(wxT(""));
    #endif
    
    sizerBootTrackFile->Add(editBootTrackFile, 1, wxEXPAND, WXC_FROM_DIP(4));
    
    buttonBootTrackFile = new wxButton(panelBootTrackFile, wxID_BUTTON_BOOTFILE, _("Browse"), wxDefaultPosition, wxDLG_UNIT(panelBootTrackFile, wxSize(-1,-1)), 0);
    
    sizerBootTrackFile->Add(buttonBootTrackFile, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, WXC_FROM_DIP(4));
    sizerImageSettings->AddGrowableCol(1);
    sizerImageSettings->AddGrowableRow(0);
    panelCreationWarning = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), wxTAB_TRAVERSAL|wxBORDER_THEME);
    
    sizerCreateFileDialog->Add(panelCreationWarning, 0, wxLEFT|wxRIGHT|wxEXPAND, WXC_FROM_DIP(4));
    
    wxBoxSizer* sizerCreationWarning = new wxBoxSizer(wxVERTICAL);
    panelCreationWarning->SetSizer(sizerCreationWarning);
    
    textWarning1 = new wxStaticText(panelCreationWarning, wxID_ANY, wxT(""), wxDefaultPosition, wxDLG_UNIT(panelCreationWarning, wxSize(-1,-1)), 0);
    
    sizerCreationWarning->Add(textWarning1, 0, wxLEFT|wxRIGHT|wxTOP, WXC_FROM_DIP(8));
    
    textWarning2 = new wxStaticText(panelCreationWarning, wxID_ANY, wxT(""), wxDefaultPosition, wxDLG_UNIT(panelCreationWarning, wxSize(-1,-1)), 0);
    
    sizerCreationWarning->Add(textWarning2, 0, wxLEFT|wxRIGHT|wxBOTTOM, WXC_FROM_DIP(8));
    
    wxBoxSizer* sizerDialogButtons = new wxBoxSizer(wxHORIZONTAL);
    
    sizerCreateFileDialog->Add(sizerDialogButtons, 0, wxALL|wxALIGN_RIGHT, WXC_FROM_DIP(4));
    
    buttonCancel = new wxButton(this, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    
    sizerDialogButtons->Add(buttonCancel, 1, wxALL|wxEXPAND, WXC_FROM_DIP(4));
    
    buttonOk = new wxButton(this, wxID_OK, _("Ok"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    
    sizerDialogButtons->Add(buttonOk, 1, wxALL|wxEXPAND, WXC_FROM_DIP(4));
    
    SetName(wxT("Ui_CreateFileDialog"));
    SetSize(wxDLG_UNIT(this, wxSize(-1,-1)));
    if (GetSizer()) {
         GetSizer()->Fit(this);
    }
    if(GetParent()) {
        CentreOnParent(wxBOTH);
    } else {
        CentreOnScreen(wxBOTH);
    }
}

Ui_CreateFileDialog::~Ui_CreateFileDialog()
{
}
