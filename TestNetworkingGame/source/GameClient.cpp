#include "GameClient.h"

#include <Input.h>
#include <glm/ext.hpp>
#include <iostream>

using glm::vec3;
using glm::vec4;

bool GameClient::startup()
{
	// Call the client startup to make sure networking is handled correctly
	Client::startup();

	// Create simple camera transforms centred on the player
	m_viewMatrix = glm::lookAt(m_localGameObjects[0]->position + vec3(0, 40, 0), m_localGameObjects[0]->position, vec3(0, 0, 1));
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, getWindowWidth() / (float)getWindowHeight(), 0.1f, 1000.f);

    return true;
}

void GameClient::update(float deltaTime)
{
	if (m_localGameObjects.size() > 0)
	{
		Client::update(deltaTime);

		aie::Input* input = aie::Input::getInstance();
		m_localGameObjects[0]->velocity = vec3(0);
		if (input->isKeyDown(aie::INPUT_KEY_LEFT))
		{
			m_localGameObjects[0]->position.x += 10.0f * deltaTime;
			m_localGameObjects[0]->velocity = vec3(10.0f, 0, 0);
		}
		else if (input->isKeyDown(aie::INPUT_KEY_RIGHT))
		{
			m_localGameObjects[0]->position.x -= 10.0f * deltaTime;
			m_localGameObjects[0]->velocity = vec3(-10.0f, 0, 0);
		}
		else if (input->isKeyDown(aie::INPUT_KEY_DOWN))
		{
			m_localGameObjects[0]->position.z -= 10.0f * deltaTime;
			m_localGameObjects[0]->velocity = vec3(0, 0, -10.0f);
		}
		else if (input->isKeyDown(aie::INPUT_KEY_UP))
		{
			m_localGameObjects[0]->position.z += 10.0f * deltaTime;
			m_localGameObjects[0]->velocity = vec3(0, 0, 10.0f);
		}

		for (auto object : m_otherClientGameObjects)
		{
			int ID = object.first;

			// If we are currently overlapping this object and are bigger than it, attempt to collect it
			if (checkOverlap(object.second) && m_localGameObjects[0]->radius > object.second->radius)
			{
				collectOrb(ID);
			}
		}

		// Update the simple camera transforms to stay centred on the player
		m_viewMatrix = glm::lookAt(m_localGameObjects[0]->position + vec3(0, 40, 0), m_localGameObjects[0]->position, vec3(0, 0, 1));
	}
}

void GameClient::draw()
{
	// If the game hasn't ended for this client yet, draw as normal
	if (!m_gameOver)
	{
		Client::draw();
	}
	// Otherwise display a game over screen
	else
	{
		// wipe the screen to the background colour
		clearScreen();
	}
}

/// <summary>
/// Override of the setStartPosition base class function called on client connection.
/// This function simply either spawns a player at the top or bottom of the screen
/// depending on their client ID.
/// </summary>
void GameClient::setStartPosition()
{
	if (m_localGameObjects[0]->ID % 2 == 0)
	{
		m_localGameObjects[0]->position = vec3(0, 0, -15);
		setupGame();
	}
	else
	{
		m_localGameObjects[0]->position = vec3(0, 0, 15);
	}
}

void GameClient::endGame()
{
	std::cout << "You lost! Game Over." << std::endl;
	m_gameOver = true;
	Application::quit();
}

void GameClient::setupGame()
{
	srand(time(NULL));

	for (int i = 0; i < 1; i++)
	{
		vec3 position = vec3(rand() % 15, 0, rand() % 15);
		vec3 velocity = vec3(0);

		sendSpawnObjectPacket(position, velocity);
	}
}

bool GameClient::checkOverlap(GameObject* object)
{
	return glm::distance(m_localGameObjects[0]->position, object->position) <= m_localGameObjects[0]->radius - object->radius;
}

// increase size and maybe speed
void GameClient::collectOrb(int orbID)
{
	m_localGameObjects[0]->radius += m_otherClientGameObjects[orbID]->radius;
	sendDespawnObjectPacket(orbID);
}
