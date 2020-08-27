#include "InputManager.h"
#include "../internal/GlobalSettings.h"

#include <SDL.h>
#include <iostream>
#include <cstdio>

#define Testing

InputManager * InputManager::sInstance = nullptr;

InputManager * InputManager::Instance(){
	if (sInstance == nullptr) sInstance = new InputManager();
	return sInstance;
}

void InputManager::Release(){
	if (sInstance == nullptr) return;
	delete sInstance;
	sInstance = nullptr;
}

void InputManager::update(){
	mouse.check_button._buttons = SDL_GetMouseState(&mouse.x, &mouse.y);
	if(sngGlobalSettings[GlobalSettings::CAPTURE_WHEN_DRAG])
		SDL_CaptureMouse(mouse.check_button(mouse.left) ? SDL_TRUE : SDL_FALSE );
}

void InputManager::postupdate(){
	fnc::usmall &asize = keyboard_check.actual_size;

	bool val = 0x00;
	keyboard_check._mapprevkeys[asize - 1] = 0x00;
	for (unsigned short i = 0; i < asize * 8; i++) {
		if (keyboard_check.keynumber < i) break;

		val = (keyboard_check._mapkeys[ keyboard_check.keynumber - i -1] != 0);

		for (unsigned char ii = 0; ii < asize; ii++) {
			bool jmp = (ii > 0) && keyboard_check._mapprevkeys[asize - ii - 1] >> 7 & 0x01;

			keyboard_check._mapprevkeys[asize - 1 - ii] = keyboard_check._mapprevkeys[asize - 1 - ii] << 1;

			if (jmp)
				keyboard_check._mapprevkeys[asize - ii] = keyboard_check._mapprevkeys[asize - ii] | 0x01;
		}
		   
		keyboard_check._mapprevkeys[0] = keyboard_check._mapprevkeys[0] | val;
	}

	mouse.check_button._prevbuttons = mouse.check_button._buttons;
}

InputManager::InputManager(){

	keyboard_check._mapkeys = SDL_GetKeyboardState(&keyboard_check.keynumber);
	keyboard_check.actual_size = static_cast<fnc::usmall>(std::ceil(static_cast<float>(keyboard_check.keynumber) / 8));

	fnc::usmall &asize = keyboard_check.actual_size;
	keyboard_check._mapprevkeys = new fnc::usmall[asize];
	
	for (unsigned short i = 0; i < asize ; i++)
		keyboard_check._mapprevkeys[i] = 0x00;

#ifdef Testing
	SDL_Scancode a;
	printf("size of scancode %u!\n", sizeof(a));
	printf("InputMannager %s\n", "initalized");
#endif
}

InputManager::~InputManager(){
	delete[] keyboard_check._mapprevkeys;

#ifdef	Testing
	printf("InputMannager %s\n", "destroyed");
#endif
}

bool InputManager::_keyboard::operator()(keyCode code){
	return _mapkeys[code];
}

bool InputManager::_keyboard::pressed(keyCode code){
	return _mapkeys[code] && !(_mapprevkeys[ code / 8] & static_cast<fnc::usmall>(std::pow(2, code % 8)));
}

bool InputManager::_keyboard::released(keyCode code){
	return !_mapkeys[code] && (_mapprevkeys[ code / 8] & static_cast<fnc::usmall>(std::pow(2, code % 8)));
}

bool InputManager::_mouse::_check::operator()(fnc::usmall code) const{
	return (_buttons & code);
}

bool InputManager::_mouse::_check::pressed(fnc::usmall code) const{
	return (_buttons & code) && !(_prevbuttons & code);
}

bool InputManager::_mouse::_check::released(fnc::usmall code) const{
	return !(_buttons & code) && (_prevbuttons & code);
}

const fig::Vector2D InputManager::_mouse::get_pos() const{
	return fig::Vector2D(static_cast<float>(x), static_cast<float>(y));
}

keyCode key::ord(char v) {
	if (v < '0' || v > '9') {
		if (v < 'a' || v >'z') return 0;
		else
			return (v - 'a' + 4);
	}
	else
		return v == '0' ? 39 : (v - '1' + 30);
}
