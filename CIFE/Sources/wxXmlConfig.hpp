//*****************************************************************************
// @file	wxXMLConfig.h
// @author	Nicolas Dextraze
// @web		http://www.nicdex.com
// @date	February 17, 2007
// @desc	wxXMLConfig class header
//*****************************************************************************
// 2022_11	Changed private attributes order and reformatted (Uwe Merker)
//*****************************************************************************

#ifndef WXXMLCONFIG_HPP_
#define WXXMLCONFIG_HPP_
// --------------------------------------------------------------------------------
#include <wx/textfile.h>
#include <wx/confbase.h>
#include <wx/xml/xml.h>
#ifdef wxUSE_STREAMS
    #include <wx/stream.h>
#endif
// --------------------------------------------------------------------------------
class wxXmlConfig;
// --------------------------------------------------------------------------------
#ifdef wxConfig
    #undef wxConfig
#endif
#define wxConfig wxXmlConfig
// --------------------------------------------------------------------------------
#define wxXmlConfigEntry wxXmlNode
#define wxXmlConfigGroup wxXmlNode
// --------------------------------------------------------------------------------
class wxXmlConfig : public wxConfigBase {

    public: // Attributes

    public: // Methods
        wxString GetGlobalFileName(const wxChar *szFile);
        wxString GetLocalFileName(const wxChar *szFile);
        virtual void SetPath(const wxString &strPath);
        virtual const wxString &GetPath() const;
        virtual bool GetFirstGroup(wxString &str, long &lIndex) const;
        virtual bool GetNextGroup(wxString &str, long &lIndex) const;
        virtual bool GetFirstEntry(wxString &str, long &lIndex) const;
        virtual bool GetNextEntry(wxString &str, long &lIndex) const;
        virtual size_t GetNumberOfEntries(bool bRecursive = false) const;
        virtual size_t GetNumberOfGroups(bool bRecursive = false) const;
        virtual bool HasGroup(const wxString &strName) const;
        virtual bool HasEntry(const wxString &strName) const;
        virtual bool Flush(bool bCurrentOnly = false);
        virtual bool RenameEntry(const wxString &oldName, const wxString &newName);
        virtual bool RenameGroup(const wxString &oldName, const wxString &newName);
        virtual bool DeleteEntry(const wxString &key, bool bGroupIfEmptyAlso = true);
        virtual bool DeleteGroup(const wxString &key);
        virtual bool DeleteAll();

#if wxUSE_STREAMS
        virtual bool Save(wxOutputStream &os, const wxMBConv &conv = wxConvAuto());
#endif

    public: // Constructor & Destructor
        wxXmlConfig(const wxString &appName = wxEmptyString,
                    const wxString &vendorName = wxEmptyString,
                    const wxString &localFilename = wxEmptyString,
                    const wxString &globalFilename = wxEmptyString,
                    long style = wxCONFIG_USE_LOCAL_FILE,
                    const wxMBConv &conv = wxConvAuto());
#ifdef wxUSE_STREAMS
        wxXmlConfig(wxInputStream &inStream, const wxMBConv &conv = wxConvAuto());
#endif
        virtual ~wxXmlConfig();

    protected: // Attributes

    protected: // Methods
        virtual bool DoReadString(const wxString &key, wxString *pStr) const;
        virtual bool DoReadLong(const wxString &key, long *pl) const;
        virtual bool DoWriteString(const wxString &key, const wxString &szValue);
        virtual bool DoWriteLong(const wxString &key, long lValue);
#if wxCHECK_VERSION(2,9,0) && wxUSE_BASE64
        virtual bool DoReadBinary(const wxString &key, wxMemoryBuffer *buf) const;
        virtual bool DoWriteBinary(const wxString &key, const wxMemoryBuffer &buf);
#endif

    private: // Attributes
        wxString m_strPath;
        wxString m_strLocalFile;
        wxString m_strGlobalFile;
        wxXmlConfigGroup *m_pCurrentGroup;
        wxXmlDocument *m_xmlDoc;
        bool m_isDirty;

    private: // Methods
        wxString GetGlobalDir();
        wxString GetLocalDir();
        void Init();
        void CleanUp();
        void Parse(const wxXmlDocument &xmlDocument, bool bLocal);
        void SetRootPath();
        bool DoSetPath(const wxString &strPath, bool createMissingComponents);
        void SetDirty();
        void ResetDirty();
        bool IsDirty();
        bool IsGroup(const wxXmlConfigGroup *group) const;
        bool IsEntry(const wxXmlConfigEntry *entry) const;

        wxXmlConfigEntry *FindEntry(const wxString &key) const;
        wxXmlConfigEntry *AddEntry(const wxString &key);
        wxXmlConfigGroup *FindGroup(const wxString &key) const;
        wxXmlConfigGroup *AddGroup(const wxString &key);

        DECLARE_NO_COPY_CLASS(wxXmlConfig);
};
// --------------------------------------------------------------------------------

#endif /* WXXMLCONFIG_HPP_ */
