#pragma once


//#define RED "\033[31m"
//#define GREEN "\033[32m"
//#define BLUE "\033[34m"
//#define CYAN "\033[36m"
//#define ORANGE "\033[33m"
//#define LGRAY "\033[37m"
//#define DFT "\033[39m"
//
//
//inline void PrintAtEx(char c, int& oldx, int& oldy, int x, int y)
//{
//    _gotoxy(oldx, oldy);
//    printf(" ");
//    _gotoxy(x, y);
//    printf("%c", c);
//    oldx = x;
//    oldy = y;
//}
//
//inline void PrintAtEx2(char c, int& oldx, int& oldy, int x, int y, const char* color)
//{
//    _gotoxy(oldx, oldy);
//    printf(" ");
//    _gotoxy(x, y);
//    printf("%s%c%s", color, c , DFT);
//    oldx = x;
//    oldy = y;
//}
//
//inline void PrintAt(char c, int x, int y)
//{
//    _gotoxy(x, y);
//    printf("%c", c);
//}
//
//inline void PrintAt2(char c, int x, int y, const char* color)
//{
//    _gotoxy(x, y);
//    printf("%s%c%s", color, c , DFT);
//}
//
//inline void PrintAtstr(const char* str, int x, int y)
//{
//    _gotoxy(x, y);
//    printf("%s", str);
//}
//
//
//inline void ClearAt(int x, int y, int size)
//{
//    _gotoxy(x, y);
//    for(int i = 0; i < size; i++)
//    {
//        printf(" ");
//    }
//}
//



typedef struct RGB_t
{
    int _r, _g, _b;
    RGB_t() : _r(0), _g(0), _b(0) {}
    RGB_t(int unique_val) : _r(unique_val), _g(unique_val), _b(unique_val) {}
    RGB_t(const int& r, const int& g, const int& b) : _r(r), _g(g), _b(b) {}
    RGB_t(int vals[3]) : _r(vals[0]), _g(vals[1]), _b(vals[2]) {}

    constexpr RGB_t& GetConst() { return *this; }

    bool operator ==(const RGB_t& other) const
    {
        return _r == other._r && _g == other._g && _b == other._b;
    }

    bool operator !=(const RGB_t& other) const
    {
        return !(*this == other);
    }
    void print(bool newLine = false) const
    {
        printf("[R: %i, G: %i, B: %i]%c", _r, _g, _b, newLine ? '\n' : ' ');
    }


}MyRGB;

typedef double Int;

struct POINT2D
{
    Int X;
    Int Y;
    POINT2D(Int x, Int y) :X(x), Y(y) {}
    POINT2D(Int u) :X(u), Y(u) {}
    POINT2D() :X(0), Y(0) {}
    POINT2D(const POINT2D& other) : X(other.X), Y(other.Y) {}

    bool operator ==(const POINT2D& other) const { return other.X == X && other.Y == Y; }
    bool operator ==(const Int v) const { return X == v && Y == v; }

    bool operator !=(const POINT2D& other) const { return !(other == *this); }
    bool operator !=(const Int v) const { return !((*this) == v); }

    bool EqualsINT(const POINT2D other) const { return (int)other.X == (int)X && (int)other.Y == (int)Y; }
    bool EqualsINT(const int v) const { return (int)X == (int)v && (int)Y == (int)v; }

    POINT2D operator+(const POINT2D& other) const { return POINT2D(X + other.X, Y + other.Y); }
    POINT2D operator+(const Int v) const { return POINT2D(X + v, Y + v); }
    POINT2D operator-(const Int v) const { return POINT2D(X - v, Y - v); }
    POINT2D operator-(const POINT2D& other)const { return POINT2D(X - other.X, Y - other.Y); }

    POINT2D& operator+=(const POINT2D& other) { X += other.X; Y += other.Y; return *this; }
    POINT2D& operator-=(const POINT2D& other) { X -= other.X; Y -= other.Y; return *this; }
    POINT2D& operator-=(const Int v) { X -= v; Y -= v; return *this; }
    POINT2D& operator+=(const Int v) { X += v; Y += v; return *this; }

};


constexpr int GOBJ_UNDEFINED = -1;
constexpr int GOBJ_NULL = 0;
constexpr int GOBJ_STATIC = 1;
constexpr int GOBJ_ENTITY = 2;

constexpr int GOBJ_CONSUMABLE = 2;
constexpr int GOBJ_DYNAMIC = 4;
constexpr int GOBJ_ACTIVE = 8;
constexpr int GOBJ_PLAYER = 16;

