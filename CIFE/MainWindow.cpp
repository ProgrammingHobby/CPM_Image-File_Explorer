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

#include "MainWindow.h"
// --------------------------------------------------------------------------------
BEGIN_EVENT_TABLE(MainWindow, wxFrame)
END_EVENT_TABLE()
// --------------------------------------------------------------------------------
MainWindow::MainWindow(wxWindow *parent) : Ui_MainWindow(parent) {

    int size = editImageFile->GetSize().GetHeight();
    buttonImageFile->SetMinSize(wxSize(size, size));
    buttonImageFile->SetMaxSize(wxSize(size, size));

    this->Fit();
    this->SetMinSize(this->GetSize());
    this->SetTitle(wxT("CP/M Image File Explorer - © Uwe Merker 2021"));
}

// --------------------------------------------------------------------------------
MainWindow::~MainWindow() {
}

// --------------------------------------------------------------------------------
