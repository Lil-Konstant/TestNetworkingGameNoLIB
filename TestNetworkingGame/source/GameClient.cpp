#include "GameClient.h"
#include <Input.h>
#include <glm/ext.hpp>
#include <iostream>
#include <cstdio>

using glm::vec3;
using glm::vec4;

/// <summary>
/// The GameClient override of startup() is simply used to choose the IP address
/// for the server we wish to connect to, it is "127.0.0.1" by default in the client
/// code, but it is redefined here to show how an end-user would change the IP for whatever
/// server they wish to connect to. The function sets the IP and then calls startup on 
/// the base client class to handle network connection.
/// </summary>
bool GameClient::startup()
{
	Client::IP = "127.0.0.1";
	Client::startup();
	return true;
}

/// <summary>
/// update() is an override of the Client base class update() function, and must first call the base
/// class update() to handle the networking functionality provided in the Client.dll. The function then
/// checks to see if there are any other objects left in the game, if not it calls endGame() on this client.
/// The function will then go through and check for up-down-left-right inputs from the user, and will apply
/// a corresponding velocity to this client's GameObject if input is found.
/// </summary>
void GameClient::update(float deltaTime)
{
	// If this clients GameObject is still alive
	if (m_localGameObjects.size() > 0)
	{
		// Call the base class update to handle networking
		Client::update(deltaTime);

		// Check for directional input from the user and move this client's GameObject if input is found
		aie::Input* input = aie::Input::getInstance();
		m_localGameObjects[0]->velocity = vec3(0); // Zero the velocity this update loop
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

		// Iterate through all orbs and other players and check for overlaps with them
		for (auto object : m_otherClientGameObjects)
		{
			// If we are currently overlapping this object and are bigger than it, attempt to collect it
			if (checkOverlap(object.second) && m_localGameObjects[0]->radius > object.second->radius)
			{
				collectOrb(object.first);
			}
		}

		// Update the simple camera transforms to stay centred on the player
		m_viewMatrix = glm::lookAt(m_localGameObjects[0]->position + vec3(0, 40, 0), m_localGameObjects[0]->position, vec3(0, 0, 1));
	}
}

/// <summary>
/// The GameClient override for draw() simply adds an AABB border that fits the extent
/// of where orbs can spawn in the game so the player doesn't get lost, and then calls
/// the base class draw() function as normal.
/// </summary>
void GameClient::draw()
{
	aie::Gizmos::addAABB(vec3(0), vec3(25, 0, 25), vec4(1, 0, 0, 1));
	Client::draw();
}

/// <summary>
/// setStartPosition is an override of the Client base class function which is called 
/// on client connection. This function simply spawns a player within a 50 unit range
/// on the map, and will setup the orbs in the game if this client is the first one to
/// be joining the server.
/// </summary>
void GameClient::setStartPosition()
{
	// If this is the first client to connect, setup the orbs in the game
	if (m_localGameObjects[0]->ID == 1)
	{
		setupOrbs();
	}

	m_localGameObjects[0]->position = vec3(-20, 0, -20) + vec3(rand() % 40, 0, rand() % 40);
}

/// <summary>
/// endGame() is a Client override called either when this client's GameObject is "swallowed" by 
/// another client (in the DespawnObject sequence), or when there are no more orbs left to eat. 
/// The function simply closes the application window and prints either a win or fail text to the 
/// console.
/// </summary>
void GameClient::endGame(bool hasWon)
{
	// Ensure the game hasn't already ended for this client
	if (!m_gameOver)
	{
		// If this client has won, print a win message
		if (hasWon)
		{
			std::cout << "You win! Congrats!" << std::endl;
		}
		// Otherwise if this client has lost, print a lose message
		else
		{
			std::cout << "You lost! Game Over." << std::endl;
		}

		// End the application loop
		m_gameOver = true;
		Application::quit();

		// Wait for user input to close the terminal
		std::system("pause");
	}
	
}

/// <summary>
/// setupOrbs() is a utility function that is used to spawn the collectable orbs
/// on the server, which are then sent to all clients as simple GameObjects. The
/// function uses rand() to spawn the orbs at random points around the map.
/// </summary>
void GameClient::setupOrbs()
{
	// Seed rand with the current system time    
	srand(time(NULL));

	// Create X orbs around the map within a X unit range from the centre
	for (int i = 0; i < 20; i++)
	{
		vec3 position = vec3(-25, 0, -25) + vec3(rand() % 50, 0, rand() % 50);

		// Request that the server spawns an object at the calculated position
		sendSpawnObjectPacket(position, vec3(0));
	}
}

/// <summary>
/// checkOverlap takes an input of a pointer to the GameObject to check overlap with, and will return
/// whether or not the inputted GameObject overlaps with this client's GameObject.
/// </summary>
/// <param name="object">The GameObject to check overlap with.</param>
/// <returns>Returns true if overlapping, false otherwise.</returns>
bool GameClient::checkOverlap(GameObject* object)
{
	return glm::distance(m_localGameObjects[0]->position, object->position) <= m_localGameObjects[0]->radius - object->radius;
}

/// <summary>
/// collectOrb() takes an input of the GameObject ID of the orb that this client is collecting,
/// and simply increases the radius of this client by the orbs radius and sends the server a request
/// to despawn the orb from all clients. This function works when collecting both orbs and other client
/// GameObjects.
/// </summary>
/// <param name="orbID">The server ID of the orb to collect.</param>
void GameClient::collectOrb(int orbID)
{
	m_localGameObjects[0]->radius += m_otherClientGameObjects[orbID]->radius;
	sendDespawnObjectPacket(orbID);
}
