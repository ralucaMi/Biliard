
#include "Laborator4.h"

#include <vector>
#include <string>
#include <iostream>
#include "Object3D.h"

#include <Core/Engine.h>
#include "Transform3D.h"
#include <include/gl.h>

using namespace std;
# define PI           3.14159265358979323846f
glm::vec3 verdeDeschis (0, 153.0f / 255.0f, 0);
glm::vec3 verdeInchis (0, 76.0f / 255.0f, 0);
float width = 1.0f;
float height = 1.7f;
float depth = 0.1f;
float thickness = 0.07f;
float ballRadius = 0.035f;
float stickRadius = 0.01f;
float stickLength = 1.0f;
float minVelocity = 0.012f;

Laborator4::Laborator4()
{
	state = WHITE_PLACEMENT;
	stickAngle = 0.0f;
	stickCharge = 0.0f;
	chargeTime = 0.0f;
	currentPlayer = 0;
}

Laborator4::~Laborator4()
{
}

void Laborator4::Init()
{
	// Create a shader program for drawing the stick wiht vertex colors
	{
		Shader *shader = new Shader("StickVertexColor");
		shader->AddShader(RESOURCE_PATH::SHADERS + "Stick.MVP.Texture.VS.glsl", GL_VERTEX_SHADER);
		shader->AddShader(RESOURCE_PATH::SHADERS + "VertexColor.FS.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	// build the table
	Mesh * mesh = Object3D::CreateBox ("table", glm::vec3 (-width * 0.5f, -depth, -height * 0.5f), glm::vec3 (width * 0.5f, 0.0f, height * 0.5f), 
		verdeDeschis, verdeDeschis, verdeDeschis, verdeDeschis, verdeDeschis, verdeDeschis);
	AddMeshToList (mesh);

	// build the up wall
	mesh = Object3D::CreateBox ("wallup", glm::vec3 (-width * 0.5 - thickness, 0.0f, height * 0.5f), glm::vec3 (width * 0.5f + thickness, thickness, height * 0.5f + thickness),
		verdeDeschis, verdeDeschis, verdeInchis, verdeInchis, verdeDeschis, verdeInchis);
	AddMeshToList (mesh);
	Collision collision;
	collision.type = Collision::PLANE;
	collision.pos = glm::vec3 (0.0f, 0.0f, height * 0.5f);
	collision.normal = glm::vec3 (0.0f, 0.0f, 1.0f);
	collisions.push_back (collision);
	
	// build the down wall
	mesh = Object3D::CreateBox ("walldown", glm::vec3 (-width * 0.5 - thickness, 0.0f, -height * 0.5f - thickness), glm::vec3 (width * 0.5f + thickness, thickness, -height * 0.5f),
		verdeDeschis, verdeDeschis, verdeInchis, verdeInchis, verdeInchis, verdeDeschis);
	AddMeshToList (mesh);
	collision.type = Collision::PLANE;
	collision.pos = glm::vec3 (0.0f, 0.0f, -height * 0.5f);
	collision.normal = glm::vec3 (0.0f, 0.0f, -1.0f);
	collisions.push_back (collision);
	
	// build the left wall
	mesh = Object3D::CreateBox ("wallleft", glm::vec3 (-width * 0.5 - thickness, 0.0f, -height * 0.5f - thickness), glm::vec3 (-width * 0.5f, thickness, height * 0.5f + thickness),
		verdeInchis, verdeDeschis, verdeInchis, verdeInchis, verdeDeschis, verdeDeschis);
	AddMeshToList (mesh);
	collision.type = Collision::PLANE;
	collision.pos = glm::vec3 (-width*0.5f, 0.0f, 0.0f);
	collision.normal = glm::vec3 (-1.0f, 0.0f, 0.0f);
	collisions.push_back (collision);
	
	// build the right wall
	mesh = Object3D::CreateBox ("wallright", glm::vec3 (width * 0.5, 0.0f, -height * 0.5f - thickness), glm::vec3 (width * 0.5f + thickness, thickness, height * 0.5f + thickness),
		verdeDeschis, verdeInchis, verdeInchis, verdeInchis, verdeDeschis, verdeDeschis);
	AddMeshToList (mesh);
	collision.type = Collision::PLANE;
	collision.pos = glm::vec3 (width*0.5f, 0.0f, 0.0f);
	collision.normal = glm::vec3 (1.0f, 0.0f, 0.0f);
	collisions.push_back (collision);

	glm::vec3 stickColor ( (float)0x99 / 255.0f,  (float)0x66 / 255.0f, (float)0x66 / 255.0f);
	mesh = Object3D::CreateBox ("stick", glm::vec3 (-stickRadius, -stickRadius, -stickRadius), 
		glm::vec3 (stickRadius, stickRadius, -stickRadius + stickLength),
		stickColor, stickColor, stickColor, stickColor, stickColor, stickColor);
	AddMeshToList(mesh);


	// Pockets
	mesh = new Mesh("sphere");
	mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
	AddMeshToList(mesh);

	pockets.push_back (new Pocket (glm::vec3 (-width * 0.5f, 0.0, height * 0.5f), thickness));
	pockets.push_back (new Pocket (glm::vec3 (width * 0.5f, 0.0, height * 0.5f), thickness ));
	pockets.push_back (new Pocket (glm::vec3 (-width * 0.5f, 0.0, -height * 0.5f), thickness));
	pockets.push_back (new Pocket (glm::vec3 (width * 0.5f, 0.0, -height * 0.5f), thickness));
	pockets.push_back (new Pocket (glm::vec3 (-width * 0.5f, 0.0, 0.0), thickness));
	pockets.push_back (new Pocket (glm::vec3 (width * 0.5f, 0.0, 0.0), thickness));
	for (int i = 0; i < pockets.size(); ++i) {
		collision.type = Collision::POCKET;
		collision.pocket = pockets[i];
		collisions.push_back(collision);
	}

	float sx = sinf (30.0f * PI / 180.0f) * ballRadius * 2.0001f;
	float sz = cosf (30.0f * PI / 180.0f) * ballRadius * 2.0001f;
	float x = 0.0f;
	float z = -height * 0.2f;
	balls.push_back (new Ball (glm::vec3 (x, ballRadius, z), ballRadius, Ball::RED));

	balls.push_back (new Ball (glm::vec3 (x - sx, ballRadius, z - sz), ballRadius, Ball::RED));
	balls.push_back (new Ball (glm::vec3 (x + sx, ballRadius, z - sz), ballRadius, Ball::YELLOW));

	balls.push_back (new Ball (glm::vec3 (x - sx * 2, ballRadius, z - sz * 2), ballRadius, Ball::YELLOW));
	balls.push_back (new Ball (glm::vec3 (x - sx * 0, ballRadius, z - sz * 2), ballRadius, Ball::BLACK));
	balls.push_back (new Ball (glm::vec3 (x + sx * 2, ballRadius, z - sz * 2), ballRadius, Ball::RED));

	balls.push_back (new Ball (glm::vec3 (x - sx * 3, ballRadius, z - sz * 3), ballRadius, Ball::YELLOW));
	balls.push_back (new Ball (glm::vec3 (x - sx * 1, ballRadius, z - sz * 3), ballRadius, Ball::RED));
	balls.push_back (new Ball (glm::vec3 (x + sx * 1, ballRadius, z - sz * 3), ballRadius, Ball::RED));
	balls.push_back (new Ball (glm::vec3 (x + sx * 3, ballRadius, z - sz * 3), ballRadius, Ball::YELLOW));

	balls.push_back (new Ball (glm::vec3 (x - sx * 4, ballRadius, z - sz * 4), ballRadius, Ball::RED));
	balls.push_back (new Ball (glm::vec3 (x - sx * 2, ballRadius, z - sz * 4), ballRadius, Ball::RED));
	balls.push_back (new Ball (glm::vec3 (x - sx * 0, ballRadius, z - sz * 4), ballRadius, Ball::YELLOW));
	balls.push_back (new Ball (glm::vec3 (x + sx * 2, ballRadius, z - sz * 4), ballRadius, Ball::YELLOW));
	balls.push_back (new Ball (glm::vec3 (x + sx * 4, ballRadius, z - sz * 4), ballRadius, Ball::RED));

	whiteBall = new Ball (glm::vec3 (0.0f,ballRadius, height * 0.35f), ballRadius, Ball::WHITE);
	balls.push_back(whiteBall);
	for (int i = 0; i < balls.size(); ++i) {
		Collision collision;
		collision.type = Collision::BALL;
		collision.ball = balls[i];
		collisions.push_back (collision);
	}

	GetCameraInput()->SetActive(false);

	cout << "Player " << currentPlayer << "\n";
}

void Laborator4::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Laborator4::Update(float deltaTimeSeconds)
{
	glLineWidth(3);
	glPointSize(5);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Render stick
	glm::mat4 stickMatrix (1);
	glm::vec3 stickPivot = whiteBall->getPosition ();
	stickMatrix *= Transform3D::Translate (stickPivot.x, stickPivot.y, stickPivot.z);
	stickMatrix *= Transform3D::RotateOY (stickAngle * PI / 180.0f);
	switch (state) {
		case CHARGING:
			if (window->MouseHold (GLFW_MOUSE_BUTTON_LEFT)){
				chargeTime += deltaTimeSeconds;
				stickCharge = 0.7f - (cosf (chargeTime * 4.f) * 0.35f + 0.35f);
			}
			else {
				if (stickCharge > minVelocity) {
					whiteBall->setVelocity (glm::vec3 (-sinf (stickAngle * PI / 180) * stickCharge, 0.0f, -cosf (stickAngle * PI / 180) * stickCharge));
					state = PLAYING;
					pocketBalls.clear ();
				}
			}
		break;
	}

	// camera setup
	switch (state) {
		case CHARGING: {
			glm::mat4 const & view = GetSceneCamera ()->GetViewMatrix ();
			glm::mat4 & v= const_cast<glm::mat4 &> (view);
			glm::mat4 cam (stickMatrix);
			cam *= Transform3D::Translate (0, 0.1f, 0.9f);
			glm::mat4 invCam = glm::inverse (cam);
			v = invCam;
		} break;
		case PLAYING:
			glm::mat4 const & view = GetSceneCamera ()->GetViewMatrix ();
			glm::mat4 & v= const_cast<glm::mat4 &> (view);
			glm::mat4 lookDown  (1);
			lookDown *= Transform3D::RotateOX (-PI * 0.5f);
			lookDown *= Transform3D::Translate (0.0f, 0.0f, 2.0f);
			glm::mat4 invCam = glm::inverse (lookDown);
			v = invCam;
			break;
	}

	// integrate balls
	if (state == PLAYING){
		float done = 0.0f;
		do {
			done += integrate (deltaTimeSeconds - done);
		} while (done < deltaTimeSeconds);

		float maxVelocity = 0.0f;
		for (int i = 0; i < balls.size(); ++i){
			integrateBallForces (balls[i], deltaTimeSeconds);
			float vel = glm::length (balls[i]->getVelocity());
			if (vel > maxVelocity)
				maxVelocity = vel;
		}
		// ================
		// handle game turn
		// ================
		if (maxVelocity < minVelocity * 3.0f){

			stickCharge = 0.0f;
			stickAngle = 0.0f;
			chargeTime = 0.0f;
			if (pocketBalls.size() == 0) {
				currentPlayer = 1 - currentPlayer;
				state = CHARGING;
			}
			else 
			{
				bool scratch = false;
				bool black = false;
				int remained = 0;
				if (players[currentPlayer].color != Ball::WHITE && players[currentPlayer].color != Ball::BLACK)
					for (int i = 0; i < balls.size (); ++i)
						if (balls[i]->getPosition ().y > 0.0f && balls[i]->getColor () == players[currentPlayer].color)
							remained ++;
				for (int i = 0; i < pocketBalls.size(); ++i){
					switch (pocketBalls[i]) {
						case Ball::WHITE:
							scratch = true;
						break;
						case Ball::BLACK:
							black = true;
						break;
					}
				}
				if (black) {
					if (remained == 0)
						cout << "Player " << currentPlayer << " wins!!!!";
					else
						cout << "Player " << (1 - currentPlayer) << " wins!!!!";
					exit (0);
				}
				if (scratch) {
					currentPlayer = 1 - currentPlayer;
					whiteBall->setPosition(glm::vec3 (0.0f,ballRadius, height * 0.35f));
					state = WHITE_PLACEMENT;
				}
				else {
					if (players[currentPlayer].color == Ball::WHITE) {
						players[currentPlayer].color = pocketBalls[0];
						players[1 - currentPlayer].color = pocketBalls[0] == Ball::RED ? Ball::YELLOW : Ball::RED;
						state = CHARGING;
					}
					else {
						if (pocketBalls[0] != players[currentPlayer].color) {
							currentPlayer = 1 - currentPlayer;
						}
						state = CHARGING;
					}
				}
			}

			int remained = 0;
			if (players[currentPlayer].color != Ball::WHITE && players[currentPlayer].color != Ball::BLACK)
				for (int i = 0; i < balls.size (); ++i)
					if (balls[i]->getPosition ().y > 0.0f && balls[i]->getColor () == players[currentPlayer].color)
						remained ++;
			if (players[currentPlayer].color != Ball::WHITE && 0 == remained)
				players[currentPlayer].color = Ball::BLACK;
			cout << "============================================\n";
			cout << "Player " << currentPlayer << "\n";
			switch (players[currentPlayer].color) {
				case Ball::WHITE:
					cout << "Color : to be choosen\n";
				break;
				case Ball::BLACK:
					cout << "Color : black\n";
				break;
				case Ball::RED:
					cout << "Color : red\n";
				break;
				case Ball::YELLOW:
					cout << "Color : yellow\n";
				break;
			}
			cout << "Remained:" << remained << "\n";
		}
	}

	if (state == CHARGING)
		RenderStickMesh(meshes["stick"], shaders["StickVertexColor"], stickMatrix, stickCharge);
	// Table
	glm::mat4 modelMatrix (1);
	RenderMesh(meshes["table"], shaders["VertexColor"], modelMatrix);
	RenderMesh(meshes["wallup"], shaders["VertexColor"], modelMatrix);
	RenderMesh(meshes["walldown"], shaders["VertexColor"], modelMatrix);
	RenderMesh(meshes["wallleft"], shaders["VertexColor"], modelMatrix);
	RenderMesh(meshes["wallright"], shaders["VertexColor"], modelMatrix);
	for (int i = 0; i < pockets.size (); ++i)
		render (pockets[i]);
	for (int i = 0; i < balls.size (); ++i)
		render (balls[i]);
}

void Laborator4::FrameEnd()
{
	DrawCoordinatSystem();
}

void Laborator4::OnInputUpdate(float deltaTime, int mods)
{
	switch (state) {
		case WHITE_PLACEMENT:
		{
			glm::vec3 pos = whiteBall->getPosition ();
			glm::vec3 add (0.0f);

			if (window->KeyHold (GLFW_KEY_D))
				add += glm::vec3 (0.2f, 0.0f, 0.0f);

			if (window->KeyHold (GLFW_KEY_A))
				add += glm::vec3 (-0.2f, 0.0f, 0.0f);

			if (window->KeyHold (GLFW_KEY_S))
				add += glm::vec3 (0.0f, 0.0f, 0.2f);

			if (window->KeyHold (GLFW_KEY_W))
				add += glm::vec3 (0.0f, 0.0f, -0.2f);

			glm::vec3 newPos = pos + add * deltaTime;

			// clamp the white ball into it's area
			float ratioHeight = 0.333333f;

			if (newPos.x < -width * 0.5f + whiteBall->getRadius())
				newPos.x = -width * 0.5f + whiteBall->getRadius();

			if (newPos.x > width * 0.5f - whiteBall->getRadius())
				newPos.x = width * 0.5f - whiteBall->getRadius();

			if (newPos.z > height * 0.5f - whiteBall->getRadius())
				newPos.z = height * 0.5f - whiteBall->getRadius();
			
			if (newPos.z < height * (0.5f - ratioHeight) + whiteBall->getRadius())
				newPos.z = height * (0.5f - ratioHeight) + whiteBall->getRadius();

			whiteBall->setPosition (newPos);

			if (window->KeyHold (GLFW_KEY_SPACE)){
				state = CHARGING;
				stickAngle = 0.0f;
				stickCharge = 0.0f;
				chargeTime = 0.0f;
			}
		} break;
	}
}

void Laborator4::OnKeyPress(int key, int mods)
{
}

void Laborator4::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Laborator4::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	switch (state)
	{
	case Laborator4::CHARGING:
		if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
			stickAngle += deltaX * 0.1f;
		break;
	}
}

void Laborator4::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Laborator4::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator4::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Laborator4::OnWindowResize(int width, int height)
{
}

void Laborator4::render(Pocket * pocket) {
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate (pocket->getPosition().x, pocket->getPosition().y, pocket->getPosition().z);
	float diameter = pocket->getRadius() * 2.0f;
	modelMatrix *= Transform3D::Scale (diameter, diameter, diameter);
	RenderMesh(meshes["sphere"], shaders["VertexColor"], modelMatrix);
}

void Laborator4::render(Ball * ball) {
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate (ball->getPosition().x, ball->getPosition().y, ball->getPosition().z);
	float diameter = ball->getRadius() * 2.0f;
	modelMatrix *= Transform3D::Scale (diameter, diameter, diameter);

	glm::vec3 color = ball->getRenderColor ();

	RenderMeshWithColor(meshes["sphere"], modelMatrix, color);
}

void Laborator4::RenderMeshWithColor(Mesh * mesh,  glm::mat4 & modelMatrix, glm::vec3 color)
{
	Shader *shader = shaders["Color"];
	if (!mesh || !shader || !shader->program)
		return;

	// render an object using the specified shader and the specified position
	shader->Use();
	glUniform3f(shader->GetUniformLocation("color"), color.r, color.g, color.b);
	glm::mat4 t = GetSceneCamera()->GetViewMatrix();
	glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(GetSceneCamera()->GetViewMatrix()));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(GetSceneCamera()->GetProjectionMatrix()));
	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	mesh->Render();
}

