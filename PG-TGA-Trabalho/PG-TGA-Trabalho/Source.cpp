/* :: TRABALHO DE PROCESSAMENTO GRAFICO 
 * :: 2022/01
 * :: GRAU A
 * :: RAFAELA CUNHA WERLE
 * ::
 */

using namespace std;
#include "Cenario.h"

// INICIALIZAÇÃO DO CENÁRIO E DO GAME
int main()
{
	Cenario*game = new Cenario;
	game->initialize(800, 600);
	game->run();
	game->finish();

	return 0;
}


