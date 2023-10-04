/*
  ==============================================================================

    WindowsHooker.h
    Created: 29 Sep 2021 11:19:23am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#if JUCE_WINDOWS


#define VK_ABNT_C1    0xC1              //Abnt C1
#define VK_ABNT_C2    0xC2                //Abnt C2
#define VK_KEY_0	0x30                //('0')	0
#define VK_KEY_1	0x31                //('1')	1
#define VK_KEY_2	0x32                //('2')	2
#define VK_KEY_3	0x33                //('3')	3
#define VK_KEY_4	0x34                //('4')	4
#define VK_KEY_5	0x35                //('5')	5
#define VK_KEY_6	0x36                //('6')	6
#define VK_KEY_7	0x37                //('7')	7
#define VK_KEY_8	0x38                //('8')	8
#define VK_KEY_9	0x39                //('9')	9
#define VK_KEY_A	0x41                //('A')	A
#define VK_KEY_B	0x42                //('B')	B
#define VK_KEY_C	0x43                //('C')	C
#define VK_KEY_D	0x44                //('D')	D
#define VK_KEY_E	0x45                //('E')	E
#define VK_KEY_F	0x46                //('F')	F
#define VK_KEY_G	0x47                //('G')	G
#define VK_KEY_H	0x48                //('H')	H
#define VK_KEY_I	0x49                //('I')	I
#define VK_KEY_J	0x4A                //('J')	J
#define VK_KEY_K	0x4B                //('K')	K
#define VK_KEY_L	0x4C                //('L')	L
#define VK_KEY_M	0x4D                //('M')	M
#define VK_KEY_N	0x4E                //('N')	N
#define VK_KEY_O	0x4F                //('O')	O
#define VK_KEY_P	0x50                //('P')	P
#define VK_KEY_Q	0x51                //('Q')	Q
#define VK_KEY_R	0x52                //('R')	R
#define VK_KEY_S	0x53                //('S')	S
#define VK_KEY_T	0x54                //('T')	T
#define VK_KEY_U	0x55                //('U')	U
#define VK_KEY_V	0x56                //('V')	V
#define VK_KEY_W	0x57                //('W')	W
#define VK_KEY_X	0x58                //('X')	X
#define VK_KEY_Y	0x59                //('Y')	Y
#define VK_KEY_Z	0x5A                //('Z')	Z
#define VK__none_    0xFF               //no VK mapping

class WindowsHooker
{
public:
    juce_DeclareSingleton(WindowsHooker, true);
    WindowsHooker();
    ~WindowsHooker();

    HHOOK keyboardhook;
    MSG msg; // struct with information about all messages in our queue

    const int keyCodes[192] = { VK_ABNT_C1, VK_ABNT_C2, VK_ADD,VK_ATTN,VK_BACK,VK_CANCEL,VK_CLEAR,VK_CRSEL,VK_DECIMAL,VK_DIVIDE,VK_EREOF,VK_ESCAPE,VK_EXECUTE,VK_EXSEL,VK_ICO_CLEAR,VK_ICO_HELP,VK_KEY_0,VK_KEY_1,VK_KEY_2,VK_KEY_3,VK_KEY_4,VK_KEY_5,VK_KEY_6,VK_KEY_7,VK_KEY_8,VK_KEY_9,VK_KEY_A,VK_KEY_B,VK_KEY_C,VK_KEY_D,VK_KEY_E,VK_KEY_F,VK_KEY_G,VK_KEY_H,VK_KEY_I,VK_KEY_J,VK_KEY_K,VK_KEY_L, VK_KEY_M,VK_KEY_N,VK_KEY_O,VK_KEY_P,VK_KEY_Q,VK_KEY_R,VK_KEY_S,VK_KEY_T,VK_KEY_U,VK_KEY_V,VK_KEY_W,VK_KEY_X,VK_KEY_Y,VK_KEY_Z,VK_MULTIPLY,VK_NONAME,VK_NUMPAD0,VK_NUMPAD1,VK_NUMPAD2,VK_NUMPAD3,VK_NUMPAD4,VK_NUMPAD5,VK_NUMPAD6,VK_NUMPAD7,VK_NUMPAD8,VK_NUMPAD9,VK_OEM_1,VK_OEM_102,VK_OEM_2,VK_OEM_3,VK_OEM_4,VK_OEM_5,VK_OEM_6,VK_OEM_7,VK_OEM_8,VK_OEM_ATTN,VK_OEM_AUTO,VK_OEM_AX,VK_OEM_BACKTAB,VK_OEM_CLEAR,VK_OEM_COMMA,VK_OEM_COPY,VK_OEM_CUSEL,VK_OEM_ENLW,VK_OEM_FINISH,VK_OEM_FJ_LOYA,VK_OEM_FJ_MASSHOU,VK_OEM_FJ_ROYA,VK_OEM_FJ_TOUROKU, VK_OEM_JUMP,VK_OEM_MINUS,VK_OEM_PA1,VK_OEM_PA2,VK_OEM_PA3,VK_OEM_PERIOD,VK_OEM_PLUS,VK_OEM_RESET,VK_OEM_WSCTRL,VK_PA1,VK_PACKET,VK_PLAY,VK_PROCESSKEY,VK_RETURN,VK_SELECT,VK_SEPARATOR,VK_SPACE,VK_SUBTRACT,VK_TAB,VK_ZOOM,
VK__none_,VK_ACCEPT,VK_APPS,VK_BROWSER_BACK,VK_BROWSER_FAVORITES,VK_BROWSER_FORWARD,VK_BROWSER_HOME,VK_BROWSER_REFRESH,VK_BROWSER_SEARCH,VK_BROWSER_STOP,VK_CAPITAL,VK_CONVERT,VK_DELETE,VK_DOWN,VK_END,VK_F1,VK_F10,VK_F11,VK_F12,VK_F13,VK_F14,VK_F15,VK_F16,VK_F17,VK_F18,VK_F19,VK_F2,VK_F20,VK_F21,VK_F22,VK_F23,VK_F24,VK_F3,VK_F4,VK_F5,VK_F6,VK_F7,VK_F8,VK_F9,VK_FINAL,VK_HELP,VK_HOME,VK_ICO_00,VK_INSERT,VK_JUNJA,VK_KANA,VK_KANJI,VK_LAUNCH_APP1,VK_LAUNCH_APP2,VK_LAUNCH_MAIL,VK_LAUNCH_MEDIA_SELECT,VK_LBUTTON,VK_LCONTROL,VK_LEFT,VK_LMENU,VK_LSHIFT,VK_LWIN,VK_MBUTTON,VK_MEDIA_NEXT_TRACK,VK_MEDIA_PLAY_PAUSE,VK_MEDIA_PREV_TRACK,VK_MEDIA_STOP,VK_MODECHANGE,VK_NEXT,VK_NONCONVERT,VK_NUMLOCK,VK_OEM_FJ_JISHO,VK_PAUSE,VK_PRINT,VK_PRIOR,VK_RBUTTON,VK_RCONTROL,VK_RIGHT,VK_RMENU,VK_RSHIFT,VK_RWIN,VK_SCROLL,VK_SLEEP,VK_SNAPSHOT,VK_UP,VK_VOLUME_DOWN,VK_VOLUME_MUTE,VK_VOLUME_UP,VK_XBUTTON1,VK_XBUTTON2 };

    const String keyNames[192] = { "Abnt C1","Abnt C2", "Numpad +", "Attn", "Backspace", "Break", "Clear", "Cr Sel", "Numpad .", "Numpad /", "Er Eof", "Esc", "Execute", "Ex Sel", "IcoClr", "IcoHlp", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "Numpad *", "NoName", "Numpad 0", "Numpad 1", "Numpad 2", "Numpad 3", "Numpad 4", "Numpad 5", "Numpad 6", "Numpad 7", "Numpad 8", "Numpad 9", "OEM_1 (: ;)", "OEM_102 (> <)", "OEM_2 (? /)", "OEM_3", "OEM_4 ({ [)", "OEM_5 (| \\)", "OEM_6 (} ])", "OEM_7 (\" ')","OEM_8","Oem Attn","Auto","Ax","Back Tab","OemClr","OEM_COMMA (< ,)","Copy","Cu Sel","Enlw","Finish","Loya","Mashu","Roya","Touroku","Jump","OEM_MINUS (_ -)","OemPa1","OemPa2","OemPa3","OEM_PERIOD (> .)","OEM_PLUS (+ =)","Reset","WsCtrl","Pa1","Packet","Play","Process","Enter","Select","Separator","Space","Num -","Tab","Zoom",
        "no VK mapping", "Accept", "Context Menu", "Browser Back", "Browser Favorites", "Browser Forward", "Browser Home", "Browser Refresh", "Browser Search", "Browser Stop", "Caps Lock", "Convert", "Delete", "Arrow Down", "End", "F1", "F10", "F11", "F12", "F13", "F14", "F15", "F16", "F17", "F18", "F19", "F2", "F20", "F21", "F22", "F23", "F24", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "Final", "Help", "Home", "Ico00 *", "Insert", "Junja", "Kana", "Kanji", "App1", "App2", "Mail", "Media", "Left Button **", "Left Ctrl", "Arrow Left", "Left Alt", "Left Shift", "Left Win", "Middle Button **", "Next Track", "Play / Pause", "Previous Track", "Stop", "Mode Change", "Page Down", "Non Convert", "Num Lock", "Jisho", "Pause", "Print", "Page Up", "Right Button **", "Right Ctrl", "Arrow Right", "Right Alt", "Right Shift", "Right Win", "Scrol Lock", "Sleep", "Print Screen", "Arrow Up", "Volume Down", "Volume Mute", "Volume Up", "X Button 1 **", "X Button 2 **" };

    HashMap<int, String> keyMap;

    static LRESULT WINAPI keyboardCallback(int nCode, WPARAM wParam, LPARAM lParam);

    String getKeyName(int keyCode);

	class Listener
	{
	public:
		virtual ~Listener() {}
		virtual void keyChanged(int keyCode, bool pressed) {}
	};

	ListenerList<Listener> listeners;
	void addListener(Listener* newListener) { listeners.add(newListener); }
	void removeListener(Listener* listener) { listeners.remove(listener); }
};

#endif