/*
	Effect by Last0xygen
*/

#include <stdafx.h>

static void OnTick()
{
	Ped player = PLAYER_PED_ID();
	std::vector<Entity> entities;

	for (Ped ped : GetAllPeds())
	{
		entities.push_back(ped);
	}

	for (Vehicle veh : GetAllVehs())
	{
		if (!IS_PED_IN_VEHICLE(player, veh, false))
		{
			entities.push_back(veh);
		}
	}

	for (Entity prop : GetAllProps()) 
	{
		entities.push_back(prop);
	}

	Vector3 playerCoord = GET_ENTITY_COORDS(player, false);
	for (Entity entity : entities)
	{
		static float startDistance = 15;
		static float maxForceDistance = 10;
		static float maxForce = 100;
		Vector3 entityCoord = GET_ENTITY_COORDS(entity, false);
		float distance = GET_DISTANCE_BETWEEN_COORDS(playerCoord.x, playerCoord.y, playerCoord.z, entityCoord.x, entityCoord.y, entityCoord.z, true);
		if (distance < startDistance) 
		{
			float forceDistance = min(max(0.f, (startDistance - distance)), maxForceDistance);
			float force = (forceDistance / maxForceDistance) * maxForce;
			APPLY_FORCE_TO_ENTITY_CENTER_OF_MASS(entity, 0, (entityCoord.x - playerCoord.x) * force, (entityCoord.y - playerCoord.y) * force, (entityCoord.z - playerCoord.z) * force, false, false, true, false);
		}
	}
}

static RegisterEffect registerEffect(EFFECT_PLAYER_FORCEFIELD, nullptr, nullptr, OnTick);
