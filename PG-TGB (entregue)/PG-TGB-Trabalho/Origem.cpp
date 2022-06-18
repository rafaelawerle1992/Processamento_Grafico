/*  TRABALHO DE PROCESSAMENTO GRAFICO - GRAU B
*   RAFAELA WERLE
* 
*   PROCESSAMENTO DE IMAGENS COM OPENCV
*
*/ 

//IMPORTAÇÃO DAS BIBLIOTECAS
#include "opencv2/objdetect.hpp"
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/photo.hpp"
#include <iostream>
#include <filesystem>

//-------------------------------------------------------------------------------
//DECLARAÇAO DE VARIÁVEIS
using namespace cv;
using namespace std;

enum mouseEvents { NONE, LEFTBUTTON_DOWN };
int mouseEvent = NONE;
int mousex, mousey;

Mat imgFundo, imgFotoOriginal, imgFotoAtual, imgFotoAtualColorida;
Mat imgFotoMiniatura, imgOlhos, imgGotas, imgFogos, imgFlower, imgCool;
Mat imgSt1, imgSt2, imgSt3, imgSt4, imgSt5, imgSticker;

enum stickers { NENHUM, OLHOS, GOTAS, FOGOS, FLOWER, COOL };
int sticker = NENHUM;

bool ligaCamera = false;
//-------------------------------------------------------------------------------




//-------------------------------------------------------------------------------
//FUNÇÃO OVERLAYIMAGE
void overlayImage(Mat* src, Mat* overlay, const Point& location)
{
    for (int y = max(location.y, 0); y < src->rows; ++y)
    {
        int fY = y - location.y;
        if (fY >= overlay->rows)
            break;

        for (int x = max(location.x, 0); x < src->cols; ++x)
        {
            int fX = x - location.x;
            if (fX >= overlay->cols)
                break;
            double opacity = ((double)overlay->data[fY * overlay->step + fX * overlay->channels() + 3]) / 255;
            for (int c = 0; opacity > 0 && c < src->channels(); ++c)
            {
                unsigned char overlayPx = overlay->data[fY * overlay->step + fX * overlay->channels() + c];
                unsigned char srcPx = src->data[y * src->step + x * src->channels() + c];
                src->data[y * src->step + src->channels() * x + c] = srcPx * (1. - opacity) + overlayPx * opacity;
            }
        }
    }
}
//FUNÇÃO INTERPOLATION
void interpolation(uchar* lut, float* curve, float* originalValue) {
    for (int i = 0; i < 256; i++) {
        int j = 0;
        float a = i;
        while (a > originalValue[j]) {
            j++;
        }
        if (a == originalValue[j]) {
            lut[i] = curve[j];
            continue;
        }
        float slope = ((float)(curve[j] - curve[j - 1])) / ((float)(originalValue[j] - originalValue[j - 1]));
        float constant = curve[j] - slope * originalValue[j];
        lut[i] = slope * a + constant;
    }
}
//-------------------------------------------------------------------------------

