#pragma once
#include <Client.h>
class GameClient : public Client
{
public:

	virtual bool startup() override;
	virtual void update(float deltaTime) override;
	virtual void setStartPosition(); // override

private:

};

