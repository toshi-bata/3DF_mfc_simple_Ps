// HSampleHModel.h : interface of the HSampleHModel.cpp class, derived from HBaseModel
// Adds application-specific data and members for each model

#ifndef _HSampleHModel_H
#define _HSampleHModel_H
#include "stdafx.h"
#include "HBaseModel.h"

#ifdef PARASOLID
#include "parasolid_kernel.h"
#include "kernel_interface.h"
#include "hp_bridge.h"
#endif // PARASOLID

class HSampleHModel : public HBaseModel
{
private:
#ifdef PARASOLID
	PK_PARTITION_t m_partition;
#endif // PARASOLID

public:
	HSampleHModel();
	~HSampleHModel();

	virtual void Init();	
	virtual void Flush();
	HFileInputResult Read(LPCTSTR FileName);
	HFileOutputResult WriteWithOptions(const wchar_t * FileName);
	void CreateBlock(const double in_dX, const double in_dY, const double in_dZ, const double in_dOX, const double in_dOY, const double in_dOZ);

#ifdef PARASOLID
	void DeleteParasolidEntity(PK_ENTITY_t entity);
	PK_PARTITION_t GetPartition() { return m_partition; };
	//	PK_ENTITY_t	Compute_Selected_Instance();
	void BlendEdge(PK_EDGE_t edge, double blendR);
	bool Boolean(PK_boolean_function_t operation, PK_BODY_t target, int number_of_tools, PK_BODY_t *tools);
#endif // PARASOLID

private:
	double m_dUnit = 1000.0;

};

#endif




