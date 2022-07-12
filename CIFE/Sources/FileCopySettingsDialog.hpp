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

#ifndef FILECOPYSETTINGSDIALOG_HPP
#define FILECOPYSETTINGSDIALOG_HPP
// --------------------------------------------------------------------------------
#include "Ui_FileCopySettingsDialog.hpp"
// --------------------------------------------------------------------------------
class Settings;
// --------------------------------------------------------------------------------
class FileCopySettingsDialog : public Ui_FileCopySettingsDialog {

    public:     // Attributes

    public:     // Methods

    public:     // Constructor & Destructor
        FileCopySettingsDialog(wxWindow *parent, Settings *settings);
        virtual ~FileCopySettingsDialog();

    protected:  // Event Methods

    private:    // Attributes
        Settings *dialogSettings;

    private:    // Methods
        void correctDialogSize();
};

// --------------------------------------------------------------------------------
#endif // FILECOPYSETTINGSDIALOG_HPP