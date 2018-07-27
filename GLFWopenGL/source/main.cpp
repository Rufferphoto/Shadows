#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <iostream>

// Library headers.



// My headers.
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// Functions
void initialiseGLFW();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouseCallback(GLFWwindow* window, double a_xPos, double a_yPos);
void scrollCallback(GLFWwindow* window, double xOffset, double yoffset);
void processInput(GLFWwindow *window);
void setLighting(const Shader &shader);
unsigned int loadTexture(const char *path);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Window
GLFWwindow* window;

// Camera creation.
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

// Mouse Position Variables.
bool firstMouse = true;
bool torchOn = true;
float lastX = 400, lastY = 300;

// Light variables.

glm::vec3 pointLightPositions[] = {
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
};
glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};
// Setup vertex data (and buffer(s)) and configure vertex attributes.
float vertices[] = {
	// positions          // normals           // texture coords
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
	0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
	0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
};

// Timing creation.
float deltaTime = 0.0f; // Time between current frame and last frame.
float lastFrame = 0.0f; // Time of last frame.



int main()
{
	initialiseGLFW();
	// ===== Build and compile the Vertex and Fragment shader program.
	Shader lightShader("resources/Shaders/vertex.glsl", "resources/Shaders/fragment.glsl");
	Shader lampShader("resources/Shaders/vertex_Lamp.glsl", "resources/Shaders/fragment_Lamp.glsl");
	Shader simpleDepthShader("resources/Shaders/vertex_Shadow.glsl", "resources/Shaders/fragment_Shadow.glsl");
	// ===== Load in models.
	Model tankScene("resources/Models/ruinedtank/tank.obj");
		

	// ====================================================================================================================
	// Vertex buffer object is first occurance of OpenGL object, it has a unique ID corresponding to that buffer,
	// so we can generate one with a buffer ID using the glGenBuffers function.
	unsigned int VBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);
	// Bind the vertex array Object first, then bind and set vertex buffer(s_), then configure vertex attribute(s).
	
	// We can bind the newly created buffer to the GL_ARRAY_BUFFER target with the glBindBuffer function.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	
	// Copy the previously defined vertex data into the buffers memory.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);

	// Position attribute.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// Configure light's VAO made it easy by using the same vertices for the light object as the 3d cube.
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// Only need to bind to the VBO 
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// ====================================================================================================================

	// Texture maps for the containers.
	unsigned int diffuseMap = loadTexture("resources/Textures/container2.png");
	unsigned int specularMap = loadTexture("resources/Textures/container2specular.png");

	// shader configuration
	// --------------------
	lightShader.UseShader();
	lightShader.setInt("material.diffuse", 0);
	lightShader.setInt("material.specular", 1);

	// Setting up depth buffer object.
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	// Creating a texture
	unsigned int depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glm::vec3 lightPos(-2.0f, 4.0f, -1.0f);
	
	// Render Loop. (Keeps running until told to stop.)
	while (!glfwWindowShouldClose(window))
	{
		// Time and frame logic calculated here.
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Input function call.
		processInput(window);

		// Rendering.
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glm::mat4 lightProjection, lightView;
		glm::mat4 lightSpaceMatrix;
		glm::mat4 view;
		glm::mat4 projection;
		glm::mat4 model;

		float near_plane = 1.0f, far_plane = 7.5f;
		lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;
		// render scene from light's point of view
		simpleDepthShader.UseShader();
		simpleDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);

		// Bind specularMap
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);
		// Render cube.
		glBindVertexArray(cubeVAO);

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
		//model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
		lightShader.setMat4("model", model);
		tankScene.Draw(lightShader);

		for (unsigned int i = 0; i < 10; i++)
		{
			// calculate the model matrix for each object and pass it to shader before drawing
			model = glm::mat4();
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			lightShader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// =====================================================================
		
		// reset viewport
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// Setting up lighting
		lightShader.UseShader();
		setLighting(lightShader);
		
		// Turn torch on and off.
		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		{
			printf("torch off");
			lightShader.setFloat("spotLight.linear", 100);
			torchOn = false;
		}
		if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		{
			printf("torch on");
			lightShader.setFloat("spotLight.linear", 0.09);
			torchOn = true;
		}

		// Create transormations
		// Pass projection matrix to shader
		projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		
		// Look at matrix creates a view matrix that looks at a give target.
		view = camera.GetViewMatrix();
				
		// Next create a view matrix, We need to move slightly backwards in the scene so that object becomes visible when in world space were located at the origin.
		// To move around the scene i.e. to move the camera backwards is the same as moving the scene forwards.
		lightShader.setMat4("projection", projection);
		lightShader.setMat4("view", view);
		//lightShader.setVec3("light.direction", -0.2f, -1.0f, -0.3f);
		//lightShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
		lightShader.setVec3("viewPos", camera.Position);
		lightShader.setVec3("lightPos", lightPos);
		lightShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);


		// ================= Render the tank ================================
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
		//model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
		lightShader.setMat4("model", model);
		tankScene.Draw(lightShader);
		// =====================================================================

		





		// ============ Render the containers. ================================
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		// Bind specularMap
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);

		// Render cube.
		glBindVertexArray(cubeVAO);
		
		for (unsigned int i = 0; i < 10; i++)
		{
			// calculate the model matrix for each object and pass it to shader before drawing
			model = glm::mat4();
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			lightShader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		// =====================================================================



		// ============ Render the lamps for the light positions. =============
		lampShader.UseShader();
		lampShader.setMat4("projection", projection);
		lampShader.setMat4("view", view);
		// Calculate the model matrix for each object and pass it to shader before draw call.
		glBindVertexArray(lightVAO);

		for (unsigned int i = 0; i < 4; i++)
		{
			model = glm::mat4();
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f));
			lampShader.setMat4("model", model);
			// Render lamp.
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		// =====================================================================
		
		

		// Swap colour buffer, (Large buffer that contains colour values for each pixel in GLFW's window),
		glfwSwapBuffers(window);
		// Check if any events are triggered. i.e. keyboard input or mouse movement.
		glfwPollEvents();

	} // End of while loop.


	// De-Allocate all resources once they've outlived their purpose.
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteBuffers(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
	

	// Clean/Delete all resources that were allocated.
	glfwTerminate();
	return 0;
}


