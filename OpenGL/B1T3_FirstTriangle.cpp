#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <random>

#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
const float MY_PI = 3.14159265358979323846f;

// Declaracion de funciones callback y funciones de entrada
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
std::vector<float> generarVerticesPoligono(int numVertices, float radius);
void processInput(GLFWwindow* window);
// Definir la resolucion de la ventana
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
// Definir los shaders
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
"   FragColor = vec4(1.0f, 0.7f, 0.2f, 1.0f);\n"
"}\n\0";

int main()
{
// Se inicializa GLFW y se configura la version de OpenGL
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "MIGUEL_2300163678", NULL, NULL);
    if (window == NULL) // Se verifica si se pudo crear la ventana
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); // Se establece la ventana como contexto actual de OpenGL
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // Se configura la funcion de redimensionar la ventana

    // Se carga GLAD para manejar los punteros de las funciones de OpenGL
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Se compilan los vertex shaders
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // Se verifica si la compilacion fue exitosa
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)// Se imprime un mensaje de error si la compilacion falla
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Se compila el fragment shader
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Se crea el programa de shaders
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);// Se adjuntan los vertex shaders al programa
    glAttachShader(shaderProgram, fragmentShader);// Se adjuntan los fragment shaders al programa
    glLinkProgram(shaderProgram);// Se vinculan los shaders

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    // Se eliminan los shaders, para liberar recursos de memoria. 
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Se definen los vertices del triangulo
    int numVertices = 10;
    ; // Cambia este valor para probar con diferentes números de vértices
    float radius = 0.6f; // Radio del polígono
    std::vector<float> vertices = generarVerticesPoligono(numVertices, radius);
    // Se crean los buffers y se enlazan con los vertices
    unsigned int VBO, VAO; //El VBO es el buffer de vertices en GPU, y el VAO es el array de vertices con las configuraciones de los atributos
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // Se enlazan los buffers. El VAO se enlaza con el VBO
    glBindVertexArray(VAO);
    // Se enlaza el buffer con los vertices. El VBO se enlaza con los vertices
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    // Se definen los atributos de los vertices, y se especifica como se deben leer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Se desenlazan los buffers. El VBO se desenlaza de los vertices
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    // Se ejecuta el bucle principal
    while (!glfwWindowShouldClose(window))
    {

        processInput(window);
        glClearColor(0.2f, 0.1f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        /*
        numVertices++;
        float radius = 0.5f; 
        std::vector<float> vertices = generarVerticesPoligono(numVertices, radius);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
        */

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, numVertices);

        
        // Retraso
        std::this_thread::sleep_for(std::chrono::milliseconds(200)); // 1000 ms = 1 segundo
       

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

std::vector<float> generarVerticesPoligono(int numVertices, float radius) {
    std::vector<float> vertices;
    for (int i = 0; i < numVertices; ++i) {
      
        float angle = 2.0f * MY_PI * i / numVertices;
        vertices.push_back(radius * cos(angle));
        vertices.push_back(radius * sin(angle));
        vertices.push_back(0.0f); 
    }
    return vertices;
}

// Funciones de entrada de teclado
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// Funcion para redimensionar la ventana
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}