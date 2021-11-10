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
    // GLFW 초기화 및 설정
    if (!glfwInit())
    {
        std::cout << "GLFW 초기화 실패!" << std::endl;
        exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // GLFW 윈도우 생성
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Homework #1-1", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "GLFW 윈도우 생성 실패!" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }


    // OpenGL Context creation
    // 현재 사용할 윈도우 지정
    glfwMakeContextCurrent(window);


    // Set Callback
    // 이벤트 기반 프로그래밍 -> 콜백 함수 등록 -> 이벤트 발생시 콜백 함수 실행
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    // Loader library
    // GL 함수 연결
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "GLAD 초기화 실패!" << std::endl;
        exit(EXIT_FAILURE);
    }


    // 기본적으로 더블 버퍼링을 사용(버퍼를 2개 사용)
    // 모니터 refresh rate에 맞춤
    glfwSwapInterval(1);


    // 셰이더 프로그램 빌드
    // 실행 시간에 동적 컴파일

    // Vertex에 대해
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
    
    // Fragment에 대해
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


    // Shader Program 객체
    // 파이프라인 구성
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

    // 셰이더 삭제
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Vertex 별 위치 정보
    float vertices[] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };

    // EBO 사용
    // 객체별 정점 고유 번호 순으로 표현
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO); // Array 생성
    glGenBuffers(1, &VBO); // VBO 버퍼 생성
    glGenBuffers(1, &EBO); // EBO 버퍼 생성

    glBindVertexArray(VAO); // VAO 바인딩

    
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // 버퍼 형태 설정
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // GPU로 복사

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // 버퍼 형태 설정
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // GPU로 복사


    // Linking Vertex Attributes
    // Vertex 속성
    // 전달 위치, 한 vertex의 크기, 데이터 타입, 정규화 필요 여부, stride(간격), offset
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);


    // #1-2 구현(와이어프레임) 시 주석 해제
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    // 루프
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        // 렌더
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // 삼각형 그리기
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        glfwSwapBuffers(window);
        glfwPollEvents(); // 이벤트 처리
    }

    // 버퍼 해제
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    // GLFW 종료
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