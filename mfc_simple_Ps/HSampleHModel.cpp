// HSampleHModel.cpp : implementation of the HSampleHModel class
//

#include "StdAfx.h"

#include "hc.h"

#include "HSampleHModel.h"


HSampleHModel::HSampleHModel()
{
#ifdef PARASOLID
	PK_ERROR_code_t PK_result = PK_PARTITION_create_empty(&m_partition);
#endif // PARASOLID

}

HSampleHModel::~HSampleHModel()
{
}

void HSampleHModel::Init()
{
	HBaseModel::Init();

	// TODO: your init here
}

void HSampleHModel::Flush()
{
	// TODO: cleanup here
	

	HBaseModel::Flush();
}


HFileInputResult HSampleHModel::Read(LPCTSTR FileName)
{
	// This process is going to take sometime, convey that to the user
	CWaitCursor show_hourglass_cursor_through_this_function;

	HFileInputResult success = InputOK;
	wchar_t extension[64];
	wchar_t const * ext = wcsrchr(FileName, L'.');
	wchar_t const * first_ext = wcschr(FileName, L'.');

	if (!ext)
		return InputBadFileName;

	++ext; //move one past the dot;
	++first_ext; //move one past the dot;

	if (ext)
		swprintf(extension, 64, L"%ls", ext);

	if (wcsieq(extension, L"xmt_txt") || wcsieq(extension, L"x_t") || wcsieq(extension, L"x_b") || wcsieq(extension, L"xmt_bin"))
	{
#ifdef PARASOLID
		HP_Set_Hash_Level(0);
	
		HC_Open_Segment_By_Key(m_ModelKey);
		
		HC_Set_Heuristics("polygon handedness = left");

		PK_PART_receive_o_t options;
		PK_ERROR_code_t		PK_result;

		PK_PART_receive_o_m(options);

		if (wcsieq(extension, L"x_b") || wcsieq(extension, L"xmt_bin"))
			options.transmit_format = PK_transmit_format_neutral_c;
		else
			options.transmit_format = PK_transmit_format_text_c;

		PK_result = HP_Read_Xmt_File(FileName, GetPartition(), 0, 0, &options, true, "custom");	// read an XMT file
		if (PK_result == PK_ERROR_schema_access_error)
		{
			TCHAR str[MVO_BUFFER_SIZE];
			_stprintf(str, _T("Could not find schema files at specified path."));
			AfxMessageBox(str, MB_ICONINFORMATION | MB_OK);
		}
		else if (PK_result != PK_ERROR_no_errors)
			AfxMessageBox(_T("Failed to read Parasolid file. HP_Read_Xmt_File returned an error"), MB_ICONINFORMATION | MB_OK);

		HC_Close_Segment();
#endif // PARASOLID
	}

	return success;
}

HFileOutputResult HSampleHModel::WriteWithOptions(const wchar_t * FileName)
{
	HFileInputResult success = OutputOK;
#ifdef PARASOLID
	int						n_bodies;
	PK_BODY_t				*bodies;
	PK_PART_transmit_o_t	options;
	PK_ERROR_code_t			PK_result;

	PK_PART_transmit_o_m(options);

	options.transmit_format = PK_transmit_format_text_c;

	PK_PARTITION_ask_bodies(m_partition, &n_bodies, &bodies);
	PK_ASSEMBLY_t *assemblies;
	int n_assemblies;
	PK_PARTITION_ask_assemblies(m_partition, &n_assemblies, &assemblies);
	PK_PART_t *parts = new PK_PART_t[n_assemblies + n_bodies];
	int n_parts = 0;
	int i;
	for (i = 0; i < n_bodies; i++)
	{
		int numref;
		PK_PART_ask_ref_instances(bodies[i], &numref, 0);
		if (!numref)
			parts[n_parts++] = bodies[i];
	}

	for (i = 0; i < n_assemblies; i++)
	{
		int numref;
		PK_PART_ask_ref_instances(assemblies[i], &numref, 0);
		if (!numref)
			parts[n_parts++] = assemblies[i];
	}

	PK_PARTITION_set_current(m_partition);

	PK_result = HP_Write_Xmt_File((FileName), n_parts, parts, &options);	// write an XMT file

	if (PK_result < 0) success = OutputFail;

	if (n_bodies > 0)
		PK_MEMORY_free(bodies);
	if (n_assemblies > 0)
		PK_MEMORY_free(assemblies);
	delete parts;
#endif // PARASOLID

	return success;
}

