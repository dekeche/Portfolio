#include "Delaunay.h"
#include <math.h>


namespace Del
{
	float quatCross(float a, float b, float c)
	{
		float p = (a + b + c) * (a + b - c) * (a - b + c) * (-a + b + c);
		return sqrt(p);
	};

	float crossProduct(Point p1, Point p2, Point p3)
	{
		float x1 = p2.x - p1.x;
		float x2 = p3.x - p2.x;

		float y1 = p2.y - p1.y;
		float y2 = p3.y - p2.y;

		return (x1*y2) - (y1 * x2);
	};
	float isFlatAngle(Point p1, Point p2, Point p3)
	{
		return crossProduct(p1, p2, p3);
	}

	bool isInCircle(Point p1, Point C1, float r)
	{
		float dist = C1.dist2(p1);
		float r2 = (r*r);
		return dist <= r2;
	}

	bool Point::operator==(Point &rhs)
	{
		return (x == rhs.x) && (y == rhs.y);
	};
	Point Point::operator+(Point &rhs)
	{
		Point p;
		p.x = x + rhs.x;
		p.y = y + rhs.y;
		return p;
	};
	Point operator/(Point &lhs, int &rhs)
	{
		Point p;
		p.x = lhs.x / rhs;
		p.y = lhs.y / rhs;
		return p;
	};
	float Point::dist2(Point &rhs)
	{
		float xdist = x - rhs.x;
		float ydist = y - rhs.y;
		float dist = xdist*xdist + ydist*ydist;
		return dist;
	};
	float Point::dist(Point &rhs)
	{
		return sqrt(dist2(rhs));
	};
	Delaunay::Edge::Edge(Point p1, Point p2)
	{
		mP1 = p1;
		mP2 = p2;
	};
	bool Delaunay::Edge::operator==(Edge &rhs)
	{
		return ((mP1 == rhs.mP1) && (mP2 == rhs.mP2)) ||
			((mP2 == rhs.mP1) && (mP1 == rhs.mP2));
	};
	float Delaunay::Edge::length()
	{
		return mP1.dist(mP2);
	};
	Point Delaunay::Edge::midpoint()
	{
		Point p;
		p.x = (mP1.x + (mP2.x - mP1.x)) / 2;
		p.y = (mP1.y + (mP2.y - mP1.y)) / 2;

		return p;
	};

	Delaunay::Triangle::Triangle(Point p1, Point p2, Point p3)
	{
		mP1 = p1;
		mP2 = p2;
		mP3 = p3;
	};
	bool Delaunay::Triangle::isCQ()
	{
		return (crossProduct(mP1, mP2, mP3) < 0);
	};
	bool Delaunay::Triangle::isCCW()
	{
		return (crossProduct(mP1, mP2, mP3) > 0);
	};
	float Delaunay::Triangle::getSideLength1()
	{
		return Edge(mP1, mP2).length();
	};
	float Delaunay::Triangle::getSideLength2()
	{
		return Edge(mP2, mP3).length();
	};
	float Delaunay::Triangle::getSideLength3()
	{
		return Edge(mP1, mP3).length();
	};
	Delaunay::Edge Delaunay::Triangle::getSide1()
	{
		return Edge(mP1, mP2);
	};
	Delaunay::Edge Delaunay::Triangle::getSide2()
	{
		return Edge(mP2, mP3);
	};
	Delaunay::Edge Delaunay::Triangle::getSide3()
	{
		return Edge(mP1, mP3);
	};

	Point Delaunay::Triangle::getCenter()
	{
		Point p = (mP1 + mP2 + mP3);
		p.x = p.x / 3;
		p.y = p.y / 3;
		return p;
	};
	Point Delaunay::Triangle::getCircumCenter()
	{
		float D = (mP1.x * (mP2.y - mP3.y) +
			mP2.x * (mP3.y - mP1.y) +
			mP3.x * (mP1.y - mP2.y)) * 2;
		float x = ((mP1.x * mP1.x + mP1.y * mP1.y) * (mP2.y - mP3.y)+
			(mP2.x * mP2.x + mP2.y * mP2.y) * (mP3.y - mP1.y) +
			(mP3.x * mP3.x + mP3.y * mP3.y) * (mP1.y - mP2.y));
		float y = ((mP1.x * mP1.x + mP1.y * mP1.y) * (mP3.x - mP2.x) +
			(mP2.x * mP2.x + mP2.y * mP2.y) * (mP1.x - mP3.x) +
			(mP3.x * mP3.x + mP3.y * mP3.y) * (mP2.x - mP1.x));
		Point P;
		P.x = x / D;
		P.y = y / D;
		return P;
	};
	float Delaunay::Triangle::getCircumRadius()
	{
		float a = getSideLength1();
		float b = getSideLength2();
		float c = getSideLength3();

		return ((a*b*c) / quatCross(a, b, c));
	};
	float Delaunay::Triangle::getArea()
	{
		float a = getSideLength1();
		float b = getSideLength2();
		float c = getSideLength3();

		return (quatCross(a, b, c) / 4);
	};
	bool Delaunay::Triangle::inCircumCircle(Point p)
	{
		return isInCircle(p, getCircumCenter(), getCircumRadius());
	};
	bool Delaunay::Triangle::IDCheck(int id)
	{
		return mP1.id > id || mP2.id > id || mP3.id > id;
	};

