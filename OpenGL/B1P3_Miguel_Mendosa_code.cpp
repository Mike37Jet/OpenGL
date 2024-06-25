#include <glad/glad.h>
#include <GLFW/glfw3.h> 
#include <iostream>
#include <chrono>
#include <learnopengl/shader_s.h>
#include <thread>


// Declaraciones de funciones de devolución de llamada
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// Configuración de la ventana
const unsigned int SCR_WIDTH = 700;
const unsigned int SCR_HEIGHT = 700;

// Variables de desplazamiento
float xOffset = 0.0f;
float yOffset = 0.0f;
float redValue = 0.0f;
float greenValue = 0.0f;
float blueValue = 0.0f;

bool colorOrigin = false;
bool adjustingRed = false;
bool adjustingGreen = false;
bool adjustingBlue = false;

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
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Figura 22", NULL, NULL);
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
	Shader ourShader("shaders/shader_vertex_B1P3.vs", "shaders/shader_fragment_B1P3.fs");

	// Configurar los datos de los vértices y los buffers
	float vertices[] = {
		// Coordenadas de los vértices para cada triángulo
		// posiciones           // colores
		 0.055f, -0.279f, 0.0f, 0.99f, 0.8f, 0.007f,  // izquierda 1
		 0.89f, -0.334f, 0.0f,  0.99f, 0.8f, 0.007f,  // derecha 1
		 0.49f, 0.1f, 0.0f,     0.99f, 0.8f, 0.007f,  // arriba 1

		-0.91f, -0.21f, 0.0f,   0.18f, 0.596f, 1.0f, // izquierda 2
		-0.48f, -0.24f, 0.0f,   0.18f, 0.596f, 1.0f, // derecha 2
		-0.71f, -0.45f, 0.0f,   0.18f, 0.596f, 1.0f, // arriba 2

		-0.37f, -0.245f, 0.0f,  0.2f, 0.796f, 0.4f,  // izquierda 3
		 0.48f, -0.307f, 0.0f,  0.2f, 0.796f, 0.4f,  // derecha 3
		 0.02f, -0.71f, 0.0f,   0.2f, 0.796f, 0.4f,  // arriba 3

		-0.63f, -0.23f, 0.0f,   0.596f, 0.795f, 0.21f, // izquierda 4
		-0.04f, -0.27f, 0.0f,   0.596f, 0.795f, 0.21f, // derecha 4
		-0.32f, 0.028f, 0.0f,   0.596f, 0.795f, 0.21f, // arriba 4

		 0.19f, 0.25f, 0.0f,    1.0f, 0.4f, 0.4f,    // izquierda 5A
		 0.63f, 0.21f, 0.0f,    1.0f, 0.4f, 0.4f,    // derecha 5A
		 0.4f, 0.016f, 0.0f,    1.0f, 0.4f, 0.4f,    // arriba 5A

		 0.19f, 0.25f, 0.0f,    1.0f, 0.4f, 0.4f,    // izquierda 5B
		 0.63f, 0.21f, 0.0f,    1.0f, 0.4f, 0.4f,    // derecha 5B
		 0.43f, 0.45f, 0.0f,    1.0f, 0.4f, 0.4f,    // arriba 5B

		-0.29f, 0.47f, 0.0f,    0.4f, 0.39f, 0.81f,  // izquierda 6A
		-0.32f, 0.028f, 0.0f,   0.4f, 0.39f, 0.81f,  // derecha 6A
		-0.09f, 0.23f, 0.0f,    0.4f, 0.39f, 0.81f,  // arriba 6A

		-0.09f, 0.23f, 0.0f,    0.4f, 0.39f, 0.81f,  // izquierda 6B
		-0.108f, -0.2f, 0.0f,   0.4f, 0.39f, 0.81f,  // derecha 6B
		-0.32f, 0.028f, 0.0f,   0.4f, 0.39f, 0.81f,  // arriba 6B

		-0.3f, 0.27f, 0.0f,     0.98f, 0.6f, 0.22f,  // izquierda 7
		-0.27f, 0.69f, 0.0f,    0.98f, 0.6f, 0.22f,  // derecha 7
		-0.49f, 0.49f, 0.0f,    0.98f, 0.6f, 0.22f  // arriba 7
	};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Configurar el VAO y el VBO
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Especificar el formato de los datos de los vértices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Especificar el formato de los datos de los colores
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

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

		// Renderizar el triángulo
		ourShader.use();
		ourShader.setBool("colorOrigin", colorOrigin);

		glUniform2f(glGetUniformLocation(ourShader.ID, "offset"), xOffset, yOffset);
		glUniform3f(glGetUniformLocation(ourShader.ID, "colorOffset"), redValue, greenValue, blueValue);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glUniform2f(glGetUniformLocation(ourShader.ID, "offset"), xOffset, yOffset);
		glUniform3f(glGetUniformLocation(ourShader.ID, "colorOffset"), redValue, greenValue, blueValue);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 3, 3);

		glUniform2f(glGetUniformLocation(ourShader.ID, "offset"), xOffset, yOffset);
		glUniform3f(glGetUniformLocation(ourShader.ID, "colorOffset"), redValue, greenValue, blueValue);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 6, 3);

		glUniform2f(glGetUniformLocation(ourShader.ID, "offset"), xOffset, yOffset);
		glUniform3f(glGetUniformLocation(ourShader.ID, "colorOffset"), redValue, greenValue, blueValue);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 9, 3);

		glUniform2f(glGetUniformLocation(ourShader.ID, "offset"), xOffset, yOffset);
		glUniform3f(glGetUniformLocation(ourShader.ID, "colorOffset"), redValue, greenValue, blueValue);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 12, 6);

		glUniform2f(glGetUniformLocation(ourShader.ID, "offset"), xOffset, yOffset);
		glUniform3f(glGetUniformLocation(ourShader.ID, "colorOffset"), redValue, greenValue, blueValue);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 18, 6);

		glUniform2f(glGetUniformLocation(ourShader.ID, "offset"), xOffset, yOffset);
		glUniform3f(glGetUniformLocation(ourShader.ID, "colorOffset"), redValue, greenValue, blueValue);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 24, 3);

		// Retraso
		std::this_thread::sleep_for(std::chrono::milliseconds(1)); // 100 ms = 0.1 segundos

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

	// Mover la figura con las flechas del teclado
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		xOffset = std::max(-0.1f, xOffset - 0.02f);
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		xOffset = std::min(0.1f, xOffset + 0.02f);
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		yOffset = std::min(0.3f, yOffset + 0.02f);
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		yOffset = std::max(-0.25f, yOffset - 0.02f);

	static bool adjustingRed = false;
	static bool adjustingGreen = false;
	static bool adjustingBlue = false;

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		colorOrigin = false;
		adjustingRed = true;
		adjustingGreen = false;
		adjustingBlue = false;
	}
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
		colorOrigin = false;
		adjustingRed = false;
		adjustingGreen = true;
		adjustingBlue = false;
	}
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
		colorOrigin = false;
		adjustingRed = false;
		adjustingGreen = false;
		adjustingBlue = true;
	}

	if (adjustingRed) {
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			redValue = std::min(1.0f, redValue + 0.02f);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			redValue = std::max(-1.0f, redValue - 0.02f);
	}
	else if (adjustingGreen) {
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			greenValue = std::min(1.0f, greenValue + 0.02f);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			greenValue = std::max(-1.0f, greenValue - 0.02f);
	}
	else if (adjustingBlue) {
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			blueValue = std::min(1.0f, blueValue + 0.02f);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			blueValue = std::max(-1.0f, blueValue - 0.02f);
	}

	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
		colorOrigin = true;
		redValue = 0.0f;
		greenValue = 0.0f;
		blueValue = 0.0f;
	}
}

// Función de devolución de llamada para ajustar el tamaño del viewport
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
