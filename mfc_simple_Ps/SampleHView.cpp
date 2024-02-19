// CSampleHView.cpp :implementation of the CParaHoopsView class
//

// Standard includes
#include "StdAfx.h"
#include "Resource.h"

#include "SampleH.h"
#include "SampleHView.h"
#include "SampleHDoc.h"
#include "MainFrm.h"

#include "HSampleHModel.h"
#include "HSampleHView.h"
#include "HBaseOperator.h"
#include "HOpCameraOrbit.h"
#include "HOpCameraZoom.h"
#include "HOpCameraPan.h"
#include "HOpCameraZoomBox.h"
#include "HOpSelectArea.h"

#include "HOpCreateCone.h"
#include "HOpCreateSphere.h"
#include "HOpCreateCylinder.h"

#include "HSampleHSelectionSet.h"

#include "HIOManager.h"

#include "hc.h"

#include "HUtilityLocaleString.h"

#include "CreateBlockDlg.h"
#include "EntityOneClickOperator.h"
#include "PsBooleanOperator.h"

extern CSampleHApp	theApp;

/////////////////////////////////////////////////////////////////////////////
// CSampleHView

IMPLEMENT_DYNCREATE(CSampleHView, CHoopsView)


CSampleHView::CSampleHView()
{
}

CSampleHView::~CSampleHView()
{
	H_SAFE_DELETE(m_pHView);
}

BEGIN_MESSAGE_MAP(CSampleHView, CHoopsView)
	//{{AFX_MSG_MAP(CSampleHView)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_COMMAND(ID_GOURAUD_SHADED, OnGouraudShaded)
	ON_COMMAND(ID_WIREFRAME, OnWireframe)
	ON_COMMAND(ID_HIDDENLINE, OnHiddenline)
	ON_COMMAND(ID_ZOOM, OnZoom)
	ON_COMMAND(ID_ZOOM_TO_EXTENTS, OnZoomToExtents)
	ON_COMMAND(ID_ZOOM_TO_WINDOW, OnZoomToWindow)
	ON_COMMAND(ID_ORBIT, OnOrbit)
	ON_UPDATE_COMMAND_UI(ID_ORBIT, OnUpdateOrbit)
	ON_COMMAND(ID_PAN, OnPan)
	ON_UPDATE_COMMAND_UI(ID_PAN, OnUpdatePan)
	ON_COMMAND(ID_WINDOW_SELECT, OnWindowSelect)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_SELECT, OnUpdateWindowSelect)
	ON_UPDATE_COMMAND_UI(ID_ZOOM, OnUpdateZoom)
	ON_UPDATE_COMMAND_UI(ID_ZOOM_TO_WINDOW, OnUpdateZoomToWindow)
	ON_COMMAND(ID_CREATE_CONE, OnCreateCone)
	ON_COMMAND(ID_CREATE_CYLINDER, OnCreateCylinder)
	ON_COMMAND(ID_CREATE_SPHERE, OnCreateSphere)
	ON_UPDATE_COMMAND_UI(ID_CREATE_CONE, OnUpdateCreateCone)
	ON_UPDATE_COMMAND_UI(ID_CREATE_CYLINDER, OnUpdateCreateCylinder)
	ON_UPDATE_COMMAND_UI(ID_CREATE_SPHERE, OnUpdateCreateSphere)
	ON_COMMAND(ID_RUN_MY_CODE, OnRunMyCode)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_PARASOLID_CREATEBLOCK, &CSampleHView::OnParasolidCreateblock)
	ON_COMMAND(ID_PARASOLID_DELETEBODY, &CSampleHView::OnParasolidDeletebody)
	ON_COMMAND(ID_PARASOLID_BLEND, &CSampleHView::OnParasolidBlend)
	ON_COMMAND(ID_Menu, &CSampleHView::OnBooleanUnite)
	ON_COMMAND(ID_BOOLEAN_SUBTRACT, &CSampleHView::OnBooleanSubtract)
	ON_COMMAND(ID_BOOLEAN_INTERSECT, &CSampleHView::OnBooleanIntersect)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSampleHView diagnostics

#ifdef _DEBUG
void CSampleHView::AssertValid() const
{
	CHoopsView::AssertValid();
}

