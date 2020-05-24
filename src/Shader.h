#pragma once

#include <string>
#include <unordered_map>
#include <glm\ext\matrix_float4x4.hpp>

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{

private:
	std::string m_FilePath;
	unsigned int m_RendererID;
	std::unordered_map<std::string, int> m_UniformLocationCache;
	//caching uniform
public:
	Shader(const std::string& filePath);
	~Shader();

	void Bind() const;
	void UnBind() const;

	void SetUniform1i(const std::string& name, int value);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform4f(const std::string& name, float f0, float f1, float f2, float f3);
	void SetUniformMat4f(const std::string& name, const glm::mat4 matrix);
private:
	int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	bool CompileShader();
	ShaderProgramSource ParseShader(const std::string& filePath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	
	int GetUniformLocation(const std::string& name);
};