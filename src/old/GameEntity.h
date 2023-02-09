#pragma once
#include "GameBase.h"
#include "GameObject.h"

class _Player;

typedef class ENTITY_GAMEOBJ_t : public GameObject
{
public:
    ENTITY_GAMEOBJ_t(Int x, Int y, int wc, int dwFlags, const char* objName = 0, int objTypeID = GOBJ_ENTITY) :
        GameObject(x, y, wc, dwFlags, objName, objTypeID),
        _oldx(x),
        _oldy(y)
    {}




    void Update() override
    {

    }
    void Render() override
    {
        if (_renderPending && !_isDisposed) {
            //printf("a\n");
            //printAtEx(_symbol, _oldx, _oldy, _x, _y);
            printAtEx2(_symbol, _oldx, _oldy, _x, _y, _color);
            _renderPending = false;
        }
    }


protected:
    double _oldx;
    double _oldy;

}EntityObj;


typedef class MOVER_ENTITY_t : public EntityObj
{
public:
    enum class eDirection : int
    {
        IDLE = 0,
    	TOP = 1,
        DOWN = 2,
        LEFT = 4,
        RIGHT = 8

    };
    /*constexpr auto CanMoveTOP() const -> bool { return (_y - _dy > 0) && !_WillCollide(); }
    constexpr auto CanMoveDOWN() const -> bool { return _y + _dy < 70 && !_WillCollide(); }
    constexpr auto CanMoveLEFT() const -> bool { return _x - _dx > 0 && !_WillCollide(); }
    constexpr auto CanMoveRIGHT() const -> bool { return _x + _dx < 30 && !_WillCollide(); }*/

    constexpr auto CanMoveTOP() const -> bool { return (_y - _dy > 0)  && _neighborData - (_neighborData & 1) == 0; }
    constexpr auto CanMoveDOWN() const -> bool { return _y + _dy < 70  && _neighborData - (_neighborData & 2) == 0; }
    constexpr auto CanMoveLEFT() const -> bool { return _x - _dx > 0   && _neighborData - (_neighborData & 4) == 0; }
    constexpr auto CanMoveRIGHT() const -> bool { return _x + _dx < 30 && _neighborData - (_neighborData & 8) == 0; }

public:
    bool allowRandDirection = true;
    POINT2D GetVelocity() const
    {
        return POINT2D((Int)_dx, (Int)_dy);
    }

    MOVER_ENTITY_t(Int x, Int y, char wc, int dwFlags, const char* objName = 0, int objTypeID = GOBJ_ENTITY) :
        EntityObj(x, y, wc, dwFlags, objName, objTypeID), Direction(eDirection::IDLE)
    {
        nextDir = eDirection::IDLE;

        _dx = .3;
        _dy = .3;
    }

    MOVER_ENTITY_t(Int x, Int y, char wc, int dwFlags, double dx, double dy, eDirection dir, const char* objName = 0, int objTypeID = GOBJ_ENTITY) :
        EntityObj(x, y, wc, dwFlags, objName, objTypeID), Direction(dir)
    {
        nextDir = eDirection::IDLE;

        _dx = dx;
        _dy = dy;
    }


    bool IntersectsNow(pIGameObject obj) const
    {
        POINT2D dx = obj->GetPosition();

        POINT2D ax = GetPosition();
        return ((int)ax.Y == (int)dx.Y) && (int)ax.X == (int)dx.X;
    }
    IGameObject* IsColliding() const
    {

        for (int i = 0; i < _objPoolCount; i++)
        {

        	pIGameObject obj = _objPool[i];
                if (IntersectsNow(obj))
                {
                    return obj;
                }

        }
        return 0;
    }
    bool IntersectsNext(pIGameObject obj) const
    {
        POINT2D dx = obj->GetPosition();

        POINT2D ax = GetPosition();
        POINT2D vel = GetVelocity();
        eDirection dir = GetDirection();
        int c = 0;
        switch (dir)
        {
        case eDirection::TOP:
            return ((int)(_y - _dy - c) == (int)dx.Y) && (int)_x == (int)dx.X;
        case eDirection::DOWN:
            return ((int)(_y + _dy + c) == (int)dx.Y) && (int)_x == (int)dx.X;
        case eDirection::LEFT:
            return ((int)(_x - _dx - c) == (int)dx.X) && (int)_y == (int)dx.Y;
        case eDirection::RIGHT:
            return ((int)(_x + _dx + c) == (int)dx.X) && (int)_y == (int)dx.Y;
        default:
            return false;
        }
    }


    bool _WillCollide() const
    {
        for (int i = 0; i < _objPoolCount; i++)
        {
            pIGameObject obj = _objPool[i];
            if (obj->IsSolid() && IntersectsNext(obj))
            {
                return true;
            }
        }
        return false;
    }

    void RandDirection()
    {
        if (_neighborData == 1 || _neighborData == 2 || _neighborData == 4 || _neighborData == 8) {
            Direction = (eDirection)_neighborData;
            return;
        }


        int _d = rand() % 4;
        unsigned int d = 1 << _d;
        if ((_neighborData & d) == d) {
            if(d == 1)
            {
                if ((int)Direction != 2)
                    Direction = (eDirection)d;
            }else if (d == 2)
            {
                if ((int)Direction != 1)
                    Direction = (eDirection)d;
            }else if (d == 4)
            {
                if ((int)Direction != 8)
                    Direction = (eDirection)d;
            }else if (d == 8)
            {
	            if((int)Direction != 4)
                    Direction = (eDirection)d;
            }




        }



    }

    void Update() override
    {
        if(allowRandDirection)
            if (rand() % 3 == 1) {
				RandDirection();
        }

        if (_selfPosInMap != 0)
        {
            OnCollisionEventArgs args;
            args.target = _selfPosInMap;
            if(IsPlayer())
            {
	            if(_selfPosInMap->IsSuper())
	            {
                    ActivateSuper(100);
                //class _Player { public: int Super = 0; void ActivateSuper(int time) { Super = time; } };
	            }
            }
            args.RequestTargetRedraw = !IsPlayer();
            OnCollision.Invoke(this, args);
        }

        switch (Direction)
        {
        case eDirection::TOP:
            if ((_neighborData & 1) == 1) {
                SetPosition(_x, _y - _dy);
                //  _selfPosInMap->RequestRender();
            }
            else
                if (allowRandDirection)

                RandDirection();
            break;
        case eDirection::DOWN:
            if ((_neighborData & 2) == 2) {
                SetPosition(_x, _y + _dy);
                // _selfPosInMap->RequestRender();
            }
            else        if (allowRandDirection)

                RandDirection();
            break;
        case eDirection::LEFT:
            if ((_neighborData & 4) == 4) {
                SetPosition(_x - _dx, _y);
                // _selfPosInMap->RequestRender();
            }
            else        if (allowRandDirection)

                RandDirection();


            break;
        case eDirection::RIGHT:
            if ((_neighborData & 8) == 8) {
                SetPosition(_x + _dx, _y);
                //_selfPosInMap->RequestRender();
            }
            else        if (allowRandDirection)

                RandDirection();
            break;
        case eDirection::IDLE:

            break;
        }

    }
    void Update1()
    {
        //
        if (nextDir != eDirection::IDLE)
        {
            ChangeDirection(nextDir);
        }
        IGameObject* c = IsColliding();
        if (c != 0)
        {
            OnCollisionEventArgs args;
            args.target = c;

            args.RequestTargetRedraw = !IsPlayer();
            OnCollision.Invoke(this, args);
        }
        switch (Direction)
        {
        case eDirection::TOP:
            if (CanMoveTOP())
                SetPosition(_x, _y - _dy);
            else
                BeforeCollision.Invoke(this, { true,false });

            break;
        case eDirection::DOWN:
            if (CanMoveDOWN())
                SetPosition(_x, _y + _dy);
            else
                BeforeCollision.Invoke(this, { true,false });
            break;
        case eDirection::LEFT:
            if (CanMoveLEFT())
                SetPosition(_x - _dx, _y);
            else
                BeforeCollision.Invoke(this, { false,true });

            break;
        case eDirection::RIGHT:
            if (CanMoveRIGHT())
                SetPosition(_x + _dx, _y);
            else
                BeforeCollision.Invoke(this, { false,true });
            break;
        case eDirection::IDLE:

            break;
        }

    }
    eDirection GetDirection() const { return Direction; }

    void ChangeDirection2(eDirection newDir)
    {
        nextDir = newDir;
    }

	void ChangeDirection(eDirection newDir)
    {
        // printf("a\n");
        eDirection oldDir = Direction;
        Direction = newDir;
        nextDir = eDirection::IDLE;
        if (_WillCollide())
        {
            Direction = oldDir;
            nextDir = newDir;
        }
        else
        {

        }
        //Direction = newDir;
    }
protected:
    eDirection Direction;
    eDirection nextDir;
    double _dx;
    double _dy;
private:

}MoverEntity, EntityGameObject;

