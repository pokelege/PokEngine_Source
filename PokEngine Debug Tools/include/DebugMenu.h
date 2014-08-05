#pragma once
#pragma warning (disable : 4127)
#pragma warning (disable : 4251)

#ifdef DEBUGMENU

#include <PokEngineDebugExportHeader.h>
#include <string>
#include <Misc\ArrayList.h>
#include <Windows.h>

class POKENGINEDEBUGTOOLS_SHARED DebugMenu
{
	struct POKENGINEDEBUGTOOLS_SHARED ElementInfo
	{
		std::string label;
		float location;
		void updateLocation( float& location );
		virtual void copyFromLocalValue() = 0;
		virtual void copyToLocalValue() = 0;
		virtual void draw() = 0;
		virtual void change( float& x ) = 0;
	};

	struct POKENGINEDEBUGTOOLS_SHARED SliderInfo : public ElementInfo
	{
		float min;
		float max;
		float percent;

		float* localValue;

		void copyFromLocalValue();
		void copyToLocalValue();
		void draw();
		void change( float& x );
	};

	struct POKENGINEDEBUGTOOLS_SHARED CheckBoxInfo : public ElementInfo
	{
		bool checked;
		bool* localValue;

		void copyFromLocalValue();
		void copyToLocalValue();
		void draw();
		void change( float& x );
	};

	struct POKENGINEDEBUGTOOLS_SHARED WatchInfo : public ElementInfo
	{
		std::string value;
		float* localValue;

		void copyFromLocalValue();
		void copyToLocalValue();
		void draw();
		void change( float & x );
	};

	static ArrayList<ElementInfo> elements;
	
	static unsigned int programID;

	static unsigned int bufferId;
	
	static unsigned int checkBoxId;
	static unsigned int checkBoxIndex;
	static unsigned int checkBoxNumIndices;
	static unsigned int checkBoxDisIndex;
	static unsigned int checkBoxDisNumIndices;

	static unsigned int lineId;
	static unsigned int lineIndex;
	static unsigned int lineNumIndecies;

	static unsigned int base;				// Base Display List For The Font Set
	static void printGL( const char* string );
public:
	static void printString( const char* string , const float& x = -1, const float& y = 0 );
	static void Initialize(HDC hdc);
	static void deleteItem( const char* label );
	static void FloatControl( const char* label , float *toDynamicChange , const float& min = 0 , const float& max = 10);
	static void BoolControl( const char* label , bool *toDynamicChange );
	static void FloatWatch( const char* label , float *toWatch );
	static void updateFromLocalValue();
	static void updateToLocalValue();
	static void updateLocation( float toScroll );
	static void drawMenu();
	static void recieveClick( float x , float y );
	static void recieveHold( float x , float y, bool toRefresh );
	static void cleanUp();
};

#define DebugMenuInitialize(hdc) DebugMenu::Initialize(hdc)
#define DebugMenuDelete(label) DebugMenu::deleteItem( label )
#define DebugMenuAddFloatControl(label, value, mini, maxi) DebugMenu::FloatControl(label, value, mini, maxi)
#define DebugMenuAddBoolControl(label, boole) DebugMenu::BoolControl(label, boole)
#define DebugMenuAddFloatWatch(label, spy) DebugMenu::FloatWatch(label, spy)
#define DebugMenuUpdateFromLocalValue DebugMenu::updateFromLocalValue()
#define DebugMenuUpdateToLocalValue DebugMenu::updateToLocalValue()
#define DebugMenuUpdateLocation(toScroll) DebugMenu::updateLocation(toScroll)
#define DebugMenuDrawMenu DebugMenu::drawMenu()
#define DebugMenuRecieveClick(x,y) DebugMenu::recieveClick(x,y)
#define DebugMenuRecieveHold(x,y, toRefresh) DebugMenu::recieveHold(x,y, toRefresh)
#define DebugMenuCleanUp DebugMenu::cleanUp()

#else
#define DebugMenuInitialize(hdc)
#define DebugMenuDelete(label)
#define DebugMenuAddFloatControl
#define DebugMenuAddBoolControl(label, boole)
#define DebugMenuAddFloatWatch(label, spy)
#define DebugMenuUpdateFromLocalValue
#define DebugMenuUpdateToLocalValue
#define DebugMenuUpdateLocation(toScroll)
#define DebugMenuDrawMenu
#define DebugMenuRecieveClick(x,y)
#define DebugMenuRecieveHold(x,y, toRefresh)
#define DebugMenuCleanUp
#endif