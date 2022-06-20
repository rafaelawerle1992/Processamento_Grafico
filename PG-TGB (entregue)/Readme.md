<h1> Trabalho Prático de Processamento Gráfico </h1>

Processamento de imagens utilizando OpenCV

<h2> Tela da Aplicação: </h2>

- Criei 17 filtros possíveis, apenas um filtro por vez, com excessão do Blur e do Binário, que podem ser aplicados sobre os demais.
- Criei 5 opções de Sticker, com tamanho pré-definido. É necesário selecionar o Sticker e depois clicar na foto para aplicar.
- Criei 3 botões de menu: Salvar, Reverter, Alternar Foto/Camera.

![image](https://user-images.githubusercontent.com/58199187/174677833-a7be11f8-b58f-4b9f-9aac-2e7338fa1058.png)



<h2> Código: </h2>

- Importação de bibliotecas e declaração das variáveis:

![image](https://user-images.githubusercontent.com/58199187/174450660-4b02a0dc-9faa-4937-983f-0ed02bc9317a.png)

- Funções pré-definidas 'overlayImage' e 'interpolation'

Não modifiquei essas duas funções, copiei prontas para o código. A primeira é para realizar a colagem de uma imagem em outra e a segunda para alteração de cores nos pixels.

![image](https://user-images.githubusercontent.com/58199187/174450694-6a87eb62-4622-476b-88af-1c20215e9d82.png)
![image](https://user-images.githubusercontent.com/58199187/174450698-e59ad9ea-401a-4a76-8c11-89845eb16079.png)

- Funções criadas para os filtros. 10 no total.

![image](https://user-images.githubusercontent.com/58199187/174450818-e8a156cf-68dc-4ed5-b48e-013f4d4a5a3e.png)

- Função que aplica o Sticker na imagem.

Primeiramente verifica se existe um sticker selecionado e então cola em cima da foto atual usando a função overlayImage.

![image](https://user-images.githubusercontent.com/58199187/174450875-97d8405d-f8d8-4eda-8fee-93fdcfb66558.png)

- Função Carrega Menu.

Criei essa função para desenhar todas as miniaturas e botões sobre a imagem de fundo. 
Primeiramente criei os botões e a imagem de fundo no CorelDraw e salvei como .jpg pra poder utilizar no trabalho.

Mapeei todas as posições que eu queria que ficassem os elementos sobre a imagem de fundo, e utilizei a função overlayImage para desenhar.

![image](https://user-images.githubusercontent.com/58199187/174450988-d4af8273-4c0e-4cc0-8968-d6c10e07cbb6.png)
![image](https://user-images.githubusercontent.com/58199187/174451041-86834263-291a-49c8-81c1-c687cdd856e5.png)
![image](https://user-images.githubusercontent.com/58199187/174451048-666ed46f-54ed-4221-b5bb-99ddbb1d3808.png)

- Função para ligar a webcam

![image](https://user-images.githubusercontent.com/58199187/174451075-4f59296a-3b73-41dc-abce-91750645f692.png)

Dependendo do filtro que for selecionado, aplico ele após caturar cada frame da camera.

![image](https://user-images.githubusercontent.com/58199187/174452637-6f9b82c2-be1c-44a5-ac63-bb6f8865cbfe.png)


- Função para salvar a imagem como jpg.

![image](https://user-images.githubusercontent.com/58199187/174451107-207044c3-a72a-4ce8-b760-151906250baa.png)

- Método de captura do mouse.

Como eu já tinha mapeado todas as posições dos elementos (miniaturas, botões e imagem) utilizei a função para decidir o que fazer dependendo de onde fosse clicado na tela.

![image](https://user-images.githubusercontent.com/58199187/174451146-ff6096c7-7f5c-4e11-87eb-b6555bd536f6.png)

- Função Main

![image](https://user-images.githubusercontent.com/58199187/174451169-95ce2269-1c65-43ce-a67d-20c0e7e6f0ab.png)

![image](https://user-images.githubusercontent.com/58199187/174451178-7b474040-5b50-4d34-9fea-ac22cc74b4d3.png)

