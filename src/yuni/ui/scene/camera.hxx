#ifndef __YUNI_GFX3D_CAMERA_HXX__
# define __YUNI_GFX3D_CAMERA_HXX__

namespace Yuni
{
namespace Gfx3D
{



	inline void Camera::moveBy(float x, float y, float z)
	{
		pPosition.translate(x, y, z);
	}


	inline void Camera::moveBy(const Vector3D<>& translation)
	{
		pPosition.translate(translation.x, translation.y, translation.z);
	}


	inline void Camera::moveTo(const Point3D<>& newPosition)
	{
		pPosition = newPosition;
	}


	inline const Point3D<>& Camera::position() const
	{
		return pPosition;
	}


	inline const Vector3D<>& Camera::direction() const
	{
		return pView;
	}


	inline const Vector3D<>& Camera::up() const
	{
		return pUp;
	}


	inline void Camera::follow(const Point3D<>& target, float distance, float heightAngle)
	{
		pPosition.x = (float)(distance * cos(Math::DegreeToRadian(heightAngle + 270.0f)) + target.x);
		pPosition.z = (float)(distance * sin(Math::DegreeToRadian(heightAngle - 270.0f)) + target.z);
		Vector3D<> direction(pPosition, target);
		lookAt(direction, pUp);
	}


	inline void Camera::lookAt(const Yuni::Vector3D<>& direction, const Yuni::Vector3D<>& up)
	{
		pView = direction;
		pUp = up;
	}



} // namespace Gfx3D
} // namespace Yuni

#endif // __YUNI_GFX3D_CAMERA_HXX__
