#pragma once
#include "View.h"

class TextEditor
{
public:
	TextEditor();
	TextEditor(char c);

	void ParseNormalMode();
	void ParseCommandMode();
	void ParseHexMode();
	float CurrentStrEnd(int row, int& column, bool &flag);
	void MoveRight();
	void MoveLeft();
	void MoveUp();
	void MoveDown();

	char status_; //n-режим редактирования(основной)/c-режим ввода команд/i-режим ввода текста/h-режим hex редактора
	String<char> file_name_; // имя открытого файла
	String<char> comman_line_; // текст в окне команд
	String<char> current_status_; // текущий режим работы VIM
	std::vector<String<char>> current_text_; // текущий текст в виде символов
	String<char> buffer_; // буффер для копирования
	float CursorPosX;
	float CursorPosY;
	int current_str; // текущая строчка
	int current_str_ind; // текущий индекс в текущей строчке
	int row;
	int column;
	int begin_row;
	int end_row;
	bool stop;
	bool was_changed;
	bool update;
	bool help;
private:
	bool IsDigit(int ind);
	void SaveToFile();
	void MoveTo(int row);
	void MoveToBeginOfStr();
	void MoveToEndOfStr();
};
