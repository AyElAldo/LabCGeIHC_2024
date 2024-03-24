/*
Pr�ctica 5: Optimizaci�n y Carga de Modelos
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_m.h"
#include "Camera.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

const float toRadians = 3.14159265f / 180.0f;
//float angulocola = 0.0f;
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;
Model Mclaren_cuerpo; // Tronco del cuerpo 
Model Mclaren_cofre; // Cabeza sin mandibula
Model Mclaren_llanta_t_i; // Llanta trasera izquierda
Model Mclaren_llanta_t_d; // Llanta trasera derecha
Model Mclaren_llanta_d_i; // Llanta delantera izquierda
Model Mclaren_llanta_d_d; // Llanta delantera derecha

Skybox skybox;

//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;


// Vertex Shader
static const char* vShader = "shaders/shader_m.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_m.frag";





void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};


	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);


}


void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.5f, 7.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 1.0f);

	/* Carga de modelos */
	
	Mclaren_cuerpo = Model();
	Mclaren_cuerpo.LoadModel("Models/carro_cuerpo.obj");

	Mclaren_cofre = Model();
	Mclaren_cofre.LoadModel("Models/cofre.obj");

	Mclaren_llanta_t_i = Model();
	Mclaren_llanta_t_i.LoadModel("Models/llanta_d_d.obj");

	Mclaren_llanta_t_d = Model();
	Mclaren_llanta_t_d.LoadModel("Models/llanta_d_d.obj");

	Mclaren_llanta_d_i = Model();
	Mclaren_llanta_d_i.LoadModel("Models/llanta_d_d.obj");

	Mclaren_llanta_d_d = Model();
	Mclaren_llanta_d_d.LoadModel("Models/llanta_d_d.obj");

	// Faltan patas

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);


	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//Se dibuja el Skybox
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformColor = shaderList[0].getColorLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		// INICIA DIBUJO DEL PISO
		color = glm::vec3(0.86f, 0.88f, 0.87f); //piso de color gris
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0,0,0));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMesh();

		//------------*INICIA DIBUJO DE NUESTROS DEM�S OBJETOS-------------------*

		// Cuerpo carro
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f,mainWindow.gettraslacionCarro()));
		modelaux = model;

		color = glm::vec3(0.12f, 0.4f, 0.41); 
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Mclaren_cuerpo.RenderModel();

		model = modelaux;

		// Cofre de carro
	
		model = glm::translate(model, glm::vec3(0.5f, 0.82f, 1.35f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotacionCofre()), glm::vec3(1.0f, 0.0f, 0.0f));

		color = glm::vec3(0.12f, 0.14f, 0.12); 
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Mclaren_cofre.RenderModel();

		model = modelaux;

		/* LLANTAS */

		color = glm::vec3(0.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		// Llantas delanteras
		modelaux = model;
		model = glm::translate(model, glm::vec3(-0.2f, 0.32f, 1.15f));
		model = glm::rotate(model, glm::radians(mainWindow.getllantaDD()), glm::vec3(1.0f, 0.0f, 0.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Mclaren_llanta_d_d.RenderModel();

		model = modelaux;

		
		model = glm::translate(model, glm::vec3(1.4f, 0.32f, 1.15f));
		model = glm::rotate(model, glm::radians(mainWindow.getllantaDI()), glm::vec3(1.0f, 0.0f, 0.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Mclaren_llanta_d_i.RenderModel();

		model = modelaux;
		
		// Llantas traseras

		model = glm::translate(model, glm::vec3(-0.2f, 0.32f, -1.52f));
		model = glm::rotate(model, glm::radians(mainWindow.getllantaTD()), glm::vec3(1.0f, 0.0f, 0.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Mclaren_llanta_t_d.RenderModel();

		model = modelaux;


		model = glm::translate(model, glm::vec3(1.4f, 0.32f, -1.52f));
		model = glm::rotate(model, glm::radians(mainWindow.getllantaTI()), glm::vec3(1.0f, 0.0f, .0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Mclaren_llanta_t_i.RenderModel();

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
