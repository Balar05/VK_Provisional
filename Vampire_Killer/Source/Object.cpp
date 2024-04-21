#include "Object.h"
#include "StaticImage.h"

Object::Object(const Point& p, ObjectType t) : Entity(p, OBJECT_PHYSICAL_SIZE, OBJECT_PHYSICAL_SIZE, OBJECT_FRAME_SIZE, OBJECT_FRAME_SIZE)
{
	type = t;

	Rectangle rc;
	const int n = TILE_SIZE;
	switch (type)
	{
	case ObjectType::GOLDEN_KEY: rc = { 16 * n,  0, n, n }; break;
	case ObjectType::SILVER_KEY: rc = { 17 * n,  0, n, n }; break;


	default: LOG("Internal error: object creation of invalid type");
	}

	ResourceManager& data = ResourceManager::Instance();
	render = new StaticImage(data.GetTexture(Resource::IMG_TILES), rc);
}
Object::~Object()
{
}
void Object::DrawDebug(const Color& col) const
{
	Entity::DrawHitbox(pos.x, pos.y, width, height, col);
}
//int Object::Points() const
//{
//	if (type == ObjectType::GOLDEN_KEY)		return POINTS_APPLE;
//	//else if (type == ObjectType::CHILI)	return POINTS_CHILI;
//	else
//	{
//		LOG("Internal error: object type invalid when giving points");
//		return 0;
//	}
//}