void CSampleHView::Dump(CDumpContext& dc) const
{
	CHoopsView::Dump(dc);
}
#endif //_DEBUG


void CSampleHView::LocalSetOperator(HBaseOperator * NewOperator) 
{
	const char* name = NewOperator->GetName();
	HC_Open_Segment_By_Key(m_pHView->GetSceneKey());
	if (!strcmp(name, "BLEND"))
		HC_Set_Visibility("lines = on");
	else
		HC_Set_Visibility("lines = off");
	HC_Close_Segment();
	m_pHView->Update();

	HBaseOperator * op = GetMyBaseView()->GetOperator();
	GetMyBaseView()->SetOperator(NewOperator);

	if (op)
		delete op;

}


/////////////////////////////////////////////////////////////////////////////
// CSampleHView message handlers


// resets the camera to view the world space extents of the model
void CSampleHView::OnZoomToExtents() 
{
	m_pHView->ZoomToExtents();
}


// sets wireframe render mode
void CSampleHView::OnWireframe() 
{
    m_pHView->RenderWireframe();
	m_pHView->Update();
}

// sets gouraud-shaded render mode
void CSampleHView::OnGouraudShaded() 
{
    m_pHView->RenderGouraud();	
	m_pHView->Update();
}


void CSampleHView::OnZoom() 
{
    LocalSetOperator(new HOpCameraZoom(m_pHView));
}


void CSampleHView::OnZoomToWindow() 
{
    LocalSetOperator(new HOpCameraZoomBox(m_pHView));
}

void CSampleHView::OnOrbit() 
{
    LocalSetOperator(new HOpCameraOrbit(m_pHView));	
}

void CSampleHView::OnPan() 
{
	LocalSetOperator(new HOpCameraPan(m_pHView));
}


void CSampleHView::OnWindowSelect() 
{
    LocalSetOperator(new HOpSelectArea(m_pHView));
}


void CSampleHView::OnUpdateOrbit(CCmdUI* pCmdUI) 
{
	if (streq(m_pHView->GetOperator()->GetName(), "HOpCameraOrbit"))
		pCmdUI->SetCheck (1);
    else
		pCmdUI->SetCheck (0);
}

void CSampleHView::OnUpdatePan(CCmdUI* pCmdUI) 
{
	if (streq(m_pHView->GetOperator()->GetName(), "HOpCameraPan"))
		pCmdUI->SetCheck (1);
    else
        pCmdUI->SetCheck (0);
}

void CSampleHView::OnUpdateZoom(CCmdUI* pCmdUI) 
{
	if (streq(m_pHView->GetOperator()->GetName(),"HOpCameraZoom"))
		pCmdUI->SetCheck (1);
    else
        pCmdUI->SetCheck (0);	
}

void CSampleHView::OnUpdateZoomToWindow(CCmdUI* pCmdUI) 
{
	if (streq(m_pHView->GetOperator()->GetName(), "HOpCameraZoomBox"))
        pCmdUI->SetCheck (1);
    else
        pCmdUI->SetCheck (0);	

}

void CSampleHView::OnUpdateWindowSelect(CCmdUI* pCmdUI) 
{
	if (streq(m_pHView->GetOperator()->GetName(), "HOpSelectArea"))
        pCmdUI->SetCheck (1);
    else
        pCmdUI->SetCheck (0);



}

void CSampleHView::OnCreateCone() 
{
    LocalSetOperator(new HOpCreateCone(m_pHView));
}


void CSampleHView::OnCreateCylinder() 
{
    LocalSetOperator(new HOpCreateCylinder(m_pHView));		
}

void CSampleHView::OnCreateSphere() 
{
    LocalSetOperator(new HOpCreateSphere(m_pHView));		
}

