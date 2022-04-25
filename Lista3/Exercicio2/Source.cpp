/* Lista 3 - Exercício 2
* Rafaela Werle
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

const float pi = 3.1416;
bool up = false;
bool down = false;
bool esq = false;
bool dir = false;
float positionX = 1.0;
float positionY = 1.0;



// Protótipo da função de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Protótipos das funções
int setupGeometry();
int generateStar(float raio, int nPoints);
int generateCircle(float raio, int nPoints);

// Dimensões da janela (pode ser alterado em tempo de execução)
const GLuint WIDTH = 800, HEIGHT = 600;


// Função MAIN
int main()
{
	// Inicialização da GLFW
	glfwInit();

	// Criação da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Lista3 - Rafaela Werle", nullptr, nullptr);
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

	// Compilando e buildando o programa de shader
	Shader shader("../shaders/ortho.vs", "../shaders/ortho.fs");

	// Gerando um buffer simples, com a geometria de um triângulo
	float raio = 0.5;
	int nPoints = 50;
	GLuint VAO = generateCircle(raio, nPoints);

	// Enviando a cor desejada (vec4) para o fragment shader
	GLint colorLoc = glGetUniformLocation(shader.ID, "inputColor");
	assert(colorLoc > -1);

	glUseProgram(shader.ID);

	//Criando a Matrix de projeção usando a GLM
	glm::mat4 projection = glm::mat4(1);
	projection = glm::ortho(0.0, 800.0, 0.0, 600.0, -1.0, 1.0);
	//Enviando matriz de projeção
	GLint projLoc = glGetUniformLocation(shader.ID, "projection");
	glUniformMatrix4fv(projLoc, 1, FALSE, glm::value_ptr(projection));

	
	// Loop da aplicação - "game loop"
	while (!glfwWindowShouldClose(window))
	{
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as funções de callback correspondentes
		glfwPollEvents();

		// Definindo as dimensões da viewport com as mesmas dimensões da janela da aplicação
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);

		// Limpa o buffer de cor
		glClearColor(0.8f, 0.8f, 0.8f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT);

		//Criando a Matrix de MODELO usando a GLM
		glm::mat4 model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(400.0, 300.0, 0));
		
		//aplicando uma transformação
		if (up) {
		//CIMA
			model = glm::translate(model, glm::vec3(positionX, positionY, 0));			
		}
		if (down) {
		//BAIXO
			model = glm::translate(model, glm::vec3(positionX, positionY, 0));
		}
		if (esq) {
		//ESQUERDA
			model = glm::translate(model, glm::vec3(positionX, positionY, 0));
		}
		if (dir) {
		//DIREITA
			model = glm::translate(model, glm::vec3(positionX, positionY, 0));
		}
		
		//enviando matrix de modelo
		GLint modelLoc = glGetUniformLocation(shader.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, FALSE, glm::value_ptr(model));

		// Chamada de desenho - drawcall
		glBindVertexArray(VAO);
		glUniform4f(colorLoc, 0.0f, 0.0f, 1.0f, 1.0f); //enviando cor para variável uniform inputColor
		glDrawArrays(GL_TRIANGLE_FAN, 0, nPoints + 2);
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

// Função de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
		up = true;
		positionY += 10.0;
		down = false;
		esq = false;
		dir = false;
	}
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
		up = false;
		down = true;
		positionY -= 10.0;
		esq = false;
		dir = false;
	}
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
		up = false;
		down = false;
		esq= true;
		positionX -= 10.0;
		dir = false;
	}
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
		up = false;
		down = false;
		esq = false;
		dir = true;
		positionX += 10.0;
	}
}

int generateCircle(float raio, int nPoints)
{
	int totalSize = (nPoints + 2) * 3;
	GLfloat* vertices = new GLfloat[totalSize];

	vertices[0] = 0.0; //x
	vertices[1] = 0.0; //y
	vertices[2] = 0.0; //z

	float angle = 0.0;
	float slice = 2 * pi / (GLfloat)nPoints;

	for (int i = 3; i < totalSize; i += 3)
	{
		vertices[i] = raio * cos(angle) * 100;
		vertices[i + 1] = raio * sin(angle) * 100;
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
