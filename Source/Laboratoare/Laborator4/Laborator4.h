//#pragma once
//#include <Component/SimpleScene.h>
//
//class Laborator4 : public SimpleScene
//{
//	public:
//		Laborator4();
//		~Laborator4();
//
//		void Init() override;
//
//	private:
//		void FrameStart() override;
//		void Update(float deltaTimeSeconds) override;
//		void FrameEnd() override;
//
//		void OnInputUpdate(float deltaTime, int mods) override;
//		void OnKeyPress(int key, int mods) override;
//		void OnKeyRelease(int key, int mods) override;
//		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
//		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
//		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
//		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
//		void OnWindowResize(int width, int height) override;
//
//	protected:
//		glm::mat4 modelMatrix;
//		float translateX, translateY, translateZ;
//		float scaleX, scaleY, scaleZ;
//		float angularStepOX, angularStepOY, angularStepOZ;
//		GLenum polygonMode;
//};

#pragma once
#include <Component/SimpleScene.h>
#include "Pocket.h"
#include "Ball.h"


class Laborator4 : public SimpleScene
{
public:
	Laborator4();
	~Laborator4();

	void Init() override;

private:
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;
	void RenderMeshWithColor(Mesh *mesh, glm::mat4 & modelMatrix, glm::vec3 color);
	void RenderStickMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix, float charge);

protected:
	enum State {
		WHITE_PLACEMENT,
		CHARGING,
		PLAYING
	};
	struct Collision {
		enum Type {
			BALL,
			POCKET,
			PLANE
		};

		Type type;
		glm::vec3 pos;
		glm::vec3 normal;
		Pocket *pocket;
		Ball *ball;
	};
	struct Player{
		Player() {
			reset();
		}
		void reset() {
			color = Ball::WHITE;
		}

		Ball::Color color;
	};
	std::vector<Pocket*> pockets;
	std::vector<Ball*> balls;
	std::vector<Collision> collisions;
	std::vector<Ball::Color> pocketBalls;
	Player players[2];
	int currentPlayer;
	Ball *whiteBall;
	State state;
	float stickAngle;
	float stickCharge;
	float chargeTime;
	void render(Pocket *pocket);
	void render(Ball *ball);
	float integrate (float deltaTimeSeconds);
	float getTimeToCollision (Ball * ball, Collision &collision, float deltaTime);
	void solveCollision (Ball * ball, Collision &collision, float deltaTime);
	void integrateBallVelocity (Ball *ball, float deltaTime);
	void integrateBallForces (Ball *ball, float deltaTime);
	bool isBall (Collision &collision, Ball * ball);
};
