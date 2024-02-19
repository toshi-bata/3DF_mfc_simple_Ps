#include "EntityOneClickOperator.h"
#include "HBaseView.h"

#include "vlist.h"
#include "BlendDlg.h"
#include "HSelectionSet.h"
#include "HEventManager.h"


EntityOneClickOperator::EntityOneClickOperator(HBaseView* view, const char* entityType, const char* processType, int DoRepeat, int DoCapture)
	: HOpSelectAperture(view, DoRepeat, DoCapture)
	, m_pCEntityType(entityType)
	, m_pCProcessType(processType)
{
	if (!strcmp(m_pCEntityType, "BODY"))
	{
		GetView()->SetDynamicHighlighting(true);
		GetView()->SetViewSelectionLevel(HViewSelectionLevel::HSelectionLevelSegment);

		HSelectionSet *highlightSelectionSet = GetView()->GetHighlightSelection();
		highlightSelectionSet->SetSelectionLevel(HSelectLevel::HSelectSegment);
	}
	else if (!strcmp(m_pCEntityType, "EDGE"))
	{
		GetView()->SetDynamicHighlighting(false);
		GetView()->SetViewSelectionLevel(HViewSelectionLevel::HSelectionLevelEntity);

		HSelectionSet* selection = GetView()->GetSelection();
		selection->SetSelectionLevel(HSelectLevel::HSelectEntity);
	}

}

EntityOneClickOperator::~EntityOneClickOperator()
{
	GetView()->SetDynamicHighlighting(false);

	HSelectionSet* selection = GetView()->GetSelection();
	selection->Init();
}


// normalize double precision vector in place 
void EntityOneClickOperator::normalize_vector(double* vector)
{
	double length;
	length = sqrt(vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2]);
	vector[0] /= length;
	vector[1] /= length;
	vector[2] /= length;
}

const char * EntityOneClickOperator::GetName()
{
	return m_pCProcessType;
}

