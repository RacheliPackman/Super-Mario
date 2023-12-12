#include "CoinEntity.h"
#include <filesystem>


using namespace cv;
using namespace std;
namespace fs = std::filesystem;


EntityStatePtr createCoinState(fs::path const& animationFolder)
{
	AnimationPtr animation(new Animation(animationFolder.string()));
	bool isCyclic = true;

	IGraphicsComponentPtr graphicsPtr(
		new SingleAnimationGraphics(animation, isCyclic));

	IPhysicsComponentPtr physicsPtr = make_shared<FixedWidgetPhysics>();

	return EntityStatePtr(new EntityState(graphicsPtr, physicsPtr));

}


EntityPtr createCoin(std::string const& animationFolder)
{
	return EntityPtr(new Entity(createCoinState(animationFolder)));
}

