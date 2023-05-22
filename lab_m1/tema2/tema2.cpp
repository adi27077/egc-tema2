#include "lab_m1/tema2/tema2.h"

#include <vector>
#include <string>
#include <iostream>

#include "components/camera.h"

using namespace std;
using namespace m1;
using namespace car;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema2::Tema2()
= default;


Tema2::~Tema2()
= default;

void Tema2::CreateCubeMesh(const glm::vec3 color, const string& name) {
	const vector<VertexFormat> vertices
	{
		VertexFormat(glm::vec3(-1, -1,  1), color, glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(1, -1,  1), color,glm::vec3(1, 0, 1)),
		VertexFormat(glm::vec3(-1,  1,  1), color, glm::vec3(1, 0, 0)),
		VertexFormat(glm::vec3(1,  1,  1), color,glm::vec3(0, 1, 0)),
		VertexFormat(glm::vec3(-1, -1, -1), color, glm::vec3(1, 1, 1)),
		VertexFormat(glm::vec3(1, -1, -1), color,glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(-1,  1, -1), color, glm::vec3(1, 1, 0)),
		VertexFormat(glm::vec3(1,  1, -1), color,glm::vec3(0, 0, 1)),
	};

	const vector<unsigned int> indices =
	{
		0, 1, 2,        1, 3, 2,
		2, 3, 7,        2, 7, 6,
		1, 7, 3,        1, 5, 7,
		6, 7, 4,        7, 5, 4,
		0, 4, 1,        1, 4, 5,
		2, 6, 4,        0, 2, 4,
	};

	meshes[name] = new Mesh(name);
	meshes[name]->InitFromData(vertices, indices);
}

void Tema2::CreateGrassMesh(const int size) {
	vector<VertexFormat> vertices;
	vector<unsigned int> indices;

	constexpr auto color = glm::vec3(0.2f, 0.94f, 0.37f);

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			vertices.emplace_back(glm::vec3(j, 0, i), color);
		}
	}

	for (int i = 0; i < size * (size - 1); i++) {
		if ((i + 1) % size == 0)
			continue;

		indices.emplace_back(i);
		indices.emplace_back(i + 1);
		indices.emplace_back(i + size);

		indices.emplace_back(i + 1);
		indices.emplace_back(i + size + 1);
		indices.emplace_back(i + size);
	}

	const string name = "grass_plane";
	meshes[name] = new Mesh(name);
	meshes[name]->InitFromData(vertices, indices);
}


void Tema2::CreateTrackMesh() {
	constexpr auto color = glm::vec3(0.2f, 0.2f, 0.2f);
	vector<VertexFormat> vertices;
	vector<unsigned int> indices;

	for (unsigned long long i = 0; i < trackPoints.size(); i++) {
		if (i == trackPoints.size() - 1) {
			auto d = trackPoints[0] - trackPoints[i];
			constexpr auto up = glm::vec3(0, 1, 0);
			auto p = normalize(cross(d, up));
			constexpr float dist = 2.5f;
			auto r = trackPoints[i] + dist * p;
			auto a = trackPoints[i] - dist * p;
			vertices.emplace_back(r, color);
			vertices.emplace_back(a, color);
			outerTrackPoints.emplace_back(r);
			innerTrackPoints.emplace_back(a);

			r = trackPoints[0] + dist * p;
			a = trackPoints[0] - dist * p;
			outerTrackPoints.emplace_back(r);
			innerTrackPoints.emplace_back(a);

			for (int j = 1; j < 50; j++) {
				auto currentPoint = trackPoints[i] + static_cast<float>(j) / 50.0f * d;
				auto d_aux = trackPoints[0] - currentPoint;
				auto p_aux = normalize(cross(d_aux, up));
				auto r_aux = currentPoint + dist * p_aux;
				auto a_aux = currentPoint - dist * p_aux;
				vertices.emplace_back(r_aux, color);
				vertices.emplace_back(a_aux, color);
			}

			continue;
		}


		auto d = trackPoints[i + 1] - trackPoints[i];
		constexpr auto up = glm::vec3(0, 1, 0);
		auto p = normalize(cross(d, up));
		constexpr float dist = 2.5f;
		auto r = trackPoints[i] + dist * p;
		auto a = trackPoints[i] - dist * p;
		vertices.emplace_back(r, color);
		vertices.emplace_back(a, color);
		outerTrackPoints.emplace_back(r);
		innerTrackPoints.emplace_back(a);

		r = trackPoints[i + 1] + dist * p;
		a = trackPoints[i + 1] - dist * p;
		outerTrackPoints.emplace_back(r);
		innerTrackPoints.emplace_back(a);

		for (int j = 1; j < 50; j++) {
			auto currentPoint = trackPoints[i] + static_cast<float>(j) / 50.0f * d;
			auto d_aux = trackPoints[i + 1] - currentPoint;
			auto p_aux = normalize(cross(d_aux, up));
			auto r_aux = currentPoint + dist * p_aux;
			auto a_aux = currentPoint - dist * p_aux;
			vertices.emplace_back(r_aux, color);
			vertices.emplace_back(a_aux, color);
		}
	}

	for (unsigned long long i = 0; i < vertices.size() - 2; i++) {

		if (i % 2 == 0) {
			indices.emplace_back(i);
			indices.emplace_back(i + 1);
			indices.emplace_back(i + 2);
		} else {
			indices.emplace_back(i);
			indices.emplace_back(i + 2);
			indices.emplace_back(i + 1);
		}
	}

	indices.emplace_back(vertices.size() - 2);
	indices.emplace_back(vertices.size() - 1);
	indices.emplace_back(0);

	indices.emplace_back(vertices.size() - 1);
	indices.emplace_back(1);
	indices.emplace_back(0);

	const string name = "track";
	meshes[name] = new Mesh(name);
	meshes[name]->InitFromData(vertices, indices);
}

