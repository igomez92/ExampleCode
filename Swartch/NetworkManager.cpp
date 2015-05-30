#include "NetworkManager.h"
#include "sqlite3.h"
#include "Users.h"
#include <time.h>
#include <string>
#include <iostream>
#include <sstream>
#include <random>

NetworkManager::NetworkManager(void)
	:done(false), count(0), maxPlayerLimit(5), initializedGame(false), numberOfPellets(5), currPlayerCount(0)
{
	std::cout << "Starting Server" << std::endl;
	listener.listen(2000); 
	selector.add(listener);
	currPlayerName = "Wrong";
	currPassword = "Wrong";
	std::cout << "This is my server ip address because Isidro wanted it: " << sf::IpAddress::getLocalAddress() << std::endl;
	defaultSize = sf::Vector2f(20,20);
	srand(time(NULL));

	//chooseRandomDirection();


}

NetworkManager::~NetworkManager(void)
{
	// Delete the sockets
	for(std::vector<SocketPlayer>::iterator it = clientMap.begin(); it != clientMap.end(); it++)
		delete it->socket;
	for(std::vector<sf::CircleShape*>::iterator it = pelletList.begin(); it != pelletList.end(); it++)
		delete *it;

}

/*
This method should only run the game loop checking collision 
between players and pellets
*/

