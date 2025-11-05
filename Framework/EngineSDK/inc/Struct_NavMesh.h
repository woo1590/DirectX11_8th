#pragma once

namespace Engine
{
	typedef struct tagNavCellData
	{
		_uint index{};
		_float3 points[3];  /*world position*/
		_float3 lines[3];
		_uint pointIndices[3];
		_int neighbors[3]{ -1,-1,-1 };
		_int linkedCells[3]{ -1,-1,-1 };
	}NAVCELL_DATA;

	typedef struct tagNavPortal
	{
		_uint cellA{}, cellB{};
		_float3 leftPoint{}, rightPoint{};

	}NAV_PORTAL;

	typedef struct tagAstarNode
	{
		_uint cellIndex{};
		_float fCost = FLT_MAX;
	}ASTAR_NODE;

	typedef struct tagAStarResult
	{
		_bool isAvailable = false;
		_float3 targetPosition;
		std::vector<_uint> cellIndices;
		std::vector<_float3> centerPoints;
		std::vector<NAV_PORTAL> portals;
	}ASTAR_RESULT;

	typedef struct tagAstarHistory
	{
		_uint parent;
		_uint portalIndex;
		_float gCost = FLT_MAX;
		_float fCost = FLT_MAX;
		_bool closed = false;
	}ASTAR_HISTORY;

	typedef struct tagNodeCompare
	{
		bool operator()(const ASTAR_NODE& a, const ASTAR_NODE& b)const { return a.fCost > b.fCost; } 
	}ASTAR_COMPARE;
}