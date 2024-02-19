// HSampleHView.h : interface of the HSampleHView class, derived from HBaseView
// Adds application-specific data and members for each view

#ifndef _HSampleHView_H__12323_FAEFAEFADFJHAEF__
#define _HSampleHView_H__12323_FAEFAEFADFJHAEF__

#include "HBaseView.h"

class HSampleHSelectionSet;

class HSampleHView : public HBaseView
{

public:

	HSampleHView(	HBaseModel *model,
							const char * 	alias = 0,	
							const char *	driver_type = 0,
							const char *	instance_name = 0,
							void *			window_handle = 0,
							void *			colormap = 0);

	~HSampleHView();

	void	Init();

	HSampleHSelectionSet* GetMySelection() { return (HSampleHSelectionSet*)m_pSelection; }
};

#endif // _HSampleHView_H__12323_FAEFAEFADFJHAEF__


