#pragma once
#include "hmain.h"
#include "frame.h"
using namespace std;
using namespace D2D1;
namespace DX2D
{
	extern ID2D1DeviceContext *hwndRT;
	class scene
	{
	public:
		map<string, frame*> objectsmap;
		vector<frame*>objectsvec;
	};
	class camera
	{
	private:
		IWICBitmap* bitmap;
		ID2D1BitmapRenderTarget* RenderTarget = NULL;
	protected:
		int2 offset;
		int2* pos;
		float* rot; // in degrees
		float x = 0;
		float y = 0;
		inline void updatematrix()
		{
			//D2D1_SIZE_F sz = RenderTarget->GetSize();
			//RenderTarget->SetTransform(Matrix3x2F::Rotation(XMConvertToRadians(*rot), { sz.width/2,sz.height / 2 }));
		}
	public:
		vector<int2*>secposvec;
		int2 customrendrotpoint;
		bool usecustomrenderrotpoint = false;
		bool ignoreXYforrendering = false;
		D2D1_POINT_2F scalepoint;
		float2 scale = { 1,1 };
		bool usescale = false;
		int2 rotpointoffset = { 0,0 };
		D2D1_POINT_2F customrotpoint;
		bool usecustomrotpoint = false;
		inline void SetScalePoint(int2 p)
		{
			scalepoint = {(float) p.x,(float)p.y };
		}
		int GetOffsetX()
		{
			return offset.x;
		}
		int GetOffsetY()
		{
			return offset.y;
		}
		inline int2 GetOffset()
		{
			return offset;
		}
		inline void SetOffset(int2 noffset)
		{
			offset = noffset;
			return;
		}
		inline int2* GetXYp()
		{
			return pos;
		}
		inline int2 GetXY()
		{
			return *pos;
		}
		inline float* GetRotP()
		{
			return rot;
		}
		vector <scene> scenes;
		bool isonlyrectrendered = false;
		D2D1_RECT_U renderRECT = { 0,0,1,1 }; // only if isonlyrectrendered == true
		ID2D1Bitmap *bm;
		bool deleteatdest = true;
		/*XMMATRIX matrix;*/
		D2D1::Matrix3x2F matrix;
		bool isrendonscreen = false;
		D2D_RECT_F loc;
		//CreateDCRenderTarget
		bool updatesecposof = true;
		inline void updatesecpos()
		{
			if(updatesecposof)
				if (!secposvec.size() == 0)
				{
					int i = 0;
					while (i < secposvec.size())
					{
						*secposvec[i] = *pos;
						i++;
					}
				}
		}
		inline ID2D1BitmapRenderTarget** GetRenderTargetP()
		{
			return &RenderTarget;
		}
		inline void SetRenderTargetP(ID2D1BitmapRenderTarget* RT)
		{
			RenderTarget = RT;
			RenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE::D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
		}
		virtual IWICBitmap* GetBitmap()
		{
			return bitmap;
		}
		virtual IWICBitmap** GetBitmapP()
		{
			return &bitmap;
		}
		inline void SetBitmapP(IWICBitmap* bm)
		{
			bitmap = bm;
		}
		inline void MovX(float n)
		{
			pos->x += n;
			updatematrix();
		}
		inline void MovY(float n)
		{
			pos->y += n;
			updatematrix();
		}
		inline int GetX()
		{
			return pos->x;
		}
		inline int GetY()
		{
			return pos->y;
		}
		inline float GetRot()
		{
			return *rot;
		}
		inline void Rotate(float ang)
		{
			if (ang > 360)
			{
				ang -= 360;
				while (ang - 360 > 360)
				{
					ang -= 360;
				}
			}
			else if (ang < 0)
			{
				ang += 360;
				while (ang + 360 < 0)
				{
					ang += 360;
				}
			}
			if (*rot + ang > 360)
			{
				ang -= 360;
				while (ang - 360 > 360)
				{
					ang -= 360;
				}
			}
			else if (*rot + ang < 0)
			{
				ang += 360;
				while (ang + 360 < 0)
				{
					ang += 360;
				}
			}
			*rot += ang;
			updatematrix();
		}
		inline int2 GetBitmapSize()
		{
			D2D_SIZE_F sz =  RenderTarget->GetSize();
			return{ (int)sz.width,(int)sz.height };
		}
		inline void SetRot(float ang)
		{
			*rot = ang;
			updatematrix();
		}
		inline void SetX(int X)
		{
			pos->x = X;
			updatesecpos();
			updatematrix();
		}
		inline void SetY(int Y)
		{
			pos->y = Y;
			updatesecpos();
			updatematrix();
		}
		inline void SetXY(int2 n)
		{
			*pos = n;
			updatesecpos();
			updatematrix();
		}
		inline void SyncPos(int2* nposp, bool deleteold)
		{
			if (deleteold)
			{
				delete pos;
			}
			pos = nposp;
			updatesecpos();
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

		inline void reconstruct()
		{
			rot = new float(0);
			pos = new int2(0, 0);
			x = pos->x;
			y = pos->y;
			RenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE::D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
			updatesecpos();
			updatematrix();
		}
		~camera()
		{
			//if (deleteatdest)
			//{
			//	delete pos;
			//	delete rot;
			//}
			//RenderTarget->Release();
		}
		camera()
		{
			rot = new float(0);
			pos = new int2(0, 0);
			matrix = Matrix3x2F::Identity();
			ID2D1BitmapRenderTarget *RT = NULL;
			hwndRT->CreateCompatibleRenderTarget(&RT);
			RenderTarget = RT;
			RenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE::D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
			x = pos->x;
			y = pos->y;
			updatematrix();
		}
		camera(bool deleteatend)
		{
			deleteatdest = deleteatend;
			rot = new float(0);
			pos = new int2(0, 0);
			matrix = Matrix3x2F::Identity();
			ID2D1BitmapRenderTarget *RT = NULL;
			hwndRT->CreateCompatibleRenderTarget(&RT);
			RenderTarget = RT;
			RenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE::D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
			x = pos->x;
			y = pos->y;
			updatematrix();
		}
		camera(int2 npos, float nangle)
		{
			pos = new int2(npos);
			rot = new float(nangle);
			matrix = Matrix3x2F::Identity();
			ID2D1BitmapRenderTarget *RT = NULL;
			hwndRT->CreateCompatibleRenderTarget(&RT);
			RenderTarget = RT;
			RenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE::D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
			x = pos->x;
			y = pos->y;
			updatematrix();
		}
		camera(int X, int Y, float nangle)
		{
			pos = new int2(X, Y);
			rot = new float(nangle);
			matrix = Matrix3x2F::Identity();
			ID2D1BitmapRenderTarget *RT = NULL;
			hwndRT->CreateCompatibleRenderTarget(&RT);
			RenderTarget = RT;
			RenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE::D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
			x = pos->x;
			y = pos->y;
			updatematrix();
		}
		camera(int2* posp, float* rotp)
		{
			pos = posp;
			rot = rotp;
			matrix = Matrix3x2F::Identity();
			ID2D1BitmapRenderTarget *RT = NULL;
			hwndRT->CreateCompatibleRenderTarget(&RT);
			RenderTarget = RT;
			RenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE::D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
			x = pos->x;
			y = pos->y;
			updatematrix();
		}
	};
}