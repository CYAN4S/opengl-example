#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.149f , 0.749f, 0.702f, 1.0f);\n"
"}\n\0";

int main()
{
    // GLFW �ʱ�ȭ �� ����
    if (!glfwInit())
    {
        std::cout << "GLFW �ʱ�ȭ ����!" << std::endl;
        exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // GLFW ������ ����
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Homework #1-1", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "GLFW ������ ���� ����!" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }


    // OpenGL Context creation
    // ���� ����� ������ ����
    glfwMakeContextCurrent(window);


    // Set Callback
    // �̺�Ʈ ��� ���α׷��� -> �ݹ� �Լ� ��� -> �̺�Ʈ �߻��� �ݹ� �Լ� ����
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    // Loader library
    // GL �Լ� ����
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "GLAD �ʱ�ȭ ����!" << std::endl;
        exit(EXIT_FAILURE);
    }


    // �⺻������ ���� ���۸��� ���(���۸� 2�� ���)
    // ����� refresh rate�� ����
    glfwSwapInterval(1);


    // ���̴� ���α׷� ����
    // ���� �ð��� ���� ������

    // Vertex�� ����
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    // Fragment�� ����
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }


    // Shader Program ��ü
    // ���������� ����
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // ���̴� ����
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Vertex �� ��ġ ����
    float vertices[] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };

    // EBO ���
    // ��ü�� ���� ���� ��ȣ ������ ǥ��
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO); // Array ����
    glGenBuffers(1, &VBO); // VBO ���� ����
    glGenBuffers(1, &EBO); // EBO ���� ����

    glBindVertexArray(VAO); // VAO ���ε�

    
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // ���� ���� ����
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // GPU�� ����

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // ���� ���� ����
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // GPU�� ����


    // Linking Vertex Attributes
    // Vertex �Ӽ�
    // ���� ��ġ, �� vertex�� ũ��, ������ Ÿ��, ����ȭ �ʿ� ����, stride(����), offset
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);


    // #1-2 ����(���̾�������) �� �ּ� ����
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    // ����
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        // ����
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // �ﰢ�� �׸���
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        glfwSwapBuffers(window);
        glfwPollEvents(); // �̺�Ʈ ó��
    }

    // ���� ����
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    // GLFW ����
    glfwTerminate();
    return 0;
}


void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}