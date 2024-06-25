#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <learnopengl/shader_s.h>
#define STB_IMAGE_IMPLEMENTATION 
#include <learnopengl/stb_image.h>
#include <iostream>

// Declaración de funciones de devolución de llamada y de procesamiento de entrada
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// Configuración
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    // Inicialización y configuración de GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Versión mayor de OpenGL 3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // Versión menor de OpenGL 3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Para Mac OS X
#endif

    // Creación de la ventana GLFW
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Exercise 10 Task 5", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Inicialización de GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Compilación de nuestro programa de shaders
    Shader ourShader("shaders/shader_exercise10.vs", "shaders/shader_exercise10.fs");

    // Configuración de datos de vértices y búferes
    float vertices[] = {
        // posiciones          // coordenadas de textura
         0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // arriba derecha
         0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // abajo derecha
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // abajo izquierda
        -0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // arriba izquierda
    };
    unsigned int indices[] = {
        0, 1, 3, // primer triángulo
        1, 2, 3  // segundo triángulo
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Atributo de posición
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Atributo de coordenadas de textura
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Cargar y crear texturas
    unsigned int texture1, texture2;
    // Textura 1
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // Configuración de parámetros de envoltura y filtrado de textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Cargar imagen, crear textura y generar mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // Indicar a stb_image que voltee la textura cargada en el eje Y
    unsigned char* data = stbi_load("textures/container.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
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
    // Configuración de parámetros de envoltura y filtrado de textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Cargar imagen, crear textura y generar mipmaps
    data = stbi_load("textures/awesomeface.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        // Nota: la imagen awesomeface.png tiene transparencia, por lo tanto tiene un canal alfa, se debe indicar a OpenGL que el tipo de datos es GL_RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // Decirle a OpenGL a qué unidad de textura pertenece cada sampler (solo se debe hacer una vez)
    ourShader.use();
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);

    // Bucle de renderizado
    while (!glfwWindowShouldClose(window))
    {
        // Procesamiento de entrada
        processInput(window);

        // Renderizado
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Enlazar texturas a las unidades de textura correspondientes
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        // Crear transformaciones
        glm::mat4 transform = glm::mat4(1.0f); // Inicializar la matriz a la matriz identidad
        transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
        transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

        // Obtener la ubicación de la matriz uniforme y establecer la matriz
        ourShader.use();
        unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

        // Renderizar el contenedor
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Resetear la matriz de transformación y aplicar escalado
        transform = glm::mat4(1.0f); // Reiniciar la matriz de transformación
        transform = glm::translate(transform, glm::vec3(-0.5f, 0.5f, 0.0f));
        float scale = sin(glfwGetTime());
        transform = glm::scale(transform, glm::vec3(scale, scale, scale));

        // Establecer la matriz de transformación
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &transform[0][0]);

        // Renderizar el segundo contenedor
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Intercambiar los buffers y comprobar eventos de entrada/salida
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // De-allocate todos los recursos
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // Terminar GLFW y limpiar todos los recursos asignados
    glfwTerminate();
    return 0;
}

// Procesar toda la entrada: consultar a GLFW si las teclas relevantes están presionadas/liberadas en este cuadro y reaccionar en consecuencia
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// GLFW: cada vez que cambie el tamaño de la ventana (por el SO o el usuario) esta función de devolución de llamada se ejecuta
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // Asegurarse de que el viewport coincida con las nuevas dimensiones de la ventana; nota que el ancho y 
    // la altura serán significativamente mayores de lo especificado en pantallas retina.
    glViewport(0, 0, width, height);
}
