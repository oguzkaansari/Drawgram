#include "OpenGL.h"
#include <cmath>

void OpenGL::SetGLProjection()
{

    if (GLRect.bottom == 0)
    { 
        GLRect.bottom = 1;
    }
    glViewport(0, 0, GLRect.right - GLRect.left, GLRect.bottom - GLRect.top);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
       
}

void OpenGL::SetGLView()
{
    SetGLProjection();   
    glMatrixMode(GL_MODELVIEW);    
    glLoadIdentity();    
}

void OpenGL::InitGL()
{   
    SetGLView();
    glClearColor(BgColor[0], BgColor[1], BgColor[2], 0.0); //ARKA PLAN RENGÝ
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glLineWidth(lineWidth); //KALINLIK
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);

   
}


bool OpenGL::CreateGLContexts(HWND hWnd, HDC& hDC, HGLRC& hRC)
{
    static PIXELFORMATDESCRIPTOR pfd;
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cRedBits = 0;
    pfd.cRedShift = 0;
    pfd.cGreenBits = 0;
    pfd.cGreenShift = 0;
    pfd.cBlueBits = 0;
    pfd.cBlueShift = 0;
    pfd.cAlphaBits = 0;
    pfd.cAlphaShift = 0;
    pfd.cAccumBits = 0;
    pfd.cAccumRedBits = 0;
    pfd.cAccumGreenBits = 0;
    pfd.cAccumBlueBits = 0;
    pfd.cAccumAlphaBits = 0;
    pfd.cDepthBits = 16;
    pfd.cStencilBits = 0;
    pfd.cAuxBuffers = 0;
    pfd.iLayerType = PFD_MAIN_PLANE;
    pfd.bReserved = 0;
    pfd.dwLayerMask = 0;
    pfd.dwVisibleMask = 0;
    pfd.dwDamageMask = 0;

    hDC = GetDC(hWnd);

    if (!ChoosePixelFormat(hDC, &pfd))
    {
        MessageBox(0, L"Can't Find A Suitable PixelFormat.", L"Error", MB_OK | MB_ICONERROR);
        return FALSE;
    }

    if (!SetPixelFormat(hDC, ChoosePixelFormat(hDC, &pfd), &pfd))
    {
        MessageBox(0, L"Can't Set The PixelFormat.", L"Error", MB_OK | MB_ICONERROR);
        return FALSE;
    }

    hRC = wglCreateContext(hDC);

    if (!hRC)
    {
        MessageBox(0, L"Can't Create A GL Rendering Context.", L"Error", MB_OK | MB_ICONERROR);
        return FALSE;
    }

    if (!wglMakeCurrent(hDC, hRC))
    {
        MessageBox(0, L"Can't activate GLRC.", L"Error", MB_OK | MB_ICONERROR);
        return FALSE;
    }

    return TRUE;
}

void OpenGL::ReleaseGLContexts(HWND hWnd, HDC hDC, HGLRC hRC)
{
    ChangeDisplaySettings(NULL, 0);
    wglMakeCurrent(hDC, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hWnd, hDC);
}

