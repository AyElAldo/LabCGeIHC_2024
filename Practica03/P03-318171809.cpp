﻿//práctica 3: Modelado Geométrico y Cámara Sintética.
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
#include <gtc\random.hpp>
//clases para dar orden y limpieza al còdigo
#include"Mesh.h"
#include"Shader.h"
#include"Sphere.h"
#include"Window.h"
#include"Camera.h"
//tecla E: Rotar sobre el eje X
//tecla R: Rotar sobre el eje Y
//tecla T: Rotar sobre el eje Z


using std::vector;

//Dimensiones de la ventana
const float toRadians = 3.14159265f/180.0; //grados a radianes
const float PI = 3.14159265f;
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;
Camera camera;
Window mainWindow;
vector<Mesh*> meshList;
vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
Sphere sp = Sphere(1.0, 20, 20); //recibe radio, slices, stacks




void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

	GLfloat cubo_vertices[] = {
		// front
		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		// back
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f
	};
	Mesh* cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(cubo);
}

// Pirámide triangular regular
void CrearPiramideTriangular()
{
	unsigned int indices_piramide_triangular[] = {
			0,1,2,
			1,3,2,
			3,0,2,
			1,0,3

	};
	GLfloat vertices_piramide_triangular[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-1.0f,	//3

	};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 12, 12);
	meshList.push_back(obj1);

}
/*
Crear cilindro, cono y esferas con arreglos dinámicos vector creados en el Semestre 2023 - 1 : por Sánchez Pérez Omar Alejandro
*/
void CrearCilindro(int res, float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//cálculo del paso interno en la circunferencia y variables que almacenarán cada coordenada de cada vértice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;

	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//ciclo for para crear los vértices de las paredes del cilindro
	for (n = 0; n <= (res); n++) {
		if (n != res) {
			x = R * cos((n)*dt);
			z = R * sin((n)*dt);
		}
		//caso para terminar el círculo
		else {
			x = R * cos((0)*dt);
			z = R * sin((0)*dt);
		}
		for (i = 0; i < 6; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			case 3:
				vertices.push_back(x);
				break;
			case 4:
				vertices.push_back(0.5);
				break;
			case 5:
				vertices.push_back(z);
				break;
			}
		}
	}

	//ciclo for para crear la circunferencia inferior
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(-0.5f);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}

	//ciclo for para crear la circunferencia superior
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(0.5);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}

	//Se generan los indices de los vértices
	for (i = 0; i < vertices.size(); i++) indices.push_back(i);

	//se genera el mesh del cilindro
	Mesh *cilindro = new Mesh();
	cilindro->CreateMeshGeometry(vertices, indices, vertices.size(), indices.size());
	meshList.push_back(cilindro);
}

//función para crear un cono
void CrearCono(int res,float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//cálculo del paso interno en la circunferencia y variables que almacenarán cada coordenada de cada vértice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;
	
	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//caso inicial para crear el cono
	vertices.push_back(0.0);
	vertices.push_back(0.5);
	vertices.push_back(0.0);
	
	//ciclo for para crear los vértices de la circunferencia del cono
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}
	vertices.push_back(R * cos(0) * dt);
	vertices.push_back(-0.5);
	vertices.push_back(R * sin(0) * dt);


	for (i = 0; i < res+2; i++) indices.push_back(i);

	//se genera el mesh del cono
	Mesh *cono = new Mesh();
	cono->CreateMeshGeometry(vertices, indices, vertices.size(), res + 2);
	meshList.push_back(cono);
}

//función para crear pirámide cuadrangular unitaria
void CrearPiramideCuadrangular()
{
	vector<unsigned int> piramidecuadrangular_indices = {
		0,3,4,
		3,2,4,
		2,1,4,
		1,0,4,
		0,1,2,
		0,2,4

	};
	vector<GLfloat> piramidecuadrangular_vertices = {
		0.5f,-0.5f,0.5f,
		0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,0.5f,
		0.0f,0.5f,0.0f,
	};
	Mesh *piramide = new Mesh();
	piramide->CreateMeshGeometry(piramidecuadrangular_vertices, piramidecuadrangular_indices, 15, 18);
	meshList.push_back(piramide);
}



void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader* shader2 = new Shader();
	shader2->CreateFromFiles(vShaderColor, fShader);
	shaderList.push_back(*shader2);
}


