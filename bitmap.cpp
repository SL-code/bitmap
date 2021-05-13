#include <iostream>
#include <fstream>
#include "bitmap.h"


Color::Color(): r{0}, g{0}, b{0}
{
}

Color::Color(float r, float g, float b):r{r}, g{g}, b{b}
{
}


Color::~Color()
{
}

Bitmap::Bitmap(int width, int height)
: m_width{width}, m_height{height}, m_colors(std::vector<Color>(width*height))
{
}

Bitmap::~Bitmap()
{
}


Color Bitmap::GetColor(int x, int y) const
{
    return m_colors[y * m_width + x];
}

void Bitmap::SetColor(const Color& color, int x, int y)
{
    m_colors[y * m_width + x].r = color.r;
    m_colors[y * m_width + x].g = color.g;
    m_colors[y * m_width + x].b = color.b;
}

void Bitmap::Export(const char* path)
{
    std::ofstream f;
    f.open(path, std::ios::out | std::ios::binary);

    if(!f.is_open()){
        std::cout << "File could not be opened" << std::endl;
        return;
    }
    unsigned char bmpPad[3] = {0, 0, 0};
    const int paddingAmount = ((4 - (m_width * 3) % 4 ) % 4);
    const int fileHeaderSize = 14;
    const int InformationHeaderSize = 40;
    const int fileSize = fileHeaderSize + InformationHeaderSize +  m_width * m_height * 3 + paddingAmount * m_width;

	unsigned char fileHeader[fileHeaderSize];
	//HEADER
	//Signature ('BM') - 2 bytes
    fileHeader[0]  =  'B' ;
	fileHeader[1]  =  'M' ;

	//File size in bytes - 4 bytes
	fileHeader[2]  =  fileSize ;
	fileHeader[3]  =  fileSize >> 8 ;
	fileHeader[4]  =  fileSize >> 16 ;
	fileHeader[5]  =  fileSize >> 24 ;

	//Reserved, unused(=0) - 4 bytes
	fileHeader[6]  =  0 ;
	fileHeader[7]  =  0 ;
	fileHeader[8]  =  0 ;
	fileHeader[9]  =  0 ;

	//DataOffset - 4 bytes
	fileHeader[10] =  fileHeaderSize + InformationHeaderSize ;
	fileHeader[11] =  0 ;
	fileHeader[12] =  0 ;
	fileHeader[13] =  0 ;


    unsigned char InformationHeader[InformationHeaderSize];
	//INFO HEADER
	//Size of info header - 4 bytes
	InformationHeader[0] =  InformationHeaderSize ;
	InformationHeader[1] =  0 ;
	InformationHeader[2] =  0 ;
	InformationHeader[3] =  0 ;

	//Horizontal width of bitmap in pixels - 4 bytes
	InformationHeader[4] =  m_width ;
	InformationHeader[5] =  m_width >> 8 ;
	InformationHeader[6] =  m_width >> 16 ;
	InformationHeader[7] =  m_width >> 24 ;

	//Vertical height of bitmat in pixels - 4 bytes
	InformationHeader[8]  =  m_height ;
	InformationHeader[9]  =  m_height >> 8;
	InformationHeader[10] =  m_height >> 16 ;
	InformationHeader[11] =  m_height >> 24 ;

	//Number of planes(=1) - 2 bytes
	InformationHeader[12] =  1 ;
	InformationHeader[13] =  0 ;

	//Bits per pixel(=1, 4, 8, 16, 24) - 2 bytes
	InformationHeader[14] =  24 ;
	InformationHeader[15] =  0 ;

	//Compresion(=0, 1, 2) - 4 bytes
	InformationHeader[16] =  0 ;
	InformationHeader[17] =  0 ;
	InformationHeader[18] =  0 ;
	InformationHeader[19] =  0 ;

	//ImageSize(=0. if Compression = 0) - 4 bytes
	InformationHeader[20] =  0 ;
	InformationHeader[21] =  0 ;
	InformationHeader[22] =  0 ;
	InformationHeader[23] =  0 ;

	//XpixelsPerM(horizontal resolution: Pixels/meter) - 4 bytes
	InformationHeader[24] =  0 ;
	InformationHeader[25] =  0 ;
	InformationHeader[26] =  0 ;
	InformationHeader[27] =  0 ;

	//YpixelsPerM(vertical resolution: Pixels/meter) - 4 bytes
	InformationHeader[28] =  0 ;
	InformationHeader[29] =  0 ;
	InformationHeader[30] =  0 ;
	InformationHeader[31] =  0 ;

	//Colors Used(for a 8-bit/ pixel bitmat this will be 100h or 256) - 4 bytes
	InformationHeader[32] =  0 ;
	InformationHeader[33] =  0 ;
	InformationHeader[34] =  0 ;
	InformationHeader[35] =  0 ;

	//Important Colors(0 = all colors are important) - 4 bytes
	InformationHeader[36] =  0 ;
	InformationHeader[37] =  0 ;
	InformationHeader[38] =  0 ;
	InformationHeader[39] =  0 ;

    f.write(reinterpret_cast<char*>(fileHeader), fileHeaderSize);
    f.write(reinterpret_cast<char*>(InformationHeader), InformationHeaderSize);

	//Pixel data (InfoHeader.ImageSize)
    for (int y = 0; y < m_height; y++){
        for (int x = 0; x < m_width; x++){
            unsigned char r = static_cast<unsigned char>(GetColor(x, y).r * 255.0f);
            unsigned char g = static_cast<unsigned char>(GetColor(x, y).g * 255.0f);
            unsigned char b = static_cast<unsigned char>(GetColor(x, y).b * 255.0f);

            unsigned char color[] = {b, g, r};
            f.write(reinterpret_cast<char*>(color), 3);
        }
        f.write(reinterpret_cast<char*>(bmpPad), paddingAmount);
    }
    f.close();
    std::cout << "File " << path << " has been saved" << std::endl;


}