void NetworkManager::run()
{

	while(!done)
	{

		//std::cout << "Updating the game" << std::endl;

		//std::cout << "hello" << std::endl;
		//int numberOfPellets = 5;


		//waitForClients();

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			done = true;
		}



		if(initializedGame)
		{
			//threadLock.lock();
			for(int x = 0; x < clientMap.size(); x++)
			{
				
				if(clientMap[x].loggedIn)
				{
					//player to pellet collisions
					for(int y = 0; y < pelletList.size(); y++)
					{
						if(pelletList[y]->getGlobalBounds().intersects(clientMap[x].player->getRect()->getGlobalBounds()))
						{
							std::cout << "Interception" << std::endl;
							clientMap[x].player->grow();
							pelletList[y]->setPosition(rand()%800,rand()%600);
							for(int z = 0; z < clientMap.size(); z++)
							{
								sf::Packet tempPacket;
								tempPacket << 4 << y << pelletList[y]->getPosition().x << pelletList[y]->getPosition().y;
								clientMap[z].socket->send(tempPacket);
							}
						}
					}
					//player to player collisions

				}
				
				for(int i = 0; i < clientMap.size(); i++)
				{
					for(int j = 0; j < clientMap.size(); j++)
					{
						//if both are players are logged in and its not itself
						if(clientMap[i].loggedIn && clientMap[j].loggedIn && clientMap[i].getPlayer()->getPlayerNumber() != clientMap[j].player->getPlayerNumber())
						{
							if(clientMap[i].player->getRect()->getGlobalBounds().intersects(clientMap[j].player->getRect()->getGlobalBounds()))
							{
								//if player i is bigger then player j
								if(clientMap[i].player->rectSize() > clientMap[j].player->rectSize())
								{
									clientMap[i].player->grow();
									clientMap[j].player->die();
								}
								//if both players are equal
								else if(clientMap[i].player->rectSize() == clientMap[j].player->rectSize())
								{
									clientMap[i].player->die();
									clientMap[j].player->die();
								}
								else if(clientMap[i].player->rectSize() < clientMap[j].player->rectSize())
								{
									clientMap[i].player->die();
									clientMap[j].player->grow();
								}
							}
						}
					}
				}
				
				//player to wall collistions

				for(int i = 0; i < clientMap.size(); i++)
				{
					//if player is logged in
					if(clientMap[i].loggedIn)
					{
						//if it touches the walls it dies
						if(clientMap[i].getPlayer()->getPosition().x > 800 || clientMap[i].getPlayer()->getRect()->getPosition().x < 0 
							|| clientMap[i].getPlayer()->getRect()->getPosition().y > 600 || clientMap[i].getPlayer()->getRect()->getPosition().y < 0)
						{
							std::cout << "player position" << clientMap[i].getPlayer()->getPosition().x << clientMap[i].getPlayer()->getPosition().y << " \n";
							clientMap[i].getPlayer()->die();
						}
					}
				}
				
				
			}
			//threadLock.unlock();

			//////

			/////


		}



		/*
		//check Packets

		for(int x = 0; x < clientMap.size(); x++)
		{

		sf::Packet packet;
		//std::cout << "Checking Packets" << std::endl;
		if(clientMap[x].socket->receive(packet) == sf::Socket::Done)
		{
		std::cout << "A packet was recieved from client: " << x + 1 << std::endl;
		if(!packet.endOfPacket())
		{

		int code;
		packet >> code;
		std::cout << "A packet was recieved with code: " << code << std::endl;
		if(code == 1)
		{
		sf::Packet temp;
		packet >> currPlayerName >> currPassword;
		if(login(currPlayerName,currPassword)  && !initializedGame)
		{
		temp << 1 << true;
		//createPellets
		for(int i = 0; i < numberOfPellets; i++)
		{
		sf::Vector2f temp;

		temp.x = rand() % 800;
		temp.y = rand() % 600;
		sf::CircleShape* tempCircle = new sf::CircleShape(5);
		tempCircle->setPosition(temp);
		std::cout << temp.x << " " << temp.y << " pellet position" << std::endl;
		pelletList.push_back(tempCircle);
		}
		initializedGame = true;
		createNewPlayer(clientMap[x].socket, currPlayerName);
		}
		else if(login(currPlayerName,currPassword))
		{
		temp << 1 << true;
		createNewPlayer(clientMap[x].socket, currPlayerName);
		}
		else
		{
		temp << 1 << false;
		}
		clientMap[x].socket->send(temp);
		std::cout <<"sent packet \n";
		}

		//recieve player positions
		if(code == 2)
		{

		sf::Vector2f direction;
		packet >> direction.x >> direction.y;
		std::cout << "Code 2 recieved: " << direction.x << " " << direction.y << std::endl;
		std::cout << "Previous Location: " << clientMap[x].getPlayer()->getRect()->getPosition().x << " " << clientMap[x].getPlayer()->getRect()->getPosition().y << "\n";
		//tempPos = it->getPlayer().getPosition();
		clientMap[x].getPlayer()->getRect()->move(direction.x * 400, direction.y * 400);

		std::cout << "New Location: " << clientMap[x].getPlayer()->getRect()->getPosition().x << " " << clientMap[x].getPlayer()->getRect()->getPosition().y << "\n";
		}
		if(code == 6)
		{

		}

		}
		}
		}	




		if(initializedGame)
		{
		//update gameState
		for(std::vector<SocketPlayer>::iterator it = clientMap.begin(); it != clientMap.end(); it++)
		{
		for(std::vector<SocketPlayer>::iterator it2 = it + 1; it2 != clientMap.end(); it2++)
		{
		if(it->getPlayer()->getName().compare("Default") != 0 && it2->getPlayer()->getName().compare("Default") != 0)
		{
		sf::RectangleShape* tempRect1, *tempRect2;
		tempRect1 = it->getPlayer()->getRect();
		tempRect2 = it2->getPlayer()->getRect();
		//player vs player collisions
		if(tempRect1->getGlobalBounds().intersects(tempRect2->getGlobalBounds()))
		{
		if((tempRect1->getSize().x * tempRect1->getSize().y) > (tempRect2->getSize().x * tempRect2->getSize().y))
		{
		it->getPlayer()->grow();
		it2->getPlayer()->die();
		}
		else if((tempRect1->getSize().x * tempRect1->getSize().y) < (tempRect2->getSize().x * tempRect2->getSize().y))
		{
		it->getPlayer()->die();
		it2->getPlayer()->grow();
		}
		else
		{
		it->getPlayer()->die();
		it2->getPlayer()->die();
		}
		}
		}
		}
		}

		//std::cout << "This is above pellet Check" << std::endl;
		//player vs pellets collistion
		for(std::vector<SocketPlayer>::iterator it = clientMap.begin(); it != clientMap.end(); it++)
		{
		//std::cout << "Checking Pellet Collision" << std::endl;
		if(it->getPlayer()->getName().compare("Default") != 0)
		{
		//std::cout << "currentPlayer name: " << it->getPlayer()->getName() << "\n";
		sf::RectangleShape* tempRect1;
		tempRect1 = it->getPlayer()->getRect();
		int pelletNumber = 0;
		for(std::list<sf::CircleShape*>::iterator it2 = pelletList.begin(); it2 != pelletList.end(); it2++)
		{
		//std::cout << "TempRect : " << tempRect1->getPosition().x << " " << tempRect1->getPosition().y << std::endl;
		//std::cout << "PelletLocation : " << (*it2)->getPosition().x << " " << (*it2)->getPosition().y << std::endl;

		if(tempRect1->getGlobalBounds().intersects((*it2)->getGlobalBounds()))
		{
		std::cout << "Intersected with a pellet" << std::endl;
		it->getPlayer()->grow();
		(*it2)->setPosition(rand()%800,rand()%600);
		for(std::vector<SocketPlayer>::iterator it3 = clientMap.begin(); it3 != clientMap.end(); it3++)
		{
		sf::Packet tempPacket;
		tempPacket << 4 << pelletNumber << (*it2)->getPosition().x << (*it2)->getPosition().y;
		//it3->getSocket()->send(tempPacket);
		}
		}
		pelletNumber++;
		}
		}
		}

		//std::cout<< "Sending all position update \n";
		//sending each client the position of all other clients
		for(std::vector<SocketPlayer>::iterator it = clientMap.begin(); it != clientMap.end(); it++)
		{
		for(std::vector<SocketPlayer>::iterator it2 = clientMap.begin(); it2 != clientMap.end(); it2++)
		{
		if(it->getPlayer()->getName().compare("Default") != 0 && it2->getPlayer()->getName().compare("Default") != 0)
		{
		if(it->getPlayer()->getName().compare(it2->getPlayer()->getName()) != 0)
		{
		sf::Packet tempPacket;
		Player* tempPlayer = it2->getPlayer();
		tempPacket << 7 << tempPlayer->getPlayerNumber() << tempPlayer->getPosition().x << tempPlayer->getPosition().y <<
		tempPlayer->getRect()->getSize().x << tempPlayer->getRect()->getSize().y;

		it->getSocket()->send(tempPacket);
		tempPacket.clear();
		}
		}
		}
		}


		//sending player size
		for(std::vector<SocketPlayer>::iterator it = clientMap.begin(); it != clientMap.end(); it++)
		{
		sf::Packet tempPacket;
		Player* tempPlayer = it->getPlayer();
		tempPacket << 5 << tempPlayer->getEaten();
		it->getSocket()->send(tempPacket);
		}

		}
		//std::cout << "Ending server" << std::endl;
		*/
	}
}

