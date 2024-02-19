#include "PsBooleanOperator.h"
#include "HBaseView.h"
#include "HSelectionSet.h"
#include "HEventManager.h"
#include "vlist.h"

PsBooleanOperator::PsBooleanOperator(HBaseView* view, const char* processType, int DoRepeat, int DoCapture)
	: HOpSelectAperture(view, DoRepeat, DoCapture)
	, m_pView(view)
	, m_pCProcessType(processType)
{
	GetView()->SetDynamicHighlighting(true);
	GetView()->SetViewSelectionLevel(HViewSelectionLevel::HSelectionLevelSegment);

	HSelectionSet *highlightSelectionSet = GetView()->GetHighlightSelection();
	highlightSelectionSet->SetSelectionLevel(HSelectLevel::HSelectSegment);

	m_iStage = 0;
}

PsBooleanOperator::~PsBooleanOperator()
{
	GetView()->SetDynamicHighlighting(false);

	HSelectionSet* selection = GetView()->GetSelection();
	selection->Init();
}

const char * PsBooleanOperator::GetName()
{
	return m_pCProcessType;
}

// normalize double precision vector in place 
void PsBooleanOperator::normalize_vector(double* vector)
{
	double length;
	length = sqrt(vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2]);
	vector[0] /= length;
	vector[1] /= length;
	vector[2] /= length;
}



int PsBooleanOperator::OnLButtonDown(HEventInfo &event)
{
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
	PK_boolean_function_t boolFunc;

	if (picks.n_faces)
	{
		PK_BODY_t body;
		error_code = PK_FACE_ask_body(picks.faces[0].entity, &body);
		assert(error_code == 0);

		if (0 == m_iStage)
		{
			m_target = body;
			m_iStage++;
		}
		else
		{
			int number_of_tools = 1;
			PK_BODY_t tools[1];
			tools[0] = body;

			if (!strcmp(m_pCProcessType, "UNITE"))
				boolFunc = PK_boolean_unite_c;
			else if (!strcmp(m_pCProcessType, "SUBTRACT"))
				boolFunc = PK_boolean_subtract_c;
			else if (!strcmp(m_pCProcessType, "INTERSECT"))
				boolFunc = PK_boolean_intersect_c;

			((HSampleHModel*)GetView()->GetModel())->Boolean(boolFunc, m_target, number_of_tools, tools);

			m_iStage = 0;
		}


	}
#endif // PARASOLID
	return (HOP_OK);
}
