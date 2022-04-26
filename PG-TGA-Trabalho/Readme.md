# Trabalho GA
#
ENTREGA DO TRABALHO DO GRAU A

- Realizei a criação das Sprites, configuração das texturas, algumas tranformações, movimento do personagem e finalização da aplicação em caso de colisão.
- Nao consegui realizar o efeito parallax com as camadas de background e nem a animação do personagem.
#

## Tela do Jogo
- Objetivo: Mover-se nas direções direita e esquerda para não ser atingido pelos balaços.
![image](https://user-images.githubusercontent.com/58199187/165349689-14bd585b-82af-4b05-97b7-8a50fc234adb.png)
#
## Código

- Classe Source
Inicialização da aplicação e chamada da classe Cenario, passando o tamanho da janela.
![image](https://user-images.githubusercontent.com/58199187/165343464-e002ffc1-7435-4857-a0e8-01328a85bbc5.png)
#
- Classe Cenario
Inicialização das variáveis que serão usadas para controlar as tansformações.<br/>
![image](https://user-images.githubusercontent.com/58199187/165344065-28acbb1b-b48c-47ad-85f7-a5c4989027bf.png)
#
Alguns métodos de inicialização que não realizei nenhuma alteração, segue o mesmo código usado nas aulas.<br/>
![image](https://user-images.githubusercontent.com/58199187/165344252-eed3e2cb-2067-48b5-a89b-411afda7e8d1.png)
#
Método Key_callback, atualizei para reconhecer a entrada das teclas RIGHT e LEFT, ambas atualizam as variaveis booleanas de contrle de direção.<br/>
E atualizam o valor da posição do personagem.<br/>
![image](https://user-images.githubusercontent.com/58199187/165344471-a0234747-2018-4d2e-bfe2-caa390f55e33.png)
#
Método da Viewport também não realizei nenhuma modificação. Criei o Método de Colisão, que verifica se o ponto central do inimigo atinge o personagem.<br/>
![image](https://user-images.githubusercontent.com/58199187/165344954-2ca5e152-dc9d-4148-b46f-9d5b0f4621b8.png)
#
Método update, onde são realizadas as transformações.<br/>
As transformações do personagem, basicamente movimentei apenas o X, pois ele move-se apenas para direita e esquerda. Coloquei um limite para que ele não ulttrapasse os limites da viewport. E defini que a cada click do botão ele se mova 50 pixels<br/>
![image](https://user-images.githubusercontent.com/58199187/165345350-d4b9de8f-d801-4331-be79-4b6b80c223c1.png)
#
A transfomação dos inimigos, eu repeti, tentei fazer um laço for, mas a execução nunca saia como eu queria, então a forma que consegui fazer foi assim repetitivo mesmo.<br/>
Cada inimigo inicia em uma determinada posição, e ao chegarem no limite inferior da tela eu setei para o topo da tela novamente.<br/>
A posição X deles é fixa, determinei no início da classe.
![image](https://user-images.githubusercontent.com/58199187/165346281-e6045def-d5a8-4300-93c6-9a870782d853.png)<br/>
Modifiquei a textura dependendo da direção que o personagem está.<br/>
![image](https://user-images.githubusercontent.com/58199187/165348639-0853ba9c-1a2e-499a-b7d1-606a46e2218d.png)
![image](https://user-images.githubusercontent.com/58199187/165349976-3a950d35-45cc-4844-834d-f1efb0ec5b35.png)
#
Outro método que utilizei os códigos padrões sem nenhuma alteração.<br/>
![image](https://user-images.githubusercontent.com/58199187/165347094-3c4fb092-8dd6-4816-a302-cb378361e246.png)
#
No método Run, apenas acresencentei a validação se houve ou não colisão.<br/>
![image](https://user-images.githubusercontent.com/58199187/165347366-19ab3f64-a2c6-4c2d-8384-02ab4c16a5af.png)
#
Criação do Background com sprites e carregamento das texturas.<br/>
![image](https://user-images.githubusercontent.com/58199187/165347532-5dc5d0ec-4149-4cc3-9066-07aaeb67a1b1.png)
#
Mesma coisa para personagem e inimigos.<br/>
![image](https://user-images.githubusercontent.com/58199187/165347652-705f466d-87e9-48d8-81f4-2fb38b88531d.png)
#
E o restante da classe são códigos que também são padrão das aulas
![image](https://user-images.githubusercontent.com/58199187/165347852-8d56a1e9-b286-4b86-b9a3-05813e5444d8.png)
#


