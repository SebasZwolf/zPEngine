#pragma once

#ifndef __particle_manager__
#define __particle_manager__

#include "fnc.h"

class ParticleManager : fnc::iSing<ParticleManager> {
public:
	static ParticleManager* Instance() {
		return  fnc::iSing<ParticleManager>::Instance();
	}
	static void Release() {
		return  fnc::iSing<ParticleManager>::Release();
	}
private:
	ParticleManager() = default;
	~ParticleManager() = default;

	friend class  fnc::iSing<ParticleManager>;
};


#endif