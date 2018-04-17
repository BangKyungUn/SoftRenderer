
#include "stdafx.h"
#include "Vector.h"
#include "SoftRenderer.h"
#include "GDIHelper.h"
#include "Renderer.h"
#include "Triangle.h"
#include "Texture.h"

bool IsInRange(int x, int y);
void PutPixel(int x, int y);

bool IsInRange(int x, int y)
{
	return (abs(x) < (g_nClientWidth / 2)) && (abs(y) < (g_nClientHeight / 2));
}

void PutPixel(IntPoint pt)
{
	if (!IsInRange(pt.X, pt.Y)) return;

	ULONG* dest = (ULONG*)g_pBits;
	DWORD offset = g_nClientWidth * g_nClientHeight / 2 + g_nClientWidth / 2 + pt.X + g_nClientWidth * -pt.Y;
	*(dest + offset) = g_CurrentColor;
}

void DrawLine(const Vector3& start, const Vector3& end)
{
	float length = (end - start).Dist();
	float inc = 1.0f / length;

	int maxLength = RoundToInt(length);
	for (int i = 0; i <= maxLength; i++)
	{
		float t = inc * i;
		if (t >= length) t = 1.0f;
		Vector3 Pt = start * (1.0f - t) + end * t;
		PutPixel(Pt.ToIntPoint());
	}

}

void Draw2DTriangle(Triangle t)
{
	for (int y = RoundToInt(t.sbbMin.Y); y < RoundToInt(t.sbbMax.Y); y++)
	{
		for (int x = RoundToInt(t.sbbMin.X); x < RoundToInt(t.sbbMax.X); x++)
		{
			Vector3 target((float)x, (float)y, 0.0f);
			float outS, outT;
			t.CalcBaryCentricCoord(target, &outS, &outT);
			if (t.IsInTrianble(outS, outT))
			{
				g_CurrentColor = t.GetPixelColor(target, outS, outT);
				PutPixel(IntPoint(x, y));
			}			
		}
	}
}

void UpdateFrame(void)
{
	// Buffer Clear
	SetColor(32, 128, 255);
	Clear();

	// Draw
	if (g_Texture->IsLoaded())
	{		
		ULONG Color32Bit = g_Texture->GetPixel(9, 1);
		BYTE* Color = (BYTE *)&Color32Bit;
		BYTE BValue = Color[0];
		BYTE GValue = Color[1];
		BYTE RValue = Color[2];
		BYTE AValue = Color[3];
	}

	Vector3 Pt1, Pt2, Pt3;

	static float offsetX = 0.0f;
	//static float offsetY = 0.0f;
	static float angle = 0.0f;
	static float scale = 1.0f;

	if (GetAsyncKeyState(VK_LEFT)) offsetX -= 1.0f;
	if (GetAsyncKeyState(VK_RIGHT)) offsetX += 1.0f;
	if (GetAsyncKeyState(VK_UP)) angle += 1.0f;
	if (GetAsyncKeyState(VK_DOWN)) angle -= 1.0f;
	if (GetAsyncKeyState(VK_PRIOR)) scale *= 1.01f;
	if (GetAsyncKeyState(VK_NEXT)) scale *= 0.99f;

	Matrix3 TMat, RMat, SMat;
	TMat.SetTranslation(offsetX, 0.0f);
	RMat.SetRotation(angle);
	SMat.SetScale(scale);
	Matrix3 TRSMat = TMat * RMat * SMat;

	Pt1.SetPoint(-160.0f, -160.0f);
	Pt2.SetPoint(200.0f, 120.0f);
	Pt3.SetPoint(-160.0f, 160.0f);

	Vertex v1(Pt1 * TRSMat);
	v1.color = RGB(255, 0, 0);
	Vertex v2(Pt2 * TRSMat);
	v2.color = RGB(0, 255, 0);
	Vertex v3(Pt3 * TRSMat);
	v3.color = RGB(0, 0, 255);

	Triangle T1(v1, v2, v3);

	SetColor(255, 0, 0);
	Draw2DTriangle(T1);

	// Buffer Swap 
	BufferSwap();
}
