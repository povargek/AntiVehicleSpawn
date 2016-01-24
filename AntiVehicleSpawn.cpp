#define HAVE_STDINT_H

#include "SDK/amx/amx.h"
#include "SDK/plugincommon.h"
#include "BitStream.h"
#include "Structs.h"

#if defined __LINUX__
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <ctype.h>
#else
#include <Windows.h>
#endif


#define PLUGIN_NAME	"AntiVehicleSpawn"
#define PLUGIN_VERSION 1.0
#define _MAKE_STRING(x) #x
#define MAKE_STRING(x)  _MAKE_STRING(x) 

typedef void (*logprintf_t)(char* format, ...);

logprintf_t logprintf;
extern void *pAMXFunctions;


RakServer *pRakServer = NULL;
CNetGame *pNetGame = NULL;

AMX* pGameModeAmx = NULL;

bool IsPlayerConnectedEx(int playerid)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
		return false;

	if (!pNetGame->pPlayerPool->bIsPlayerConnectedEx[playerid])
		return false;

	return pNetGame->pPlayerPool->pPlayer[playerid] != NULL;
}

bool IsValidVehicleEx(int vehicleid)
{
	if (vehicleid < 0 || vehicleid >= MAX_VEHICLES)
		return false;

	if (!pNetGame->pVehiclePool->bVehicleSlotState[vehicleid])
		return false;

	return pNetGame->pVehiclePool->pVehicle[vehicleid] != NULL;
}

void OnVehWrecked(RPCParameters *rpcParams)
{
	if (pRakServer && pGameModeAmx)
	{
		int iPlayer;
		USHORT usVehicle;

		iPlayer = pRakServer->GetIndexFromPlayerID(rpcParams->sender);
		
		if (!IsPlayerConnectedEx(iPlayer)) return;

		RakNet::BitStream bsData(rpcParams->input, rpcParams->numberOfBitsOfData / 8, false);

		bsData.Read(usVehicle);

		if (!IsValidVehicleEx(usVehicle)) return;

		CPlayerPool *pPlayerPool = pNetGame->pPlayerPool;
		CVehicle *pVehicle = pNetGame->pVehiclePool->pVehicle[usVehicle];

		if (!pVehicle) return;

		if (!pPlayerPool->pPlayer[iPlayer]->byteVehicleStreamedIn[pVehicle->wVehicleID]) return;

		int idx;
		cell ret = 0;

		if (!amx_FindPublic(pGameModeAmx, "OnVehicleRequestDeath", &idx))
		{
			amx_Push(pGameModeAmx, iPlayer);
			amx_Push(pGameModeAmx, usVehicle);
			amx_Exec(pGameModeAmx, &ret, idx);

			if ((cell)ret)
			{
				pVehicle->bDead = true;
				pVehicle->bDeathNotification = 0;
				pVehicle->wKillerID = iPlayer;
			}

			return;
		}
		else
		{
			pVehicle->bDead = true;
			pVehicle->bDeathNotification = 0;
			pVehicle->wKillerID = iPlayer;
		}

		return;

	}
}

PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports() 
{
	return SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES;
}

int RPC_VehicleDestroyed = 136;
void **ppPluginData;

PLUGIN_EXPORT bool PLUGIN_CALL Load( void **ppData ) 
{
	logprintf = (logprintf_t)ppData[PLUGIN_DATA_LOGPRINTF];
	pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];

	ppPluginData = ppData;


	logprintf("--------------------------\nAntiVehicleSpawn plugin by povargek for SAMP 0.3.7 R2-1 loaded\n--------------------------");
	return true;
}

PLUGIN_EXPORT void PLUGIN_CALL Unload()
{
	logprintf("--------------------------\nAntiVehicleSpawn plugin by povargek SAMP 0.3.7 R2-1 unloaded\n--------------------------");
}

PLUGIN_EXPORT int PLUGIN_CALL AmxLoad(AMX *amx) 
{	
	static bool bFirstLoad = false;
	if (bFirstLoad == false)
	{
		int(*pfn_GetNetGame)(void) = (int(*)(void))ppPluginData[PLUGIN_DATA_NETGAME];
		pNetGame = (CNetGame*)pfn_GetNetGame();

		int(*pfn_GetRakServer)(void) = (int(*)(void))ppPluginData[PLUGIN_DATA_RAKSERVER];
		pRakServer = (RakServer*)pfn_GetRakServer();

		pRakServer->UnregisterAsRemoteProcedureCall(&RPC_VehicleDestroyed);
		pRakServer->RegisterAsRemoteProcedureCall(&RPC_VehicleDestroyed, OnVehWrecked);

		bFirstLoad = true;
	}

	if (&pNetGame->pGameModePool->m_amx == amx)
	{
		pGameModeAmx = amx;
	}
	
	return AMX_ERR_NONE;
}

PLUGIN_EXPORT int PLUGIN_CALL AmxUnload(AMX *amx) 
{
	return AMX_ERR_NONE;
}

PLUGIN_EXPORT void PLUGIN_CALL ProcessTick()
{
}
