#pragma once
#include <vector>
namespace Del
{
	struct Point {
		float x;
		float y;
		int id;
		bool operator==(Point &rhs);
		Point operator+(Point &rhs);
		
		float dist2(Point &rhs);
		float dist(Point &rhs);
	};
	float quatCross(float a, float b, float c);

	float crossProduct(Point p1, Point p2, Point p3);

	float isFlatAngle(Point p1, Point p2, Point p3);

	bool isInCircle(Point p1, Point C1, float r);

	class Delaunay
	{
	public:
		Delaunay();
		void AddSetup(std::vector<Point>* list);

		void Run();

		std::vector<std::vector<int>>* GetValues();

	private:
		struct Edge
		{
			Edge(Point p1, Point p2);
			bool operator==(Edge &rhs);
			float length();
			Point midpoint();


			Point mP1;
			Point mP2;
		};
		class Triangle
		{
		public:
			Triangle(Point p1, Point p2, Point p3);
			bool isCQ();
			bool isCCW();
			float getSideLength1();
			float getSideLength2();
			float getSideLength3();

			Edge getSide1();
			Edge getSide2();
			Edge getSide3();

			Point getCenter();
			Point getCircumCenter();
			float getCircumRadius();
			float getArea();
			bool inCircumCircle(Point p);

			bool IDCheck(int id);
		private:
			Point mP1;
			Point mP2;
			Point mP3;
		};

		std::vector<Point>* mList;
		std::vector<std::vector<int>> mResult;
	};
}
