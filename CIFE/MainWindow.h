/***********************************************************************************
**
**	Copyright (C) 14.07.2021 Uwe Merker Germany
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H
// --------------------------------------------------------------------------------
#include "Ui_MainWindow.h"
// --------------------------------------------------------------------------------
#include <wx/arrstr.h>
// --------------------------------------------------------------------------------
class CpmTools;
class CpmGuiInterface;
// --------------------------------------------------------------------------------
class MainWindow : public Ui_MainWindow {
    public:     // Attributes

    public:     // Methods

    public:     // Constructor & Destructor
        MainWindow(wxWindow *parent);
        virtual ~MainWindow();

    protected:  // Event Methods
        void onMenuCloseClicked(wxCommandEvent &event);
        void onMenuAboutClicked(wxCommandEvent &event);
        void onButtonImageFileClicked(wxCommandEvent &event);
        void onButtonClearMessagesClicked(wxCommandEvent &event);
        void onButtonSaveMessagesClicked(wxCommandEvent &event);
        void onImageTypeChanged(wxCommandEvent &event);
        void onViewRefresh(wxCommandEvent &event);

    private:    // Attributes
        CpmTools *cpmtools;
        CpmGuiInterface *cpmguiinterface;

    private:    // Methode
        wxArrayString getImageTypes();
        void correctWindowSize();

        DECLARE_EVENT_TABLE()
};

// --------------------------------------------------------------------------------
#endif // MAINWINDOW_H
