#include "Tga.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

namespace GeneralLib
{
	//----------------------------------------------------------------------------------------------
	// Public : CTga
	//--	--------------------------------------------------------------------------------------------
	CTga::CTga()
	{
		m_pImageBuffer = NULL;
		m_nWidth = m_nHeight = 0;
	}

	//----------------------------------------------------------------------------------------------
	// Public : CTga
	//----------------------------------------------------------------------------------------------
	CTga::CTga( const char *sFileName )
	{
		bool bFound = Read( sFileName );
		assert( bFound );
	}

	//----------------------------------------------------------------------------------------------
	// Public : CTga
	//----------------------------------------------------------------------------------------------
	CTga::CTga( U32 nWidth, U32 nHeight, U32 nBpp )
	{
		Create( nWidth, nHeight, nBpp );
	}

	//----------------------------------------------------------------------------------------------
	// Public : ~CTga
	//----------------------------------------------------------------------------------------------
	CTga::~CTga()
	{
		if (m_pImageBuffer)
			delete [] m_pImageBuffer;
	}

	//----------------------------------------------------------------------------------------------
	// Public : Create
	//----------------------------------------------------------------------------------------------
	void CTga::Create( U32 nWidth, U32 nHeight, U32 nBpp )
	{
		assert( nBpp == 32 || nBpp == 24 );
		m_nWidth = nWidth;
		m_nHeight = nHeight; 
		m_nBytesPerPixel = nBpp / 8;
		m_pImageBuffer = new U8[m_nWidth * m_nHeight * m_nBytesPerPixel];
	}

	//----------------------------------------------------------------------------------------------
	// Public : Set
	//----------------------------------------------------------------------------------------------
	void CTga::Set( const SColor &oColor, U32 nX, U32 nY )
	{
		m_pImageBuffer[m_nBytesPerPixel * (nY * m_nWidth + nX) + 0] = oColor.m_nBlue;
		m_pImageBuffer[m_nBytesPerPixel * (nY * m_nWidth + nX) + 1] = oColor.m_nGreen;
		m_pImageBuffer[m_nBytesPerPixel * (nY * m_nWidth + nX) + 2] = oColor.m_nRed;

		if ( m_nBytesPerPixel == 4 )
			m_pImageBuffer[m_nBytesPerPixel * (nY * m_nWidth + nX) + 3] = oColor.m_nAlpha;
	}

	//----------------------------------------------------------------------------------------------
	// Public : Get
	//----------------------------------------------------------------------------------------------
	void CTga::Get( SColor &oColor, U32 nX, U32 nY ) const
	{
		oColor.m_nBlue = m_pImageBuffer[m_nBytesPerPixel * (nY * m_nWidth + nX) + 0];
		oColor.m_nGreen = m_pImageBuffer[m_nBytesPerPixel * (nY * m_nWidth + nX) + 1];
		oColor.m_nRed = m_pImageBuffer[m_nBytesPerPixel * (nY * m_nWidth + nX) + 2];

		if ( m_nBytesPerPixel == 4 )
			oColor.m_nAlpha = m_pImageBuffer[m_nBytesPerPixel * (nY * m_nWidth + nX) + 3];
	}