//FILTROS
//GRAYSCALE
void filtroCinza(Mat& imgOrigem, Mat& imgSaida) {
    imgOrigem = imgFotoAtualColorida.clone();
    cvtColor(imgOrigem, imgSaida, COLOR_BGR2GRAY);
}
//NEGATIVE
void filtroNegativo(Mat& imgOrigem, Mat& imgSaida) {
    imgOrigem = imgFotoAtualColorida.clone();
    cvtColor(imgOrigem, imgSaida, COLOR_BGR2GRAY);
    for (int i = 0; i < imgSaida.rows; i++) {
        for (int j = 0; j < imgSaida.cols; j++) {
            imgSaida.at<uchar>(i, j) = 255 - imgSaida.at<uchar>(i, j);
        }
    }
}
//CANNY
void filtroCanny(Mat& imgOrigem, Mat& imgSaida) {
    imgOrigem = imgFotoAtualColorida.clone();
    cvtColor(imgOrigem, imgSaida, COLOR_BGR2GRAY);
    Canny(imgSaida, imgSaida, 50, 150);
}
//SOBEL
void filtroSobel(Mat& imgOrigem, Mat& imgSaida) {
    imgOrigem = imgFotoAtualColorida.clone();
    cvtColor(imgOrigem, imgSaida, COLOR_BGR2GRAY);
    Mat grad_x, grad_y;
    Mat abs_grad_x, abs_grad_y;
    Sobel(imgOrigem, grad_x, CV_16S, 1, 0, 1, 1, 0, BORDER_DEFAULT);
    Sobel(imgOrigem, grad_y, CV_16S, 0, 1, 1, 1, 0, BORDER_DEFAULT);
    //Convertendo para o CV_8U 8bits
    convertScaleAbs(grad_x, abs_grad_x);
    convertScaleAbs(grad_y, abs_grad_y);
    addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, imgSaida);
}
//LAPLACE
void filtroLaplace(Mat& imgOrigem, Mat& imgSaida) {
    imgOrigem = imgFotoAtualColorida.clone();
    Mat src_gray, dst;
    int kernel_size = 3;
    int scale = 1;
    int delta = 0;
    int ddepth = CV_16S;    
    GaussianBlur(imgOrigem, imgOrigem, Size(3, 3), 0, 0, BORDER_DEFAULT);
    cvtColor(imgOrigem, src_gray, COLOR_BGR2GRAY); // Convert the image to grayscale
    Laplacian(src_gray, dst, ddepth, kernel_size, scale, delta, BORDER_DEFAULT);
    convertScaleAbs(dst, imgSaida);
}
//BLUR
void filtroBlur(Mat& imgOrigem, Mat& imgSaida) {
    GaussianBlur(imgOrigem, imgSaida, Size(5, 5), 5, 5);
}
//QUENTE
void filtroQuente(Mat& imgOrigem, Mat& imgSaida) {
    
    imgOrigem = imgFotoAtualColorida.clone();
    imgSaida = imgOrigem.clone();

    vector<Mat> channels;
    split(imgSaida, channels);

    float redValuesOriginal[] = { 0, 60, 110, 150, 235, 255 };
    float redValues[] = { 0, 102, 185, 220, 245, 245 };
    float greenValuesOriginal[] = { 0, 68, 105, 190, 255 };
    float greenValues[] = { 0, 68, 120, 220, 255 };
    float blueValuesOriginal[] = { 0, 88, 145, 185, 255 };
    float blueValues[] = { 0, 12, 140, 212, 255 };

    Mat lookupTable(1, 256, CV_8U);
    uchar* lut = lookupTable.ptr();

    interpolation(lut, blueValues, blueValuesOriginal);
    LUT(channels[0], lookupTable, channels[0]);

    interpolation(lut, greenValues, greenValuesOriginal);
    LUT(channels[1], lookupTable, channels[1]);

    interpolation(lut, redValues, redValuesOriginal);
    LUT(channels[2], lookupTable, channels[2]);

    merge(channels, imgSaida);
}
//FRIO
void filtroFrio(Mat& imgOrigem, Mat& imgSaida) {

    imgOrigem = imgFotoAtualColorida.clone();
    imgSaida = imgOrigem.clone();

    vector<Mat> channels;
    split(imgSaida, channels);

    float redValuesOriginal[] = { 0, 88, 145, 185, 255 };
    float redValues[] = { 0, 12, 140, 212, 255 };
    float greenValuesOriginal[] = { 0, 68, 105, 190, 255 };
    float greenValues[] = { 0, 68, 120, 220, 255 };
    float blueValuesOriginal[] = { 0, 60, 110, 150, 235, 255 } ;
    float blueValues[] = { 0, 102, 185, 220, 245, 245 } ;

    Mat lookupTable(1, 256, CV_8U);
    uchar* lut = lookupTable.ptr();

    interpolation(lut, blueValues, blueValuesOriginal);
    LUT(channels[0], lookupTable, channels[0]);

    interpolation(lut, greenValues, greenValuesOriginal);
    LUT(channels[1], lookupTable, channels[1]);

    interpolation(lut, redValues, redValuesOriginal);
    LUT(channels[2], lookupTable, channels[2]);

    merge(channels, imgSaida);
}
//MOON
void filtroMoon(Mat& imgOrigem, Mat& imgSaida) {

    imgOrigem = imgFotoAtualColorida.clone();
    imgSaida = imgOrigem.clone();

    cvtColor(imgOrigem, imgSaida, COLOR_BGR2Lab);

    vector<Mat> channels;
    split(imgSaida, channels);

    float originValues[] = { 0, 15, 30, 50, 70, 90, 120, 160, 180, 210, 255 };
    float values[] = { 0, 0, 5, 15, 60, 110, 150, 190, 210, 230, 255 };

    Mat lookupTable(1, 256, CV_8U);
    uchar* lut = lookupTable.ptr();

    interpolation(lut, values, originValues);
    LUT(channels[0], lookupTable, channels[0]);

    merge(channels, imgSaida);

    cvtColor(imgSaida, imgSaida, COLOR_Lab2BGR);
    cvtColor(imgSaida, imgSaida, COLOR_BGR2HSV);

    split(imgSaida, channels);

    channels[1] = channels[1] * 0.01;
    min(channels[1], 255, channels[1]);
    max(channels[1], 0, channels[1]);

    merge(channels, imgSaida);

    cvtColor(imgSaida, imgSaida, COLOR_HSV2BGR);
}
//BINARIO
void filtroBinario(Mat& imgOrigem, Mat& imgSaida) {
    //Set the threshold and maximum values
    double thresh = 100;
    double maxValue = 255;

    //Binary threshold
    threshold(imgOrigem, imgSaida, thresh, maxValue, THRESH_BINARY);
}

