#pragma once
#include <SDL_pixels.h>
bool operator==(const SDL_Color& l, const SDL_Color& r)
{
	return l.a == r.a &&
		l.b == r.b &&
		l.g == r.g &&
		l.r == r.r;
}

bool operator!=(const SDL_Color& l, const SDL_Color& r)
{
	return !operator==(l, r);
}
