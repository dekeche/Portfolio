#include "Graphics.h"
#include <SFML\Graphics.hpp>
#include <iostream>
#include <fstream>
using namespace std;
GraphicInterface::GraphicInterface() {};
void GraphicInterface::Setup()
{
	bool done = false;
	bool donePartial = false;
	Settings mPresets = mGenerator.GetSettings();
	int inputI;
	float inputF;
	string FileName;
	string Garbage;
	fstream File;
	while (!done)
	{
		printf("CurrentPreset: \n");
		printf("RoomSize: \n");
		printf("Tiny:   %d,%d\n", mPresets.SizeRange[0].Low, mPresets.SizeRange[0].High);
		printf("Small:  %d,%d\n", mPresets.SizeRange[1].Low, mPresets.SizeRange[1].High);
		printf("Medium: %d,%d\n", mPresets.SizeRange[2].Low, mPresets.SizeRange[2].High);
		printf("Large:  %d,%d\n", mPresets.SizeRange[3].Low, mPresets.SizeRange[3].High);
		printf("Huge:   %d,%d\n", mPresets.SizeRange[4].Low, mPresets.SizeRange[4].High);
		printf("\n");

		printf("Target Room Size:      %d\n", mPresets.TargetSize);
		printf("# Target Rooms:        %d\n", mPresets.targetRooms);
		printf("# Target Dead Ends:    %d\n", mPresets.targetDeadEnds);
		printf("Corridor Size:         %d\n", mPresets.CorridorSize);
		printf("Dungeon Size Modifier: %f\n", mPresets.MapSize);
		printf("Dungeon Connectivity Rate (0-100): %d\n", mPresets.Fusion);

		printf("\n");

		printf("Chose:\n1) Room Size\n2) Target Room Size\n3) # Rooms\n4) # Dead Ends\n5) Corridor Size\n6) Size Modifier\n7) Connectivity\n8) Load File\n9) Save File\n0) Done\n");
		cin >> inputI;
		switch (inputI)
		{
		case 1:
			donePartial = false;
			while (!donePartial)
			{
				printf("RoomSize: \n");
				printf("Tiny:   %d,%d\n", mPresets.SizeRange[0].Low, mPresets.SizeRange[0].High);
				printf("Small:  %d,%d\n", mPresets.SizeRange[1].Low, mPresets.SizeRange[1].High);
				printf("Medium: %d,%d\n", mPresets.SizeRange[2].Low, mPresets.SizeRange[2].High);
				printf("Large:  %d,%d\n", mPresets.SizeRange[3].Low, mPresets.SizeRange[3].High);
				printf("Huge:   %d,%d\n", mPresets.SizeRange[4].Low, mPresets.SizeRange[4].High);
				printf("\n");
				printf("Chose a room:\n1) Tiny\n2) Small\n3) Medium\n4) Large\n5) Huge\n0) done\n");
				cin >> inputI;
				if (inputI == 0)
					donePartial = true;
				else if (inputI < 0 || inputI > 5)
				{
					printf("Invalid Input\n");
				}
				else
				{
					int index = inputI - 1;
					printf("Low: %d || Hight: %d\n", mPresets.SizeRange[index].Low, mPresets.SizeRange[index].High);
					printf("Input Low: \n");
					cin >> inputI;
					mPresets.SizeRange[index].Low = inputI;
					printf("Input High: \n");
					cin >> inputI;
					mPresets.SizeRange[index].High = inputI;
				}
			}
			break;
		case 2:
			printf("Target Room Size:      %d\n", mPresets.TargetSize);
			printf("Chose a new room target size:\n1) Tiny\n2) Small\n3) Medium\n4) Large\n5) Huge\n0) done\n");
			cin >> inputI;
			if (inputI < 1 || inputI > 5)
			{
				inputI--;
				mPresets.TargetSize = inputI;
			}
			break;
		case 3:
			printf("# Target Rooms:        %d\n", mPresets.targetRooms);
			printf("Input new # Target Rooms: ");
			cin >> inputI;
			if (inputI > 0)
			{
				mPresets.targetRooms = inputI;
			}
			break;
		case 4:
			printf("# Target Dead Ends:    %d\n", mPresets.targetDeadEnds);
			printf("Input new # Target Dead Ends: ");
			cin >> inputI;
			if (inputI > 0)
			{
				mPresets.targetDeadEnds = inputI;
			}
			break;
		case 5:
			printf("Corridor Size:         %d\n", mPresets.CorridorSize);
			printf("Input new Corridor Size (Do not make this larger than 1/2 the lowest size of the target room): \n");
			cin >> inputI;
			if (inputI > 0)
			{
				mPresets.targetDeadEnds = inputI;
			}
			break;
		case 6:
			printf("Dungeon Size Modifier: %f\n", mPresets.MapSize);
			printf("Enter a new float modifier: \n");
			cin >> inputF;
			if (inputF > 0)
			{
				mPresets.MapSize = inputF;
			}
			break;
		case 7:
			printf("Dungeon Connectivity Rate (0-100): %d\n", mPresets.Fusion);
			donePartial = false;
			while (!donePartial)
			{
				printf("Enter a new Connectivity Rate (0-100): \n");
				cin >> inputI;
				if (inputI < 0 || inputI > 100)
				{
					printf("Error, Invalid number");
				}
				else
				{
					donePartial = true;
					mPresets.Fusion = inputI;
				}
			}
			break;
		case 8:
			printf("Input File Name\n");
			cin >> FileName;
			File.open(FileName, fstream::in);
			if (File.is_open())
			{
				File >> Garbage;
				File >> Garbage >> mPresets.SizeRange[0].Low >> Garbage >> mPresets.SizeRange[0].High;
				File >> Garbage >> mPresets.SizeRange[1].Low >> Garbage >> mPresets.SizeRange[1].High;
				File >> Garbage >> mPresets.SizeRange[2].Low >> Garbage >> mPresets.SizeRange[2].High;
				File >> Garbage >> mPresets.SizeRange[3].Low >> Garbage >> mPresets.SizeRange[3].High;
				File >> Garbage >> mPresets.SizeRange[4].Low >> Garbage >> mPresets.SizeRange[4].High;
				File >> Garbage;
				File >> Garbage >> mPresets.TargetSize;
				File >> Garbage >> mPresets.targetRooms;
				File >> Garbage >> mPresets.targetDeadEnds;
				File >> Garbage >> mPresets.CorridorSize;
				File >> Garbage >> mPresets.Fusion;
			}
			else
			{
				printf("Error Opening File\n");
			}
			File.close();
			break;
		case 9:
			printf("Input File Name\n");
			cin >> FileName;
			File.open("Test.txt", fstream::out | fstream::trunc);
			File << "Room_Sizes\n";
			File << "Tiny_Low " << mPresets.SizeRange[0].Low << " Tiny_High " << mPresets.SizeRange[0].High << endl;
			File << "Small_Low " << mPresets.SizeRange[1].Low << " Small_High " << mPresets.SizeRange[1].High << endl;
			File << "Med_Low " << mPresets.SizeRange[2].Low << " Med_High " << mPresets.SizeRange[2].High << endl;
			File << "Large_Low " << mPresets.SizeRange[3].Low << " Large_High " << mPresets.SizeRange[3].High << endl;
			File << "Huge_Low " << mPresets.SizeRange[4].Low << " Huge_High " << mPresets.SizeRange[4].High << endl;
			File << "--------\n";
			File << "Target_Size: " << mPresets.TargetSize << endl;
			File << "Num_Rooms: " << mPresets.targetRooms << endl;
			File << "Num_Dead: " << mPresets.targetDeadEnds << endl;
			File << "Corridor_Size: " << mPresets.CorridorSize << endl;
			File << "Fusion_Rate: " << mPresets.Fusion << endl;
			break;
		case 0:
			done = true;
			break;
		default:
			printf("Invalid input\n");
			break;
		}
	}
	mGenerator.LoadSettings(mPresets);
};
void GraphicInterface::run() 
{
	bool nextAction = false;
	bool testAction = false;
	srand(time(NULL));
	int xDisp = 400;
	int yDisp = 300;

	float scale = 1.0f;
	float scaleDist = 2.0f;

	int progress = 0;
	sf::RenderWindow App(sf::VideoMode(800, 600), "Dungeon Generator");

	//mGenerator.LoadSettings();
	mGenerator.GenerateRooms();
	std::vector<Room> RoomsList = mGenerator.GetRooms();
	std::vector<Room> BaseRoomList = mGenerator.GetBaseRooms();
	std::vector<Room> DeadEndList = mGenerator.GetDeadEnds();
	std::vector<Connection> Connections;
	std::vector<Connection> Pruned;
	Matrix* Dungeon;
	int sizeR = RoomsList.size();
	int sizeB = BaseRoomList.size();
	int sizeD = DeadEndList.size();
	int sizeC;
	int sizeP;
	int dLength;
	int dHeight;

	while (App.isOpen())
	{
		sf::Event Event;
		while (App.pollEvent(Event))
		{
			// Close window : exit
			if (Event.type == sf::Event::Closed)
				App.close();
			if (Event.type == sf::Event::KeyReleased)
			{
				nextAction = true;
			}
		}
		App.clear(sf::Color::Black);
		switch (progress)
		{
		case 0://inti rooms show
			if (nextAction)
			{
				progress = 1;
				nextAction = false;
			}
			else
			{
				for (int i = 0; i < sizeR; i++)
				{
					Room temp = RoomsList[i];
					b2Vec2 pos = temp.mpSelf->GetPosition();

					sf::RectangleShape Rect = sf::RectangleShape(sf::Vector2f(temp.wx, temp.hy));
					Rect.setFillColor(sf::Color::Blue);
					Rect.setOutlineColor(sf::Color::Black);
					Rect.setOutlineThickness(-1.0f);
					Rect.setOrigin(sf::Vector2f(temp.wx / 2, temp.hy / 2));
					Rect.setScale(scale, scale);
					Rect.setPosition(sf::Vector2f((pos.x*scaleDist) + xDisp, (pos.y*scaleDist) + yDisp));


					App.draw(Rect);
				}
			}
			break;
		case 1://physics rooms
			if (testAction)
			{
				if (nextAction)
				{
					progress = 2;
					nextAction = false;
					testAction = false;
				}
			}
			else
			{
				mGenerator.PhysicsRooms();
				testAction = true;
			}
			for (int i = 0; i < sizeR; i++)
			{
				Room temp = RoomsList[i];
				b2Vec2 pos = temp.mpSelf->GetPosition();

				sf::RectangleShape Rect = sf::RectangleShape(sf::Vector2f(temp.wx, temp.hy));
				Rect.setFillColor(sf::Color::Blue);
				Rect.setOutlineColor(sf::Color::White);
				Rect.setOutlineThickness(-1.0f);
				Rect.setOrigin(sf::Vector2f(temp.wx / 2, temp.hy / 2));
				Rect.setScale(scale, scale);
				Rect.setPosition(sf::Vector2f((pos.x*scaleDist) + xDisp, (pos.y*scaleDist) + yDisp));


				App.draw(Rect);
			}
			break;
		case 2://define base and end.
			if (nextAction)
			{
				mGenerator.SetupDungeon();
				Dungeon = mGenerator.getCurrentDungeon();
				dLength = Dungeon->size();
				dHeight = (*Dungeon)[0].size();
				nextAction = false;
				progress = 3;
			}
			for (int i = 0; i < sizeD; i++)
			{
				Room temp = DeadEndList[i];
				b2Vec2 pos = temp.mpSelf->GetPosition();

				sf::RectangleShape Rect = sf::RectangleShape(sf::Vector2f(temp.wx, temp.hy));
				Rect.setFillColor(sf::Color::Red);
				Rect.setOutlineColor(sf::Color::White);
				Rect.setOutlineThickness(-1.0f);
				Rect.setOrigin(sf::Vector2f(temp.wx / 2, temp.hy / 2));
				Rect.setScale(scale, scale);
				Rect.setPosition(sf::Vector2f((pos.x*scaleDist) + xDisp, (pos.y*scaleDist) + yDisp));


				App.draw(Rect);
			}
			for (int i = 0; i < sizeB; i++)
			{
				Room temp = BaseRoomList[i];
				b2Vec2 pos = temp.mpSelf->GetPosition();

				sf::RectangleShape Rect = sf::RectangleShape(sf::Vector2f(temp.wx, temp.hy));
				Rect.setFillColor(sf::Color::Green);
				Rect.setOutlineColor(sf::Color::White);
				Rect.setOutlineThickness(-1.0f);
				Rect.setOrigin(sf::Vector2f(temp.wx / 2, temp.hy / 2));
				Rect.setScale(scale, scale);
				Rect.setPosition(sf::Vector2f((pos.x*scaleDist) + xDisp, (pos.y*scaleDist) + yDisp));


				App.draw(Rect);
			}
			break;
		case 3://setup dungeon
			if (nextAction)
			{
				progress = 4;
				nextAction = false;
				mGenerator.ConnectRooms();
				Connections = mGenerator.GetConnections();
				sizeC = Connections.size();
			}
			for (int i = 0; i < dLength; i++)
			{
				for (int j = 0; j < dHeight; j++)
				{
					if ((*Dungeon)[i][j] > 0)
					{
						sf::RectangleShape Rect = sf::RectangleShape(sf::Vector2f(scale, scale));
						Rect.setOutlineColor(sf::Color::White);
						Rect.setOutlineThickness(-0.2f);
						Rect.setPosition(sf::Vector2f(scale*i, scale*j));

						if ((*Dungeon)[i][j] == 1)
						{
							Rect.setFillColor(sf::Color::Green);
						}
						else if ((*Dungeon)[i][j] == 2)
						{
							Rect.setFillColor(sf::Color::Red);
						}
						App.draw(Rect);

					}
				}
			}
			break;
		case 4://connect dungeon
			if (nextAction)
			{
				progress = 5;
				nextAction = false;
				mGenerator.PruneRooms();
				Pruned = mGenerator.GetPruned();
				sizeP = Pruned.size();
			}
			for (int i = 0; i < dLength; i++)
			{
				for (int j = 0; j < dHeight; j++)
				{
					if ((*Dungeon)[i][j] > 0)
					{
						sf::RectangleShape Rect = sf::RectangleShape(sf::Vector2f(scale, scale));
						Rect.setOutlineColor(sf::Color::White);
						Rect.setOutlineThickness(-0.2f);
						Rect.setPosition(sf::Vector2f(scale*i, scale*j));

						if ((*Dungeon)[i][j] == 1)
						{
							Rect.setFillColor(sf::Color::Green);
						}
						else if ((*Dungeon)[i][j] == 2)
						{
							Rect.setFillColor(sf::Color::Red);
						}
						App.draw(Rect);

					}
				}
			}
			for (int i = 0; i < sizeC; i++)
			{
				sf::VertexArray temp(sf::LinesStrip,2);
				float xPos = (Connections[i].r1->xStart + Connections[i].r1->xEnd) /2;
				float yPos = (Connections[i].r1->yStart + Connections[i].r1->yEnd) / 2;
				temp[0].position.x = (xPos*scale);
				temp[0].position.y = (yPos*scale);
				temp[0].color = sf::Color::Blue;

				xPos = (Connections[i].r2->xStart + Connections[i].r2->xEnd) / 2;
				yPos = (Connections[i].r2->yStart + Connections[i].r2->yEnd) / 2;
				temp[1].position.x = (xPos*scale);
				temp[1].position.y = (yPos*scale);
				temp[1].color = sf::Color::Blue;

				App.draw(temp);
			}
			break;
		case 5://prune dungeon
			if (nextAction)
			{
				progress = 6;
				nextAction = false;
				mGenerator.CreateCorridors();
			}
			for (int i = 0; i < dLength; i++)
			{
				for (int j = 0; j < dHeight; j++)
				{
					if ((*Dungeon)[i][j] > 0)
					{
						sf::RectangleShape Rect = sf::RectangleShape(sf::Vector2f(scale, scale));
						Rect.setOutlineColor(sf::Color::White);
						Rect.setOutlineThickness(-0.2f);
						Rect.setPosition(sf::Vector2f(scale*i, scale*j));

						if ((*Dungeon)[i][j] == 1)
						{
							Rect.setFillColor(sf::Color::Green);
						}
						else if ((*Dungeon)[i][j] == 2)
						{
							Rect.setFillColor(sf::Color::Red);
						}
						App.draw(Rect);

					}
				}
			}
			for (int i = 0; i < sizeP; i++)
			{
				sf::VertexArray temp(sf::LinesStrip, 2);
				float xPos = (Pruned[i].r1->xStart + Pruned[i].r1->xEnd) / 2;
				float yPos = (Pruned[i].r1->yStart + Pruned[i].r1->yEnd) / 2;
				temp[0].position.x = (xPos*scale);
				temp[0].position.y = (yPos*scale);
				temp[0].color = sf::Color::Blue;

				xPos = (Pruned[i].r2->xStart + Pruned[i].r2->xEnd) / 2;
				yPos = (Pruned[i].r2->yStart + Pruned[i].r2->yEnd) / 2;
				temp[1].position.x = (xPos*scale);
				temp[1].position.y = (yPos*scale);
				temp[1].color = sf::Color::Blue;

				App.draw(temp);
			}
			break;
		case 6:
			for (int i = 0; i < dLength; i++)
			{
				for (int j = 0; j < dHeight; j++)
				{
					if ((*Dungeon)[i][j] > 0)
					{
						sf::RectangleShape Rect = sf::RectangleShape(sf::Vector2f(scale, scale));
						Rect.setOutlineColor(sf::Color::White);
						Rect.setOutlineThickness(-0.2f);
						Rect.setPosition(sf::Vector2f(scale*i, scale*j));

						if ((*Dungeon)[i][j] == 1)
						{
							Rect.setFillColor(sf::Color::Green);
						}
						else if ((*Dungeon)[i][j] == 2)
						{
							Rect.setFillColor(sf::Color::Red);
						}
						else if ((*Dungeon)[i][j] == 3)
						{
							Rect.setFillColor(sf::Color::White);
						}
						App.draw(Rect);

					}
				}
			}
			break;
		default:
			break;
		}
		App.display();
	}
};

