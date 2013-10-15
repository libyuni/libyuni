#ifndef __YUNI_GFX3D_CAMERA_H__
# define __YUNI_GFX3D_CAMERA_H__

# include "../../yuni.h"
# include "../../core/smartptr.h"
# include "../../core/point3D.h"
# include "../../core/vector3D.h"
# include "../../core/quaternion.h"

namespace Yuni
{
namespace Gfx3D
{


	/*!
	** \brief A camera defines a position and a frustum on a scene and ways to move in i
	**
	** By default, a camera is positioned at origin and looks toward -Z, with +Y as up.
	*/
	class Camera
	{
	public:
		//! Smart pointer
		typedef SmartPtr<Camera>  Ptr;

	public:
		static float defaultMovementSpeed;
		static float defaultRotationSpeed;


	public:
		//! Empty constructor, default values
		Camera();

		//! Constructor with default position
		Camera(const Yuni::Point3D<>& position);

		//! Constructor with default position and view vectors
		Camera(const Yuni::Point3D<>& position, const Yuni::Vector3D<>& direction,
			const Yuni::Vector3D<>& up);

		//! Constructor with specific movement speeds
		Camera(float movementSpeed, float rotationSpeed);

		//! Full constructor
		Camera(const Yuni::Point3D<>& position, const Yuni::Vector3D<>& direction,
			const Yuni::Vector3D<>& up, float movementSpeed, float rotationSpeed);

		//! Move the camera by a given translation vector (relative)
		void moveBy(float x, float y, float z);
		//! Move the camera by a given translation vector (relative)
		void moveBy(const Yuni::Vector3D<>& translation);

		//! Move the camera to a given point in space (absolute)
		void moveTo(const Yuni::Point3D<>& newPosition);

		//! Rotate by a certain oriented angle (in degrees)
		void rotateAroundX(float angle);
		void rotateAroundY(float angle);
		void rotateAroundZ(float angle);

		//! Current position
		const Yuni::Point3D<>& position() const;

		//! Current view vector, direction of the camera
		const Yuni::Vector3D<>& direction() const;

		//! Current up vector of the view
		const Yuni::Vector3D<>& up() const;

		//! Follow a target (3rd-person mode)
		void follow(const Point3D<>& target, float distance, float heightAngle);

		//! Set orientation of the camera to look along a given direction
		void lookAt(const Yuni::Vector3D<>& direction, const Yuni::Vector3D<>& up);

		//! Vertical field of view angle (degrees)
		float fov() const { return pFoV; }


	protected:
		//! Position of the camera
		Yuni::Point3D<> pPosition;

		//! View vector of the camera
		Yuni::Vector3D<> pView;

		//! Up vector of the camera
		Yuni::Vector3D<> pUp;

		//! Movement speed (m/s)
		float pMovementSpeed;

		//! Rotation speed (coefficient applied to the angular speed)
		float pRotationSpeed;

		//! Field of view (degrees)
		float pFoV;

	}; // class Camera



} // namespace Gfx3D
} // namespace Yuni

# include "camera.hxx"

#endif // __YUNI_GFX3D_CAMERA_H__
