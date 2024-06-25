#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <learnopengl/shader_s.h>
#include <iostream>

// Declaraciones de funciones de devolución de llamada
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// Configuración de la ventana
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    // Inicializar y configurar GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Versión mayor de OpenGL 3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // Versión menor de OpenGL 3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Perfil principal de OpenGL

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Configuración específica para MacOS
#endif

    // Crear la ventana GLFW
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CG Exercise 7 Task 4", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate(); // Terminar GLFW si falla la creación de la ventana
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Cargar todas las funciones de OpenGL con GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Construir y compilar nuestro programa de shaders
    Shader ourShader("shaders/shader_vertex_B1P3.vs", "shaders/shader_fragment_B1P3.fs");

    // Configurar los datos de los vértices y los atributos del vértice
    float vertices[] = {
        // posiciones        // colores
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // abajo derecha
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // abajo izquierda
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // arriba
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Primero, enlazar el VAO, luego el VBO y configurar los atributos del vértice
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Especificar el formato de los datos de los vértices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Especificar el formato de los datos de los colores
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Bucle de renderizado
    while (!glfwWindowShouldClose(window))
    {
        // Procesar la entrada
        processInput(window);

        // Renderizar
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);



        // Renderizar el triángulo
        ourShader.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Intercambiar los buffers y sondear eventos de entrada/salida
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // De-allocate todos los recursos una vez que ya no sean necesarios
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // Terminar GLFW y liberar todos los recursos previamente asignados
    glfwTerminate();
    return 0;
}

// Procesar toda la entrada: consultar a GLFW si las teclas relevantes están presionadas/liberadas en este frame y reaccionar en consecuencia
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true); 
}

// GLFW: cada vez que se cambia el tamaño de la ventana (por el sistema operativo o el usuario) esta función de callback se ejecuta
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height); 
}
