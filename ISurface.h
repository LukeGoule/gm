#pragma once

#include "Interface.h"
#include "Color.h"
#include "IAppSystem.h"

enum FontDrawType_t
{
	// Use the "additive" value from the scheme file
	FONT_DRAW_DEFAULT = 0,

	// Overrides
	FONT_DRAW_NONADDITIVE,
	FONT_DRAW_ADDITIVE,

	FONT_DRAW_TYPE_COUNT = 2,
};

enum EFontFlags
{
	FONTFLAG_NONE,
	FONTFLAG_ITALIC = 0x001,
	FONTFLAG_UNDERLINE = 0x002,
	FONTFLAG_STRIKEOUT = 0x004,
	FONTFLAG_SYMBOL = 0x008,
	FONTFLAG_ANTIALIAS = 0x010,
	FONTFLAG_GAUSSIANBLUR = 0x020,
	FONTFLAG_ROTARY = 0x040,
	FONTFLAG_DROPSHADOW = 0x080,
	FONTFLAG_ADDITIVE = 0x100,
	FONTFLAG_OUTLINE = 0x200,
	FONTFLAG_CUSTOM = 0x400,
	FONTFLAG_BITMAP = 0x800,
};

struct IntRect
{
	int x0;
	int y0;
	int x1;
	int y1;
};

typedef unsigned long HFont;
typedef unsigned int VPANEL;

enum class FontDrawType {
	FONT_DRAW_DEFAULT = 0,
	FONT_DRAW_NONADDITIVE,
	FONT_DRAW_ADDITIVE,
	FONT_DRAW_TYPE_COUNT = 2,
};

class ISurface
{
public:
	void draw_set_color(Color col)
	{
		typedef void(__thiscall* original_fn)(PVOID, Color);
		return GetVFunc<original_fn>(this, 10)(this, col);
	}

	void unlock_cursor()
	{
		typedef void(__thiscall* original_fn)(PVOID);
		GetVFunc< original_fn >(this, 61)(this);
	}

	void lock_cursor()
	{
		typedef void(__thiscall* original_fn)(PVOID);
		GetVFunc< original_fn >(this, 62)(this);
	}

	void push_make_current(VPANEL panel, bool useInsets)
	{
		typedef void(__thiscall* original_fn)(PVOID, VPANEL, bool);
		return GetVFunc<original_fn>(this, 8)(this, panel, useInsets);
	}

	void pop_make_current(VPANEL panel)
	{
		typedef void(__thiscall* original_fn)(PVOID, VPANEL);
		return GetVFunc<original_fn>(this, 9)(this, panel);
	}

	void draw_outlined_rect(int x0, int y0, int x1, int y1)
	{
		typedef void(__thiscall* original_fn)(PVOID, int, int, int, int);
		GetVFunc<original_fn>(this, 14)(this, x0, y0, x1, y1);
	}

	void draw_filled_rect(int x0, int y0, int x1, int y1)
	{
		typedef void(__thiscall* original_fn)(PVOID, int, int, int, int);
		GetVFunc<original_fn>(this, 12)(this, x0, y0, x1, y1);
	}

	void surface_get_cursor_pos(int &x, int &y)
	{
		typedef void(__thiscall* original_fn)(PVOID, int&, int&);
		GetVFunc<original_fn>(this, 97)(this, x, y);
	}

	void draw_outlined_circle(int x, int y, int radius, int segments)
	{
		typedef void(__thiscall* original_fn)(PVOID, int, int, int, int);
		GetVFunc<original_fn>(this, 100)(this, x, y, radius, segments);
	}

	unsigned long create_font()
	{
		typedef unsigned int(__thiscall* original_fn)(PVOID);
		return GetVFunc<original_fn>(this, 66)(this);
	}

	void set_font_glyph_set(unsigned long font, const char* windowsFontName, int tall, int weight, int blur, int scanlines, int flags)
	{
		typedef void(__thiscall* original_fn)(PVOID, unsigned long, const char*, int, int, int, int, int, int, int);
		GetVFunc<original_fn>(this, 67)(this, font, windowsFontName, tall, weight, blur, scanlines, flags, 0, 0);
	}

	void draw_line(int x0, int y0, int x1, int y1)
	{
		typedef void(__thiscall* original_fn)(PVOID, int, int, int, int);
		GetVFunc<original_fn>(this, 15)(this, x0, y0, x1, y1);
	}

	void draw_set_text_font(unsigned long font)
	{
		typedef void(__thiscall* original_fn)(PVOID, unsigned long);
		GetVFunc<original_fn>(this, 17)(this, font); // 21 mb
	}

	void draw_set_text_color(int r, int g, int b, int a)
	{
		typedef void(__thiscall* original_fn)(PVOID, int, int, int, int);
		GetVFunc<original_fn>(this, 19)(this, r, g, b, a); // 19
	}

	void draw_set_text_color(Color col)
	{
		typedef void(__thiscall* original_fn)(PVOID, Color);
		return GetVFunc<original_fn>(this, 18)(this, col);
	}

	void draw_set_text_pos(int x, int y) // maybe
	{
		typedef void(__thiscall* original_fn)(PVOID, int, int);
		GetVFunc<original_fn>(this, 20)(this, x, y);
	}

	void get_text_size(HFont font, const wchar_t *text, int &wide, int &tall)
	{
		typedef void(__thiscall* original_fn)(PVOID, HFont, const wchar_t*, int&, int&);
		GetVFunc<original_fn>(this, 76)(this, font, text, wide, tall);
	}

	void draw_print_text(const wchar_t* text, int textLen)
	{
		typedef void(__thiscall* original_fn)(PVOID, const wchar_t*, int, int);
		GetVFunc<original_fn>(this, 22)(this, text, textLen, 0);
	}
};