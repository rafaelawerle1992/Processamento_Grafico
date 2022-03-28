#include <iostream>
#include <string>
#include <assert.h>

using namespace std;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

#include "Shader.h"

// Protótipo da função de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Protótipos das funções
int setupGeometry();

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

	// Gerando um buffer simples, com a geometria de um triângulo
	GLuint VAO = setupGeometry();
	
	// Enviando a cor desejada (vec4) para o fragment shader
	// Utilizamos a variáveis do tipo uniform em GLSL para armazenar esse tipo de info
	// que não está nos buffers
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

		glLineWidth(10);
		glPointSize(10);

		// Chamada de desenho - drawcall
		glBindVertexArray(VAO);

		/*
		//a. Apenas com o polígono preenchido
		// POLIGONO - GL_TRIANGLES
		glUniform4f(colorLoc, 0.0f, 0.0f, 1.0f, 1.0f); //enviando cor para variável uniform inputColor
		glDrawArrays(GL_TRIANGLES, 0, 6);
		*/
		
		/*
		//b. Apenas com contorno  
		// CONTORNO - GL_LINE_LOOP
		glUniform4f(colorLoc, 0.0f, 1.0f, 0.0f, 1.0f); //enviando cor para variável uniform inputColor
		glDrawArrays(GL_LINE_LOOP, 0, 6);
		*/
		
		/*
		//c. Apenas como pontos  
		// PONTOS - GL_POINTS
		glUniform4f(colorLoc, 1.0f, 0.0f, 0.0f, 1.0f); //enviando cor para variável uniform inputColor
		glDrawArrays(GL_POINTS, 0, 6);
		*/

		
		//d. Com as 3 formas de desenho juntas 
		// POLIGONO - GL_TRIANGLES
		glUniform4f(colorLoc, 0.0f, 0.0f, 1.0f, 1.0f); //enviando cor para variável uniform inputColor
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// CONTORNO - GL_LINE_LOOP
		glUniform4f(colorLoc, 0.0f, 1.0f, 0.0f, 1.0f); //enviando cor para variável uniform inputColor
		glDrawArrays(GL_LINE_LOOP, 0, 6);
		// PONTOS - GL_POINTS
		glUniform4f(colorLoc, 1.0f, 0.0f, 0.0f, 1.0f); //enviando cor para variável uniform inputColor
		glDrawArrays(GL_POINTS, 0, 6);

				
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


int setupGeometry()
{
	GLfloat vertices[] = {
		0.0, -0.5, 0.0,
		 0.5, -0.5, 0.0,
		 0.0, 0.5, 0.0,
		 0.0, -0.5, 0.0,
		 -0.5, -0.5, 0.0,
		 -0.5, 0.5, 0.0,
	};

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

