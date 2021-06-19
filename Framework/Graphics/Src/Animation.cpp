#include "Precompiled.h"
#include "Animation.h"

using namespace MAGE::Math;
using namespace MAGE::Graphics;

//Return the Matrix Transformation at the given time after 
//the Rotation and Translation calculated by the Vector Position,
//and Scaling of the object calculated from Quaternion position 
Matrix4 MAGE::Graphics::Animation::GetTransformation(float time) const
{
	Matrix4 matrixRot = Matrix4::RotationQuaternion(GetRotation(time));
	Matrix4 matrixPos = Matrix4::Translation(GetPosition(time));
	Matrix4 matrixScale = Matrix4::Scaling(GetScale(time));
	return matrixScale * matrixRot * matrixPos;
}

/*
Get Positon & Get Scale As noted in Peter Presentation Week 2 1:38
Suppose you have Key Frame 0,1,2. Your array ie: "PositionKeys" 
for loop from the first key frame and find a key frame that is bigger than 
 the time you inputted. Afterwards caluclate the value used to lerp the two
 positions in between the current key frame and its next keyframe. 
 For Rotations however you need to use Slerp instead of Lerp. 
*/

Vector3 MAGE::Graphics::Animation::GetPosition(float time) const
{
	int mPositionSize = static_cast<int>(mPositionKeys.size());
	if (mPositionSize > 0)
	{
		int nextKey = 0;
		for (int i = 0; i < mPositionSize; ++i)
		{
			nextKey = i;
			if (mPositionKeys[i].time > time)
			{
				break;
			}
		}
		int currentKey = Max(0, nextKey - 1);
		//Subtract the next Keyframe time from the current key frame time
		float totalTime = (mPositionKeys[nextKey].time - mPositionKeys[currentKey].time);
		float calculatedTime;
		if (totalTime != 0)
		{			
			//This calculated Time is used to lerp the FramePositions of 
			//current key and the next by that value to smooth the translations
			calculatedTime = time - mPositionKeys[currentKey].time / totalTime;
		}
		else 
		{
			calculatedTime = time - mPositionKeys[currentKey].time;

		}
		Vector3 frameBegin = mPositionKeys[currentKey].key;
		Vector3 frameEnd = mPositionKeys[nextKey].key;
		return Lerp(frameBegin, frameEnd, calculatedTime);
	}
		return Vector3::Zero;
	
}

Quaternion MAGE::Graphics::Animation::GetRotation(float time) const
{
	int mRotationSize = static_cast<int>(mRotationKeys.size());
	if (mRotationSize > 0)
	{
		int nextKey = 0;
		for (int i = 0; i < mRotationSize; ++i)
		{
			nextKey = i;
			if (mRotationKeys[i].time > time)
			{
				break;
			}
		}
		int currentKey = Max(0,nextKey - 1 );
		float totalTime = (mRotationKeys[nextKey].time - mRotationKeys[currentKey].time);
		float calculatedTime;
		if (totalTime != 0)
		{
			calculatedTime = time - mRotationKeys[currentKey].time / totalTime;
		}
		else
		{
			calculatedTime = time - mRotationKeys[currentKey].time;
		}
		Quaternion q0 = mRotationKeys[currentKey].key;
		Quaternion q1 = mRotationKeys[nextKey].key;
		return (Slerp(q0, q1, calculatedTime));
	}
	return Quaternion::Identity;
}

Vector3 MAGE::Graphics::Animation::GetScale(float time) const
{
	int mScaleSize = static_cast<int>(mScaleKeys.size());
	if (mScaleSize > 0)
	{
		int nextKey = 0;
		for (int i = 0; i < mScaleSize; ++i)
		{
			nextKey = i;
			if (mScaleKeys[i].time > time)
			{
				break;
			}
		}
		int currentKey = Max(0, nextKey - 1);
		float totalTime = (mScaleKeys[nextKey].time - mScaleKeys[currentKey].time);
		float calculatedTime;
		if (totalTime != 0)
		{
			calculatedTime = time - mScaleKeys[currentKey].time / totalTime;
		}
		else
		{
			calculatedTime = time - mScaleKeys[currentKey].time;

		}
		Vector3 frameBegin = mScaleKeys[currentKey].key;
		Vector3 frameEnd = mScaleKeys[nextKey].key;
		return Lerp(frameBegin, frameEnd, calculatedTime);
	}
	return Vector3::Zero;
}





