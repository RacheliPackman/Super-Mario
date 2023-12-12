#pragma once
#include "Animation.h"
#include <opencv2/opencv.hpp>
#include "Timer.h"
#include "HeroEntity.h"
#include "EntitiesPool.h"
#include "Graphics.h"
#include "LiveEntity.h"
#include "ScoresEntity.h"
#include "HandGrenadeEntity.h"
#include "ShelfEntity.h"
#include "CoinEntity.h"


class Game : public IObserver
{
private:
	cv::Mat background;
	EntityPtr hero;
	EntitiesPool slimePool;
	EntitiesPool handGrenadePool;
	EntitiesPool coinPool;
	std::shared_ptr<std::vector<EntityPtr>> slimesInUse;
	std::shared_ptr<std::vector<EntityPtr>> handGrenadeInUse;
	std::shared_ptr<std::vector<EntityPtr>>coinsInUse;
	Timer timer;
	LiveEntityPtr lives;
	ScoresEntityPtr scores;
	IObserverPtr gamePtr;
	std::shared_ptr<std::vector<EntityPtr>> shelfs;
	bool isOnShelf = false;
public:
	Game(int width, int height);
	void run();
	virtual void onNotify(Event const& e) override;
};

