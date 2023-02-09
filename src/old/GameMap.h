#pragma once
#include "GameEntity.h"
#include "GameObject.h"



#include "src/engine/EngineCore.h"
typedef class GAMEMAP_t
{
public:
    GAMEMAP_t(){}



    GAMEMAP_t(int offx, int offy, int w, int h, const char* map_str) :
        _offx(offx),
        _offy(offy),
        _width(w),
        _height(h),
        _raw_map(map_str),
        _anchors((POINT2D*)calloc(32, sizeof(POINT2D)))
    {
        for (int i = 0; i < 32; i++)
            _anchors[i] = { 0,0 };
       /* lazyInit();*/
    }
    void PreRender() const
    {

    }
    void Render() const
    {



    }
    void Update()
    {



    }


    pIGameObject SpawnPlayer()
    {
        pIGameObject o = new _Player(_anchors[0].X, _anchors[0].Y, 'O', GO_PLAYER, "Player"); // white
        o->SetColor(White);
        return o;

    }

    IGameObject** SpawnGhosts()
    {
        char c = 'O';

        EntityGameObject** ghosts = new EntityGameObject * [4];
        ghosts[0] = new EntityGameObject(_anchors[1].X - 1, _anchors[1].Y, c, GO_ENEMY);  // lightgreen
        ghosts[1] = new EntityGameObject(_anchors[1].X - 2, _anchors[1].Y, c, GO_ENEMY);  // white
        ghosts[2] = new EntityGameObject(_anchors[1].X + 1, _anchors[1].Y, c, GO_ENEMY);  // lightred
        ghosts[3] = new EntityGameObject(_anchors[1].X, _anchors[1].Y, c, GO_ENEMY);      // lightaqua

        ghosts[0]->SetColor(LightGreen);
        ghosts[1]->SetColor(White);
        ghosts[2]->SetColor(LightRed);
        ghosts[3]->SetColor(LightAqua);

        ghosts[0]->ChangeDirection(EntityGameObject::eDirection::LEFT);
        ghosts[1]->ChangeDirection(EntityGameObject::eDirection::LEFT);
        ghosts[2]->ChangeDirection(EntityGameObject::eDirection::RIGHT);
    	ghosts[3]->ChangeDirection(EntityGameObject::eDirection::RIGHT);

        return (IGameObject**)ghosts;

    }
    IGameObject** SpawnTiles()
    {
        IGameObject** tiles = new IGameObject * [_width * _height];
        for (int yy = 0; yy < _height; yy++)
        {
            for (int xx = 0; xx < _width; xx++)
            {
                const int index = xx + yy * _width;
                const char c = _raw_map[index];
                tiles[index] = new StaticGameObject(_offx + xx, _offy + yy, c, 0);
                switch (c)
                {
                case '*':
                    tiles[index] = new StaticGameObject(_offx + xx, _offy + yy, '#', GO_SOLID); // blue
                    tiles[index]->SetColor(Blue);
                    break;
                case 'S':
                    _anchors[0] = { Int(_offx + xx),Int(_offy + yy) };
                    break;
                case '.':
                    tiles[index] = new StaticGameObject(_offx + xx, _offy + yy, '.', GO_AUTOREDRAW | GO_DISPOSABLE);
                    tiles[index]->SetColor(Yellow);
                    break;
                case 'F':
                    tiles[index] = new StaticGameObject(_offx + xx, _offy + yy, 'o', GO_AUTOREDRAW | GO_DISPOSABLE | GO_SUPER);
                    tiles[index]->SetColor(Yellow);
                    break;
                case 'P':
                    if(_anchors[2] == 0)
                    {
                        _anchors[2] = { Int(_offx + xx), Int(_offy + yy )};
                        tiles[index] = new StaticGameObject(_offx + xx, _offy + yy, '<', GO_AUTOREDRAW);

                        tiles[index]->SetColor(Yellow);

                    }else
                    {
                        _anchors[3] = { Int(_offx + xx), Int(_offy + yy) };
                        tiles[index] = new StaticGameObject(_offx + xx, _offy + yy, '>', GO_AUTOREDRAW);
                        tiles[index]->SetColor(Yellow);
                    }
                    break;
                case 'G':
                    _anchors[1] = { Int(_offx + xx),Int(_offy + yy - 1) };
                    break;
                case 'A':
                    tiles[index] = new StaticGameObject(_offx + xx, _offy + yy, '!', GO_SOLID);
                    //tiles[index]->SetColor(Yellow);
                    if(_anchors[26] == 0)
						_anchors[26] = { Int(_offx + xx),Int(_offy + yy) };
                    else if (_anchors[27] == 0)
                        _anchors[27] = { Int(_offx + xx),Int(_offy + yy) };
                    else if (_anchors[28] == 0)
                        _anchors[28] = { Int(_offx + xx),Int(_offy + yy) };
                    else if (_anchors[29] == 0)
                        _anchors[29] = { Int(_offx + xx),Int(_offy + yy) };
                    else if (_anchors[30] == 0)
                        _anchors[30] = { Int(_offx + xx),Int(_offy + yy) };
                    else if (_anchors[31] == 0)
                        _anchors[31] = { Int(_offx + xx),Int(_offy + yy) };
                    break;
                    default:
                    //tiles[index] = new StaticGameObject(_offx + xx, _offy + yy, '?', GO_SOLID);
                    break;
                }


                //_tiles[index] = Tile(xx,yy)
            }

        }
        return tiles;
    }
    int GetWidth() const { return _width; }
    int GetHeight() const { return _height; }

    POINT2D* _anchors;
    int Size() { return _width * _height; }
    int _offx;
    int _offy;
private:

    int _width;
    int _height;
    const char* _raw_map;






}GameMap;