//---------------------------------------------------------------------------
//FUNÇÃO PARA APLICAÇÃO DE STICKERS NA FOTO
void aplicaSticker(double x, double y) {

    if (sticker == NENHUM) {
        cout << "Nenhum sticker selecionado" << endl;
    }
    else if (sticker == OLHOS) {
        cout << "Aplicando sticker olhos" << endl;
        resize(imgOlhos, imgSticker, Size(100, 50));
        overlayImage(&imgFotoAtual, &imgSticker, Point(x - 350 - 50, y - 50 - 25));//posição - desloc da foto no fundo - desloc do sticker
        overlayImage(&imgFotoAtualColorida, &imgSticker, Point(x - 350 - 50, y - 50 - 25));
        sticker = NENHUM;
    }
    else if (sticker == GOTAS) {
        cout << "Aplicando sticker gotas" << endl;
        resize(imgGotas, imgSticker, Size(300, 200));
        overlayImage(&imgFotoAtual, &imgSticker, Point(x - 350 - 150, y - 50 - 100));
        overlayImage(&imgFotoAtualColorida, &imgSticker, Point(x - 350 - 100, y - 50 - 125));
        sticker = NENHUM;
    }
    else if (sticker == FOGOS) {
        cout << "Aplicando sticker fogos" << endl;
        resize(imgFogos, imgSticker, Size(150, 150));
        overlayImage(&imgFotoAtual, &imgSticker, Point(x - 350 - 75, y - 50 - 75));
        overlayImage(&imgFotoAtualColorida, &imgSticker, Point(x - 350 - 75, y - 50 - 75));
        sticker = NENHUM;
    }
    else if (sticker == FLOWER) {
        cout << "Aplicando sticker flower" << endl;
        resize(imgFlower, imgSticker, Size(250, 200));
        overlayImage(&imgFotoAtual, &imgSticker, Point(x - 350 - 125, y - 50 - 100));
        overlayImage(&imgFotoAtualColorida, &imgSticker, Point(x - 350 - 125, y - 50 - 100));
        sticker = NENHUM;
    }
    else if (sticker == COOL) {
        cout << "Aplicando sticker cool" << endl;
        resize(imgCool, imgSticker, Size(120, 120));
        overlayImage(&imgFotoAtual, &imgSticker, Point(x - 350 - 60, y - 50 - 60));
        overlayImage(&imgFotoAtualColorida, &imgSticker, Point(x - 350 - 60, y - 50 - 60));
        sticker = NENHUM;
    }
}

