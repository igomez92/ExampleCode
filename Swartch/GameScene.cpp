#include "GameScene.h"
#include "Util.h"



GameScene::GameScene(void)
	:backgroundOffsetMed(0),backgroundOffsetLow(0),defaultSpeed(400), done(false), windowUpdating(true), deltaTime(0.0f),
	TOP_OF_WINDOW(0),alphaDifference(1),glowCounter(0)
{
	mainPlayer = new Player();	
	playerspeed = defaultSpeed;
}

GameScene::~GameScene(void)
{
}

void GameScene::initializeGame()
{
	//set the rand seed
	srand(time(NULL));

	//load background textures
	sf::Texture* bgImage = new sf::Texture;
	bgImage->loadFromFile("media/backgrounds/starsLow.png");
	bgImage->setRepeated(true);
	backgroundSpriteLow.setTexture(*bgImage);
	backgroundSpriteLow.setTextureRect(sf::IntRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT * 2));
	backgroundSpriteLow.setOrigin(0, SCREEN_HEIGHT);

	bgImage->loadFromFile("media/backgrounds/starsLow.png");("media/backgrounds/starsMed.png");
	bgImage->setRepeated(true);
	backgroundSpriteMed.setTexture(*bgImage);
	backgroundSpriteMed.setTextureRect(sf::IntRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT * 2));
	backgroundSpriteMed.setOrigin(0, SCREEN_HEIGHT);

	font.loadFromFile("arial.ttf");
	userNameText.setFont(font);
	userNameText.setColor(sf::Color::White);
	userNameText.setPosition(0,0);

	(*mainPlayer).setName("");
	(*mainPlayer).setPosition(sf::Vector2f(0, 0));
	(*mainPlayer).setSpeed(400);
	(*mainPlayer).setPlayerNumber(1);
	(*mainPlayer).setScore(0);

	//userNameText.setString(playerName);
}


int GameScene::updatePlayer()
{
	if(!done)
	{
		deltaTime = clock.getElapsedTime().asSeconds();
		clock.restart();
		mainPlayer->move(deltaTime);
		for(int i = 0; i < enemyPlayers.size(); i++)
		{
			enemyPlayers[i]->move(deltaTime);
		}
		//color update, makes things glow
		int alpha = mainPlayer->getRect()->getFillColor().a;

		if(alpha < 150)
		{
			alphaDifference *= -1;
			alpha = 150;
		}
		else if(alpha > 250)
		{
			alphaDifference *= -1;
			alpha = 250;
		}
		glowCounter += 20;

		if(glowCounter >= 5 )
		{
			glowCounter = 0;
			mainPlayer->getRect()->setFillColor(sf::Color(mainPlayer->getRect()->getFillColor().r, 
				mainPlayer->getRect()->getFillColor().g, mainPlayer->getRect()->getFillColor().b, alpha - alphaDifference));
		}

		//std::cout << "Windows updating " << windowUpdating << std::endl;
		if(windowUpdating)
		{
			sf::Vector2f dir;
			dir.x = -9;

			// Move players
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				dir = sf::Vector2f(0,-1);
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				dir = sf::Vector2f(0,1); 
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				dir = sf::Vector2f(-1,0);
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				dir = sf::Vector2f(1,0);
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
			{
				//std::cout << "Disconnected from server" << std::endl;
				sf::Packet newPacket;
				newPacket << 9 << mainPlayer->getPlayerNumber();
				currentSocket->send(newPacket);
				done = true;
				return 1;

			}
			if(dir.x != -9)
			{
				sf::Packet newPacket;
				//std::cout << "sending code 2 packet" << std::endl;
				newPacket << 2 << dir.x << dir.y;
				currentSocket->send(newPacket);
			}
		}
	}
	return 0;
}

void GameScene::updateGame(sf::RenderWindow& window, sf::Event& Event)
{
	
	backgroundSpriteMed.setPosition(0, backgroundOffsetMed);

	deltaTime = (deltaTimer.getElapsedTime() - lastFrameTime).asSeconds();
	lastFrameTime = deltaTimer.getElapsedTime();


	if(updatePlayer() == 1 && windowUpdating)
		window.close();

	backgroundOffsetLow += 320 * deltaTime;

	if (backgroundOffsetLow > 512)
		backgroundOffsetLow -= 512;

	backgroundSpriteLow.setPosition(0, backgroundOffsetLow);
	backgroundOffsetMed += 400 * deltaTime;

	if (backgroundOffsetMed > 512)
		backgroundOffsetMed -= 512;

	backgroundSpriteMed.setPosition(0, backgroundOffsetMed);

	if(Event.type == sf::Event::Closed || Event.key.code == sf::Keyboard::Escape)
	{
		done = true;
	}
	if(Event.type == sf::Event::KeyReleased)
	{

	}
	std::string enemyInfo = "";
	for(int x = 0; x < enemyPlayers.size(); x++)
		enemyInfo += enemyPlayers[x]->getName() + " " + std::to_string(enemyPlayers[x]->getScore()) + "\n";
	userNameText.setString(playerName + " " + std::to_string(mainPlayer->getScore()) + "\n" + enemyInfo);
}