int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	//Cilindro y cono reciben resolución (slices, rebanadas) y Radio de circunferencia de la base y tapa

	CrearCubo();//índice 0 en MeshList
	CrearPiramideTriangular();//índice 1 en MeshList
	CrearCilindro(25, 1.0f);//índice 2 en MeshList
	CrearCono(25, 2.0f);//índice 3 en MeshList
	CrearPiramideCuadrangular();//índice 4 en MeshList
	CreateShaders();
	
	

	/*Cámara se usa el comando: glm::lookAt(vector de posición, vector de orientación, vector up));
	En la clase Camera se reciben 5 datos:
	glm::vec3 vector de posición,
	glm::vec3 vector up,
	GlFloat yaw rotación para girar hacia la derecha e izquierda
	GlFloat pitch rotación para inclinar hacia arriba y abajo
	GlFloat velocidad de desplazamiento,
	GlFloat velocidad de vuelta o de giro
	Se usa el Mouse y las teclas WASD y su posición inicial está en 0,0,1 y ve hacia 0,0,-1.
	*/

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.3f);

	
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	//glm::mat4 projection = glm::ortho(-1, 1, -1, 1, 1, 10);
	
	//Loop mientras no se cierra la ventana
	sp.init(); //inicializar esfera
	sp.load();//enviar la esfera al shader

	glm::mat4 model(1.0);//Inicializar matriz de Modelo 4x4

	glm::vec3 color = glm::vec3(0.0f,0.0f,0.0f); //inicializar Color para enviar a variable Uniform;

	while (!mainWindow.getShouldClose())
	{

		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		//Recibir eventos del usuario
		glfwPollEvents();
		//Cámara
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//Limpiar la ventana
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		uniformColor = shaderList[0].getColorLocation();

		model = glm::mat4(1.0);
		//Traslación inicial para posicionar en -Z a los objetos
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		//otras transformaciones para el objeto
		//model = glm::scale(model, glm::vec3(0.5f,0.5f,0.5f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));  //al presionar la tecla Y se rota sobre el eje y
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//la línea de proyección solo se manda una vez a menos que en tiempo de ejecución
		//se programe cambio entre proyección ortogonal y perspectiva
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(1.0f, 0.f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[0]->RenderMesh(); //dibuja cubo y pirámide triangular
		//meshList[3]->RenderMeshGeometry(); //dibuja las figuras geométricas cilindro, cono, pirámide base cuadrangular
		//sp.render(); //dibuja esfera



		//ejercicio: Instanciar primitivas geométricas para recrear el dibujo de la práctica pasada en 3D,
		//se requiere que exista piso y la casa tiene una ventana azul circular justo en medio de la pared trasera y solo 1 puerta frontal.


		/* Creacion del piraminx */
		/*
		Cada piramide contenido en la piramide mayor (negra) va a tener un tamaño igual para que sea regular
		*/
		float tam_piramide = 4.75f; // Dimension para las piramides pequeñas

		model = glm::mat4(1.0f);

		color = glm::vec3(0.0f, 0.0f, 0.0f); // Color negro
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(20.0f, 20.0f, 20.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh(); // Piramide triangular


		/* Cara roja */

		// Piramide de hasta arriba
		model = glm::mat4(1.0f);
		color = glm::vec3(1.0f, 0.0f, 0.0f); // Color rojo
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::translate(model, glm::vec3(-0.0f, 5.8f, -3.15f));
		//model = glm::rotate(model, glm::radians())
		model = glm::scale(model, glm::vec3(tam_piramide, tam_piramide, tam_piramide));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		/* Segunda fila de la cara roja */
		// Piramide centro
		model = glm::mat4(1.0f);
		color = glm::vec3(1.0f, 0.0f, 0.0f); // Color rojo
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::translate(model, glm::vec3(-0.0f, 0.3f, -1.9f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f)); // rota eje z
		model = glm::rotate(model, glm::radians(29.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // rota eje x
		model = glm::scale(model, glm::vec3(tam_piramide, tam_piramide, tam_piramide));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		// Izquierda
		model = glm::mat4(1.0f);
		color = glm::vec3(1.0f, 0.0f, 0.0f); // Color rojo
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::translate(model, glm::vec3(-3.2f, -0.4f, -1.7f));
		model = glm::scale(model, glm::vec3(tam_piramide, tam_piramide, tam_piramide));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		// Derecha
		model = glm::mat4(1.0f);
		color = glm::vec3(1.0f, 0.0f, 0.0f); // Color rojo
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::translate(model, glm::vec3(3.2f, -0.4f, -1.7f));
		model = glm::scale(model, glm::vec3(tam_piramide, tam_piramide, tam_piramide));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		/* Tercera fila */
		// Centro
		model = glm::mat4(1.0f);
		color = glm::vec3(1.0f, 0.0f, 0.0f); // Color rojo
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::translate(model, glm::vec3(-0.0f, -6.5f, 0.05f));
		model = glm::scale(model, glm::vec3(tam_piramide, tam_piramide, tam_piramide));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		// Izquierda 1
		model = glm::mat4(1.0f);
		color = glm::vec3(1.0f, 0.0f, 0.0f); // Color rojo
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::translate(model, glm::vec3(-6.2f, -6.5f, -0.05f));
		model = glm::scale(model, glm::vec3(tam_piramide, tam_piramide, tam_piramide));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		// Izquierda 2
		model = glm::mat4(1.0f);
		color = glm::vec3(1.0f, 0.0f, 0.0f); // Color rojo
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::translate(model, glm::vec3(-3.0f, -5.5f, -0.25f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f)); // rota eje z
		model = glm::rotate(model, glm::radians(29.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // rota eje x
		model = glm::scale(model, glm::vec3(tam_piramide, tam_piramide, tam_piramide));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		// Derecha 1
		model = glm::mat4(1.0f);
		color = glm::vec3(1.0f, 0.0f, 0.0f); // Color rojo
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::translate(model, glm::vec3(3.0f, -5.5f, -0.25f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f)); // rota eje z
		model = glm::rotate(model, glm::radians(29.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // rota eje x
		model = glm::scale(model, glm::vec3(tam_piramide, tam_piramide, tam_piramide));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		// Derecha 2
		model = glm::mat4(1.0f);
		color = glm::vec3(1.0f, 0.0f, 0.0f); // Color rojo
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::translate(model, glm::vec3(6.2f, -6.5f, 0.05f));
		model = glm::scale(model, glm::vec3(tam_piramide, tam_piramide, tam_piramide));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();


		/* Cara verde */
		// Piramide de hasta arriba
		model = glm::mat4(1.0f);
		color = glm::vec3(0.0f, 1.0f, 0.0f); // Color verde
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::translate(model, glm::vec3(-.6f, 5.8f, -4.5f));
		model = glm::scale(model, glm::vec3(tam_piramide, tam_piramide, tam_piramide));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		/* Segunda fila verde */
		// Piramide verde centro
		model = glm::mat4(1.0f);
		color = glm::vec3(0.0f, 1.0f, 0.0f); // Color verde
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::translate(model, glm::vec3(-1.6f, 0.3f, -6.0f)); // x = 2.5


		model = glm::rotate(model, glm::radians(130.0f), glm::vec3(0.0f, 0.0f, 1.0f)); // rota eje z (130)
		model = glm::rotate(model, glm::radians(-10.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // rota eje x (-10)
		model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // rota eje y (10)

		//model = glm::rotate(model, glm::radians(130.0f), glm::vec3(0.0f, 0.0f, 1.0f)); // rota eje z (130)
		//model = glm::rotate(model, glm::radians(20.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // rota eje x (25) 
		//model = glm::rotate(model, glm::radians(10.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // rota eje y (10)
		model = glm::scale(model, glm::vec3(tam_piramide, tam_piramide, tam_piramide));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		// Izquierda segunda fila
		model = glm::mat4(1.0f);
		color = glm::vec3(0.0f, 1.0f, 0.0f); // Color verde
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::translate(model, glm::vec3(-.5f, 0.0f, -8.7f)); // x = 2.5
		model = glm::scale(model, glm::vec3(tam_piramide, tam_piramide, tam_piramide));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		// Derecha segunda fila
		model = glm::mat4(1.0f);
		color = glm::vec3(0.0f, 1.0f, 0.0f); // Color verde
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::translate(model, glm::vec3(-3.4f, 0.0f, -2.9f)); 
		model = glm::scale(model, glm::vec3(tam_piramide, tam_piramide, tam_piramide));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		/* Tercera fila */

		// Centro
		model = glm::mat4(1.0f);
		color = glm::vec3(0.0f, 1.0f, 0.0f); // Color verde
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::translate(model, glm::vec3(-3.6f, -6.2f, -7.5f));
		model = glm::scale(model, glm::vec3(tam_piramide, tam_piramide, tam_piramide));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		// Izquierda 1
		model = glm::mat4(1.0f);
		color = glm::vec3(0.0f, 1.0f, 0.0f); // Color verde
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::translate(model, glm::vec3(-0.65f, -6.2f, -13.0f)); 
		model = glm::scale(model, glm::vec3(tam_piramide, tam_piramide, tam_piramide));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();


		// Derecha 2
		model = glm::mat4(1.0f);
		color = glm::vec3(0.0f, 1.0f, 0.0f); // Color verde
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::translate(model, glm::vec3(-6.3f, -6.2f, -1.6f));
		model = glm::scale(model, glm::vec3(tam_piramide, tam_piramide, tam_piramide));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();


		/* Cara Azul */

		// Piramide arriba azul
		model = glm::mat4(1.0f);
		color = glm::vec3(0.0f, 0.0f, 1.0f); // Color azul
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::translate(model, glm::vec3(0.6f, 5.9f, -4.15f));
		model = glm::scale(model, glm::vec3(tam_piramide, tam_piramide, tam_piramide));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		/* Segunda fila azul */
		// Izquierda azul
		model = glm::mat4(1.0f);
		color = glm::vec3(0.0f, 0.0f, 1.0f); // Color azul
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::translate(model, glm::vec3(3.45f, 0.05f, -2.8f));
		model = glm::scale(model, glm::vec3(tam_piramide, tam_piramide, tam_piramide));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		// Derecha azul
		model = glm::mat4(1.0f);
		color = glm::vec3(0.0f, 0.0f, 1.0f); // Color azul
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::translate(model, glm::vec3(0.65f, 0.05f, -8.6f));
		model = glm::scale(model, glm::vec3(tam_piramide, tam_piramide, tam_piramide));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();


		/* Tercera fila azul */

		// Izquierda azul 1
		model = glm::mat4(1.0f);
		color = glm::vec3(0.0f, 0.0f, 1.0f); // Color azul
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::translate(model, glm::vec3(6.7f, -6.5f, -1.4f));
		model = glm::scale(model, glm::vec3(tam_piramide, tam_piramide, tam_piramide));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		// Centro azul 
		model = glm::mat4(1.0f);
		color = glm::vec3(0.0f, 0.0f, 1.0f); // Color azul
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::translate(model, glm::vec3(3.7f, -6.5f, -7.4f));
		model = glm::scale(model, glm::vec3(tam_piramide, tam_piramide, tam_piramide));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		// Derecha dos
		model = glm::mat4(1.0f);
		color = glm::vec3(0.0f, 0.0f, 1.0f); // Color azul
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::translate(model, glm::vec3(0.7f, -6.5f, -13.4f));
		model = glm::scale(model, glm::vec3(tam_piramide, tam_piramide, tam_piramide));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();


		/* Cara amarilla */

		// Esquina 
		model = glm::mat4(1.0f);
		color = glm::vec3(1.0f, 1.0f, 0.0f); // Color amarillo
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::translate(model, glm::vec3(-0.0f, -7.7f, -13.4f));
		model = glm::scale(model, glm::vec3(tam_piramide, tam_piramide, tam_piramide));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		/* Segunda fila amarilla */

		// Centro amarillo
		model = glm::mat4(1.0f);
		color = glm::vec3(1.0f, 1.0f, 0.0f); // Color amarillo
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::translate(model, glm::vec3(-0.0f, -7.7f, -12.3f));
		model = glm::scale(model, glm::vec3(tam_piramide, tam_piramide, tam_piramide));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // rota eje y
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		// Izquierda
		model = glm::mat4(1.0f);
		color = glm::vec3(1.0f, 1.0f, 0.0f); // Color amarillo
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::translate(model, glm::vec3(-3.0f, -7.7f, -6.5f));
		model = glm::scale(model, glm::vec3(tam_piramide, tam_piramide, tam_piramide));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		// Derecha
		model = glm::mat4(1.0f);
		color = glm::vec3(1.0f, 1.0f, 0.0f); // Color amarillo
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::translate(model, glm::vec3(3.0f, -7.7f, -6.5f));
		model = glm::scale(model, glm::vec3(tam_piramide, tam_piramide, tam_piramide));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		/* Tercera fila */

		// Izquierda 1
		model = glm::mat4(1.0f);
		color = glm::vec3(1.0f, 1.0f, 0.0f); // Color amarillo
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::translate(model, glm::vec3(-0.0f, -7.7f, -0.7f));
		model = glm::scale(model, glm::vec3(tam_piramide, tam_piramide, tam_piramide));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		// Izquierda 2 rotado amarillo
		model = glm::mat4(1.0f);
		color = glm::vec3(1.0f, 1.0f, 0.0f); // Color amarillo
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::translate(model, glm::vec3(-3.0f, -7.7f, -5.75f));
		model = glm::scale(model, glm::vec3(tam_piramide, tam_piramide, tam_piramide));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // rota eje y
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		// Centro
		model = glm::mat4(1.0f);
		color = glm::vec3(1.0f, 1.0f, 0.0f); // Color amarillo
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::translate(model, glm::vec3(-6.0f, -7.7f, -0.7f));
		model = glm::scale(model, glm::vec3(tam_piramide, tam_piramide, tam_piramide));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		// Derecha 1 rotado
		model = glm::mat4(1.0f);
		color = glm::vec3(1.0f, 1.0f, 0.0f); // Color amarillo
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::translate(model, glm::vec3(3.0f, -7.7f, -5.75f));
		model = glm::scale(model, glm::vec3(tam_piramide, tam_piramide, tam_piramide));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // rota eje y
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();


		// Derecha 2
		model = glm::mat4(1.0f);
		color = glm::vec3(1.0f, 1.0f, 0.0f); // Color amarillo
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::translate(model, glm::vec3(6.0f, -7.7f, -0.7f));
		model = glm::scale(model, glm::vec3(tam_piramide, tam_piramide, tam_piramide));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();




		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}

	
		