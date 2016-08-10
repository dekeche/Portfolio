#include "Generator.h"
#include "Delaunay.h"

#include <Box2D\Box2D.h>
#include <math.h>

using namespace std;

Generator::Generator()
{

	b2Vec2 gravity(0.0f, 0.0f);
	mpWorld = new b2World(gravity);

	mSettings.MapSize = 5.0f;
	mSettings.targetDeadEnds = 10;
	mSettings.targetRooms = 20;
	mSettings.TargetSize = 2;

	mSettings.SizeRange[0].Low = 5;
	mSettings.SizeRange[0].High = 10;
	mSettings.SizeRange[1].Low = 15;
	mSettings.SizeRange[1].High = 20;
	mSettings.SizeRange[2].Low = 25;
	mSettings.SizeRange[2].High = 30;
	mSettings.SizeRange[3].Low = 30;
	mSettings.SizeRange[3].High = 35;
	mSettings.SizeRange[4].Low = 35;
	mSettings.SizeRange[4].High = 40;

	mSettings.CorridorSize = 3;

	mSettings.Fusion = 10;
};

void Generator::LoadSettings(Settings seed)
{
	mSettings = seed;
};
Settings Generator::GetSettings()
{
	return mSettings;
};

void Generator::RunCreation()
{
	int iterr = 0;
	GenerateRooms();
	PhysicsRooms();
	while (!SetupDungeon())
	{
		printf("Running Physics\n");
		PhysicsRooms();
		iterr++;
		if (iterr > 10)
		{
			printf("ERROR___ERROR__TO_MANY_RUNS\n");
			return;
		}
	}
	ConnectRooms();
	PruneRooms();
	CreateCorridors();
	ConnectDeadEnds();
};

void Generator::ReRun()
{
	if (mpWorld != nullptr)
	{
		mRooms.clear();
		mBaseRooms.clear();
		mBaseRooms.clear();
		mDeadEnd.clear();
		mConnections.clear();
		mCPrune.clear();
		Paths.clear();
		mBaseRegion.clear();
		mEndRegion.clear();
		CurrentDungeon.clear();
		delete mpWorld;

		b2Vec2 gravity(0.0f, 0.0f);
		mpWorld = new b2World(gravity);
	}
	int iterr = 0;
	GenerateRooms();
	PhysicsRooms();
	while (!SetupDungeon())
	{
		PhysicsRooms();
		iterr++;
		if (iterr > 10)
		{
			printf("ERROR___ERROR__TO_MANY_RUNS\n");
			return;
		}
	}
	ConnectRooms();
	PruneRooms();
	CreateCorridors();
	ConnectDeadEnds();
};
bool Generator::GenerateRooms()
{
	float random;
	int roomType;

	int numRooms;
	Point Spawn;
	Room Created;

	int baseRooms = mSettings.targetDeadEnds + mSettings.targetRooms;
	float mapSize = baseRooms * mSettings.MapSize;
	//base Rooms * size + x * 1-size = 

	numRooms = mapSize;
	numRooms += baseRooms;

	float Rates[5];
	int Rooms[5];



	Rooms[0] = 0;
	Rooms[1] = mSettings.TargetSize;
	Rates[0] = (float)mSettings.targetDeadEnds / (float)numRooms;
	Rates[1] = ((float)mSettings.targetRooms / (float)numRooms) + Rates[0];
	float remaining = (1.0f - Rates[1]) / 3.0f;

	for (int i = 2, j = 1; i < 5; i++, j++)
	{
		Rooms[i] = (j == mSettings.TargetSize) ? ++j : j;
		Rates[i] = Rates[i - 1] + remaining;
	}


	for (int i = 0; i < numRooms; i++)
	{

		//what type of room
		random = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

		for (int i = 0; i < 5; i++)
		{
			if (Rates[i] > random)
			{
				//spawn room at random, at point spawn.
				if (i == 0)
				{
					Spawn = getRandomPoint(5.0f,5.0f);
				}
				else
				{
					Spawn = getRandomPoint(5.0f,0.0f);
				}
				Created = getRandomRoom(Spawn, mSettings.SizeRange[Rooms[i]]);
				mRooms.push_back(Created);

				if (i == 0)
				{
					mDeadEnd.push_back(Created);
				}
				else if (i == 1)
				{
					mBaseRooms.push_back(Created);
				}
				i = 5;

			}
		}
	}

	return true;
};