vector<glm::vec3> Tema2::CreateCarTrajectory(const float dist) const {
	vector<glm::vec3> trajectory;

	for (unsigned long long i = 0; i < trackPoints.size(); i++) {
		if (i == trackPoints.size() - 1) {
			auto d = trackPoints[0] - trackPoints[i];
			constexpr auto up = glm::vec3(0, 1, 0);
			auto p = normalize(cross(d, up));
			trajectory.emplace_back(trackPoints[i] + dist * p);

			continue;
		}

		auto d = trackPoints[i + 1] - trackPoints[i];
		constexpr auto up = glm::vec3(0, 1, 0);
		auto p = normalize(cross(d, up));
		trajectory.emplace_back(trackPoints[i] + dist * p);
		
	}
	return trajectory;
}

void Tema2::CreateTreeMesh() {
	CreateCubeMesh(glm::vec3(0.1f, 0.41f, 0.16f), "tree_crown");
	CreateCubeMesh(glm::vec3(0.4f, 0.25f, 0.15f), "tree_trunk");
}

void Tema2::RenderTree(const glm::vec3& position, const int projectionType) {
	{
		auto modelMatrix = glm::mat4(1);
		modelMatrix = translate(modelMatrix, position);
		modelMatrix = scale(modelMatrix, glm::vec3(0.25f, 1, 0.25f));
		RenderSimpleMesh(meshes["tree_trunk"], shaders["MyShader"], modelMatrix, projectionType);
	}

	{
		auto modelMatrix = glm::mat4(1);
		modelMatrix = translate(modelMatrix, position + glm::vec3(0, 2, 0));
		modelMatrix = scale(modelMatrix, glm::vec3(1, 1, 1));
		RenderSimpleMesh(meshes["tree_crown"], shaders["MyShader"], modelMatrix, projectionType);
	}
}