void Laborator4::RenderStickMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix, float charge)
{
	if (!mesh || !shader || !shader->program)
		return;

	// render an object using the specified shader and the specified position
	shader->Use();
	glUniform1f(shader->GetUniformLocation("Charge"), charge);
	glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(GetSceneCamera()->GetViewMatrix()));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(GetSceneCamera()->GetProjectionMatrix()));
	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	mesh->Render();
}

float Laborator4::integrate(float deltaTime) {
	// Detect the first collision
	float firstCollisionTime = deltaTime;
	Collision *firstCollision = NULL;
	Ball *firstCollisionBall = NULL;
	for (int i = 0; i < balls.size(); ++i) {
		Ball * ball = balls[i];
		if (glm::length (ball->getVelocity()) == 0.0f)
			continue;
		for (int j = 0; j < collisions.size(); ++j) {
			Collision & collision = collisions[j];
			float timeToCollision = getTimeToCollision (ball, collision, deltaTime);
			if (timeToCollision < firstCollisionTime) {
				firstCollision = &collision;
				firstCollisionTime = timeToCollision;
				firstCollisionBall = ball;
			}
		}
	}

	// integrate to the first collision
	if (firstCollisionTime < deltaTime) {
		solveCollision (firstCollisionBall, *firstCollision, firstCollisionTime);
		for (int i = 0; i < balls.size(); ++i) {
			Ball * ball = balls[i];
			if (ball == firstCollisionBall || isBall (*firstCollision, ball))
				continue;
			integrateBallVelocity (ball, firstCollisionTime);
		}
		return firstCollisionTime;
	} 
	
	// no collision integrate all
	for (int i = 0; i < balls.size(); ++i)
		integrateBallVelocity (balls[i], deltaTime);
	return deltaTime;
}

