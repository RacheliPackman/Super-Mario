#include "Game.h"
#include "Config.h"
#include <algorithm>

Game::Game(int width, int height)
	:timer(100), slimesInUse(new std::vector<EntityPtr>()),
	handGrenadeInUse(new std::vector<EntityPtr>()),
	shelfs(new std::vector<EntityPtr>()),
	coinsInUse(new std::vector<EntityPtr>()),
	slimePool(createSlimeEnemy, R"(../Animations/SlimeOrange)"),
	handGrenadePool(createHandGrenade, R"(../Animations/HandGrenade)"),
	coinPool(createCoin, R"(../Animations/Coin)"),
	gamePtr(this)
{
	background = cv::imread(R"(../Animations/background.png)", cv::IMREAD_UNCHANGED);
	resize(background, background, cv::Size(width, height));

	hero = createHero(R"(../Animations/Hero)", width, height , height* 2/3);
	hero->reset(cv::Point(background.size().width / 2, height * 2 / 3));

	lives = std::make_shared<LiveEntity>();
	lives->reset(cv::Point(background.size().width - 100, 20));

	scores = std::make_shared<ScoresEntity>(1.5, 200, cv::HersheyFonts::FONT_HERSHEY_SIMPLEX);
	scores->reset(cv::Point(40, 70));

	EntityPtr shelf = createShelf(R"(../Animations/Shelf)");
	shelf->reset(cv::Point(width / 8 + 20, height * 3 / 4));
	shelfs->push_back(shelf);
	shelf = createShelf(R"(../Animations/Shelf)");
	shelf->reset(cv::Point(width * 2 / 8 + 100, height * 5 / 8));
	shelfs->push_back(shelf);
	shelf = createShelf(R"(../Animations/Shelf)");
	shelf->reset(cv::Point(width * 3 / 8 + 100, height * 7 / 16));
	shelfs->push_back(shelf);
	shelf = createShelf(R"(../Animations/Shelf)");
	shelf->reset(cv::Point(width * 4 / 5, height * 3 / 4));
	shelfs->push_back(shelf);
}
void Game::run()
{
	timer.Register(gamePtr);

	slimesInUse->push_back(slimePool.getNext());
	(*slimesInUse)[0]->reset(cv::Point(background.size().width * 2 / 3, background.size().height * 4 / 5));

	for (int i = 0; i < 4; i++) {
		coinsInUse->push_back(coinPool.getNext());
	}
	(*coinsInUse)[0]->reset(cv::Point(background.size().width / 8 + 20 + 50, background.size().height * 3 / 4 - 70));
	(*coinsInUse)[1]->reset(cv::Point(background.size().width * 2 / 8 + 100 + 50, background.size().height * 5 / 8 - 70));
	(*coinsInUse)[2]->reset(cv::Point(background.size().width * 3 / 8 + 100 + 50, background.size().height * 7 / 16 - 70));
	(*coinsInUse)[3]->reset(cv::Point(background.size().width * 4 / 5 + 50, background.size().height * 3 / 4 - 70));

	

	for (auto it = slimesInUse->begin(); it != slimesInUse->end(); it++)
		timer.Register(*it);

	for (auto it = shelfs->begin(); it != shelfs->end(); it++)
		timer.Register(*it);

	for (auto it = coinsInUse->begin(); it != coinsInUse->end(); it++)
		timer.Register(*it);
	
	timer.Register(hero);

	hero->Register(lives);
	hero->Register(scores);
	hero->Register(hero);
	hero->Register(gamePtr);

	
	while (false == isToExit)
	{
		cv::Mat canvas = background.clone();
		timer.tick();
		isOnShelf = false;

		for (EntityPtr shelf : (*shelfs)) {
			if (hero->getState()->getPhysics()->checkCollisionBottom(shelf->getState()->getPhysics()) /*&& shelf != currShelf*/ )
			{
				hero->Notify(Event{ EventSenders::SENDER_ENTITY_STATE, EventTypes::EVENT_PHYSICS, EventCodes::JUMP_TO_SHELF });
				isOnShelf = true;
			}
			shelf->draw(canvas);
		}

		if (!isOnShelf && hero->getState()->getPhysics()->getTL().y < background.size().height * 2 / 3) {
			hero->Notify(Event{ EventSenders::SENDER_ENTITY_STATE, EventTypes::EVENT_PHYSICS, EventCodes::FALL_FROM_SHELF });
		}

		for (int i = 0; i < coinsInUse->size(); i++) {

			(*coinsInUse)[i]->draw(canvas);
			if (hero->checkCollision((*coinsInUse)[i]))
			{
				hero->Notify(Event{ EventSenders::SENDER_ENTITY_STATE, EventTypes::EVENT_PHYSICS, EventCodes::COLLISION_WITH_FOOD_SCORE });
			}
			
		}

		for (EntityPtr slime : (*slimesInUse)) {
			if (hero->checkCollision(slime))
			{
				hero->Notify(Event{ EventSenders::SENDER_ENTITY_STATE, EventTypes::EVENT_PHYSICS, EventCodes::COLLISION_WITH_ENEMY });
			}
			
			slime->draw(canvas);
		}

		for (int i = 0; i < handGrenadeInUse->size();i++) {
			(*handGrenadeInUse)[i]->draw(canvas);
			for (int j = 0; j < slimesInUse->size(); j++) {
				if ((*handGrenadeInUse)[i]->checkCollision((*slimesInUse)[j])) {
					(*handGrenadeInUse)[i]->Notify(Event{ EventSenders::SENDER_ENTITY_STATE, EventTypes::EVENT_PHYSICS, EventCodes::KILL_ENEMY });
				}
			}
		}


		hero->draw(canvas);
		lives->draw(canvas);
		scores->draw(canvas);
		cv::imshow("Game", canvas);
	}
}

