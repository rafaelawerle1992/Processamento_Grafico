#include "Cenario.h"

//definição de variáveis
static bool keys[1024];
static bool resized;
static GLuint width, height;
bool direita = false;
bool esquerda = false;
float xhp = 400;
float yhp = 150;
float move1 = 600;
float move2 = 700;
float move3 = 750;
float move4 = 900;
float move5 = 800;


Cenario::Cenario()
{
}

Cenario::~Cenario()
{
}

void Cenario::initialize(GLuint w, GLuint h)
{
	width = w;
	height = h;	
	initializeGraphics();
}

void Cenario::initializeGraphics()
{
	// Inicia GLFW
	glfwInit();

	// Criação da janela GLFW
	window = glfwCreateWindow(width, height, "Game_Pomo_de _Ouro", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da função de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

	//Setando a callback de redimensionamento da janela
	glfwSetWindowSizeCallback(window, resize);
	
	// GLAD: carrega todos os ponteiros d funções da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	// Compilando e buildando o programa de shader
	addShader("../shaders/sprite.vs", "../shaders/sprite.fs");

	//Configurando o Cenario
	setupScene();
	

	resized = true; 
}

void Cenario::addShader(string vFilename, string fFilename)
{
	shader = new Shader (vFilename.c_str(), fFilename.c_str());
}

void Cenario::key_callback(GLFWwindow * window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
		direita = true;
		if (xhp <= width-50) 
			xhp += 50;
	}
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
		esquerda = true;
		if (xhp >= 50 ) 
			xhp -= 50;
	}
}

void Cenario::resize(GLFWwindow * window, int w, int h)
{
	width = w;
	height = h;
	resized = true;

	// Definição da ViewPort
	glViewport(0, 0, width, height);
}

void Cenario::update()
{
	if (keys[GLFW_KEY_ESCAPE])
		glfwSetWindowShouldClose(window, GL_TRUE);

	//Transformação PERSONAGEM com uso das setas direita e esquerda
	if (direita) {
		objects[10]->setPosition(glm::vec3(xhp, yhp, 0));
		direita = false;
	}
	if (esquerda) {
		objects[10]->setPosition(glm::vec3(xhp, yhp, 0));
		esquerda = false;
	}

	//Transformação OBJETO 1
	if (move1 >= -20) move1 -= 0.2;
	objects[5]->setPosition(glm::vec3(100, move1, 0));

	//Transformação OBJETO 2
	if (move2 >= -20) move2 -= 0.2;
	objects[6]->setPosition(glm::vec3(200, move2, 0));

	//Transformação OBJETO 3
	if (move3 >= -20) move3 -= 0.3;
	objects[7]->setPosition(glm::vec3(400, move3, 0));

	//Transformação OBJETO 4
	if (move4 >= -20) move4 -= 0.3;
	objects[8]->setPosition(glm::vec3(550, move4, 0));

	//Transformação OBJETO 5
	if (move5 >= -20) move5 -= 0.2;
	objects[9]->setPosition(glm::vec3(700, move5, 0));
	//objects[1]->setAngle((float)glfwGetTime());

}

void Cenario::render()
{
	// Limpa o buffer de cor
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//se houve redimensionamento na janela, redefine a projection matrix
	if (resized) 
	{	setupCamera2D();
		resized = false;
	}

	//atualiza e desenha os Sprites
	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->update();
		objects[i]->draw();
	}
}

void Cenario::run()
{
	//GAME LOOP
	while (!glfwWindowShouldClose(window))
	{
		// Checa se houveram eventos de input
		glfwPollEvents();

		//Update
		update();

		//Render
		render();
		
		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}
}

void Cenario::finish()
{
	// Finaliza a execução da GLFW, limpando os recursos alocados por ela.
	glfwTerminate();
}


void Cenario::setupScene()
{
	Sprite* obj;
	//CRIAÇÃO DO BACKGROUND (5 CAMADAS)
	for (int i = 0; i < 5; i++) {
		obj = new Sprite;
		obj->setPosition(glm::vec3(width / 2, height / 2, 0.0));
		obj->setDimension(glm::vec3(width, height, 1.0f));
		obj->setAngle(glm::radians(180.0f));
		obj->setShader(shader);
		objects.push_back(obj);
	}

	//CRIAÇÃO DOS POMOS
	for (int i = 0; i < 5; i++) {
		obj = new Sprite;
		obj->setPosition(glm::vec3(100.0f, 600, 0.0));
		obj->setDimension(glm::vec3(40.0f, 40.0f, 1.0f));
		obj->setAngle(glm::radians(180.0f));
		obj->setShader(shader);
		objects.push_back(obj);
	}

	//CRIAÇÃO DO PERSONAGEM
	obj = new Sprite;
	obj->setPosition(glm::vec3(xhp, yhp, 0.0));
	obj->setDimension(glm::vec3(70, 90, 1.0f));
	obj->setAngle(glm::radians(180.0f));
	obj->setShader(shader);
	objects.push_back(obj);

	
	unsigned int texID = loadTexture("../textures/camada7.png");
	objects[0]->setTexture(texID);

	texID = loadTexture("../textures/camada4.png");
	objects[1]->setTexture(texID);

	texID = loadTexture("../textures/camada3.png");
	objects[2]->setTexture(texID);

	texID = loadTexture("../textures/camada2.png");
	objects[3]->setTexture(texID);

	texID = loadTexture("../textures/camada1.png");
	objects[4]->setTexture(texID);


	texID = loadTexture("../textures/pomo.png");
	objects[5]->setTexture(texID);
	objects[6]->setTexture(texID);
	objects[7]->setTexture(texID);
	objects[8]->setTexture(texID);
	objects[9]->setTexture(texID);

	texID = loadTexture("../textures/hp1.png");
	objects[10]->setTexture(texID);

	//Definindo a janela do mundo (ortho2D)
	ortho2D[0] = 0.0f; //xMin
	ortho2D[1] = 800.0f; //xMax
	ortho2D[2] = 0.0f; //yMin
	ortho2D[3] = 600.0f; //yMax

	//Habilita transparência
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


}

void Cenario::setupCamera2D() //TO DO: parametrizar aqui
{
	float zNear = -1.0, zFar = 1.0; //estão fixos porque não precisamos mudar

	projection = glm::ortho(ortho2D[0], ortho2D[1], ortho2D[2], ortho2D[3], zNear, zFar);


	//Obtendo o identificador da matriz de projeção para enviar para o shader
	GLint projLoc = glGetUniformLocation(shader->ID, "projection");
	//Enviando a matriz de projeção para o shader
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

unsigned int Cenario::loadTexture(string filename)
{
	unsigned int texture;

	// load and create a texture 
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
										   // set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
	
	if (data)
	{
		if (nrChannels == 3) //jpg, bmp
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else //png
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

	glActiveTexture(GL_TEXTURE0);

	return texture;
}
