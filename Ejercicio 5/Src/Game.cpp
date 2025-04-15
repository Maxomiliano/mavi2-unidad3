#include "Game.h"
#include "Box2DHelper.h"

// Constructor de la clase Game
Game::Game(int ancho, int alto, std::string titulo)
{
	// Inicialización de la ventana de renderizado
	wnd = new RenderWindow(VideoMode(ancho, alto), titulo);
	wnd->setVisible(true);
	fps = 60;
	wnd->setFramerateLimit(fps);
	frameTime = 1.0f / fps;
	SetZoom(); // Configuración del zoom de la cámara
	InitPhysics(); // Inicialización del mundo físico
}

// Método principal que maneja el bucle del juego
void Game::Loop()
{
	while (wnd->isOpen())
	{
		wnd->clear(clearColor); // Limpia la ventana con un color especificado
		DoEvents(); // Procesa los eventos del sistema
		UpdatePhysics(); // Actualiza la simulación física
		DrawGame(); // Dibuja el juego en la ventana
		wnd->display(); // Muestra la ventana renderizada
	}
}

// Actualiza la simulación física
void Game::UpdatePhysics()
{
	phyWorld->Step(frameTime, 8, 8); // Avanza la simulación física
	phyWorld->ClearForces(); // Limpia las fuerzas aplicadas a los cuerpos
	phyWorld->DebugDraw(); // Dibuja el mundo físico (para depuración)
}

// Dibuja los elementos del juego en la ventana
void Game::DrawGame(){}

// Procesa los eventos del sistema
void Game::DoEvents()
{
	Event evt;
	while (wnd->pollEvent(evt))
	{
		switch (evt.type)
		{
		case Event::Closed:
			wnd->close(); // Cierra la ventana
			break;
		}
	}
}

// Configura el área visible en la ventana de renderizado
void Game::SetZoom()
{
	View camara;
	camara.setSize(100.0f, 100.0f); // Tamaño del área visible
	camara.setCenter(50.0f, 50.0f); // Centra la vista en estas coordenadas
	wnd->setView(camara); // Asigna la vista a la ventana
}

// Inicializa el mundo físico y los elementos estáticos del juego
void Game::InitPhysics()
{
	float density = 1.0f;
	float friction = 0.5f;
	float restitution = 0.5f;

	// Inicializa el mundo físico con la gravedad por defecto
	phyWorld = new b2World(b2Vec2(0.0f, 9.8f));

	// Inicializa el renderizador de depuración para el mundo físico
	debugRender = new SFMLRenderer(wnd);
	debugRender->SetFlags(UINT_MAX); // Configura el renderizado para que muestre todo
	phyWorld->SetDebugDraw(debugRender);

	// Crea los elementos estáticos del juego (suelo y paredes)
	b2Body* groundBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10);
	groundBody->SetTransform(b2Vec2(50.0f, 100.0f), 0.0f);

	b2Body* leftWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
	leftWallBody->SetTransform(b2Vec2(0.0f, 50.0f), 0.0f);

	b2Body* rightWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
	rightWallBody->SetTransform(b2Vec2(100.0f, 50.0f), 0.0f);

	b2Body* topWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10);
	topWallBody->SetTransform(b2Vec2(50.0f, 0.0f), 0.0f);


	b2Body* headBody = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 5, 5, density, friction, restitution);
	headBody->SetTransform(b2Vec2(50.0f, 79.0f), 0.0f);

	b2Body* centerBody = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 10, 20, density, friction, restitution);
	centerBody->SetTransform(b2Vec2(50.0f, 85.0f), 0.0f);

	b2Body* leftArmBody = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 2, 15, density, friction, restitution);
	leftArmBody->SetTransform(b2Vec2(45.0f, 85.0f), 0.0f);

	b2Body* rightArmBody = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 2, 15, density, friction, restitution);
	rightArmBody->SetTransform(b2Vec2(55.0f, 85.0f), 0.0f);

	b2Body* leftLegBody = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 2, 15, density, friction, restitution);
	leftLegBody->SetTransform(b2Vec2(48.0f, 100.0f), 0.0f);

	b2Body* rightLegBody = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 2, 15, density, friction, restitution);
	rightLegBody->SetTransform(b2Vec2(52.0f, 100.0f), 0.0f);

	//Cabeza y torso
	b2Vec2 headAnchor(50.0f, 81.5f);
	b2Vec2 centerAnchor(50.0f, 79.0f);

	b2Vec2 localAnchorHead = headBody->GetLocalPoint(headAnchor);
	b2Vec2 localAnchorTorso = centerBody->GetLocalPoint(centerAnchor);

	b2DistanceJoint* headJoint = Box2DHelper::CreateDistanceJoint(phyWorld, headBody, localAnchorHead, centerBody, localAnchorTorso, 0, 0.5f, 0.2f);

	
	//Brazo izquierdo y torso
	b2Vec2 centerAnchorLA(47.0f, 80.0f);
	b2Vec2 leftArmAnchor(45.0f, 77.5f);

	b2Vec2 localAnchorLeftArm = leftArmBody->GetLocalPoint(leftArmAnchor);
	b2Vec2 localAnchorTorsoLeftArm = centerBody->GetLocalPoint(centerAnchorLA);

	b2DistanceJoint* leftArmJoint = Box2DHelper::CreateDistanceJoint(phyWorld, leftArmBody, localAnchorLeftArm, centerBody, localAnchorTorsoLeftArm, 0, 0.5f, 0.2f);	

	//Brazo derecho y torso
	b2Vec2 centerAnchorRA(53.0f, 80.0f);
	b2Vec2 rightArmAnchor(55.0f, 77.5f);

	b2Vec2 localAnchorRightArm = rightArmBody->GetLocalPoint(rightArmAnchor);
	b2Vec2 localAnchorTorsoRightArm = centerBody->GetLocalPoint(centerAnchorRA);

	b2DistanceJoint* rightArmJoint = Box2DHelper::CreateDistanceJoint(phyWorld, rightArmBody, localAnchorRightArm, centerBody, localAnchorTorsoRightArm, 0, 0.8f, 0.1f);
}

