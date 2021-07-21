#include "Camera.h"
#include "glm/gtc/matrix_transform.hpp"

const float DEF_FOV = 45.0f; 

//------------------------------------------------------------
// Base Camera class constructor
//------------------------------------------------------------
Camera::Camera()
: mPosition(glm::vec3(0.0f, 0.0f, 0.0f)),
  mTargetPos(glm::vec3(0.0f, 0.0f, 0.0f)),
  mUp(glm::vec3(0.0f, 1.0f, 0.0f)),
  mRight(1.0f, 0.0f, 0.0f),
  WORLD_UP(0.0f, 1.0f, 0.0f),
  mYaw(glm::pi<float>()),
  mPitch(0.0f),
  mFOV(DEF_FOV)
{
}

//------------------------------------------------------------
// Returns view matrix
//------------------------------------------------------------
glm::mat4 Camera::getViewMatrix()const
{
	return glm::lookAt(mPosition, mTargetPos, mUp);
}

//------------------------------------------------------------
// Base Camera 
//------------------------------------------------------------
const glm::vec3& Camera::getLook() const
{
	return mLook;
}

//------------------------------------------------------------
// Returns camera's local right vector
//------------------------------------------------------------
const glm::vec3& Camera::getRight() const
{
	return mRight;
}

//------------------------------------------------------------
// Returns camera's local up vector
//------------------------------------------------------------
const glm::vec3& Camera::getUp() const
{
	return mUp;
}

//------------------------------------------------------------
// Returns camera's position
//------------------------------------------------------------
const glm::vec3& Camera::getPosition() const
{
	return mPosition;
}

//-----------------------------------------------------------------------------
// FPSCamera - Constructor
//-----------------------------------------------------------------------------
FPSCamera::FPSCamera(glm::vec3 position, float yaw, float pitch)
{
	mPosition = position;
	mYaw = yaw;
	mPitch = pitch;
}

//-----------------------------------------------------------------------------
// Sets the camera position in world space
//-----------------------------------------------------------------------------
void FPSCamera::setPosition(const glm::vec3& position)
{
	mPosition = position;
}

//-----------------------------------------------------------------------------
// Sets the incremental position of the camera in world space
//-----------------------------------------------------------------------------
void FPSCamera::move(const glm::vec3& offsetPos)
{
	mPosition += offsetPos;
	updateCameraVectors();
}

//-----------------------------------------------------------------------------
// Sets the incremental orientation of the camera
//-----------------------------------------------------------------------------
void FPSCamera::rotate(float yaw, float pitch)
{
	mYaw += glm::radians(yaw);
	mPitch += glm::radians(pitch);

	// Constrain the pitch
	mPitch = glm::clamp(mPitch, -glm::pi<float>() / 2.0f + 0.1f, glm::pi<float>() / 2.0f - 0.1f);
	updateCameraVectors();
}

//-----------------------------------------------------------------------------
// Calculates the front vector from the Camera's (updated) Euler Angles
//-----------------------------------------------------------------------------
void FPSCamera::updateCameraVectors()
{
	// Calculate the view direction vector based on yaw and pitch angles (roll not considered) radius is 1 for normalized length
	glm::vec3 look;
	look.x = cosf(mPitch) * sinf(mYaw);
	look.y = sinf(mPitch);
	look.z = cosf(mPitch) * cosf(mYaw);

	mLook = glm::normalize(look);

	// Re-calculate the Right and Up vector.
	mRight = glm::normalize(glm::cross(mLook, WORLD_UP));
	mUp = glm::normalize(glm::cross(mRight, mLook));

	mTargetPos = mPosition + mLook;
}


//------------------------------------------------------------
// OrbitCamera - constructor
//------------------------------------------------------------
OrbitCamera::OrbitCamera()
	: mRadius(10.0f)
{}

//------------------------------------------------------------
// Sets the target to look at
//------------------------------------------------------------
void OrbitCamera::setLookAt(const glm::vec3& target)
{
	mTargetPos = target;
}

//------------------------------------------------------------
// Sets the radius of camera to target distance
//------------------------------------------------------------
void OrbitCamera::setRadius(float radius)
{
	// Clamp the radius
	mRadius = glm::clamp(radius, 2.0f, 80.0f);
}

//------------------------------------------------------------
// Rotates the camera around the target look at position given yaw and pitch in degrees.
//------------------------------------------------------------
void OrbitCamera::rotate(float yaw, float pitch)
{
	mYaw = glm::radians(yaw);
	mPitch = glm::radians(pitch);

	mPitch = glm::clamp(mPitch, -glm::pi<float>() / 2.0f + 0.1f, glm::pi<float>() / 2.0f - 0.1f);

	// Update Front, Right and Up Vectors using the updated Euler angles
	updateCameraVectors();
}

//------------------------------------------------------------
// Calculates the front vector from the Camera's
//------------------------------------------------------------
void OrbitCamera::updateCameraVectors()
{
	mPosition.x = mTargetPos.x + mRadius * cosf(mPitch) * sinf(mYaw);
	mPosition.y = mTargetPos.y + mRadius * sinf(mPitch);
	mPosition.z = mTargetPos.z + mRadius * cosf(mPitch) * cosf(mYaw);
}
