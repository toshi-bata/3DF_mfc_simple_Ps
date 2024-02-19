// HSampleHView.cpp : implementation of the HSampleHView class
// 
#include "stdafx.h"
#include "HSampleHView.h"
#include "HSampleHSelectionSet.h"
#include "hc.h"


HSampleHView::HSampleHView(HBaseModel *model,
    const char * 		alias,	
    const char *		driver_type,
    const char *		instance_name,
    void *		window_handle,
    void *		colormap) : HBaseView(model, alias, driver_type, instance_name, window_handle, colormap)

{
}

HSampleHView::~HSampleHView()
{
}

// app-specific init function
void HSampleHView::Init()
{
	// call base's init function first to get the default HOOPS hierarchy for the view
	HBaseView::Init();

	// create our app-specific Selection object and initialize
	m_pSelection = new HSampleHSelectionSet(this);
    m_pSelection->Init();

	// TODO: Add your initialization here
}

