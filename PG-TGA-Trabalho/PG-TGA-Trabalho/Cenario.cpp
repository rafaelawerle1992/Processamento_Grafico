#include "Cenario.h"


//definição de variáveis
static bool keys[1024];
static bool resized;
static GLuint width, height;

// variáveis de controle
bool direita = false, esquerda = false, colide = false, movedir = false, moveesq = false;

// inicialização das coordenadas dos personagem e dos inimigos.
float xhp = 400, yhp = 150;
float xp1 = 100, yp1 = 700;
float xp2 = 200, yp2 = 600;
float xp3 = 300, yp3 = 750;
float xp4 = 400, yp4 = 900;
float xp5 = 500, yp5 = 650;
float xp6 = 600, yp6 = 800;
float xp7 = 700, yp7 = 700;
float xp = 100, yp = 700;

//variáveis auxiliares de movimentação
float moveY = 1;
float moveback = 0;


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
	window = glfwCreateWindow(width, height, "GameHP_PG_GA", nullptr, nullptr);
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
		movedir = true;
		moveesq = false;
		if (xhp <= width-50) 
			xhp += 50;
	}
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
		esquerda = true;
		movedir = false;
		moveesq = true;
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

void Cenario::colisao(float xp, float yp, float xh, float yh)
{
	if (xh == xp && yp <= yh + 40 && yp > yh - 40)
		colide = true;
}

void Cenario::update()
{
	if (keys[GLFW_KEY_ESCAPE])
		glfwSetWindowShouldClose(window, GL_TRUE);

	//Transformação PERSONAGEM com uso das setas direita e esquerda
	if (direita) {
		objects[5]->setPosition(glm::vec3(xhp, yhp, 0));
		unsigned int texID = loadTexture("../textures/hpdir.png");
		objects[5]->setTexture(texID);
		direita = false;
	}
	if (esquerda) {
		objects[5]->setPosition(glm::vec3(xhp, yhp, 0));
		unsigned int texID = loadTexture("../textures/hpesq.png");
		objects[5]->setTexture(texID);
		esquerda = false;
	}		

	//Transformação OBJETO 1
	if (yp1 >= -20) yp1 -= moveY;
	else yp1 = 600;
	objects[6]->setPosition(glm::vec3(xp1, yp1, 0));
	colisao(xp1, yp1, xhp, yhp);	

	//Transformação OBJETO 2
	if (yp2 >= -20) yp2 -= moveY;
	else yp2 = 600;
	objects[7]->setPosition(glm::vec3(xp2, yp2, 0));
	colisao(xp2, yp2, xhp, yhp);

	//Transformação OBJETO 3
	if (yp3 >= -20) yp3 -= moveY;
	else yp3 = 600;
	objects[8]->setPosition(glm::vec3(xp3, yp3, 0));
	colisao(xp3, yp3, xhp, yhp);

	//Transformação OBJETO 4
	if (yp4 >= -20) yp4 -= moveY;
	else yp4 = 600;
	objects[9]->setPosition(glm::vec3(xp4, yp4, 0));
	colisao(xp4, yp4, xhp, yhp);

	//Transformação OBJETO 5
	if (yp5 >= -20) yp5 -= moveY;
	else yp5 = 600;
	objects[10]->setPosition(glm::vec3(xp5, yp5, 0));	
	colisao(xp5, yp5, xhp, yhp);

	//Transformação OBJETO 6
	if (yp6 >= -20) yp6 -= moveY;
	else yp6 = 600;
	objects[11]->setPosition(glm::vec3(xp6, yp6, 0));
	colisao(xp6, yp6, xhp, yhp);

	//Transformação OBJETO 7
	if (yp7 >= -20) yp7 -= moveY;
	else yp7 = 600;
	objects[12]->setPosition(glm::vec3(xp7, yp7, 0));
	colisao(xp7, yp7, xhp, yhp);
	
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
		if (colide) {
			finish();
		}

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

void Cenario::setupBackground()
{
	Sprite* obj;
	//CRIAÇÃO DO BACKGROUND (objetos indices de [0] a [4])
	for (int i = 0; i < 5; i++) {
		obj = new Sprite;
		obj->setPosition(glm::vec3(width / 2, height / 2, 0.0));
		obj->setDimension(glm::vec3(width, height, 1.0f));
		obj->setAngle(glm::radians(180.0f));
		obj->setShader(shader);
		objects.push_back(obj);
	}

	//Carregando TEXTURAS
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
}

void Cenario::setupScene()
{
	setupBackground();

	Sprite* obj;

	//CRIAÇÃO DO PERSONAGEM (indice do objeto [5])
	obj = new Sprite;
	obj->setPosition(glm::vec3(xhp, yhp, 0.0));
	obj->setDimension(glm::vec3(70, 100, 1.0f));
	obj->setAngle(glm::radians(180.0f));
	obj->setShader(shader);
	objects.push_back(obj);

	//CRIAÇÃO DOS BALAÇOS (indice dos objetos de [6] até [12])
	for (int i = 0; i < 7; i++) {
		obj = new Sprite;
		obj->setPosition(glm::vec3(100.0f, 600, 0.0));
		obj->setDimension(glm::vec3(40.0f, 40.0f, 1.0f));
		obj->setAngle(glm::radians(180.0f));
		obj->setShader(shader);
		objects.push_back(obj);
	}

	//Carregando TEXTURAS
	unsigned int texID = loadTexture("../textures/hp1.png");
	objects[5]->setTexture(texID); 
	texID = loadTexture("../textures/balaco.png");
	objects[6]->setTexture(texID);
	objects[7]->setTexture(texID);
	objects[8]->setTexture(texID);
	objects[9]->setTexture(texID);
	objects[10]->setTexture(texID);
	objects[11]->setTexture(texID);
	objects[12]->setTexture(texID);

	//Definindo tamanho da janela
	ortho2D[0] = 0.0f; 
	ortho2D[1] = 800.0f; 
	ortho2D[2] = 0.0f; 
	ortho2D[3] = 600.0f; 

	//Habilita transparência
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


}

void Cenario::setupCamera2D() //TO DO: parametrizar aqui
{
	projection = glm::ortho(ortho2D[0], ortho2D[1], ortho2D[2], ortho2D[3], -1.0f,1.0f);
	GLint projLoc = glGetUniformLocation(shader->ID, "projection");
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