void Generator::PhysicsRooms()
{
	for (int i = 0; i < 900; i++)
	{
		mpWorld->Step(1.0f / 60.0f, 6, 2);
	}
};

bool Generator::ConnectRooms()
{
	int size = mBaseRooms.size();
	vector<Del::Point> list;
	for (int i = 0; i < size; i++)
	{
		Del::Point temp;
		temp.x = mBaseRooms[i].mpSelf->GetPosition().x;
		temp.y = mBaseRooms[i].mpSelf->GetPosition().y;
		temp.id = i;
		list.push_back(temp);
	}
	Del::Delaunay calc;
	calc.AddSetup(&list);
	calc.Run();
	Paths = *calc.GetValues();
	size = Paths.size();
	for (int i = 0; i < size; i++)
	{
		for (int k = 0; k < size; k++)
		{
			if (Paths[i][k] != 0 && i < k)
			{
				Connection temp;
				temp.r1 = &(mBaseRegion[i]);
				temp.r2 = &(mBaseRegion[k]);
				mConnections.push_back(temp);
			}
		}
	}
	return true;
};

bool Generator::PruneRooms()
{
	Matrix Unused = Paths;
	int size = Paths.size();
	vector<bool> inUse;//is this node already connected?
	vector<int> active;//list of connected nodes.
	int from = 0;
	int to = 0;//node values;
	int value = INT_MAX;//current value to test;
	inUse.resize(size, false);
	active.push_back(0);
	inUse[0] = true;
	while (!active.empty())
	{
		int sizeA = active.size();
		for (int i = 0; i < sizeA; i++)
		{
			bool pCon = false;
			for (int k = 0; k < size; k++)
			{
				if (!inUse[k])
				{
					int test = Paths[active[i]][k];
					if (test > 0)
					{
						pCon = true;
						if (test < value)
						{
							value = test;
							from = active[i];
							to = k;
						}
					}
				}
			}
			if (!pCon)
			{
				active.erase(active.begin() + i);
				i--;
				sizeA--;
			}
		}
		if (active.size() > 0)
		{
			inUse[to] = true;
			active.push_back(to);
			Connection temp;
			temp.r1 = &(mBaseRegion[to]);
			temp.r2 = &(mBaseRegion[from]);

			Unused[from][to] = 0;
			mCPrune.push_back(temp);

			value = INT_MAX;
			from = 0;
			to = 0;
		}
	}
	for (int i = 0; i < size; i++)
	{
		for (int k = i + 1; k < size; k++)
		{
			if (Unused[i][k] != 0)
			{
				int r = rand() % 100;
				if (r < mSettings.Fusion)
				{
					Connection temp;
					temp.r1 = &(mBaseRegion[i]);
					temp.r2 = &(mBaseRegion[k]);
 					mCPrune.push_back(temp);
				}
			}
		}
	}
	return true;
};

