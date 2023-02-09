#pragma once
#include <vector>

#include "GameMap.h"
#include "GameObject.h"


inline void BeforeCollisionHandler(IGameObject* source, const EntityCollisionEventArgs& args)
{

    EntityGameObject* me = (EntityGameObject*)source;

    bool coin = ((rand() % 10) % 2) == 0;
    if (me != 0)
    {
        if (args.horizontal)
        {
            if (coin)
                me->ChangeDirection(EntityGameObject::eDirection::TOP);
            else
                me->ChangeDirection(EntityGameObject::eDirection::DOWN);
        }
        if (args.vertical)
        {
            if (coin)
                me->ChangeDirection(EntityGameObject::eDirection::LEFT);
            else
                me->ChangeDirection(EntityGameObject::eDirection::RIGHT);
        }
    }
}

inline void OnCollisionHandler(IGameObject* source, OnCollisionEventArgs& args)
{
    if (source == args.target)
        return;
    if (source->IsPlayer() && !args.target->IsEnemy())
    {
        args.target->Dispose();

    }
    else if (args.RequestTargetRedraw && !args.target->IsDisposed())
    {
        args.target->RequestRender();
    }


}



class Game
{
public:
    friend void GameOver(Game& game);
	Game()
	{

	}
	Game(const char* mapFile)
	{
		_mapFile = mapFile;
	}
    Game(int offx, int offy, int width, int height, const char* map_str)
    {
        _map = GameMap(offx, offy, width, height, map_str);
    }
    Game(GameMap map)
	{
        _map = map;
	}


    void KeyPressed(char key)
	{
        switch (key)
        {
        case 'q':

            break;
        case 'w':
            ((_Player*)_player)->ChangeDirection2(EntityGameObject::eDirection::TOP);
            break;
        case 'a':
            ((_Player*)_player)->ChangeDirection2(EntityGameObject::eDirection::LEFT);
            break;
        case 's':
            ((_Player*)_player)->ChangeDirection2(EntityGameObject::eDirection::DOWN);
            break;
        case 'd':
            ((_Player*)_player)->ChangeDirection2(EntityGameObject::eDirection::RIGHT);
            break;
        case 'r':
            auto pSpawn = _map._anchors[0];
            auto gSpawn = _map._anchors[1];

            _player->SetPosition(pSpawn.X, pSpawn.Y);
            ((GameObject*)_ghostBlob[0])->_isDisposed = false;
            ((GameObject*)_ghostBlob[1])->_isDisposed = false;
            ((GameObject*)_ghostBlob[2])->_isDisposed = false;
            ((GameObject*)_ghostBlob[3])->_isDisposed = false;
            _ghostBlob[0]->SetPosition(gSpawn.X - 2, gSpawn.Y);
            _ghostBlob[1]->SetPosition(gSpawn.X - 1, gSpawn.Y);
            _ghostBlob[2]->SetPosition(gSpawn.X, gSpawn.Y);
            _ghostBlob[3]->SetPosition(gSpawn.X + 1, gSpawn.Y);
            for (auto o : _tileBlob) {
                ((GameObject*)o)->_isDisposed = false;
                ((GameObject*)o)->RequestRender();
            }
            break;
        }
	}

    void Respawn()
	{

	}

	void Initialize()
	{
		_tileBlob = DataBlob<IGameObject*>(_map.SpawnTiles(),_map.Size());
        if (_map._anchors[1] != 0) {
            _ghostBlob = DataBlob<IGameObject*>(_map.SpawnGhosts(), 4);
            for (auto o : _ghostBlob)
            {
                // o->BindToObjectPool(_tileBlob.begin(), _map.Size());
                ((MoverEntity*)o)->OnCollision += OnCollisionHandler;
                ((MoverEntity*)o)->BeforeCollision += BeforeCollisionHandler;
            }
        }

        _player = _map.SpawnPlayer();
        ((MoverEntity*)_player)->OnCollision += &OnCollisionHandler;
        _player->BindToObjectPool(_tileBlob.begin(), _map.Size());


        int total = _tileBlob.Count() + _ghostBlob.Count() + 1;
        _objArr = DataArray<IGameObject*>(total );

        for (auto obj : _tileBlob)
            _objArr.Add2(&obj);
        for (auto obj : _ghostBlob)
            _objArr.Add2(&obj);
        _objArr.Add2(&_player);


        for (auto o : _ghostBlob)
            o->BindToObjectPool(_objArr.begin(), _objArr.Count());

        _player->BindToObjectPool(_objArr.begin(),_objArr.Count());
	}



