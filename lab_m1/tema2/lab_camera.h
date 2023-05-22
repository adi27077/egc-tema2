#pragma once

namespace my_camera
{
    class Camera
    {
     public:
        Camera()
        {
            position    = glm::vec3(0, 2, 5);
            forward     = glm::vec3(0, 0, -1);
            up          = glm::vec3(0, 1, 0);
            right       = glm::vec3(1, 0, 0);
            distanceToTarget = 2;
        }

        Camera(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up)
        {
            Set(position, center, up);
        }

        ~Camera()
        = default;

        // Update camera
        void Set(const glm::vec3 &position1, const glm::vec3 &center, const glm::vec3 &up1)
        {
            this->position = position1;
            forward     = normalize(center - position1);
            right       = cross(forward, up1);
            this->up    = cross(right, forward);
        }

        void MoveForward(const float distance)
        {
            // Translates the camera using the `dir` vector computed from
            // `forward`. Movement will always keep the camera at the same
            // height. For example, if you rotate your head up/down, and then
            // walk forward, then you will still keep the same relative
            // distance (height) to the ground!
            const glm::vec3 dir = normalize(glm::vec3(forward.x, 0, forward.z));
            position += dir * distance;
        }

        void TranslateForward(const float distance)
        {
            // TODO(student): Translate the camera using the `forward` vector.
            // What's the difference between `TranslateForward()` and
            // `MoveForward()`?
            position += normalize(forward) * distance;
        }

        void TranslateUpward(const float distance)
        {
            // TODO(student): Translate the camera using the `up` vector.
            position += distance * normalize(up);

        }

        void TranslateRight(const float distance)
        {
            // TODO(student): See instructions below. Read the entire thing!
            // You need to translate the camera using the `right` vector.
            // Usually, however, translation using camera's `right` vector
            // is not very useful, because if the camera is rotated around the
            // `forward` vector, then the translation on the `right` direction
            // will have an undesired effect, more precisely, the camera will
            // get closer or farther from the ground. The solution is to
            // actually use the projected `right` vector (projected onto the
            // ground plane), which makes more sense because we will keep the
            // same distance from the ground plane.
            position += distance * normalize(right);
        }

        void RotateFirstPerson_OX(const float angle)
        {
            // TODO(student): Compute the new `forward` and `up` vectors.
            // Don't forget to normalize the vectors! Use `glm::rotate()`.
            forward = normalize(rotate(glm::mat4(1.f), angle, right) * glm::vec4(forward, 0));
            up = normalize(cross(right, forward));
        }

        void RotateFirstPerson_OY(const float angle)
        {
            // TODO(student): Compute the new `forward`, `up` and `right`
            // vectors. Use `glm::rotate()`. Don't forget to normalize the
            // vectors!
            forward = normalize(rotate(glm::mat4(1.f), angle, glm::vec3(0, 1, 0)) * glm::vec4(forward, 0));
            right = normalize(rotate(glm::mat4(1.f), angle, glm::vec3(0, 1, 0)) * glm::vec4(right, 0));
            up = normalize(cross(right, forward));
        }

        void RotateFirstPerson_OZ(const float angle)
        {
            // TODO(student): Compute the new `right` and `up`. This time,
            // `forward` stays the same. Use `glm::rotate()`. Don't forget
            // to normalize the vectors!
            right = normalize(rotate(glm::mat4(1.f), angle, forward) * glm::vec4(right, 0));
            up = normalize(rotate(glm::mat4(1.f), angle, forward) * glm::vec4(up, 0));
            
        }

        void RotateThirdPerson_OX(const float angle)
        {
            // TODO(student): Rotate the camera in third-person mode around
            // the OX axis. Use `distanceToTarget` as translation distance.
            TranslateForward(distanceToTarget);
            RotateFirstPerson_OX(angle);
            TranslateForward(-distanceToTarget);
        }

        void RotateThirdPerson_OY(const float angle)
        {
            // TODO(student): Rotate the camera in third-person mode around
            // the OY axis.
            TranslateForward(distanceToTarget);
            RotateFirstPerson_OY(angle);
            TranslateForward(-distanceToTarget);
        }

        void RotateThirdPerson_OZ(const float angle)
        {
            // TODO(student): Rotate the camera in third-person mode around
            // the OZ axis.
            TranslateForward(distanceToTarget);
            RotateFirstPerson_OZ(angle);
            TranslateForward(-distanceToTarget);
        }

        glm::mat4 GetViewMatrix() const {
            // Returns the view matrix
            return lookAt(position, position + forward, up);
        }

        glm::vec3 GetTargetPosition() const {
            return position + forward * distanceToTarget;
        }

        float distanceToTarget{};
        glm::vec3 position{};
        glm::vec3 forward{};
        glm::vec3 right{};
        glm::vec3 up{};
    };
}   // namespace my_camera
