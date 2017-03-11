#include "Image.h"
#include "Screen.h"
#include "Rec.h"

// Error catching if file is missing from location.
Image::Image(std::string const& filename)
{
	if (!HAPI.LoadTexture(filename, &pixdata, width, height))
	{
		HAPI_UserResponse response;
		HAPI.UserMessage("Do you want to exit?", "Exit?", HAPI_ButtonType::eButtonTypeYesNo, &response);

		if (response == HAPI_UserResponse::eUserResponseYes)
		{
			HAPI.Close();
		}
	}

	//Checking to find out if any of the alpha values are 255.
	for (unsigned int i = 0; i < height * width * 4; i += 4) //Goes over entire image.
	{
		if (pixdata[i + 3] != 255)
		{
			hasTransparency = true;
			break;
		}

	}
}

Image::~Image() 
{
	if (pixdata) delete[] pixdata;
}
// Blits image with transparency.
void Image::TransparentBlit(Screen& imgscreen, int x, int y)
{
	BYTE *destinPnter =  imgscreen.pixscreen + (x + y * imgscreen.screenWidth) * 4;
	BYTE *sourcePnter = pixdata;

	for (unsigned int y = 0; y < height; y++)
	{
		for (unsigned int x = 0; x < width; x++)
		{
			BYTE alpha = sourcePnter[3];
			
			// Alpha checking.
			if (alpha > 0)
			{
				if (alpha == 255)
				{
					memcpy(destinPnter, sourcePnter, 4);
				}
				else
				{
					destinPnter[0] = destinPnter[0] + ((alpha *(sourcePnter[0] - destinPnter[0])) >> 8);
					destinPnter[1] = destinPnter[1] + ((alpha *(sourcePnter[1] - destinPnter[1])) >> 8);
					destinPnter[2] = destinPnter[2] + ((alpha *(sourcePnter[2] - destinPnter[2])) >> 8);
				}
			}

			destinPnter += 4;
			sourcePnter += 4;
		}

		destinPnter += (imgscreen.screenWidth - width) * 4;
	}
}
// Clips an image on and off screen.
void Image::ClipBlit(Screen & imgscreen, const Rec & destRec, int x, int y, int frameNum)
{
	Rec clippedRec(0, width, 0, height); // Creating a new rectangle.

	clippedRec.Translate(x, y); // Translating x and y co-ordinate.

	// Check to see if new rectangle is out of screen.
	if (clippedRec.OutsideOf(destRec))
	{
		return;
	}

	clippedRec.ClipTo(destRec); // Clip to rectangle destination.
	clippedRec.Translate(-x, -y); // Translating the negative x and y co-ordinate.
	//clippedRec.Translate((width / 12) * frameNum, 0); // Translating width by frame number.

	// Set x to 0 if the value it holds is less then 0.
	if (x < 0)
	{
		x = 0;
	}

	// Set y to 0 if the value it holds is less then 0.
	if (y < 0)
	{
		y = 0;
	}

	BYTE *destinPnter = imgscreen.pixscreen + (x + y * destRec.width()) * 4;
	BYTE *sourcePnter = pixdata + (clippedRec.left + clippedRec.top * width) * 4;

	for (int y = 0; y < clippedRec.height(); y++)
	{
		for (int x = 0; x < clippedRec.width(); x++)
		{
			BYTE alpha = sourcePnter[3];

			// Alpha checking for clipped image.
			if (alpha > 0)
			{
				if (alpha == 255)
				{
					memcpy(destinPnter, sourcePnter, 4);
				}
				else
				{
					destinPnter[0] = destinPnter[0] + ((alpha *(sourcePnter[0] - destinPnter[0])) >> 8);
					destinPnter[1] = destinPnter[1] + ((alpha *(sourcePnter[1] - destinPnter[1])) >> 8);
					destinPnter[2] = destinPnter[2] + ((alpha *(sourcePnter[2] - destinPnter[2])) >> 8);
				}
			}
			destinPnter += 4;
			sourcePnter += 4;
		}
		destinPnter += (destRec.width() - clippedRec.width()) * 4;
		sourcePnter += (width - clippedRec.width()) * 4;
	}
}

// Clips an image on and off screen.
void Image::ClipSpriteSheet(Screen & imgscreen, const Rec & destRec, int x, int y, int frameNum)
{
	Rec clippedRec(0, width/3, 0, height); // Creating a new rectangle.

	clippedRec.Translate(x, y); // Translating x and y co-ordinate.

								// Check to see if new rectangle is out of screen.
	if (clippedRec.OutsideOf(destRec))
	{
		return;
	}

	clippedRec.ClipTo(destRec); // Clip to rectangle destination.
	clippedRec.Translate(-x, -y); // Translating the negative x and y co-ordinate.
								  // Set x to 0 if the value it holds is less then 0.
	if (x < 0)
	{
		x = 0;
	}

	// Set y to 0 if the value it holds is less then 0.
	if (y < 0)
	{
		y = 0;
	}

	clippedRec.Translate((width/3) * frameNum, 0); // Translating width by frame number.

	

	BYTE *destinPnter = imgscreen.pixscreen + (x + y * destRec.width()) * 4;
	BYTE *sourcePnter = pixdata + (clippedRec.left + clippedRec.top * width) * 4;

	for (int y = 0; y < clippedRec.height(); y++)
	{
		for (int x = 0; x < clippedRec.width(); x++)
		{
			BYTE alpha = sourcePnter[3];

			// Alpha checking for clipped image.
			if (alpha > 0)
			{
				if (alpha == 255)
				{
					memcpy(destinPnter, sourcePnter, 4);
				}
				else
				{
					destinPnter[0] = destinPnter[0] + ((alpha *(sourcePnter[0] - destinPnter[0])) >> 8);
					destinPnter[1] = destinPnter[1] + ((alpha *(sourcePnter[1] - destinPnter[1])) >> 8);
					destinPnter[2] = destinPnter[2] + ((alpha *(sourcePnter[2] - destinPnter[2])) >> 8);
				}
			}
			destinPnter += 4;
			sourcePnter += 4;
		}
		destinPnter += (destRec.width() - clippedRec.width()) * 4;
		sourcePnter += (width - clippedRec.width()) * 4;
	}
}

// Blits an image, also checks if image has transparency.
void Image::Blit(Screen& imgscreen, int x, int y)
{
	// Checks to see if image has transparency otherwise blits as intended.
	if (hasTransparency)
	{
		TransparentBlit(imgscreen, x, y);
		return;
	}
	
	
	BYTE *destinPnter = imgscreen.pixscreen + (x + y * imgscreen.screenWidth) * 4;
	BYTE *sourcePnter = pixdata;

	for (unsigned int y = 0; y < height; y++)
	{
		memcpy(destinPnter, sourcePnter, width * 4);

		sourcePnter += width * 4;
		destinPnter += imgscreen.screenWidth * 4;
	}
}