#include"C_function.h"

bool C_Hitcheck(float top1, float left1, float bottom1, float right1,
	float top2, float left2, float bottom2, float right2)
{
	//“–‚½‚Á‚Ä‚¢‚éŽž
	if ((right1 > left2) &&
		(left1 < right2) &&
		(top1 < bottom2) &&
		(bottom1 > top2))
	{
		return true;
	}
	//‚»‚êˆÈŠO
	else
		return false;
}

void C_OffsetBox(float top, float left, float& bottom, float& right, float sizeX, float sizeY)
{
	top = top;
	left = left;
	bottom = top + sizeY;
	right = left + sizeX;
}