	Delaunay::Delaunay()
	{
	};
	void Delaunay::AddSetup(std::vector<Point>* list) 
	{
		mList = list;
	};

	void Delaunay::Run() 
	{
		int maxID;
		int size = mList->size();
		maxID = size-1;
		mResult.resize(size);
		for (int i = 0; i < size; i++)
		{
			mResult[i].resize(size);
			for (int j = 0; j < size; j++)
			{
				mResult[i][j] = 0;
			}
		}

		if (size <= 3)
		{
			for (int i = 0; i < size; i++)
			{
				for (int k = 0; k < size; k++)
				{
					if (k != i)
					{
						mResult[i][k] = 1;
					}
					else
					{
						mResult[i][k] = 0;
					}
				}
			}
			return;
		}

		int trmax = size * 4;
		float minX, minY, maxX, maxY;
		minX = maxX = (*mList)[0].x;
		minY = maxY = (*mList)[0].y;

		size = mList->size();
		for (int i = 0; i < size; i++)
		{
			if (minX > (*mList)[i].x) minX = (*mList)[i].x;
			if (maxX < (*mList)[i].x) maxX = (*mList)[i].x;
			if (minY > (*mList)[i].y) minY = (*mList)[i].y;
			if (maxY < (*mList)[i].y) maxY = (*mList)[i].y;
		}

		float dx = (maxX - minX);
		float dy = (maxY - minY);
		float deltaMax = dx < dy ? dy : dx;
		float midX = (minX + maxX)*0.5f;
		float midY = (minY + maxY)*0.5f;

		Point p1,p2,p3;
		p1.x = midX - 2 * deltaMax;
		p1.y = midY - deltaMax;
		p2.x = midX;
		p2.y = midY + 2 * deltaMax;
		p3.x = midX + 2 * deltaMax;
		p3.y = midY - deltaMax;
		p1.id = maxID + 1;
		p2.id = maxID + 2;
		p3.id = maxID + 3;

		mList->push_back(p1);
		mList->push_back(p2);
		mList->push_back(p3);

		for (int i = 0; i < size; i++)
		{
			float y = (*mList)[i].y;
			float x = (*mList)[i].x;
			if (minY > y) minY = y;
			else if (maxY < y) maxY = y;
			if (minX > x) minX = x;
			else if (maxX < x) maxX = x;
		}

		std::vector<Triangle> triangles;

		Triangle temp(p1, p2, p3);
		triangles.push_back(temp);
		
		for (int i = 0; i < size; i++)
		{
			Point target = (*mList)[i];
			std::vector<Edge> edges;
			int numTri = triangles.size();

			for (int j = numTri - 1; j >= 0; j--)
			{
				if (triangles[j].inCircumCircle(target))
				{
					edges.push_back(triangles[j].getSide1());
					edges.push_back(triangles[j].getSide2());
					edges.push_back(triangles[j].getSide3());
					triangles.erase(triangles.begin() + j);
				}
			}
			int numEdge = edges.size();
			for (int j = 0; j < numEdge-1; j++)
			{
				bool check = false;
				for (int k = j + 1; k < numEdge; k++)
				{
					if (edges[j] == edges[k])
					{
						edges.erase(edges.begin() + k);
						k--;
						check = true;
						numEdge--;
					}
				}
				if (check)
				{
					edges.erase(edges.begin() + j);
					j--;
					numEdge--;
				}
			}
			numEdge = edges.size();
			for (int j = 0; j < numEdge; j++)
			{
				triangles.push_back(Triangle(edges[j].mP1,
					edges[j].mP2,
					target));
			}
		}

		size = triangles.size();
		for (int i = size - 1; i >= 0; i--)
		{
			if (triangles[i].IDCheck(maxID))
			{
				triangles.erase(triangles.begin() + i);
			}
		}
		size = triangles.size();
		for (int i = 0; i < size; i++)
		{
			Edge side1 = triangles[i].getSide1();
			Edge side2 = triangles[i].getSide2();
			Edge side3 = triangles[i].getSide3();
			mResult[side1.mP1.id][side1.mP2.id] = (int)side1.length();
			mResult[side1.mP2.id][side1.mP1.id] = (int)side1.length();
			mResult[side2.mP1.id][side2.mP2.id] = (int)side2.length();
			mResult[side2.mP2.id][side2.mP1.id] = (int)side2.length();
			mResult[side3.mP1.id][side3.mP2.id] = (int)side3.length();
			mResult[side3.mP2.id][side3.mP1.id] = (int)side3.length();
		}
		
	};

	std::vector<std::vector<int>>* Delaunay::GetValues() 
	{
		return &mResult;
	};
}