// this functions creates a new HBaseView Object, and initializes its camera,
// render mode, query dialog, and operator
void CSampleHView::OnInitialUpdate() 
{
	// create the the Hoops View object to be associated with this MFC View
	// as well as a generic HOOPS segment hierarchy

	char        szTemp[256];

	// DEBUG_STARTUP_CLEAR_BLACK = 0x00004000 makes ogl driver startup black
    long        debug_flags = 0x00004000;

	// we must call the base class initialization first to get the proper palette for this view
	// (either the same palette is shared among all views, or this view has a unique palette)
	CHoopsView::OnInitialUpdate();

	CSampleHDoc * pDoc = (CSampleHDoc *)GetDocument();
	ASSERT_VALID(pDoc);
 
	HBaseModel * hmodel = pDoc->GetHoopsModel();

	// create the HOOPS BaseView object corresponding with this MFC View, passing in the 
	// Model Object, the MFC View's window handle, and palette
    m_pHView = new HSampleHView(hmodel, NULL, "opengl", NULL, (void *) m_hWnd, (void *)(HPALETTE)*(GetPalette()));

	// initialize view(s)
	m_pHView->Init();

    sprintf (szTemp, "debug = %u", debug_flags);
	HC_Open_Segment_By_Key(m_pHView->GetViewKey());
	  HC_Set_Driver_Options(szTemp);
	HC_Close_Segment();

	LocalSetOperator(new HOpCameraOrbit(m_pHView));
    m_pHView->FitWorld();		// fit the camera to the scene extents
	m_pHView->CameraPositionChanged(true);
	m_pHView->RenderGouraud();  // set the render mode to gouraud

	//we need to adjust the axis window outside the mvo class as the calculation of the window
	//extents is mfc specific
 	//AdjustAxisWindow();

	if (pDoc->m_bStreamFile)
	{
		// we need to load the stream the file into the viewport
		m_pHView->GetModel()->SetFileLoadComplete(false);
		HC_Open_Segment_By_Key(m_pHView->GetModel()->GetModelKey());
			TK_Status read_status = HTK_Read_Stream_File(pDoc->GetPathName(), m_pHView->GetModel()->GetStreamFileTK());
		HC_Close_Segment();

		// TODO: Better handling to error during read
		if (read_status == TK_Version)
			AfxMessageBox(ID_ERROR_READ_INCORRECT_VERSION);
		else if( read_status == TK_Error )
			AfxMessageBox(ID_ERROR_READ_INVALID_HSF_FILE);

		m_pHView->GetModel()->SetFileLoadComplete(true);

		// let the doc know that a new file has been loaded
		pDoc->ReadPostProcess();

		m_pHView->FitWorld();		// fit the camera to the scene extents
		m_pHView->CameraPositionChanged(true);
	}
	else if (pDoc->m_bSolidFile)
	{
		m_pHView->FitWorld();		// fit the camera to the scene extents
		m_pHView->CameraPositionChanged(true);
	}

	m_pHView->SetHandednessFromModel();
}

void CSampleHView::OnUpdateCreateCone(CCmdUI* pCmdUI) 
{
	if (streq(m_pHView->GetOperator()->GetName(), "HOpCreateCone"))
		pCmdUI->SetCheck (1);
    else
		pCmdUI->SetCheck (0);


}

void CSampleHView::OnUpdateCreateCylinder(CCmdUI* pCmdUI) 
{
	if (streq(m_pHView->GetOperator()->GetName(), "HOpCreateCylinder"))
		pCmdUI->SetCheck (1);
    else
		pCmdUI->SetCheck (0);


}

void CSampleHView::OnUpdateCreateSphere(CCmdUI* pCmdUI) 
{
	if (streq(m_pHView->GetOperator()->GetName(), "HOpCreateSphere"))
		pCmdUI->SetCheck (1);
    else
		pCmdUI->SetCheck (0);


}


void CSampleHView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	CHoopsView::OnPrint(pDC, pInfo);
}