float Laborator4::getTimeToCollision(Ball * ball, Collision & collision, float deltaTime) {
	switch (collision.type) {
		case Collision::PLANE: {
			// compute the velocity along the plane normal
			float velAlongNormal = glm::dot (ball->getVelocity(), collision.normal);
			if (velAlongNormal < 0.0f)
				return deltaTime;

			// compute the distance to plane
			glm::vec3 to = ball->getPosition() - collision.pos;
			float dist = glm::dot (to, -collision.normal) - ball->getRadius();

			// compute the distance traveled along the normal velocity
			float distAlongNormal = velAlongNormal * deltaTime;
			if (distAlongNormal <= dist)
				return deltaTime;
			
			// compute the time to collision
			return dist / velAlongNormal;
		}
		case Collision::BALL: {
			glm::vec3 velocity = ball->getVelocity() - collision.ball->getVelocity ();
			glm::vec3 to = collision.ball->getPosition () - ball->getPosition();
			if (glm::dot (velocity, to) < 0)
				return deltaTime;
			glm::vec3 normalizedVelocity = glm::normalize (velocity);

			// compute the dist to contact
			float dist = glm::dot (normalizedVelocity, to);
			float distToVelocityLineSq = glm::dot(to, to) - dist * dist;
			if (distToVelocityLineSq > (ball->getRadius() + collision.ball->getRadius()) * (ball->getRadius() + collision.ball->getRadius()))
				return deltaTime;
			float bias = sqrtf ((ball->getRadius() + collision.ball->getRadius()) * (ball->getRadius() + collision.ball->getRadius()) - distToVelocityLineSq);
			dist = dist - bias;

			// compute the time till contact
			float timeToContact = dist / glm::length (velocity);
			if (timeToContact >= deltaTime)
				return deltaTime;

			return timeToContact;
		}
		case Collision::POCKET: {
			glm::vec3 velocity = ball->getVelocity() ;
			glm::vec3 to = collision.pocket->getPosition () - ball->getPosition();
			if (glm::dot (velocity, to) < 0)
				return deltaTime;
			glm::vec3 normalizedVelocity = glm::normalize (velocity);

			// compute the dist to contact
			float dist = glm::dot (normalizedVelocity, to);
			float distToVelocityLineSq = glm::dot(to, to) - dist * dist;
			if (distToVelocityLineSq > collision.pocket->getRadius() * collision.pocket->getRadius())
				return deltaTime;
			float bias = sqrtf (collision.pocket->getRadius() * collision.pocket->getRadius() - distToVelocityLineSq);
			dist = dist - bias;

			// compute the time till contact
			float timeToContact = dist / glm::length (velocity);
			if (timeToContact < 0.0f)
				return 0.0f;
			if (timeToContact >= deltaTime)
				return deltaTime;

			return timeToContact;
		}
	}
	return deltaTime;
}

