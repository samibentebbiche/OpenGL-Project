//TP4 Partie 3
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader.hpp>
#include <glm.hpp>
#include <gtx/transform.hpp>

using namespace glm;

struct STRVertex
{
	vec3 position;
	vec3 couleur;
};



void Resize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


int main()
{
	if (!glfwInit())
	{
		printf("Could not initialize glfw.\n");
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(640, 480, "OpenGL TP 3", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	//initialiser GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Could not initialize GLAD" << std::endl;
		return -1;
	}



	float hauteur = -1.0f;
	float position_x = -10.0f;// y = 10x
	float position_y = -5;//position_x * 2;
	float vitesse = 0.02f;
	float z = 0;

	GLuint vertex_index[] =
	{
		0,1,2,2,3,0,  4,5,6,6,7,4  ,0,4,1,1,4,5  ,4,1,3,4,3,7
	};

	//déclaration des sommets
	STRVertex vertices_1[] =
	{
		vec3(position_x + (1.0f), hauteur, 1.0f + position_y),       vec3( 0.0f, 0.0f, 1.0f), // 0
		vec3(position_x + (-1.0f), hauteur, 1.0f + position_y),       vec3(0.0f, 0.0f, 1.0f), //1
		vec3 (position_x  +(-1.0f), hauteur, -1.0f + position_y),      vec3(0.0f, 0.0f, 1.0f),  //2 
		vec3(position_x + (1.0f), hauteur, -1.0f + position_y),       vec3(0.0f, 0.0f, 1.0f), //3
		

		vec3(position_x +(1.0f), hauteur + 0.5f , 1.0f + position_y),       vec3(0.0f, 0.0f, 1.0f), //4
		vec3(position_x  +(-1.0f),hauteur + 0.5f , 1.0f + position_y),       vec3(0.0f, 0.0f, 1.0f), // 5
		vec3(position_x  +(-1.0f),hauteur + 0.5f , -1.0f + position_y),      vec3(0.0f, 0.0f, 1.0f), //6
		vec3(position_x +(1.0f), hauteur + 0.5f , -1.0f + position_y),       vec3(0.0f, 0.0f, 1.0f), //7
		


	};



	//déclaration des buffers
	GLuint VAO;
	GLuint VBO;
	GLuint IBO;

	//Générer et lier le VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);




	//générer le VBO et le lier sur la cible GL_ARRAY_BUFFER
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO); 


	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 24 * sizeof(unsigned int), vertex_index, GL_DYNAMIC_DRAW);

	//Fournir les sommets a OpenGL pour qu'ils soient placés dans le VBO 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_1), vertices_1, GL_DYNAMIC_DRAW);

	//Lier le premier buffer d'attributs (les sommets) et configurer le pointeur 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(STRVertex), (void*)offsetof(STRVertex, position));

	//indique � OpenGL qu'on utilise un attribut donné
	glEnableVertexAttribArray(0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(STRVertex), (void*)offsetof(STRVertex, couleur));

	//Dbinder le VAO et le VBO :
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);



	mat4 Projection = perspective(radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	mat4 View = lookAt(vec3(2, 2, 5), vec3(0, z, 0), vec3(0, 1, 0));
	mat4 Model = mat4(1.0f);
	//Model = translate(Model, vec3(1.0f, 0.0f, 0.0f));
	//mat4 MVP = Projection * View * Model;

	GLuint ShaderProgram;
	ShaderProgram = LoadShaders("shader/SimpleVertexShader.vertexshader",
		"shader/SimpleFragmentShader.fragmentshader");

	GLuint MatrixID = glGetUniformLocation(ShaderProgram, "MVP");

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	while (!glfwWindowShouldClose(window))
	{

		position_x = position_x + vitesse;

		


		if (position_x > 5 || position_x < -15) {

			z = z + 0.5;
			std::cout << z << std::endl;
			//position_x = 8.0f;// y = 10x
			//position_y = -4;//position_x * 2;
			vitesse = vitesse * -1;
		}
		View = lookAt(vec3(2, 2, 5), vec3(0, z, 0), vec3(0, 1, 0));
		Model = translate(Model, vec3(vitesse, 0.0f, vitesse/4));
		mat4 MVP = Projection * View * Model;
		glUseProgram(ShaderProgram);
		//la couleur du fond
		glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		//Lier le VAO 
		glBindVertexArray(VAO);
		//Dessiner le triangle 
		//glDrawArrays(GL_TRIANGLES, 0, 18);
		glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, (void*)0);

		glfwSetFramebufferSizeCallback(window, Resize);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
};