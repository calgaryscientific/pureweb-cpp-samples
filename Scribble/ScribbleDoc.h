
// ScribbleDoc.h : interface of the CScribbleDoc class
//


#pragma once


class CScribbleDoc : public CDocument
{
protected: // create from serialization only
    CScribbleDoc();
    DECLARE_DYNCREATE(CScribbleDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
    virtual BOOL OnNewDocument();
    virtual void Serialize(CArchive& ar);

// Implementation
public:
    virtual ~CScribbleDoc();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
    DECLARE_MESSAGE_MAP()
};


