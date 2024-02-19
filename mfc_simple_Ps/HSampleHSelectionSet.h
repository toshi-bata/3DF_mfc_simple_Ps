// HSampleHSelectionSet.h : interface of the HSampleHSelectionSet class
// manages list of selected (highlighted) items for view

#ifndef _HSampleHSelectionSet_H 
#define _HSampleHSelectionSet_H

#include "HSelectionSet.h"

class HSampleHSelectionSet : public HSelectionSet
{

public:

	HSampleHSelectionSet(HBaseView* view);

	virtual void Select(HC_KEY key, int num_include_keys, HC_KEY * include_keys, bool emit_message = false);
	virtual void DeSelect(HC_KEY key, int num_include_keys, HC_KEY * include_keys, bool emit_message = false);
	virtual void DeSelectAll();

};

#endif  // _HSampleHSelectionSet_H