bool Generator::SetupDungeon()
{
	mBaseRegion.clear();
	mEndRegion.clear();
	CurrentDungeon.clear();
	int minY = 0;
	int maxY = 0;
	int minX = 0;
	int maxX = 0;
	int size = mBaseRooms.size();
	
	b2Vec2 tempVec = mBaseRooms[0].mpSelf->GetPosition();
	float yMin = tempVec.y - mBaseRooms[0].hy;
	float yMax = tempVec.y + mBaseRooms[0].hy;
	float xMin = tempVec.x - mBaseRooms[0].wx;
	float xMax = tempVec.x + mBaseRooms[0].wx;
	minY = yMin;
	maxY = yMax;
	minX = xMin;
	maxX = xMax;

	for (int i = 1; i < size; i++)
	{
		tempVec = mBaseRooms[i].mpSelf->GetPosition();
		yMin = tempVec.y - mBaseRooms[i].hy;
		yMax = tempVec.y + mBaseRooms[i].hy;
	    xMin = tempVec.x - mBaseRooms[i].wx;
		xMax = tempVec.x + mBaseRooms[i].wx;

		if (yMin < minY)
		{
			minY = yMin;
		}
		else if (yMax > maxY)
		{
			maxY = yMax;
		}
		if (xMin < minX)
		{
			minX = xMin;
		}
		else if (xMax > maxX)
		{
			maxX = xMax;
		}
	}
	size = mDeadEnd.size();
	for (int i = 0; i < size; i++)
	{
		tempVec = mDeadEnd[i].mpSelf->GetPosition();
		yMin = tempVec.y - mDeadEnd[i].hy;
		yMax = tempVec.y + mDeadEnd[i].hy;
		xMin = tempVec.x - mDeadEnd[i].wx;
		xMax = tempVec.x + mDeadEnd[i].wx;

		if (yMin < minY)
		{
			minY = yMin;
		}
		else if (yMax > maxY)
		{
			maxY = yMax;
		}
		if (xMin < minX)
		{
			minX = xMin;
		}
		else if (xMax > maxX)
		{
			maxX = xMax;
		}
	}
	int xDif = maxX - minX;
	int yDif = maxY - minY;

	CurrentDungeon.resize(xDif);
	for (int i = 0; i < xDif; i++)
	{
		CurrentDungeon[i].resize(yDif, 0);
	}

	int xStart;
	int xEnd;
	int yStart;
	int yEnd;
	size = mBaseRooms.size();
	for (int i = 0; i < size; i++)
	{
		tempVec = mBaseRooms[i].mpSelf->GetPosition();
		xStart = (int)(tempVec.x - ((float)mBaseRooms[i].wx)/2.0f) - minX;
		xEnd = (int)(tempVec.x + ((float)mBaseRooms[i].wx) / 2.0f) - minX;
		yStart = (int)(tempVec.y - ((float)mBaseRooms[i].hy) / 2.0f) - minY;
		yEnd = (int)(tempVec.y + ((float)mBaseRooms[i].hy) / 2.0f) - minY;

		Region tempReg;
		tempReg.xStart = xStart;
		tempReg.xEnd = xEnd;
		tempReg.yStart = yStart;
		tempReg.yEnd = yEnd;

		mBaseRegion.push_back(tempReg);
		for (int j = xStart; j < xEnd; j++)
		{
			for (int k = yStart; k < yEnd; k++)
			{
				if (j < 0)
				{
					printf("J to low");
				}
				else if (j > xDif)
				{
					printf("J to high");
				}

				if (k < 0)
				{
					printf("k to low");
				}
				else if (k > yDif)
				{
					printf("k to high");
				}
				if (CurrentDungeon[j][k] != 0)
				{
					printf("ODD, DATA CROSS\n");
					return false;
				}
				CurrentDungeon[j][k] = 1;
			}
		}
	}


	size = mDeadEnd.size();
	for (int i = 0; i < size; i++)
	{
		tempVec = mDeadEnd[i].mpSelf->GetPosition();
		xStart = (int)(tempVec.x - ((float)mDeadEnd[i].wx) / 2.0f) - minX;
		xEnd = (int)(tempVec.x + ((float)mDeadEnd[i].wx) / 2.0f) - minX;
		yStart = (int)(tempVec.y - ((float)mDeadEnd[i].hy) / 2.0f) - minY;
		yEnd = (int)(tempVec.y + ((float)mDeadEnd[i].hy) / 2.0f) - minY;

		Region tempReg;
		tempReg.xStart = xStart;
		tempReg.xEnd = xEnd;
		tempReg.yStart = yStart;
		tempReg.yEnd = yEnd;

		mEndRegion.push_back(tempReg);
		for (int i = xStart; i < xEnd; i++)
		{
			for (int j = yStart; j < yEnd; j++)
			{
				if (CurrentDungeon[i][j] != 0)
				{
					printf("ODD, DATA CROSS\n");
					return false;
				}
				CurrentDungeon[i][j] = 2;
			}
		}
	}

	return true;
};

