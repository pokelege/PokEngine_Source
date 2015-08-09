#include <Input\XBox360XInput.h>
#include <Windows.h>
#include <Xinput.h>
bool XBox360XInput::isDown( unsigned short key , unsigned int player )
{
	XINPUT_STATE state;
	ZeroMemory( &state , sizeof( XINPUT_STATE ) );
	DWORD result = XInputGetState( player , &state );
	if ( result == ERROR_SUCCESS && (state.Gamepad.wButtons & key) == key )
	{
		return true;
	}
	else return false;
}

float XBox360XInput::leftTrigger( unsigned int player )
{
	XINPUT_STATE state;
	ZeroMemory( &state , sizeof( XINPUT_STATE ) );
	DWORD result = XInputGetState( player , &state );
	if ( result == ERROR_SUCCESS )
	{
		return ((float)state.Gamepad.bLeftTrigger) / 255.0f;
	}
	else return 0;
}
float XBox360XInput::rightTrigger( unsigned int player )
{
	XINPUT_STATE state;
	ZeroMemory( &state , sizeof( XINPUT_STATE ) );
	DWORD result = XInputGetState( player , &state );
	if ( result == ERROR_SUCCESS )
	{
		return (( float ) state.Gamepad.bRightTrigger) / 255.0f;
	}
	else return 0;
}
glm::vec2 XBox360XInput::leftStick( unsigned int player )
{
	XINPUT_STATE state;
	ZeroMemory( &state , sizeof( XINPUT_STATE ) );
	DWORD result = XInputGetState( player , &state );
	if ( result == ERROR_SUCCESS )
	{
		return glm::vec2(state.Gamepad.sThumbLX, state.Gamepad.sThumbLY);
	}
	else return glm::vec2();
}
glm::vec2 XBox360XInput::rightStick( unsigned int player )
{
	XINPUT_STATE state;
	ZeroMemory( &state , sizeof( XINPUT_STATE ) );
	DWORD result = XInputGetState( player , &state );
	if ( result == ERROR_SUCCESS )
	{
		return glm::vec2( state.Gamepad.sThumbLX , state.Gamepad.sThumbLY );
	}
	else return glm::vec2();
}
void XBox360XInput::setVibration( unsigned short left , unsigned short right , unsigned int player )
{
	XINPUT_VIBRATION vibration;
	vibration.wLeftMotorSpeed = left;
	vibration.wRightMotorSpeed = right;
	XInputSetState( player , &vibration );
}

bool XBox360XInput::exists( unsigned int player )
{
	XINPUT_STATE state;
	ZeroMemory( &state , sizeof( XINPUT_STATE ) );
	DWORD result = XInputGetState( player , &state );
	return result == ERROR_SUCCESS;
}

//glm::ivec2 XBox360XInput::getBattery( unsigned int player )
//{
//	XINPUT_BATTERY_INFORMATION battery;
//	ZeroMemory( &battery , sizeof( battery ) );
//	if ( XInputGetBatteryInformation( player , BATTERY_DEVTYPE_GAMEPAD , &battery ) == ERROR_SUCCESS )
//	{
//		return glm::ivec2( battery.BatteryLevel , battery.BatteryType );
//	}
//	return glm::ivec2(-1,-1);
//}