void CSampleHView::OnFileSaveAs() 
{
#ifndef PARASOLID
	CString filter = _T("HOOPS Stream File (*.hsf)|*.hsf|HOOPS Metafile (*.hmf)|*.hmf|Postscript File (*.ps)|*.ps|HPGL/2 Plot File (*.hp)|*.hp|TIFF Image File (*.tif)|*.tif||");
#else
	CString filter = _T("HOOPS Stream File (*.hsf)|*.hsf|Parasolid Files (*.x_t)|*.x_t|HOOPS Metafile (*.hmf)|*.hmf|Postscript File (*.ps)|*.ps|HPGL/2 Plot File (*.hp)|*.hp|TIFF Image File (*.tif)|*.tif||");
#endif
	CFileDialog my_dlg(FALSE, _T(".hsf"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOREADONLYRETURN, 
						filter, NULL);

	CString pathname;
	int width = 8, height = 10;

	CSampleHDoc * pDoc = (CSampleHDoc *)GetDocument();

	if (my_dlg.DoModal())
	{
		TCHAR extension[1024] = {_T("")}; 
		unsigned int i;

		pathname = my_dlg.GetPathName();

		HUtility::FindFileNameExtension(pathname, extension);

		// make it lower case
		for(i = 0; i < _tcslen(extension); i++)
     		extension[i] = (TCHAR) tolower(extension[i]);

		if (!_tcseq(pathname, _T("")))
		{
			if( _tcseq(extension, _T("hsf")) )
			{
				HC_Open_Segment_By_Key(m_pHView->GetModel()->GetModelKey());
					HStreamFileToolkit* tk = m_pHView->GetModel()->GetStreamFileTK();
					assert(tk);
					tk->Restart();
 					if (HTK_Write_Stream_File(pathname, tk) == TK_Error)
						OutputDebugString(_T("CSampleHView::OnFileSaveAs - Failed to write stream file"));

				HC_Close_Segment();

			}
			else if (_tcseq(extension, _T("x_t")))
			{
				HFileInputResult result = ((HSampleHModel*)(pDoc->m_pHoopsModel))->WriteWithOptions(pathname);
			}
			else {
				HFileOutputResult result;
				result = pDoc->m_pHoopsModel->Write(pathname, m_pHView, static_cast<float>(width), static_cast<float>(height));
				if(result != InputOK){
					char result_str[32];
					HIOManager::GetFileOutputResultString(result, result_str);
					wchar_t message[MVO_BUFFER_SIZE];
					wchar_t result_wstr[32];
					mbstowcs(result_wstr, result_str, 31);
					swprintf(message, MVO_BUFFER_SIZE-1, L"Saving %ls failed with error %ls", (LPCTSTR)pathname, result_wstr);
					MessageBoxW(message, L"Error!", MB_OK);
				}
			}
		}
	}
}

void CSampleHView::OnHiddenline() 
{
	m_pHView->RenderHiddenLine();
	m_pHView->Update();	
}


void CSampleHView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	if (!bActivate)
	{
 		HOpCameraOrbit *op = (HOpCameraOrbit *)m_pHView->GetOperator();
		if (strcmp(op->GetName(),"HOpCameraOrbit")==0)
			op->m_Angle1 = op->m_Angle2 = op->m_Angle3 = 0;			

 		m_pHView->Update();
	}

	CHoopsView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}


void CSampleHView::OnRunMyCode() 
{
	// this is a function in which developers can
	// write their own simple HOOPS functions
	AfxMessageBox(_T("I love HOOPS!!!!!"));
}



void CSampleHView::OnParasolidCreateblock()
{
	CreateBlockDlg *pDlg = new CreateBlockDlg(100.0, 100.0, 100.0, this);
	if (pDlg->DoModal() == IDOK)
	{
		double dXL = pDlg->m_dEditXL;
		double dYL = pDlg->m_dEditYL;
		double dZL = pDlg->m_dEditZL;
		double dOX = pDlg->m_dEditOX;
		double dOY = pDlg->m_dEditOY;
		double dOZ = pDlg->m_dEditOZ;

		CSampleHDoc * pDoc = (CSampleHDoc *)GetDocument();
		((HSampleHModel*)(pDoc->m_pHoopsModel))->CreateBlock(dXL, dYL, dZL, dOX, dOY, dOZ);

		m_pHView->Update();
	}
}


void CSampleHView::OnParasolidDeletebody()
{
	LocalSetOperator(new EntityOneClickOperator(m_pHView, "BODY", "DELETE_BODY"));
}


void CSampleHView::OnParasolidBlend()
{
	LocalSetOperator(new EntityOneClickOperator(m_pHView, "EDGE", "BLEND"));
}


void CSampleHView::OnBooleanUnite()
{
	LocalSetOperator(new PsBooleanOperator(m_pHView, "UNITE"));
}


void CSampleHView::OnBooleanSubtract()
{
	LocalSetOperator(new PsBooleanOperator(m_pHView, "SUBTRACT"));
}


void CSampleHView::OnBooleanIntersect()
{
	LocalSetOperator(new PsBooleanOperator(m_pHView, "INTERSECT"));
}