    int Run()
	{



        PreRender();
        Update();
        Render();

        return 0;
	}
    void PreRender()
	{
        for (auto o : _tileBlob)
            o->Render();
	}

    void Debug()
	{
        auto pp = _player->GetPosition();
        int index = ((int)pp.X - _map._offx) + ((int)pp.Y - _map._offy) * _map.GetWidth();
        auto str = CheckNearsStr(index, ((_Player*)_player)->GetDirection());
        auto r = CheckNears(index, ((_Player*)_player)->GetDirection());
		printAts("DEBUG: ", 0, 0);
        clearAts(0, 1, 40);
        gotoxy(0, 1);
        printf("Player: %s - %2i [%2i,%2i] [%-.1f,%-.1f]", str, r, (int)pp.X, (int)pp.Y, pp.X, pp.Y);

        for(int i = 0; i < 4; i++)
        {
            auto o = _ghostBlob[i];
            auto pp = o->GetPosition();
        	int index = ((int)pp.X - _map._offx) + ((int)pp.Y - _map._offy) * _map.GetWidth();
            auto str = CheckNearsStr(index, ((EntityGameObject*)o)->GetDirection());
            auto r = CheckNears(index, ((EntityGameObject*)o)->GetDirection());

            clearAts(0, 2+i, 40);
            gotoxy(0, 2+i);
            printf("Ghost %i: %s - %2i [%2i,%2i] [%-.1f,%-.1f]",i+1, str, r, (int)pp.X, (int)pp.Y, pp.X, pp.Y);

        }
	}

	void Update()
	{

        // pass data from map to entity

                auto pp = _player->GetPosition();
        auto index = ((int)pp.X - _map._offx) + ((int)pp.Y - _map._offy) * _map.GetWidth();
        auto r = CheckNears(index, ((_Player*)_player)->GetDirection());
        _player->SetNeighborData((GAMEOBJECT_t*)_tileBlob[index]->IsDisposed()?0: (GAMEOBJECT_t*)_tileBlob[index], r);
        for (auto o : _ghostBlob) {
            auto pp = o->GetPosition();
            auto index = ((int)pp.X - _map._offx) + ((int)pp.Y - _map._offy) * _map.GetWidth();
            auto r = CheckNears(index, ((EntityGameObject*)o)->GetDirection());
            o->SetNeighborData((GAMEOBJECT_t*)_tileBlob[index]->IsDisposed() ? 0 : (GAMEOBJECT_t*)_tileBlob[index], r);
        }
        
        // debug
       // Debug();

		// update positions
        for (auto o : _ghostBlob) {
            o->Update();
        }
		_player->Update();




        // portal handling
        for (auto o : _ghostBlob) {
            auto p = POINT2D((int)o->GetPosition().X, (int)o->GetPosition().Y );
            if(p == _map._anchors[2])
                o->SetPosition(_map._anchors[3].X-1, _map._anchors[3].Y);
            else if (p == _map._anchors[3])
                o->SetPosition(_map._anchors[2].X+1, _map._anchors[2].Y);
        }
        auto p = POINT2D((int)_player->GetPosition().X, (int)_player->GetPosition().Y);
        if (p == _map._anchors[2])
            _player->SetPosition(_map._anchors[3].X-1, _map._anchors[3].Y);
        else if (p == _map._anchors[3])
            _player->SetPosition(_map._anchors[2].X+1, _map._anchors[2].Y);



        // Death condition
        for (auto o : _ghostBlob) {
            if (o->IsDisposed())
                continue;
            auto gx = o->GetPosition();
            auto px = _player->GetPosition();
            auto pSpawn = _map._anchors[0];
            auto gSpawn = _map._anchors[1];
            if ((int)gx.X == (int)px.X && (int)gx.Y == (int)px.Y){
                if (((_Player*)_player)->IsSuperActive()) {
                    //o->SetPosition(gSpawn.X, gSpawn.Y);
                    o->Dispose();
                }
                else
                {

                    _player->SetPosition(pSpawn.X, pSpawn.Y);
                    ((GameObject*)_ghostBlob[0])->_isDisposed = false;
                    ((GameObject*)_ghostBlob[1])->_isDisposed = false;
                    ((GameObject*)_ghostBlob[2])->_isDisposed = false;
                    ((GameObject*)_ghostBlob[3])->_isDisposed = false;
                	_ghostBlob[0]->SetPosition(gSpawn.X - 2, gSpawn.Y);
                    _ghostBlob[1]->SetPosition(gSpawn.X - 1, gSpawn.Y);
                    _ghostBlob[2]->SetPosition(gSpawn.X, gSpawn.Y);
                    _ghostBlob[3]->SetPosition(gSpawn.X + 1, gSpawn.Y);
                    for (auto o : _tileBlob) {
                        ((GameObject*)o)->_isDisposed = false;
                        ((GameObject*)o)->RequestRender();
                    }
                    break;
                }
            }
        }
        
	}
	void Render()
	{

        _player->Render();
		for (auto o : _ghostBlob)
			o->Render();
        //_ghostBlob[0]->Render();

	}