int EntityOneClickOperator::OnLButtonDown(HEventInfo &event)
{
	if (!strcmp(m_pCEntityType, "EDGE"))
	{
		HPoint  new_pos;
		int		res = 0;
		bool	need_update = false;

		HSelectionSet* selection = GetView()->GetSelection();

		new_pos = event.GetMouseWindowPos();

		// perform standard Windows explorer method of selecting and deselecting
		// using Shift and Control keys

		// if neither the Control key nor Shift key is down, deselect everything
		if (selection->GetSize() > 0 &&
			!event.Control() && !event.Shift())
		{
			selection->DeSelectAll();
			GetView()->EmitDeSelectAllMessage();
			need_update = true;
		}

		HC_Open_Segment_By_Key(GetView()->GetViewKey());
		res = HC_Compute_Selection(".",
			(selection->GetSubwindowPenetration() ? "" : "./scene/overwrite"),
			"v, selection bias = lines, related selection limit = -1, selection sorting, internal selection limit = 0", new_pos.x, new_pos.y);
		HC_Close_Segment();

		if (res > 0) {
			HandleSelection(event);
			GetView()->Update();	// update the scene to reflect the new highlight attributes
		}
	}

#ifdef PARASOLID
	SetNewPoint(event.GetMouseViewpointPos());

	HPoint view_vector[2];
	view_vector[0] = GetNewPoint();
	view_vector[1] = GetNewPoint();
	view_vector[1].z += 1.0f;
	
	HC_Open_Segment_By_Key(GetView()->GetViewKey());
	HC_Compute_Coordinates("./scene/overwrite", "viewpoint", &view_vector[0], "world", &view_vector[0]);
	HC_Compute_Coordinates("./scene/overwrite", "viewpoint", &view_vector[1], "world", &view_vector[1]);
	HC_Close_Segment();

	PK_BODY_pick_topols_o_t pick_options;
	PK_BODY_pick_topols_r_t picks;
	PK_AXIS1_sf_t picking_ray;
	PK_ERROR_t error_code;
	PK_BODY_pick_topols_o_m(pick_options);
	pick_options.method = PK_BODY_pick_radial_c;

	if (!strcmp(m_pCEntityType, "EDGE"))
	{
		HPoint point = event.GetMousePixelPos();

		double dXPixel = (double)GetView()->GetXSize();
		double dYPixel = (double)GetView()->GetYSize();
		double dPixelDist = sqrt(dXPixel * dXPixel + dYPixel * dYPixel);

		HPoint view_size[2];
		view_size[0].Set(0, 0, 0);
		view_size[1].x = dXPixel;
		view_size[1].y = dYPixel;
		view_size[1].z = 0;
		HC_Open_Segment_By_Key(GetView()->GetViewKey());
		HC_Compute_Coordinates("./scene/overwrite", "local pixels", &view_size[0], "world", &view_size[0]);
		HC_Compute_Coordinates("./scene/overwrite", "local pixels", &view_size[1], "world", &view_size[1]);
		HC_Close_Segment();
		
		double dDist = sqrt(
			(view_size[1].x - view_size[0].x) * (view_size[1].x - view_size[0].x) + 
			(view_size[1].y - view_size[0].y) * (view_size[1].y - view_size[0].y) + 
			(view_size[1].z - view_size[0].z) * (view_size[1].z - view_size[0].z));

		dDist = dDist / dPixelDist * 5;

		pick_options.max_edges = 1;
		pick_options.max_edge_dist = dDist;
	}

	int n_bodies;
	PK_BODY_t *bodies;
	error_code = PK_PARTITION_ask_bodies(((HSampleHModel*)GetView()->GetModel())->GetPartition(),
		&n_bodies, &bodies);
	assert(error_code == 0);

	VList<PK_BODY_t> body_list;
	VList<PK_TRANSF_t> transf_list;
	int i, j;
	for (i = 0; i < n_bodies; i++) {
		int n_instances;
		PK_INSTANCE_t *instances;
		error_code = PK_PART_ask_ref_instances(bodies[i], &n_instances, &instances);
		assert(error_code == 0);

		if (n_instances == 0) {
			body_list.AddLast(bodies[i]);
			transf_list.AddLast(PK_ENTITY_null);
		}
		else {
			for (j = 0; j < n_instances; j++) {
				PK_INSTANCE_sf_t instance_sf;
				error_code = PK_INSTANCE_ask(instances[j], &instance_sf);
				assert(error_code == 0);

				// we already know that instance_sf.part == bodies[i], but we need to grab the
				//   transforms
				body_list.AddLast(instance_sf.part);
				transf_list.AddLast(instance_sf.transf);
			}
		}
		PK_MEMORY_free(instances);
	}
	PK_MEMORY_free(bodies);

	n_bodies = body_list.Count();
	bodies = new PK_BODY_t[n_bodies];
	body_list.ItemsToArray(bodies);

	PK_TRANSF_t *body_transfs;
	body_transfs = new PK_TRANSF_t[n_bodies];
	transf_list.ItemsToArray(body_transfs);

	picking_ray.location.coord[0] = view_vector[0].x;
	picking_ray.location.coord[1] = view_vector[0].y;
	picking_ray.location.coord[2] = view_vector[0].z;

	// axis MUST contain a unit vector rw
	picking_ray.axis.coord[0] = view_vector[1].x - view_vector[0].x;
	picking_ray.axis.coord[1] = view_vector[1].y - view_vector[0].y;
	picking_ray.axis.coord[2] = view_vector[1].z - view_vector[0].z;

	normalize_vector(picking_ray.axis.coord);

	error_code = PK_BODY_pick_topols(n_bodies, bodies, body_transfs, &picking_ray, &pick_options, &picks);
	assert(error_code == 0);
	delete[] bodies;
	delete[] body_transfs;

	if (picks.n_faces)
	{
		if (!strcmp(m_pCProcessType, "DELETE_BODY"))
		{
			PK_BODY_t body;
			error_code = PK_FACE_ask_body(picks.faces[0].entity, &body);
			assert(error_code == 0);

			DeleteBody(body);

			GetView()->Update();
		}
		else if (!strcmp(m_pCProcessType, "BLEND") && picks.n_edges)
		{
			PK_EDGE_t edge = picks.edges[0].entity;

			BlendDlg *pDlg = new BlendDlg(10.0);
			if (pDlg->DoModal() == IDOK)
			{
				double blendR = pDlg->m_dBlendR;
				((HSampleHModel*)GetView()->GetModel())->BlendEdge(edge, blendR);

			}
		}
	}
#endif // PARASOLID

//	selection->DeSelectAll();
	GetView()->Update();

	return (HOP_OK);
}

#ifdef PARASOLID
void EntityOneClickOperator::DeleteBody(PK_BODY_t body)
{
	((HSampleHModel*)GetView()->GetModel())->DeleteParasolidEntity(body);
}
#endif // PARASOLID
