#pragma once

#ifndef __particle_manager__
#define __particle_manager__

#include "fnc.h"
#include <array>
#include <vector>
#include <memory>

struct SDL_Renderer;
using part_system = unsigned int;

class ParticleManager : fnc::iSing<ParticleManager> {
public:
	static ParticleManager* Instance() { return  fnc::iSing<ParticleManager>::Instance(); }
	static void Release() { return  fnc::iSing<ParticleManager>::Release(); }

	void step();
	void draw();

	SDL_Renderer* renderer;

	void Create();

	void set_velocity(fig::Point<float> vel, fig::Point<float> vel_variation);
	void set_lifetime(fnc::ushort life, fnc::ushort life_variation);
	void set_colors(vis::Color birth_color, vis::Color death_color);


	part_system Save();
	void Discard();

	struct ParticleSystem {
		bool active = false;

		fnc::ushort index = 499;

		fig::Point<float> vel = { 0,0 }, varvel = {0,0};
		vis::Color colorStart = { 0xff,0xff,0xff }, colorEnd = {0xaa,0xaa,0xaa};
		fnc::ushort life = 60, varlife = 6;
	
		struct Particle{
			bool active = false;

			fig::Point<float> vel;
			fig::Point<float> pos;
			fnc::ushort life;
		};

		fnc::ushort particle_count = 0;
		std::array<Particle, 500> particle_list;

		ParticleSystem() = default;
		ParticleSystem(const ParticleSystem&)	= delete;
		ParticleSystem(ParticleSystem&&)		= delete;
	};

	void burst(part_system ps, fig::Point<short> pos);

private:

	std::vector<std::unique_ptr<ParticleSystem>> particle_system_list;
	std::unique_ptr<ParticleSystem> tmp_particle_system;

	ParticleManager();
	~ParticleManager();

	friend class  fnc::iSing<ParticleManager>;
};


#endif