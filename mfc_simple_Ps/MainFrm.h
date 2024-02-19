#if !defined(AFX_CMainFrame_H__952D6293_4286_11D2_A564_00A0C989307C__INCLUDED_)
#define AFX_CMainFrame_H__952D6293_4286_11D2_A564_00A0C989307C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// MainFrm.h : header file
//

#include "CHoopsFrm.h"
/////////////////////////////////////////////////////////////////////////////
// CMyHoopsFrm frame

class CMainFrame : public CHoopsFrame
{
protected:

// Attributes
public:

// Operations
public:
	CMainFrame();           // protected constructor used by dynamic creation
	DECLARE_DYNAMIC(CMainFrame)

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyHoopsFrm)
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMainFrame();

	// Generated message map functions
	//{{AFX_MSG(CMyHoopsFrm)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSegmentBrowser();
	afx_msg void OnUpdateTopolbrowser(CCmdUI* pCmdUI); 

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMainFrame_H__952D6293_4286_11D2_A564_00A0C989307C__INCLUDED_)
