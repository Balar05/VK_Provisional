#pragma once
#include "Entity.h"

#define OBJECT_PHYSICAL_SIZE	14
#define OBJECT_FRAME_SIZE		16

//#define POINTS_APPLE	10
//#define POINTS_CHILI	20

enum class ObjectType { GOLDEN_KEY, SILVER_KEY };

class Object : public Entity
{
public:
	Object(const Point& p, ObjectType t);
	~Object();

	void DrawDebug(const Color& col) const;
	bool isCollected = false;
	//int Points() const;
	ObjectType type;
private:

};