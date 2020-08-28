#include "GameManager.h"
#include "Tools/IManage.h"
#include "Tools/ECS/Components.h"

#include <iostream>

#include "Tools/AssetManager.h"
#include "Tools/InputManager.h"

#include "internal/Graphics.h"

#include "Tools/Master.h"
#include "Tools/ParticleManager.h"

#include "fnc.h"

class TitleComponent : public Component {
public:
	TitleComponent() {
	}
	~TitleComponent() override {
		printf("\ttittle ");
	}

	part_system ps;
	void init() {
		auto &pm = *ParticleManager::Instance();
		pm.Create();
		pm.set_colors( { 0xcc, 0x66,0x00 }, { 0xff, 0xe5,0xcc });
		pm.set_velocity({ 0,-2}, { 2, 2 });
		pm.set_lifetime(80, 0);

		ps = pm.Save();
	}

	void step() override {
		auto& master_tool = *MasterTool::Instance();
		if (sngInput.keyboard_check.pressed(key::ord('a')))
			master_tool.play.sfx("pop-5.mp3", 0);
	}
	char dr = 1, dg = 1, db = 1;
	void draw() override {
		short x = sngInput.mouse.get_pos().x;
		short y = sngInput.mouse.get_pos().y;

		ParticleManager::Instance()->burst(ps, {x,y});

		auto& draw_tool = MasterTool::Instance()->draw;

		auto& color = draw_tool.set.background_color;

		switch (rand() % 3)
		{
		case 0:
			if (color.r == 0xff || color.r == 0x00) dr *= -1;
				color.r += dr;
		break;case 1:
			if (color.g == 0xff || color.g == 0x00) dg *= -1;
				color.g += dg;
		break; case 2:
			if (color.b == 0xff || color.b == 0x00) db *= -1;
				color.b += db;
		break;
		}

		{
			draw_tool.set.color = { 0xff,0xaa,0xff };
			MasterTool::Instance()->draw.set.font = AssetManager::Instance()->font["regular.fnt"];
			draw_tool.text("test text!", 10, 10);
		}
	}
private:
};

class CustomComponent : public Component {
private:
	fnc::func<void, CustomComponent*> iA;
public:
	CustomComponent(fnc::func<void, CustomComponent*> iA) : iA(iA) {}
	~CustomComponent() override { printf("\tcustom "); }

	bac::Transform* transform;

	void init() override{
		transform = &entity->getComponent<TransformComponent>();
	};
	void step() override{
		iA(this);
	}
	void draw() override {
		MasterTool::Instance()->draw.set.font = AssetManager::Instance()->font["regular.fnt"];

		MasterTool::Instance()->draw.text("hello world!\nnow, you are supposed\nto be able to write paragraphs", transform->getPos().x, transform->getPos().y);
	}
};

class MainMenu : public Room {
	Entity* tittle;
public:
	MainMenu(IManager& g, bool persistence) : Room(g, persistence) {}

	void init() override {
		tittle = &createEntity();

		tittle->addComponent<TransformComponent>(50, 200);
		tittle->addComponent<TitleComponent>();
	}
};

int main(int argc, char * argv[]) {

	//GAME ENGINE INIT
	GameInstance Game = GameManager::Start({ "zPaperEngine", 640, 480 }, {});

	//FAILURE HANDLING
	if (!Game) {
		std::cin.get();
		return 1; 
	}

	//WINDOW CONFIGURATION
	sngGlobalSettings(GlobalSettings::BORDERED_SCREEN);
	sngGlobalSettings(GlobalSettings::CAPTURE_WHEN_DRAG);
	sngGlobalSettings(GlobalSettings::WINDOW_UNGRAB);

	auto& Assets= sngAManager;

	//FONT LOAD
	Assets.font.load("OpenSans-Regular.ttf", 20, "regular.fnt", fncFont::Blended);

	//SFX LOAD
	Assets.sfx.load("pop-5.mp3");

	//TEXTURES LOAD
	auto *texture = Assets.texture.load("items.png");

	//ANIMATED SPRITES CONFIG
	Assets.sprite.load(texture, "items.spr", 9, { 16,16 }, { 0,0 },
		[](fnc::ushort index, fncSprite& texture)->fig::Point<fnc::ushort> { return { 0, static_cast<fnc::ushort>(texture.size.y * index) };	}
	);
	Assets.sprite.load(texture, "subitems.spr", 7, { 16,16 }, { 0,0 }, 
		[](fnc::ushort index, fncSprite& texture)->fig::Point<fnc::ushort> { return { 0, static_cast<fnc::ushort>(48 + texture.size.y * index) }; }
	);

	auto& manager = *Manager::Instance();

	auto& m = *MasterTool::Instance();
	m.draw.set.background_color = { 0xaa, 0xaa, 0xff };

	//ROOM TRANSITION
	manager.gotoRoom<MainMenu>();
	Game.Run();
	
	//GAME ENGINE CLOSE
	GameManager::End();

	//std::cin.get();
	return 0;
}