bool Generator::CreateCorridors()
{
	int size = mCPrune.size();
	for (int i = 0; i < size; i++)
	{
		Connection test = mCPrune[i];

		bool toLeft = test.r1->xEnd < test.r2->xStart;
		bool toRight = test.r2->xEnd < test.r1->xStart;
		bool toUp = test.r1->yEnd < test.r2->yStart;
		bool toDown = test.r2->yEnd < test.r1->yStart;

		if ((toLeft || toRight) && (toUp || toDown))
		{
			int first = rand() % 2;
			if (first == 0)
			{
				CreateCorridorXY(&test);
			}
			else
			{
				CreateCorridorYX(&test);
			}
		}
		else if(toLeft || toRight)
		{
			CreateCorridorY(&test, toLeft);
		}
		else if (toUp || toDown)
		{
			int y = toUp ? -1 : 1;
			CreateCorridorX(&test, toUp);
		}
	}
	return true;
};

void Generator::ConnectDeadEnds()
{
	int size = mEndRegion.size();
	for (int i = 0; i < size; i++)
	{
		if (!MoveEndRegion(&(mEndRegion[i])))
		{
			DrawConnectionEnd(&(mEndRegion[i]));
		}
	}
};


bool Generator::MoveEndRegion(Region* DeadEnd)
{
	int xStart = DeadEnd->xStart - 1;
	int yStart = DeadEnd->yStart - 1;

	int xEnd = DeadEnd->xEnd + 1;
	int yEnd = DeadEnd->yEnd + 1;
	bool Left = false, Right = false, Up = false, Down = false;

	for (int i = xStart; i < xEnd; i++)
	{
		if (CurrentDungeon[i][yStart] != 0)
		{
			Down = true;
			break;
		}
	}
	for (int i = xStart; i < xEnd; i++)
	{
		if (CurrentDungeon[i][yEnd] != 0)
		{
			Up = true;
			break;
		}
	}
	for (int i = yStart; i < yEnd; i++)
	{
		if (CurrentDungeon[xStart][i] != 0)
		{
			Left = true;
			break;
		}
	}
	for (int i = yStart; i < yEnd; i++)
	{
		if (CurrentDungeon[xEnd][i] != 0)
		{
			Right = true;
			break;
		}
	}
	return Right || Left || Up || Down;
};
void Generator::DrawConnectionEnd(Region* DeadEnd)
{
	bool Left = false, Right = false, Up = false, Down = false;
	
	int xMid = (DeadEnd->xStart + DeadEnd->xEnd) /2;
	int yMid = (DeadEnd->yStart + DeadEnd->yEnd) /2;
	int xMax = CurrentDungeon.size();
	int yMax = CurrentDungeon[0].size();

	int distance = INT_MAX;
	int tempMax;

	tempMax = xMax - xMid;
	for (int i = 0; i < tempMax; i++)
	{
		if (CurrentDungeon[xMid + i][yMid] != 0 && CurrentDungeon[xMid + i][yMid] != 2)
		{
			Right = true;
			distance = i;
			i = tempMax;
		}
	}
	tempMax = xMid-1;
	for (int i = 0; i < tempMax; i++)
	{
		if (CurrentDungeon[xMid - i][yMid] != 0 && CurrentDungeon[xMid - i][yMid] != 2)
		{
			if (Right)
			{
				if (i < distance)
				{
					Right = false;
					Left = true;
					distance = i;
				}
				else if (i == distance)
					Left = true;
			}
			else
			{
				Left = true;
				distance = i;
			}
			i = tempMax;
		}
	}
	tempMax = yMax - yMid;
	for (int i = 0; i < tempMax; i++)
	{
		if (CurrentDungeon[xMid][yMid + i] != 0 && CurrentDungeon[xMid][yMid + i] != 2)
		{
			if (Right || Left)
			{
				if (i < distance)
				{
					Right = Left = false;
					Up = true;
					distance = i;
				}
				else if (i == distance)
					Up = true;
			}
			else
			{
				Up = true;
				distance = i;
			}
			i = tempMax;
		}
	}
	tempMax = yMid-1;
	for (int i = 0; i < tempMax; i++)
	{
		if (CurrentDungeon[xMid][yMid - i] != 0 && CurrentDungeon[xMid][yMid - i] != 2)
		{
			if (Up || Right || Left)
			{
				if (i < distance)
				{
					Up = Right = Left = false;
					Down = true;
					distance = i;
				}
				else if (i == distance)
					Down = true;
			}
			else
			{
				Down = true;
				distance = i;
			}
			i = tempMax;
		}
	}
	int num = (Right + Left + Up + Down);
	if (num > 1)
	{
		int random = rand() % num;
		if (Right && random != 0)
		{
			Right = false;
			random--;
		}
		if (Left && random != 0)
		{
			Left = false;
			random--;
		}
		if (Up && random != 0)
		{
			Up = false;
			random--;
		}
		if (Down && random != 0)
		{
			Down = false;
			random--;
		}
	}

	if (distance == 0)
	{
		printf("ERROR");
	}
	int Displace = (mSettings.CorridorSize - (mSettings.CorridorSize%2)) / 2;
	if (Right)
	{
		DrawCorridorX(1, mSettings.CorridorSize, xMid, yMid - Displace, xMid + distance);
	}
	else if (Left)
	{
		DrawCorridorX(-1, mSettings.CorridorSize, xMid, yMid - Displace, xMid - distance);
	}
	else if (Up)
	{
		DrawCorridorY(1, mSettings.CorridorSize, xMid - Displace, yMid, yMid + distance);
	}
	else if(Down)
	{
		DrawCorridorY(-1, mSettings.CorridorSize, xMid - Displace, yMid, yMid - distance);
	}
};


