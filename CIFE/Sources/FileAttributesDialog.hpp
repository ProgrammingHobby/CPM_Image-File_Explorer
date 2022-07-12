/***********************************************************************************
**
**	Copyright (C) 01.12.2021 Uwe Merker Germany
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

#ifndef FILEATTRIBUTESDIALOG_H
#define FILEATTRIBUTESDIALOG_H
// --------------------------------------------------------------------------------
#include "Ui_FileAttributesDialog.hpp"
// --------------------------------------------------------------------------------
class FileAttributesDialog : public Ui_FileAttributesDialog {
    public:     // Attributes

    public:     // Methods
        void setAttributes(wxString attributes);
        int getAttributes();

    public:     // Constructor & Destructor
        FileAttributesDialog(wxWindow *parent);
        virtual ~FileAttributesDialog();

    protected:  // Event Methods

    private:    // Attributes
        /* CP/M file attributes */
        const int CPM_ATTR_F1 = 1;
        const int CPM_ATTR_F2 = 2;
        const int CPM_ATTR_F3 = 4;
        const int CPM_ATTR_F4 = 8;
        const int CPM_ATTR_RO = 256;   /* Read-only */
        const int CPM_ATTR_SYS = 512;   /* System */
        const int CPM_ATTR_ARCV = 1024;  /* Archive */
        wxCheckBox *checkBoxes[7];

    private:    // Methods
};

// --------------------------------------------------------------------------------
#endif // FILEATTRIBUTESDIALOG_H