	//----------------------------------------------------------------------------------------------
	// Public : Read
	//----------------------------------------------------------------------------------------------
	bool CTga::Read( const char *sFileName )
	{
		// Open the file
		FILE *pFile;
		if ( (fopen_s(&pFile, sFileName, "rb")) != 0 )
			return false;

		// Read the file header
		// Have to read piece-by-piece because the struct is padded to be in 32-bit sections
		STgaFileHeader oFileHeader;
		{
			if ( fread( (void *)&oFileHeader.m_nIdentSize, sizeof(U8), 1, pFile ) <= 0 )
				return false;
			if ( fread( (void *)&oFileHeader.m_nColorMapType, sizeof(U8), 1, pFile ) <= 0 )
				return false;
			if ( fread( (void *)&oFileHeader.m_nImageType, sizeof(U8), 1, pFile ) <= 0 )
				return false;
			if ( fread( (void *)&oFileHeader.m_nColorMapStart, sizeof(U16), 1, pFile ) <= 0 )
				return false;
			if ( fread( (void *)&oFileHeader.m_nColorMapLength, sizeof(U16), 1, pFile ) <= 0 )
				return false;
			if ( fread( (void *)&oFileHeader.m_nColorMapBits, sizeof(U8), 1, pFile ) <= 0 )
				return false;
			if ( fread( (void *)&oFileHeader.m_nXStart, sizeof(U16), 1, pFile ) <= 0 )
				return false;
			if ( fread( (void *)&oFileHeader.m_nYStart, sizeof(U16), 1, pFile ) <= 0 )
				return false;
			if ( fread( (void *)&oFileHeader.m_nWidth, sizeof(U16), 1, pFile ) <= 0 )
				return false;
			if ( fread( (void *)&oFileHeader.m_nHeight, sizeof(U16), 1, pFile ) <= 0 )
				return false;
			if ( fread( (void *)&oFileHeader.m_nBits, sizeof(U8), 1, pFile ) <= 0 )
				return false;
			if ( fread( (void *)&oFileHeader.m_nDescriptor, sizeof(SImageDescriptor), 1, pFile ) <= 0 )
				return false;
		}

		// Resize image buffer
		Create( oFileHeader.m_nWidth, oFileHeader.m_nHeight, oFileHeader.m_nBits );

		// Only 32-bit and 24-bit tgas supported
		assert(oFileHeader.m_nBits == 32 || oFileHeader.m_nBits == 24);

		// Palettes not supported
		assert(oFileHeader.m_nColorMapType == 0);

		// Only bottom-left and top-left mode supported
		assert(oFileHeader.m_nDescriptor.m_nImageOrigin == 0 || oFileHeader.m_nDescriptor.m_nImageOrigin == 2);

		// Ignore the ID field
		if ( oFileHeader.m_nIdentSize > 0 )
		{
			U8 *pBuffer = new U8[oFileHeader.m_nIdentSize];
			if ( fread( (void *)pBuffer, oFileHeader.m_nIdentSize, 1, pFile ) <= 0 )
				return false;
			delete [] pBuffer;
		}

		for ( U32 nY = 0; nY < oFileHeader.m_nHeight; nY++ )
		{
			// Read the row 
			U8 *pColorRow = &m_pImageBuffer[nY * m_nWidth * m_nBytesPerPixel];
			if ( oFileHeader.m_nDescriptor.m_nImageOrigin == 0 )	// bottom row written first
				pColorRow = &m_pImageBuffer[(m_nHeight - nY - 1) * m_nWidth * m_nBytesPerPixel];

			if ( fread( (void *)pColorRow, oFileHeader.m_nWidth * m_nBytesPerPixel, 1, pFile ) <= 0 )
				return false;
		}

		fclose( pFile );
		return true;
	}

