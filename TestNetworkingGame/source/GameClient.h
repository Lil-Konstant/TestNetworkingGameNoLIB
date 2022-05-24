#pragma once
#include <Client.h>

class GameClient : public Client
{
public:

	virtual bool startup() override;
	virtual void update(float deltaTime) override;
	virtual void draw() override;

private:

	virtual void setStartPosition() override;
	virtual void endGame() override;
	
	void setupGame();
	bool checkOverlap(GameObject* object);
	void collectOrb(int orbID);
};
