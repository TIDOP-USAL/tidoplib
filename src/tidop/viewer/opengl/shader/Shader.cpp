#include "Shader.h"

#include <fstream>

namespace tl
{

const char* vertexShaderSource = "#version 430 core\n"
    "\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec4 aColor;\n"
    "layout (location = 2) in vec3 aNormal;\n"
    "layout (location = 3) in vec2 aTexCoord;\n"
    "layout (location = 4) in float aLabel;\n"
    "\n"
    "out vec4 Color;\n"
    "out vec3 Normal;\n"
    "out vec2 TexCoord;\n"
    "out float Label;\n"
    "\n"
    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"
    "\n"
    "void main()\n"
    "{\n"
    "   gl_Position = projection * view * model * vec4(aPos.xyz, 1.0);\n"
    "	Color = aColor;\n"
    "	Normal = aNormal;\n"
    "	TexCoord = aTexCoord;\n"
    "   Label = aLabel;\n"
    "}\0";

const char* fragmentShaderSource = "#version 430 core\n"
    "\n"
    "out vec4 FragColor;\n"
    "\n"
    "in vec4 Color;\n"
    "in vec3 Normal;\n"
    "in vec2 TexCoord;\n"
    "in float Label;\n"
    "\n"
    "uniform sampler2D tex;\n"
    "uniform bool hasTexture;\n"
    "\n"
    "void main()\n"
    "{\n"
    "   vec4 color = Color;\n"
    "   vec3 lightPos = vec3(1.0, 0.5, 0.75);\n"
    "   vec4 light = vec4(1.0, 1.0, 0.85, 1.0);\n"
    "	vec4 ambient = vec4(0.3);\n"
    "	if(hasTexture) {\n"
    "		color = texture(tex, TexCoord) * dot(lightPos, Normal) * light;\n"
    "		//color = texture(tex, TexCoord);\n"
    "   }\n"
    "	FragColor = color;\n"
    "}\n\0";


Shader::Shader(const std::string& _code, const ShaderType& _shaderType)
    : code(_code), shaderType(_shaderType) {
    compileShader();
}

Shader::Shader()
    : code(""), filePath(""), shaderID(0), shaderType(ShaderType::None) {
}

Shader::Shader(const Shader& shader)
    : code(shader.code), filePath(shader.filePath), shaderID(shader.shaderID),
    shaderType(shader.shaderType) {
}

Shader::Shader(Shader&& shader) noexcept
    : code(std::move(shader.code)), filePath(std::move(shader.filePath)),
    shaderID(shader.shaderID), shaderType(std::move(shader.shaderType)) {
}

Shader& Shader::operator=(const Shader& shader) {
    code = shader.code;
    filePath = shader.filePath;
    shaderID = shader.shaderID;
    shaderType = shader.shaderType;
    return *this;
}

std::string Shader::readFile(const std::string& path) {
    std::ifstream file(path);
    std::string line = "", code = "";
    if (file.is_open()) {
        while (getline(file, line)) code += line + "\n";
        file.close();
    }
    return code;
}

void Shader::compileShader() {
    // Create shader
    std::string debugShader = "";
    switch (shaderType) {
    case ShaderType::Vertex:
        shaderID = glCreateShader(GL_VERTEX_SHADER);
        debugShader = "Vertex";
        break;
    case ShaderType::Fragment:
        shaderID = glCreateShader(GL_FRAGMENT_SHADER);
        debugShader = "Fragment";
        break;
    default:
        debugShader = "Not implemented yet";
        break;
    }
    const char* chrCode = code.c_str();
    glShaderSource(shaderID, 1, &chrCode, NULL);
    glCompileShader(shaderID);
    // Show error if any
    int success;
    char infoLog[512];
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
        std::cout << debugShader << " shader compilation error: " << infoLog << std::endl;
    }
}

ShaderProgram::ShaderProgram(const Shader& _vertexShader, const Shader& _fragmentShader)
    : vertexShader(_vertexShader), fragmentShader(_fragmentShader), shaderProgramID(0) {
    link();
}

ShaderProgram::ShaderProgram() : shaderProgramID(0) {}

ShaderProgram::ShaderProgram(const ShaderProgram& shaderProgram)
    : vertexShader(shaderProgram.vertexShader), fragmentShader(shaderProgram.fragmentShader),
    shaderProgramID(shaderProgram.shaderProgramID) {
}

ShaderProgram::ShaderProgram(ShaderProgram&& shaderProgram) noexcept
    : vertexShader(std::move(shaderProgram.vertexShader)), fragmentShader(std::move(shaderProgram.fragmentShader)),
    shaderProgramID(shaderProgram.shaderProgramID) {
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(shaderProgramID);
}

ShaderProgram& ShaderProgram::operator=(const ShaderProgram& shaderProgram) {
    vertexShader = shaderProgram.vertexShader;
    fragmentShader = shaderProgram.fragmentShader;
    shaderProgramID = shaderProgram.shaderProgramID;
    return *this;
}

void ShaderProgram::link() {
    // Link program
    shaderProgramID = glCreateProgram();
    glAttachShader(shaderProgramID, vertexShader.getShaderID());
    glAttachShader(shaderProgramID, fragmentShader.getShaderID());
    glLinkProgram(shaderProgramID);
    // Check for linking errors
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgramID, 512, NULL, infoLog);
        std::cout << "Couldn't link shaders\n" << infoLog << std::endl;
    }
    // Delete shaders
    vertexShader.deleteShader();
    fragmentShader.deleteShader();
}

void ShaderProgram::uniformInt(const std::string& uniform, int value) {
    int location = glGetUniformLocation(shaderProgramID, uniform.c_str());
    glUniform1i(location, value);
}

void ShaderProgram::uniformFloat(const std::string& uniform, float value) {
    int location = glGetUniformLocation(shaderProgramID, uniform.c_str());
    glUniform1f(location, value);
}

void ShaderProgram::uniformVec3(const std::string& uniform, const Vector3f& vec) {
    int location = glGetUniformLocation(shaderProgramID, uniform.c_str());
    glUniform3fv(location, 1, vec.data());
}

void ShaderProgram::uniformMat4(const std::string& uniform, const Matrix4x4f& mat) {
    int location = glGetUniformLocation(shaderProgramID, uniform.c_str());
    glUniformMatrix4fv(location, 1, GL_TRUE, mat.data());
}

void ShaderProgram::uniformTextureArray(const std::string& uniform, std::vector<int>& textures) {
    int location = glGetUniformLocation(shaderProgramID, uniform.c_str());
    glUniform1iv(location, textures.size(), &textures[0]);
}

ShaderProgram::Ptr getDefaultShaderProgram() {
    Shader vertexShader = Shader::fromCode(vertexShaderSource, Shader::ShaderType::Vertex);
    Shader fragmentShader = Shader::fromCode(fragmentShaderSource, Shader::ShaderType::Fragment);
    return ShaderProgram::New(vertexShader, fragmentShader);
}

ShaderProgram::Ptr getShaderProgramFromPointCloud() {
    ShaderProgram::Ptr shaderProgram = nullptr;
    return shaderProgram;
}

}