Matrix Generator::Result()
{
	Matrix temp;
	return temp;
};
Point Generator::getRandomPoint(float radius, float innerRadius)
{
	float r1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float r2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float r3 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

	float angle = 2 * b2_pi * r1;
	float dist = r2 + r3;
	if (dist > 1) {
		dist -= 2;
	}

	Point value;
	value.x = radius*dist*cos(angle);
	value.y = radius*dist*sin(angle);

	if (innerRadius > 0)
	{
		if (value.x > 0)
		{
			value.x += innerRadius;
		}
		else if (value.x < 0)
		{
			value.x -= innerRadius;
		}
		else
		{
			int i = rand() % 2;
			if (i > 0)
			{
				value.x += innerRadius;
			}
			else
			{
				value.x -= innerRadius;
			}
		}

		if (value.y > 0)
		{
			value.y += innerRadius;
		}
		else if (value.y < 0)
		{
			value.y -= innerRadius;
		}
		else
		{
			int i = rand() % 2;
			if (i > 0)
			{
				value.y += innerRadius;
			}
			else
			{
				value.y -= innerRadius;
			}
		}
	}

	return value;
};

Room Generator::getRandomRoom(Point pos, RoomSizeRange range)
{
	Room value;
	int x = rand() % (range.High - range.Low) + range.Low;
	int y = rand() % (range.High - range.Low) + range.Low;
	b2Body *body;
	b2BodyDef def;
	def.position.Set(pos.x, pos.y);
	def.type = b2_dynamicBody;
	def.fixedRotation = true;
	def.linearDamping = 5.0f;
	body = mpWorld->CreateBody(&def);

	b2PolygonShape RoomSize;
	RoomSize.SetAsBox(((float)x + 2.0f) / 2.0f, ((float)y + 2.0f) / 2.0f);//,b2Vec2(((float)x)/2.0f, ((float)y) /2.0f),0);

	b2FixtureDef RoomFix;

	RoomFix.shape = &RoomSize;
	RoomFix.density = 0.0f;
	RoomFix.friction = 0.0f;

	body->CreateFixture(&RoomFix);

	value.mpSelf = body;
	value.hy = y;
	value.wx = x;

	return value;
};

