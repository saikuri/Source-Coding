#pragma once

#include <HAPI_lib.h>

using namespace HAPISPACE;

class Rec; // Forward declaration of the rec class.
struct Screen; // Forward declaration of the screen class.

class Image
{
public:
	Image(std::string const& filename);
	~Image();
	
	//If any pixel data exists, delete pixel data.
	void Blit(Screen& imgscreen, int x = 0, int y = 0);
	void TransparentBlit(Screen& imgscreen, int x = 0, int y = 0); //Set a default parameter inside a header file.
	void ClipBlit(Screen& imgscreen, const Rec &destRec, int x = 0, int y = 0, int frameNum = 0);
	void ClipSpriteSheet(Screen& imgscreen, const Rec &destRec, int x = 0, int y = 0, int frameNum = 0);
	inline int GetWidth() const { return width; } // Function inlined due to not needing a body within the cpp file. Returns image width.
	inline int GetHeight() const { return height; } // Function inlined due to not needing a body within the cpp file. Returns image height.
private:
	BYTE* pixdata{ nullptr };
	unsigned int width{ 0 };
	unsigned int height{ 0 };
	bool hasTransparency; // Check to see if image has transparency or not.
};

