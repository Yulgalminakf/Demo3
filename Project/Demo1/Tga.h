#ifndef _TGA_H_
#define _TGA_H_

typedef unsigned int U32;
typedef unsigned char U8;
typedef float F32;
typedef unsigned short U16;

namespace GeneralLib
{
	class CTga
	{
	public:
		struct SColor
		{
			U8 m_nRed;
			U8 m_nGreen;
			U8 m_nBlue;
			U8 m_nAlpha;
			SColor() {}
			SColor( U8 nRed, U8 nGreen, U8 nBlue, U32 nAlpha ) : m_nRed(nRed), m_nGreen(nGreen), m_nBlue(nBlue), m_nAlpha(nAlpha) {}
		};

		CTga();

		// Constructor to call if reading from a TGA file
		CTga( const char *sFileName ); // Calls Read()

		// Constructor to call if writing a TGA file
		CTga( U32 nWidth, U32 nHeight, U32 nBpp ); // Calls Create()

		~CTga();

		void Create( U32 nWidth, U32 nHeight, U32 nBpp );

		// Set a color at the given location
		void Set( const SColor &oColor, U32 nX, U32 nY );

		// Get a color from the given location
		// Note: If format is 24 bpp, then the alpha channel value is undefined
		void Get( SColor &oColor, U32 nX, U32 nY ) const;

		// Read from a TGA file
		bool Read( const char *sFileName );

		// Write out a TGA file
		bool Write( const char *sFileName ) const;

		// Image properties
		U32 GetWidth() const;
		U32 GetHeight() const;
		U32 GetBytesPerPixel() const; 

		// Get the raw buffer
		const U8 *GetBuffer() const;

	private:

		struct SImageDescriptor
		{
			U8  m_nAlphaBits : 4;      // Number of attribute/alpha bits
			U8  m_nImageOrigin : 2;	   // Screen destination of 1st pixel.  00-bl, 01-br, 10-tl, 11-tr
			U8  m_nReserved : 2;
		};

		struct STgaFileHeader
		{
			U8  m_nIdentSize;          // size of ID field that follows 18 U8 header (0 usually)
			U8  m_nColorMapType;	   // type of colour map 0=none, 1=has palette
			U8  m_nImageType;          // type of image 0=none,1=indexed,2=rgb,3=grey,+8=rle packed

			U16 m_nColorMapStart;     // first colour map entry in palette
			U16 m_nColorMapLength;    // number of colours in palette
			U8  m_nColorMapBits;      // number of bits per palette entry 15,16,24,32

			U16 m_nXStart;             // image x origin
			U16 m_nYStart;             // image y origin
			U16 m_nWidth;              // image width in pixels
			U16 m_nHeight;             // image height in pixels
			U8  m_nBits;               // image bits per pixel 8,16,24,32

			SImageDescriptor  m_nDescriptor;         // image descriptor bits (vh flip bits)

			// pixel data follows header
		};

		// A 2D image is a 1D array of SColors
		U8 *m_pImageBuffer;
		U32 m_nWidth;
		U32 m_nHeight;
		U32 m_nBytesPerPixel;
	};
}

#endif