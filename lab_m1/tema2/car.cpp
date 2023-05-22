#include "car.h"

using namespace car;

void Car::computeEnemyCarMovement(const float deltaTimeSeconds) {
	const auto carDirection = points[pointIndex == points.size() - 1 ? 0 : pointIndex + 1] - currentPosition;

	if (justStarted) {
		justStarted = false;
		angle = glm::acos(dot(normalize(carDirection), initialDirection));
	}

	if (length(carDirection) < 0.1f) {
		//Trajectory is changed, need to update direction and rotation
		pointIndex++;
		if (pointIndex == points.size()) {
			pointIndex = 0;
		}
		currentPosition = points[pointIndex];
		const auto newDirection = points[pointIndex == points.size() - 1 ? 0 : pointIndex + 1] - currentPosition;
		currentPosition += normalize(newDirection) * deltaTimeSeconds * speed;
		if (normalize(newDirection).x < 0) {
			angle = -glm::acos(dot(normalize(newDirection), initialDirection));
		}
		else {
			angle = glm::acos(dot(normalize(newDirection), initialDirection));
		}
	}
	else {
		currentPosition += normalize(carDirection) * deltaTimeSeconds * speed;
	}
}