void Tema2::Init()
{
	CreateCubeMesh(glm::vec3(0, 1, 1),"cube1");
	CreateGrassMesh(1000);
	CreateTrackMesh();
	CreateTreeMesh();
	CreateCubeMesh(glm::vec3(1, 0, 0), "car1");
	CreateCubeMesh(glm::vec3(1, 1, 0), "car2");
	CreateCubeMesh(glm::vec3(0, 0, 1), "car3");
	CreateCubeMesh(glm::vec3(1, 0, 1), "car4");
	CreateCubeMesh(glm::vec3(0, 1, 1), "player_car");

	const auto trajectory2 = CreateCarTrajectory(1.5f);
	const auto trajectory3 = CreateCarTrajectory(-1.5f);
	const auto trajectory4 = CreateCarTrajectory(0.6f);

	cars.emplace_back("car1", 12, trackPoints, trackPoints[12], 2.0f);
	cars.emplace_back("car2", 0, trajectory2, trajectory2[0], 4.0f);
	cars.emplace_back("car3", 6, trajectory3, trajectory3[6], 6.0f);
	cars.emplace_back("car4", 3, trajectory4, trajectory4[3], 8.0f);
	playerCarPosition = trackPoints[14];

	camera = new my_camera::Camera();
	camera->Set(playerCarPosition + glm::vec3(-2.4f, 1.5f, 0.0f), playerCarPosition + glm::vec3(0.0f, 1.0f, 0.0f), OY);
	camera->distanceToTarget = 3.0f;

	minimapCamera = new my_camera::Camera();
	minimapCamera->Set(playerCarPosition + glm::vec3(0.0f, 25.0f, 0.0f), playerCarPosition, OX);

	projectionMatrixPerspective = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
	projectionMatrixOrtho = glm::ortho(-15.0f, 15.0f, -15.0f, 15.0f, 0.01f, 200.0f);

	// Create a shader program for drawing face polygon with the color of the normal
	{
		const auto shader = new Shader("MyShader");
		shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
		shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

}


void Tema2::FrameStart()
{
	// Clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0.75f, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	const glm::ivec2 resolution = window->GetResolution();
	// Sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);

	const auto viewportX = static_cast<int>(static_cast<float>(resolution.x) * 5.f / 6.f) - 20;
	constexpr auto viewportY = 20;
	const auto viewportWidth = static_cast<int>(static_cast<float>(resolution.x) / 6.f);
	const auto viewportHeight = static_cast<int>(static_cast<float>(resolution.y) / 6.f);
	miniViewport = ViewportArea(viewportX, viewportY, viewportWidth, viewportHeight);
}

void Tema2::RenderScene(const float deltaTimeSeconds, const int projectionType) {
	RenderTree(glm::vec3(0, 0, 0), projectionType);

	{
		auto modelMatrix = glm::mat4(1);
		modelMatrix = translate(modelMatrix, glm::vec3(-25, 0, -25));
		modelMatrix = scale(modelMatrix, glm::vec3(.1f, 1, .1f));
		RenderSimpleMesh(meshes["grass_plane"], shaders["MyShader"], modelMatrix, projectionType);
	}

	{
		auto modelMatrix = glm::mat4(1);
		modelMatrix = translate(modelMatrix, glm::vec3(0, 0.2f, 0));
		RenderSimpleMesh(meshes["track"], shaders["MyShader"], modelMatrix, projectionType);
	}

	for (const auto& position : treePositions) {
		RenderTree(position, projectionType);
	}

	for (auto& car : cars) {
		car.computeEnemyCarMovement(deltaTimeSeconds);
		auto modelMatrix = glm::mat4(1);
		modelMatrix = translate(modelMatrix, car.getCurrentPosition() + glm::vec3(0, 0.35f, 0));
		modelMatrix = glm::rotate(modelMatrix, car.getAngle(), glm::vec3(0, 1, 0));
		modelMatrix = scale(modelMatrix, glm::vec3(0.25f, 0.25f, 0.5f));
		RenderSimpleMesh(meshes[car.getName()], shaders["MyShader"], modelMatrix, projectionType);
	}

	{
		auto modelMatrix = glm::mat4(1);
		modelMatrix = translate(modelMatrix, playerCarPosition - glm::vec3(0, 0.5f, 0));
		modelMatrix = rotate(modelMatrix, playerCarAngle, glm::vec3(0, 1, 0));
		modelMatrix = rotate(modelMatrix, RADIANS(90), glm::vec3(0, 1, 0));
		modelMatrix = scale(modelMatrix, glm::vec3(0.25f, 0.25f, 0.5f));
		RenderSimpleMesh(meshes["player_car"], shaders["MyShader"], modelMatrix, projectionType);
	}
}


void Tema2::Update(const float deltaTimeSeconds)
{
	playerCarPosition = camera->GetTargetPosition();
	minimapCamera->Set(playerCarPosition + glm::vec3(0.0f, 25.0f, 0.0f), playerCarPosition, OX);

	RenderScene(deltaTimeSeconds, PERSPECTIVE);

	glViewport(miniViewport.x, miniViewport.y, miniViewport.width, miniViewport.height);

	RenderScene(deltaTimeSeconds, ORTHO);

}


void Tema2::FrameEnd()
{
	//DrawCoordinateSystem();
}

void Tema2::RenderSimpleMesh(const Mesh* mesh, const Shader* shader, const glm::mat4& modelMatrix, const int projectionType) const {
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// Render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	// TODO(student): Get shader location for uniform mat4 "Model"
	const GLint loc_model = glGetUniformLocation(shader->program, "Model");

	// TODO(student): Set shader uniform "Model" to modelMatrix
	glUniformMatrix4fv(loc_model, 1, GL_FALSE, value_ptr(modelMatrix));

	// TODO(student): Get shader location for uniform mat4 "View"
	const GLint loc_view = glGetUniformLocation(shader->program, "View");

	// TODO(student): Set shader uniform "View" to viewMatrix
	glm::mat4 viewMatrix = projectionType == PERSPECTIVE ? camera->GetViewMatrix() : minimapCamera->GetViewMatrix();
	glUniformMatrix4fv(loc_view, 1, GL_FALSE, value_ptr(viewMatrix));

	// TODO(student): Get shader location for uniform mat4 "Projection"
	const GLint loc_projection = glGetUniformLocation(shader->program, "Projection");

	// TODO(student): Set shader uniform "Projection" to projectionMatrix
	glm::mat4 projectionMatrix = projectionType == PERSPECTIVE ? projectionMatrixPerspective : projectionMatrixOrtho;
	glUniformMatrix4fv(loc_projection, 1, GL_FALSE, value_ptr(projectionMatrix));

	//Get shader location for uniform vec3 "CarPosition"
	const GLint loc_car_position = glGetUniformLocation(shader->program, "CarPosition");

	//Set shader uniform "CarPosition" to playerCarPosition
	glUniform3fv(loc_car_position, 1, value_ptr(playerCarPosition));

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->m_VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, nullptr);
}

