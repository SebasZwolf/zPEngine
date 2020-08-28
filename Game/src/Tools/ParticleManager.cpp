#include "ParticleManager.h"

#include <SDL_image.h>

#include "internal/Graphics.h"

ParticleManager* ParticleManager::sInstance = NULL;

ParticleManager::ParticleManager() {
	renderer = Graphics::Instance()->ask();
	printf("Particle Manager initialized!\n");
}

ParticleManager::~ParticleManager() {
	particle_system_list.clear();

	printf("Particle Manager destroyed!\n");
}

void ParticleManager::step() {
	for (auto &a : particle_system_list) {
		if (!a->active) continue;

		for (auto& p : a->particle_list) {
			if (!p.active) continue;

			p.life--;
			p.pos.x += p.vel.x;
			p.pos.y += p.vel.y;

			if (p.life <= 10) {
				p.active = false;
				a->particle_count--;
			}
		}

		if (a->particle_count == 0) a->active = false;
	}
}

void ParticleManager::draw() {
	for (auto &a : particle_system_list) {
		if (!a->active) continue;
		for (auto& p : a->particle_list) {
			if (!p.active) continue;
			
			float prs = static_cast<float>(p.life) /static_cast<float>( a->life + a->varlife);

			SDL_Rect dst{
				p.pos.x - 20 * prs,
				p.pos.y - 20 * prs,
				40 * prs,
				40 * prs
			};

			SDL_SetRenderDrawColor(
				renderer,
				fnc::lerp(a->colorStart.r, a->colorEnd.r, prs),
				fnc::lerp(a->colorStart.g, a->colorEnd.g, prs),
				fnc::lerp(a->colorStart.b, a->colorEnd.b, prs),
				255 * prs);

			SDL_RenderFillRect(renderer, &dst);
		}
	}
}

void ParticleManager::Create() {
	if (tmp_particle_system) { printf("There's still an unborn particle system in factory!\n"); return; }
	tmp_particle_system = std::make_unique<ParticleSystem>();
}

void ParticleManager::set_velocity(fig::Point<float> vel, fig::Point<float> vel_variation) {
	tmp_particle_system->vel = vel;
	tmp_particle_system->varvel = vel_variation;
}
void ParticleManager::set_lifetime(fnc::ushort life, fnc::ushort life_variation) {
	tmp_particle_system->life = life;
	tmp_particle_system->varlife = life_variation;
}
void ParticleManager::set_colors(vis::Color birth_color, vis::Color death_color) {
	tmp_particle_system->colorStart = birth_color;
	tmp_particle_system->colorEnd = death_color;
}

part_system ParticleManager::Save() {
	particle_system_list.emplace_back(std::move(tmp_particle_system));
	return particle_system_list.size() - 1;
}

void ParticleManager::Discard() {
	tmp_particle_system.reset();
	return;
}

void ParticleManager::burst(part_system ps_index, fig::Point<short> pos) {
	if (particle_system_list[ps_index]->particle_count < particle_system_list[ps_index]->particle_list.size()) {
		auto &ps = *particle_system_list[ps_index];

		/*while (ps.particle_list[ps.particle_list.size() - ps.index - 1].active) {
			ps.index = (++ps.index % ps.particle_list.size());
		}*/
		
		
		auto &p = ps.particle_list[ps.particle_list.size() - ps.index - 1];

		p.active = true;
		p.pos.x = pos.x;
		p.pos.y = pos.y;
		
		p.vel = ps.vel;
		p.vel.x += (ps.varvel.x * (static_cast<float>(rand() % 101) / 100.0f - .5f));
		p.vel.y += (ps.varvel.y * (static_cast<float>(rand() % 101) / 100.0f - .5f));

		p.life = ps.life + (static_cast<float>(ps.varlife) * static_cast<float>(rand() % 101) / 100.0f);

		ps.active = true;
		ps.particle_count++;


		ps.index = ++ps.index % ps.particle_list.size();
	}
}