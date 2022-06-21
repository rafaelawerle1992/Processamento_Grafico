/*  TRABALHO DE PROCESSAMENTO GRAFICO - GRAU B
*   RAFAELA WERLE
* 
*   PROCESSAMENTO DE IMAGENS COM OPENCV
*
*/ 

//IMPORTAÇÃO DAS BIBLIOTECAS
#include "opencv2/core/utility.hpp"
#include "opencv2/video.hpp"
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
enum filtros {  F0, F1, F2, F3, F4, F5, F6, 
                F7, F8, F9, F10, F11, F12,
                F13, F14, F15, F16, F17, F18}; //18 FILTROS POSSÍVEIS.
int filtro = F0;

bool cam = false;


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
// MENOS BRILHO
void filtroEscuro(Mat& imgOrigem, Mat& imgSaida) {
    convertScaleAbs(imgOrigem, imgSaida, 0.5, 0.0);
}
//GRAYSCALE
void filtroCinza(Mat& imgOrigem, Mat& imgSaida) {
    cvtColor(imgOrigem, imgSaida, COLOR_BGR2GRAY);
}
//NEGATIVE
void filtroNegativo(Mat& imgOrigem, Mat& imgSaida) {
    cvtColor(imgOrigem, imgSaida, COLOR_BGR2GRAY);
    for (int i = 0; i < imgSaida.rows; i++) {
        for (int j = 0; j < imgSaida.cols; j++) {
            imgSaida.at<uchar>(i, j) = 255 - imgSaida.at<uchar>(i, j);
        }
    }
}
//CANNY
void filtroCanny(Mat& imgOrigem, Mat& imgSaida) {
      cvtColor(imgOrigem, imgSaida, COLOR_BGR2GRAY);
    Canny(imgSaida, imgSaida, 50, 150);
}
//SOBEL
void filtroSobel(Mat& imgOrigem, Mat& imgSaida) {
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
// MAIS BRILHO
void filtroClaro(Mat& imgOrigem, Mat& imgSaida) {
    convertScaleAbs(imgOrigem, imgSaida, 1.0, 50.0);
}
//BLUR
void filtroBlur(Mat& imgOrigem, Mat& imgSaida) {
    GaussianBlur(imgOrigem, imgSaida, Size(5, 5), 5, 5);
}
//QUENTE
void filtroQuente(Mat& imgOrigem, Mat& imgSaida) {
    
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
//ERODE
void filtroErode(Mat& imgOrigem, Mat& imgSaida) {
    Mat imgDilate;
    Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5));
    dilate(imgOrigem, imgDilate, kernel);
    erode(imgDilate, imgSaida, kernel);
}
//ENHANCE
void filtroEnhance(Mat& imgOrigem, Mat& imgSaida){
    imgOrigem = imgFotoAtualColorida.clone();
    detailEnhance(imgOrigem, imgSaida);
}
//PENCIL
void filtroPencilGray(Mat& imgOrigem, Mat& imgSaida){
    Mat img1;
    imgOrigem = imgFotoAtualColorida.clone();
    pencilSketch(imgOrigem, img1, imgSaida, 10, 0.1f, 0.03f);
}
//PENCILGRAY
void filtroPencil(Mat& imgOrigem, Mat& imgSaida){
    Mat img;
    imgOrigem = imgFotoAtualColorida.clone();
    pencilSketch(imgOrigem, imgSaida, img, 10, 0.1f, 0.03f);
}
//STYLIZATION
void filtroStylization(Mat& imgOrigem, Mat& imgSaida){
    imgOrigem = imgFotoAtualColorida.clone();
    stylization(imgOrigem, imgSaida);
}

