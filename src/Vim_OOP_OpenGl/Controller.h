#pragma once
#include <GLFW/glfw3.h>
#include <gl/GL.h>
//#include <GLFW/glut.h>
#include "TextEditor.h"


class Controller
{
public:
	Controller();
	void Execute(); // ������� ����� ���������� 
private:
	View view_;
};