	//----------------------------------------------------------------------------------------------
	// Public : Write
	//----------------------------------------------------------------------------------------------
	bool CTga::Write( const char *sFileName ) const
	{
		if ( !m_pImageBuffer )
			return false;
	
		U32 nImageSize = m_nHeight * m_nWidth * sizeof(SColor);

		U32 nFileSize = sizeof(STgaFileHeader) + nImageSize;

		// Create the file
		FILE *pFile;
		if ( (fopen_s(&pFile, sFileName, "wb")) != 0 )
			return false;

		// Fill in file header
		STgaFileHeader oFileHeader;
		oFileHeader.m_nIdentSize = 0;				// No optional image ID section
		oFileHeader.m_nColorMapType = 0;			// No palette
		oFileHeader.m_nImageType = 0;				// Standard image type
		oFileHeader.m_nColorMapStart = 0;			// No palette
		oFileHeader.m_nColorMapLength = 0;			// No palette
		oFileHeader.m_nColorMapBits = 0;			// No palette
		oFileHeader.m_nXStart = 0;					// Start at 0,0
		oFileHeader.m_nYStart = 0;					// Start at 0,0
		oFileHeader.m_nWidth = m_nWidth;
		oFileHeader.m_nHeight = m_nHeight;
		oFileHeader.m_nBits = 32;					// Only 32-bit formats supported
		oFileHeader.m_nDescriptor.m_nAlphaBits = 3;
		oFileHeader.m_nDescriptor.m_nImageOrigin = 2;	// Top-left origin
		oFileHeader.m_nDescriptor.m_nReserved = 0;

		// Write the file header
		{
			if ( fwrite( (void *)&oFileHeader.m_nIdentSize, sizeof(U8), 1, pFile ) <= 0 )
				return false;
			if ( fwrite( (void *)&oFileHeader.m_nColorMapType, sizeof(U8), 1, pFile ) <= 0 )
				return false;
			if ( fwrite( (void *)&oFileHeader.m_nImageType, sizeof(U8), 1, pFile ) <= 0 )
				return false;
			if ( fwrite( (void *)&oFileHeader.m_nColorMapStart, sizeof(U16), 1, pFile ) <= 0 )
				return false;
			if ( fwrite( (void *)&oFileHeader.m_nColorMapLength, sizeof(U16), 1, pFile ) <= 0 )
				return false;
			if ( fwrite( (void *)&oFileHeader.m_nColorMapBits, sizeof(U8), 1, pFile ) <= 0 )
				return false;
			if ( fwrite( (void *)&oFileHeader.m_nXStart, sizeof(U16), 1, pFile ) <= 0 )
				return false;
			if ( fwrite( (void *)&oFileHeader.m_nYStart, sizeof(U16), 1, pFile ) <= 0 )
				return false;
			if ( fwrite( (void *)&oFileHeader.m_nWidth, sizeof(U16), 1, pFile ) <= 0 )
				return false;
			if ( fwrite( (void *)&oFileHeader.m_nHeight, sizeof(U16), 1, pFile ) <= 0 )
				return false;
			if ( fwrite( (void *)&oFileHeader.m_nBits, sizeof(U8), 1, pFile ) <= 0 )
				return false;
			if ( fwrite( (void *)&oFileHeader.m_nDescriptor, sizeof(SImageDescriptor), 1, pFile ) <= 0 )
				return false;
		}

		for ( U32 nY = 0; nY < m_nHeight; nY++ )
		{
			// Write the row
			const U8 *pColorRow = &m_pImageBuffer[nY * m_nWidth * m_nBytesPerPixel];
			if ( fwrite( (void *)pColorRow, m_nWidth * m_nBytesPerPixel, 1, pFile ) <= 0 )
				return false;
		}


		fclose( pFile );
		return true;
	}

	//----------------------------------------------------------------------------------------------
	// Public : GetWidth
	//----------------------------------------------------------------------------------------------
	U32 CTga::GetWidth() const
	{
		return m_nWidth;
	}

	//----------------------------------------------------------------------------------------------
	// Public : GetHeight
	//----------------------------------------------------------------------------------------------
	U32 CTga::GetHeight() const
	{
		return m_nHeight;
	}

	//----------------------------------------------------------------------------------------------
	// Public : GetBytesPerPixel
	//----------------------------------------------------------------------------------------------
	U32 CTga::GetBytesPerPixel() const
	{
		return m_nBytesPerPixel;
	}

	//----------------------------------------------------------------------------------------------
	// Public : GetBuffer
	//----------------------------------------------------------------------------------------------
	const U8 *CTga::GetBuffer() const
	{
		return (U8*)m_pImageBuffer;
	}
}