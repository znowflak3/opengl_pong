#include "TestPong.h"

#include <GL/glew.h>

#include "Renderer.h"
#include <vendor\imgui\imgui.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


test::TestPong::TestPong()
	: m_Projection(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
	m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
	m_PlayerA(glm::vec3(0, 270, 0), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)),
	m_PlayerB(glm::vec3(960, 270, 0), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)),
	m_Ball(glm::vec3(480, 270, 0), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f))
{

	float positions[] = {
		-50.0f, -50.0f, 0.0f, 0.0f,
		50.0f, -50.0f, 1.0f, 0.0f,
		50.0f, 50.0f, 1.0f, 1.0f,
		-50.0f, 50.0f, 0.0f, 1.0f
	};

	float ballPositions[] = {
		-40.0f, -40.0f, 0.0f, 0.0f,
		40.0f, -40.0f, 1.0f, 0.0f,
		40.0f, 40.0f, 1.0f, 1.0f,
		-40.0f, 40.0f, 0.0f, 1.0f
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	m_VAO = std::make_unique<VertexArray>();
	m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));
	VertexBufferLayout layout;
	layout.Push<float>(2);
	layout.Push<float>(2);
	m_VAO->AddBuffer(*m_VertexBuffer, layout);

	m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);

	m_Shader = std::make_unique<Shader>("res/shaders/PongPlayer.shader");

	//ball

	m_BallVAO = std::make_unique<VertexArray>();
	m_BallVertexBuffer = std::make_unique<VertexBuffer>(ballPositions, 4 * 4 * sizeof(float));
	VertexBufferLayout ballLayout;
	ballLayout.Push<float>(2);
	ballLayout.Push<float>(2);
	m_BallVAO->AddBuffer(*m_BallVertexBuffer, ballLayout);

	m_BallIndexBuffer = std::make_unique<IndexBuffer>(indices, 6);

	m_BallShader = std::make_unique<Shader>("res/shaders/Basic.shader");
	m_BallShader->Bind();

	m_BallTexture = std::make_unique<Texture>("res/textures/Circle.png");
	m_BallShader->SetUniform1i("u_Texture", 0);

	//ball speed
	m_BallVelocityX = 300.0f;
	m_BallVelocityY = 150.0f;
}

test::TestPong::~TestPong()
{
}

void test::TestPong::OnUpdate(float deltaTime, InputManager* input)
{
	m_DeltaTime = deltaTime;

	BallMovement();
	ProcessAInput(input, &m_PlayerA);
	ProcessBInput(input, &m_PlayerB);
}

void test::TestPong::OnRender()
{

	GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
	GLCall(glClear(GL_COLOR_BUFFER_BIT));

	Renderer renderer;

	m_BallTexture->Bind();
	{
		glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Ball.Position);
		glm::mat4 mvp = m_Projection * m_View * model;
		m_BallShader->Bind();
		m_BallShader->SetUniformMat4f("u_MVP", mvp);
		//m_BallShader->SetUniform4f("u_Color", m_PlayerB.Color.x, m_PlayerB.Color.y, m_PlayerB.Color.z, 0.5f);
		renderer.Draw(*m_BallVAO, *m_BallIndexBuffer, *m_BallShader);
	}
	{
		glm::mat4 model = glm::translate(glm::mat4(1.0f), m_PlayerA.Position);
		glm::mat4 mvp = m_Projection * m_View * model;
		m_Shader->Bind();
		m_Shader->SetUniformMat4f("u_MVP", mvp);
		m_Shader->SetUniform4f("u_Color", m_PlayerA.Color.x, m_PlayerA.Color.y, m_PlayerA.Color.z, m_PlayerA.Color.w);
		renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
	}
	{
		glm::mat4 model = glm::translate(glm::mat4(1.0f), m_PlayerB.Position);
		glm::mat4 mvp = m_Projection * m_View * model;
		m_Shader->Bind();
		m_Shader->SetUniformMat4f("u_MVP", mvp);
		m_Shader->SetUniform4f("u_Color", m_PlayerB.Color.x, m_PlayerB.Color.y, m_PlayerB.Color.z, m_PlayerB.Color.w);
		renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
	}

}

void test::TestPong::OnImGuiRender()
{
	ImGui::Text("Player B", m_PlayerB.Position.x, m_PlayerB.Position.y);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}

bool test::TestPong::DidCollide(glm::vec3 newPos)
{
	glm::vec2 positionsA[] = {
	glm::vec2((-50.0f + newPos.x), (-50.0f + newPos.y)),
	glm::vec2((50.0f + newPos.x), (-50.0f + newPos.y)),
	glm::vec2((50.0f + newPos.x), (50.0f + newPos.y)),
	glm::vec2((-50.0f + newPos.x), (50.0f + newPos.y)),
	};
	glm::vec2 positionsB[] = {
	glm::vec2((-50.0f + m_PlayerB.Position.x), (-50.0f + m_PlayerB.Position.y)),
	glm::vec2((50.0f + m_PlayerB.Position.x), (-50.0f + m_PlayerB.Position.y)),
	glm::vec2((50.0f + m_PlayerB.Position.x), (50.0f + m_PlayerB.Position.y)),
	glm::vec2((-50.0f + m_PlayerB.Position.x), (50.0f + m_PlayerB.Position.y))
	};


	for (int i = 0; i < 4; i++)
	{
		if ((positionsA[i].x >= positionsB[0].x && positionsA[i].x <= positionsB[1].x) &&
			(positionsA[i].y >= positionsB[0].y && positionsA[i].y <= positionsB[3].y))
		{
			return true;

		}
	}

	return false;
}