// ===================================== Utility functions =================================

// Function to initialise GLFW. Stripping a lot of code from main.
void initialiseGLFW()
{
	// Initialise GLFW.
	glfwInit();
	// Configure the window.
	// Setting the major version to 3 which tells GLFW which version to use.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	// Setting the minor version to 3 which tells GLFW which version to use.
	// Meaning GLFW can make the proper arrangements when creating the OpenGL context.
	// If the user does not have the proper OpenGL version then GLFW will fail to run.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Use core profile meaning access to a smaller subset of OpenGL features, i.e. without backwards-compatible features
	// that we will no longer need.
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Use the line below to initialize for Mac OS X.
	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Creating a window object.
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CT6025 Reassessment", NULL, NULL);
	// If window fails to be created.
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}

	// Set the current context to the window we just created.
	glfwMakeContextCurrent(window);
	// Tell GLFW to call the custom resize function on every window resize.
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetCursorPosCallback(window, mouseCallback);


	// Initialize GLAD before calling any OpenGL function.
	// glfwGetProcAddress defines the correct function based on the OS we're compiling for.
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}


	// Tell GLFW to capture the mouse.
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glEnable(GL_DEPTH_TEST);
	// Registering the scrolling callback.
}

// Setting up lighting values and positions.
void setLighting(const Shader &shader)
{
	shader.setVec3("viewPos", camera.Position);
	
	// Set light properties.
	// Directional Light
	shader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
	shader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
	shader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
	shader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
	// Point Light
	shader.setVec3("pointLights[0].position", pointLightPositions[0]);
	shader.setVec3("pointLights[0].ambient", 1.0f, 0.05f, 0.05f);
	shader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
	shader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
	shader.setFloat("pointLights[0].constant", 1.0f);
	shader.setFloat("pointLights[0].linear", 0.09);
	shader.setFloat("pointLights[0].quadratic", 0.032);
	// point light 2
	shader.setVec3("pointLights[1].position", pointLightPositions[1]);
	shader.setVec3("pointLights[1].ambient", 0.05f, 1.00f, 0.05f);
	shader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
	shader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
	shader.setFloat("pointLights[1].constant", 1.0f);
	shader.setFloat("pointLights[1].linear", 0.09);
	shader.setFloat("pointLights[1].quadratic", 0.032);
	// po light 3
	shader.setVec3("pointLights[2].position", pointLightPositions[2]);
	shader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 1.0f);
	shader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
	shader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
	shader.setFloat("pointLights[2].constant", 1.0f);
	shader.setFloat("pointLights[2].linear", 0.09);
	shader.setFloat("pointLights[2].quadratic", 0.032);
	// po light 4
	shader.setVec3("pointLights[3].position", pointLightPositions[3]);
	shader.setVec3("pointLights[3].ambient", 0.35f, 0.35f, 1.0f);
	shader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
	shader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
	shader.setFloat("pointLights[3].constant", 1.0f);
	shader.setFloat("pointLights[3].linear", 0.09);
	shader.setFloat("pointLights[3].quadratic", 0.032);
	// Spotight (attached to camera.Front) as a torch.
	shader.setVec3("spotLight.position", camera.Position);
	shader.setVec3("spotLight.direction", camera.Front);
	shader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
	shader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
	shader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
	shader.setFloat("spotLight.constant", 1.0f);
	// lightShader.setFloat("spotLight.linear", 0.09);
	shader.setFloat("spotLight.quadratic", 0.032);
	shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
	shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
	// Set cube material properties.
	shader.setFloat("material.shininess", 32.0f);
}

// Function for when the user resizes the window it adjusts the viewport.
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// To calculate pitch and yaw values we need to tell GLFW to listen to mouse-movement events. 
// This can be done with a callback function.
void mouseCallback(GLFWwindow* window, double a_xPos, double a_yPos)
{
	if (firstMouse) // this bool variable is initially set to true
	{
		lastX = a_xPos;
		lastY = a_yPos;
		firstMouse = false;
	}

	// When handling mouse input for an FPS style camera, serveral steps should be done to retrieve direction vector.

	float xOffset = a_xPos - lastX;
	float yOffset = lastY - a_yPos;

	lastX = a_xPos;
	lastY = a_yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}

void scrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	camera.ProcessMouseScroll(yOffset);
}

// Function to process Input for the window passed in.
void processInput(GLFWwindow *window)
{
	
	// Input detection.
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) // Move forward.
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) // Move backward.
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) // Move left.
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) // Move right.
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) // Move up.
		camera.ProcessKeyboard(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) // Moving down.
		camera.ProcessKeyboard(DOWN, deltaTime);
	// Using glfwGetKey that takes in the window as an input together with a key,
	// The function returns whether this key is currently being pressed.
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const * path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
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
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
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



