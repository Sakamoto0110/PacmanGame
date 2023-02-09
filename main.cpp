//#include <stdio.h>
//#define FLAGS_NO_TEMPLATES
//#include "src/misc/flags.h"
//
//
//
//const flag _v1 = 0b0001;
//const flag _v2 = 0b0010;
//const flag _v3 = 0b0100;
//int main() {
//
//	
//	flag v = 0;
//	
//	
//	printf("base values: %i\n",v);
//	fadd(_v3, v);
//	//v = fadd<_v3>(v);
//	//fadd(v1, v3);
//	/*if(fchk<_v3>(v)) {
//		printf("exists\n");
//	}*/
//	if (fchk(_v3,v)) {
//		printf("exists\n");
//	}
//	printf("add values: %i\n", v);
//	v =	frem(_v3,v);
//	if (!fchk(_v3, v)) {
//		printf("not exists\n");
//	}
//	printf("del values: %i\n", v);
//	return 0;
//}
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include "LinuxHelper.h"

#include <Windows.h>
#include <conio.h>
#define WINDOWS
inline void gotoxy(SHORT x, SHORT y) {
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { x, y });
}
#include "src/engine/EngineCore.h"
inline void PrintAtEx(char c, int& oldx, int& oldy, int x, int y)
{
    gotoxy(oldx, oldy);
    printf(" ");
    gotoxy(x, y);
    printf("%c", c);
    oldx = x;
    oldy = y;
}

inline void PrintAtEx2(char c, int& oldx, int& oldy, int x, int y, const int color)
{
    gotoxy(oldx, oldy);
    printf(" ");
    gotoxy(x, y);
    //print("", color);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
    printf("%c", c);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), White);
    //print("", color_white);
    //printf("%s%c%s", color, c , color_white);
    oldx = x;
    oldy = y;
}

inline void PrintAt(char c, int x, int y)
{
    gotoxy(x, y);
    printf("%c", c);
}


inline void PrintAt2(char c, int x, int y, const int color)
{
    gotoxy(x, y);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
    printf("%c", c);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), White);
    //printf("%s%c%s", color, c , DFT);
    //print("", color);
    //print("", color_white);
}

inline void PrintAtstr(const char* str, int x, int y)
{
    gotoxy(x, y);
    printf("%s", str);
}


inline void ClearAt(int x, int y, int size)
{
    gotoxy(x, y);
    for (int i = 0; i < size; i++)
    {
        printf(" ");
    }
}
inline void ShowConsoleCursor(bool showFlag)
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO     cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
}


#define printAtEx(c,oldx,oldy,x,y)        PrintAtEx(c,(int&)(oldx),(int&)(oldy),(int)(x),(int)(y))
#define printAtEx2(c,oldx,oldy,x,y,color) PrintAtEx2(c,(int&)(oldx),(int&)(oldy),(int)(x),(int)(y),(color))
#define printAtf(c,x,y)                   PrintAt(c,x,y)
#define printAts(str,x,y)                 PrintAtstr(str,x,y)
#define printAt(c,x,y)                    PrintAt(c,x,y)
#define printAt2(c,x,y,color)             PrintAt2(c,x,y,color)
#define clearAts(x,y,s)                   ClearAt(x,y,s)


#define cimg_display 0

#include "src/external/CImg/CImg.h"
//#include "CImg/CImg.h"
//#include "CImg/CImg.h"

#include "src/old/Events.h"


#define WINDOWS
#define WIN32
#include "src/engine/EngineCore.h"
#include "src/old/GameBase.h"
#include "src/old/GameMap.h"
#include "src/old/Game.h"
#include "src/old/GameObject.h"
#include "src/old/GameEntity.h"
#define STOPED -1
#define RUNNING 1
#define PAUSED 0

#define FPS 15

using namespace cimg_library;



#ifdef LINUX
#define YES_LINUX
#endif

#ifdef WINDOWS
#define YES_WIN32
#endif

#ifdef WIN32
#define YES_WIN32
#endif


PixelMap PixelMapFromFile(const char* filename)
{


    CImg<int> src = cimg_library::CImg<int>(filename);
    PixelMap pixel(src.width(), src.height());
    for (int y = 0; y < src.height(); y++)
        for (int x = 0; x < src.width(); x++)
            pixel[x + y * src.width()] = MyRGB({ src[src.offset(x,y,0,0)],src[src.offset(x,y,0,1)],src[src.offset(x,y,0,2)] });

    return pixel;
}

GAMEMAP_t* LoadMap(int offsetX, int offsetY, const PixelMap& pixelMap)
{
    if (!IsRGBMapValid(pixelMap))
    {
        printf("Invalid map format.\n");
        system("pause");
        return 0;
    }
    const char* map_str = MakeMapString(pixelMap, RGBtoCHAR);
    return (new GameMap(offsetX, offsetY, pixelMap.width, pixelMap.height, map_str));
}

const char* MAP_STR = "????????????????????????*********************??*F.......F*F.......F*??*.***.***.*.***.***.*??*.***.***.*.***.***.*??*.***.***.*.***.***.*??*...................*??*.***.*.*****.*.***.*??*.***.*.*****.*.***.*??*F....*...*...*....F*??*****.***.*.***.*****??????*.*.......*.*??????????*.*.**G**.*.*??????*****.*.*   *.*.*****??P....F..*   *..F....P??*****.*.*****.*.*****??????*.*.......*.*??????????*.*.*****.*.*??????*****.*.*****.*.*****??*F........*........F*??*.***.***.*.***.***.*??*...*.....S.....*...*??***.*.*.*****.*.*.***??***.*.*.*****.*.*.***??*.....*...*...*.....*??*.*******.*.*******.*??*F.................F*??*********************???????????????????????? ";
#include <cstring>
int main() {
    ShowConsoleCursor(false);
    //  CImg<int> src = CImg<int>(500,400,1,3,0);
    //clrscr();


    // Game Initialize
    Game* game = new Game(1,1,23,29,MAP_STR);
    //Game* game = new Game(*LoadMap(0, 0, PixelMapFromFile("pacmanFull.png")));
    //game->Initialize();
    //Game* game = new Game(*LoadMap(0, 0, PixelMapFromFile("res/pacmanFull.jpeg")));
    game->Initialize();
    //



    clock_t stime = clock();
    int state = RUNNING;
#ifdef YES_LINUX
    enable_raw_mode();
#endif
    fflush(stdout);
    game->PreRender();
    game->Render();
    while (state != STOPED)
    {
        int time = (clock() - stime) / (CLOCKS_PER_SEC / FPS);
        if (time != stime)
        {
            stime = time;
            if (state == PAUSED) {
                continue;
            }
            game->Run();
           


            fflush(stdout);
        }
        if (_kbhit()) {
            char key = _getch();
            if (key == 'k') {
                state = STOPED;
                //clrscr();
            }
            else {
                game->KeyPressed(key);
            }
#ifdef YES_LINUX
            tcflush(0, TCIFLUSH);
#endif
            
        }
    }
#ifdef YES_LINUX
    disable_raw_mode();
#endif
    
    printf("\n");



    return 0;
}
