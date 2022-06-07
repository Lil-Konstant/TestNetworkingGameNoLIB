#pragma once
#include <Client.h>

/// <summary>
/// GameClient is a class which derives from the Client class defined in the
/// NetworkingModule Client.lib library, and extends the class to include logic
/// specific to this test application game. This class acts as the main application 
/// class for the test app, and allows users to control an orb in the middle of the 
/// screen that has 2D directional movement and can "eat" other orbs on the screen 
/// to get bigger.The class implements special update logic for user input and 
/// checking GameObject overlaps, special setStartPosition logic for client placment 
/// and orb setup, and special endGame() logic.
/// </summary>
class GameClient : public Client
{
public:

	virtual bool startup() override;
	virtual void update(float deltaTime) override;
	virtual void draw() override;

private:

	// Client overrides, called in Client.lib code
	virtual void setStartPosition() override;
	virtual void endGame(bool hasWon) override;
	
	// Test App specific functions
	void setupOrbs();
	bool checkOverlap(GameObject* object);
	void collectOrb(int orbID);
};
