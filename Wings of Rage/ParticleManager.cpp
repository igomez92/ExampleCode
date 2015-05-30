#pragma once
#include "ParticleManager.h"


ParticleManager::ParticleManager()
{

}

ParticleManager::~ParticleManager()
{

}

void ParticleManager::doExplosion(std::list<Particle*>& allParticles, sf::Vector2f pos, sf::Color particleColor, float lifeExpectancy)
{
	allParticles.push_back(new Particle("media/particle.png", pos, sf::Vector2f (0,-600), particleColor, lifeExpectancy));
	allParticles.push_back(new Particle("media/particle.png", pos, sf::Vector2f(0, 600), particleColor,lifeExpectancy));
	allParticles.push_back(new Particle("media/particle.png", pos, sf::Vector2f(600,0), particleColor,lifeExpectancy));
	allParticles.push_back(new Particle("media/particle.png", pos, sf::Vector2f(-600, 0), particleColor,lifeExpectancy));
	allParticles.push_back(new Particle("media/particle.png", pos, sf::Vector2f(600,600), particleColor,lifeExpectancy));
	allParticles.push_back(new Particle("media/particle.png", pos, sf::Vector2f(600,-600), particleColor,lifeExpectancy));
	allParticles.push_back(new Particle("media/particle.png", pos, sf::Vector2f(-600,600), particleColor,lifeExpectancy));
	allParticles.push_back(new Particle("media/particle.png", pos, sf::Vector2f(-600,-600), particleColor,lifeExpectancy));
}

void ParticleManager::doHitParticle(std::list<Particle*>& allParticles, sf::Vector2f pos, sf::Vector2f vel, sf::Color particleColor, float lifeExpectancy)
{
	allParticles.push_back(new Particle("media/particle.png", pos, sf::Vector2f(vel.x,vel.y), particleColor, lifeExpectancy));
}

void ParticleManager::doEnginePartcle(std::list<Particle*>& allParticles, sf::Vector2f pos, sf::Color particleColor, float lifeExpectancy)
{
	allParticles.push_back(new Particle("media/particle.png", pos, sf::Vector2f(rand() % 70 - 35, rand() % 400 + 30), particleColor, lifeExpectancy)); 
	allParticles.push_back(new Particle("media/particle.png", pos, sf::Vector2f(rand() % 70 - 35, rand() % 400 + 30), sf::Color::Color(0, 255, 108, 255), lifeExpectancy)); 
	allParticles.push_back(new Particle("media/particle.png", pos, sf::Vector2f(rand() % 70 - 35, rand() % 400 + 30), sf::Color::Color(0, 106, 255, 255), lifeExpectancy)); 

}

void ParticleManager::doLaserParticleEffect(std::list<Particle*>& allParticles, sf::Vector2f pos, sf::Vector2f vel, sf::Color particleColor, float lifeExpectancy)
{
	for(sf::Vector2f spacingDifference = pos; (spacingDifference.x < SCREEN_WIDTH && spacingDifference.x > 0) 
		&& (spacingDifference.y < SCREEN_HEIGHT && spacingDifference.y > 0); spacingDifference.x++ && spacingDifference.y++)
	{
		allParticles.push_back(new Particle("media/particle.png", spacingDifference, vel, particleColor, lifeExpectancy)); 
	}
}

/*TODO later*/

void ParticleManager::doPlasmaSwordParticleEffect(std::list<Particle*> allParticles, sf::Vector2f pos, sf::Vector2f vel, sf::Color particleColor, float lifeExpectancy)
{

}

void ParticleManager::doSpiralParticleEffect(std::list<Particle*> allParticles, sf::Vector2f pos, sf::Color particleColor, float lifeExpectancy)
{

}

void ParticleManager::doGunnerShotParticleEffect(std::list<Particle*> allParticles, sf::Vector2f pos, float lifeExpectancy)
{

}