//---------------------------------------------------------------------------
//FUNÇÃO PARA APLICAÇÃO DE STICKERS NA FOTO
void aplicaSticker(double x, double y) {

    if (sticker == NENHUM) {
        cout << "Nenhum sticker selecionado" << endl;
    }
    else if (sticker == OLHOS) {
        cout << "Aplicando sticker olhos" << endl;
        resize(imgOlhos, imgSticker, Size(200, 75));
        overlayImage(&imgFotoAtual, &imgSticker, Point(x - 350 - 100, y - 50 - 35));//posição - desloc da foto no fundo - desloc do sticker
        overlayImage(&imgFotoAtualColorida, &imgSticker, Point(x - 350 - 100, y - 50 - 35));
    }
    else if (sticker == GOTAS) {
        cout << "Aplicando sticker gotas" << endl;
        resize(imgGotas, imgSticker, Size(300, 200));
        overlayImage(&imgFotoAtual, &imgSticker, Point(x - 350 - 150, y - 50 - 100));
        overlayImage(&imgFotoAtualColorida, &imgSticker, Point(x - 350 - 100, y - 50 - 125));
    }
    else if (sticker == FOGOS) {
        cout << "Aplicando sticker fogos" << endl;
        resize(imgFogos, imgSticker, Size(150, 150));
        overlayImage(&imgFotoAtual, &imgSticker, Point(x - 350 - 75, y - 50 - 75));
        overlayImage(&imgFotoAtualColorida, &imgSticker, Point(x - 350 - 75, y - 50 - 75));
    }
    else if (sticker == FLOWER) {
        cout << "Aplicando sticker flower" << endl;
        resize(imgFlower, imgSticker, Size(250, 200));
        overlayImage(&imgFotoAtual, &imgSticker, Point(x - 350 - 125, y - 50 - 100));
        overlayImage(&imgFotoAtualColorida, &imgSticker, Point(x - 350 - 125, y - 50 - 100));
    }
    else if (sticker == COOL) {
        cout << "Aplicando sticker cool" << endl;
        resize(imgCool, imgSticker, Size(120, 120));
        overlayImage(&imgFotoAtual, &imgSticker, Point(x - 350 - 60, y - 50 - 60));
        overlayImage(&imgFotoAtualColorida, &imgSticker, Point(x - 350 - 60, y - 50 - 60));
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
    resize(imgBotao2, imgBotao2, Size(50, 45));
    imgBotao3 = imread("textures/botao3.jpg");
    resize(imgBotao3, imgBotao3, Size(90, 45));
    //FORMATA EM RGBA E DESENHA OS BOTÃOS SOBRE O FUNDO    
    cvtColor(imgBotao1, imgBotao1, COLOR_BGR2BGRA);
    overlayImage(&imgFundo, &imgBotao1, Point(350.0, 2.5));
    cvtColor(imgBotao2, imgBotao2, COLOR_BGR2BGRA);
    overlayImage(&imgFundo, &imgBotao2, Point(525.0, 2.5));
    cvtColor(imgBotao3, imgBotao3, COLOR_BGR2BGRA);
    overlayImage(&imgFundo, &imgBotao3, Point(660.0, 2.5));

    //SETUP DAS MINIATURAS DE FILTROS
    filtroEscuro(imgFotoOriginal, imgFotoMiniatura);
    resize(imgFotoMiniatura, imgFotoMiniatura, Size(85, 100));
    cvtColor(imgFotoMiniatura, imgFotoMiniatura, COLOR_BGR2BGRA);
    overlayImage(&imgFundo, &imgFotoMiniatura, Point(50.0, 0.0));
    //
    filtroCinza(imgFotoOriginal, imgFotoMiniatura);
    resize(imgFotoMiniatura, imgFotoMiniatura, Size(85, 100));
    cvtColor(imgFotoMiniatura, imgFotoMiniatura, COLOR_BGR2BGRA);
    overlayImage(&imgFundo, &imgFotoMiniatura, Point(50.0, 100.0));
    //
    filtroNegativo(imgFotoOriginal, imgFotoMiniatura);
    resize(imgFotoMiniatura, imgFotoMiniatura, Size(85, 100));
    cvtColor(imgFotoMiniatura, imgFotoMiniatura, COLOR_BGR2BGRA);
    overlayImage(&imgFundo, &imgFotoMiniatura, Point(50.0, 200.0));
    //
    filtroCanny(imgFotoOriginal, imgFotoMiniatura);
    resize(imgFotoMiniatura, imgFotoMiniatura, Size(85, 100));
    cvtColor(imgFotoMiniatura, imgFotoMiniatura, COLOR_BGR2BGRA);
    overlayImage(&imgFundo, &imgFotoMiniatura, Point(50.0, 300.0));
    //
    filtroSobel(imgFotoOriginal, imgFotoMiniatura);
    resize(imgFotoMiniatura, imgFotoMiniatura, Size(85, 100));
    cvtColor(imgFotoMiniatura, imgFotoMiniatura, COLOR_BGR2BGRA);
    overlayImage(&imgFundo, &imgFotoMiniatura, Point(50.0, 400.0));
    //
    filtroLaplace(imgFotoOriginal, imgFotoMiniatura);
    resize(imgFotoMiniatura, imgFotoMiniatura, Size(85, 100));
    cvtColor(imgFotoMiniatura, imgFotoMiniatura, COLOR_BGR2BGRA);
    overlayImage(&imgFundo, &imgFotoMiniatura, Point(50.0, 500.0));
    //
    filtroClaro(imgFotoOriginal, imgFotoMiniatura);
    resize(imgFotoMiniatura, imgFotoMiniatura, Size(85, 100));
    cvtColor(imgFotoMiniatura, imgFotoMiniatura, COLOR_BGR2BGRA);
    overlayImage(&imgFundo, &imgFotoMiniatura, Point(140.0, 0.0));
    //
    filtroBlur(imgFotoOriginal, imgFotoMiniatura);
    resize(imgFotoMiniatura, imgFotoMiniatura, Size(85, 100));
    cvtColor(imgFotoMiniatura, imgFotoMiniatura, COLOR_BGR2BGRA);
    overlayImage(&imgFundo, &imgFotoMiniatura, Point(140.0, 100.0));
    //
    filtroQuente(imgFotoOriginal, imgFotoMiniatura);
    resize(imgFotoMiniatura, imgFotoMiniatura, Size(85, 100));
    cvtColor(imgFotoMiniatura, imgFotoMiniatura, COLOR_BGR2BGRA);
    overlayImage(&imgFundo, &imgFotoMiniatura, Point(140.0, 200.0));
    //
    filtroFrio(imgFotoOriginal, imgFotoMiniatura);
    resize(imgFotoMiniatura, imgFotoMiniatura, Size(85, 100));
    cvtColor(imgFotoMiniatura, imgFotoMiniatura, COLOR_BGR2BGRA);
    overlayImage(&imgFundo, &imgFotoMiniatura, Point(140.0, 300.0));
    //
    filtroMoon(imgFotoOriginal, imgFotoMiniatura);
    resize(imgFotoMiniatura, imgFotoMiniatura, Size(85, 100));
    cvtColor(imgFotoMiniatura, imgFotoMiniatura, COLOR_BGR2BGRA);
    overlayImage(&imgFundo, &imgFotoMiniatura, Point(140.0, 400.0));
    //
    filtroBinario(imgFotoOriginal, imgFotoMiniatura);
    resize(imgFotoMiniatura, imgFotoMiniatura, Size(85, 100));
    cvtColor(imgFotoMiniatura, imgFotoMiniatura, COLOR_BGR2BGRA);
    overlayImage(&imgFundo, &imgFotoMiniatura, Point(140.0, 500.0));
    //
    filtroErode(imgFotoOriginal, imgFotoMiniatura);
    resize(imgFotoMiniatura, imgFotoMiniatura, Size(85, 100));
    cvtColor(imgFotoMiniatura, imgFotoMiniatura, COLOR_BGR2BGRA);
    overlayImage(&imgFundo, &imgFotoMiniatura, Point(230.0, 0.0));
    //
    filtroEnhance(imgFotoOriginal, imgFotoMiniatura);
    resize(imgFotoMiniatura, imgFotoMiniatura, Size(85, 100));
    cvtColor(imgFotoMiniatura, imgFotoMiniatura, COLOR_BGR2BGRA);
    overlayImage(&imgFundo, &imgFotoMiniatura, Point(230.0, 100.0));
    //
    filtroPencilGray(imgFotoOriginal, imgFotoMiniatura);
    resize(imgFotoMiniatura, imgFotoMiniatura, Size(85, 100));
    cvtColor(imgFotoMiniatura, imgFotoMiniatura, COLOR_BGR2BGRA);
    overlayImage(&imgFundo, &imgFotoMiniatura, Point(230.0, 200.0));
    //
    filtroPencil(imgFotoOriginal, imgFotoMiniatura);
    resize(imgFotoMiniatura, imgFotoMiniatura, Size(85, 100));
    cvtColor(imgFotoMiniatura, imgFotoMiniatura, COLOR_BGR2BGRA);
    overlayImage(&imgFundo, &imgFotoMiniatura, Point(230.0, 300.0));
    //
    filtroStylization(imgFotoOriginal, imgFotoMiniatura);
    resize(imgFotoMiniatura, imgFotoMiniatura, Size(85, 100));
    cvtColor(imgFotoMiniatura, imgFotoMiniatura, COLOR_BGR2BGRA);
    overlayImage(&imgFundo, &imgFotoMiniatura, Point(230.0, 400.0));
    //
    
    //REDIMENSIONA AS MINIATURAS DOS STICKERS E DESENHA SOBRE O FUNDO    
    resize(imgOlhos, imgSt1, Size(60, 35));
    resize(imgGotas, imgSt2, Size(70, 40));
    resize(imgFlower, imgSt3, Size(60, 40));
    resize(imgFogos, imgSt4, Size(60, 40));
    resize(imgCool, imgSt5, Size(50, 40));
    overlayImage(&imgFundo, &imgSt1, Point(355.0, 555));
    overlayImage(&imgFundo, &imgSt2, Point(435.0, 555));
    overlayImage(&imgFundo, &imgSt3, Point(515.0, 555));
    overlayImage(&imgFundo, &imgSt4, Point(595.0, 555));
    overlayImage(&imgFundo, &imgSt5, Point(675.0, 555));
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

            resize(image, image, Size(400, 500));           
            imgFotoAtualColorida = image.clone();
                       
            if (filtro == F1) { filtroEscuro(image, image); }
            if (filtro == F2) { filtroCinza(image, image); }
            if (filtro == F3) { filtroNegativo(image, image); }
            if (filtro == F4) { filtroCanny(image, image); }
            if (filtro == F5) { filtroSobel(image, image); }
            if (filtro == F6) { filtroLaplace(image, image); }
            if (filtro == F7) { filtroClaro(image, image); }
            if (filtro == F8) { filtroBlur(image, image); }
            if (filtro == F9) { filtroQuente(image, image); }
            if (filtro == F10) { filtroFrio(image, image); }
            if (filtro == F11) { filtroMoon(image, image); }
            if (filtro == F12) { filtroBinario(image, image); }
            if (filtro == F13) { filtroErode(image, image); }
            if (filtro == F14) { filtroEnhance(image, image); }
            if (filtro == F15) { filtroPencilGray(image, image); }
            if (filtro == F16) { filtroPencil(image, image); }
            if (filtro == F17) { filtroStylization(image, image); }
            if (filtro == F18) { filtroStylization(image, image); }      


            imgFotoAtual = image.clone();
            cvtColor(image, image, COLOR_BGR2BGRA);
            overlayImage(&imgFundo, &image, Point(350.0, 50.0));
            imshow("PG - Trabalho Pratico Grau B - Rafaela Cunha Werle", imgFundo);
                      
            
            if (!cam || waitKey(10) >= 0) {                
                break;
            }            
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
//RESET
void reset() {
    imgFotoAtual = imgFotoOriginal.clone();
    imgFotoAtualColorida = imgFotoOriginal.clone();
    filtro = F0;
    sticker = NENHUM;
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
        if (x > 50.0 && x < 50.0 + 85.0 && y > 0.0 && y < 0.0 + 100.0) { //filtro 1
            filtro = F1;
            if (!cam)
                filtroEscuro(imgFotoAtual, imgFotoAtual);
        }
        else if (x > 50.0 && x < 50.0 + 85.0 && y > 100.0 && y < 100.0 + 100.0) { //filtro 2
            filtro = F2;
            if (!cam)
                filtroCinza(imgFotoAtual, imgFotoAtual);
        }
        else if (x > 50.0 && x < 50.0 + 85.0 && y > 200.0 && y < 200.0 + 100.0) { //filtro 3
            filtro = F3;
            if (!cam)
                filtroNegativo(imgFotoAtual, imgFotoAtual);
        }
        else if (x > 50.0 && x < 50.0 + 85.0 && y > 300.0 && y < 300.0 + 100.0) { //filtro 4
            filtro = F4;
            if (!cam)
                filtroCanny(imgFotoAtual, imgFotoAtual);
        }
        else if (x > 50.0 && x < 50.0 + 85.0 && y > 400.0 && y < 400.0 + 100.0) { //filtro 5
            filtro = F5;
            if (!cam)
                filtroSobel(imgFotoAtual, imgFotoAtual);
        }
        else if (x > 50.0 && x < 50.0 + 85.0 && y > 500.0 && y < 500.0 + 100.0) { //filtro 6            
            filtro = F6;
            if (!cam)
                filtroLaplace(imgFotoAtual, imgFotoAtual);
        }
        else if (x > 140.0 && x < 140.0 + 85.0 && y > 0.0 && y < 0.0 + 100.0) { //filtro 7
            filtro = F7;
            if (!cam)
                filtroClaro(imgFotoAtual, imgFotoAtual);
        }
        else if (x > 140.0 && x < 140.0 + 85.0 && y > 100.0 && y < 100.0 + 100.0) { //filtro 8
            filtro = F8;
            if (!cam)
                filtroBlur(imgFotoAtual, imgFotoAtual);
        }
        else if (x > 140.0 && x < 140.0 + 85.0 && y > 200.0 && y < 200.0 + 100.0) { //filtro 9
            filtro = F9;
            if (!cam)
                filtroQuente(imgFotoAtual, imgFotoAtual);
        }
        else if (x > 140.0 && x < 140.0 + 85.0 && y > 300.0 && y < 300.0 + 100.0) { //filtro 10
            filtro = F10;
            if (!cam)
                filtroFrio(imgFotoAtual, imgFotoAtual);
        }
        else if (x > 140.0 && x < 140.0 + 85.0 && y > 400.0 && y < 400.0 + 100.0) { //filtro 11
            filtro = F11;
            if (!cam)
                filtroMoon(imgFotoAtual, imgFotoAtual);
        }
        else if (x > 140.0 && x < 140.0 + 85.0 && y > 500.0 && y < 500.0 + 100.0) { //filtro 12
            filtro = F12;
            if (!cam)
                filtroBinario(imgFotoAtual, imgFotoAtual);
        }
        else if (x > 230.0 && x < 230.0 + 85.0 && y > 0.0 && y < 0.0 + 100.0) { //filtro 13
            filtro = F13;
            if (!cam)
                filtroErode(imgFotoAtual, imgFotoAtual);
        }
        else if (x > 230.0 && x < 230.0 + 85.0 && y > 100.0 && y < 100.0 + 100.0) { //filtro 14
            filtro = F14;
            if (!cam)
                filtroEnhance(imgFotoAtual, imgFotoAtual);
        }
        else if (x > 230.0 && x < 230.0 + 85.0 && y > 200.0 && y < 200.0 + 100.0) { //filtro 15
            filtro = F15;
            if (!cam)
                filtroPencilGray(imgFotoAtual, imgFotoAtual);
        }
        else if (x > 230.0 && x < 230.0 + 85.0 && y > 300.0 && y < 300.0 + 100.0) { //filtro 16
            filtro = F16;
            if (!cam)
                filtroPencil(imgFotoAtual, imgFotoAtual);
        }
        else if (x > 230.0 && x < 230.0 + 85.0 && y > 400.0 && y < 400.0 + 100.0) { //filtro 17
            filtro = F17;
            if (!cam)
                filtroStylization(imgFotoAtual, imgFotoAtual);
        }
        

        //AÇÕES BOTÕES
        else if (x > 350.0 && x < 350.0 + 90.0 && y > 2.5 && y < 2.5 + 45.0) { //mudar cam/foto
            filtro = F0;
            if (!cam) {
                cam = true;
                alteraCamera();
            }
            else {
                cam = false;
            }
        }
        else if (x > 525.0 && x < 525.0 + 50.0 && y>2.5 && y < 2.5 + 45.0) { //reverter tudo
            if (!cam)
                reset();
        }
        else if (x > 660.0 && x < 660.0 + 90.0 && y > 2.5 && y <2.5 + 45.0) { //salvar imagem
            salvaImagem();
        }        

        //AÇÕES STICKERS
        else if (x > 355.0 && x < 355.0 + 65 && y > 555.0 && y < 555.0 + 40.0) { //olhos
           sticker = OLHOS;
        }
        else if (x > 435.0 && x < 435.0 + 65 && y > 555.0 && y < 555.0 + 40.0) { //gotas
           sticker = GOTAS;
        }
        else if (x > 515.0 && x < 515.0 + 65 && y > 555.0 && y < 555.0 + 40.0) { //fogos
           sticker = FLOWER;
        }
        else if (x > 595.0 && x < 595.0 + 65 && y > 555.0 && y < 555.0 + 40.0) { //flower
           sticker = FOGOS;
        }
        else if (x > 675.0 && x < 675.0 + 65 && y > 555.0 && y < 555.0 + 40.0) { //cool
           sticker = COOL;
        }
        //area da foto
        else if (x > 350.0 && x < 350.0 + 400.0 && y > 50.0 && y < 50.0 + 500.0) {
          aplicaSticker(mousex, mousey);
          sticker = NENHUM;
        }
        if (!cam) {
            cvtColor(imgFotoAtual, imgFotoAtual, COLOR_BGR2BGRA);
            overlayImage(&imgFundo, &imgFotoAtual, Point(350.0, 50.0));
            imshow("PG - Trabalho Pratico Grau B - Rafaela Cunha Werle", imgFundo);
        }
    }
}

//---------------------------------------------------------------------------
//MÉTODO MAIN
int main()
{
    String text = "";
    cout << "Digite o nome da imagem .jpg: ";
    getline(cin, text);

    String nomeImg = "textures/" + text + ".jpg";
    

    //CARREGAMENTO E DIMENSIONAMENTO DO FUNDO PRINCIPAL E DA FOTO PARA EDIÇÃO.
    imgFundo = imread("textures/fundo.jpg");
    imgFotoOriginal = imread(nomeImg);
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
    namedWindow("PG - Trabalho Pratico Grau B - Rafaela Cunha Werle", WINDOW_AUTOSIZE);
    imshow("PG - Trabalho Pratico Grau B - Rafaela Cunha Werle", imgFundo);
    setMouseCallback("PG - Trabalho Pratico Grau B - Rafaela Cunha Werle", mouseCallback, NULL);
   

    for (;;) {
        char c = (char)waitKey();
        if (c == 27)
            break;
    }
    return 0;
}
