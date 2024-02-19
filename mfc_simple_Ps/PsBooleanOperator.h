#pragma once
#include "HOpSelectAperture.h"
#include "HSampleHModel.h"

class PsBooleanOperator :
	public HOpSelectAperture
{
public:
	PsBooleanOperator(HBaseView* view, const char* processType, int DoRepeat = 0, int DoCapture = 1);
	~PsBooleanOperator();
	const char * GetName();
	virtual int OnLButtonDown(HEventInfo &event);

private:
	void normalize_vector(double* vector);

	HBaseView* m_pView;
	const char* m_pCProcessType;
	int m_iStage;
	PK_BODY_t m_target;


};

