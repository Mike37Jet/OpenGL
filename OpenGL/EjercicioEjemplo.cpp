#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 700;
const unsigned int SCR_HEIGHT = 700;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

// Fragment shader sources
const char* fragmentShaderSourceAmarillo = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.99f, 0.8f, 0.007f, 1.0f);\n"
"}\n\0";

const char* fragmentShaderSourceAzul = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.18f, 0.596f, 1.0f, 1.0f);\n"
"}\n\0";

const char* fragmentShaderSourceVerdeO = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.2f, 0.796f, 0.4f, 1.0f);\n"
"}\n\0";

const char* fragmentShaderSourceVerdeC = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.596f, 0.795f, 0.21f, 1.0f);\n"
"}\n\0";

const char* fragmentShaderSourceRojo = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.4f, 0.4f, 1.0f);\n"
"}\n\0";

const char* fragmentShaderSourceMorado = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.4f, 0.39f, 0.81f, 1.0f);\n"
"}\n\0";

const char* fragmentShaderSourceNaranja = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.98f, 0.6f, 0.22f, 1.0f);\n"
"}\n\0";

unsigned int createShaderProgram(const char* vertexSource, const char* fragmentSource) {
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

int main() {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader programs
    unsigned int shaderProgramAmarillo = createShaderProgram(vertexShaderSource, fragmentShaderSourceAmarillo);
    unsigned int shaderProgramAzul = createShaderProgram(vertexShaderSource, fragmentShaderSourceAzul);
    unsigned int shaderProgramVerdeO = createShaderProgram(vertexShaderSource, fragmentShaderSourceVerdeO);
    unsigned int shaderProgramVerdeC = createShaderProgram(vertexShaderSource, fragmentShaderSourceVerdeC);
    unsigned int shaderProgramRojo = createShaderProgram(vertexShaderSource, fragmentShaderSourceRojo);
    unsigned int shaderProgramMorado = createShaderProgram(vertexShaderSource, fragmentShaderSourceMorado);
    unsigned int shaderProgramNaranja = createShaderProgram(vertexShaderSource, fragmentShaderSourceNaranja);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        0.055f, -0.279f, 0.0f, // left  1
        0.89f, -0.334f, 0.0f, // right 1
        0.49f, 0.1f, 0.0f,  // top  1
        -0.91f, -0.21f, 0.0f, // left  2
        -0.48f, -0.24f, 0.0f, // right 2
        -0.71f, -0.45f, 0.0f,  // top  2
        -0.37f, -0.245f, 0.0f, // left  3
        0.48f, -0.307f, 0.0f, // right 3
        0.02f, -0.71f, 0.0f,  // top  3
        -0.63f, -0.23f, 0.0f, // left  4
        -0.04f, -0.27f, 0.0f, // right 4
        -0.32f, 0.028f, 0.0f,  // top  4
        0.19f, 0.25f, 0.0f, // left  5A
        0.63f, 0.21f, 0.0f, // right 5A
        0.4f, 0.016f, 0.0f,  // top  5A
        0.19f, 0.25f, 0.0f, // left  5B
        0.63f, 0.21f, 0.0f, // right 5B
        0.43f, 0.45f, 0.0f,  // top  5B
        -0.29f, 0.47f, 0.0f, // left  6A
        -0.32f, 0.028f, 0.0f, // right 6A
        -0.09f, 0.23f, 0.0f,  // top  6A
        -0.09f, 0.23f, 0.0f, // left  6B
        -0.108f, -0.2f, 0.0f, // right 6B
        -0.32f, 0.028f, 0.0f,  // top  6B
        -0.3f, 0.27f, 0.0f, // left  7
        -0.27f, 0.69f, 0.0f, // right 7
        -0.49f, 0.49f, 0.0f,  // top  7
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // render loop
    while (!glfwWindowShouldClose(window)) {
        // input
        processInput(window);

        // render
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle with different colors
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

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
