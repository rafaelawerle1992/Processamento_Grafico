/* Lista 2 - Exercicio 1, 2, 3
* Rafaela Werle
*
* EXERC�CIO 1 - ATIVAR AS LINHAS 79 e 136.
* EXERC�CIO 2 - ATIVAR AS LINHAS 82 e 136.
* EXERC�CIO 3 - ATIVAR AS LINHAS 85 e 145.
 */

#include <iostream>
#include <string>
#include <assert.h>

using namespace std;

// GLAD
#include <glad/glad.h>
// GLFW
#include <GLFW/glfw3.h>
//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// SHADER
#include "Shader.h"

// Prot�tipo da fun��o de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Prot�tipos das fun��es
int setupGeometry();

// Dimens�es da janela (pode ser alterado em tempo de execu��o)
const GLuint WIDTH = 800, HEIGHT = 600;


// Fun��o MAIN
int main()
{
	// Inicializa��o da GLFW
	glfwInit();

	// Cria��o da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Lista2 - Rafaela Werle", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da fun��o de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

	// GLAD: carrega todos os ponteiros d fun��es da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	// Obtendo as informa��es de vers�o
	const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	// Compilando e buildando o programa de shader
	Shader shader("../shaders/ortho.vs","../shaders/ortho.fs");

	// Gerando um buffer simples, com a geometria de um tri�ngulo
	GLuint VAO = setupGeometry();
	
	// Enviando a cor desejada (vec4) para o fragment shader
	GLint colorLoc = glGetUniformLocation(shader.ID, "inputColor");
	assert(colorLoc > -1);
	
	glUseProgram(shader.ID);

	//Criando a Matrix de proje��o usando a GLM
	glm::mat4 projection = glm::mat4(1);
	
	//EXERC�CIO 1
	projection = glm::ortho(-10.0, 10.0, -10.0, 10.0, -1.0, 1.0);

	//EXERC�CIO 2
	//projection = glm::ortho(0.0, 800.0, 600.0, 0.0, -1.0, 1.0);

	//EXERC�CIO 3 - retirar tamb�m coment�rio da linha
	//projection = glm::ortho(0.0, 800.0, 600.0, 0.0, -1.0, 1.0);
	
	GLint projLoc = glGetUniformLocation(shader.ID, "projection");
	glUniformMatrix4fv(projLoc, 1, FALSE, glm::value_ptr(projection));

	// Loop da aplica��o - "game loop"
	while (!glfwWindowShouldClose(window))
	{
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as fun��es de callback correspondentes
		glfwPollEvents();

		// Definindo as dimens�es da viewport com as mesmas dimens�es da janela da aplica��o
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);

		// Limpa o buffer de cor
		glClearColor(0.8f, 0.8f, 0.8f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT);

		// Chamada de desenho - drawcall
		glBindVertexArray(VAO);		
		// POLIGONO - GL_TRIANGLES
		glUniform4f(colorLoc, 0.0f, 0.0f, 1.0f, 1.0f); //enviando cor para vari�vel uniform inputColor
		glDrawArrays(GL_TRIANGLES, 0, 6);
						
		glBindVertexArray(0);

		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}
	// Pede pra OpenGL desalocar os buffers
	glDeleteVertexArrays(1, &VAO);
	// Finaliza a execu��o da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;
}

// Fun��o de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}


int setupGeometry()
{
	
	//EXERC�CIO 1, 2
	
	GLfloat vertices[] = {
		-5.0, -5.0, 0.0,
		 5.0, -5.0, 0.0,
		 0.0, 5.0, 0.0,
	};
	
	
	//EXERC�CIO 3
	/*
	GLfloat vertices[] = {
		100.0, 0.0, 0.0,
		 700.0, 0.0, 0.0,
		 400.0, 300.0, 0.0,
	};
	*/

	
	GLuint VBO, VAO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0); 

	glBindVertexArray(0); 

	return VAO;
}

