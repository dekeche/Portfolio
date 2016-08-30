# Portfolio

##Neural Network - Unity
This was a self-created project to find out more about evolution algorithms and neural networking.
The basic setup is that x units are spawned at the centre of a wrapped world, and must move towards randomly placed food to survive.
The basic neural network consist of six input nodes, four hidden nodes, and three output nodes.
The input nodes search for if an item is within a certain range and angle of the unit. Left, Right, Forward. Food or other Unit.
Three hidden nodes correspond directly to the three output nodes, Left, Right and Forward.
The final hidden node is triggered by a lack of input, and simply links to the Forward output.
This is so that if the unit does not perceive any food around it, it will still move around.
Finally, the evolution algorithm determines the minimum threshold for each node to trigger, and the intensity of the signal sent from the node.

##PDG - Procedural Dungeon Generation - C++ - Box2D - SFML
This was a self-created project to explore the process of procedural generation, taking inspiration from TinyKeep.
The end goal was to create a library for producing a procedural dungeon, with varying sizes of rooms, number of corridors, 
and a variable number of dead ends.
The basic process is simple, produce x random rooms of varying size at a common center, use physics to distribute them so that none overlap,
discover the base rooms and remove excess, use a spanning tree to determine room connections, place corridors, and finally connect up the dead ends.

##The Last Light - Code Snippets - Unreal
As the senior capstone project at Champlain, I worked on The Last Light, an atmospheric horror game.
One of the key elements of the game was the concept of needing to stay in the light, and having to turn on switches and breakers to light up different areas.
Before I joined the team, the main solution they had been using was to hard code the behaviour of each switch on the level, not the best approach.
Of course, this was not the best approach, and Mindtree games wanted to change the setup, but they didn't know how.
So, my solution was to create the Circuit System. The base idea was to create a class that would interface between a set of lights, or a circuit, 
and the various switches on the level.
To do this, I needed three main classes: a central manager for the system, a component for the lights, and a modified class for the switches
The light component basically registers the light to a particulate circuit.
The central manager takes the registered lights, and generates an array for each circuit, basically a matrix [circuit #][light index]
Finally, the switch simply tells the manager to turn on/off a circuit, and the manger would turn all the lights on that circuit on/off as requested
This class was latter expanded to handle the breaker box mechanic, as well as being able to set circuits on/off by default.
Now, the original system, used in the game, was based around an incorrect methodology. Basically, for every problem, create an object to solve it.
So the original manager, or Circuit Board, had different functions to register different kinds of objects. This basically required constant maintenance, and took up a lot of my time.
The code present here, however, has been rewritten to take a more ECS approach to the problem. Basically, any object that needs to register with the Circuit System, should use the
Circuit Component or Circuit Board Component class, or a derivation of those classes.