void HSampleHModel::CreateBlock(const double in_dX, const double in_dY, const double in_dZ, const double in_dOX, const double in_dOY, const double in_dOZ)
{
	double dX = in_dX / m_dUnit;
	double dY = in_dY / m_dUnit;
	double dZ = in_dZ / m_dUnit;
	double dOX = in_dOX / m_dUnit;
	double dOY = in_dOY / m_dUnit;
	double dOZ = in_dOZ / m_dUnit;

#ifdef PARASOLID
	PK_PARTITION_set_current(m_partition);
	
	PK_AXIS2_sf_s basis_set;
	basis_set.location.coord[0] = dOX;
	basis_set.location.coord[1] = dOY;
	basis_set.location.coord[2] = dOZ;
	basis_set.axis.coord[0] = 0.;
	basis_set.axis.coord[1] = 0.;
	basis_set.axis.coord[2] = 1.;
	basis_set.ref_direction.coord[0] = 1.;
	basis_set.ref_direction.coord[1] = 0.;
	basis_set.ref_direction.coord[2] = 0.;
	PK_BODY_t solid_body = PK_ENTITY_null;
	PK_ERROR_code_t PK_result = PK_BODY_create_solid_block(dX, dY, dZ, &basis_set, &solid_body);

	HC_Open_Segment_By_Key(m_ModelKey);
	HP_Render_Entity(solid_body);
	HC_Close_Segment();

#endif // PARASOLID
}


#ifdef PARASOLID
void HSampleHModel::DeleteParasolidEntity(PK_ENTITY_t entity)
{
	HP_Delete_Entity_Geometry(1, &entity);
	PK_ENTITY_delete(1, &entity);
}

void HSampleHModel::BlendEdge(PK_EDGE_t edge, double blendR)
{
	PK_PARTITION_set_current(m_partition);

	double dR = blendR / m_dUnit;

	int n_faces = 0;
	PK_FACE_t* faces = NULL;
	PK_ERROR_code_t error_code;

	error_code = PK_EDGE_ask_faces(edge, &n_faces, &faces);

	PK_BODY_t body = NULL;
	error_code = PK_FACE_ask_body(faces[0], &body);

	PK_EDGE_t edges[] = { edge };

	PK_EDGE_set_blend_constant_o_t options;
	PK_EDGE_set_blend_constant_o_m(options);

	options.properties.propagate = PK_blend_propagate_yes_c;
	options.properties.ov_cliff_end = PK_blend_ov_cliff_end_yes_c;

	int n_blend_edges = 0;
	PK_EDGE_t* blend_edges = NULL;

	error_code = PK_EDGE_set_blend_constant(1, edges, dR, &options, &n_blend_edges, &blend_edges);

	PK_BODY_t partr = PK_ENTITY_null;
	PK_EDGE_ask_body(blend_edges[0], &partr);

	PK_BODY_fix_blends_o_t optionsFix;
	PK_BODY_fix_blends_o_m(optionsFix);

	int n_blends = 0;
	PK_FACE_t* blends = 0;
	PK_FACE_array_t* unders = 0;
	int* topols = 0;
	PK_blend_fault_t fault;
	PK_EDGE_t fault_edge = PK_ENTITY_null;
	PK_ENTITY_t fault_topol = PK_ENTITY_null;

	// perform the Blend operation
	error_code = PK_BODY_fix_blends(partr, &optionsFix, &n_blends, &blends, &unders, &topols, &fault, &fault_edge, &fault_topol);

	if (error_code != 0)
	{
		wchar_t error_message[256];
		_swprintf(error_message, _T("failed during Blend operation \n Error Code : %d"), error_code);
		MessageBox(NULL, error_message, 0, MB_OK);
	}

	PK_MEMORY_free(faces);
	PK_MEMORY_free(blend_edges);
	PK_MEMORY_free(blends);
	PK_MEMORY_free(unders);

	HP_Update_Entity(body);
}

bool HSampleHModel::Boolean(PK_boolean_function_t operation, PK_BODY_t target, int number_of_tools, PK_BODY_t *tools)
{
	PK_ERROR_code_t error_code;
	PK_BODY_boolean_o_t options;
	PK_TOPOL_track_r_t tracking;
	PK_boolean_r_t results;
	PK_PARTITION_t partition;

	HP_Delete_Entity_Geometry(1, &target);
	HP_Delete_Entity_Geometry(number_of_tools, tools);

	//assert(target);
	//assert(number_of_tools > 0);
	PK_BODY_boolean_o_m(options);
	options.function = operation;
	//options.target_material_side = PK_boolean_material_outside_c;

	PK_ENTITY_ask_partition(target, &partition);

	for (int i = 0; i < number_of_tools; i++)
		PK_BODY_change_partition(tools[i], partition);

	// perform the Boolean operation
	error_code = PK_BODY_boolean_2(target, number_of_tools, tools, &options, &tracking, &results);

	int n_attribs = 0;
	PK_ATTRIB_t* attribs = NULL;
	PK_PART_ask_all_attribs(target, 0, &n_attribs, &attribs);

	if (error_code != 0)
	{
		wchar_t error_message[256];
		_swprintf(error_message, _T("failed during Boolean operation \n Error Code : %d"), error_code);
		MessageBox(NULL, error_message, 0, MB_OK);
		return 1;
	}

	// render bodies
	HC_Open_Segment_By_Key(m_ModelKey);
	HP_Render_Entities(results.n_bodies, results.bodies);
	HC_Close_Segment();

	return 0;
}
#endif // PARASOLID

