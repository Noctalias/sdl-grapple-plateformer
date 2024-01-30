// apps/LIFAMI/TPMiniProjet.cpp
#include<Grapic.h>

using namespace grapic;


//----------------------CONSTANTES--------------------------
const unsigned int DIMW_HEIGHT = 480;
const unsigned int DIMW_WIDTH = 736;
const float DELTA_TIME = 0.001; // Variations entre 2 temps
const float GRAVITY = -9.81;
const float JUMP_SPEED = 1;

const int SIZE_BLOCK = 32;
const int MAP_SIZE_X= 16;
const int MAP_SIZE_Y = 16;
const float GROUND_DISTANCE = 0.1;

const unsigned int CHARACTER_SIZE = 16;
const float PLAYER_MOVEMENT_SPEED = 0.3;
const char CONTROLS[] = "sfe";
//----------------------CONSTANTES------------------------
struct Vec2
{
	double x, y;
};

Vec2 makeVec2(double x, double y)
{
	Vec2 v2;
	v2.x = x;
	v2.y = y;

	return v2;
}

Vec2 operator* (Vec2 a, float lambda)
{
	Vec2 v;
	v.x = a.x * lambda;
	v.y = a.y * lambda;

	return v;
}
Vec2 operator+ (Vec2 a, Vec2 b)
{
	Vec2 v;
	v.x = a.x + b.x;
	v.y = a.y + b.y;

	return v;
}

//Definition du terrain
const unsigned char MAP[MAP_SIZE_X][MAP_SIZE_Y] = {
		{ 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 0, 2},
		{ 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{ 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{ 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
		{ 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 0, 2},
		{ 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{ 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{ 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2}
};

struct Player
{
	Vec2 pos, force, velocity;
	double mass;
	int nbLife;
	bool isUsingPhysics; // Vérifie si le joueur est affecté par la physique (comme la gravité) ou non
};

struct Camera
{
	Vec2 pos;
};

struct World
{
	Image blocks[2];
	Camera camera;
	Player player;
};



initWorld(struct World &world)
{
	world.blocks[0] = image("data/miniProjet/block.png");
	world.blocks[1] = image("data/miniProjet/stop.png");
	world.camera.pos = makeVec2(0,0);

	world.player.pos = makeVec2(0.5 ,50); //+ SIZE_BLOCK * 1.5);
	world.player.force = makeVec2(0,0);
	world.player.velocity = makeVec2(0,0);
	world.player.mass = 50;
	world.player.isUsingPhysics = false;
	std::cout << "isUsingPhysics: " << world.player.isUsingPhysics << std::endl;

}

void draw(World &world)
{
	int x, y;
	//Creation des blocks
	for (y = 0; y < MAP_SIZE_Y; y++)
	{
		for (x = 0; x < MAP_SIZE_X ; x++)
		{
			if (MAP[y][x] != 0)
			{
				image_draw(world.blocks[MAP[y][x] - 1], (x - world.camera.pos.x) * SIZE_BLOCK,
				           (y - world.camera.pos.y) * SIZE_BLOCK, SIZE_BLOCK, SIZE_BLOCK);
			}

		}
	}
	circleFill((world.player.pos.x - world.camera.pos.x), (world.player.pos.y - world.camera.pos.y), SIZE_BLOCK / 2);
}

void computeVelocity(World &world)
{
	world.player.velocity = world.player.velocity +  world.player.force * DELTA_TIME;
}

void applyGravity(World &world)
{
	 world.player.force.x = 0;
	 world.player.force.y = GRAVITY / world.player.mass; //* DELTA_TIME;
}

void gestionCollision(World &world, Vec2 oldPos)
{
	if (MAP[int(world.player.pos.y)][int(world.player.pos.x)] != 0) // Si le joueur rencontre un bloc...
	{
		std::cout << "COLLISION, SETTING THE PLAYER'S POSITION TO IT'S OLD POSITION" << std::endl;
		std::cout << std::endl;
		world.player.pos.y = oldPos.y + GROUND_DISTANCE; // ...le joueur va se retrouver dans son ancienne position
	}
	else
	{
		std::cout << "PAS DE COLLISION, APPLYING GRAVITY " << std::endl;
		std::cout << std::endl;
		applyGravity(world);
	}
}

void controls(World &world)
{
	if(isKeyPressed(CONTROLS[0])) // if 'S' (LEFT) pressed
	{
		world.player.pos.x -= PLAYER_MOVEMENT_SPEED;
		//std::cout << "Going left..." << std::endl;
	}

	if(isKeyPressed(CONTROLS[1])) // if 'F' (RIGHT) pressed
	{
		world.player.pos.x += PLAYER_MOVEMENT_SPEED;
		//std::cout << "Going right..."<< std::endl;
	}

	if(isKeyPressed(CONTROLS[2])) // if 'E' (JUMP) pressed
	{
		if (world.player.isUsingPhysics == false)
		{
			world.player.velocity.y += JUMP_SPEED;
			world.player.isUsingPhysics = true;
			std::cout << "Jumping... isUsingPhysic: " << world.player.isUsingPhysics << std::endl;
		}

		//applyGravity(world);

		//wworld.player.pos.y += PLAYER_MOVEMENT_SPEED;
	}
}

void updatePlayer(World &world, Vec2 oldPos)
{
	std::cout << "BEFORE UPDATE PLAYER CALL " << std::endl;
	std::cout << "Coordinates of player: X: " << world.player.pos.x << " Y: " << world.player.pos.y << std::endl;
	std::cout << "AFTER UPDATE PLAYER CALL " << std::endl;
	std::cout << std::endl;


	world.player.pos = world.player.pos + world.player.velocity; //* DELTA_TIME;

	gestionCollision(world, oldPos);

	if (world.player.pos.y > 0 && world.player.pos.y < MAP_SIZE_Y
	    && world.player.pos.x > 0 && world.player.pos.x < MAP_SIZE_X
	    && MAP[int(world.player.pos.y - GROUND_DISTANCE)][int(world.player.pos.x)] != 0) // Si un bloc est detecté en dessous du joueur (par rapport a GROUND_DISTANCE) ...
	{
		std::cout << "ON ZE GROUND " << /*+MAP[int(world.player.pos.y - GROUND_DISTANCE)][int(world.player.pos.x)] <<*/ std::endl;
		world.player.isUsingPhysics = false; //...le joueur n'est plus affécté par les physiques (le joueur pourra donc re-sauter)
		std::cout << "PLAYER IS NOT AFFECTED BY PHYSICS" << std::endl;
	}

}

void update(World &world)
{
	Vec2 oldPos = world.player.pos;
	gestionCollision(world, oldPos);
	/*if(world.player.pos.x == oldPos.x && world.player.pos.y == oldPos.y) //Si la position n'a pas changé => Pas de collision(cf. gestionCollision) => Application gravité
	{
		applyGravity(world);
	}*/
	controls(world);
	computeVelocity(world);
	updatePlayer(world, oldPos);
}


int main(int , char**)
{
	winInit("GrAPple-iC",DIMW_WIDTH,DIMW_HEIGHT);
	backgroundColor(121, 134, 203); //bleu ciel
	setKeyRepeatMode(true);
	qToQuit(false);

	bool stop = false;

	World world;
	initWorld(world);

	while(!stop)
	{
		winClear();

		update(world);
		draw(world);


		stop = winDisplay();
		delay(DELTA_TIME);
	}

	winQuit();
	return 0;
}
