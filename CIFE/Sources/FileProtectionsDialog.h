/***********************************************************************************
**
**	Copyright (C) 04.12.2021 Uwe Merker Germany
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

#ifndef FILEPROTECTIONSDIALOG_H
#define FILEPROTECTIONSDIALOG_H
// --------------------------------------------------------------------------------
#include "Ui_FileProtectionsDialog.h"
// --------------------------------------------------------------------------------
class FileProtectionsDialog : public Ui_FileProtectionsDialog {
    public:     // Attributes

    public:     // Methods
        void setProtections(wxString protections);
        int getProtections();

    public:     // Constructor & Destructor
        FileProtectionsDialog(wxWindow *parent);
        virtual ~FileProtectionsDialog();

    protected:  // Event Methods

    private:    // Attributes
        const int CPM_ATTR_PWDEL = 2048;    /* Password required to delete */
        const int CPM_ATTR_PWWRITE = 4096;  /* Password required to write */
        const int CPM_ATTR_PWREAD = 8192;   /* Password required to read */

    private:    // Methods
};

// --------------------------------------------------------------------------------
#endif // FILEPROTECTIONSDIALOG_H
