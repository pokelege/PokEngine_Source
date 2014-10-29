#include <Input\KeyInput.h>
#include <Windows.h>
bool KeyInput::isDown( unsigned int key )
{
	return GetAsyncKeyState( key ) < 0;
}
bool KeyInput::wasPressed( unsigned int key )
{
	return GetAsyncKeyState( key ) != 0;
}