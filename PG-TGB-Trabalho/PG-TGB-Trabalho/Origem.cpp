/*  TRABALHO DE PROCESSAMENTO GRAFICO - GRAU B
*   RAFAELA WERLE
* 
*   PROCESSAMENTO DE IMAGENS
*
*/ 

//IMPORTAÇÃO DAS BIBLIOTECAS
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#include <iostream>

//DECLARAÇAO DE VARIÁVEIS
using namespace cv;
using namespace std;
void drawText(Mat& image);
enum mouseEvents {NONE, LEFTBUTTON_DOWN};
int mouseEvent = NONE;
int mousex, mousey;

enum filtros {NENHUM, FILTRO1, FILTRO2, FILTRO3, FILTRO4, FILTRO5, FILTRO6, FILTRO7, FILTRO8, FILTRO9, FILTRO10};
int filtro = NENHUM;


Mat imgFotoOriginal, imgFotoAtual, imgFotoBackup;

Mat imgAlpha, imgFoto, imgNariz;
Mat imgOriginal, imgFundo, imgFiltro;
Mat imgCinza, imgNegativa, imgBlur, imgCanny, imgBlurCanny, imgDilate, imgErode;
Mat imgCrop, imgReSize, imgScale, imgEqual;
Mat grad_x, grad_y;
Mat abs_grad_x, abs_grad_y, grad;

//Variáveis de imagens das miniaturas do MENU.
Mat imgSt1, imgSt2, imgSt3, imgSt4, imgSt5;
Mat imgFiltro1, imgFiltro2, imgFiltro3, imgFiltro4, imgFiltro5, imgFiltro6, imgFiltro7, imgFiltro8, imgFiltro9, imgFiltro10;

bool jaTem = false;

//METODO DE INICIALIZAÇÃO DO MENU



