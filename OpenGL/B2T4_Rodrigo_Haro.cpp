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

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char* path);

const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 800;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::vec3 lightPos(0.4f, 1.0f, -2.0f);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Rodrigo Haro 1600830705", NULL, NULL);
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

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    Shader lightingShader("shaders/shader_exercise14t2_materials.vs", "shaders/shader_exercise14t2_materials.fs");
    Shader lightCubeShader("shaders/shader_exercise14_lightcube.vs", "shaders/shader_exercise14_lightcube.fs");

    float verticesHomero[] = {
        // posicion          // text coords1    // text coords2     // text coords3     // normals
        // atras
        -0.5f, -0.5f, -0.5f,  0.347f, 0.5049f,  0.3398f, 0.5024f,   0.2295f, 0.3259f,  0.0f,  0.0f, -1.0f,
        0.5f, -0.5f, -0.5f,  0.1446f, 0.5049f, 0.1474f, 0.5024f,   0.0361f, 0.3259f,   0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.1446f, 0.684f,  0.1474f, 0.6691f,   0.0361f, 0.4950f,   0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.1446f, 0.684f,  0.1474f, 0.6691f,   0.0361f, 0.4950f,   0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.347f, 0.684f,   0.3398f, 0.6691f,   0.2295f, 0.4950f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.347f, 0.5049f,  0.3398f, 0.5024f,   0.2295f, 0.3259f,  0.0f,  0.0f, -1.0f,
        // frente
        -0.5f, -0.5f,  0.5f,  0.553f, 0.5049f,  0.5395f, 0.5024f,   0.4282f, 0.3259f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f,  0.5f,  0.756f, 0.5049f,  0.7285f, 0.5024f,   0.6220f, 0.3259f,   0.0f,  0.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.756f, 0.684f,   0.7285f, 0.6691f,   0.6220f, 0.4950f,   0.0f,  0.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.756f, 0.684f,   0.7285f, 0.6691f,   0.6220f, 0.4950f,   0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.553f, 0.684f,   0.5395f, 0.6691f,   0.4282f, 0.4950f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.553f, 0.5049f,  0.5395f, 0.5024f,   0.4282f, 0.3259f,  0.0f,  0.0f,  1.0f,
        //izquierda
        -0.5f,  0.5f,  0.5f,  0.5513f, 0.684f,   0.5346f, 0.6691f,  0.4248f, 0.4950f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.3509f, 0.684f,   0.3452f, 0.6691f,  0.2319f, 0.4950f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.3509f, 0.5049f,  0.3452f, 0.5024f,  0.2319f, 0.3259f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.3509f, 0.5049f,  0.3452f, 0.5024f,  0.2319f, 0.3259f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.5513f, 0.5049f,  0.5346f, 0.5024f,  0.4248f, 0.3259f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.5513f, 0.684f,   0.5346f, 0.6691f,  0.4248f, 0.4950f, -1.0f,  0.0f,  0.0f,
        //derecha
        0.5f,  0.5f,  0.5f,  0.759f, 0.684f,    0.73388f, 0.6691f,  0.6245f, 0.4950f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.9594f, 0.684f,   0.9233f, 0.6691f,   0.8188f, 0.4950f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.9594f, 0.5049f, 0.9233f, 0.5024f,    0.8188f, 0.3259f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.9594f, 0.5049f, 0.9233f, 0.5024f,    0.8188f, 0.3259f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.759f, 0.5049f,  0.73388f, 0.5024f,   0.6245f, 0.3259f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.759f, 0.684f,    0.73388f, 0.6691f,  0.6245f, 0.4950f,  1.0f,  0.0f,  0.0f,
        //abajo
        -0.5f, -0.5f, -0.5f,  0.553f, 0.2481,  0.5381f, 0.2598f,   0.4282f, 0.0784f,   0.0f, -1.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.759f, 0.2481,  0.7290f, 0.2598f,   0.6220f, 0.0784f,    0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.759f, 0.502f,  0.7290f, 0.4969f,   0.6220f, 0.3191f,    0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.759f, 0.502f,  0.7290f, 0.4969f,   0.6220f, 0.3191f,    0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.553f, 0.502f,  0.5381f, 0.4969f,   0.4282f, 0.3191f,   0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.553f, 0.2481,  0.5381f, 0.2598f,   0.4282f, 0.0784f,   0.0f, -1.0f,  0.0f,
        //arriba
        -0.5f,  0.5f, -0.5f,  0.553f, 0.9425f, 0.5381f, 0.9154f,   0.4282f, 0.7432f,   0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.759f, 0.9425f, 0.7290f, 0.9154f,   0.6220f, 0.7432f,    0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.759f, 0.688f,  0.7290f, 0.6753f,   0.6220f, 0.5f,       0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.759f, 0.688f,  0.7290f, 0.6753f,   0.6220f, 0.5f,       0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.553f, 0.688f,  0.5381f, 0.6753f,   0.4282f, 0.5f,      0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.553f, 0.9425f,  0.5381f, 0.9154f,   0.4282f, 0.7432f,  0.0f,  1.0f,  0.0f
    };

    // world space positions of our cubes
    glm::vec3 cubePositions[] = {
        glm::vec3(0.7f,  0.0f,  -2.2f),
        glm::vec3(-0.7f, 0.0f,  -4.0f),
        glm::vec3(1.5f,  -0.5f,  -5.0f),
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesHomero), verticesHomero, GL_STATIC_DRAW);


    // atributo de posicion
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // atributo de textura 1
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // atributo de textura 2
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);
    // atributo de textura 3
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(7 * sizeof(float)));
    glEnableVertexAttribArray(3);
    // atributo de normales
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(9 * sizeof(float)));
    glEnableVertexAttribArray(4);

    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // diffuse
    unsigned int texture1_diffuse = loadTexture("textures/Texture5.png");
    unsigned int texture2_diffuse = loadTexture("textures/Texture6.png");
    unsigned int texture3_diffuse = loadTexture("textures/spiderman.png");

    // specular
    unsigned int texture1_specular = loadTexture("textures/Texture7_specular.png");
    unsigned int texture2_specular = loadTexture("textures/panda_specular.png");
    unsigned int texture3_specular = loadTexture("textures/pelogris_specular.jpg");

    lightingShader.use();

    lightingShader.setInt("material.diffuse", 0);
    lightingShader.setInt("material.specular", 1);

    glActiveTexture(GL_TEXTURE0);

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float translateX = sin(glfwGetTime());
        float translateZ = sin(2 * glfwGetTime()) * 2;

        glm::vec3 lightPosition = glm::vec3(lightPos.x + translateX, lightPos.y, lightPos.z + translateZ);

        lightingShader.use();
        lightingShader.setVec3("light.position", lightPosition);
        lightingShader.setVec3("viewPos", camera.Position);

        lightingShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
        lightingShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
        lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

        lightingShader.setFloat("material.shininess", 64.0f);

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[0]);
        lightingShader.setMat4("model", model);

        lightingShader.setInt("numTexCoord", 1);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1_diffuse);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture1_specular);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[1]);
        lightingShader.setMat4("model", model);

        lightingShader.setInt("numTexCoord", 2);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture2_diffuse);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2_specular);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[2]);
        lightingShader.setMat4("model", model);

        lightingShader.setInt("numTexCoord", 3);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture3_diffuse);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture3_specular);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        lightCubeShader.use();
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::translate(model, glm::vec3(translateX, 0.0f, translateZ));
        model = glm::scale(model, glm::vec3(0.5f));
        lightCubeShader.setMat4("model", model);

        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &lightCubeVAO);

    glfwTerminate();
    return 0;
}

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

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}