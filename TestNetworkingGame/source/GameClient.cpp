#include "GameClient.h"
#include <Input.h>
#include <glm/ext.hpp>

using glm::vec3;

bool GameClient::startup()
{
	// Call the client startup to make sure networking is handled correctly
	Client::startup();

	// create simple camera transforms
	m_viewMatrix = glm::lookAt(vec3(0, 40, 0), vec3(0), vec3(0, 0, 1));
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, getWindowWidth() / (float)getWindowHeight(), 0.1f, 1000.f);

    return true;
}

void GameClient::update(float deltaTime)
{
	Client::update(deltaTime);

	aie::Input* input = aie::Input::getInstance();
	m_myGameObject.velocity = glm::vec3(0);
	if (input->isKeyDown(aie::INPUT_KEY_LEFT))
	{
		m_myGameObject.position.x += 10.0f * deltaTime;
		m_myGameObject.velocity = glm::vec3(10.0f, 0, 0);
	}
	if (input->isKeyDown(aie::INPUT_KEY_RIGHT))
	{
		m_myGameObject.position.x -= 10.0f * deltaTime;
		m_myGameObject.velocity = glm::vec3(-10.0f, 0, 0);
	}
}

void GameClient::setStartPosition()
{
	if (m_myGameObject.ID == 1)
	{
		m_myGameObject.position = vec3(0, 0, 15);
	}
	else
	{
		m_myGameObject.position = vec3(0, 0, -15);
	}
}
