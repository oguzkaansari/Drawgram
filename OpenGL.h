#pragma once
#include <GL\freeglut.h>
#include <vector> 

using namespace std;

class OpenGL
{
	public:	
		struct Object {
			vector<float> points; //(x1,y1,x2,y2... s�ras�yla �izim noktalar�n� tutar.)
			GLfloat colorRGB[3] = {0.0, 0.0, 0.0}; //Renk parametrelerini RGB s�ras�yla tutar.
			int state = 0; // �ekil bilgisi
			int filled = 0; // ��i dolu mu 
			float lineWidth = 0;
		}; 
		vector<Object> objects;
		GLfloat DrawColor[3] = {1.0, 1.0, 1.0}; //default �izim rengi beyaz
		GLfloat BgColor[3] = { 0.0, 0.0, 0.0 }; //arkaplan rengi siyah
		vector<float> penInnerPoints;
		int DRAW_STATE = 0; //Butonun anl�k �izim durumu 
		int filled = 0; // �ekillerinin i�lerinin dolu/bo� olma durumu (1 = dolu)
		int lineWidth = 2;
		float pStartX, pStartY, pEndX, pEndY;
		RECT GLRect;
		RECT MainWinRect;
		void SavePoints(int);
		void RedrawGLScene(void);
		void SetGLProjection();
		void SetGLView();
		void InitGL();
		void ResizeGLWindow(int way);
		void DrawGLScene(void);
		bool CreateGLContexts(HWND hWnd, HDC& hDC, HGLRC& hRC);
		void ReleaseGLContexts(HWND hWnd, HDC hDC, HGLRC hRC);
		void SaveGLWindow(char* filename);
		void ScrollGLWindow(int scrollType, int length);
		void ChangeGLColor(GLfloat red, GLfloat green, GLfloat blue, int target);
		void FillGLObject(void);

};