bool test::TestPong::CheckCircleCollision(glm::vec2 ball, glm::vec2 player)
{
	m_BallRadius = 40.0f;
	glm::vec2 center(glm::vec2(ball.x - 40.0f, ball.y - 40.0f) + m_BallRadius);

	// calculate AABB info (center, half-extents)
	glm::vec2 aabb_half_extents(50.0f, 50.0f);
	glm::vec2 aabb_center(
		player.x - 50.0f + aabb_half_extents.x,
		player.y - 50.0f + aabb_half_extents.y
	);

	// get difference vector between both centers
	glm::vec2 difference = center - aabb_center;
	glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
	// add clamped value to AABB_center and we get the value of box closest to circle
	glm::vec2 closest = aabb_center + clamped;
	// retrieve vector between center circle and closest point AABB and check if length <= radius
	difference = closest - center;
	return glm::length(difference) < m_BallRadius;

}

void test::TestPong::ProcessAInput(InputManager* input, GameObject2D* object)
{
	float velocity = 300.0f;



	if (input->IsKeyPressed("W"))
	{
		glm::vec3 newPos = object->Position;
		newPos.y += (1 * velocity * m_DeltaTime);

		if (!DidCollide(newPos) && !CheckCircleCollision(m_Ball.Position, newPos))
			object->Position.y += 1 * velocity * m_DeltaTime;

	}
	else if (input->IsKeyPressed("S"))
	{
		glm::vec3 newPos = object->Position;
		newPos.y -= (1 * velocity * m_DeltaTime);

		if (!DidCollide(newPos) && !CheckCircleCollision(m_Ball.Position, newPos))
			object->Position.y -= 1 * velocity * m_DeltaTime;
	}
	else if (input->IsKeyPressed("A"))
	{
		glm::vec3 newPos = object->Position;
		newPos.x -= (1 * velocity * m_DeltaTime);

		if (!DidCollide(newPos) && !CheckCircleCollision(m_Ball.Position, newPos))
			object->Position.x -= 1 * velocity * m_DeltaTime;
	}
	else if (input->IsKeyPressed("D"))
	{
		glm::vec3 newPos = object->Position;
		newPos.x += (1 * velocity * m_DeltaTime);

		if (!DidCollide(newPos) && !CheckCircleCollision(m_Ball.Position, newPos))
			object->Position.x += 1 * velocity * m_DeltaTime;
	}


}

void test::TestPong::ProcessBInput(InputManager* input, GameObject2D* object)
{
	if (input->IsKeyPressed("UP"))
	{
		object->Position.y++;
	}
	else if (input->IsKeyPressed("DOWN"))
	{
		object->Position.y--;
	}
}

void test::TestPong::BallMovement()
{
	//if (!CheckCircleCollision(m_Ball, m_PlayerA) && !CheckCircleCollision(m_Ball, m_PlayerB))

	m_Ball.Position.x += 1 * m_BallVelocityX * m_DeltaTime;
	m_Ball.Position.y += 1 * m_BallVelocityY * m_DeltaTime;

	std::cout << "ball position: " << m_Ball.Position.x << " " << m_Ball.Position.y << std::endl;

	if ((m_Ball.Position.x - 40.0f) <= 0.0f)
	{
		m_BallVelocityX = -m_BallVelocityX;
		m_Ball.Position.x = 40.0f;
		std::cout << "PlayerB hit the goal!" << std::endl;
	}
	else if ((m_Ball.Position.x + 40.0f) >= 960.0f)
	{
		m_BallVelocityX = -m_BallVelocityX;
		m_Ball.Position.x = 920.0f;
		std::cout << "PlayerA hit the goal!" << std::endl;
	}

	if ((m_Ball.Position.y - 40.0f) <= 0.0f)
	{

		m_BallVelocityY = -m_BallVelocityY;
		m_Ball.Position.y = 40.0f;
		std::cout << "Ball hit the lower line!" << std::endl;
	}
	else if ((m_Ball.Position.y + 40.0f) >= 540.0f)
	{
		m_BallVelocityY = -m_BallVelocityY;
		m_Ball.Position.y = 500.0f;
		std::cout << "Ball hit the upper line!" << std::endl;
	}

	//check collision with playerA then B
	if (CheckCircleCollision(m_Ball.Position, m_PlayerA.Position))
	{
		m_BallVelocityX = -m_BallVelocityX;
	}
	if (CheckCircleCollision(m_Ball.Position, m_PlayerB.Position))
	{
		m_BallVelocityX = -m_BallVelocityX;
	}

}

