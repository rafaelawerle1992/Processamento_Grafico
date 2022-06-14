# Hello OpenCV 

Projeto com o exemplo Sample do OpenCV e captura de câmera - v.1.0 - Para propósitos didáticos

Por Rossana B Queiroz 

### Resumindo as etapas:

1) Baixar o OpenCV e descompactar (dá mais de 1GB!)
2) Baixar o CMake
3) No CMake, gerar o .sln do Visual Studio:

![image](https://user-images.githubusercontent.com/2465857/167528383-ae1f80be-40a8-41f2-84ac-e6fb307c777e.png)

4) No Visual Studio (ou IDE de pref.), compilar todo o projeto
5) Clonar este projeto 
6) Testar este projeto, mudando o caminho dos seguintes 
diretórios nas propriedades do projeto:

  - Em C/C++ -> Geral -> Diretórios de Inclusão Adicionais:
    - _<seu_diretorio>_\opencv\sources\modules\imgcodecs\include
    - _<seu_diretorio>_\opencv\sources\modules\imgproc\include
    - _<seu_diretorio>_\opencv\sources\modules\core\include
    - _<seu_diretorio>_\opencv\sources\modules\calib3d\include
    - _<seu_diretorio>_\opencv\sources\modules\features2d\include
    - _<seu_diretorio>_\opencv\sources\modules\dnn\include
    - _<seu_diretorio>_\opencv\sources\modules\flann\include
    - _<seu_diretorio>_\opencv\sources\modules\videoio\include
    - _<seu_diretorio>_\opencv\sources\modules\video\include
    - _<seu_diretorio>_\opencv\sources\modules\stitching\include
    - _<seu_diretorio>_\opencv\sources\modules\photo\include
    - _<seu_diretorio>_\opencv\sources\modules\objdetect\include
    - _<seu_diretorio>_\opencv\sources\modules\ml\include
    - _<seu_diretorio>_\opencv\sources\modules\highgui\include
    - _<seu_diretorio>_\opencv\sources\modules\gapi\include
    - _<seu_diretorio>_\opencv\build\3rdparty\ippicv\ippicv_win\icv\include
    - _<seu_diretorio>_\opencv\build\3rdparty\ippicv\ippicv_win\iw\include
    - _<seu_diretorio>_\opencv\build

  - Em Vinculador -> Geral -> Diretórios de Biblioteca Adicionais:
    - _<seu_diretorio>_\opencv\build\3rdparty\lib\Debug

  - Em Vinculador -> Entrada -> Dependências Adicionais:
    - _<seu_diretorio>_\opencv\build\3rdparty\lib\Debug\ippiwd.lib
    - _<seu_diretorio>_\opencv\build\3rdparty\ippicv\ippicv_win\icv\lib\ia32\ippicvmt.lib
    - _<seu_diretorio>_\opencv\build\lib\Debug\opencv_gapi455d.lib
    - _<seu_diretorio>_\opencv\build\lib\Debug\opencv_highgui455d.lib
    - _<seu_diretorio>_\opencv\build\lib\Debug\opencv_ml455d.lib
    - _<seu_diretorio>_\opencv\build\lib\Debug\opencv_objdetect455d.lib
    - _<seu_diretorio>_\opencv\build\lib\Debug\opencv_photo455d.lib
    - _<seu_diretorio>_\opencv\build\lib\Debug\opencv_stitching455d.lib
    - _<seu_diretorio>_\opencv\build\lib\Debug\opencv_video455d.lib
    - _<seu_diretorio>_\opencv\build\lib\Debug\opencv_videoio455d.lib
    - _<seu_diretorio>_\opencv\build\lib\Debug\opencv_imgcodecs455d.lib
    - _<seu_diretorio>_\opencv\build\lib\Debug\opencv_calib3d455d.lib
    - _<seu_diretorio>_\opencv\build\lib\Debug\opencv_features2d455d.lib
    - _<seu_diretorio>_\opencv\build\lib\Debug\opencv_flann455d.lib
    - _<seu_diretorio>_\opencv\build\lib\Debug\opencv_dnn455d.lib
    - _<seu_diretorio>_\opencv\build\lib\Debug\opencv_imgproc455d.lib
    - _<seu_diretorio>_\opencv\build\lib\Debug\opencv_core455d.lib

  - Além disso, no diretório Debug gerado após a primeira compilação, é necessário colocar as .dlls (ou configurar no projeto para pegar do diretório do OpenCV onde elas estão). O diretório é: 
    - _<seu_diretorio>_\opencv\build\bin\Debug -> São todas as dlls com nomes correspondentes às .lib
