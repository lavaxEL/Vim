#include "Controller.h"

TextEditor TE('n');
Controller::Controller()
{
	this->view_ = View('n');
}
//std::string str = "";
void AddSymbol(GLFWwindow* window, unsigned int codepoint)
{
	if (TE.status_ == 'n' || TE.status_ == 'c' || TE.status_=='h') // если главный режим или режим ввода команд или hex
	{
		char c = codepoint;
		TE.comman_line_.append(1, c);
		
	}
	else if (TE.status_ == 'i' && TE.current_text_.size()>0) // если режим ввода текста
	{
		if (TE.column == 35)
			TE.MoveRight();
		char c = codepoint;
		TE.current_text_[TE.current_str].insert(TE.current_str_ind, 1, c);
		TE.MoveRight();
		if (TE.current_str == TE.current_text_.size() - 1 && TE.current_str_ind == TE.current_text_[TE.current_str].size()-2)
			TE.MoveRight();
		TE.was_changed = true;
	}
	TE.update = true;
}
void CheckKeys(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action != GLFW_PRESS && action!=GLFW_REPEAT)
		return;
	if (key == 257) // нажата клавиша enter
	{
		if (TE.status_ == 'n')
			TE.ParseNormalMode();
		else if (TE.status_ == 'c')
			TE.ParseCommandMode();
		else if (TE.status_ == 'i')
		{
			TE.current_text_.push_back(String<char>());
			for (int i = TE.current_text_.size() - 1; i > TE.current_str + 1; --i)
				TE.current_text_[i] = TE.current_text_[i - 1];
			TE.current_text_[TE.current_str + 1].clear();
			for (int i = TE.current_str_ind; i < TE.current_text_[TE.current_str].size(); ++i)
				TE.current_text_[TE.current_str + 1].append(1, TE.current_text_[TE.current_str][i]);
			TE.current_text_[TE.current_str].erase(TE.current_str_ind, TE.current_text_[TE.current_str].size() - TE.current_str_ind);
			TE.current_text_[TE.current_str].append(1, '\n');
			++TE.current_str;
			TE.current_str_ind = 0;
			TE.column = 0;
			++TE.row;
			TE.CursorPosX = -1;
			TE.CursorPosY -= 0.075;
			if (TE.row == TE.end_row)
			{
				++TE.end_row;
				++TE.begin_row;
				TE.CursorPosY += 0.075;
			}
		}
		else if (TE.status_ == 'h')
			TE.ParseHexMode();
		TE.comman_line_.clear();
		TE.comman_line_.append("#");
	}
	else if (key == 261)
	{
		if (TE.status_ == 'i')
		{
			if (TE.current_text_[TE.current_str][TE.current_str_ind] == '\n')
			{
				if (TE.current_str == TE.current_text_.size() - 1)
					return;
				TE.current_text_[TE.current_str].erase(TE.current_str_ind, 1);
				
				for (int i = 0; i < TE.current_text_[TE.current_str+1].size(); ++i)
					TE.current_text_[TE.current_str].append(1, TE.current_text_[TE.current_str+1][i]);
				for (int i = TE.current_str+1; i < TE.current_text_.size() - 1; ++i)
					TE.current_text_[i] = TE.current_text_[i + 1];
				TE.current_text_.pop_back();
				TE.update = true;
				TE.was_changed = true;
				return;
			}
			TE.current_text_[TE.current_str].erase(TE.current_str_ind, 1);
			TE.was_changed = true;
		}
	}
	else if (key == 259) // bs
	{
		if (TE.status_ == 'n' || TE.status_ == 'c' || TE.status_ == 'h')
		{
			if (TE.comman_line_.size() > 1)
				TE.comman_line_.erase(TE.comman_line_.size() - 1, 1);
		}
		else if (TE.status_ == 'i')
		{
			if (TE.column == 0 && TE.row == 0)
				return;
			if (TE.current_str_ind > 0)
			{
				if (TE.column == 0 && TE.row != 0)
					TE.MoveLeft();
				TE.current_text_[TE.current_str].erase(TE.current_str_ind-1, 1);
				TE.MoveLeft();
			}
			else if (TE.current_str > 0)
			{
				TE.MoveLeft();
				TE.current_text_[TE.current_str].erase(TE.current_text_[TE.current_str].size() - 1, 1);
				for (int i = 0; i < TE.current_text_[TE.current_str+1].size(); ++i)
					TE.current_text_[TE.current_str].append(1, TE.current_text_[TE.current_str+1][i]);
				for (int i = TE.current_str+1; i < TE.current_text_.size() - 1; ++i)
					TE.current_text_[i] = TE.current_text_[i + 1];
				TE.current_text_.pop_back();
			}
			
			TE.was_changed = true;
		}
		
	}
	else if (key == 256) // esc
	{
		if (TE.comman_line_.size() > 1)
		{
			TE.comman_line_.clear();
			TE.comman_line_.append("#");
		}
		else
		{
			if (TE.status_ == 'h')
			{
				TE.CursorPosX = -1;
				TE.CursorPosY = 0.76;
				TE.row = 0;
				TE.column = 0;
				TE.current_str = 0;
				TE.current_str_ind = 0;
				TE.begin_row = 0;
				TE.end_row = 24;
			}
			TE.status_ = 'n';
			TE.current_status_ = "Normal mode";
		}
	}
	else if (key == GLFW_KEY_RIGHT && TE.status_ != 'h')
	{
		TE.MoveRight();
	}
	else if (key == GLFW_KEY_LEFT && TE.status_ != 'h')
	{
		TE.MoveLeft();
	}
	else if (key == GLFW_KEY_DOWN && TE.status_ != 'h')
	{
		TE.MoveDown();
	}
	else if (key == GLFW_KEY_UP && TE.row>0 && TE.status_ != 'h')
	{
		TE.MoveUp();
	}
	else if (key == 294)
	{
		TE.status_ = 'h';
		TE.current_status_ = "Hex mode";
		TE.CursorPosX = -0.797f;
		TE.CursorPosY = 0.73f;
		TE.row = 0;
		TE.column = 1;
		TE.current_str = 0;
		TE.current_str_ind = 0;
		TE.begin_row = 0;
		TE.end_row = 24;
	}
	else if (key == GLFW_KEY_RIGHT && TE.status_ == 'h')
	{
		if (TE.column == 16)
		{
			++TE.row;
			TE.column = 1;
			TE.CursorPosX = -0.797f;
			TE.CursorPosY -= 0.075;
			if (TE.row == TE.end_row)
			{
				++TE.begin_row;
				++TE.end_row;
				TE.CursorPosY += 0.075;
			}
		}
		else
		{
			++TE.column;
			TE.CursorPosX += 0.075f;
		}

	}
	else if (key == GLFW_KEY_LEFT && TE.status_ == 'h')
	{
		if (TE.column == 1 && TE.row>0)
		{
			--TE.row;
			TE.column = 16;
			TE.CursorPosX = 0.328f;
			TE.CursorPosY += 0.075;
			if (TE.row == TE.begin_row-1)
			{
				--TE.begin_row;
				--TE.end_row;
				TE.CursorPosY -= 0.075;
			}
		}
		else if(TE.row > 0 || TE.column>1)
		{
			--TE.column;
			TE.CursorPosX -= 0.075f;
		}
	}
	else if (key == GLFW_KEY_DOWN && TE.status_ == 'h')
	{
		if (TE.row == TE.end_row-1)
		{
			++TE.end_row;
			++TE.begin_row;
			++TE.row;
		}
		else
		{
			TE.CursorPosY -= 0.075;
			++TE.row;
		}
		
	}
	else if (key == GLFW_KEY_UP && TE.status_ == 'h' && TE.row>0)
	{
		if (TE.row == TE.begin_row)
		{
			--TE.end_row;
			--TE.begin_row;
			--TE.row;
		}
		else
		{
			TE.CursorPosY += 0.075;
			--TE.row;
		}
		
	}
	TE.update = true;
}
void Controller::Execute()
{
	glfwSetCharCallback(view_.window_, AddSymbol);
	glfwSetKeyCallback(view_.window_, CheckKeys);
	while (!glfwWindowShouldClose(view_.window_) && !TE.stop) // пока окно не закрыто
	{

		glfwPollEvents();
		if (TE.update)
		{
			view_.DrawMainScreen(TE.status_, TE.comman_line_, TE.file_name_, TE.current_status_, TE.current_text_, TE.CursorPosX, TE.CursorPosY, 
				TE.row, TE.column, TE.begin_row, TE.end_row, TE.help);
			TE.help = false;
			TE.update = false;
		}
			
	}
}