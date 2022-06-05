#include "View.h"

View::View()
{ }
View::View(char name)
{
    String<char> noth("noth");
    if (!glfwInit())
        exit(-1);
    str_begin = 0;
    ind_begin = 0;
    point = false;
	window_ = glfwCreateWindow(640, 480, "Vim Leontiev E.P. production", NULL, NULL);
}
void View::DrawLine(GLfloat x1, GLfloat x2, GLfloat y1, GLfloat y2)
{
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();
}
void View::DrawText(String<char> text, int length, int x, int y)
{
    glMatrixMode(GL_PROJECTION);
    double* matrix = new double[16];
    glGetDoublev(GL_PROJECTION_MATRIX, matrix);
    glLoadIdentity();
    glOrtho(0, 400, 0, 400, -5, 5);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
    glLoadIdentity();
    glRasterPos2i(x, y);
    for (int i = 0; i < length; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (int)text[i]);
    }
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(matrix);
    glMatrixMode(GL_MODELVIEW);
    delete[] matrix;
}
char* View::IntToMyString(int input)
{
    String<char> text;
    char* txt = new char[1 * sizeof(char)];
    int i = 0;
    do
    {
        txt[i] = '0'+input % 10;
        input /= 10;
        i++;
    } while (input > 0);
    txt[i] = '\0';
    return txt;
    
}
void View::DrawBytes()
{
    String<char> text;
    text.append("00");
    DrawText(text, text.length(), 35, 365);
    text.clear();
    text.append("01");
    DrawText(text, text.length(), 50, 365);
    text.clear();
    text.append("02");
    DrawText(text, text.length(), 65, 365);
    text.clear();
    text.append("03");
    DrawText(text, text.length(), 80, 365);
    text.clear();
    text.append("04");
    DrawText(text, text.length(), 95, 365);
    text.clear();
    text.append("05");
    DrawText(text, text.length(), 110, 365);
    text.clear();
    text.append("06");
    DrawText(text, text.length(), 125, 365);
    text.clear();
    text.append("07");
    DrawText(text, text.length(), 140, 365);
    text.clear();
    text.append("08");
    DrawText(text, text.length(), 155, 365);
    text.clear();
    text.append("09");
    DrawText(text, text.length(), 170, 365);
    text.clear();
    text.append("0a");
    DrawText(text, text.length(), 185, 365);
    text.clear();
    text.append("0b");
    DrawText(text, text.length(), 200, 365);
    text.clear();
    text.append("0c");
    DrawText(text, text.length(), 215, 365);
    text.clear();
    text.append("0d");
    DrawText(text, text.length(), 230, 365);
    text.clear();
    text.append("0e");
    DrawText(text, text.length(), 245, 365);
    text.clear();
    text.append("0f");
    DrawText(text, text.length(), 260, 365);
    text.clear();
}
void View::DrawHexCode(char sym, int x, int y)
{
    char hex[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
    String<char> code;
    code.append(1, hex[(sym & 0xF0) >> 4]);
    code.append(1, hex[sym & 0xF]);
    DrawText(code, code.length(), x, y);
    code.clear();
}
void View::DrawIndex(int row, int x, int y)
{
    String<char> code;
    do 
    {
        int r(row % 16);
        if (r > 9) 
            r += (int)'A' - 10; 
        else 
            r += (int)'0'; 
        code.insert(0, 1, (char)r);
        row /= 16;
    } while (row);
    if (code.length() == 1)
        code.insert(0, 3, '0');
    else if(code.length() == 2)
        code.insert(0, 2, '0');
    else if (code.length() == 3)
        code.insert(0, 1, '0');
    DrawText(code, code.length(), x, y);
}
void View::StartForHexMode(int begin_row, int end_row, std::vector<String<char>> current_text)
{
    str_begin = 0;
    ind_begin = 0;
    point = false;
    int row = 0;
    if (begin_row == row)
        return;
    int column = 1;
    for (int i = 0; i < current_text.size(); ++i) 
    {
        for (int j = 0; j < current_text[i].size(); ++j)
        {
            if (current_text[i][j] == '\n')
            {
                if (column == 16)
                {
                    if (row+1 == begin_row)
                    {
                        str_begin = i + 1;
                        ind_begin = 0;
                        point = true;
                        return;
                    }
                   // ++column;
                }
                else
                {
                    ++column;
                    if (column == 16)
                    {
                        if (row == begin_row)
                        {
                            str_begin = i+1;
                            ind_begin = 0;
                            return;
                        }
                    }
                }
            }
            if (column == 16)
            {
                column = 1;
                ++row;
                if (row == begin_row)
                {
                    str_begin = i;
                    ind_begin = j+1;
                    return;
                }
            }
            else
                ++column;  
        }
    }
    str_begin = 999999;
}
void View::StartForNormMode(int begin_row, std::vector<String<char>> current_text)
{
    str_begin = 0;
    ind_begin = 0;
    int row = 0;
    if (begin_row == row)
        return;
    int column = 1;
    for (int i = 0; i < current_text.size(); ++i)
    {
        for (int j = 0; j < current_text[i].size(); ++j)
        {
            if (current_text[i][j] == '\n')
            {
                ++row;
                column = 0;
                ++column;
                if (row == begin_row)
                {
                    str_begin = i + 1;
                    ind_begin = 0;
                    return;
                }
            }
            else if (column == 35)
            {
                ++row;
                column = 0;
                ++column;
                if (row == begin_row)
                {
                    str_begin = i;
                    ind_begin = j + 1;
                    return;
                }
            }
            else
                ++column;
        }
    }
    str_begin = 999999;
}
void View::DrawMainScreen(char mode, String<char> command_text, String<char> cuurent_file_name_text, String<char> current_mode_text, 
    std::vector<String<char>> current_text, float cursor_x, float cursor_y, int row, int column, int begin_row, int end_row, bool help)
{
    glfwMakeContextCurrent(window_);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_QUADS);
    glVertex2f(-1.0f, 0.9f);
    glVertex2f(0.4f, 0.9f);
    glVertex2f(0.4f, 1.0f);
    glVertex2f(-1.0f, 1.0f);
    glEnd();
    glColor3f(1.0f, 1.0f, 1.0f);
    DrawLine(0.4f, 0.4f, 1.0f, -1.0f); 
    DrawLine(-1.0f, 1.0f, 0.8f, 0.8f);
    DrawLine(-1.0f, 1.0f, 0.9f, 0.9f);
    
    String<char> text;
    if (help) // вывод информации по командам в текущем режиме
    {
        text.clear();
        if (mode == 'c')
        {
            text.append("o x.txt-open x.txt");
            DrawText(text, text.length(), 283, 349);
            text.clear();
            text.append("x-save and exit");
            DrawText(text, text.length(), 283, 334);
            text.clear();
            text.append("w-save");
            DrawText(text, text.length(), 283, 319);
            text.clear();
            text.append("q-exit(not changed)");
            DrawText(text, text.length(), 283, 304);
            text.clear();
            text.append("q!-exit(without save)");
            DrawText(text, text.length(), 283, 289);
            text.clear();
            text.append("0-999-move to string");
            DrawText(text, text.length(), 283, 274);
            text.clear();
            text.append("h-help");
            DrawText(text, text.length(), 283, 259);
            text.clear();
            text.append("Esc-=>normal");
            DrawText(text, text.length(), 283, 244);
            text.clear();
            text.append("w x.txt-save in x.txt");
            DrawText(text, text.length(), 283, 229);
            text.clear();
        }
        else if (mode == 'n')
        {
            text.append("i-=>input");
            DrawText(text, text.length(), 283, 349);
            text.clear();
            text.append("I-move to beg=>input");
            DrawText(text, text.length(), 283, 334);
            text.clear();
            text.append("A-move to end=>input");
            DrawText(text, text.length(), 283, 319);
            text.clear();
            text.append(":-=>command");
            DrawText(text, text.length(), 283, 304);
            text.clear();
            text.append("^-move to begin str");
            DrawText(text, text.length(), 283, 289);
            text.clear();
            text.append("$-move to end str");
            DrawText(text, text.length(), 283, 274);
            text.clear();
            text.append("w-move to end word");
            DrawText(text, text.length(), 283, 259);
            text.clear();
            text.append("b-move to begin word");
            DrawText(text, text.length(), 283, 244);
            text.clear();
            text.append("gg-move to begin file");
            DrawText(text, text.length(), 283, 229);
            text.clear();
            text.append("G-move to end file");
            DrawText(text, text.length(), 283, 214);
            text.clear();
            text.append("NG-move to N");
            DrawText(text, text.length(), 283, 199);
            text.clear();
            text.append("dd-cut str");
            DrawText(text, text.length(), 283, 184);
            text.clear();
            text.append("y-copy str");
            DrawText(text, text.length(), 283, 169);
            text.clear();
            text.append("p-paste");
            DrawText(text, text.length(), 283, 154);
            text.clear();
            text.append("S-del str=>input");
            DrawText(text, text.length(), 283, 139);
            text.clear();
            text.append("/text-search");
            DrawText(text, text.length(), 283, 124);
            text.clear();
            text.append("?text-reverse search");
            DrawText(text, text.length(), 283, 109);
            text.clear();
            text.append("diw-del word");
            DrawText(text, text.length(), 283, 94);
            text.clear();
            text.append("yw-copy word");
            DrawText(text, text.length(), 283, 79);
            text.clear();
            text.append("h-help");
            DrawText(text, text.length(), 283, 64);
            text.clear();
        }
    }
    DrawText(command_text, command_text.length(), 0, 385); // отрисовка текста командной строки
    
    if (current_mode_text.size() > 0)
        DrawText(current_mode_text, current_mode_text.length(), 283, 385); // отрисовка текста текущего мода
    if (mode != 'h')
    {
        text.append("Current file: ");
        DrawText(text, text.length(), 0, 365); 
        if (cuurent_file_name_text.size() > 0)
            DrawText(cuurent_file_name_text, cuurent_file_name_text.length(), 80, 365); // отрисовка текста текущего файла
    }
    else
    {
        DrawBytes();
        DrawLine(-0.86f, -0.86f, 0.9f, -1.0f);
    } 
    String<char> symbol;
    int startX = 0;
    int startY = 349;
    int x = startX;
    int y = startY;
    if (mode != 'h')
    {
        StartForNormMode(begin_row, current_text);
        for (int i = str_begin; i < current_text.size(); ++i) // вывод по буковке
        {
            for (int j = ind_begin; j < current_text[i].size(); ++j)
            {
                if (current_text[i][j] == '\n')
                    break;
                symbol.append(1, current_text[i][j]);
                DrawText(symbol, 1, x, y);
                symbol.clear();
                x += 8;
                if (x > 275)
                {
                    x = startX;
                    y -= 15;
                }
            }
            ind_begin = 0;
            x = startX;
            y -= 15;
        }
        str_begin = 0;
    }
    else
    {
        StartForHexMode(begin_row, end_row, current_text);
        startX = 285;
        x = startX;
        if (point)
        {
            symbol.append(1, '.');
            DrawText(symbol, 1, x, y);
            x += 7;
        }
        symbol.clear();
        for (int i = str_begin; i < current_text.size(); ++i) // вывод по буковке
        {
            for (int j = ind_begin; j < current_text[i].size(); ++j)
            {
                if (current_text[i][j] == '\n')
                {
                    symbol.append(1, '.'); 
                    DrawText(symbol, 1, x, y);
                    x += 7;
                    if (x > 390)
                    {
                        x = startX;
                        y -= 15;
                    }
                }
                else
                    symbol.append(1, current_text[i][j]);
                DrawText(symbol, 1, x, y);
                symbol.clear();
                x += 7;
                if (x > 390)
                {
                    x = startX;
                    y -= 15;
                }
            }
            ind_begin = 0;
        }
        str_begin = 0;
    }
    if (current_text.size() > 0 && mode!='h')
    {
        text.clear();
        text.append("Column: ");
        DrawText(text, text.length(), 150, 365);
        text.clear();
        char* clmn = IntToMyString(column);
        for (int j = std::strlen(clmn)-1; j >= 0; --j)
        {
            text.append(1, clmn[j]);
        }
        DrawText(text, text.length(), 193, 365);
        text.clear();
        text.append("Row: ");
        DrawText(text, text.length(), 220, 365);
        text.clear();
        char* rw = IntToMyString(row);
        for (int j = std::strlen(rw) - 1; j >= 0; --j)
        {
            text.append(1, rw[j]);
        }
        DrawText(text, text.length(), 245, 365);


        glLineWidth(2.0f);
        glColor3f(1.0f, 0.2f, 0.2f);
        glBegin(GL_LINES);
        glVertex2f(cursor_x, cursor_y - 0.04f);
        glVertex2f(cursor_x, cursor_y + 0.04f);
        glEnd();
    }
    else if(mode == 'h')// если хекс мод
    {
        StartForHexMode(begin_row, end_row, current_text);
        symbol.clear();
        startX = 35;
        startY = 349;
        x = startX;
        y = startY;
        if (point)
        {
            symbol.append("0a");
            DrawText(symbol, 2, x, y);
            x += 15;
        }
        symbol.clear();
        for (int i = str_begin; i < current_text.size(); ++i) // вывод по буковке
        {
            for (int j = ind_begin; j < current_text[i].size(); ++j)
            {
                if (current_text[i][j] == '\n')
                {
                    symbol.append("0d");
                    DrawText(symbol, 2, x, y);
                    x += 15;
                    if (x > 260)
                    {
                        x = startX;
                        y -= 15;
                    }
                    symbol.clear();
                    symbol.append("0a");
                    DrawText(symbol, 2, x, y);
                    symbol.clear();
                }
                else
                    DrawHexCode(current_text[i][j], x, y);
                x += 15;
                if (x > 260)
                {
                    x = startX;
                    y -= 15;
                }
            }
            ind_begin = 0;
        }
        str_begin = 0;
        startX = 2;
        startY = 349;
        x = startX;
        y = startY;
        for (int i = begin_row; i < end_row; ++i)
        {
            DrawIndex(i*16, x, y);
            y -= 15;
        }
        glLineWidth(2.0f);
        glColor3f(1.0f, 0.2f, 0.2f);
        glBegin(GL_LINES);
        glVertex2f(cursor_x - 0.03f, cursor_y);
        glVertex2f(cursor_x + 0.03f, cursor_y);
        glEnd();
    }
    glfwSwapBuffers(window_);
    glfwPollEvents();
    
}