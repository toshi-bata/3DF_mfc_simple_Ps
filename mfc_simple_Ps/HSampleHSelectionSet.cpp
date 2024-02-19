#include "StdAfx.h"
#include "hc.h"
#include "HSampleHSelectionSet.h"

HSampleHSelectionSet::HSampleHSelectionSet(HBaseView* view):HSelectionSet(view)
{
}


void HSampleHSelectionSet::Select(HC_KEY key, int num_include_keys, HC_KEY * include_keys, bool emit_message)
{
	// TODO: Process your selection
	HSelectionSet::Select(key, num_include_keys, include_keys, emit_message);
}



void HSampleHSelectionSet::DeSelect(HC_KEY key, int num_include_keys, HC_KEY * include_keys, bool emit_message)
{
	// TODO: Process your de-selection
	HSelectionSet::DeSelect(key, num_include_keys, include_keys, emit_message);
}

void HSampleHSelectionSet::DeSelectAll()
{
	// TODO: Process your de-selectall
	HSelectionSet::DeSelectAll();
}


