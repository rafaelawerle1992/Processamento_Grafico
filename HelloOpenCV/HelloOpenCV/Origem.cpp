#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#include <iostream>

using namespace cv;
using namespace std;

void drawText(Mat& image);

enum mouseEvents {NONE, LEFTBUTTON_DOWN};

int mouseEvent = NONE;
int mousex, mousey;
Mat imgAlpha, imgNariz;

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



static void mouseCallback(int event, int x, int y, int flags, void* userdata) {

    if (event == EVENT_LBUTTONDOWN) {
        cout << "Left click has made, Position(" << x << "," << y << ")" << endl;
        mouseEvent = LEFTBUTTON_DOWN;
        mousex = x;
        mousey = y;
        cout << "lalalalalaal" << endl;
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

int main()
{
    cout << "Built with OpenCV " << CV_VERSION << endl;

    Mat imgOriginal, imgFundo;
    Mat imgCinza, imgBlur, imgCanny, imgBlurCanny, imgDilate, imgErode;
    Mat imgCrop, imgReSize, imgScale, imgEqual;



    imgOriginal = imread("pikachu.jpg");

    resize(imgOriginal, imgScale, Size(), 0.5, 0.5);
    cvtColor(imgScale, imgCinza, COLOR_BGR2GRAY);
    GaussianBlur(imgScale, imgBlur, Size(5, 5), 5, 0);

    Mat grad_x, grad_y;
    Sobel(imgCinza, grad_x, CV_16S, 1, 0, 1, 1, 0, BORDER_DEFAULT);
    Sobel(imgCinza, grad_y, CV_16S, 0, 1, 1, 1, 0, BORDER_DEFAULT);
    
    Mat abs_grad_x, abs_grad_y, grad;
    
    //Convertendo para o CV_8U 8bits
    convertScaleAbs(grad_x, abs_grad_x);
    convertScaleAbs(grad_y, abs_grad_y);
    
    addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);

    imshow("Sobel", grad);

    //Canny
    Canny(imgCinza, imgCanny, 25, 75);
    Canny(imgBlur, imgBlurCanny, 25, 75);

    //Equal
    equalizeHist(imgBlur, imgEqual);

    Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5));
   
    dilate(imgBlurCanny, imgDilate, kernel);
    erode(imgDilate, imgErode, kernel);

       
   


    Rect areaCrop(100, 100, 50, 50);

    imgCrop = imgOriginal(areaCrop);
    
    //imgCinza.copyTo(imgFundo.rowRange(400, 400).colRange(20, 400));

    imgNariz = imread("nariz.png", IMREAD_UNCHANGED);
    resize(imgNariz, imgNariz, Size(), 0.2, 0.2);

    cvtColor(imgScale, imgAlpha, COLOR_BGR2BGRA);

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
