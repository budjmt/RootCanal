#pragma once

#include "ColliderObject.h"
#include "CollisionManager.h"
#include "AudioManager.h"
#include <tchar.h>

#define MAX_X 75
#define MAX_Y 75

class Ship :
	public ColliderObject
{
public:
	Ship(Mesh* mesh, Material* material, AudioManager* am);
	//Ship(vec3 p, vec3 dims, vec3 sc, vec3 rA, float r, Drawable* s);

    float getHealth();
    float getXray();

	AudioManager* getAudioManager();

    void usingXray( bool value );
    bool usingXray();

	void addHealth(float hp);
	void update(float dt);
	void processMovement(float dt);
	void handleCollision(ColliderObject* other, Manifold& m, double dt, size_t& numCollisions);
private:
	float health;
    float xray;
    bool _usingXray;
	Camera* cam;
	AudioManager* audioManager;
};