#include <stdio.h>
#include <glew.h>
#include <glfw3.h>
//Dimensiones de la ventana
const int WIDTH = 800, HEIGHT = 600;

int main()
{
	//Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	GLFWwindow *mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Practica 1  Ejercicio 1", NULL, NULL); // Crea el contenedor

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tamaño de Buffer
	int BufferWidth, BufferHeight; // Espacio de memoria de video de la ventana
	glfwGetFramebufferSize(mainWindow, &BufferWidth, &BufferHeight);

	//asignar el contexto
	/* El contenendor recibira los datos de teclado y de mouse */
	glfwMakeContextCurrent(mainWindow); // Indica que la ventana que acabamos de crear, es la ventana activa


	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// Asignar valores de la ventana y coordenadas
	//Asignar Viewport
	glViewport(0, 0, BufferWidth, BufferHeight); // Se acomoda en el origen del espacio disponible
	printf("Version de Opengl: %s \n",glGetString(GL_VERSION));
	printf("Marca: %s \n", glGetString(GL_VENDOR));
	printf("Renderer: %s \n", glGetString(GL_RENDERER));
	printf("Shaders: %s \n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	//Loop mientras no se cierra la ventana

	int counter = 0, counter2 = 0;
	float rojo = 0.0f;
	float verde = 0.0f;
	float azul = 0.0f;

	while (!glfwWindowShouldClose(mainWindow))
	{
		/* El procesador  determina cada cuando es un ciclo de reloj */
		//Recibir eventos del usuario
		glfwPollEvents(); // Espera algun dato teclado, mouse, joystick

		// Codigo aqui
		counter2++;
		if (counter2 % 50 == 0) {
			counter = (counter2 + 1) % 3;
		}

		if (counter == 1) {
			azul = 0.0f;
			verde = 0.0f;
			rojo = 1.0f;
		}
		else if (counter == 2) {
			rojo = 0.0f;
			azul = 0.0f;
			verde = 1.0f;
		}
		else {
			azul = 1.0f;
			verde = 0.0f;
			rojo = 0.0f;
		}

		//Limpiar la ventana
		glClearColor(rojo, verde, azul,1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(mainWindow);
		// NO ESCRIBIR NADA DESPUES

	}


	return 0;
}