//METODO PARA APLICAÇAO DE STICKERS
void overlayImage(Mat* src, Mat* overlay, const Point& location)
{
    for (int y = max(location.y, 0); y < src->rows; ++y)
    {
        if (jaTem)
            break;

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

void carregaMenu() {

    //Carrega os botões cam/foto, salvar, limpar
    Mat imgBotao1 = imread("textures/botao1.jpg");
    resize(imgBotao1, imgBotao1, Size(90, 45));
    cvtColor(imgBotao1, imgBotao1, COLOR_BGR2BGRA);
    overlayImage(&imgAlpha, &imgBotao1, Point(505.0, 2.5));
    Mat imgBotao2 = imread("textures/botao2.jpg");
    resize(imgBotao2, imgBotao2, Size(45, 45));
    cvtColor(imgBotao2, imgBotao2, COLOR_BGR2BGRA);
    overlayImage(&imgAlpha, &imgBotao2, Point(752.5, 277.5));
    Mat imgBotao3 = imread("textures/botao3.jpg");
    resize(imgBotao3, imgBotao3, Size(90, 45));
    cvtColor(imgBotao3, imgBotao3, COLOR_BGR2BGRA);
    overlayImage(&imgAlpha, &imgBotao3, Point(505.0, 552.5));



    //transforma os filtros já salvos em miniaturas.
    resize(imgCinza, imgFiltro1, Size(65, 80));
    resize(imgBlur, imgFiltro2, Size(65, 80));
    resize(imgNegativa, imgFiltro3, Size(65, 80));
    resize(grad, imgFiltro4, Size(65, 80));
    resize(imgBlurCanny, imgFiltro5, Size(65, 80));
    resize(imgFoto, imgFiltro6, Size(65, 80));
    resize(imgFoto, imgFiltro7, Size(65, 80));
    resize(imgEqual, imgFiltro8, Size(65, 80));
    resize(imgFoto, imgFiltro9, Size(65, 80));
    resize(imgFoto, imgFiltro10, Size(65, 80));

    //converte as miniaturas para RGBA
    cvtColor(imgFiltro1, imgFiltro1, COLOR_BGR2BGRA);
    cvtColor(imgFiltro2, imgFiltro2, COLOR_BGR2BGRA);
    cvtColor(imgFiltro3, imgFiltro3, COLOR_BGR2BGRA);
    cvtColor(imgFiltro4, imgFiltro4, COLOR_BGR2BGRA);
    cvtColor(imgFiltro5, imgFiltro5, COLOR_BGR2BGRA);
    cvtColor(imgFiltro6, imgFiltro6, COLOR_BGR2BGRA);
    cvtColor(imgFiltro7, imgFiltro7, COLOR_BGR2BGRA);
    cvtColor(imgFiltro8, imgFiltro8, COLOR_BGR2BGRA);
    cvtColor(imgFiltro9, imgFiltro9, COLOR_BGR2BGRA);
    cvtColor(imgFiltro10, imgFiltro10, COLOR_BGR2BGRA);

    //desenha as miniaturas em suas determinadas posições sobre imagem de fundo.
    overlayImage(&imgAlpha, &imgFiltro1, Point(145.0, 50.0));
    overlayImage(&imgAlpha, &imgFiltro2, Point(145.0, 155.0));
    overlayImage(&imgAlpha, &imgFiltro3, Point(145.0, 260.0));
    overlayImage(&imgAlpha, &imgFiltro4, Point(145.0, 365.0));
    overlayImage(&imgAlpha, &imgFiltro5, Point(145.0, 470.0));
    overlayImage(&imgAlpha, &imgFiltro6, Point(235.0, 50.0));
    overlayImage(&imgAlpha, &imgFiltro7, Point(235.0, 155.0));
    overlayImage(&imgAlpha, &imgFiltro8, Point(235.0, 260.0));
    overlayImage(&imgAlpha, &imgFiltro9, Point(235.0, 365.0));
    overlayImage(&imgAlpha, &imgFiltro10, Point(235.0, 470.0));
}

void aplicaFiltro() {

    cout << "filtro atual:" << filtro << endl;

    if (filtro == FILTRO1) {
        cvtColor(imgCinza, imgCinza, COLOR_BGR2BGRA); 
        overlayImage(&imgAlpha, &imgCinza, Point(350.0, 50.0));
    }
    else if (filtro == FILTRO2) {
        cvtColor(imgBlur, imgBlur, COLOR_BGR2BGRA);
        overlayImage(&imgAlpha, &imgBlur, Point(350.0, 50.0));
    }
    else if (filtro == FILTRO3) {
        cvtColor(imgNegativa, imgNegativa, COLOR_BGR2BGRA);
        overlayImage(&imgAlpha, &imgNegativa, Point(350.0, 50.0));
    }
    else if (filtro == FILTRO4) {
        cvtColor(grad, grad, COLOR_BGR2BGRA);
        overlayImage(&imgAlpha, &grad, Point(350.0, 50.0));
    }
    else if (filtro == FILTRO5) {
        cvtColor(imgBlurCanny, imgBlurCanny, COLOR_BGR2BGRA);
        overlayImage(&imgAlpha, &imgBlurCanny, Point(350.0, 50.0));
    }
}

void reverterAlteracoes() {
    overlayImage(&imgAlpha, &imgFoto, Point(350.0, 50.0));
}

//MÉTODO DE CAPTURA DO MOUSE
static void mouseCallback(int event, int x, int y, int flags, void* userdata) {

    if (event == EVENT_LBUTTONDOWN) {
        cout << "Left click has made, Position(" << x << "," << y << ")" << endl;
        mouseEvent = LEFTBUTTON_DOWN;
        mousex = x;
        mousey = y;
        cout << "lalalalalaal" << endl;


        //analisar onde foi clicado, e selecionar ação a ser tomada.
        if (x>145 && x< 145 + 65 && y>50 && y<50+80){
            filtro = FILTRO1;
            aplicaFiltro();
        }
        else if (x > 145 && x < 145 + 65 && y>155 && y < 155 + 80) {
            filtro = FILTRO2;
            aplicaFiltro();
        }
        else if (x > 145 && x < 145 + 65 && y>260 && y < 260 + 80) {
            filtro = FILTRO3;
            aplicaFiltro();
        }
        else if (x > 145 && x < 145 + 65 && y>365 && y < 365 + 80) {
            filtro = FILTRO4;
            aplicaFiltro();
        }
        else if (x > 145 && x < 145 + 65 && y>470 && y < 470 + 80) {
            filtro = FILTRO5;
            aplicaFiltro();
        }
        else if (x > 752.5 && x < 752.5 + 45 && y>277.5 && y < 277.5 + 45) {
            filtro = NENHUM;
            reverterAlteracoes();
        }

        overlayImage(&imgAlpha, &imgNariz, Point(mousex - imgNariz.cols / 2, mousey - imgNariz.rows / 2));
        imshow("MainWindow", imgAlpha);
    }
    else if (event == EVENT_RBUTTONDOWN) {
        cout << "Right click has made, Position(" << x << "," << y << ")" << endl;
    }
    else if (event == EVENT_MBUTTONDOWN) {
        cout << "Middle click has made, Position(" << x << "," << y << ")" << endl;
    }
    else if (event == EVENT_MOUSEMOVE ) {
        cout << "Current Mouse position(" << x << "," << y << ")" << endl;
    }
}

//MÉTODO MAIN
int main()
{
    cout << "Built with OpenCV " << CV_VERSION << endl;
        
    imgOriginal = imread("textures/fundo.jpg");
    imgFoto = imread("textures/foto.jpg");

    resize(imgOriginal, imgAlpha, Size(800, 600));
    resize(imgFoto, imgFoto, Size(400, 500));
    imshow("original", imgFoto);
    //APLICAÇÃO DE FILTROS
    

    //CINZA
    cvtColor(imgFoto, imgCinza, COLOR_BGR2GRAY); 
    imshow("cinza", imgCinza);

    //NEGATIVA
    cvtColor(imgFoto, imgNegativa, COLOR_BGR2GRAY);
    for (int i = 0; i < imgNegativa.rows; i++) {
        for (int j = 0; j < imgNegativa.cols; j++) {
            imgNegativa.at<uchar>(i, j) = 255 - imgNegativa.at<uchar>(i, j);
        }
    }

    //BLUR
    GaussianBlur(imgFoto, imgBlur, Size(5, 5), 5, 5);
    imshow("blur", imgBlur);

    //CANNY
    Canny(imgFoto, imgCanny, 25, 75);
    Canny(imgBlur, imgBlurCanny, 25, 75);
    imshow("canny", imgCanny);
    imshow("blurcanny", imgBlurCanny);

    //SOBEL
    Sobel(imgFoto, grad_x, CV_16S, 1, 0, 1, 1, 0, BORDER_DEFAULT);
    Sobel(imgFoto, grad_y, CV_16S, 0, 1, 1, 1, 0, BORDER_DEFAULT);    
    //Convertendo para o CV_8U 8bits
    convertScaleAbs(grad_x, abs_grad_x);
    convertScaleAbs(grad_y, abs_grad_y);    
    addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);
    imshow("sobel", grad);

    //DILATE, ERODE
    Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5));
    dilate(imgBlur, imgDilate, kernel);
    erode(imgDilate, imgErode, kernel);
    imshow("dilate", imgDilate);
    imshow("erode", imgErode);

    

    //EQUAL
    equalizeHist(imgCinza, imgEqual);
   
    

    imshow("negativo", imgNegativa);

    //CARREGAMENTO STICKER
    imgNariz = imread("textures/nariz.png", IMREAD_UNCHANGED);
    resize(imgNariz, imgNariz, Size(),0.2, 0.2);

    //CARREGA MENU DE MINIATURAS PARA SELEÇÃO
    carregaMenu();

    //CONVERSÃO DAS IMAGENS DE FUNDO E PRINCIPAL PARA RGBA E DESENHA A FOTO SOBRE O FUNDO
    cvtColor(imgAlpha, imgAlpha, COLOR_BGR2BGRA);
    cvtColor(imgFoto, imgFoto, COLOR_BGR2BGRA);
    overlayImage(&imgAlpha, &imgFoto, Point(350.0, 50.0));

    //VERIFICA MOUSE E DESENHA IMAGEM.
    namedWindow("MainWindow", WINDOW_AUTOSIZE);
    imshow("MainWindow", imgAlpha);
    setMouseCallback("MainWindow", mouseCallback, NULL);



    for (;;) {

        char c = (char)waitKey();

        if (c == 27)
            break;

    }

    return 0;

    /*cvtColor(imgOriginal, imgCinza, COLOR_BGR2GRAY);
    resize(imgCinza, imgCinza, Size(100, 150));
    imgCinza.copyTo(imgFundo.rowRange(10, 10 + 150).colRange(10, 10+100));*/


    //imshow("foto fundo", imgFundo);
    //imshow("foto foto", imgOriginal);
    //imshow("foto cinza", imgCinza);
        
    //waitKey(0);
      
    //cvtColor(imgScale, imgCinza, COLOR_BGR2GRAY);
    //imgCinza.copyTo(imgFundo.rowRange(400, 400).colRange(20, 400));

    //imshow("Foto original", imgScale);
    //imshow("foto cinza", imgCinza);
    //imshow("foto embaçada", imgBlur);
    //imshow("foto borda", imgCanny);
    //imshow("foto borda 2", imgBlurCanny);
    //imshow("foto equalizada", imgEqual);
    //imshow("foto SOBEL", grad);
    //imshow("foto dilatada", imgDilate);
    //imshow("foto erudida", imgErode);
    //imshow("foto original", imgOriginal);
    
    ////desenhar na imagem
    //Mat img = imread("foto.png");
    //drawText(img);

    //// Leitura da imagem original
    //Mat src = imread("pikachu.jpg");
    //Mat dois = imread("pikachu.jpg");
    //Mat uniao;

    ///*União de duas imagens na mesma janela.*/
    //cv::hconcat(src, dois, uniao);

    ////Verifica se não tem erros ao ler a imagem
    //if (!src.data)
    //{
    //    cout << "Erro ao carregar imagem" << endl;
    //    return -1;
    //}

    //// Criando mais de uma janela    
    //namedWindow("Minha Window", 1);
    //namedWindow("Minha Window 2", 2);

    ////Criando uma track bar para mudar o brilho brightness
    //int iSliderValue1 = 50;
    //createTrackbar("Brilho", "Minha Window", &iSliderValue1, 100);

    ////Criando track bar parar mudar o contraste
    //int iSliderValue2 = 50;
    //createTrackbar("Contraste", "Minha Window", &iSliderValue2, 100);

    //while (true)
    //{
    //    //Change the brightness and contrast of the image (For more infomation http://opencv-srf.blogspot.com/2013/07/change-contrast-of-image-or-video.html)

    //    Mat dst;
    //    int iBrightness = iSliderValue1 - 50;
    //    double dContrast = iSliderValue2 / 50.0;
    //    src.convertTo(dst, -1, dContrast, iBrightness);

    //    //show the brightness and contrast adjusted image
    //    imshow("Minha Window", dst);
    //    imshow("Minha Window 2", uniao);



    //    // Wait until user press some key for 50ms
    //    int iKey = waitKey(50);

    //    //if user press 'ESC' key
    //    if (iKey == 27)
    //    {
    //        break;
    //    }
    //}


    
       


    /*Mat image;
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
            drawText(image);
            imshow("Sample", image);
            if (waitKey(10) >= 0)
                break;
        }
    }
    else
    {
        cout << "No capture" << endl;
        image = Mat::zeros(480, 640, CV_8UC1);
        drawText(image);
        imshow("Sample", image);
        waitKey(0);
    }*/
}




void drawText(Mat& image)
{
    putText(image, "Hello OpenCV",
        Point(20, 50),
        FONT_HERSHEY_COMPLEX, 1, // font face and scale
        Scalar(255, 255, 255), // white
        1, LINE_AA); // line thickness and type
}
