#include "Game.h"
#include "Box2DHelper.h"

// Constructor de la clase Game
Game::Game(int ancho, int alto, std::string titulo)
{
	// Inicializaci�n de la ventana de renderizado
	wnd = new RenderWindow(VideoMode(ancho, alto), titulo);
	wnd->setVisible(true);
	fps = 60;
	wnd->setFramerateLimit(fps);
	frameTime = 1.0f / fps;
	SetZoom(); // Configuraci�n del zoom de la c�mara
	InitPhysics(); // Inicializaci�n del mundo f�sico
}

// M�todo principal que maneja el bucle del juego
void Game::Loop()
{
	while (wnd->isOpen())
	{
		wnd->clear(clearColor); // Limpia la ventana con un color especificado
		DoEvents(); // Procesa los eventos del sistema
		UpdatePhysics(); // Actualiza la simulaci�n f�sica
		DrawGame(); // Dibuja el juego en la ventana
		wnd->display(); // Muestra la ventana renderizada
	}
}

// Actualiza la simulaci�n f�sica
void Game::UpdatePhysics()
{
	phyWorld->Step(frameTime, 8, 8); // Avanza la simulaci�n f�sica
	phyWorld->ClearForces(); // Limpia las fuerzas aplicadas a los cuerpos
	phyWorld->DebugDraw(); // Dibuja el mundo f�sico (para depuraci�n)
}

// Dibuja los elementos del juego en la ventana
void Game::DrawGame()
{
	/*
	// Dibujamos el suelo
	sf::RectangleShape groundShape(sf::Vector2f(500, 5));
	groundShape.setFillColor(sf::Color::Red);
	groundShape.setPosition(0, 95);
	wnd->draw(groundShape);

	// Dibujamos las paredes
	sf::RectangleShape leftWallShape(sf::Vector2f(10, alto)); // Alto de la ventana
	leftWallShape.setFillColor(sf::Color::Red);
	leftWallShape.setPosition(100, 0); // X = 100 para que comience donde termina el suelo
	wnd->draw(leftWallShape);

	sf::RectangleShape rightWallShape(sf::Vector2f(10, alto)); // Alto de la ventana
	rightWallShape.setFillColor(sf::Color::Cyan);
	rightWallShape.setPosition(90, 0); // X = 90 para que comience donde termina el suelo
	wnd->draw(rightWallShape);

	controlBodyAvatar->Actualizar(); // Actualiza la posici�n del avatar
	controlBodyAvatar->Dibujar(*wnd); // Dibuja el avatar en la ventana
	*/
}

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
	if (mouse->isButtonPressed(mouse->Left))
	{
		// Convert� la posici�n del mouse a coordenadas del mundo
		Vector2i pixelPos = mouse->getPosition(*wnd);
		Vector2f worldPos = wnd->mapPixelToCoords(pixelPos);
		b2Vec2 b2MousePos(worldPos.x, worldPos.y);
		if (!wasMousePressed)
		{
			wasMousePressed = true;
			if (controlBody->GetFixtureList()->TestPoint(b2MousePos))
			{
				draggedBody = controlBody;
			}
			else if (controlBody->GetFixtureList()->TestPoint(b2MousePos))
			{
				draggedBody = controlBody;
			}
		}
		else if (draggedBody != nullptr)
		{
			draggedBody->SetAwake(true);
			draggedBody->SetTransform(b2MousePos, draggedBody->GetAngle());
		}
	}
	else
	{
		wasMousePressed = false;
		draggedBody = nullptr;
	}


	// Mueve el cuerpo controlado por el teclado
	controlBody->SetAwake(true); // Activa el cuerpo para que responda a fuerzas y colisiones
	if (Keyboard::isKeyPressed(Keyboard::Left))
		controlBody->SetLinearVelocity(b2Vec2(-50.0f, 0.0f));
	if (Keyboard::isKeyPressed(Keyboard::Right))
		controlBody->SetLinearVelocity(b2Vec2(50.0f, 0.0f));
	if (Keyboard::isKeyPressed(Keyboard::Down))
		controlBody->SetLinearVelocity(b2Vec2(0.0f, 50.0f));
	if (Keyboard::isKeyPressed(Keyboard::Up))
		controlBody->SetLinearVelocity(b2Vec2(0.0f, -50.0f));
}

// Configura el �rea visible en la ventana de renderizado
void Game::SetZoom()
{
	View camara;
	camara.setSize(100.0f, 100.0f); // Tama�o del �rea visible
	camara.setCenter(50.0f, 50.0f); // Centra la vista en estas coordenadas
	wnd->setView(camara); // Asigna la vista a la ventana
}

// Inicializa el mundo f�sico y los elementos est�ticos del juego
void Game::InitPhysics()
{
	// Inicializa el mundo f�sico con la gravedad por defecto
	phyWorld = new b2World(b2Vec2(0.0f, 9.8f));

	// Inicializa el renderizador de depuraci�n para el mundo f�sico
	debugRender = new SFMLRenderer(wnd);
	debugRender->SetFlags(UINT_MAX); // Configura el renderizado para que muestre todo
	phyWorld->SetDebugDraw(debugRender);

	// Crea los elementos est�ticos del juego (suelo y paredes)
	b2Body* groundBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10);
	groundBody->SetTransform(b2Vec2(50.0f, 100.0f), 0.0f);

	b2Body* leftWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
	leftWallBody->SetTransform(b2Vec2(0.0f, 50.0f), 0.0f);

	b2Body* rightWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
	rightWallBody->SetTransform(b2Vec2(100.0f, 50.0f), 0.0f);

	b2Body* topWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10);
	topWallBody->SetTransform(b2Vec2(50.0f, 0.0f), 0.0f);

	// Crea un cuerpo de c�rculo controlado por el teclado
	controlBody = Box2DHelper::CreateCircularDynamicBody(phyWorld, 5, 1.0f, 0.5, 0.1f);
	controlBody->SetTransform(b2Vec2(50.0f, 50.0f), 0.0f);

	b2Body* circuloIzq = Box2DHelper::CreateCircularDynamicBody(phyWorld, 5, 1.0f, 0.5, 0.1f);
	circuloIzq->SetTransform(b2Vec2(80.0f, 10.0f), 0.0f);

	Box2DHelper::CreateDistanceJoint(phyWorld, circuloIzq, circuloIzq->GetWorldCenter(),
		controlBody, controlBody->GetWorldCenter() , 10.0f, 0.5f, 0.5f);
}