bool Tema2::sameSide(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& a, const glm::vec3& b) const {
	const glm::vec3 cp1 = cross(b - a, p1 - a);
	const glm::vec3 cp2 = cross(b - a, p2 - a);
	if (dot(cp1, cp2) >= 0) {
		return true;
	}
	return false;
}

bool Tema2::pointInTriangle(const glm::vec3& p, const glm::vec3& a, const glm::vec3& b, const glm::vec3& c) const {
	if (sameSide(p, a, b, c) && sameSide(p, b, a, c) && sameSide(p, c, a, b)) {
		return true;
	}
	return false;
}

bool Tema2::outOfTrack(const glm::vec3& position) const {
	for (unsigned long long i = 0; i < innerTrackPoints.size(); i++) {
		const unsigned long long next = i == innerTrackPoints.size() - 1 ? 0 : i + 1;
		auto triangleA = glm::vec3(innerTrackPoints[i].x, 0, innerTrackPoints[i].z);
		auto triangleB = glm::vec3(outerTrackPoints[i].x, 0, outerTrackPoints[i].z);
		auto triangleC = glm::vec3(innerTrackPoints[next].x, 0, innerTrackPoints[next].z);
		if (pointInTriangle(position, triangleA, triangleB, triangleC)) {
			return false;
		}
		triangleA = glm::vec3(outerTrackPoints[i].x, 0, outerTrackPoints[i].z);
		triangleB = glm::vec3(outerTrackPoints[next].x, 0, outerTrackPoints[next].z);
		triangleC = glm::vec3(innerTrackPoints[next].x, 0, innerTrackPoints[next].z);
		if (pointInTriangle(position, triangleA, triangleB, triangleC)) {
			return false;
		}

	}
	return true;
}

bool Tema2::enemyCarCollision() const {
	return std::any_of(cars.begin(), cars.end(), [this](const Car& car) {
		if (sqrt(pow(car.getCurrentPosition().x - playerCarPosition.x, 2.f) + pow(car.getCurrentPosition().z - playerCarPosition.z, 2.f)) < .8f) {
			return true;
		}
		return false;
	});
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema2::OnInputUpdate(const float deltaTime, int mods)
{
	const auto playerCarDirection = normalize(camera->forward);
	const auto nextPosition = playerCarPosition + playerCarDirection * deltaTime * carSpeed;
	const auto backPosition = playerCarPosition - playerCarDirection * deltaTime * carSpeed;
	const bool collisionFront = outOfTrack(glm::vec3(nextPosition.x, 0, nextPosition.z));
	const bool collisionBack = outOfTrack(glm::vec3(backPosition.x, 0, backPosition.z));
	const bool carCollision = enemyCarCollision();
	// Add key press event
	if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT) && !carCollision) {
		constexpr float cameraSpeed = 1.4f;

		if (window->KeyHold(GLFW_KEY_W) && !collisionFront) {
			camera->MoveForward(deltaTime * cameraSpeed * carSpeed);
		}

		if (window->KeyHold(GLFW_KEY_D)) {
			camera->RotateThirdPerson_OY(-deltaTime * cameraSpeed);
			playerCarAngle -= deltaTime * cameraSpeed;
		}

		if (window->KeyHold(GLFW_KEY_S) && !collisionBack) {
			camera->MoveForward(-deltaTime * cameraSpeed * carSpeed);
		}

		if (window->KeyHold(GLFW_KEY_A)) {
			camera->RotateThirdPerson_OY(deltaTime * cameraSpeed);
			playerCarAngle += deltaTime * cameraSpeed;
		}
	}
}


void Tema2::OnKeyPress(int key, int mods)
{
	// Add key press event
}


void Tema2::OnKeyRelease(int key, int mods)
{
	// Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// Add mouse move event
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// Add mouse button press event
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}
