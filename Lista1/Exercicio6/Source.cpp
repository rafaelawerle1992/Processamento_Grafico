/* Lista 1 - Exercicio 6
* Rafaela Werle
* 
* Circulo completo - ativar as linhas 70 e 101
* a. Octagno - ativar as linhas 79 e 113
* b. Pentagono - ativar linhas 83 e 113
* c. PacMan - ativar as linhas 70 e 117
* d. Fatia de Pizza - ativar as linhas 70 e 121
* e. Estrela - não consegui fazer
 */

#include <iostream>
#include <string>
#include <assert.h>
#include <math.h>

using namespace std;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

#include "Shader.h"

const float pi = 3.1416;

// Protótipo da função de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Protótipos das função que gera o cirulo
int generateCircle(float raio, int n);

// Dimensões da janela (pode ser alterado em tempo de execução)
const GLuint WIDTH = 800, HEIGHT = 600;

// Função MAIN
int main()
{
	// Inicialização da GLFW
	glfwInit();

	// Criação da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Ola Triangulo! - Rafaela Werle", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da função de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

	// GLAD: carrega todos os ponteiros d funções da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	// Obtendo as informações de versão
	const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	// Definindo as dimensões da viewport com as mesmas dimensões da janela da aplicação
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

		// Compilando e buildando o programa de shader
	Shader shader("../shaders/hello.vs","../shaders/hello.fs");


	// Gerando um buffer simples, com a geometria de um circulo
	
	/* //CIRCULO COMPLETO / PACMAN / FATIA DE PIZZA
	int nPoints = 50;
	*/

	/* //OCTAGONO 
	int nPoints = 8;
	*/

	 //PENTAGONO
	int nPoints = 5;
	

	GLuint VAO = generateCircle(0.5, nPoints);

		
	// Enviando a cor desejada (vec4) para o fragment shader
	GLint colorLoc = glGetUniformLocation(shader.ID, "inputColor");
	assert(colorLoc > -1);

	glUseProgram(shader.ID);
	
	// Loop da aplicação - "game loop"
	while (!glfwWindowShouldClose(window))
	{
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as funções de callback correspondentes
		glfwPollEvents();

		// Limpa o buffer de cor
		glClearColor(0.8f, 0.8f, 0.8f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT);

		// Chamada de desenho - drawcall
		glBindVertexArray(VAO);
		glUniform4f(colorLoc, 0.0f, 0.0f, 1.0f, 1.0f); //enviando cor para variável uniform inputColor

		/* //CIRCULO COMPLETO		
		glDrawArrays(GL_TRIANGLE_FAN, 0, nPoints+2);
		*/

		 //OCTAGONO / PENTAGONO
		glDrawArrays(GL_TRIANGLE_FAN, 0, nPoints+2);
		

		/* //PACMAN		
		glDrawArrays(GL_TRIANGLE_FAN, 0, nPoints-2);
		*/

		/* //FATIA DE PIZZA		
		glDrawArrays(GL_TRIANGLE_FAN, 0, nPoints/5);
		*/

		/*ESTRELA*/

							
		glBindVertexArray(0);

		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}

	// Pede pra OpenGL desalocar os buffers
	glDeleteVertexArrays(1, &VAO);
	// Finaliza a execução da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

int generateCircle(float raio, int nPoints)
{
	int totalSize = (nPoints + 2) * 3;
	GLfloat* vertices = new GLfloat[totalSize];

	vertices[0] = 0.0; //x
	vertices[1] = 0.0; //y
	vertices[2] = 0.0; //z

	float angle = 0.0;
	float slice = 2 * pi / (GLfloat) nPoints;
	for (int i = 3; i < totalSize; i += 3)
	{
		vertices[i] = raio * cos(angle);
		vertices[i + 1] = raio * sin(angle);
		vertices[i + 2] = 0.0;

		angle += slice;
	}

	GLuint VBO, VAO;

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, totalSize * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return VAO;

}