void OpenGL::DrawGLScene(void)
{

    glClear(GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    glColor3f(DrawColor[0], DrawColor[1], DrawColor[2]); // ÇÝZÝM RENGÝ

    
    if (DRAW_STATE == 1) //KALEM MODU
    {
        glBegin(GL_LINES);
        glVertex2f((GLfloat)pStartX, -(GLfloat)pStartY);
        glVertex2f((GLfloat)pEndX, -(GLfloat)pEndY);
        glEnd();
        pStartX = pEndX;
        pStartY = pEndY;
        penInnerPoints.push_back(pEndX);
        penInnerPoints.push_back(pEndY);
    }

    if (DRAW_STATE == 2) //ÇÝZGÝ MODU
    {          
        glClear(GL_COLOR_BUFFER_BIT);
        RedrawGLScene();
        glBegin(GL_LINES);
        glVertex2f((GLfloat)pStartX, -(GLfloat)pStartY); 
        glVertex2f((GLfloat)pEndX, -(GLfloat)pEndY);
        glEnd();
    }
    
    if (DRAW_STATE == 3) // ÜÇGEN MODU 
    {
        float centerX = (pEndX + pStartX) / 2;
        float centerY;
        glClear(GL_COLOR_BUFFER_BIT);
        RedrawGLScene();
        if (filled == 1)
        {
            glBegin(GL_POLYGON);
        }
        else
        {
            glBegin(GL_LINE_LOOP);
        }

        if (pEndX < pStartX)
        {
            centerY = pStartY + (pEndX - pStartX);           
        }
        else 
        {
            centerY = pStartY - (pEndX - pStartX);           
        }      
        glVertex2f((GLfloat)pEndX, -(GLfloat)pEndY);
        glVertex2f((GLfloat)pStartX, -(GLfloat)pStartY);
        glVertex2f((GLfloat)centerX, -(GLfloat)centerY);
       
        glEnd();

    } 

    if (DRAW_STATE == 4) // DÝKDÖRTGEN MODU 
    {
        glClear(GL_COLOR_BUFFER_BIT);
        RedrawGLScene();
        if (filled == 1)
        {
            glBegin(GL_POLYGON);
        }
        else
        {
            glBegin(GL_LINE_LOOP);
        }

        if ((pStartX > pEndX && pStartY < pEndY) || (pStartX < pEndX && pStartY > pEndY))
        {
            glVertex2f((GLfloat)pStartX, -(GLfloat)pStartY);
            glVertex2f((GLfloat)pEndX, -(GLfloat)pStartY);
            glVertex2f((GLfloat)pEndX, -(GLfloat)pEndY);
            glVertex2f((GLfloat)pStartX, -(GLfloat)pEndY);
        }
        else
        {
            glVertex2f((GLfloat)pStartX, -(GLfloat)pStartY);
            glVertex2f((GLfloat)pStartX, -(GLfloat)pEndY);
            glVertex2f((GLfloat)pEndX, -(GLfloat)pEndY);
            glVertex2f((GLfloat)pEndX, -(GLfloat)pStartY);
        }
        glEnd();
    }

    if (DRAW_STATE == 5) // BEÞGEN MODU
    {
        glClear(GL_COLOR_BUFFER_BIT);
        RedrawGLScene();
        
        float p1X = (float)sin(28) * (pEndX - pStartX) + pEndX;
        float p1Y = (float)cos(28) * (pEndX - pStartX) + pStartY;
        float p2X = ((pEndX - pStartX) / 2) + pStartX;
        float p2Y = p1Y + ((float)sin(36) * (pEndX - pStartX));
        float p3X = pStartX - ((float)sin(28) * (pEndX - pStartX));
        float p3Y = p1Y;

        if (filled == 1)
        {
            glBegin(GL_POLYGON);
        }
        else
        {
            glBegin(GL_LINE_LOOP);
        }

        glVertex2f((GLfloat)pEndX, -(GLfloat)pEndY);
        glVertex2f((GLfloat)p1X, -(GLfloat)p1Y);
        glVertex2f((GLfloat)p2X, -(GLfloat)p2Y);
        glVertex2f((GLfloat)p3X, -(GLfloat)p3Y);
        glVertex2f((GLfloat)pStartX, -(GLfloat)pStartY);

        glEnd();
    }
    
    if (DRAW_STATE == 6)
    {
        glClear(GL_COLOR_BUFFER_BIT);
        RedrawGLScene();
        if (filled == 1)
        {
            glBegin(GL_POLYGON);
        }
        else
        {
            glBegin(GL_LINE_LOOP);
        }

    }

    if (DRAW_STATE == 7) // ÇEMBER MODU 
    {
        glClear(GL_COLOR_BUFFER_BIT); 
        RedrawGLScene();
        //Merkez noktalarý
        float centerX = (pEndX + pStartX) / 2;
        float centerY = (pEndY + pStartY) / 2;
        // Baþlangýç ve merkez noktasý arasýndaki uzaklýk(Yarýçap)
        float radius = sqrt(pow(centerX - pStartX, 2) + pow(centerY - pStartY, 2));
        if (filled == 1)
        {
            glBegin(GL_POLYGON);
        }
        else
        {
            glBegin(GL_LINE_LOOP);
        }

        for (int i = 0; i < 100; i++) { //DÖNGÜNÜN ÝÇÝNDE YARIÇAP DEÐÝÞÝYOR!
            float theta = 2.0f * 3.1415926f * float(i) / float(100);//get the current angle 
            float x = radius * cosf(theta);//calculate the x component 
            float y = radius * sinf(theta);//calculate the y component 
            glVertex2f(GLfloat(x + centerX), -GLfloat(y * 2 + centerY));//output vertex 
        }                
        glEnd();        
    }

    if (DRAW_STATE == 8) // ELÝPS MODU 
    {
        glClear(GL_COLOR_BUFFER_BIT);
        RedrawGLScene();
        //Merkez noktalarý
        float centerX = (pEndX + pStartX) / 2;
        float centerY = (pEndY + pStartY) / 2;
        // Baþlangýç ve merkez noktasý arasýndaki uzaklýk(Yarýçap)
        float radius = sqrt(pow(centerX - pStartX, 2) + pow(centerY - pStartY, 2));

        if (filled == 1)
        {
            glBegin(GL_POLYGON);
        }
        else
        {
            glBegin(GL_LINE_LOOP);
        }

        for (int i = 0; i < 100; i++) { //DÖNGÜNÜN ÝÇÝNDE YARIÇAP DEÐÝÞÝYOR!
            float theta = 2.0f * 3.1415926f * float(i) / float(100);//get the current angle 
            float x = radius * cosf(theta);//calculate the x component 
            float y = radius * sinf(theta);//calculate the y component 
            glVertex2f(GLfloat(x + centerX), -GLfloat(y + centerY));//output vertex 
        }
        glEnd();
    }

    if (DRAW_STATE == 9) // SÝLGÝ MODU 
    {
        glColor3f(BgColor[0], BgColor[1], BgColor[2]);
        glBegin(GL_LINES);
        glVertex2f((GLfloat)pStartX, -(GLfloat)pStartY);
        glVertex2f((GLfloat)pEndX, -(GLfloat)pEndY);
        glEnd();
        pStartX = pEndX;
        pStartY = pEndY;
        penInnerPoints.push_back(pEndX);
        penInnerPoints.push_back(pEndY);
    }     

    glPopMatrix();
    return;
}

void OpenGL::RedrawGLScene(void) 
{
    if (objects.size() == 0)
    {
        return; 
    }
    glClear(GL_DEPTH_BUFFER_BIT);
    glPushMatrix();

    for (int i = objects.size() - 1; i >= 0; i-- ) 
    {
        
        Object object = objects[i];
        glColor3f(object.colorRGB[0], object.colorRGB[1], object.colorRGB[2]);
        glLineWidth(object.lineWidth);

        if (object.state == 1) //KALEM MODU 
        {
            for (int i = 0; i < object.points.size() - 3; i += 2) 
            {
                glBegin(GL_LINES);
                glVertex2f((GLfloat)object.points[i], -(GLfloat)object.points[i+1]);
                glVertex2f((GLfloat)object.points[i+2], -(GLfloat)object.points[i+3]);
                glEnd();

            }
        }
        if (object.state == 2) //ÇÝZGÝ MODU
        {
            glBegin(GL_LINES);
            glVertex2f((GLfloat)object.points[0], -(GLfloat)object.points[1]);
            glVertex2f((GLfloat)object.points[2], -(GLfloat)object.points[3]);
            glEnd();
        }

        if (object.state == 3) // ÜÇGEN MODU
        {
            float centerX = (object.points[2] + object.points[0]) / 2;
            float centerY;

            if (object.filled == 1)
            {
                glBegin(GL_POLYGON);
            }
            else
            {
                glBegin(GL_LINE_LOOP);
            }

            if (object.points[2] < object.points[0])
            {
                centerY = object.points[1] + (object.points[2] - object.points[0]);
            }
            else
            {
                centerY = object.points[1] - (object.points[2] - object.points[0]);
            }
            glVertex2f((GLfloat)object.points[2], -(GLfloat)object.points[3]);
            glVertex2f((GLfloat)object.points[0], -(GLfloat)object.points[1]);
            glVertex2f((GLfloat)centerX, -(GLfloat)centerY);

            glEnd();

        }

        if (object.state == 4) //DÝKDÖRTGEN MODU
        {
            if (object.filled == 1)
            {
                glBegin(GL_POLYGON);
            }
            else
            {
                glBegin(GL_LINE_LOOP);
            }

            if ((object.points[0] > object.points[2] && object.points[1] < object.points[3])
                || (object.points[0] < object.points[2] && object.points[1] > object.points[3]))
            {
                glVertex2f((GLfloat)object.points[0], -(GLfloat)object.points[1]);
                glVertex2f((GLfloat)object.points[2], -(GLfloat)object.points[1]);
                glVertex2f((GLfloat)object.points[2], -(GLfloat)object.points[3]);
                glVertex2f((GLfloat)object.points[0], -(GLfloat)object.points[3]);
            }
            else
            {
                glVertex2f((GLfloat)object.points[0], -(GLfloat)object.points[1]);
                glVertex2f((GLfloat)object.points[0], -(GLfloat)object.points[3]);
                glVertex2f((GLfloat)object.points[2], -(GLfloat)object.points[3]);
                glVertex2f((GLfloat)object.points[2], -(GLfloat)object.points[1]);
            }
            glEnd();

        }           

        if (object.state == 5) // BEÞGEN MODU
        {
            float p1X = (float)sin(28) * (object.points[2] - object.points[0]) + object.points[2];
            float p1Y = (float)cos(28) * (object.points[2] - object.points[0]) + object.points[1];
            float p2X = ((object.points[2] - object.points[0]) / 2) + object.points[0];
            float p2Y = p1Y + ((float)sin(36) * (object.points[2] - object.points[0]));
            float p3X = object.points[0] - ((float)sin(28) * (object.points[2] - object.points[0]));
            float p3Y = p1Y;

            if (object.filled == 1)
            {
                glBegin(GL_POLYGON);
            }
            else
            {
                glBegin(GL_LINE_LOOP);
            }

            glVertex2f((GLfloat)object.points[2], -(GLfloat)object.points[3]);
            glVertex2f((GLfloat)p1X, -(GLfloat)p1Y);
            glVertex2f((GLfloat)p2X, -(GLfloat)p2Y);
            glVertex2f((GLfloat)p3X, -(GLfloat)p3Y);
            glVertex2f((GLfloat)object.points[0], -(GLfloat)object.points[1]);

            glEnd();
        }
        if (object.state == 7) //ÇEMBER MODU
        {
            if (object.filled == 1)
            {
                glBegin(GL_POLYGON);
            }
            else
            {
                glBegin(GL_LINE_LOOP);
            }
            //Merkez noktalarý
            float centerX = (object.points[2] + object.points[0]) / 2;
            float centerY = (object.points[3] + object.points[1]) / 2;
            // Baþlangýç ve merkez noktasý arasýndaki uzaklýk(Yarýçap)
            float radius = sqrt(pow((centerX - object.points[0]), 2) + pow((centerY - object.points[1]), 2));
            for (int i = 0; i < 100; i++) {
                float theta = 2.0f * 3.1415926f * float(i) / float(100);//get the current angle 
                float x = radius * cosf(theta);//calculate the x component 
                float y = radius * sinf(theta);//calculate the y component 
                glVertex2f((GLfloat)(x + centerX), -(GLfloat)(y * 2 + centerY));//output vertex 
            }
            glEnd();

        }

        if (object.state == 8) // ELÝPS MODU
        {
            if (object.filled == 1)
            {
                glBegin(GL_POLYGON);
            }
            else
            {
                glBegin(GL_LINE_LOOP);
            }
            //Merkez noktalarý
            float centerX = (object.points[2] + object.points[0]) / 2;
            float centerY = (object.points[3] + object.points[1]) / 2;
            // Baþlangýç ve merkez noktasý arasýndaki uzaklýk(Yarýçap)
            float radius = sqrt(pow((centerX - object.points[0]), 2) + pow((centerY - object.points[1]), 2));
            for (int i = 0; i < 100; i++) {
                float theta = 2.0f * 3.1415926f * float(i) / float(100);//get the current angle 
                float x = radius * cosf(theta);//calculate the x component 
                float y = radius * sinf(theta);//calculate the y component 
                glVertex2f((GLfloat)(x + centerX), -(GLfloat)(y + centerY));//output vertex 
            }
            glEnd();

        }

        if (object.state == 9) //SÝLGÝ MODU
        {            
            glColor3f(BgColor[0], BgColor[1], BgColor[2]);
            for (int i = 0; i < object.points.size() - 3; i += 2)
            {
                glBegin(GL_LINES);
                glVertex2f((GLfloat)object.points[i], -(GLfloat)object.points[i + 1]);
                glVertex2f((GLfloat)object.points[i + 2], -(GLfloat)object.points[i + 3]);
                glEnd();

            }

        }
    
    }
    glPopMatrix();
    glColor3f(DrawColor[0], DrawColor[1], DrawColor[2]);
    glLineWidth(lineWidth);
    
    return;
}

void OpenGL::FillGLObject(void) 
{
    if (objects.size() == 0)
    {
        return;
    }    

    for (int i = objects.size() - 1; i >= 0; i--)
    {
        if (objects[i].state == 3) //Üçgen
        {
            float centerX = (objects[i].points[2] + objects[i].points[0]) / 2;
            float centerY;

            if (objects[i].points[2] < objects[i].points[0])
            {
                centerY = objects[i].points[1] + (objects[i].points[2] - objects[i].points[0]);
            }
            else
            {
                centerY = objects[i].points[1] - (objects[i].points[2] - objects[i].points[0]);
            }
            float dist = sqrt(pow(pStartX - centerX, 2) +pow(pStartY - centerY, 2) * 1.0);
        
            if (dist <= (objects[i].points[1]) - centerY)
            {
                objects[i].filled = 1;
                objects[i].colorRGB[0] = DrawColor[0];
                objects[i].colorRGB[1] = DrawColor[1];
                objects[i].colorRGB[2] = DrawColor[2];

                break;
            }            

        }

        if (objects[i].state == 4) //Dikdörtgen
        {

            if (pStartX >= objects[i].points[0] && pStartX <= objects[i].points[2]
                && pStartY >= objects[i].points[1] && pStartY <= objects[i].points[3])
            {                               
                objects[i].filled = 1;
                objects[i].colorRGB[0] = DrawColor[0];
                objects[i].colorRGB[1] = DrawColor[1];
                objects[i].colorRGB[2] = DrawColor[2];

                break;
            }

            if (pStartX <= objects[i].points[0] && pStartX >= objects[i].points[2]
                && pStartY >= objects[i].points[1] && pStartY <= objects[i].points[3])
            {
                objects[i].filled = 1;
                objects[i].colorRGB[0] = DrawColor[0];
                objects[i].colorRGB[1] = DrawColor[1];
                objects[i].colorRGB[2] = DrawColor[2];

                break;
            }

            if (pStartX >= objects[i].points[0] && pStartX <= objects[i].points[2]
                && pStartY <= objects[i].points[1] && pStartY >= objects[i].points[3])
            {
                objects[i].filled = 1;
                objects[i].colorRGB[0] = DrawColor[0];
                objects[i].colorRGB[1] = DrawColor[1];
                objects[i].colorRGB[2] = DrawColor[2];

                break;
            }

            if (pStartX <= objects[i].points[0] && pStartX >= objects[i].points[2]
                && pStartY <= objects[i].points[1] && pStartY >= objects[i].points[3])
            {
                objects[i].filled = 1;
                objects[i].colorRGB[0] = DrawColor[0];
                objects[i].colorRGB[1] = DrawColor[1];
                objects[i].colorRGB[2] = DrawColor[2];

                break;
            }
        }

        if (objects[i].state == 5) // Beþgen
        {
            float p1X = (float)sin(28) * (objects[i].points[2] - objects[i].points[0]) + objects[i].points[2];
            float p1Y = (float)cos(28) * (objects[i].points[2] - objects[i].points[0]) + objects[i].points[1];
            float p2X = ((objects[i].points[2] - objects[i].points[0]) / 2) + objects[i].points[0];
            float p2Y = p1Y + ((float)sin(36) * (objects[i].points[2] - objects[i].points[0]));
            float p3X = objects[i].points[0] - ((float)sin(28) * (objects[i].points[2] - objects[i].points[0]));
            float p3Y = p1Y;

            if (pStartX <= p1X && pStartX >= p3X && pStartY >= p2Y && pStartY <= objects[i].points[1])
            {
                objects[i].filled = 1;
                objects[i].colorRGB[0] = DrawColor[0];
                objects[i].colorRGB[1] = DrawColor[1];
                objects[i].colorRGB[2] = DrawColor[2];

                break;
            }

            if (pStartX >= p1X && pStartX <= p3X && pStartY <= p2Y && pStartY >= objects[i].points[1])
            {
                objects[i].filled = 1;
                objects[i].colorRGB[0] = DrawColor[0];
                objects[i].colorRGB[1] = DrawColor[1];
                objects[i].colorRGB[2] = DrawColor[2];

                break;
            }
        }

        if (objects[i].state == 7 || objects[i].state == 8) // Çember veya elips
        {
            float centerX = (objects[i].points[2] + objects[i].points[0]) / 2;
            float centerY = (objects[i].points[3] + objects[i].points[1]) / 2;
            // Baþlangýç ve merkez noktasý arasýndaki uzaklýk(Yarýçap)
            float radius = sqrt(pow((centerX - objects[i].points[0]), 2) + pow((centerY - objects[i].points[1]), 2));
            float dist = sqrt(pow(pStartX - centerX, 2) + pow(pStartY - centerY, 2) * 1.0);

            if (dist <= radius)
            {
                objects[i].filled = 1;
                objects[i].colorRGB[0] = DrawColor[0];
                objects[i].colorRGB[1] = DrawColor[1];
                objects[i].colorRGB[2] = DrawColor[2];

                break;
            }

        }
    }
    RedrawGLScene();    
    return;
}



void OpenGL::SavePoints(int state)
{
    Object object;

    if (state == 1 || state == 9) //KALEM VEYA SÝLGÝ TARAFINDAN ÇÝZÝLEN ÖGELERÝN TÜM ARA NOKTALARI TUTULUYOR.
    {
        object.points = penInnerPoints;
        penInnerPoints.clear();
    }
    else{
        object.points.push_back(pStartX);
        object.points.push_back(pStartY);
        object.points.push_back(pEndX);
        object.points.push_back(pEndY);
    }     
        
    glGetFloatv(GL_CURRENT_COLOR, object.colorRGB);
    
    object.state = state;
    object.filled = filled;
    object.lineWidth = lineWidth;
    objects.push_back(object);
    return;
}

void OpenGL :: SaveGLWindow(char* filename)
{   //width ve height 4 ün katý olunca çalýþýyor.
    int width = GLRect.right;
    int height = GLRect.bottom;
    while (width % 4 != 0){width += 1;}
    while (height % 4 != 0) { height += 1; }

    BITMAPFILEHEADER bf;
    BITMAPINFOHEADER bi;

    unsigned char *image;
    image = new unsigned char[(width * height * 3)];

    FILE *file = fopen(filename, "wb");
    if (image != NULL)
    {
        if (file != NULL)
        {
            glPixelStorei(GL_PACK_ALIGNMENT, 1);
            glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, image);

            memset(&bf, 0, sizeof(bf));
            memset(&bi, 0, sizeof(bi));

            bf.bfType = 'MB';
            bf.bfSize = sizeof(bf) + sizeof(bi) + width * height * 3;
            bf.bfOffBits = sizeof(bf) + sizeof(bi);
            bi.biSize = sizeof(bi);
            bi.biWidth = width;
            bi.biHeight = height;
            bi.biPlanes = 1;
            bi.biBitCount = 24;
            bi.biSizeImage = width * height * 3;

            fwrite(&bf, sizeof(bf), 1, file);
            fwrite(&bi, sizeof(bi), 1, file);
            fwrite(image, sizeof(unsigned char), height * width * 3, file); 

            fclose(file);
        }
        
    }
    
}


void OpenGL::ResizeGLWindow(int way)
{

    if (way == 0) // saða geniþlet
    {

        
    }

    if (way == 1) // aþaðý geniþlet 
    {
       

    }

    
    return;
}


void OpenGL :: ScrollGLWindow(int scrollType, int scrollLength) 
{
    if (scrollType == 0) // Dikey kaydýrma 
    {      
        
    }

    if (scrollType == 1) // Yatay kaydýrma
    {
    
    }

}

void OpenGL::ChangeGLColor(GLfloat red, GLfloat green, GLfloat blue, int target)
{
    if (target == 0) // Çizim rengini deðiþtir
    {
        DrawColor[0] = red / 255;
        DrawColor[1] = green / 255;
        DrawColor[2] = blue / 255;

        return;
    }

    if (target == 1) // Arkaplan rengini deðiþtir
    {
        BgColor[0] = red / 255;
        BgColor[1] = green / 255;
        BgColor[2] = blue / 255;
        glClearColor(BgColor[0], BgColor[1], BgColor[2], 0.0); 
        glClear(GL_COLOR_BUFFER_BIT);
        RedrawGLScene();
        return;
    }
}


