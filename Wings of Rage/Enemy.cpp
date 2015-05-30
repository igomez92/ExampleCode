#include "Enemy.h"
#include "TextureManager.h"

Enemy::Enemy(std::string file, sf::Vector2f pos, int health)
	:pos(pos), health(health), collisiondmg(10)
{
	sf::Texture* image = _getTexture(file);
	image->setSmooth(true);
	sprite.setTexture(*image);
	sprite.setPosition(pos);
	sprite.setOrigin(sprite.getLocalBounds().width/2, sprite.getLocalBounds().height/2);
	sprite.rotate(180);
	sprite.setScale(1.5,1.5);
	type = Normal;
}


Enemy::~Enemy(void)
{

}

void Enemy::update(float deltaTime, std::list<Bullet*>& eList, std::list<Bullet*>& pList, Player& player)
{
	pos.y += 100 * deltaTime;
	sprite.setPosition(pos);
	if(pos.y > SCREEN_HEIGHT + sprite.getLocalBounds().height + 200)
	{
		health = 0;
	}

	if (bulletClock.getElapsedTime().asSeconds() > 0.5)
	{
		eList.push_back(new Bullet("media/bullet.png", pos, sf::Vector2f(0, 400), 10, sf::Color(255, 50, 50)));
		bulletClock.restart();
	}
}

void Enemy::draw(sf::RenderWindow& window)
{
	window.draw(sprite);
}

void Enemy::takeDam(int dam)
{
	health -= dam;
}

bool Enemy::isDead()
{
	if(health <= 0)
		return true;
	
	return false;
}

sf::FloatRect Enemy::getSize()
{
	return sprite.getLocalBounds();
}
int Enemy::getHealth()
{
	return health;
}

int Enemy::collisiondamage()
{
	return collisiondmg;
}