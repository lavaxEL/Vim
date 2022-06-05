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

	char status_; //n-����� ��������������(��������)/c-����� ����� ������/i-����� ����� ������/h-����� hex ���������
	String<char> file_name_; // ��� ��������� �����
	String<char> comman_line_; // ����� � ���� ������
	String<char> current_status_; // ������� ����� ������ VIM
	std::vector<String<char>> current_text_; // ������� ����� � ���� ��������
	String<char> buffer_; // ������ ��� �����������
	float CursorPosX;
	float CursorPosY;
	int current_str; // ������� �������
	int current_str_ind; // ������� ������ � ������� �������
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