void Game::onNotify(Event const& e) 
{
	
	if (e.sender == EventSenders::SENDER_KEYBOARD
		&&
		e.type == EventTypes::EVENT_KEY_PRESSED
		&&
		e.code == EventCodes::KEY_SPACE)
	{
		handGrenadeInUse->push_back(handGrenadePool.getNext());
		(*(handGrenadeInUse->end()-1))->reset(hero->getState()->getPhysics()->getTL() + cv::Point(30, 40));
		
		timer.Register(*(handGrenadeInUse->end()-1));

		
		(*(handGrenadeInUse->end() - 1))->Register(lives);
		(*(handGrenadeInUse->end() - 1))->Register(scores);
		(*(handGrenadeInUse->end() - 1))->Register(gamePtr);
		(*(handGrenadeInUse->end() - 1))->getState()->Register(gamePtr);

	}
	if (e.sender == EventSenders::SENDER_ENTITY_STATE
		&&
		e.type == EventTypes::EVENT_PHYSICS
		&&
		e.code == EventCodes::ENTITY_PHYSICS_FINISHED)
	{
		auto handGrenadeFinishedIt = std::find_if(handGrenadeInUse->begin(), handGrenadeInUse->end(),
			[=](EntityPtr handGrenade) {return handGrenade->getState()->getPhysics()->getTL().y >= background.size().height; });

		handGrenadePool.returnEntity(*handGrenadeFinishedIt);

		timer.UnRegister(*handGrenadeFinishedIt);

		(*handGrenadeFinishedIt)-> getState() ->UnRegister(gamePtr);

		handGrenadeInUse->erase(handGrenadeFinishedIt);

	}
	
	if (e.sender == EventSenders::SENDER_ENTITY_STATE
		&&
		e.type == EventTypes::EVENT_PHYSICS
		&&
		e.code == EventCodes::KILL_ENEMY)
	{	
		for (auto slimeIt = slimesInUse->begin(); slimeIt != slimesInUse->end() ; slimeIt++) {
			for (auto handGrenadeIt = handGrenadeInUse->begin();handGrenadeIt != handGrenadeInUse->end(); handGrenadeIt++) {
				if ((*slimeIt)->checkCollision(*handGrenadeIt))
				{
					handGrenadePool.returnEntity(*handGrenadeIt);
					slimePool.returnEntity(*slimeIt);

					timer.UnRegister(*handGrenadeIt);
					timer.UnRegister(*slimeIt);

					(*handGrenadeIt)->getState()->UnRegister(gamePtr);

					slimesInUse->erase(slimeIt);
					handGrenadeInUse->erase(handGrenadeIt);
					return;
				}
			}
		}

	}

	if (e.sender == EventSenders::SENDER_ENTITY_STATE
		&&
		e.type == EventTypes::EVENT_PHYSICS
		&&
		e.code == EventCodes::COLLISION_WITH_FOOD_SCORE)
	{
			for (auto coinIt = coinsInUse->begin(); coinIt != coinsInUse->end(); coinIt++) {
				if ((*coinIt)->checkCollision(hero))
				{
					coinPool.returnEntity(*coinIt);

					timer.UnRegister(*coinIt);

					coinsInUse->erase(coinIt);
					return;
				}
			}

	}

	if (e.sender == EventSenders::SENDER_KEYBOARD
		&&
		e.type == EventTypes::EVENT_KEY_PRESSED
		&&
		e.code == EventCodes::KEY_ESC)
	{
		isToExit = true;
	}


}
