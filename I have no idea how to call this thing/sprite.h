#pragma once
#include "hmain.h"
using namespace std;
namespace DX2D
{
	class sprite
	{
	protected:
		ID2D1Bitmap *bitmap;
	protected:
		int2* pos;
		int2* offset;
		float* rot; // in degrees
		inline void updatematrix()
		{

		}
	public:
		bool forceoverridepos = false;
		int2* overridepos = nullptr;
		bool useidentp = false; // if true and identp == nullptr then sprite will be deleted
		bool* identp = nullptr; //^
		bool* render = nullptr; // if nullptr or true sprite will be rendered. 
		bool deleteatdest = true;
		bool breakonrender = false;
		/*XMMATRIX matrix;*/
		D2D1::Matrix3x2F matrix;
		D2D_RECT_F lastloc = { 0,0,0,0 };
		bool savelastloc = false;
		bool lastlochaststarted = false;
		bool breakonlocchange = true;
		bool isrendonscreen = false;
		D2D1_SIZE_F size;
		bool usecustompoint = false;
		int2 customrpoint = { 0,0 };
		void copypointers()
		{
			if (identp != nullptr)
			{
				identp = new bool(*identp);
			}
			if (render != nullptr)
			{
				render = new bool(*render);
			}
			pos = new int2(*pos);
			offset = new int2(*offset);
			rot = new float(*rot);
		}
		HRESULT SetBitmapFromFile(LPCWSTR szFileName, ID2D1RenderTarget* camRT)
		{
			HRESULT hr = 1;
			// Create a decoder
			IWICBitmapDecoder *pDecoder = NULL;
			IWICImagingFactory2*  fac;
			CoCreateInstance(
				CLSID_WICImagingFactory,
				NULL,
				CLSCTX_INPROC_SERVER,
				IID_IWICImagingFactory2,
				(LPVOID*)&fac
			);
			hr = fac->CreateDecoderFromFilename(
				szFileName,                      // Image to be decoded
				NULL,                            // Do not prefer a particular vendor
				GENERIC_READ,                    // Desired read access to the file
				WICDecodeMetadataCacheOnDemand,  // Cache metadata when needed
				&pDecoder                        // Pointer to the decoder
			);

			// Retrieve the first frame of the image from the decoder
			IWICBitmapFrameDecode *pFrame = NULL;

			if (SUCCEEDED(hr))
			{

				hr = pDecoder->GetFrame(0, &pFrame);
				IWICFormatConverter* pConverter;
				hr = fac->CreateFormatConverter(&pConverter);
				hr = pConverter->Initialize(
					pFrame,
					GUID_WICPixelFormat32bppPBGRA,
					WICBitmapDitherTypeNone,
					NULL,
					0.f,
					WICBitmapPaletteTypeMedianCut
				);
				ID2D1RenderTarget* RT = camRT;
				hr = RT->CreateBitmapFromWicBitmap(
					pConverter,
					NULL,
					&bitmap
				);
			}
			else
			{
				DebugBreak();
			}
			size = bitmap->GetSize();

			return hr;
		}
		inline void SetBitmap(ID2D1Bitmap *bm)
		{
			bitmap = bm;
			updatematrix();
		}
		ID2D1Bitmap* GetBitmap()
		{
			return bitmap;
		}
		ID2D1Bitmap** GetBitmapP()
		{
			return &bitmap;
		}
		inline int2 GetSize()
		{
			D2D1_SIZE_F sz = bitmap->GetSize();
			return{ (int) sz.width,(int)sz.height };
		}
		inline void offsetbysize()
		{
			D2D1_SIZE_F sz = bitmap->GetSize();
			pos->x -= sz.width / 2;
			pos->y -= sz.height / 2;
		}
		inline int2* GetXYp()
		{
			return pos;
		}
		inline int2** GetXYpp()
		{
			return &pos;
		}
		inline int* GetXp()
		{
			return &pos->x;
		}
		inline int* GetYp()
		{
			return &pos->y;
		}
		inline float GetX()
		{
			return pos->x;
		}
		inline float GetY()
		{
			return pos->y;
		}
		inline float GetRot()
		{
			return *rot;
		}
		inline void Rotate(float ang)
		{
			*rot += ang;
			updatematrix();
		}
		inline void SetRot(float ang)
		{
			*rot = ang;
			updatematrix();
		}
		inline void SetX(int X)
		{
			pos->x = X;
			updatematrix();
		}
		inline void SetY(int Y)
		{
			pos->y = Y;
			updatematrix();
		}
		inline void SetXY(int2 XY)
		{
			pos->x = XY.x;
			pos->y = XY.y;
		}
		inline void SetOffsetXYp(int2* noffset, bool deleteold)
		{
			if (deleteold)
			{
				delete offset;
			}
			offset = noffset;
		}
		inline void SyncPos(int2* nposp, bool deleteold)
		{
			if (deleteold)
			{
				delete pos;
			}
			pos = nposp;
			updatematrix();
		}
		inline void SyncRot(float* nrotp, bool deleteold)
		{
			if (deleteold)
			{
				delete rot;
			}
			rot = nrotp;
			updatematrix();
		}
		inline int2* GetOffSetp()
		{
			return offset;
		}
		~sprite()
		{
			if (deleteatdest)
			{
				//delete pos;
				//delete rot;
			}
		}
		sprite()
		{
			offset = new int2(0, 0);
			rot = new float(0);
			pos = new int2(0, 0);
			updatematrix();
		}
		sprite(bool construct, bool updatematrixatstart)
		{
			if (construct)
			{
				offset = new int2(0, 0);
				rot = new float(0);
				pos = new int2(0, 0);
				if (updatematrixatstart)
					updatematrix();
			}
			else
			{

			}
		}
		sprite(bool deleteatend)
		{
			offset = new int2(0, 0);
			deleteatdest = deleteatend;
			rot = new float(0);
			pos = new int2(0, 0);
			updatematrix();
		}
		sprite(int2 npos, float nangle)
		{
			offset = new int2(0, 0);
			pos = new int2(npos);
			rot = new float(nangle);
			updatematrix();
		}
		sprite(int X, int Y, float nangle)
		{
			offset = new int2(0, 0);
			pos = new int2(X, Y);
			rot = new float(nangle);
			updatematrix();
		}
		sprite(int2* posp, float* rotp)
		{
			offset = new int2(0, 0);
			pos = posp;
			rot = rotp;
			updatematrix();
		}
	};
}