constexpr int GO_SOLID = 1;
constexpr int GO_AUTOREDRAW = 2;
constexpr int GO_PLAYER = 4;
constexpr int GO_ENEMY = 8;
constexpr int GO_DISPOSABLE = 16;
constexpr int GO_SUPER = 32;

template<typename T>
struct DataArray
{
    T* Tdata;
    int MaxCount;
private:

    int _ptr;
public:
    int Count() const { return _ptr+1; }
    DataArray(int size = 0)
    {
        Tdata = new T[size];
        MaxCount = size;
        _ptr = -1;
    }

    DataArray(DataArray<T>& other)
    {
        MaxCount = other.MaxCount;
        Tdata = new T[MaxCount];
        for(int i = 0; i < MaxCount; i++)
        {
            Tdata[i] = other.Tdata[i];
        }
    }


    void Add(T& t)
    {
        Tdata[++_ptr] = t;
    }
    void Add2(T* t)
    {
        Tdata[++_ptr] = *t;
    }

    T& operator[](int index) {
        return Tdata[index];
    }
    operator T* () {
        return begin();
    }

    T* begin() { return Tdata; }
    T* end() {   return Tdata + _ptr; }
};

struct PixelMap
{
    int width;
    int height;
    MyRGB* pixel;

    PixelMap(int w, int h)
    {
        width = w;
        height = h;
        pixel = new MyRGB[width * height];
    }

    PixelMap(const PixelMap& other) {
	    width = other.width;
	    height = other.height;
	    pixel = new MyRGB[width * height];
	    for (int i = 0; i < width * height; i++)
		    pixel[i] = other[i];
    }


    ~PixelMap()
    {
        delete[] pixel;
    }

    MyRGB& operator[](int index) const
    {
        return pixel[index];
    }

    MyRGB* begin() { return &(*this)[0]; }
    MyRGB* end() { return &(*this)[width * height - 1]; }
    friend PixelMap PixelMapFromFile(const char*);
};


typedef char(*funcRGBtoCHAR)(MyRGB&);
inline bool IsRGBMapValid(const PixelMap& rgbMeta)
{
    MyRGB white(255, 255, 255);
    for (int y = 0; y < rgbMeta.height; y++)
        for (int x = 0; x < rgbMeta.width; x++)
            if (x == 0 || x == rgbMeta.width - 1 && y == 0 || y == rgbMeta.height - 1)
                if (rgbMeta[x + y * rgbMeta.width] != white)
                    return false;
    // * * *
    return true;
}
inline const char* MakeMapString(PixelMap rgbMeta, funcRGBtoCHAR mapFunc = NULL)
{
    if (mapFunc == NULL)
        mapFunc = [](auto) {return ' '; };
    int size = rgbMeta.width * rgbMeta.height;
    char* map_str = new char[size];
    int index = 0;
    for (MyRGB& rgb : rgbMeta)
    {
        map_str[index++] = mapFunc(rgb);

    }

    return map_str;
}
inline char RGBtoCHAR(MyRGB& rgb)
{
    const MyRGB white(255, 255, 255);
    const MyRGB red(255, 0, 0);
    const MyRGB green(0, 255, 0);
    const MyRGB blue(0, 0, 255);
    const MyRGB yellow(255, 255, 0);
    const MyRGB pink(255, 0, 255);
    const MyRGB black(0, 0, 0);
    const MyRGB orange(255, 125, 0);
    const MyRGB cyan(0, 255, 255);

    if (rgb == white)
    {
        return ' ';
    }
    if (rgb == blue)
    {
        return '*';
    }
    if (rgb == green)
    {
        return 'S';
    }
    if (rgb == yellow)
    {
        return '.';
    }
    if (rgb == pink)
    {
        return 'P';
    }
    if (rgb == red)
    {
        return 'G';
    }
    if (rgb == orange)
    {
        return 'F';
    }
    if (rgb == cyan)
    {
        return 'A';
    }
    if (rgb == black)
    {
        return ' ';
    }
    return ' ';
}






template<typename T>
struct DataBlob
{
private:
    T* _begin;
    T* _end;
    T* _ptr;
public:
    DataBlob() {}

    DataBlob(T* b, T* e)
    {
        _begin = b;
        _end = e;
        _ptr = _begin;

    }

    DataBlob(T* b, int size)
    {
        _begin = b;
        _ptr = b;
        _end = _begin + size;
    }

	T* begin()
    {
        return _begin;
    }
    T* end()
    {
        return _end;
    }

    int Count() { return _end - _begin; }

    T& operator[](int index)
    {
        return *(_ptr = _begin + index);
    }

    operator T& () {
        return *_ptr;
    }


};