/*

class Bitmap{

public:
    Bitmap(int width, int height);
    ~Bitmap()

    Color GetColor(int x, int y) const;
    void SetColor(const Color& color, int x, int y);

    void Export(const char* path);

private:
    int m_width;
    int m_height;
    std::vector<Color> m_colors;

};
























class Bitmap{
	
	std::fstream m_file;
	int m_width, m_height;
	std::vector<char> m_content;
	
public:
	Bitmap(const char* file, int width, int height){
        m_file.open(file, std::ios::in | std::ios::out | std::ios::binary);
        m_width = width ;
	    m_height = height;
        char m_content[3 * m_width * m_height];

    }

	void setColor(Color& color, int x, int y){
		m_content[y * m_width + x + 0] = color.r;
        m_content[y * m_width + x + 1] = color.g;
        m_content[y * m_width + x + 2] = color.b;
	}

	void Export(){
		int header_size = 54;
	int content_size = 2*2;
	unsigned char header[header_size];
	unsigned char content[content_size];
	//HEADER
	//Signature ('BM') - 2 bytes
    header[0]  =  0x42 ;
	header[1]  =  0x40 ;

	//File size in bytes - 4 bytes
	header[2]  =  0x3A ;
	header[3]  =  0x00 ;
	header[4]  =  0x00 ;
	header[5]  =  0x00 ;

	//Reserved, unused(=0) - 4 bytes
	header[6]  =  0x00 ;
	header[7]  =  0x00 ;
	header[8]  =  0x00 ;
	header[9]  =  0x00 ;

	//DataOffset - 4 bytes
	header[10] =  0x36 ;
	header[11] =  0x00 ;
	header[12] =  0x00 ;
	header[13] =  0x00 ;

	//INFO HEADER
	//Size of info header - 4 bytes
	header[14] =  0x28 ;
	header[15] =  0x00 ;
	header[16] =  0x00 ;
	header[17] =  0x00 ;

	//Horizontal width of bitmap in pixels - 4 bytes
	header[18] =  0x02 ;
	header[19] =  0x00 ;
	header[20] =  0x00 ;
	header[21] =  0x00 ;

	//Vertical height of bitmat in pixels - 4 bytes
	header[22] =  0x02 ;
	header[23] =  0x00 ;
	header[24] =  0x00 ;
	header[25] =  0x00 ;

	//Number of planes(=1) - 2 bytes
	header[26] =  0x01 ;
	header[27] =  0x00 ;

	//Bits per pixel(=1, 4, 8, 16, 24) - 2 bytes
	header[28] =  0x01 ;
	header[29] =  0x00 ;

	//Compresion(=0, 1, 2) - 4 bytes
	header[30] =  0x00 ;
	header[31] =  0x00 ;
	header[32] =  0x00 ;
	header[33] =  0x00 ;

	//ImageSize(=0. if Compression = 0) - 4 bytes
	header[34] =  0x00 ;
	header[35] =  0x00 ;
	header[36] =  0x00 ;
	header[37] =  0x00 ;

	//XpixelsPerM(horizontal resolution: Pixels/meter) - 4 bytes
	header[38] =  0x02 ;
	header[39] =  0x00 ;
	header[40] =  0x00 ;
	header[41] =  0x00 ;

	//YpixelsPerM(vertical resolution: Pixels/meter) - 4 bytes
	header[42] =  0x02 ;
	header[43] =  0x00 ;
	header[44] =  0x00 ;
	header[45] =  0x00 ;

	//Colors Used(for a 8-bit/ pixel bitmat this will be 100h or 256) - 4 bytes
	header[46] =  0xFF ;
	header[47] =  0x00 ;
	header[48] =  0x00 ;
	header[49] =  0x00 ;

	//Important Colors(0 = all colors are important) - 4 bytes
	header[50] =  0x00 ;
	header[51] =  0x00 ;
	header[52] =  0x00 ;
	header[53] =  0x00 ;

	//Pixel data (InfoHeader.ImageSize)
	content[0] = 0;
	content[1] = 0;
    	content[2] = 0;
    	content[3] = 0;
	for(int i = 0; i < header_size; i++){
		photo << header[i];
	}
	for(int i = 0; i < content_size; i++){
		photo << content[i];
	}
//    	photo.write(header, 14);
//    	photo.write(content, 4);
    	photo.close();

        for(int y = 0; y < m_height; y++){
            for(int x = 0; x < m_width; x++){
                m_file << &m_content[y * m_width + x] ;
            }
        }
        m_file.close();

    }

};

*/
