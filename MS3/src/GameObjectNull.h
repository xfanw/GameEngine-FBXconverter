#ifndef GAME_OBJECT_NULL_H
#define GAME_OBJECT_NULL_H

#include "MathEngine.h"
#include "GameObject.h"
#include "GraphicsObject_Null.h"


class GameObjectNull : public GameObject
{
public:

	GameObjectNull(GraphicsObject_Null  *graphicsObject);

	// Big four
	GameObjectNull() = delete;
	GameObjectNull(const GameObjectNull &) = delete;
	GameObjectNull &operator=(GameObjectNull &) = delete;
	virtual ~GameObjectNull() override;


	virtual void Update(Time currentTime) override;

public:

};

#endif