/*
* Copyright (c) 2006-2012 Erin Catto http://www.box2d.org
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

#ifndef AddPair_H
#define AddPair_H

class AddPair : public Test
{
public:

	AddPair()
	{

		b2Vec2 gravity(0.0f, 0.0f);
		m_world->SetGravity(gravity);
		float random;
		float MapSizeMod = 5.0f;
		int targetDeadEnds = 5;
		int targetRooms = 10;
		int TargetSize = 3;
		int SizeRange[5][2];
		SizeRange[0][0] = 1;
		SizeRange[0][1] = 2;

		SizeRange[1][0] = 2;
		SizeRange[1][1] = 4;

		SizeRange[2][0] = 4;
		SizeRange[2][1] = 6;

		SizeRange[3][0] = 6;
		SizeRange[3][1] = 8;

		SizeRange[4][0] = 8;
		SizeRange[4][1] = 10;


		int baseRooms = targetDeadEnds + targetRooms;
		float mapSize = baseRooms * MapSizeMod;
		int numRooms = mapSize;

		float Rates[5];
		int Rooms[5];

		Rooms[0] = 0;
		Rooms[1] = TargetSize;
		Rates[0] = (float)targetDeadEnds / (float)numRooms;
		Rates[1] = ((float)targetRooms / (float)numRooms) + Rates[0];
		float remaining = (1.0f - Rates[1]) / 3.0f;

		for (int i = 2, j = 1; i < 5; i++, j++)
		{
			Rooms[i] = (j == TargetSize) ? ++j : j;
			Rates[i] = Rates[i - 1] + remaining;
		}

		float radius = 5.0f;
		for (int i = 0; i < numRooms; i++)
		{ 
			b2Vec2 value;
			{

				float r1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				float r2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				float r3 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

				float angle = 2 * b2_pi * r1;
				float dist = r2 + r3;
				if (dist > 1) {
					dist -= 2;
				}

				value.x = radius*dist*cos(angle);
				value.y = radius*dist*sin(angle);
			}

			//what type of room
			random = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

			for (int i = 0; i < 5; i++)
			{
				if (Rates[i] > random)
				{
					//spawn room at random, at point spawn.
					int high = SizeRange[Rooms[i]][0];
					int low = SizeRange[Rooms[i]][1];

					int x = rand() % (high - low) + low;
					int y = rand() % (high - low) + low;
					b2Body *body;
					b2BodyDef def;
					def.position.Set(value.x, value.y);
					def.type = b2_dynamicBody;
					def.fixedRotation = true;
					def.linearDamping = 5.0f;
					body = m_world->CreateBody(&def);

					b2PolygonShape RoomSize;
					RoomSize.SetAsBox(((float)x) / 2.0f, ((float)y) / 2.0f, b2Vec2(((float)x) / 2.0f, ((float)y) / 2.0f), 0);

					b2FixtureDef RoomFix;

					RoomFix.shape = &RoomSize;
					RoomFix.density = 0.0f;
					RoomFix.friction = 0.0f;

					body->CreateFixture(&RoomFix);

					i = 5;
				}
			}
		}
	}

	static Test* Create()
	{
		return new AddPair;
	}
};

#endif