void GraphicInterface::Debug()
{
	srand(time(NULL));

	float scale = 2.0f;
	mGenerator.RunCreation();
	Matrix* Dungeon;
	std::vector<Connection> Connections;
	int dLength;
	int dHeight;
	int cSize;


	Connections = mGenerator.GetPruned();
	Dungeon = mGenerator.getCurrentDungeon();
	dLength = Dungeon->size();
	dHeight = (*Dungeon)[0].size();
	cSize = Connections.size();

	sf::RenderWindow App(sf::VideoMode(dLength*scale, dHeight*scale), "Dungeon Generator");
	while (App.isOpen())
	{
		sf::Event Event;
		while (App.pollEvent(Event))
		{
			// Close window : exit
			if (Event.type == sf::Event::Closed)
				App.close();
			if (Event.type == sf::Event::KeyReleased)
			{
				App.close();
				printf("Rerun\n");
				mGenerator.ReRun();
				Connections = mGenerator.GetPruned();
				Dungeon = mGenerator.getCurrentDungeon();
				dLength = Dungeon->size();
				dHeight = (*Dungeon)[0].size();
				cSize = Connections.size();
				App.create(sf::VideoMode(dLength*scale, dHeight*scale), "Dungeon Generator");
			}
		}
		App.clear(sf::Color::Black);
		for (int i = 0; i < dLength; i++)
		{
			for (int j = 0; j < dHeight; j++)
			{
				if ((*Dungeon)[i][j] > 0)
				{
					sf::RectangleShape Rect = sf::RectangleShape(sf::Vector2f(scale, scale));
					Rect.setOutlineColor(sf::Color::White);
					Rect.setOutlineThickness(-0.2f);
					Rect.setPosition(sf::Vector2f(scale*i, scale*j));

					if ((*Dungeon)[i][j] == 1)
					{
						Rect.setFillColor(sf::Color::Green);
					}
					else if ((*Dungeon)[i][j] == 2)
					{
						Rect.setFillColor(sf::Color::Red);
					}
					else if ((*Dungeon)[i][j] == 3)
					{
						Rect.setFillColor(sf::Color::White);
					}
					else if ((*Dungeon)[i][j] == 4)
					{
						Rect.setFillColor(sf::Color::Blue);
					}
					App.draw(Rect);

				}
			}
		}
		for (int i = 0; i < cSize; i++)
		{
			sf::VertexArray temp(sf::LinesStrip, 2);
			float xPos = (Connections[i].r1->xStart + Connections[i].r1->xEnd) / 2;
			float yPos = (Connections[i].r1->yStart + Connections[i].r1->yEnd) / 2;
			temp[0].position.x = (xPos*scale);
			temp[0].position.y = (yPos*scale);
			temp[0].color = sf::Color::Blue;

			xPos = (Connections[i].r2->xStart + Connections[i].r2->xEnd) / 2;
			yPos = (Connections[i].r2->yStart + Connections[i].r2->yEnd) / 2;
			temp[1].position.x = (xPos*scale);
			temp[1].position.y = (yPos*scale);
			temp[1].color = sf::Color::Blue;

			App.draw(temp);
		}
		App.display();
	}
}