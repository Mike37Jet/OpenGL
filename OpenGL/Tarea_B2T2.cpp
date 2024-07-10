#include <glad/glad.h>
#include <GLFW/glfw3.h> 
#include <iostream>
#include <chrono>
#include <learnopengl/shader_s.h>
#include <thread>
#include <learnopengl/shader_s.h>
#define STB_IMAGE_IMPLEMENTATION 
#include <learnopengl/stb_image.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Declaraciones de funciones de devolución de llamada
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// Configuración de la ventana
const unsigned int SCR_WIDTH = 700;
const unsigned int SCR_HEIGHT = 700;

// Variables de desplazamiento
int cambiarTextura[2] = { 0, 1 };

// Variables de posición
float posX = 0.0f;
float posY = 0.0f;
float speedX = 0.002f;
float speedY = 0.002f;

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
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Miguel Mendosa_2300163678", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Falló al crear la ventana de GLFW" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Cargar todos los punteros de funciones de OpenGL usando GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Falló al inicializar GLAD" << std::endl;
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
	Shader ourShader("shaders/vertexshader_B2T2.vs", "shaders/fragmentshader_B2T2.fs");

	// Configurar los datos de los vértices y los buffers
	float vertices[] = {
		// Coordenadas de los vértices para cada triángulo
		// posiciones         // coordenadas de textura
		 0.19f, 0.25f, 0.0f,    0.7f, 0.83f, // izquierda 1A rojo X
		 0.63f, 0.21f, 0.0f,    0.99f, 0.55f, // derecha 1A rojo U
		 0.4f, 0.016f, 0.0f,    0.708f, 0.556f, // arriba 1A rojo T

		 0.19f, 0.25f, 0.0f,    0.7f, 0.83f, // izquierda 1B rojo X
		 0.63f, 0.21f, 0.0f,    0.99f, 0.55f, // derecha 1B rojo U
		 0.43f, 0.45f, 0.0f,    0.996f, 0.84f, // arriba 1B rojo V

		 0.055f, -0.279f, 0.0f, 0.28f, 0.554f, // izquierda 2 amarillo M
		 0.89f, -0.334f, 0.0f,  0.852f, 0.0f, // derecha 2 amarillo B
		 0.49f, 0.1f, 0.0f,     0.82f, 0.556f, // arriba 2 amarillo K

		-0.37f, -0.245f, 0.0f,  0.001f, 0.82f, // izquierda 3 verde oscuro D
		 0.48f, -0.307f, 0.0f,  0.57f, 0.277f, // derecha 3 verde oscuro E
		 0.02f, -0.71f, 0.0f,   0.001f, 0.26f, // arriba 3 verde oscuro F

		-0.91f, -0.21f, 0.0f,   0.0f, 0.57f, // izquierda 4 azul A
		-0.48f, -0.24f, 0.0f,   0.276f, 0.293f, // derecha 4 azul C
		-0.71f, -0.45f, 0.0f,   0.0f, 0.286f, // arriba 4 azul G

		-0.636f, -0.234f, 0.0f,   0.183f, 0.399f, // izquierda 5 verde claro I
		-0.042f, -0.272f, 0.0f,   0.56f, 0.019f, // derecha 5 verde claro H
		-0.32f, 0.029f, 0.0f,     0.546f, 0.38f, // arriba 5 verde claro J

		-0.29f, 0.47f, 0.0f,    0.833f, 0.656f, // izquierda 6A morado O
		-0.32f, 0.029f, 0.0f,   0.546f, 0.38f, // derecha 6A morado J
		-0.09f, 0.236f, 0.0f,   0.826f, 0.38f, // arriba 6A morado N

		-0.09f, 0.236f, 0.0f,   0.826f, 0.38f, // izquierda 6B morado N
		-0.108f, -0.21f, 0.0f,  0.56f, 0.096f, // derecha 6B morado L
		-0.32f, 0.029f, 0.0f,   0.546f, 0.38f, // arriba 6B morado J

		-0.302f, 0.272f, 0.0f,  0.706f, 0.536f, // izquierda 7 naranja P
		-0.274f, 0.694f, 0.0f,  0.98f, 0.803f, // derecha 7 naranja Q
		-0.49f, 0.49f, 0.0f,    0.706f, 0.803f  // arriba 7 naranja R
	};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Configurar el VAO y el VBO
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Especificar el formato de los datos de los vértices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Especificar el formato de los datos de las coordenadas de textura
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	unsigned int texture1, texture2;

	// Generar y configurar la textura 1
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// Configurar los parámetros de envoltura de la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Configurar los parámetros de filtrado de la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// Cargar y generar la textura 1
	int width, height, nrChannels;
	unsigned char* data = stbi_load("textures/texture1.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Falló al cargar la textura 1" << std::endl;
	}
	stbi_image_free(data);

	// Generar y configurar la textura 2
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// Configurar los parámetros de envoltura de la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Configurar los parámetros de filtrado de la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Cargar y generar la textura 2
	data = stbi_load("textures/texture2.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Falló al cargar la textura 2" << std::endl;
	}
	stbi_image_free(data);

	// Decirle a OpenGL cuál es cada shader
	ourShader.use();
	ourShader.setInt("texture1", 0);
	ourShader.setInt("texture2", 1);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::scale(transform, glm::vec3(0.5f, 0.5f, 0.0f));

	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		// Procesar entrada de usuario
		processInput(window);

		// Renderizar
		if (posX >= 0.5f || posX <= -0.5f || posY >= 0.6f || posY <= -0.5f)
		{
			// Cambiar el color de fondo cada vez que haya una colisión
			float r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
			float g = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
			float b = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
			glClearColor(r, g, b, 1.0f);
		}

		glClear(GL_COLOR_BUFFER_BIT);

		// Actualizar posición
		posX += speedX;
		posY += speedY;

		glUniform2f(glGetUniformLocation(ourShader.ID, "offsetPosition"), posX, posY);

		if (posX >= 0.5f || posX <= -0.5f) {
			speedX = -(speedX + (speedX > 0.0f ? 0.001f : 0.0f));

			// Resetear valores de velocidad
			if (speedX > 0.003f) {
				speedX = 0.002f;
			}
			else if (speedX < -0.003f) {
				speedX = -0.002f;
			}

		}

		if (posY >= 0.6f || posY <= -0.5f) {
			speedY = -speedY;

		}


		
		float timeValue = (sin((float)glfwGetTime()) / 8.0f);
		// Crear transformaciones
		glm::mat4 transform = glm::mat4(1.0f);
		transform = glm::translate(transform, glm::vec3(posX, posY, 0.0f));
		transform = glm::rotate(transform, sin((float)glfwGetTime()), glm::vec3(0.0f, 0.0f, 0.1f));
		transform = glm::scale(transform, glm::vec3(0.5f + timeValue, 0.5f + timeValue, 0.0f));



		// Establecer la transformación en el shader
		unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

		// Dibujar los triángulos
		ourShader.use();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		ourShader.setInt("textureSelector", cambiarTextura[0]);


		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 12);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		ourShader.setInt("textureSelector", cambiarTextura[1]);


		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 12, 15);


		std::this_thread::sleep_for(std::chrono::milliseconds(10)); // 100 ms = 0.1 segundos
		// Intercambiar los buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Liberar recursos
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	// Terminar GLFW
	glfwTerminate();
	return 0;
}

// Procesar entrada de usuario
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
		cambiarTextura[0] = 0;
		cambiarTextura[1] = 1;
	}

	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
		cambiarTextura[0] = 1;
		cambiarTextura[1] = 0;
	}
}

// Función de devolución de llamada para cambiar el tamaño del framebuffer
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
