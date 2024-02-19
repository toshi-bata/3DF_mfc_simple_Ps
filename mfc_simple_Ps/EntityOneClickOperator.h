#pragma once
#include "HOpSelectAperture.h"
#include "HSampleHModel.h"

class HBaseOperator;

class EntityOneClickOperator :
	public HOpSelectAperture
{
public:
	EntityOneClickOperator(HBaseView* view, const char* entityType, const char* processType, int DoRepeat = 0, int DoCapture = 1);
	~EntityOneClickOperator();

	const char * GetName();

	virtual int OnLButtonDown(HEventInfo &event);

private:
	void normalize_vector(double* vector);
#ifdef PARASOLID
	void DeleteBody(PK_BODY_t body);
#endif // PARASOLID

	const char* m_pCEntityType;
	const char* m_pCProcessType;

};