    int CheckNears(int targetIndex, MOVER_ENTITY_t::eDirection dir)
    {

        IGameObject* top = _tileBlob[targetIndex-_map.GetWidth()];
        IGameObject* down = _tileBlob[targetIndex+_map.GetWidth()];
        IGameObject* left = _tileBlob[targetIndex-1];
        IGameObject* right = _tileBlob[targetIndex+1];
        int result = 0;
        constexpr int topByte   = 1; // top
        constexpr int downByte  = 2; // down
        constexpr int leftByte  = 4; // left
        constexpr int rightByte = 8; // right
        constexpr int selfByte = 16; // right
        IGameObject* self = _tileBlob[targetIndex];


       if (!top->IsSolid() )
            result |= topByte;
        if (!down->IsSolid()  )
            result |= downByte;
        if (!left->IsSolid()  )
            result |= leftByte;
        if (!right->IsSolid() )
            result |= rightByte;
        /*
        if (!top->IsSolid() && (int)dir != downByte)
            result |= topByte;
        if (!down->IsSolid() && (int)dir != topByte)
            result |= downByte;
        if (!left->IsSolid() && (int)dir != rightByte)
            result |= leftByte;
        if (!right->IsSolid() && (int)dir != leftByte)
            result |= rightByte;
        */
        return result;
    }

    char* CheckNearsStr(int targetIndex, MOVER_ENTITY_t::eDirection dir)
    {

        IGameObject* top = _tileBlob[targetIndex - _map.GetWidth()];
        IGameObject* down = _tileBlob[targetIndex + _map.GetWidth()];
        IGameObject* left = _tileBlob[targetIndex - 1];
        IGameObject* right = _tileBlob[targetIndex + 1];
        IGameObject* self = _tileBlob[targetIndex];
        int result = 0;
        char* _result = new char[6]{ '0','0','0','0','0',0 };
		constexpr int topByte = 1; // top
        constexpr int downByte = 2; // down
        constexpr int leftByte = 4; // left
        constexpr int rightByte = 8; // right
        constexpr int selfByte = 16; // right
        if (!self->IsDisposed())
            _result[4] = '1';
        if (!top->IsSolid() && (int)dir != downByte)
            _result[3] = '1';
        if (!down->IsSolid() && (int)dir != topByte)
            _result[2] = '1';
        if (!left->IsSolid() && (int)dir != rightByte)
            _result[1] = '1';
        if (!right->IsSolid() && (int)dir != leftByte)
            _result[0] = '1';



        return _result;
    }


	DataBlob<IGameObject*>& GetTiles()
	{
		return _tileBlob;
	}
	DataBlob<IGameObject*>& GetGhosts()
	{
		return _ghostBlob;
	}
	IGameObject* GetPlayer()
	{
		return _player;
	}

private:
    DataArray<IGameObject*> _objArr;
	DataBlob<IGameObject*> _tileBlob;
	DataBlob<IGameObject*> _ghostBlob;
	IGameObject* _player;
	GameMap _map;
	const char* _mapFile;

};