void Generator::CreateCorridorXY(Connection* c)
{
	int avalible;

	int xDir;
	int yDir;
	int xStart;
	int yStart;
	int xEnd;
	int yEnd;
	//draw from r1, x;
	if (c->r1->xEnd < c->r2->xStart)
	{
		xStart = c->r1->xEnd;
		xDir = 1;
	}
	else if (c->r2->xEnd < c->r1->xStart)
	{
		xStart = c->r1->xStart;
		xDir = -1;
	}
	avalible = c->r1->yEnd - c->r1->yStart;
	avalible -= mSettings.CorridorSize;

	yStart = (rand() % avalible) + c->r1->yStart;

	avalible = c->r2->xEnd - c->r2->xStart;
	avalible -= mSettings.CorridorSize;

	//need to edit later.
	xEnd = (rand() % avalible) + c->r2->xStart;
	
	DrawCorridorX(xDir, mSettings.CorridorSize, xStart, yStart, xEnd);
	//draw from r2, y;
	
	yEnd = yStart;
	if (c->r2->yEnd < c->r1->yStart)
	{
		yStart = c->r2->yEnd;
		yDir = 1;
	}
	else if(c->r1->yEnd < c->r2->yStart)
	{
		yStart = c->r2->yStart;
		yDir = -1;
	}
	xStart = xEnd;

	DrawCorridorY(yDir, mSettings.CorridorSize, xStart, yStart, yEnd);
};
void Generator::CreateCorridorYX(Connection* c)
{
	int avalible;

	int xDir;
	int yDir;
	int xStart;
	int yStart;
	int xEnd;
	int yEnd;

	if (c->r1->yEnd < c->r2->yStart)
	{
		yStart = c->r1->yEnd;
		yDir = 1;
	}
	else if (c->r2->yEnd < c->r1->yStart)
	{
		yStart = c->r1->yStart;
		yDir = -1;
	}
	avalible = c->r1->xEnd - c->r1->xStart;
	avalible -= mSettings.CorridorSize;

	xStart = (rand() % avalible) + c->r1->xStart;

	avalible = c->r2->yEnd - c->r2->yStart;
	avalible -= mSettings.CorridorSize;

	//need to edit later.
	yEnd = (rand() % avalible) + c->r2->yStart;

	DrawCorridorY(yDir, mSettings.CorridorSize, xStart, yStart, yEnd);


	xEnd = xStart;
	if (c->r2->xEnd < c->r1->xStart)
	{
		xStart = c->r2->xEnd;
		xDir = 1;
	}
	else if (c->r1->xEnd < c->r2->xStart)
	{
		xStart = c->r2->xStart;
		xDir = -1;
	}
	yStart = yEnd;

	DrawCorridorX(xDir, mSettings.CorridorSize, xStart, yStart, xEnd);
};
void Generator::CreateCorridorY(Connection* c, bool toLeft)
{
	int amount;
	int yStart;
	int xStart;
	int xEnd;
	if (c->r1->yStart < c->r2->yStart)
	{
		amount = c->r1->yEnd - c->r2->yStart;
		yStart = c->r2->yStart;
	}
	else if (c->r1->yStart > c->r2->yStart)
	{
		amount = c->r2->yEnd - c->r1->yStart;
		yStart = c->r1->yStart;
	}
	else
	{
		yStart = c->r1->yStart;
		if (c->r1->yEnd < c->r2->yEnd)
		{
			amount = c->r1->yEnd - c->r1->yStart;
		}
		else
		{
			amount = c->r2->yEnd - c->r1->yStart;
		}
	}

	if (amount < mSettings.CorridorSize)
	{
		int x;
		int y;
		Region temp = *(c->r1);

		if (c->r1->yStart < c->r2->yStart)
		{
			temp.yEnd = c->r2->yStart-1;
		}
		else
		{
			temp.yStart = c->r2->yEnd+1;
		}
		Connection tempC;
		tempC.r1 = &temp;
		tempC.r2 = c->r2;
		int first = rand() % 2;
		if (first == 0)
		{
			CreateCorridorXY(&tempC);
		}
		else
		{
			CreateCorridorYX(&tempC);
		}
	}
	else
	{
		int diff;
		if (amount - mSettings.CorridorSize > 0)
			diff = rand() % (amount - mSettings.CorridorSize);
		else
			diff = 0;
		yStart += diff;
		if (!toLeft)
		{
			xStart = c->r2->xEnd;
			xEnd = c->r1->xStart;
		}
		else
		{
			xStart = c->r1->xEnd;
			xEnd = c->r2->xStart;
		}


		for (int i = 0; i < mSettings.CorridorSize; i++)
		{
			for (int k = xStart; k != xEnd; k++)
			{
				if (CurrentDungeon[k][i + yStart] == 0)
				{
					CurrentDungeon[k][i + yStart] = 3;
				}
			}
		}
	}
};
void Generator::CreateCorridorX(Connection* c, bool toUp)
{
	int amount;
	int xStart;
	int yStart;
	int yEnd;
	if (c->r1->xStart < c->r2->xStart)
	{
		amount = c->r1->xEnd - c->r2->xStart;
		xStart = c->r2->xStart;
	}
	else if(c->r1->xStart > c->r2->xStart)
	{
		amount = c->r2->xEnd - c->r1->xStart;
		xStart = c->r1->xStart;
	}
	else
	{
		xStart = c->r1->xStart;
		if (c->r1->xEnd < c->r2->xEnd)
		{
			amount = c->r1->xEnd - c->r1->xStart;
		}
		else
		{
			amount = c->r2->xEnd - c->r1->xStart;
		}
	}

	if (amount < mSettings.CorridorSize)
	{
		int x;
		int y;
		Region temp = *(c->r1);

		if (c->r1->xStart < c->r2->xStart)
		{
			temp.xEnd = c->r2->xStart - 1;
		}
		else
		{
			temp.xStart = c->r2->xEnd + 1;
		}
		Connection tempC;
		tempC.r1 = &temp;
		tempC.r2 = c->r2;
		int first = rand() % 2;
		if (first == 0)
		{
			CreateCorridorXY(&tempC);
		}
		else
		{
			CreateCorridorYX(&tempC);
		}
	}
	else
	{
		int diff;
		if (amount - mSettings.CorridorSize > 0)
			diff = rand() % (amount - mSettings.CorridorSize);
		else
			diff = 0;
		xStart += diff;
		if (!toUp)
		{
			yStart = c->r2->yEnd;
			yEnd = c->r1->yStart;
		}
		else
		{
			yStart = c->r1->yEnd;
			yEnd = c->r2->yStart;
		}

		for (int i = 0; i < mSettings.CorridorSize; i++)
		{
			for (int k = yStart; k != yEnd; k++)
			{
				if (CurrentDungeon[i + xStart][k] == 0)
				{
					CurrentDungeon[i + xStart][k] = 3;
				}
			}
		}
	}
};


void Generator::DrawCorridorX(int dir, int length, int xStart, int yStart, int xEnd)
{
	int yEnd = length + yStart;

	if (dir < 0)
	{
		//modify values?
		int temp = xStart;
		xStart = xEnd;
		xEnd = temp;
	}
	for (int i = yStart; i < yEnd; i++)
	{
		for (int k = xStart; k < xEnd+length; k++)
		{
			if (CurrentDungeon[k][i] == 0)
			{
				CurrentDungeon[k][i] = 3;
			}
		}
	}
};
void Generator::DrawCorridorY(int dir, int length, int xStart, int yStart, int yEnd)
{
	int xEnd = length + xStart;

	if (dir < 0)
	{
		//modify values?
		int temp = yStart;
		yStart = yEnd;
		yEnd = temp;
	}

	for (int i = xStart; i < xEnd; i++)
	{
		for (int k = yStart; k < yEnd; k++)
		{
			if (CurrentDungeon[i][k] == 0)
			{
				CurrentDungeon[i][k] = 3;
			}
		}
	}
};
