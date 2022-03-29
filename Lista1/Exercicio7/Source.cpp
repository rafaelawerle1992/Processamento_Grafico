/* Lista 1 - Exercicio 7
* Rafaela Werle
* 
* Variáveis de entrada nas linhas 70, 71, 72 - 
quantidade de vertices, angulo entre os vertices e raio inicial.
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
int generateElipse(float raio, int nPoints, float vAngle );

// Dimensões da janela (pode ser alterado em tempo de execução)
const GLuint WIDTH = 800, HEIGHT = 600;

// Função MAIN
int main()
{
	// Inicialização da GLFW
	glfwInit();

	// Criação da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Lista1 - Exercício7 - Rafaela Werle", nullptr, nullptr);
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


	// Gerando um buffer simples, com a geometria de uma elipse
	
	int nPoints = 200;
	float raio = 0.1;
	float angulo = 10.0;
	GLuint VAO = generateElipse (raio, nPoints, angulo);

		
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

		glPointSize(10);

		// Chamada de desenho - drawcall
		glBindVertexArray(VAO);
		glUniform4f(colorLoc, 1.0f, 0.0f, 1.0f, 1.0f); //enviando cor para variável uniform inputColor
		glDrawArrays(GL_LINE_STRIP, 1, nPoints-1);
		
							
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

int generateElipse(float raio, int nPoints, float angulo)
{
	int totalSize = (nPoints) * 3;
	GLfloat* vertices = new GLfloat[totalSize];

	vertices[0] = 0.0; //x
	vertices[1] = 0.0; //y
	vertices[2] = 0.0; //z

	float vraio = (raio/360) * angulo; //variação no raio, dependente da variação de angulo de cada ponto.
	float vangulo = (angulo / 180) * pi; // calculo para variação de angulo (pode ser escolhido o angulo entre os pontos.)

	float angle = 0.0;

	for (int i = 3; i < totalSize; i += 3)
	{
		vertices[i] = raio * cos(angle);
		vertices[i + 1] = raio * sin(angle);
		vertices[i + 2] = 0.0;

		raio += vraio;
		angle += vangulo;
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