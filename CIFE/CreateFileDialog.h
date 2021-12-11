/***********************************************************************************
**
**	Copyright (C) 11.12.2021 Uwe Merker Germany
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

#ifndef CREATEFILEDIALOG_H
#define CREATEFILEDIALOG_H
// --------------------------------------------------------------------------------
#include "Ui_CreateFileDialog.h"
// --------------------------------------------------------------------------------
class CreateFileDialog : public Ui_CreateFileDialog {

    public:     // Attributes

    public:     // Methods
        wxString getBootTrackFile();
        wxString getFileSystemLabel();
        bool getUseTimestamps();

    public:     // Constructor & Destructor
        CreateFileDialog(wxWindow *parent);
        virtual ~CreateFileDialog();

    protected:  // Event Methods
    void onButtonBootFileClicked(wxCommandEvent &event);

    private:    // Attributes

    private:    // Methods

        DECLARE_EVENT_TABLE()
};

// --------------------------------------------------------------------------------
#endif // CREATEFILEDIALOG_H
