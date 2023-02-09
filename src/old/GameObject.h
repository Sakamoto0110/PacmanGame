#pragma once

#include "GameBase.h"
struct BeforeObjectConsumedEventArgs
{
    bool CANCEL = false;
};

struct EntityCollisionEventArgs
{
    bool vertical = false;
    bool horizontal = false;

};
struct OnCollisionEventArgs
{
    bool RequestTargetRedraw = false;
    class IGAMEOBJECT_t* target;
};

typedef class IGAMEOBJECT_t
{
public:
    template<typename TArgs> using ObjectMapEvent = Event< IGAMEOBJECT_t*, TArgs>;

    using BeforeActionEvent = ObjectMapEvent<void*>;
    using ActionEvent = ObjectMapEvent<void*>;
    using AfterActionEvent = ObjectMapEvent<void*>;

    using BeforeConsumedEvent = ObjectMapEvent<BeforeObjectConsumedEventArgs&>;
    using ConsumedEvent = ObjectMapEvent<BeforeObjectConsumedEventArgs&>;

    using BeforeEntityCollision = ObjectMapEvent<const EntityCollisionEventArgs&>;
    using OnEntityCollision = ObjectMapEvent<OnCollisionEventArgs&>;


    virtual void SetPosition(double x, double y) = 0;
    virtual POINT2D GetPosition() const = 0;
    virtual const char* GetObjTypeName() const = 0;
    //virtual bool Intersects() const = 0;
    virtual void Render() = 0;

    virtual int GetTypeID() const = 0;
    virtual void SetColor(int colorID) = 0;
    virtual bool IsSolid() const = 0;
    virtual void BindToObjectPool(IGAMEOBJECT_t**, int) = 0;
    virtual void Update() = 0;
    virtual void RequestRender() = 0;
    virtual bool IsAutoRedraw() const = 0;
    virtual bool IsPlayer() const = 0;
    virtual bool IsDisposed() const = 0;
    virtual bool IsEnemy() const = 0;
    virtual bool IsNull() const = 0;

    virtual void SetNeighborData(unsigned int value) = 0;
	virtual void SetNeighborData(class GAMEOBJECT_t * target, unsigned int value) = 0;
    virtual bool IsDisposable() const = 0;
    virtual void Dispose() = 0;
    virtual bool IsSuper() const = 0;
}IGameObject, * pIGameObject;


typedef class GAMEOBJECT_t : public IGameObject
{
public:
    BeforeEntityCollision BeforeCollision;
    OnEntityCollision OnCollision;
    bool _isDisposed = false;
    bool IsDisposed() const override { return _isDisposed; }
    void Dispose() override { _isDisposed = true; }
    bool IsNull() const override { return (_objTypeID & GOBJ_NULL) != GOBJ_NULL; }
    bool IsDisposable() const override {return (_dwFlags & GO_DISPOSABLE) == GO_DISPOSABLE; }
    bool IsSuper() const override { return (_dwFlags & GO_SUPER) == GO_SUPER; }
    GAMEOBJECT_t(Int x, Int y, char wc, int dwFlags, const char* objName = 0, int typeID = GOBJ_UNDEFINED) :
        _objTypeID(typeID),
        _objName(objName),
        _x(x),
        _y(y),
        _symbol(wc),
        _dwFlags(dwFlags) {}



    bool IsEnemy() const override
    {
        return (_dwFlags & GO_ENEMY) != 0;
    }

    /*
    GAMEOBJECT_t(Int x, Int y, char wc, int dwFlags, Color color, const char* objName = 0, int typeID = GOBJ_UNDEFINED) :
        _objTypeID(typeID),
        _objName(objName),
        _x(x),
        _y(y),
        _symbol(wc),
        _dwFlags(dwFlags),
        _color(color) {}
    */
    const char* GetObjTypeName() const override { return _GetObjTypeName(*this); }
    void RequestRender() override
    {
        _renderPending = true;
    }
    int GetTypeID() const override
    {
        return _objTypeID;
    }
    void SetColor(int colorID) override
    {
        _color = colorID;
    }
    bool IsSolid() const override
    {
        return (_dwFlags & GO_SOLID) != 0;
    }
    bool IsAutoRedraw() const override
    {
        return (_dwFlags & GO_AUTOREDRAW) != 0;
    }
    bool IsPlayer() const override
    {
        return (_dwFlags & GO_PLAYER) != 0;
    }
    void BindToObjectPool(IGAMEOBJECT_t** objPool, int cObjPool) override
    {
        _objPool = objPool;
        _objPoolCount = cObjPool;
    }
    int Super = 0;
    bool IsSuperActive() { return Super > 0; }
    void ActivateSuper(int time) { Super = time;}
protected:
    int _objPoolCount;
    pIGameObject* _objPool;

    int   _objTypeID;
    const char* _objName;
    //LPCWSTR _objTypeName;
    int _color;
    double _x;
    double _y;
    char _symbol;
    int _dwFlags;


    bool _renderPending = true;

    friend const char* _GetObjTypeName(const GAMEOBJECT_t&);
    auto SetPosition(double x, double y) -> void override { _renderPending = (_x != x || _y != y); _x = x; _y = y; }
    auto GetPosition() const -> POINT2D override { return { (Int)_x,(Int)_y }; }

    unsigned int _neighborData = 0;
    GAMEOBJECT_t* _selfPosInMap = 0;
    void SetNeighborData(unsigned int value)
    {
        _neighborData = value;
    }
    void SetNeighborData(GAMEOBJECT_t* target, unsigned int value)
    {
        _selfPosInMap = target;
        _neighborData = value;
    }


	/*void Render() override
    {

    }*/

}GameObject, * pGameObject;

inline const char* _GetObjTypeName(const GAMEOBJECT_t& obj)
{
    switch (obj._objTypeID)
    {
    case GOBJ_NULL:
        return "NullObject";
    case GOBJ_STATIC:
        return "StaticObject";
    case GOBJ_CONSUMABLE:
        return "ConsumableObject";
    case GOBJ_DYNAMIC:
        return "DynamicObject";
    case GOBJ_ACTIVE:
        return "ActiveObject";
    case GOBJ_PLAYER:
        return "PlayerObject";
    default:
        return "UndefinedObject";

    }
}

typedef class NULL_GAMEOBJ_t : public GAMEOBJECT_t
{
public:
    NULL_GAMEOBJ_t() : GAMEOBJECT_t(0, 0, 0, 0, 0, 0) {}

    void Update() override {}
    void Render() override {}

}NullGameObject;

typedef class STATIC_GAMEOBJ_t : public GameObject
{
public:
    STATIC_GAMEOBJ_t(Int x, Int y, char wc, int dwFlags, const char* objName = 0, int objTypeID = GOBJ_STATIC)
        : GAMEOBJECT_t(x, y, wc, dwFlags, objName, objTypeID) {}


    void Update() override {}
    void Render() override
    {
        if (_renderPending && !_isDisposed)
        {
            printAt2(_symbol, _x, _y,_color);
            _renderPending = false;
        }
    }

}StaticGameObject, TileGameObject;
