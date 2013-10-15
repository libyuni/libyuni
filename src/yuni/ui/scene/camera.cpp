
#include "../../core/math/trigonometric.h"
#include "camera.h"

namespace Yuni
{
namespace Gfx3D
{

	float Camera::defaultMovementSpeed = 0.5f;
	float Camera::defaultRotationSpeed = 1.5f;



	Camera::Camera():
		pPosition(0.0f, 0.0f, 0.0f),
		pView(0.0f, 0.0f, -1.0f),
		pUp(0.0f, 1.0f, 0.0f),
		pMovementSpeed(defaultMovementSpeed),
		pRotationSpeed(defaultRotationSpeed),
		pFoV(60.0f)
	{}


	Camera::Camera(const Yuni::Point3D<>& position):
		pPosition(position),
		pView(0.0f, 0.0f, -1.0f),
		pUp(0.0f, 1.0f, 0.0f),
		pMovementSpeed(defaultMovementSpeed),
		pRotationSpeed(defaultRotationSpeed),
		pFoV(60.0f)
	{}


	Camera::Camera(const Yuni::Point3D<>& position, const Yuni::Vector3D<>& direction,
		const Yuni::Vector3D<>& up):
		pPosition(position),
		pView(direction),
		pUp(up),
		pMovementSpeed(defaultMovementSpeed),
		pRotationSpeed(defaultRotationSpeed),
		pFoV(60.0f)
	{}


	Camera::Camera(float movementSpeed, float rotationSpeed):
		pPosition(0.0f, 0.0f, 0.0f),
		pView(0.0f, 0.0f, -1.0f),
		pUp(0.0f, 1.0f, 0.0f),
		pMovementSpeed(movementSpeed),
		pRotationSpeed(rotationSpeed),
		pFoV(60.0f)
	{}


	Camera::Camera(const Yuni::Point3D<>& position, const Yuni::Vector3D<>& direction,
		const Yuni::Vector3D<>& up, float movementSpeed, float rotationSpeed):
		pPosition(position),
		pView(direction),
		pUp(up),
		pMovementSpeed(movementSpeed),
		pRotationSpeed(rotationSpeed),
		pFoV(60.0f)
	{}



} // namespace Gfx3D
} // namespace Yuni
