#include "SimpleCamera.h"

#include "../Input/Input.h"

void SimpleCamera::OnUpdate()
{
	auto transform = Camera::current->transform;

	if (Input::IsKeyPressed(Key::W))
	{
		Vector3 forward = transform->Forward();
		transform->position += Vector3(forward * moveSpeed);
	}
	if (Input::IsKeyPressed(Key::S))
	{
		Vector3 forward = transform->Forward();
		transform->position -= Vector3(forward * moveSpeed);
	}
	if (Input::IsKeyPressed(Key::SPACE))
	{
		Vector3 up = transform->Up();
		transform->position += Vector3(up * moveSpeed);
	}
	if (Input::IsKeyPressed(Key::SHIFT))
	{
		Vector3 up = transform->Up();
		transform->position -= Vector3(up * moveSpeed);
	}
	if (Input::IsKeyPressed(Key::D))
	{
		Vector3 right = transform->Right();
		transform->position += Vector3(right * moveSpeed);
	}
	if (Input::IsKeyPressed(Key::A))
	{
		Vector3 right = transform->Right();
		transform->position -= Vector3(right * moveSpeed);
	}

	const auto delta = Input::GetMouseDelta();
	transform->rotation.x += delta.second * rotateSpeed;
	transform->rotation.y += delta.first * rotateSpeed;
}
