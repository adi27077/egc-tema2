#pragma once

#include <vector>

#include "components/simple_scene.h"
#include "lab_camera.h"
#include "car.h"


namespace m1
{
	class Tema2 final : public gfxc::SimpleScene
    {
     public:
        Tema2();
        ~Tema2() override;

        void Init() override;

		struct ViewportArea
		{
			ViewportArea() : x(0), y(0), width(1), height(1) {}
			ViewportArea(const int x, const int y, const int width, const int height)
				: x(x), y(y), width(width), height(height) {}
			int x;
			int y;
			int width;
			int height;
		};

     private:
    	void CreateCubeMesh(glm::vec3 color, const std::string& name);
    	void CreateGrassMesh(int size);
    	void CreateTrackMesh();
        void CreateTreeMesh();
		std::vector<glm::vec3> CreateCarTrajectory(float dist) const;

        void RenderTree(const glm::vec3& position, int projectionType);

        void FrameStart() override;
		void RenderScene(float deltaTimeSeconds, int projectionType);
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

		void RenderSimpleMesh(const ::Mesh* mesh, const ::Shader* shader, const glm::mat4& modelMatrix, int projectionType) const;

		bool sameSide(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& a, const glm::vec3& b) const;
		bool pointInTriangle(const glm::vec3& p, const glm::vec3& a, const glm::vec3& b, const glm::vec3& c) const;
		bool outOfTrack(const glm::vec3& position) const;
        bool enemyCarCollision() const;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

		const glm::vec3 OX = glm::vec3(1, 0, 0);
		const glm::vec3 OY = glm::vec3(0, 1, 0);

		float carSpeed = 3.0f;

		std::vector<car::Car> cars;

		float playerCarAngle = 0;
		glm::vec3 playerCarPosition{};

		my_camera::Camera* camera{};
		my_camera::Camera* minimapCamera{};
		glm::mat4 projectionMatrixPerspective{};
		glm::mat4 projectionMatrixOrtho{};
		const int PERSPECTIVE = 1;
		const int ORTHO = 2;
		ViewportArea miniViewport;

		std::vector<glm::vec3> outerTrackPoints;
		std::vector<glm::vec3> innerTrackPoints;

    	const std::vector<glm::vec3> trackPoints = {
	        glm::vec3(25.64f, 0.0f, 38.59f),
	        glm::vec3(29.17f, 0.0f, 42.92f),
	        glm::vec3(28.66f, 0.0f, 47.16f),
	        glm::vec3(24.02f, 0.0f, 49.37f),
	        glm::vec3(17.98f, 0.0f, 48.57f),
	        glm::vec3(12.54f, 0.0f, 47.05f),
	        glm::vec3(7.5f, 0.0f, 43.33f),
	        glm::vec3(7.7f, 0.0f, 38.18f),
	        glm::vec3(13.14f, 0.0f, 36.37f),
	        glm::vec3(17.17f, 0.0f, 32.94f),
	        glm::vec3(17.78f, 0.0f, 27.3f),
	        glm::vec3(10.0f, 0.0f, 20.0f),
	        glm::vec3(7.29f, 0.0f, 15.61f),
	        glm::vec3(10.62f, 0.0f, 6.23f),
	        glm::vec3(17.27f, 0.0f, 4.01f),
	        glm::vec3(25.54f, 0.0f, 4.62f),
	        glm::vec3(27.15f, 0.0f, 9.86f),
	        glm::vec3(25.54f, 0.0f, 15.91f),
	        glm::vec3(27.66f, 0.0f, 21.96f),
	        glm::vec3(34.51f, 0.0f, 22.46f),
	        glm::vec3(39.65f, 0.0f, 18.73f),
	        glm::vec3(45.2f, 0.0f, 21.25f),
	        glm::vec3(45.7f, 0.0f, 26.79f),
	        glm::vec3(43.68f, 0.0f, 31.83f),
	        glm::vec3(37.74f, 0.0f, 34.15f),
	        glm::vec3(32.8f, 0.0f, 33.55f),
	        glm::vec3(27.05f, 0.0f, 34.45f)
        };

		const std::vector<glm::vec3> treePositions = {
            glm::vec3(30, 0, 52),
            glm::vec3(20, 0, 52),
            glm::vec3(30, 0, 38),
            glm::vec3(25, 0, 45),
            glm::vec3(20, 0, 35),
			glm::vec3(15, 0, 41),
			glm::vec3(5, 0, 45),
			glm::vec3(0, 0, 38),
			glm::vec3(10, 0, 27),
			glm::vec3(10, 0, 34),
			glm::vec3(15, 0, 20),
			glm::vec3(10, 0, 52),
			glm::vec3(20, 0, 45),
			glm::vec3(30, 0, 27),
			glm::vec3(47, 0, 34),
			glm::vec3(38, 0, 38),
			glm::vec3(33, 0, 45),
			glm::vec3(38, 0, 52),
			glm::vec3(47, 0, 52),
			glm::vec3(50, 0, 27),
			glm::vec3(40, 0, 27),
			glm::vec3(22, 0, 27),
			glm::vec3(22, 0, 27),
			glm::vec3(36, 0, 15),
			glm::vec3(44, 0, 17),
			glm::vec3(30, 0, 17),
			glm::vec3(20, 0, 17),
			glm::vec3(14, 0, 17),
			glm::vec3(50, 0, 20),
			glm::vec3(40, 0, 24),
			glm::vec3(22, 0, 22),
			glm::vec3(4, 0, 20),
			glm::vec3(4, 0, 24),
			glm::vec3(35, 0, 10),
			glm::vec3(30, 0, 12),
			glm::vec3(20, 0, 8),
			glm::vec3(15, 0, 10),
			glm::vec3(5, 0, 7),
			glm::vec3(23, 0, 1),
			glm::vec3(16, 0, 1),
			glm::vec3(9, 0, 1),
			glm::vec3(30, 0, 5),
			glm::vec3(15, 0, 13),
			glm::vec3(4, 0, 14),
			glm::vec3(6, 0, 10),
		};

    };

}   // namespace m1
