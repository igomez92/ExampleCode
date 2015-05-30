#pragma once

#include "Utility.h"
#include "Particle.h"
#include <list>
#include <SFML\Graphics.hpp>

class ParticleManager
{
public:
	ParticleManager();
	~ParticleManager();
	void doExplosion(std::list<Particle*>& allParticles, sf::Vector2f pos, sf::Color particleColor, float lifeExpectancy);
	void doHitParticle(std::list<Particle*>& allParticles, sf::Vector2f pos, sf::Vector2f vel, sf::Color particleColor, float lifeExpectancy);
	void doEnginePartcle(std::list<Particle*>& allParticles, sf::Vector2f pos, sf::Color particleColor, float lifeExpectancy);
	void doLaserParticleEffect(std::list<Particle*>& allParticles, sf::Vector2f pos, sf::Vector2f vel, sf::Color particleColor, float lifeExpectancy);
	void doPlasmaSwordParticleEffect(std::list<Particle*> allParticles, sf::Vector2f pos, sf::Vector2f vel, sf::Color particleColor, float lifeExpectancy);
	void doSpiralParticleEffect(std::list<Particle*> allParticles, sf::Vector2f pos, sf::Color particleColor, float lifeExpectancy);
	void doGunnerShotParticleEffect(std::list<Particle*> allParticles, sf::Vector2f pos, float lifeExpectancy);
};