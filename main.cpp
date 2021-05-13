#include <iostream>
#include "bitmap.h"
#include <cstdlib>


int main(int argc, char **argv)
{
	int width = 800;
	int height = 600;
    Bitmap bitmap(width, height);

    for (int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            bitmap.SetColor(Color((float)x/(float)width,
                                  1.0f - (float)x/(float)width,
                                  1.0f - (float)y/(float)height),
                            x,
                            y);
        }
    }
    bitmap.Export("photo.bmp");



    return 0;
}