/*
Constantly listening for new clients
that want to join the game and add to the 
client list
*/
void NetworkManager::addNewClient()
{
	while(!done)
	{
		threadLock.lock();
		//std::cout << "Looking for new clients" << std::endl;
		if(selector.wait(sf::milliseconds(10.f)))
		{
			if(selector.isReady(listener))
			{

				sf::TcpSocket* client = new sf::TcpSocket;

				if(listener.accept(*client) == sf::Socket::Done)
				{


					std::cout << "Client " << count << " joined the server" << std::endl;

					threadLock.lock();
					selector.add(*client);

					clientMap.push_back(SocketPlayer(client, new Player()));
					// Allow for input without threading
					client->setBlocking(false);
					++count;
					threadLock.unlock();
				}
				else 
				{
					delete client;
				}
			}
		}
	}
}

/*
Loops through all current clients and 
accepts all packets and parses the information 
immediately
*/

void NetworkManager::readClients()
{
	while(!done)
	{
		//threadLock.lock();
		//std::cout << "Reading from current clients" << std::endl;
		for(int x = 0; x < clientMap.size(); x++)
		{
			sf::Packet tempPacket;
			if(clientMap[x].getSocket()->receive(tempPacket) == sf::Socket::Done)
			{
				//std::cout << "A Packet was recieved from client: " << x + 1 << std::endl;
				int code;
				tempPacket >> code;
				sf::Packet temp;
				if(code == 1)
				{

					tempPacket >> currPlayerName >> currPassword;
					if(login(currPlayerName,currPassword)  && !initializedGame)
					{
						temp << 1 << true;
						//createPellets
						for(int i = 0; i < numberOfPellets; i++)
						{
							sf::Vector2f temp;

							temp.x = rand() % 800;
							temp.y = rand() % 600;
							sf::CircleShape* tempCircle = new sf::CircleShape(5);
							tempCircle->setPosition(temp);
							std::cout << temp.x << " " << temp.y << " pellet position" << std::endl;
							pelletList.push_back(tempCircle);
						}
						initializedGame = true;
						clientMap[x].getSocket()->send(temp);
						createNewPlayer(clientMap[x].socket, currPlayerName, x);
					}
					else if(login(currPlayerName,currPassword))
					{
						temp << 1 << true;
						clientMap[x].getSocket()->send(temp);

						createNewPlayer(clientMap[x].socket, currPlayerName, x);
					}
					else
					{
						temp << 1 << false;
						clientMap[x].getSocket()->send(temp);
					}

				}
				else if(code == 2)
				{
					temp << 5;
					sf::Vector2f direction;
					tempPacket >> direction.x >> direction.y;

					//std::cout << "Sending code 2 Packet" << std::endl;
					clientMap[x].getPlayer()->move(direction);

					temp << clientMap[x].player->getPlayerNumber() <<  clientMap[x].player->getPosition().x << clientMap[x].player->getPosition().y
						<< clientMap[x].player->getRect()->getGlobalBounds().width << clientMap[x].player->getRect()->getGlobalBounds().height;
					clientMap[x].getSocket()->send(temp);

					//send this infor to everyone else
					sf::Packet enemyPacket;
					enemyPacket << 6 << clientMap[x].player->getPlayerNumber() << clientMap[x].player->getPosition().x << clientMap[x].player->getPosition().y
						<< clientMap[x].player->getRect()->getGlobalBounds().width << clientMap[x].player->getRect()->getGlobalBounds().height;

					for(int y = 0; y < clientMap.size();y++)
					{
						if( x != y)
							clientMap[y].socket->send(enemyPacket);
					}
				}

				else if(code == 9)
				{
					//threadLock.lock();
					int clientToDelete, playerNumber;
					clientToDelete = -1;
					tempPacket >> playerNumber;
					for(int x = 0; x < clientMap.size(); x++)
					{
						//std::cout << clientMap[x].player->getPlayerNumber() << " currentPlayer" << std::endl;
						if(clientMap[x].player->getPlayerNumber() == playerNumber)
							clientToDelete = x;
					}
					if(clientToDelete != -1)
					{
						//std::cout << "A Client was deleted at : " << clientToDelete << std::endl;
						delete clientMap[clientToDelete].socket;
						clientMap.erase(clientMap.begin() + clientToDelete);
					}
					sf::Packet deletionPacket;
					deletionPacket << 9 << playerNumber;
					for(int x = 0; x < clientMap.size(); x++)
					{
						clientMap[x].socket->send(deletionPacket);
					}
					//queueLock.unlock();
				}
			}
		}
		//threadLock.unlock();
	}
}

