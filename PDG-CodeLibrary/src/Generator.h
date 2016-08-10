#pragma once

#include <vector>
#include <Box2D\Box2D.h>

typedef std::vector<std::vector<int>> Matrix;

/*
** Tiny: 1x1 - 2x2
** Small: 3x3 - 5x5
** Medium: 6x6 - 8x8
** Large: 9x9 - 11x11
** XLarge: 12x12 - 14x14
*/
struct Point
{
	float x;
	float y;
};
struct RoomSizeRange
{
	int Low;
	int High;
};

struct Room
{
	b2Body* mpSelf;
	int hy;
	int wx;
};

struct Region
{
	int xStart;
	int xEnd;
	int yStart;
	int yEnd;
};
struct Connection
{
	Region* r1;
	Region* r2;
};

struct Settings
{
	RoomSizeRange SizeRange[5];

	int TargetSize;//what room size is the main room () dead ends are default size tiny(0)
	float MapSize; // how large is the map, basically consider it as a multiple of the number of rooms.
	int targetRooms; // number of main rooms
	int targetDeadEnds; // number of dead rooms
	int CorridorSize;

	int Fusion;
};

class Generator
{
public:
	Generator();

	void LoadSettings(Settings seed);

	Settings GetSettings();

	void RunCreation();
	
	void ReRun();

	bool GenerateRooms();

	void PhysicsRooms();

	bool ConnectRooms();

	bool PruneRooms();

	bool SetupDungeon();

	bool CreateCorridors();

	void ConnectDeadEnds();

	Matrix Result();

	std::vector<Room> GetRooms()
	{
		return mRooms;
	};
	std::vector<Room> GetBaseRooms()
	{
		return mBaseRooms;
	};
	std::vector<Room> GetDeadEnds()
	{
		return mDeadEnd;
	};
	std::vector<Connection> GetConnections()
	{
		return mConnections;
	};
	std::vector<Connection> GetPruned()
	{
		return mCPrune;
	};

	Matrix* getCurrentDungeon()
	{
		return &CurrentDungeon;
	}

private:

	
	bool MoveEndRegion(Region* DeadEnd);
	void DrawConnectionEnd(Region* DeadEnd);

	void CreateCorridorXY(Connection* c);
	void CreateCorridorYX(Connection* c);
	void CreateCorridorY(Connection* c, bool toLeft);
	void CreateCorridorX(Connection* c, bool toUp);


	//left/right
	void DrawCorridorX(int dir, int length, int xStart, int yStart, int xEnd);
	void DrawCorridorY(int dir, int length, int xStart, int yStart, int yEnd);


	Point getRandomPoint(float radius, float innerRadius);

	Room getRandomRoom(Point pos, RoomSizeRange range);
	Settings mSettings;

	std::vector<Room> mRooms;
	std::vector<Room> mBaseRooms;
	std::vector<Room> mDeadEnd;
	std::vector<Connection> mConnections;
	std::vector<Connection> mCPrune;
	Matrix Paths;



	std::vector<Region> mBaseRegion;
	std::vector<Region> mEndRegion;
	Matrix CurrentDungeon;

	b2World* mpWorld;
};