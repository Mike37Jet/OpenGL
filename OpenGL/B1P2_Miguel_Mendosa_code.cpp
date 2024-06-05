#include <glad/glad.h> // Biblioteca para cargar las funciones de OpenGL
#include <GLFW/glfw3.h> // Biblioteca para crear ventanas y manejar eventos
#include <iostream>
#include <chrono>
#include <thread>

// Declaraciones de funciones de devolución de llamada
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// Configuración de la ventana
const unsigned int SCR_WIDTH = 700;
const unsigned int SCR_HEIGHT = 700;

// Código fuente del shader de vértices
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

// Código fuente de los shaders de fragmentos para diferentes colores

// Amarillo
const char* fragmentShaderSourceAmarillo = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.99f, 0.8f, 0.007f, 1.0f);\n"
"}\n\0";

// Azul
const char* fragmentShaderSourceAzul = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.18f, 0.596f, 1.0f, 1.0f);\n"
"}\n\0";

// Verde Oscuro
const char* fragmentShaderSourceVerdeO = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.2f, 0.796f, 0.4f, 1.0f);\n"
"}\n\0";

// Verde Claro
const char* fragmentShaderSourceVerdeC = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.596f, 0.795f, 0.21f, 1.0f);\n"
"}\n\0";

// Rojo
const char* fragmentShaderSourceRojo = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.4f, 0.4f, 1.0f);\n"
"}\n\0";

// Morado
const char* fragmentShaderSourceMorado = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.4f, 0.39f, 0.81f, 1.0f);\n"
"}\n\0";

// Naranja
const char* fragmentShaderSourceNaranja = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.98f, 0.6f, 0.22f, 1.0f);\n"
"}\n\0";