/*
Send Packets in write queue to all those that need it
*/
void NetworkManager::writeToClients()
{
	while(!done)
	{


		//update clients
		//threadLock.lock();
		/*
		while(!updateQueue.empty())
		{

		for(int x = 0; x < clientMap.size(); x++)
		{
		if(clientMap[x].loggedIn)
		{
		//threadLock.lock();
		clientMap[x].socket->send(updateQueue.front());
		//threadLock.unlock();
		}

		}
		//threadLock.lock();
		updateQueue.pop();
		//threadLock.unlock();

		}
		*/
		//threadLock.unlock();
	}
}

/*
createNewPlayer;
send main Player update;
send other players new player update;
send pellet update;
send all other enemies info;

*/
void NetworkManager::createNewPlayer(sf::TcpSocket* socket, std::string name, int vectPosition)
{
	//threadLock.lock();
	clientMap[vectPosition].loggedIn = true;
	clientMap[vectPosition].getPlayer()->setName(name);
	clientMap[vectPosition].getPlayer()->setPlayerNumber(currPlayerCount++);
	clientMap[vectPosition].getPlayer()->getRect()->setPosition(sf::Vector2f(rand()%800,rand()%600));
	clientMap[vectPosition].getPlayer()->getRect()->setSize(defaultSize);


	sf::Packet tempPacket;
	sf::Vector2f tempPos = clientMap[vectPosition].getPlayer()->getPosition();
	sf::RectangleShape* tempRect = clientMap[vectPosition].getPlayer()->getRect();

	std::cout << tempPos.x << " " << tempPos.y << " " << tempRect->getGlobalBounds().width << " " << tempRect->getGlobalBounds().height << " info sent" << std::endl; 
	float score = clientMap[vectPosition].getPlayer()->getScore();
	int clientNumber = clientMap[vectPosition].getPlayer()->getPlayerNumber();

	tempPacket << 2 << tempPos.x << tempPos.y << tempRect->getGlobalBounds().width << tempRect->getGlobalBounds().height<< score << clientNumber;
	std::cout << "Sent New Player packet name: " << clientMap.back().getPlayer()->getName() << std::endl;
	clientMap[vectPosition].socket->send(tempPacket);


	sf::Packet pelletPacket;
	int pallet = 0;
	for(std::vector<sf::CircleShape*>::iterator it = pelletList.begin(); it!= pelletList.end(); it++)
	{
		std::cout << "pellet packet sent" << std::endl;
		pelletPacket << 4 << pallet << (*it)->getPosition().x << (*it)->getPosition().y << (*it)->getRadius();
		pallet++;
		socket->send(pelletPacket);
		pelletPacket.clear();
	}

	tempPacket.clear();

	for(int currEnemy = 0; currEnemy < clientMap.size(); currEnemy++)
	{
		if(currEnemy != vectPosition && clientMap[currEnemy].loggedIn)
		{
			//send currently player position to enemies
			tempPacket << 3 << clientMap[vectPosition].player->getPlayerNumber()
				<< tempPos.x << tempPos.y << tempRect->getGlobalBounds().width << tempRect->getGlobalBounds().height<< score;
			clientMap[currEnemy].socket->send(tempPacket);

			//send enemy position to player
			sf::Packet enemyPacket;
			enemyPacket << 3 << clientMap[currEnemy].player->getPlayerNumber() << clientMap[currEnemy].player->getPosition().x <<
				clientMap[currEnemy].player->getPosition().y << clientMap[currEnemy].player->getRect()->getGlobalBounds().width <<
				clientMap[currEnemy].player->getRect()->getGlobalBounds().height << clientMap[currEnemy].player->getScore();

			clientMap[vectPosition].socket->send(enemyPacket);
		}
	}
	//threadLock.unlock();
}



