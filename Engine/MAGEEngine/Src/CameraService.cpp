#include "Precompiled.h"
#include "CameraService.h"

using namespace MAGE;

META_DERIVED_BEGIN(CameraService, Service)
META_NO_FIELD
META_CLASS_END

void MAGE::CameraService::Initialize()
{
	mActiveCamera = AddCamera("Default");
}

 Graphics::Camera* MAGE::CameraService::AddCamera(const char* name)
{		
	// if it doesn't exist then create it and return.
	 return &mCameraMap[name];
}

Graphics::Camera* MAGE::CameraService::FindCamera(const char* name)
{
	auto iter = mCameraMap.find(name);

	return (iter == mCameraMap.end()) ? nullptr : &(iter->second);;
}

void MAGE::CameraService::SetActiveCamera(const char* name)
{
	auto camera = FindCamera(name);
	if (camera)
		mActiveCamera = camera;
}

Graphics::Camera& MAGE::CameraService::GetActiveCamera()
{
	ASSERT(mActiveCamera, "CameraService -- No Active Camera");
	return *mActiveCamera;	
}

const Graphics::Camera& MAGE::CameraService::GetActiveCamera() const
{
	ASSERT(mActiveCamera, "CameraService -- No Active Camera");
	return *mActiveCamera;
}
