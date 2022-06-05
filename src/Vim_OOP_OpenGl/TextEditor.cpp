#include "TextEditor.h"
#include <fstream>
TextEditor::TextEditor()
{ }
TextEditor::TextEditor(char c)
{
	comman_line_ = "#";
	status_ = c;
    current_status_ = "Normal mode";
    CursorPosX = -1;
    CursorPosY = 0.76;
    row = 0;
    column = 0;
    current_str = 0;
    current_str_ind = 0;
    begin_row = 0;
    end_row = 24;
    stop = false;
    was_changed = false;
    update = true;
    help = false;
}
float TextEditor::CurrentStrEnd(int row, int &column, bool &flag)
{
    int cur_row = 0;
    float end = -1;
    for (int i = 0; i < current_text_.size(); ++i)
    {
        for (int j = 0; j < current_text_[i].size(); ++j)
        {
            if (cur_row == row)
            {
                if (current_text_[i][j] == '\n' || column == 35)
                {
                    if (current_text_[i][j] == '\n' && column!=35)
                        flag = true;
                    return end;
                }
                    
                ++column;
                end += 0.0397;
            }
            else if (cur_row < row)
            {
                if (current_text_[i][j] == '\n' || column == 35)
                {
                    ++cur_row;
                    if (current_text_[i][j] == '\n' && column == 35 && cur_row == row)
                    {
                        flag = true;
                        column = 0;
                        return end;
                    }     
                    if (current_text_[i][j] == '\n' && column == 35)
                        ++cur_row;
                    column = 0;
                    if (current_text_[i][j] == '\n')
                        break;
                    else if(cur_row == row)
                        end += 0.0397;
                }
                ++column;
            }
        }
    }
    return -2;
}
bool TextEditor::IsDigit(int ind)
{
    if (comman_line_[ind] == '0' || comman_line_[ind] == '1' || comman_line_[ind] == '2' || comman_line_[ind] == '3' || comman_line_[ind] == '4' ||
        comman_line_[ind] == '5' || comman_line_[ind] == '6' || comman_line_[ind] == '7' || comman_line_[ind] == '8' || comman_line_[ind] == '9')
        return true;
    else
        return false;
}
void TextEditor::SaveToFile()
{
    std::ofstream file;
    char* file_name = new char[file_name_.size() * sizeof(char)];
    for (int i = 0; i < file_name_.size(); ++i)
        file_name[i] = file_name_[i];
    file_name[file_name_.size()] = '\0';
    file.open(file_name, std::ofstream::out | std::ofstream::trunc);
    for (int i = 0; i < current_text_.size(); ++i)
        for (int j = 0; j < current_text_[i].size(); ++j)
            file << current_text_[i][j];
    file.close();
}
void TextEditor::MoveRight()
{
    if (current_str == current_text_.size() - 1 && current_str_ind == current_text_[current_str].size() - 1)
        return;
    if (column == 35)
    {
        CursorPosY -= 0.075;
        CursorPosX = -1;
        column = 0;
        if (row == end_row - 1)
        {
            ++end_row;
            ++begin_row;
            CursorPosY += 0.075;
        }
        ++row;
    }
    else
    {
        if (current_text_[current_str][current_str_ind] == '\n')
        {
            ++current_str;
            current_str_ind = 0;
            CursorPosY -= 0.075;
            CursorPosX = -1;
            column = 0;
            if (row == end_row - 1)
            {
                ++end_row;
                ++begin_row;
                CursorPosY += 0.075;
            }
            ++row;
        }
        else
        {
            CursorPosX += 0.0397;
            ++column;
            ++current_str_ind;
        }
    }
}
void TextEditor::MoveLeft()
{
    bool scroll = false;
    if (column == 0 && row == 0)
        return;
    if (current_str_ind == 0)
    {
        if (row == begin_row)
                scroll = true;
        int beg = begin_row;
        int end = end_row;
        int target_str=current_str-1;
        CursorPosX = -1;
        CursorPosY = 0.76;
        row = 0;
        column = 0;
        current_str = 0;
        current_str_ind = 0;
        begin_row = 0;
        end_row = 24;
        while (current_str != target_str || current_str_ind != current_text_[current_str].size()-1)
            MoveRight();
        if (column==35)
            MoveRight();
        for (int i = begin_row; i < beg; ++i)
        {
            ++begin_row;
            ++end_row;
            CursorPosY += 0.075;
        }
    }
    else if (column == 0)
    {
        if (row == begin_row)
            scroll = true;
        column = 35;
        --row;
        CursorPosY += 0.075;
        CursorPosX = 0.389f;
    }
    else
    {
        --column;
        CursorPosX -= 0.0397;
        --current_str_ind;
    }
    if (scroll)
    {
        CursorPosY -= 0.075;
        --begin_row;
        --end_row;
    }
}
void TextEditor::MoveUp()
{
    if (row == 0)
        return;
    int col = column;
    int r = row;
    do
    {
        MoveLeft();
    } while (column > col || row == r);
}
void TextEditor::MoveDown()
{
    int col = column;
    int r = row;
    do
    {
        if (current_str == current_text_.size() - 1 && current_str_ind == current_text_[current_str].size() - 1)
            return;
        MoveRight();
        if (row - r == 2)
        {
            MoveLeft();
            return;
        }
    } while (column != col);
}
void TextEditor::MoveTo(int target)
{
    if (target < 0)
        return;
    if (target < row) // вверх
    {
        CursorPosX = -1;
        CursorPosY = 0.76;
        row = 0;
        column = 0;
        current_str = 0;
        current_str_ind = 0;
        begin_row = 0;
        end_row = 24;
        while (row != target)
            MoveRight();
    }
    else // вниз
    {
        while (row != target && (current_str!=current_text_.size()-1 || current_str_ind!= current_text_[current_str].size()-1))
            MoveRight();
    }
}
void TextEditor::MoveToBeginOfStr()
{
    CursorPosX = -1;
    current_str_ind -= column;
    column = 0;
}
void TextEditor::MoveToEndOfStr()
{
    MoveRight();
    while (column != 0 && (current_str != current_text_.size() - 1 || current_str_ind != current_text_[current_str].size() - 1))
        MoveRight();
    if (current_str != current_text_.size() - 1 || current_str_ind != current_text_[current_str].size() - 1)
        MoveLeft();
}
void TextEditor::ParseHexMode()
{
    if (comman_line_[comman_line_.size() - 1] == 'G' && comman_line_.size() > 2)
    {
        comman_line_.erase(comman_line_.size() - 1, 1);
        comman_line_.erase(0, 1);
        int num = 0;
        int temp = 0;
        String<char> code;
        while (code != comman_line_ && num<65536)
        {
            code.clear();
            do
            {
                int r(temp % 16);
                if (r > 9)
                    r += (int)'A' - 10;
                else
                    r += (int)'0';
                code.insert(0, 1, (char)r);
                temp /= 16;
            } while (temp);
            ++num;
            temp = num;
        }
        --num;
        begin_row = num / 16;
        end_row = begin_row + 24;
        column = (num % 16)+1;
        CursorPosX = -0.797f+((column-1)* 0.075f);
        CursorPosY = 0.73f;
        row = begin_row;
    }
}
void TextEditor::ParseNormalMode()
{
    if (comman_line_.size() == 1)
        return;
    if (current_text_.size() == 0 || (current_text_.size() == 1 && current_text_[0].size() <= 1))
        if (comman_line_[1] != 'i' && comman_line_[1] != ':' && comman_line_[1] != 'h')
            return;
    if (comman_line_.size() == 2)
    {
        if (comman_line_[1] == 'i') // вывод текста перед курсором
        {
            status_ = 'i';
            current_status_ = "Input mode";
            if (current_text_.size() == 0)
            {
                current_text_.push_back(String<char>());
                current_text_[0].append(1, '\n');
            }
        }
        else if (comman_line_[1] == 'I') // Перейти в начало строки и начать ввод текста
        {
            MoveToBeginOfStr();
            status_ = 'i';
            current_status_ = "Input mode";
        }
        else if (comman_line_[1] == 'A') // Перейти в конец строки и начать ввод текста
        {
            MoveToEndOfStr();
            status_ = 'i';
            current_status_ = "Input mode";
        }
        else if (comman_line_[1] == 'S') // Удалить содержимое строки и начать ввод текста
        {
            status_ = 'i';
            current_status_ = "Input mode";
            MoveToEndOfStr();
            while (true)
            {
                if (column == 0)
                    break;
                current_text_[current_str].erase(current_str_ind - 1, 1);
                MoveLeft();
            }
            if (current_text_[current_str].size() <= 102)
            {
                for (int i = current_str; i < current_text_.size() - 1; ++i)
                    current_text_[i] = current_text_[i + 1];
                current_text_.pop_back();
                CursorPosX = -1;
                CursorPosY = 0.76;
                row = 0;
                column = 0;
                current_str = 0;
                current_str_ind = 0;
                begin_row = 0;
                end_row = 24;
            }
            was_changed = true;
        }
        else if (comman_line_[1] == ':')
        {
            status_ = 'c';
            current_status_ = "Command mode";
        }
        else if (comman_line_[1] == 'G') // 
        {
            while (current_str != current_text_.size() - 1 || current_str_ind != current_text_[current_text_.size() - 1].size() - 1)
                MoveRight();
        }
        else if (comman_line_[1] == '^') // курсор в начало строки
        {
            MoveToBeginOfStr();
        }
        else if (comman_line_[1] == '$') // курсор в конец строки
        {
            MoveToEndOfStr();
        }
        else if (comman_line_[1] == 'w') // курсор в конец слова или в конец следующего следующего
        {
            if (current_str == current_text_.size() - 1 && current_str_ind == current_text_[current_text_.size() - 1].size() - 1)
                return;
            int cur = current_str_ind;
            while (current_text_[current_str][current_str_ind] != ' ' && current_text_[current_str][current_str_ind] != ',' &&
                current_text_[current_str][current_str_ind] != '?' && current_text_[current_str][current_str_ind] != ':' &&
                current_text_[current_str][current_str_ind] != '-' && current_text_[current_str][current_str_ind] != '!' &&
                current_text_[current_str][current_str_ind] != '\n' && current_text_[current_str][current_str_ind] != '.')
                MoveRight();
            bool exit = true;
            int s = current_str_ind;
            for (int i = current_str; i < current_text_.size(); ++i)
            {
                for (int j = s; j < current_text_[i].size(); ++j)
                {
                    if (current_text_[i][j] != ' ' && current_text_[i][j] != ',' &&
                        current_text_[i][j] != '?' && current_text_[i][j] != ':' &&
                        current_text_[i][j] != '-' && current_text_[i][j] != '!' &&
                        current_text_[i][j] != '\n' && current_text_[i][j] != '.')
                    {
                        exit = false;
                        break;
                    }

                }
                s = 0;
            }
            if (exit)
                return;
            if (cur == current_str_ind /*|| cur == current_str_ind - 1*/)
            {
                while (current_text_[current_str][current_str_ind] == ' ' || current_text_[current_str][current_str_ind] == ',' ||
                    current_text_[current_str][current_str_ind] == '?' || current_text_[current_str][current_str_ind] == ':' ||
                    current_text_[current_str][current_str_ind] == '-' || current_text_[current_str][current_str_ind] == '!' ||
                    current_text_[current_str][current_str_ind] == '\n' || current_text_[current_str][current_str_ind] == '.')
                    MoveRight();
            }
            while (current_text_[current_str][current_str_ind] != ' ' && current_text_[current_str][current_str_ind] != ',' &&
                current_text_[current_str][current_str_ind] != '?' && current_text_[current_str][current_str_ind] != ':' &&
                current_text_[current_str][current_str_ind] != '-' && current_text_[current_str][current_str_ind] != '!' &&
                current_text_[current_str][current_str_ind] != '\n' && current_text_[current_str][current_str_ind] != '.')
                MoveRight();
        }
        else if (comman_line_[1] == 'b') // в начало текущего или в начало предыдущего следующего
        {
        MoveLeft();
        bool that = false;
        if (current_text_[current_str][current_str_ind] == ' ' || current_text_[current_str][current_str_ind] == ',' ||
            current_text_[current_str][current_str_ind] == '?' || current_text_[current_str][current_str_ind] == ':' ||
            current_text_[current_str][current_str_ind] == '-' || current_text_[current_str][current_str_ind] == '!' ||
            current_text_[current_str][current_str_ind] == '\n' || current_text_[current_str][current_str_ind] == '.')
            that = false;
        else
            that = true;
        if (that)
        {
            while (current_text_[current_str][current_str_ind] != ' ' && current_text_[current_str][current_str_ind] != ',' &&
                current_text_[current_str][current_str_ind] != '?' && current_text_[current_str][current_str_ind] != ':' &&
                current_text_[current_str][current_str_ind] != '-' && current_text_[current_str][current_str_ind] != '!' &&
                current_text_[current_str][current_str_ind] != '\n' && current_text_[current_str][current_str_ind] != '.')
            {
                MoveLeft();
                if (column == 0 && row == 0)
                    return;
            }
            MoveRight();
        }
        else
        {
            while (current_text_[current_str][current_str_ind] == ' ' || current_text_[current_str][current_str_ind] == ',' ||
                current_text_[current_str][current_str_ind] == '?' || current_text_[current_str][current_str_ind] == ':' ||
                current_text_[current_str][current_str_ind] == '-' || current_text_[current_str][current_str_ind] == '!' ||
                current_text_[current_str][current_str_ind] == '\n' || current_text_[current_str][current_str_ind] == '.')
            {
                {
                    MoveLeft();
                    if (column == 0 && row == 0)
                        return;
                }
            }
            while (current_text_[current_str][current_str_ind] != ' ' && current_text_[current_str][current_str_ind] != ',' &&
                current_text_[current_str][current_str_ind] != '?' && current_text_[current_str][current_str_ind] != ':' &&
                current_text_[current_str][current_str_ind] != '-' && current_text_[current_str][current_str_ind] != '!' &&
                current_text_[current_str][current_str_ind] != '\n' && current_text_[current_str][current_str_ind] != '.')
            {
                MoveLeft();
                if (column == 0 && row == 0)
                    return;
            }
            MoveRight();
        }
        }
        else if (comman_line_[1] == 'y') // копировать текущую строку
        {
            int start = 0;
            int end = 0;
            MoveToBeginOfStr();
            start = current_str_ind;
            MoveToEndOfStr();
            end = current_str_ind;
            buffer_.clear();
            for (int i = start; i < end; ++i)
                buffer_.append(1, current_text_[current_str][i]);
        }
        else if (comman_line_[1] == 'p') // вставить
        {
        bool new_str = false;
            for (int i = 0; i < buffer_.size(); ++i)
            {
                if (column == 35)
                    new_str = true;
                current_text_[current_str].insert(current_str_ind, 1, buffer_[i]);
                MoveRight();
            }
            if (new_str)
                MoveRight();
            was_changed = true;
        }
        else if (comman_line_[1] == 'h') // команды
        {
            help = true;
        }
        return;
    }
    else if (comman_line_[1] == 'g' && comman_line_[2] == 'g')
    {
        while (current_str != 0 || current_str_ind != 0)
            MoveLeft();
    }
    else if (comman_line_[comman_line_.size() - 1] == 'G')
    {
        int num = 0;
        int k = 1;
        for (int i = comman_line_.size() - 2; i > 0; --i)
        {
            num += (comman_line_[i] - '0') * k;
            k *= 10;
        }
        while (current_str != 0 || current_str_ind != 0)
            MoveLeft();
        while (row != num && (current_str != current_text_.size() - 1 || current_str_ind != current_text_[current_str].size() - 2))
            MoveRight();
    }
    else if (comman_line_[1] == 'd' && comman_line_[2] == 'd')
    {
        buffer_.clear();
        MoveToEndOfStr();
        while (true)
        {
            if (column == 0)
                break;
            buffer_.append(1, current_text_[current_str][current_str_ind - 1]);
            current_text_[current_str].erase(current_str_ind - 1, 1);
            MoveLeft();
        }
        if (current_text_[current_str].size() <= 102)
        {
            for (int i = current_str; i < current_text_.size() - 1; ++i)
                current_text_[i] = current_text_[i + 1];
            current_text_.pop_back();
            CursorPosX = -1;
            CursorPosY = 0.76;
            row = 0;
            column = 0;
            current_str = 0;
            current_str_ind = 0;
            begin_row = 0;
            end_row = 24;
        }
        was_changed = true;
    }
    else if (comman_line_[1] == '/' && comman_line_.size() > 2)
    {
        MoveRight();
        char* text = new char[comman_line_.size() * sizeof(char)];
        int j = 0;
        for (int i = 2; i < comman_line_.size(); ++i, ++j)
            text[j] = comman_line_[i];
        text[j] = '\0';
        int res = 0;
        res = current_text_[current_str].find(text, current_str_ind);
        if (res != -1 && res < current_text_[current_str].size() && res>0)
        {
            while (current_str_ind != res)
                MoveRight();
            return;
        }
        for (int i = current_str + 1; i < current_text_.size(); ++i)
        {
            res = current_text_[i].find(text);
            if (res != -1 && res < current_text_[i].size() && res>0)
            {
                while (current_str_ind != res || current_str != i)
                    MoveRight();
                return;
            }
        }
    }
    else if (comman_line_[1] == '?' && comman_line_.size() > 2)
    {
        char* text = new char[comman_line_.size() * sizeof(char)];
        int j = 0;
        for (int i = 2; i < comman_line_.size(); ++i, ++j)
            text[j] = comman_line_[i];
        text[j] = '\0';
        int res = 0;
        res = current_text_[current_str].find(text);
        if (res != -1 && res < current_text_[current_str].size() && res < current_str_ind && res>0)
        {
            while (current_str_ind != res && res > 0)
                MoveLeft();
            return;
        }
        if (current_str == 0)
            return;
        for (int i = current_str - 1; i >= 0; --i)
        {
            res = current_text_[i].find(text);
            if (res != -1 && res < current_text_[i].size() && res>0)
            {
                while (current_str_ind != res || current_str != i)
                    MoveLeft();
                return;
            }
        }

    }
    else if (comman_line_[1] == 'd' && comman_line_[2] == 'i' && comman_line_[3] == 'w')
    {
        int beg = 0;
        int end = 0;
        if (current_text_[current_str][current_str_ind] == ' ' || current_text_[current_str][current_str_ind] == ',' ||
            current_text_[current_str][current_str_ind] == '?' || current_text_[current_str][current_str_ind] == ':' ||
            current_text_[current_str][current_str_ind] == '-' || current_text_[current_str][current_str_ind] == '!' ||
            current_text_[current_str][current_str_ind] == '\n' || current_text_[current_str][current_str_ind] == '.')
            return;
        while (current_text_[current_str][current_str_ind] != ' ' && current_text_[current_str][current_str_ind] != ',' &&
            current_text_[current_str][current_str_ind] != '?' && current_text_[current_str][current_str_ind] != ':' &&
            current_text_[current_str][current_str_ind] != '-' && current_text_[current_str][current_str_ind] != '!' &&
            current_text_[current_str][current_str_ind] != '\n' && current_text_[current_str][current_str_ind] != '!')
            MoveRight();
        if (current_text_[current_str][current_str_ind] == ' ')
            end = current_str_ind;
        else
            end = current_str_ind - 1;
        bool strt = false;
        do
        {
            if (column == 0 && row == 0)
            {
                strt = true;
                break;
            }
            MoveLeft();
        } while (current_text_[current_str][current_str_ind] != ' ' && current_text_[current_str][current_str_ind] != ',' &&
            current_text_[current_str][current_str_ind] != '?' && current_text_[current_str][current_str_ind] != ':' &&
            current_text_[current_str][current_str_ind] != '-' && current_text_[current_str][current_str_ind] != '!' &&
            current_text_[current_str][current_str_ind] != '\n' && current_text_[current_str][current_str_ind] != '!');
        beg = current_str_ind;
        if (current_text_[current_str][current_str_ind] == '\n')
        {
            MoveRight();
            beg = -1;
        }
        if (strt)
            beg = -1;
        for (int i = end; i > beg; --i)
            current_text_[current_str].erase(i, 1);
        was_changed = true;

    }
    else if (comman_line_[1] == 'y' && comman_line_[2] == 'w')
    {
    int beg = 0;
    int end = 0;
    if (current_text_[current_str][current_str_ind] == ' ' || current_text_[current_str][current_str_ind] == ',' ||
        current_text_[current_str][current_str_ind] == '?' || current_text_[current_str][current_str_ind] == ':' ||
        current_text_[current_str][current_str_ind] == '-' || current_text_[current_str][current_str_ind] == '!' ||
        current_text_[current_str][current_str_ind] == '\n' || current_text_[current_str][current_str_ind] == '.')
        return;
    while (current_text_[current_str][current_str_ind] != ' ' && current_text_[current_str][current_str_ind] != ',' &&
        current_text_[current_str][current_str_ind] != '?' && current_text_[current_str][current_str_ind] != ':' &&
        current_text_[current_str][current_str_ind] != '-' && current_text_[current_str][current_str_ind] != '!' &&
        current_text_[current_str][current_str_ind] != '\n' && current_text_[current_str][current_str_ind] != '!')
        MoveRight();
    end = current_str_ind;
    bool strt = false;
    do
    {
        if (column == 0 && row == 0)
        {
            strt = true;
            break;
        }
        MoveLeft();
    } while (current_text_[current_str][current_str_ind] != ' ' && current_text_[current_str][current_str_ind] != ',' &&
        current_text_[current_str][current_str_ind] != '?' && current_text_[current_str][current_str_ind] != ':' &&
        current_text_[current_str][current_str_ind] != '-' && current_text_[current_str][current_str_ind] != '!' &&
        current_text_[current_str][current_str_ind] != '\n' && current_text_[current_str][current_str_ind] != '!');
    beg = current_str_ind+1;
    if (current_text_[current_str][current_str_ind] == '\n')
    {
        MoveRight();
        beg = 0;
    }
    if (strt)
        beg = 0;
    else
        MoveRight();
    buffer_.clear();
    for (int i = beg; i < end; ++i)
        buffer_.append(1, current_text_[current_str][i]);

    }

}
void TextEditor::ParseCommandMode()
{
    int number = 0;
    if (comman_line_.size() == 1)
        return;
    if (current_text_.size() == 0 || (current_text_.size() == 1 && current_text_[0].size() <= 1))
        if (comman_line_[1] != 'q' && comman_line_[1] != 'o' && comman_line_[1] != 'h')
            return;
    if (comman_line_.size() == 2) // все однобуквенные команды
    {
        if (comman_line_[1] == 'x') // Записать в текущий файл и выйти
        {
            SaveToFile();
            stop = true;
        }
        else if (comman_line_[1] == 'w') // Записать в текущий файл
        {
            SaveToFile();
        }
        else if (comman_line_[1] == 'q') // Выйти. Если файл был изменён, то выход возможен только через q!
        {
            if (!was_changed)
                stop = true;
        }
        else if (comman_line_[1] == 'h') // справка по командам
        {
            help = true;
        }
        else if (IsDigit(1)) // перейти на эту строку
        {
            number = (comman_line_[1] - '0');
            MoveTo(number);
        }
        return;
    }
    else if (comman_line_.size() == 3) // все 2 буквенные команды
    {
        if (comman_line_[1] == 'q' && comman_line_[2] == '!') // выйти без сохранения
        {
            stop = true;
        }
        else if (IsDigit(1) && IsDigit(2)) // перейти на эту строку
        {
            number = (comman_line_[1] - '0') * 10 + (comman_line_[2] - '0');
            MoveTo(number);
        }
        return;
    }
    else if (comman_line_.size() == 4) // все 3 буквенные команды
    {
        if (IsDigit(1) && IsDigit(2) && IsDigit(3)) // перейти на эту строку
        {
            number = (comman_line_[1] - '0') * 100 + (comman_line_[2] - '0')*10+ (comman_line_[3] - '0');
            MoveTo(number);
        }
        return;
    }
    else if (comman_line_[1] == 'o') // открытие файла
    {
        char *file_name;
        file_name = new char[(comman_line_.size() - 1) * sizeof(char)];
        for (int i = 3; i < comman_line_.size(); ++i)
        {
            file_name[i - 3] = comman_line_[i];
        }
        file_name[comman_line_.size() - 3] = '\0';
        std::ifstream file;
        file.open(file_name);
        if (file.is_open())
            file_name_ = file_name;
        else
            return;
        char c = 'n';
        current_text_.clear();
        current_text_.push_back(String<char>());
        int i = 0;
        while (!file.eof())
        {
            c = file.get();
            current_text_[i].append(1, c);
            if (c == '\n')
            {
                ++i;
                current_text_.push_back(String<char>());
            }
        }
        current_text_[current_text_.size() - 1][current_text_[current_text_.size() - 1].size() - 1] = '\n';
        //current_text_[current_text_.size() - 1].append(1, '\n');
        was_changed = false;
        CursorPosX = -1;
        CursorPosY = 0.76;
        row = 0;
        column = 0;
        current_str = 0;
        current_str_ind = 0;
        begin_row = 0;
        end_row = 24;
        stop = false;
    }
    else if (comman_line_[1] == 'w') // записать в файл
    {
        if (current_text_.size() == 0)
            return;
        char* file_name;
        file_name = new char[(comman_line_.size() - 1) * sizeof(char)];
        for (int i = 3; i < comman_line_.size(); ++i)
        {
            file_name[i - 3] = comman_line_[i];
        }
        file_name[comman_line_.size() - 3] = '\0';
        std::ofstream file;
        current_text_[current_text_.size() - 1].erase(current_text_[current_text_.size() - 1].size() - 1, 1);
        file.open(file_name, std::ofstream::out | std::ofstream::trunc);
        for (int i = 0; i < current_text_.size(); ++i)
            for (int j = 0; j < current_text_[i].size(); ++j)
                file << current_text_[i][j];
        file.close();
    }

    // далее идет длинное число если (если успею сделать)
}