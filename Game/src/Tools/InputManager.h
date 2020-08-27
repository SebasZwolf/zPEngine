#pragma once
#ifndef __input_mannager__
#define __input_mannager__

#include "fnc.h"

typedef fnc::ushort keyCode;

namespace key {
	//Should avoid writing the character in mayus!
	keyCode ord(char v);

	enum : keyCode {
		vk_return = 40,//Same as vk_enter in GML
		vk_escape, vk_backspace, vk_tab, vk_space,
		ng_comma = 54, ng_period, ng_slash,
		ng_capsLock = 57,
		vk_f1 = 58, vk_f2, vk_f3, vk_f4, vk_f5, vk_f6, vk_f7, vk_f8, vk_f9, vk_f10, vk_f11, vk_f12,
		vk_printScreen = 70, ng_scrollLock, vk_pause, vk_insert,
		vk_home = 74, vk_pageUp, vk_delete, vk_end, vk_pageDown, vk_right, vk_left, vk_down, vk_up,
		vk_numLock = 83,
		vk_divide = 84, vk_multiply, vk_substract, vk_add, ng_enter, vk_num0, vk_num1, vk_num2, vk_num3, vk_num4, vk_num5, vk_num6, vk_num7, vk_num8, vk_num9, vk_decimal,
		vk_lcontrol = 224, vk_lshift, vk_lalt, vk_rwindows,
		vk_rcontrol = 228, vk_rshift, vk_ralt, vk_lwindows
		//sp_control, sp_shift, sp_alt, sp_windows
	};
}

class InputManager{
private:
	static InputManager* sInstance;
	
public:
	static InputManager* Instance();
	static void Release();

	struct _keyboard{
	public:
		bool operator()(keyCode code);
		bool pressed(keyCode code);
		bool released(keyCode code);
	private:
		const fnc::usmall *_mapkeys;
		fnc::usmall *_mapprevkeys;
		int keynumber;
		fnc::usmall actual_size;

		friend class InputManager;
	} keyboard_check;

	struct 	_mouse{
	public:
		enum button : fnc::usmall{
			left = 1, middle = 2, right = 4, extra1 = 8, extra2 = 16
		};
		const fig::Vector2D get_pos() const;
		struct _check{
			bool operator()(fnc::usmall) const;
			bool pressed(fnc::usmall) const;
			bool released(fnc::usmall) const;
		private:
			fnc::ushort _buttons;
			fnc::ushort _prevbuttons;

			friend class InputManager;
		}check_button;
	private:
		int x;
		int y;
		friend class InputManager;
	}mouse;

	void update();
	void postupdate();

private:
	InputManager();
	~InputManager();
};

#define sngInput (*InputManager::Instance())
#endif // __input_mannager__


