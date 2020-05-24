#pragma once

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include <Texture.h>

#include "Test.h"
#include <GameObject2D.h>
#include <GLFW\glfw3.h>


namespace test
{
	class TestPong : public Test
	{
	public:
		TestPong();
		~TestPong();

		void OnUpdate(float deltaTime, InputManager * input);
		void OnRender();
		void OnImGuiRender();

		void ProcessAInput(InputManager * input, GameObject2D * object);
		void ProcessBInput(InputManager * input, GameObject2D * object);
		void BallMovement();

		bool DidCollide(glm::vec3 newPos);
		bool CheckCircleCollision(glm::vec2 ball, glm::vec2 player);
		
	private:
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;

		std::unique_ptr<VertexArray> m_BallVAO;
		std::unique_ptr<VertexBuffer> m_BallVertexBuffer;
		std::unique_ptr<IndexBuffer> m_BallIndexBuffer;
		std::unique_ptr<Shader> m_BallShader;
		std::unique_ptr<Texture> m_BallTexture;


		float m_DeltaTime;
		float m_BallRadius;

		float m_BallVelocityX;
		float m_BallVelocityY;

		glm::mat4 m_View, m_Projection;
		GameObject2D m_PlayerA, m_PlayerB, m_Ball;
	};
}