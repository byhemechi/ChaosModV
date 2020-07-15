#include <stdafx.h>

static Vector3 DegToRadian(const Vector3& angles)
{
	Vector3 vec3;
	vec3.x = angles.x * 0.0174532925199433F;
	vec3.y = angles.y * 0.0174532925199433F;
	vec3.z = angles.z * 0.0174532925199433F;

	return vec3;
}

static Vector3 GetCoordsFromGameplayCam(float distance)
{
	Vector3 rot = DegToRadian(GET_GAMEPLAY_CAM_ROT(2));
	Vector3 coords = GET_GAMEPLAY_CAM_COORD();

	rot.y = distance * cos(rot.x);
	coords.x = coords.x + rot.y * std::sin(rot.z * -1.0f);
	coords.y = coords.y + rot.y * std::cos(rot.z * -1.0f);
	coords.z = coords.z + distance * sin(rot.x);

	return coords;
}

static void OnTick()
{
	static const Hash catHash = GET_HASH_KEY("a_c_cat_01");
	LoadModel(catHash);

	for (Ped ped : GetAllPeds())
	{
		if (IS_PED_SHOOTING(ped))
		{
			Vector3 spawnPos;
			Vector3 spawnRot;

			if (IS_PED_A_PLAYER(ped))
			{
				Vector3 camCoords = GET_GAMEPLAY_CAM_COORD();
				Vector3 pedPos = GET_ENTITY_COORDS(ped, false);

				float distCamToPed = GET_DISTANCE_BETWEEN_COORDS(pedPos.x, pedPos.y, pedPos.z, camCoords.x, camCoords.y, camCoords.z, true);

				spawnPos = GetCoordsFromGameplayCam(distCamToPed + .5f);
				spawnRot = GET_GAMEPLAY_CAM_ROT(2);
			}
			else
			{
				spawnPos = GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(ped, .0f, 1.f, .0f);
				spawnRot = GET_ENTITY_ROTATION(ped, 2);
			}

			Ped cat = CREATE_PED(28, catHash, spawnPos.x, spawnPos.y, spawnPos.z, .0f, true, false);
			SET_ENTITY_ROTATION(cat, spawnRot.x, spawnRot.y, spawnRot.z, 2, true);

			SET_PED_TO_RAGDOLL(cat, 3000, 3000, 0, true, true, false);

			APPLY_FORCE_TO_ENTITY_CENTER_OF_MASS(cat, 1, .0f, 300.f, 0.f, false, true, true, false);

			SET_PED_AS_NO_LONGER_NEEDED(&cat);
		}
	}

	SET_MODEL_AS_NO_LONGER_NEEDED(catHash);
}

static RegisterEffect registerEffect(EFFECT_PEDS_CAT_GUNS, nullptr, nullptr, OnTick);