void GameScene::draw(sf::RenderWindow& window)
{
	window.draw(backgroundSpriteLow);
	window.draw(backgroundSpriteMed ,sf::RenderStates(sf::BlendAdd));


	window.draw((*mainPlayer->getRect())); 

	for(std::vector<Player*>::iterator it = enemyPlayers.begin(); it != enemyPlayers.end(); it++)
	{
		window.draw((*(*it)->getRect()));
	}



	for (int i = 0; i <5 ; i++) 
	{
		window.draw(pelletList[i]);
	}
	window.draw(userNameText);

}

void GameScene::checkPacket()
{
	while(!done)
	{
		sf::Packet tempPacket;
		if(currentSocket->receive(tempPacket) == sf::Socket::Done)
		{

			int code = 0;

			tempPacket >> code;
			//std::cout << "A Packet was recieved with code: " << code << std::endl;
			if(code == 2)
			{
				//std::cout << "Code 2 was recieved" << std::endl;
				sf::Vector2f tempPos;
				sf::Vector2f tempRect;

				float score;
				int clientNumber;
				tempPacket >> tempPos.x >> tempPos.y >> tempRect.x >> tempRect.y >> score >> clientNumber;
				std::cout << tempPos.x << " " << tempPos.y << " " << tempRect.x << " " << tempRect.y << " recieved" << std::endl; 
				mainPlayer = new Player(playerName,tempPos,400,-1,10);
				mainPlayer->playerRect->setFillColor(sf::Color(0,255,0));
				mainPlayer->playerRect->setSize(tempRect);
				mainPlayer->setPosition(tempPos);
				mainPlayer->setPlayerNumber(clientNumber);
				mainPlayer->setScore(score);
				//std::cout << "ClientNumber " << clientNumber << std::endl;


			}
			if(code == 3)
			{
				std::cout << "New Enemy " << std::endl;
				Player* enemy = new Player();
				int playerNum;
				sf::Vector2f tempPos;
				sf::Vector2f tempRect;
				std::string name;

				float score;
				tempPacket >> playerNum >> tempPos.x >> tempPos.y >> tempRect.x >> tempRect.y >> score>>name;
				(*enemy).setPosition(tempPos);
				(*enemy).setRectSize(tempRect);
				(*enemy).setPlayerNumber(playerNum);
				(*enemy).setScore(score);
				(*enemy).getRect()->setFillColor(sf::Color(255,0,0));
				(*enemy).setName(name);
				enemyPlayers.push_back(enemy);
			}
			if(code == 4)
			{
				std::cout << "Pellet was recieved" << std::endl;
				int pelletnum;
				sf::Vector2f pos;
				tempPacket >> pelletnum >> pos.x >> pos.y ;
				std::cout << pelletnum << " " << pos.x << " " << pos.y << std::endl; 
				sf::CircleShape temp = sf::CircleShape(5);
				temp.setPosition(pos);
				pelletList[pelletnum] = temp;

			}
			//std::cout << "Still in reading loop" << std::endl;
			if(code == 5)
			{
				int score;
				int playerNum;
				tempPacket >> playerNum;
				sf::Vector2f newPosition;
				tempPacket >> newPosition.x >> newPosition.y;
				sf::Vector2f newSize;
				tempPacket >> newSize.x >> newSize.y;
				tempPacket >> score;
				//tempPacket >> score;
				if(playerNum == mainPlayer->getPlayerNumber())
				{
				//std::cout << "Main player movement" << std::endl;
				
				mainPlayer->setMoveTo(newPosition);
				mainPlayer->playerRect->setSize(newSize);
				mainPlayer->setScore(score);

				}
				/*else
				{
					for(int i = 0; i < enemyPlayers.size(); i++)
					{
						if(playerNum == enemyPlayers[i]->getPlayerNumber())
						{
								//std::cout << "Enemy player movement" << std::endl;
								
								enemyPlayers[i]->getRect()->setPosition(newPosition);
								enemyPlayers[i]->getRect()->setSize(newSize);
						}
					}
				}*/

			}
			if(code == 6)
			{
				int playerNum;
				int score; 
				tempPacket >> playerNum;
				sf::Vector2f newPosition;
				tempPacket >> newPosition.x >> newPosition.y;
				sf::Vector2f newSize;
				tempPacket >> newSize.x >> newSize.y;
				tempPacket >> score;
				for(int i = 0; i < enemyPlayers.size(); i++)
					{
						if(playerNum == enemyPlayers[i]->getPlayerNumber())
						{
								//std::cout << "Enemy player movement" << std::endl;
								
								enemyPlayers[i]->setMoveTo(newPosition);
								enemyPlayers[i]->getRect()->setSize(newSize);
								enemyPlayers[i]->setScore(score);
						}
					}
			}
			if(code == 9)
			{
				int enemyNumber;
				tempPacket >> enemyNumber;
				int enemyToDelete = -1;
				std::cout << enemyPlayers.size() << std::endl;

				for(int x = 0; x < enemyPlayers.size(); x++)
				{
					//std::cout << "Looping through enemies currently at : " << x << std::endl;
					if(enemyPlayers[x]->getPlayerNumber() == enemyNumber)
						enemyToDelete = x;
				}
				if(enemyToDelete != -1)
				{
					//std::cout << "Deleting enemy at " << enemyToDelete << std::endl;
					delete enemyPlayers[enemyToDelete]->getRect();
					enemyPlayers.erase(enemyPlayers.begin() + enemyToDelete);

				}
			}
		}
	}
}