//---------------------------------------------------------------------------
//CONFIGURAÇÃO DO MENU DE SELEÇÃO DE FILTROS E STICKERS
void setupMenu() {
    Mat imgBotao1, imgBotao2, imgBotao3;
    //SETUP DOS BOTÕES DE MENU
    imgBotao1 = imread("textures/botao1.jpg");
    resize(imgBotao1, imgBotao1, Size(90, 45));
    imgBotao2 = imread("textures/botao2.jpg");
    resize(imgBotao2, imgBotao2, Size(45, 45));
    imgBotao3 = imread("textures/botao3.jpg");
    resize(imgBotao3, imgBotao3, Size(90, 45));
    //FORMATA EM RGBA E DESENHA OS BOTÃOS SOBRE O FUNDO    
    cvtColor(imgBotao1, imgBotao1, COLOR_BGR2BGRA);
    overlayImage(&imgFundo, &imgBotao1, Point(505.0, 2.5));
    cvtColor(imgBotao2, imgBotao2, COLOR_BGR2BGRA);
    overlayImage(&imgFundo, &imgBotao2, Point(752.5, 277.5));
    cvtColor(imgBotao3, imgBotao3, COLOR_BGR2BGRA);
    overlayImage(&imgFundo, &imgBotao3, Point(505.0, 552.5));

    //SETUP DAS MINIATURAS DE FILTROS
    filtroCinza(imgFotoOriginal, imgFotoMiniatura);
    resize(imgFotoMiniatura, imgFotoMiniatura, Size(85, 100));
    cvtColor(imgFotoMiniatura, imgFotoMiniatura, COLOR_BGR2BGRA);
    overlayImage(&imgFundo, &imgFotoMiniatura, Point(125.0, 50.0));
    //
    filtroNegativo(imgFotoOriginal, imgFotoMiniatura);
    resize(imgFotoMiniatura, imgFotoMiniatura, Size(85, 100));
    cvtColor(imgFotoMiniatura, imgFotoMiniatura, COLOR_BGR2BGRA);
    overlayImage(&imgFundo, &imgFotoMiniatura, Point(125.0, 150.0));
    //
    filtroCanny(imgFotoOriginal, imgFotoMiniatura);
    resize(imgFotoMiniatura, imgFotoMiniatura, Size(85, 100));
    cvtColor(imgFotoMiniatura, imgFotoMiniatura, COLOR_BGR2BGRA);
    overlayImage(&imgFundo, &imgFotoMiniatura, Point(125.0, 250.0));
    //
    filtroSobel(imgFotoOriginal, imgFotoMiniatura);
    resize(imgFotoMiniatura, imgFotoMiniatura, Size(85, 100));
    cvtColor(imgFotoMiniatura, imgFotoMiniatura, COLOR_BGR2BGRA);
    overlayImage(&imgFundo, &imgFotoMiniatura, Point(125.0, 350.0));
    //
    filtroLaplace(imgFotoOriginal, imgFotoMiniatura);
    resize(imgFotoMiniatura, imgFotoMiniatura, Size(85, 100));
    cvtColor(imgFotoMiniatura, imgFotoMiniatura, COLOR_BGR2BGRA);
    overlayImage(&imgFundo, &imgFotoMiniatura, Point(125.0, 450.0));
    //
    filtroBlur(imgFotoOriginal, imgFotoMiniatura);
    resize(imgFotoMiniatura, imgFotoMiniatura, Size(85, 100));
    cvtColor(imgFotoMiniatura, imgFotoMiniatura, COLOR_BGR2BGRA);
    overlayImage(&imgFundo, &imgFotoMiniatura, Point(215.0, 50.0));
    //
    filtroQuente(imgFotoOriginal, imgFotoMiniatura);
    resize(imgFotoMiniatura, imgFotoMiniatura, Size(85, 100));
    cvtColor(imgFotoMiniatura, imgFotoMiniatura, COLOR_BGR2BGRA);
    overlayImage(&imgFundo, &imgFotoMiniatura, Point(215.0, 150.0));
    //
    filtroFrio(imgFotoOriginal, imgFotoMiniatura);
    resize(imgFotoMiniatura, imgFotoMiniatura, Size(85, 100));
    cvtColor(imgFotoMiniatura, imgFotoMiniatura, COLOR_BGR2BGRA);
    overlayImage(&imgFundo, &imgFotoMiniatura, Point(215.0, 250.0));
    //
    filtroMoon(imgFotoOriginal, imgFotoMiniatura);
    resize(imgFotoMiniatura, imgFotoMiniatura, Size(85, 100));
    cvtColor(imgFotoMiniatura, imgFotoMiniatura, COLOR_BGR2BGRA);
    overlayImage(&imgFundo, &imgFotoMiniatura, Point(215.0, 350.0));
    //
    filtroBinario(imgFotoOriginal, imgFotoMiniatura);
    resize(imgFotoMiniatura, imgFotoMiniatura, Size(85, 100));
    cvtColor(imgFotoMiniatura, imgFotoMiniatura, COLOR_BGR2BGRA);
    overlayImage(&imgFundo, &imgFotoMiniatura, Point(215.0, 450.0));

    //REDIMENSIONA AS MINIATURAS DOS STICKERS E DESENHA SOBRE O FUNDO    
    resize(imgOlhos, imgSt1, Size(65, 40));
    resize(imgGotas, imgSt2, Size(65, 80));
    resize(imgFogos, imgSt3, Size(65, 80));
    resize(imgFlower, imgSt4, Size(65, 80));
    resize(imgCool, imgSt5, Size(65, 80));
    overlayImage(&imgFundo, &imgSt1, Point(50.0, 60.0));
    overlayImage(&imgFundo, &imgSt2, Point(50.0, 155.0));
    overlayImage(&imgFundo, &imgSt3, Point(50.0, 260.0));
    overlayImage(&imgFundo, &imgSt4, Point(50.0, 365.0));
    overlayImage(&imgFundo, &imgSt5, Point(50.0, 470.0));
}

