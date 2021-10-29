/***********************************************************************************
**
**	Copyright (C) 18.10.2021 Uwe Merker Germany
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

#ifndef RENAMEFILEDIALOG_H
#define RENAMEFILEDIALOG_H
// --------------------------------------------------------------------------------
#include "Ui_RenameFileDialog.h"
// --------------------------------------------------------------------------------
class RenameFileDialog : public Ui_RenameFileDialog {

    public:     // Attributes

    public:     // Methods
        void setOldUser(int user);
        void setOldName(wxString name);
        int getNewUser();
        wxString getNewName();

    public:     // Constructor & Destructor
        RenameFileDialog(wxWindow *parent);
        virtual ~RenameFileDialog();

    protected:  // Event Methods
        void onChangeUserClicked(wxCommandEvent &event);

    private:    // Attributes

    private:    // Methods

        DECLARE_EVENT_TABLE()
};

// --------------------------------------------------------------------------------
#endif // RENAMEFILEDIALOG_H
