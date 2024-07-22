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
void framebuffer_size_callback(GLFWwindow * window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

unsigned int loadTexture(const char* path);

// Configuración de la ventana
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Cámara
Camera camera(glm::vec3(3.5f, 3.0f, 12.0f)); // Inicializar la cámara en el espacio
float lastX = SCR_WIDTH / 2.0f; // Posición del mouse en X
float lastY = SCR_HEIGHT / 2.0f; // Posición del mouse en Y
bool firstMouse = true; // Bandera para controlar la primera vez que se mueve el mouse

// Tiempo
float deltaTime = 0.0f;	// Tiempo entre el frame actual y el último frame
float lastFrame = 0.0f; // Tiempo del último frame

// Posición de la luz
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

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
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Miguel Mendosa - 2300163678", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Centrar la ventana en la pantalla
	int screenWidth, screenHeight;
	GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);
	screenWidth = mode->width;
	screenHeight = mode->height;
	glfwSetWindowPos(window, (screenWidth - SCR_WIDTH) / 2, (screenHeight - SCR_HEIGHT) / 2);

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
	Shader lightingShader("shaders/vertexshader_B2T4.vs", "shaders/fragmentshader_B2T4.fs");
	Shader lightCubeShader("shaders/vertexshader_B2T4_lightcube.vs", "shaders/fragmentshader_B2T4_lightcube.fs");

	// Definición de los datos de los vértices y configuración de los atributos del vértice
	float vertices[] = {
		// Posiciones         // normales         // Coordenadas de textura
												  //Cara frontal
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.551f, 0.508f, 0.559f, 0.481f, 0.551f, 0.500f, 0.548f, 0.425f, //F
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.754f, 0.508f, 0.764f, 0.481f, 0.754f, 0.500f, 0.753f, 0.425f, //H
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.754f, 0.684f, 0.764f, 0.653f, 0.754f, 0.680f, 0.753f, 0.683f, //G
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.754f, 0.684f, 0.764f, 0.653f, 0.754f, 0.680f, 0.753f, 0.683f, //G
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.552f, 0.684f, 0.558f, 0.653f, 0.552f, 0.680f, 0.548f, 0.683f, //E
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.551f, 0.508f, 0.558f, 0.481f, 0.551f, 0.500f, 0.548f, 0.425f, //F

												  //Cara trasera
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.147f, 0.506f, 0.150f, 0.481f, 0.147f, 0.506f, 0.139f, 0.425f, //A
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.350f, 0.506f, 0.335f, 0.481f, 0.350f, 0.506f, 0.344f, 0.425f, //D
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.350f, 0.645f, 0.335f, 0.624f, 0.350f, 0.645f, 0.344f, 0.683f, //C
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.350f, 0.645f, 0.335f, 0.624f, 0.350f, 0.645f, 0.344f, 0.683f, //C
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.147f, 0.645f, 0.150f, 0.624f, 0.147f, 0.645f, 0.139f, 0.683f, //B
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.147f, 0.504f, 0.150f, 0.481f, 0.147f, 0.504f, 0.139f, 0.425f, //A
		
												  //Cara izquierda del personaje
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.754f, 0.508f, 0.764f, 0.481f, 0.754f, 0.500f, 0.753f, 0.425f, //H
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.956f, 0.508f, 0.968f, 0.481f, 0.956f, 0.500f, 0.958f, 0.425f, //J
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.956f, 0.684f, 0.968f, 0.653f, 0.956f, 0.680f, 0.958f, 0.683f, //I
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.956f, 0.684f, 0.968f, 0.653f, 0.956f, 0.680f, 0.958f, 0.683f, //I
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.754f, 0.684f, 0.764f, 0.653f, 0.754f, 0.680f, 0.753f, 0.683f, //G
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.754f, 0.508f, 0.764f, 0.481f, 0.754f, 0.500f, 0.753f, 0.425f, //H
		
												  //Cara derecha del personaje    
		0.5f, -0.5f,  0.5f,   1.0f,  0.0f,  0.0f,  0.350f, 0.508f, 0.355f, 0.481f, 0.350f, 0.500f, 0.344f, 0.425f, //D
		0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,  0.552f, 0.508f, 0.559f, 0.481f, 0.552f, 0.500f, 0.548f, 0.425f, //F
		0.5f,  0.5f, -0.5f,   1.0f,  0.0f,  0.0f,  0.552f, 0.684f, 0.559f, 0.653f, 0.552f, 0.680f, 0.548f, 0.683f, //E
		0.5f,  0.5f, -0.5f,   1.0f,  0.0f,  0.0f,  0.552f, 0.684f, 0.559f, 0.653f, 0.552f, 0.680f, 0.548f, 0.683f, //E
		0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,  0.350f, 0.684f, 0.355f, 0.653f, 0.350f, 0.680f, 0.344f, 0.683f, //C
		0.5f, -0.5f,  0.5f,   1.0f,  0.0f,  0.0f,  0.350f, 0.508f, 0.355f, 0.481f, 0.350f, 0.500f, 0.344f, 0.425f, //D
		
												  //Cara inferior
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.551f, 0.301f, 0.559f, 0.266f, 0.551f, 0.241f, 0.548f, 0.166f, //M
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.754f, 0.301f, 0.764f, 0.266f, 0.754f, 0.241f, 0.753f, 0.166f, //N
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.754f, 0.508f, 0.764f, 0.461f, 0.754f, 0.500f, 0.753f, 0.425f, //H
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.754f, 0.508f, 0.764f, 0.461f, 0.754f, 0.500f, 0.753f, 0.425f, //H
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.551f, 0.508f, 0.559f, 0.461f, 0.551f, 0.500f, 0.548f, 0.425f, //F
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.551f, 0.301f, 0.559f, 0.266f, 0.551f, 0.241f, 0.548f, 0.166f, //M
		
												  //Cara superior
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.552f, 0.684f, 0.559f, 0.653f, 0.552f, 0.680f, 0.549f, 0.677f, //E
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.754f, 0.684f, 0.764f, 0.653f, 0.754f, 0.680f, 0.752f, 0.677f, //G
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.754f, 0.847f, 0.764f, 0.827f, 0.754f, 0.847f, 0.752f, 0.935f, //L
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.754f, 0.847f, 0.764f, 0.827f, 0.754f, 0.847f, 0.752f, 0.935f, //L
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.552f, 0.847f, 0.559f, 0.827f, 0.552f, 0.847f, 0.549f, 0.935f, //K
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.552f, 0.684f, 0.559f, 0.653f, 0.552f, 0.680f, 0.549f, 0.677f, //E
	};

	// Posiciones de los cubos en el espacio
	glm::vec3 cubePositions[] = {
		glm::vec3(3.5f, 1.0f, 1.75f),
		glm::vec3(1.5f, 1.0f, 5.5f),
		glm::vec3(5.5f, 1.0f, 5.5f),
	};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Posición
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// Normales
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// Coordenadas de textura
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	// Coordenadas de textura 2
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);
	// Coordenadas de textura 3
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(10 * sizeof(float)));
	glEnableVertexAttribArray(4);
	// Coordenadas de textura 4
	glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(12 * sizeof(float)));
	glEnableVertexAttribArray(5);

	// Crear el VAO para la luz
	unsigned int lightCubeVAO;
	glGenVertexArrays(1, &lightCubeVAO);
	glBindVertexArray(lightCubeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Atributos de posición de los vértices (el cubo de la luz no necesita normales)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Carga y creación de texturas
	unsigned int texture1, texture2, texture3, texture4;
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
	data = stbi_load("textures/meliodas.png", &width, &height, &nrChannels, 0);
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

	// Textura 3
	glGenTextures(1, &texture3);
	glBindTexture(GL_TEXTURE_2D, texture3);
	// Configuración de los parámetros de wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Configuración de los parámetros de filtrado
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Carga de la imagen, creación de la textura y generación de mipmaps
	data = stbi_load("textures/spiderman.png", &width, &height, &nrChannels, 0);
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

	// Textura 4
	glGenTextures(1, &texture4);
	glBindTexture(GL_TEXTURE_2D, texture4);
	// Configuración de los parámetros de wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Configuración de los parámetros de filtrado
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Carga de la imagen, creación de la textura y generación de mipmaps
	data = stbi_load("textures/cubo.png", &width, &height, &nrChannels, 0);
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
	lightingShader.use();
	glUniform1i(glGetUniformLocation(lightingShader.ID, "texture1"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.ID, "texture2"), 1);
	glUniform1i(glGetUniformLocation(lightingShader.ID, "texture3"), 2);
	glUniform1i(glGetUniformLocation(lightingShader.ID, "texture4"), 3);

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
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float translateX = (sin(glfwGetTime()) * 3.5f) + 2.3f;
		float translateY = (sin(5 * glfwGetTime()) * 0.5f) + 0.3f;
		float translateZ = (sin(2 * glfwGetTime()) * 3) + 2.2f;

		glm::vec3 lightPosition = glm::vec3(lightPos.x + translateX, lightPos.y + translateY, lightPos.z + translateZ);

		// Configurar el shader de iluminación y pasarle las matrices de transformación
		lightingShader.use();

		// light properties
		lightingShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
		lightingShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
		lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

		// material properties
		lightingShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
		lightingShader.setFloat("material.shininess", 64.0f);

		// Enlazar las texturas a las unidades de textura correspondientes
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, texture3);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, texture4);

		// Crear las transformaciones
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		lightingShader.setMat4("view", view);
		lightingShader.setMat4("projection", projection);

		// Configura la posición y color de la luz
		lightingShader.setVec3("light.position", lightPosition);
		lightingShader.setVec3("viewPos", camera.Position);
		lightingShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
		lightingShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
		lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

		// Renderizar los cubos
		glBindVertexArray(VAO);
		for (unsigned int i = 0; i < 3; i++)
		{
			glm::mat4 model1 = glm::mat4(1.0f);
			model1 = glm::translate(model1, cubePositions[i]);

			// Aplicar una rotación independiente a cada cubo
			float angle = (180.0f);
			model1 = glm::rotate(model1, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));

			// Aplicar el salto a los cubos
			float jumpHeight = 0.5f; // Altura del salto
			float jumpSpeed = 2.0f; // Velocidad del salto
			float yOffset = glm::sin(currentFrame * jumpSpeed + i) * jumpHeight + 0.4f;
			model1 = glm::translate(model1, glm::vec3(0.0f, yOffset, 0.0f));

			// Aplicar el escalado mínimo a los cubos
			float scaleValue = glm::sin(currentFrame * jumpSpeed + i) * 0.20f + 1.0f; // Escalado basado en el tiempo
			model1 = glm::scale(model1, glm::vec3(1.0f, scaleValue, 1.0f));

			// Use texture1 for the first 3 cubes, texture2 for the rest
			if (i < 1)
			{
				lightingShader.setInt("useTexture", 3);
			}
			else if (i < 2) {
				lightingShader.setInt("useTexture", 2);
			}
			else if (i < 3) {
				lightingShader.setInt("useTexture", 1);
			}

			lightingShader.setMat4("model", model1);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		lightingShader.setInt("useTexture", 0);

		// Dibujar una plataforma de 5x5 cubos
		for (int x = 0; x < 8; x++)
		{
			for (int z = 0; z < 8; z++)
			{
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(x, 0.0f, z)); // Posicionar el cubo
				lightingShader.setMat4("model", model);
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
		}

		// Dibuja el cubo de la luz
		lightCubeShader.use();
		lightCubeShader.setMat4("projection", projection);
		lightCubeShader.setMat4("view", view);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::translate(model, glm::vec3(translateX, translateY, translateZ));
		model = glm::scale(model, glm::vec3(0.2f));
		lightCubeShader.setMat4("model", model);

		glBindVertexArray(lightCubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Intercambiar buffers y sondear eventos de E/S
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// De-allocar todos los recursos
	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightCubeVAO);
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