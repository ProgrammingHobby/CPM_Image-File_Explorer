/***********************************************************************************
**
**	Copyright (C) 08.06.2022 Uwe Merker Germany
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

#ifndef CIFELISTVIEW_H
#define CIFELISTVIEW_H
// --------------------------------------------------------------------------------
#include <wx/listctrl.h>
// --------------------------------------------------------------------------------
class CifeListView : public wxListView {

    public:     // Attributes

    public:     // Methods
        void enableSizing(bool enable = false);

    public:     // Constructor & Destructor
        CifeListView(wxWindow *parent, wxWindowID id = wxID_ANY, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize, long style = wxTAB_TRAVERSAL);
        virtual ~CifeListView();

    protected:  // Event Methods
        void OnSize(wxSizeEvent &event);

    private:    // Attributes
        bool sizingEnabled;

    private:    // Methods

        DECLARE_EVENT_TABLE()

};

// --------------------------------------------------------------------------------

#endif // CIFELISTVIEW_H
