#if !defined(AFX_CSampleHDoc_H__FCA603FC_1694_11D2_B736_00805F85736F__INCLUDED_)
#define AFX_CSampleHDoc_H__FCA603FC_1694_11D2_B736_00805F85736F__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// SampleHDoc.h : header file
//

#include "CHoopsDoc.h"
#include "HModelInfo.h"

/////////////////////////////////////////////////////////////////////////////
// CSampleHDoc document

class CSampleHDoc : public CHoopsDoc
{
protected:
	CSampleHDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CSampleHDoc)

// Attributes

public:

// Operations

	void ReadPostProcess();
	bool m_bStreamFile;
	bool m_bSolidFile;

public:

	long UpdateModelGeometryType();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSampleHDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSampleHDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CSampleHDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CSampleHDoc_H__FCA603FC_1694_11D2_B736_00805F85736F__INCLUDED_)