class _Player : public MoverEntity
{
public:
    _Player(Int x, Int y, char wc, int dwFlags, const char* objName = 0, int objTypeID = GOBJ_ENTITY) :
        MoverEntity(x, y, wc, dwFlags, 0.4, 0.4, eDirection::IDLE, objName, objTypeID)
    {
        allowRandDirection = false;

    }




    bool IsDead = false;

    /*void ChangeDirection(eDirection newDir)
    {
        Direction = newDir;
    }*/

    int c = 1;
public:
    void Update() override
    {
        //this->SetColor(White);

    	if(IsSuperActive())
        {

            Super--;
            if(c==1)
            {
                //this->SetColor(Blue);
                c = 0;
            }else
            {
                //this->SetColor(White);
                c = 1;
            }
        }


        MoverEntity::Update();
        //if (_neighborData >= 1)
           // RandDirection();
        switch (Direction)
        {
        case eDirection::TOP:
            this->_symbol = '^';
            //if ((_neighborData & 1) == 1) {
              //  SetPosition(_x, _y - _dy);
              //  _selfPosInMap->RequestRender();
            //}
            break;
        case eDirection::DOWN:
            this->_symbol = 'v';
            //if ((_neighborData & 2) == 2) {
              //  SetPosition(_x, _y + _dy);
               // _selfPosInMap->RequestRender();
            //}
            break;
        case eDirection::LEFT:
            this->_symbol = '<';
            //if ((_neighborData & 4) == 4) {
              //  SetPosition(_x - _dx, _y);
               // _selfPosInMap->RequestRender();
            //}

            break;
        case eDirection::RIGHT:
            this->_symbol = '>';
            //if ((_neighborData & 8) == 8) {
             //   SetPosition(_x + _dx, _y);
                //_selfPosInMap->RequestRender();
            //}
            break;
        case eDirection::IDLE:

            break;
        }





        if (nextDir != eDirection::IDLE)
        {
            if (nextDir == eDirection::TOP && (_neighborData & 1) == 1) {
                Direction = nextDir;
                nextDir = eDirection::IDLE;
            }
            else if (nextDir == eDirection::DOWN && (_neighborData & 2) == 2) {
                Direction = nextDir;
                nextDir = eDirection::IDLE;
            }
            else if (nextDir == eDirection::LEFT && (_neighborData & 4) == 4) {
                Direction = nextDir;
                nextDir = eDirection::IDLE;
            }
            else if (nextDir == eDirection::RIGHT && (_neighborData & 8) == 8) {
                Direction = nextDir;
                nextDir = eDirection::IDLE;
            }

        }









    }
};
