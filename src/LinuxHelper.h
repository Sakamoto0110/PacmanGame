#pragma once
#ifdef LINUX



#include <stdio.h>
#include <stdlib.h>
#include <termios.h>

void gotoxy(int x, int y) {
    printf("%c[%d;%df", 0x1B, y, x);
}
void clrscr() {
    system("clear");
}

void PrintAtEx(char c, int& oldx, int& oldy, int x, int y)
{
    _gotoxy(oldx, oldy);
    printf(" ");
    _gotoxy(x, y);
    printf("%c", c);
    oldx = x;
    oldy = y;
}

void PrintAtEx2(char c, int& oldx, int& oldy, int x, int y, const char* color)
{
    _gotoxy(oldx, oldy);
    printf(" ");
    _gotoxy(x, y);
    printf("%s%c%s", color, c, DFT);
    oldx = x;
    oldy = y;
}

void PrintAt(char c, int x, int y)
{
    _gotoxy(x, y);
    printf("%c", c);
}

void PrintAt2(char c, int x, int y, const char* color)
{
    _gotoxy(x, y);
    printf("%s%c%s", color, c, DFT);
}

void PrintAtstr(const char* str, int x, int y)
{
    _gotoxy(x, y);
    printf("%s", str);
}


void ClearAt(int x, int y, int size)
{
    _gotoxy(x, y);
    for (int i = 0; i < size; i++)
    {
        printf(" ");
    }
}



static struct termios _old, _new;

/* Initialize new terminal i/o settings */
void initTermios(int echo)
{
    tcgetattr(0, &_old); //grab old terminal i/o settings
    _new = _old; //make new settings same as old settings
    _new.c_lflag &= ~ICANON; //disable buffered i/o
    _new.c_lflag &= echo ? ECHO : ~ECHO; //set echo mode
    tcsetattr(0, TCSANOW, &_new); //apply terminal io settings
}

/* Restore old terminal i/o settings */
void resetTermios(void)
{
    tcsetattr(0, TCSANOW, &_old);
}

/* Read 1 character - echo defines echo mode */
char getch_(int echo)
{
    char ch;
    initTermios(echo);
    ch = getchar();
    resetTermios();
    return ch;
}

/*
Read 1 character without echo
getch() function definition.
*/
char getch(void)
{
    return getch_(0);
}

/*
Read 1 character with echo
getche() function definition.
*/
char getche(void)
{
    return getch_(1);
}

#include <sys/ioctl.h>
void enable_raw_mode()
{
    termios term;
    tcgetattr(0, &term);
    term.c_lflag &= ~(ICANON | ECHO); // Disable echo as well
    tcsetattr(0, TCSANOW, &term);
}

void disable_raw_mode()
{
    termios term;
    tcgetattr(0, &term);
    term.c_lflag |= ICANON | ECHO;
    tcsetattr(0, TCSANOW, &term);
}

bool kbhit()
{
    int byteswaiting;
    ioctl(0, FIONREAD, &byteswaiting);
    return byteswaiting > 0;
}

#endif