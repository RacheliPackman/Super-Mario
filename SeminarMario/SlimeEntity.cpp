#include "SlimeEntity.h"
#include <filesystem>


using namespace cv;
using namespace std;
namespace fs = std::filesystem;


EntityStatePtr createSlimeState(fs::path const& animationFolder)
{
	AnimationPtr animation(new Animation(animationFolder.string()));
	bool isCyclic = true;

	IGraphicsComponentPtr graphicsPtr(
		new SingleAnimationGraphics(animation, isCyclic));

	IPhysicsComponentPtr physicsPtr = make_shared<FixedWidgetPhysics>();

	return EntityStatePtr(new EntityState(graphicsPtr, physicsPtr));
	
}


EntityPtr createSlimeEnemy(std::string const& animationFolder)
{
	return EntityPtr(new Entity(createSlimeState(animationFolder)));
}

