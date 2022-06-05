#pragma once
#include <vector>
#include "DynArray.h"
#include <GLFW/glut.h>
#include <GLFW/glfw3.h>
#include <gl/GL.h>
//#include "Controller.h"


class View
{
public:
	View();
	View(char name);
	void DrawMainScreen(char mode, String<char> command_text, String<char> cuurent_file_name_text, String<char> current_mode_text,
	 std::vector<String<char>> current_text, float cursor_x, float cursor_y, int row, int column, int begin_row, int end_row, bool help); // отрисвка экрана обычного режима
	int str_begin;
	int ind_begin;
	bool point;
	GLFWwindow* window_;
private:
	void DrawLine(GLfloat x1, GLfloat x2, GLfloat y1, GLfloat y2);
	void DrawText(String<char> text, int length, int x, int y);
	char* IntToMyString(int input);
	void DrawBytes();
	void DrawHexCode(char sym, int x, int y);
	void DrawIndex(int row, int x, int y);
	void StartForHexMode(int begin_row, int end_row, std::vector<String<char>> current_text);
	void StartForNormMode(int begin_row, std::vector<String<char>> current_text);
};