int NetworkManager::readPacket(sf::Packet packet)
{
	return 0;
}


void NetworkManager::addUser(Users* user)
{

}

static int callback(void *data, int argc, char **argv, char **azColName){

	// fprintf(stderr, "%s: ", (const char*)data);
	std::cout<< "here" << std::endl;

	printf("\n");
	return 0;
}

bool NetworkManager::login(std::string currPlayerName , std::string currPassword)
{

	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	char *sql;
	const char* data = "Callback function called";

	rc = sqlite3_open("testdb.db", &db);
	if( rc ){
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		exit(0);
	}else{
		fprintf(stderr, "Opened database successfully\n");
	}


	// Create SQL statement 
	sqlite3_stmt *statement;
	sql = "SELECT * from USERS";

	// Execute SQL statement 

	if(sqlite3_prepare(db,sql,-1,&statement,0) == SQLITE_OK)
	{
		int ctotal = sqlite3_column_count(statement);
		int res = 0;

		while(1)
		{
			res = sqlite3_step(statement);

			if(res == SQLITE_ROW)
			{

				std::string s1 = (char*)sqlite3_column_text(statement,0);
				std::string s2 = (char*)sqlite3_column_text(statement,1);
				Users* user = new Users(s1,s2);
				usersList.push_front(user);
			}

			if (res == SQLITE_DONE || res == SQLITE_ERROR)
			{
				std::cout << "done" << std::endl;
				break;
			}
		}
	}

	if( rc != SQLITE_OK ){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}else{
		fprintf(stdout, "Operation done successfully\n");
	}
	//check againts existing entries
	for (auto it = usersList.begin(); it != usersList.end(); it++) {
		if((**it).myusername == currPlayerName && (**it).mypassword == currPassword)
		{
			std::cout << "Log in success" << std::endl;
			return true;

		}else if((**it).myusername == currPlayerName && (**it).mypassword != currPassword)
		{
			std::cout << " Log in Fail \n";
			return false;
		}
	}
	//if not in existing entries then add
	// std::ostringstream s;
	// s << "INSERT INTO USERS VALUES (\'" << currPlayerName<< "\' , \'" << currPassword << "\');";
	//sql = "INSERT INTO USERS VALUES (" + currPlayerName + "," + currPassword + ");" ;

	std::string insertQuery = "INSERT INTO USERS VALUES ('" + currPlayerName + "','" + currPassword + "');";

	std::cout<< insertQuery<<std::endl;
	const char* sql2 = insertQuery.c_str();
	rc = sqlite3_exec(db,sql2,callback,0,&zErrMsg);
	if( rc != SQLITE_OK ){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}else{
		fprintf(stdout, "Records created successfully\n");
		return true;
	}
	sqlite3_close(db);

	return false;
}