//---------------------------------------------------------------------------
//ABRE A CAPTURA DE VIDEO
void alteraCamera(){
    Mat image;
    VideoCapture capture;
    capture.open(0);
    if (capture.isOpened())
    {
        cout << "Capture is opened" << endl;
        for (;;)
        {
            capture >> image;
            if (image.empty())
                break;
            //imshow("Sample", image);
            //imgFotoAtual = image;
            resize(image, image, Size(400, 500));
            cvtColor(image, image, COLOR_BGR2BGRA);
            imgFotoAtual = image.clone();
            imgFotoAtualColorida = image.clone();
            overlayImage(&imgFundo, &imgFotoAtual, Point(350.0, 50.0));
            imshow("MainWindow", imgFundo);
            if (!ligaCamera)
                break;

            if (waitKey(10) >= 0)
                break;            
        }
    }
    else
    {
        cout << "No capture" << endl;
        image = Mat::zeros(480, 640, CV_8UC1);
        imshow("Sample", image);
        waitKey(0);
    }
}

//---------------------------------------------------------------------------
//FUNÇÃO PARA SALVAR IMAGEM ALTERADA EM ARQUIVO.JPG
void salvaImagem() {

    String nomeImg = "";
    cout << "Digite o nome da imagem: ";
    getline(cin, nomeImg);

    String novaImagem = "../imgSalva/" + nomeImg + ".jpg";

    while (true) {
        bool isSalvo = imwrite(novaImagem, imgFotoAtual);
        if (isSalvo == false) {
            cout << "erro ao salvar imagem" << endl;
            cin.get();
        }
        cout << "Imagem salva com sucesso" << endl;
        break;
    }
}

//---------------------------------------------------------------------------
//MÉTODO DE CAPTURA DO MOUSE
static void mouseCallback(int event, int x, int y, int flags, void* userdata) {

    if (event == EVENT_LBUTTONDOWN) {
        cout << "Left click has made, Position(" << x << "," << y << ")" << endl;
        mouseEvent = LEFTBUTTON_DOWN;
        mousex = x;
        mousey = y;
        cout << "lalalalalaal" << endl;

        //analisar onde foi clicado, e selecionar ação a ser tomada.
        //AÇÕES FILTROS
        if (x > 145.0 && x < 145.0 + 65.0 && y > 50.0 && y < 50.0 + 100.0) { //filtro 1
            filtroCinza(imgFotoAtual, imgFotoAtual);
        }
        else if (x > 125.0 && x < 125.0 + 85.0 && y > 150.0 && y < 150.0 + 100.0) { //filtro 2
            filtroNegativo(imgFotoAtual, imgFotoAtual);
        }
        else if (x > 125.0 && x < 125.0 + 85.0 && y > 250.0 && y < 250.0 + 100.0) { //filtro 3
            filtroCanny(imgFotoAtual, imgFotoAtual);
        }
        else if (x > 125.0 && x < 125.0 + 85.0 && y > 350.0 && y < 350.0 + 100.0) { //filtro 4
            filtroSobel(imgFotoAtual, imgFotoAtual);
        }
        else if (x > 125.0 && x < 125.0 + 85.0 && y > 450.0 && y < 450.0 + 100.0) { //filtro 5            
            filtroLaplace(imgFotoAtual, imgFotoAtual);
        }
        if (x > 215.0 && x < 215.0 + 85.0 && y > 50.0 && y < 50.0 + 100.0) { //filtro 6
            filtroBlur(imgFotoAtual, imgFotoAtual);
        }
        else if (x > 215.0 && x < 215.0 + 85.0 && y > 150.0 && y < 150.0 + 100.0) { //filtro 7
            filtroQuente(imgFotoAtual, imgFotoAtual);
        }
        else if (x > 215.0 && x < 215.0 + 85.0 && y > 250.0 && y < 250.0 + 100.0) { //filtro 8
            filtroFrio(imgFotoAtual, imgFotoAtual);
        }
        else if (x > 215.0 && x < 215.0 + 85.0 && y > 350.0 && y < 350.0 + 100.0) { //filtro 9
            filtroMoon(imgFotoAtual, imgFotoAtual);
        }
        else if (x > 215.0 && x < 215.0 + 85.0 && y > 450.0 && y < 450.0 + 100.0) { //filtro 10
            filtroBinario(imgFotoAtual, imgFotoAtual);
        }

        //AÇÕES BOTÕES
        else if (x > 505.0 && x < 505.0 + 90.0 && y > 2.5 && y < 2.5 + 45.0) { //mudar cam/foto
            if (!ligaCamera) {
                ligaCamera = true;
                alteraCamera();
            }
            else {
                ligaCamera = false;
            }

        }
        else if (x > 505.0 && x < 505.0 + 90.0 && y > 552.5 && y < 552.5 + 45.0) { //salvar imagem
            salvaImagem();
        }
        else if (x > 752.5 && x < 752.5 + 45.0 && y>277.5 && y < 277.5 + 45.0) { //reverter tudo
            imgFotoAtual = imgFotoOriginal.clone();
            imgFotoAtualColorida = imgFotoOriginal.clone();
        }

        //AÇÕES STICKERS
        else if (x > 50.0 && x < 50.0 + 65 && y > 50.0 && y < 50.0 + 80.0) { //olhos
           sticker = OLHOS;
        }
        else if (x > 50.0 && x < 50.0 + 65 && y > 155.0 && y < 155.0 + 80.0) { //gotas
           sticker = GOTAS;
        }
        else if (x > 50.0 && x < 50.0 + 65 && y > 260.0 && y < 260.0 + 80.0) { //fogos
           sticker = FOGOS;
        }
        else if (x > 50.0 && x < 50.0 + 65 && y > 365.0 && y < 365.0 + 80.0) { //flower
           sticker = FLOWER;
        }
        else if (x > 50.0 && x < 50.0 + 65 && y > 470.0 && y < 470.0 + 80.0) { //cool
           sticker = COOL;
        }
        //area da foto
        else if (x > 350.0 && x < 350.0 + 400.0 && y > 50.0 && y < 50.0 + 500.0) {
          aplicaSticker(mousex, mousey);
        }

        cvtColor(imgFotoAtual, imgFotoAtual, COLOR_BGR2BGRA);         
        overlayImage(&imgFundo, &imgFotoAtual, Point(350.0, 50.0));
        imshow("MainWindow", imgFundo);
    }
}

