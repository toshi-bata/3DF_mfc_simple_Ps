// CSampleHApp.cpp : Defines the class behaviors for the application.
//

#include "StdAfx.h"
#include "Resource.h"

#include "SampleH.h"
#include "MainFrm.h"
#include "SampleHDoc.h"
#include "SampleHView.h"

#include "ChildFrm.h"

#include "HDB.h"
#include "CTDriver.h"

#include "HUtilityLocaleString.h"

#include "hoops_license.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef PARASOLID
#include "parasolid_kernel.h"
#include "kernel_interface.h"
#include "hp_bridge.h"
#endif // PARASOLID

/////////////////////////////////////////////////////////////////////////////
// CSampleHApp

BEGIN_MESSAGE_MAP(CSampleHApp, CHoopsApp)
	//{{AFX_MSG_MAP(CSampleHApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
#ifdef PARASOLID
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
#endif //PARASOLID
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSampleHApp construction

CSampleHApp::CSampleHApp()
{
	m_pHoopsDB = NULL;
	m_pCTDriver = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSampleHApp object

CSampleHApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSampleHApp initialization

BOOL CSampleHApp::InitInstance()
{
	HC_Define_System_Options("license = `" HOOPS_LICENSE "`");

	m_pHoopsDB = new HDB();
	m_pHoopsDB->Init();
	//m_pCTDriver must be started after m_pHoopsDB is created
	m_pCTDriver = new CTDriver(10);
	m_pCTDriver->StartTimer();

	char fontDirectory[MAX_PATH+32];
	::GetWindowsDirectoryA(fontDirectory, MAX_PATH);
	strcat(fontDirectory, "\\Fonts");
	char buf[MAX_PATH+64] = {""};
	sprintf(buf, "font directory = %s", fontDirectory);
 	HC_Define_System_Options(buf);

	// Standard initialization
	// If you are planning to use this source code in Visual Studio 6.0 or 
	// earlier versions, you should uncomment the following
//#ifdef _AFXDLL
//	Enable3dControls();			// Call this when using MFC in a shared DLL
//#else
//	Enable3dControlsStatic();	// Call this when linking to MFC statically
//#endif

	LoadStdProfileSettings();   // Load standard INI file options (including MRU)

#ifdef PARASOLID

	bool has_schema = false;

	{
		wchar_t *schema_path;
		schema_path = _wgetenv(L"P_SCHEMA");
		if (schema_path)
		{
			HP_Init(schema_path, PK_LOGICAL_false, PK_LOGICAL_true);
			has_schema = true;
		}
	}
	if (!has_schema)
	{
		AfxMessageBox(_T("Could not determine location of your Schema Files.\ndefine P_SCHEMA environment variable."), MB_ICONSTOP);
		// we still wanna initialize the bridge, if not file load, 
		// user can atleast do geom creation and stuff - Rajesh B (30-May-03)
		HP_Init("", PK_LOGICAL_false, PK_LOGICAL_true);
	}

	PK_TOPOL_facet_mesh_o_t     generation_settings;
	PK_TOPOL_facet_mesh_o_m(generation_settings);

	generation_settings.match = PK_facet_match_geom_c;
	generation_settings.is_curve_chord_tol = PK_LOGICAL_false;
	generation_settings.curve_chord_tol = 0.0;
	generation_settings.is_curve_chord_ang = PK_LOGICAL_true;
	generation_settings.curve_chord_ang = 0.6;
	generation_settings.is_surface_plane_tol = PK_LOGICAL_false;
	generation_settings.surface_plane_tol = 0.0;
	generation_settings.is_surface_plane_ang = PK_LOGICAL_true;
	generation_settings.surface_plane_ang = 0.25;
	generation_settings.is_facet_plane_tol = PK_LOGICAL_false;
	generation_settings.facet_plane_tol = 0.0;
	generation_settings.is_facet_plane_ang = PK_LOGICAL_true;
	generation_settings.facet_plane_ang = 0.01;

	PK_TOPOL_render_line_o_t line_opts;
	PK_TOPOL_render_line_o_m(line_opts);

	line_opts.is_curve_chord_tol = PK_LOGICAL_true;
	line_opts.curve_chord_tol = 0.000344;
	line_opts.is_curve_chord_ang = PK_LOGICAL_true;
	line_opts.curve_chord_ang = 0.39269908;

	HP_Set_Tessellation_Options(&line_opts, 0, &generation_settings);
	HP_Set_Rendering_Options("no merge faces");

#endif // PARASOLID


	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_HOOPSTYPE,
		RUNTIME_CLASS(CSampleHDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CSampleHView));
	AddDocTemplate(pDocTemplate);

	// create main MDI Frame window using our custom HOOPS Frame
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The main window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(m_nCmdShow);
	m_pMainWnd->UpdateWindow();

	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void CSampleHApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}



/////////////////////////////////////////////////////////////////////////////
// CHoopsApp commands

int CSampleHApp::ExitInstance() 
{


	//m_pCTDriver must be deleted before m_pHoopsDB
	if (m_pCTDriver) {
		delete m_pCTDriver;
		m_pCTDriver = NULL;
	}

	if (m_pHoopsDB)
	{
		delete m_pHoopsDB;
		m_pHoopsDB = NULL;
	}

#ifdef PARASOLID
	HP_Close();
#endif // PARASOLID

	return CHoopsApp::ExitInstance();
}


#ifdef PARASOLID
void CSampleHApp::OnFileOpen()
{
	CString filter = _T("Parasolid Files (*.x_t, *.x_b, *.xmt_txt, *.xmt_bin)|*.x_t;*.x_b;*.xmt_txt;*.xmt_bin|HOOPS Stream Files (*.hsf)|*.hsf|All Files (*.*)|*.*||");

	CFileDialog dlg(TRUE, _T(".hsf"), NULL, OFN_HIDEREADONLY, filter, NULL);
	if (dlg.DoModal() == IDOK)
	{
		CString pathname;
		pathname = dlg.GetPathName();

		OpenDocumentFile(pathname);
	}
}
#endif // PARASOLID
