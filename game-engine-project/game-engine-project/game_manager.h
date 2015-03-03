//This file holds all pertinent game information like actions players
//can perform, game mechanics, updating AI, and other relevant information.
//It should have an InputManager in it for getting input from players, and
//an AIManager for controlling the AI of the game effectively

#pragma once

class GameManager
{
private:

public:
	GameManager();
	~GameManager();
	void Update(float dt);
	void Startup();
	void Shutdown();
};