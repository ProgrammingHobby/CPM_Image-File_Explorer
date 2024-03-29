//////////////////////////////////////////////////////////////////////
// This file was auto-generated by codelite's wxCrafter Plugin
// wxCrafter project file: Ui_MainWindow.wxcp
// Do not modify this file by hand!
//////////////////////////////////////////////////////////////////////

#ifndef _CIFE_GUI_DESIGNS_UI_MAINWINDOW_BASE_CLASSES_HPP
#define _CIFE_GUI_DESIGNS_UI_MAINWINDOW_BASE_CLASSES_HPP

// clang-format off
#include <wx/settings.h>
#include <wx/xrc/xmlres.h>
#include <wx/xrc/xh_bmp.h>
#include <wx/frame.h>
#include <wx/iconbndl.h>
#include <wx/artprov.h>
#include <wx/sizer.h>
#include <wx/menu.h>
#include <wx/statusbr.h>
#include <wx/panel.h>
#include <wx/gbsizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/splitter.h>
#include "CifeListView.hpp"
#include <wx/statline.h>
#include <wx/button.h>
#if wxVERSION_NUMBER >= 2900
#include <wx/persist.h>
#include <wx/persist/toplevel.h>
#include <wx/persist/bookctrl.h>
#include <wx/persist/treebook.h>
#endif

#ifdef WXC_FROM_DIP
#undef WXC_FROM_DIP
#endif
#if wxVERSION_NUMBER >= 3100
#define WXC_FROM_DIP(x) wxWindow::FromDIP(x, NULL)
#else
#define WXC_FROM_DIP(x) x
#endif

// clang-format on

class Ui_MainWindow : public wxFrame
{
public:
    enum {
        wxID_BUTTON_SAVE_MESSAGES = 10001,
        wxID_FILE_NEW = 10002,
        wxID_FILE_OPEN = 10003,
        wxID_COPY_SETTINGS = 10004,
        wxID_CLEAR_HISTORY = 10005,
        wxID_FILE_CLOSE = 10006,
        wxID_ATTRIBUTES = 10007,
        wxID_PROTECTIONS = 10008,
        wxID_GENERAL_SETTINGS = 10009,
        wxID_IMAGE_CONTENTS = 10010,
        wxID_CREATE_NEW = 10011,
        wxID_QUIT = 10012,
        wxID_CHECK_IMAGE = 10013,
        wxID_BUTTON_CLEAR_MESSAGES = 10014,
    };
protected:
    wxMenuBar* menuMainWindow;
    wxMenu* menuFile;
    wxMenuItem* menuItemNew;
    wxMenuItem* menuItemOpen;
    wxMenu* menuRecentFiles;
    wxMenuItem* menuItemSeparator;
    wxMenuItem* menuItemClearHistory;
    wxMenuItem* menuItemSeparator1;
    wxMenuItem* menuItemFileClose;
    wxMenuItem* menuItemSeparator2;
    wxMenuItem* menuItemQuit;
    wxMenu* menuEdit;
    wxMenuItem* menuItemCut;
    wxMenuItem* menuItemCopy;
    wxMenuItem* menuItemPaste;
    wxMenuItem* menuItemSelectAll;
    wxMenuItem* menuItemSeparator3;
    wxMenuItem* menuItemRename;
    wxMenuItem* menuItemDelete;
    wxMenuItem* menuItemSeparator4;
    wxMenuItem* menuItemAttributes;
    wxMenuItem* menuItemProtections;
    wxMenuItem* menuItemSeparator5;
    wxMenuItem* menuItemFormatCurrent;
    wxMenu* menuView;
    wxMenuItem* menuItemRefresh;
    wxMenu* menuTools;
    wxMenuItem* menuItemCheckImage;
    wxMenu* menuOptions;
    wxMenuItem* menuItemGeneralSettings;
    wxMenuItem* menuItemCopySettings;
    wxMenu* menuHelp;
    wxMenuItem* menuItemAbout;
    wxStatusBar* statusMainWindow;
    wxPanel* panelImageFile;
    wxStaticText* textImageType;
    wxTextCtrl* editImageType;
    wxStaticText* textImageFile;
    wxTextCtrl* editImageFile;
    wxPanel* panelImageViews;
    wxSplitterWindow* splitterImageViews;
    wxPanel* splitterPageImageContents;
    CifeListView* listImageContents;
    wxStaticLine* lineImageContents;
    wxTextCtrl* textContentsInfo;
    wxPanel* splitterPageMessages;
    wxButton* buttonClearMessages;
    wxButton* buttonSaveMessages;
    wxStaticLine* lineTextMessages;
    wxPanel* panelTextMessages;
    wxTextCtrl* textMessages;

protected:

public:
    wxMenuBar* GetMenuMainWindow() { return menuMainWindow; }
    wxStatusBar* GetStatusMainWindow() { return statusMainWindow; }
    wxStaticText* GetTextImageType() { return textImageType; }
    wxTextCtrl* GetEditImageType() { return editImageType; }
    wxStaticText* GetTextImageFile() { return textImageFile; }
    wxTextCtrl* GetEditImageFile() { return editImageFile; }
    wxPanel* GetPanelImageFile() { return panelImageFile; }
    CifeListView* GetListImageContents() { return listImageContents; }
    wxStaticLine* GetLineImageContents() { return lineImageContents; }
    wxTextCtrl* GetTextContentsInfo() { return textContentsInfo; }
    wxPanel* GetSplitterPageImageContents() { return splitterPageImageContents; }
    wxButton* GetButtonClearMessages() { return buttonClearMessages; }
    wxButton* GetButtonSaveMessages() { return buttonSaveMessages; }
    wxStaticLine* GetLineTextMessages() { return lineTextMessages; }
    wxTextCtrl* GetTextMessages() { return textMessages; }
    wxPanel* GetPanelTextMessages() { return panelTextMessages; }
    wxPanel* GetSplitterPageMessages() { return splitterPageMessages; }
    wxSplitterWindow* GetSplitterImageViews() { return splitterImageViews; }
    wxPanel* GetPanelImageViews() { return panelImageViews; }
    Ui_MainWindow(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("CP/M Image File Explorer"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(640,480), long style = wxDEFAULT_FRAME_STYLE);
    virtual ~Ui_MainWindow();
};

#endif