void Laborator4::solveCollision(Ball * ball, Collision &collision, float deltaTime) {
	switch (collision.type) {
		case Collision::PLANE: {
			integrateBallVelocity (ball, deltaTime);
			// reflect velocity
			ball->setVelocity (glm::reflect(ball->getVelocity(), collision.normal));
		} break;
		case Collision::BALL: {
			integrateBallVelocity (ball, deltaTime);
			integrateBallVelocity (collision.ball, deltaTime);

			// super gay sphere sphere collision
			glm::vec3 n = glm::normalize (ball->getPosition () - collision.ball->getPosition ());
			float v1 = glm::dot (ball->getVelocity(), n);
			float v2 = glm::dot (collision.ball->getVelocity(), n);
			float optimizedP = v1 - v2;
			ball->setVelocity(ball->getVelocity() - optimizedP * n);
			collision.ball->setVelocity(collision.ball->getVelocity() + optimizedP * n);
		}break;
		case Collision::POCKET: {
			ball->setPosition (collision.pocket->getPosition () + glm::vec3 (0, -ball->getRadius() - 0.1, 0));
			ball->setVelocity (glm::vec3 (0));
			pocketBalls.push_back (ball->getColor ());
		}break;
	}
}

void Laborator4::integrateBallVelocity(Ball * ball, float deltaTime) {
	ball->setPosition(ball->getPosition() + ball->getVelocity() * deltaTime);
}

void Laborator4::integrateBallForces(Ball * ball, float deltaTime) {
	float velocity = glm::length(ball->getVelocity ());
	if (velocity == 0.0f)
		return;
	glm::vec3 dir = glm::normalize(ball->getVelocity ());
	glm::vec3 drag = - dir * velocity * 0.1f;
	ball->setVelocity(ball->getVelocity() + deltaTime * drag);
}

bool Laborator4::isBall(Collision &collision, Ball *ball) {
	return collision.type == Collision::BALL && collision.ball == ball;
}

