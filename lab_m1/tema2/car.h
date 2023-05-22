#pragma once

#include "components/simple_scene.h"

namespace car
{
	class Car {
	 public:

		Car() = default;
		Car(std::string name, const unsigned long long positionIndex, std::vector<glm::vec3> points, const glm::vec3 currentPosition, const float speed)
			: name(std::move(name)), pointIndex(positionIndex), points(std::move(points)), currentPosition(currentPosition), speed(speed) {
		}


		void computeEnemyCarMovement(float deltaTimeSeconds);
		std::string getName() const { return name; }
		glm::vec3 getCurrentPosition() const { return currentPosition; }
		float getAngle() const { return angle; }

	 private:
		std::string name;
		unsigned long long pointIndex{};
		std::vector<glm::vec3> points;
		glm::vec3 currentPosition{};
		float angle{};
		float speed = 1.0f;
		glm::vec3 initialDirection = glm::vec3(0, 0, 1);
		bool justStarted = true;
	};
		
}