//---------------------------------------------------------------------------
//MÉTODO MAIN
int main()
{
    cout << "Built with OpenCV " << CV_VERSION << endl;

    //CARREGAMENTO E DIMENSIONAMENTO DO FUNDO PRINCIPAL E DA FOTO PARA EDIÇÃO.
    imgFundo = imread("textures/fundo.jpg");
    imgFotoOriginal = imread("textures/foto.jpg");
    resize(imgFundo, imgFundo, Size(800, 600));
    resize(imgFotoOriginal, imgFotoOriginal, Size(400, 500));
    imgFotoAtual = imgFotoOriginal.clone();
    imgFotoAtualColorida = imgFotoOriginal.clone();

    //-------------------------------------------------------------
    //CARREGAMENTO DOS STICKERS
    imgOlhos = imread("textures/olhos.png", IMREAD_UNCHANGED);
    imgGotas = imread("textures/gotas.png", IMREAD_UNCHANGED);
    imgFogos = imread("textures/fogos.png", IMREAD_UNCHANGED);
    imgFlower = imread("textures/flower.png", IMREAD_UNCHANGED);
    imgCool = imread("textures/cool.png", IMREAD_UNCHANGED);

    //--------------------------------------------------------------
    //CARREGA MENU DE MINIATURAS PARA SELEÇÃO
    setupMenu();

    //--------------------------------------------------------------
    //CONVERSÃO DAS IMAGENS DE FUNDO E PRINCIPAL PARA RGBA E DESENHA A FOTO SOBRE O FUNDO
    cvtColor(imgFundo, imgFundo, COLOR_BGR2BGRA);
    cvtColor(imgFotoOriginal, imgFotoOriginal, COLOR_BGR2BGRA);
    overlayImage(&imgFundo, &imgFotoOriginal, Point(350.0, 50.0));

    //--------------------------------------------------------------
    //VERIFICA MOUSE E DESENHA IMAGEM
    namedWindow("MainWindow", WINDOW_AUTOSIZE);
    imshow("MainWindow", imgFundo);
    setMouseCallback("MainWindow", mouseCallback, NULL);
   

    for (;;) {
        char c = (char)waitKey();
        if (c == 27)
            break;
    }
    return 0;
}