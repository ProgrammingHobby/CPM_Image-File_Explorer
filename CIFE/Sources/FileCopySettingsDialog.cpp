/***********************************************************************************
**
**	Copyright (C) 13.05.2022 Uwe Merker Germany
**
**	Diese Datei ist Teil von CPM_Image-File_Explorer.
**
**	CPM_Image-File_Explorer ist Freie Software: Sie können es unter den Bedingungen
**	der GNU General Public License, wie von der Free Software Foundation,
**	Version 3 der Lizenz oder (nach Ihrer Wahl) jeder späteren
**	veröffentlichten Version, weiterverbreiten und/oder modifizieren.
**
**	CPM_Image-File_Explorer wird in der Hoffnung, dass es nützlich sein wird, aber
**	OHNE JEDE GEWÄHRLEISTUNG, bereitgestellt; sogar ohne die implizite
**	Gewährleistung der MARKTFÄHIGKEIT oder EIGNUNG FÜR EINEN BESTIMMTEN ZWECK.
**	Siehe die GNU General Public License für weitere Details.
**
**	Sie sollten eine Kopie der GNU General Public License zusammen mit diesem
**	Programm erhalten haben. Wenn nicht, siehe <http://www.gnu.org/licenses/>.
**
***********************************************************************************/

#include "FileCopySettingsDialog.hpp"
#include "Settings.h"
// --------------------------------------------------------------------------------
FileCopySettingsDialog::FileCopySettingsDialog(wxWindow *parent, Settings *settings) : Ui_FileCopySettingsDialog(parent) {
    dialogSettings = settings;

    spinCtrlDefaultUserNumber->SetValue(dialogSettings->readInteger("CpmOptions", "DefaultUserNumber", 0));
    editTextFileEndings->SetValue(dialogSettings->readString("CpmOptions", "TextfileEndings", "txt pip pas"));

    correctDialogSize();

}

// --------------------------------------------------------------------------------
FileCopySettingsDialog::~FileCopySettingsDialog() {
    dialogSettings->writeInteger("CopySettingsDialog", "PosX", this->GetPosition().x);
    dialogSettings->writeInteger("CopySettingsDialog", "PosY", this->GetPosition().y);
    dialogSettings->writeInteger("CopySettingsDialog", "SizeX", this->GetSize().x);
    dialogSettings->writeInteger("CopySettingsDialog", "SizeY", this->GetSize().y);

    dialogSettings->writeInteger("CpmOptions", "DefaultUserNumber", spinCtrlDefaultUserNumber->GetValue());
    dialogSettings->writeString("CpmOptions", "TextfileEndings", editTextFileEndings->GetValue());
}

// --------------------------------------------------------------------------------
void FileCopySettingsDialog::correctDialogSize() {
    this->SetMinSize(wxSize(0, 0));
    int width = this->GetBestSize().GetWidth();
    int height = this->GetBestSize().GetHeight();
    wxSize size;
    size.x = dialogSettings->readInteger("CopySettingsDialog", "SizeX", width);
    size.y = dialogSettings->readInteger("CopySettingsDialog", "SizeY", (height * 1.5));
    this->SetSize(size);
    this->SetMinSize(wxSize(width, (height * 1.5)));
    wxPoint point;
    point.x = dialogSettings->readInteger("CopySettingsDialog", "PosX", 10);
    point.y = dialogSettings->readInteger("CopySettingsDialog", "PosY", 10);
    this->SetPosition(point);
}

// --------------------------------------------------------------------------------
