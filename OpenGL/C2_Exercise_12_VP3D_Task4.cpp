#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/shader.h>
#include <learnopengl/camera.h>

#define STB_IMAGE_IMPLEMENTATION 
#include <learnopengl/stb_image.h>

#include <iostream>

// Funciones de callback y de manejo de entrada
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// Configuración de la ventana
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Cámara
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f)); // Inicializar la cámara en el espacio
float lastX = SCR_WIDTH / 2.0f; // Posición del mouse en X
float lastY = SCR_HEIGHT / 2.0f; // Posición del mouse en Y
bool firstMouse = true; // Bandera para controlar la primera vez que se mueve el mouse

// Tiempo
float deltaTime = 0.0f;	// Tiempo entre el frame actual y el último frame
float lastFrame = 0.0f;

int main()
{
    // Inicialización y configuración de GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Creación de la ventana GLFW
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Capturar el mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Cargar los punteros de funciones de OpenGL con GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Configuración global de OpenGL
    glEnable(GL_DEPTH_TEST);

    // Compilación y enlace de los shaders
    Shader ourShader("shaders/shader_exercise12.vs", "shaders/shader_exercise12.fs");

    // Definición de los datos de los vértices y configuración de los atributos del vértice
    float vertices[] = {
        // Posiciones       // Coordenadas de textura
        -0.5f, -0.5f, -0.5f,  0.551f, 0.508f,
         0.5f, -0.5f, -0.5f,  0.754f, 0.508f,
         0.5f,  0.5f, -0.5f,  0.754f, 0.684f, //Cara frontal
         0.5f,  0.5f, -0.5f,  0.754f, 0.684f,
        -0.5f,  0.5f, -0.5f,  0.552f, 0.684f,
        -0.5f, -0.5f, -0.5f,  0.551f, 0.508f,

        -0.5f, -0.5f,  0.5f,  0.147f, 0.506f,
         0.5f, -0.5f,  0.5f,  0.350f, 0.506f,
         0.5f,  0.5f,  0.5f,  0.350f, 0.645f, //Cara trasera
         0.5f,  0.5f,  0.5f,  0.350f, 0.645f,
        -0.5f,  0.5f,  0.5f,  0.147f, 0.645f,
        -0.5f, -0.5f,  0.5f,  0.147f, 0.504f,

        -0.5f, -0.5f, -0.5f,  0.754f, 0.508f,
        -0.5f, -0.5f,  0.5f,  0.956f, 0.508f,
        -0.5f,  0.5f,  0.5f,  0.956f, 0.684f, //Cara izquierda del personaje
        -0.5f,  0.5f,  0.5f,  0.956f, 0.684f,
        -0.5f,  0.5f, -0.5f,  0.754f, 0.684f,
        -0.5f, -0.5f, -0.5f,  0.754f, 0.508f,

         0.5f, -0.5f,  0.5f,  0.350f, 0.508f,
         0.5f, -0.5f, -0.5f,  0.552f, 0.508f,
         0.5f,  0.5f, -0.5f,  0.552f, 0.684f, //Cara derecha del personaje
         0.5f,  0.5f, -0.5f,  0.552f, 0.684f,
         0.5f,  0.5f,  0.5f,  0.350f, 0.684f,
         0.5f, -0.5f,  0.5f,  0.350f, 0.508f,

        -0.5f, -0.5f,  0.5f,  0.551f, 0.301f,
         0.5f, -0.5f,  0.5f,  0.754f, 0.301f,
         0.5f, -0.5f, -0.5f,  0.754f, 0.508f, //Cara inferior
         0.5f, -0.5f, -0.5f,  0.754f, 0.508f,
        -0.5f, -0.5f, -0.5f,  0.551f, 0.508f,
        -0.5f, -0.5f,  0.5f,  0.551f, 0.301f,

        -0.5f,  0.5f, -0.5f,  0.552f, 0.684f,
         0.5f,  0.5f, -0.5f,  0.754f, 0.684f,
         0.5f,  0.5f,  0.5f,  0.754f, 0.847f, //Cara superior
         0.5f,  0.5f,  0.5f,  0.754f, 0.847f,
        -0.5f,  0.5f,  0.5f,  0.552f, 0.847f,
        -0.5f,  0.5f, -0.5f,  0.552f, 0.684f
    };

    // Posiciones de los cubos en el espacio
    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,   0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Atributo de posición
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Atributo de coordenada de textura
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Carga y creación de texturas
    unsigned int texture1, texture2;
    // Textura 1
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // Configuración de los parámetros de wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Configuración de los parámetros de filtrado
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Carga de la imagen, creación de la textura y generación de mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // Voltear la textura en el eje Y
    unsigned char* data = stbi_load("textures/Texture6.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // Textura 2
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // Configuración de los parámetros de wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Configuración de los parámetros de filtrado
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Carga de la imagen, creación de la textura y generación de mipmaps
    data = stbi_load("textures/awesomeface.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // Enlazar las texturas a los samplers del shader
    ourShader.use();
    glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
    glUniform1i(glGetUniformLocation(ourShader.ID, "texture2"), 1);

    // Bucle de renderizado
    while (!glfwWindowShouldClose(window))
    {
        // Lógica del tiempo por frame
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Entrada del usuario
        processInput(window);

        // Renderizado
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Enlazar las texturas a las unidades de textura correspondientes
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        // Activar el shader
        ourShader.use();

        // Crear las transformaciones
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        ourShader.setMat4("view", view);
        ourShader.setMat4("projection", projection);

        // Renderizar los cubos
        glBindVertexArray(VAO);
        for (unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            if (i % 3 == 0) // Rotar solo cada tercer cubo
            {
                angle = glfwGetTime() * 25.0f;
            }
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            ourShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // Intercambiar buffers y sondear eventos de E/S
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // De-allocar todos los recursos
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // Terminar GLFW
    glfwTerminate();
    return 0;
}

// Procesar toda la entrada: consultar GLFW si las teclas relevantes fueron presionadas/liberadas en este frame
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// Callback cuando el tamaño de la ventana cambia
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // Asegurarse de que el viewport coincide con las nuevas dimensiones de la ventana
    glViewport(0, 0, width, height);
}

// Callback cuando el mouse se mueve
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // Invertir ya que las coordenadas Y van de abajo hacia arriba

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// Callback cuando la rueda del mouse se desplaza
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}