int main()
{
    // Inicializar y configurar GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Fix para compilación en OS X
#endif

    // Crear la ventana de GLFW
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Cargar todos los punteros de funciones de OpenGL usando GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Centrar la ventana en la pantalla
    int screenWidth, screenHeight;
    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);
    screenWidth = mode->width;
    screenHeight = mode->height;
    glfwSetWindowPos(window, (screenWidth - SCR_WIDTH) / 2, (screenHeight - SCR_HEIGHT) / 2);

    // Construir y compilar nuestro programa de shaders
    // Shader de vértices
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // Verificar errores de compilación del shader
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Crear y compilar los shaders de fragmentos para cada color
    unsigned int fragmentShaderAmarillo = glCreateShader(GL_FRAGMENT_SHADER);
    unsigned int fragmentShaderAzul = glCreateShader(GL_FRAGMENT_SHADER);
    unsigned int fragmentShaderVerdeO = glCreateShader(GL_FRAGMENT_SHADER);
    unsigned int fragmentShaderVerdeC = glCreateShader(GL_FRAGMENT_SHADER);
    unsigned int fragmentShaderRojo = glCreateShader(GL_FRAGMENT_SHADER);
    unsigned int fragmentShaderMorado = glCreateShader(GL_FRAGMENT_SHADER);
    unsigned int fragmentShaderNaranja = glCreateShader(GL_FRAGMENT_SHADER);

    // Shader Amarillo
    glShaderSource(fragmentShaderAmarillo, 1, &fragmentShaderSourceAmarillo, NULL);
    glCompileShader(fragmentShaderAmarillo);

    // Shader Azul
    glShaderSource(fragmentShaderAzul, 1, &fragmentShaderSourceAzul, NULL);
    glCompileShader(fragmentShaderAzul);

    // Shader Verde Oscuro
    glShaderSource(fragmentShaderVerdeO, 1, &fragmentShaderSourceVerdeO, NULL);
    glCompileShader(fragmentShaderVerdeO);

    // Shader Verde Claro
    glShaderSource(fragmentShaderVerdeC, 1, &fragmentShaderSourceVerdeC, NULL);
    glCompileShader(fragmentShaderVerdeC);

    // Shader Rojo
    glShaderSource(fragmentShaderRojo, 1, &fragmentShaderSourceRojo, NULL);
    glCompileShader(fragmentShaderRojo);

    // Shader Morado
    glShaderSource(fragmentShaderMorado, 1, &fragmentShaderSourceMorado, NULL);
    glCompileShader(fragmentShaderMorado);

    // Shader Naranja
    glShaderSource(fragmentShaderNaranja, 1, &fragmentShaderSourceNaranja, NULL);
    glCompileShader(fragmentShaderNaranja);

    // Enlazar los shaders en programas de shader separados para cada color
    int shaderProgramAmarillo = glCreateProgram();
    glAttachShader(shaderProgramAmarillo, vertexShader);
    glAttachShader(shaderProgramAmarillo, fragmentShaderAmarillo);
    glLinkProgram(shaderProgramAmarillo);

    int shaderProgramAzul = glCreateProgram();
    glAttachShader(shaderProgramAzul, vertexShader);
    glAttachShader(shaderProgramAzul, fragmentShaderAzul);
    glLinkProgram(shaderProgramAzul);

    int shaderProgramVerdeO = glCreateProgram();
    glAttachShader(shaderProgramVerdeO, vertexShader);
    glAttachShader(shaderProgramVerdeO, fragmentShaderVerdeO);
    glLinkProgram(shaderProgramVerdeO);

    int shaderProgramVerdeC = glCreateProgram();
    glAttachShader(shaderProgramVerdeC, vertexShader);
    glAttachShader(shaderProgramVerdeC, fragmentShaderVerdeC);
    glLinkProgram(shaderProgramVerdeC);

    int shaderProgramRojo = glCreateProgram();
    glAttachShader(shaderProgramRojo, vertexShader);
    glAttachShader(shaderProgramRojo, fragmentShaderRojo);
    glLinkProgram(shaderProgramRojo);

    int shaderProgramMorado = glCreateProgram();
    glAttachShader(shaderProgramMorado, vertexShader);
    glAttachShader(shaderProgramMorado, fragmentShaderMorado);
    glLinkProgram(shaderProgramMorado);

    int shaderProgramNaranja = glCreateProgram();
    glAttachShader(shaderProgramNaranja, vertexShader);
    glAttachShader(shaderProgramNaranja, fragmentShaderNaranja);
    glLinkProgram(shaderProgramNaranja);

    // Configurar los datos de los vértices y los buffers
    float vertices[] = {
        // Coordenadas de los vértices para cada triángulo
        0.055f, -0.279f, 0.0f, // izquierda 1
        0.89f, -0.334f, 0.0f, // derecha 1
        0.49f, 0.1f, 0.0f,  // arriba 1

        -0.91f, -0.21f, 0.0f, // izquierda 2
        -0.48f, -0.24f, 0.0f, // derecha 2
        -0.71f, -0.45f, 0.0f,  // arriba 2

        -0.37f, -0.245f, 0.0f, // izquierda 3
        0.48f, -0.307f, 0.0f, // derecha 3
        0.02f, -0.71f, 0.0f,  // arriba 3

        -0.63f, -0.23f, 0.0f, // izquierda 4
        -0.04f, -0.27f, 0.0f, // derecha 4
        -0.32f, 0.028f, 0.0f,  // arriba 4

        0.19f, 0.25f, 0.0f, // izquierda 5A
        0.63f, 0.21f, 0.0f, // derecha 5A
        0.4f, 0.016f, 0.0f,  // arriba 5A

        0.19f, 0.25f, 0.0f, // izquierda 5B
        0.63f, 0.21f, 0.0f, // derecha 5B
        0.43f, 0.45f, 0.0f,  // arriba 5B

        -0.29f, 0.47f, 0.0f, // izquierda 6A
        -0.32f, 0.028f, 0.0f, // derecha 6A
        -0.09f, 0.23f, 0.0f,  // arriba 6A

        -0.09f, 0.23f, 0.0f, // izquierda 6B
        -0.108f, -0.2f, 0.0f, // derecha 6B
        -0.32f, 0.028f, 0.0f,  // arriba 6B

        -0.3f, 0.27f, 0.0f, // izquierda 7
        -0.27f, 0.69f, 0.0f, // derecha 7
        -0.49f, 0.49f, 0.0f  // arriba 7
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Configurar el VAO y el VBO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Especificar el formato de los datos de los vértices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Modo de dibujo de los triángulos

    // Bucle de renderizado
    while (!glfwWindowShouldClose(window))
    {
        // Procesar entrada
        processInput(window);

        // Renderizar
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Color de fondo
        glClear(GL_COLOR_BUFFER_BIT);

        // Dibujar los triángulos con diferentes colores

        glUseProgram(shaderProgramAmarillo);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(shaderProgramAzul);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 3, 3);

        glUseProgram(shaderProgramVerdeO);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 6, 3);

        glUseProgram(shaderProgramVerdeC);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 9, 3);

        glUseProgram(shaderProgramRojo);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 12, 6);

        glUseProgram(shaderProgramMorado);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 18, 6);

        glUseProgram(shaderProgramNaranja);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 24, 3);

        // Retraso
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 100 ms = 0.1 segundos

        // Intercambiar los buffers y procesar eventos de entrada/salida
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // De-allocar todos los recursos
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

// Función para procesar la entrada del usuario
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// Función de devolución de llamada para ajustar el tamaño del viewport
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
