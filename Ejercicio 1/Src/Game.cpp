#include "Game.h"
#include "Box2DHelper.h"

// Constructor de la clase Game
Game::Game(int ancho, int alto, std::string titulo) : ballPosition(50, 50), ballVelocity(1, 0.5f) // Inicializa la posición y velocidad de la bola
{
	wnd = new RenderWindow(VideoMode(ancho, alto), titulo); // Crea una ventana con las dimensiones y título especificados
	wnd->setVisible(true); // Hace visible la ventana
	fps = 60; // Establece el límite de cuadros por segundo (FPS)
	wnd->setFramerateLimit(fps); // Aplica el límite de FPS a la ventana
	frameTime = 1.0f / fps; // Calcula el tiempo por cuadro en segundos
	SetZoom(); // Configura el "zoom" o vista de la cámara
	InitPhysics(); // Inicializa la simulación de física
}

void Game::Loop()
{
	while (wnd->isOpen()) // Bucle principal del juego que se ejecuta mientras la ventana esté abierta
	{
		wnd->clear(clearColor); // Limpia la ventana con el color de fondo
		DoEvents(); // Maneja los eventos (input del usuario)
		CheckCollitions(); // Verifica colisiones (a implementar)
		UpdatePhysics(); // Actualiza la simulación de física
		DrawGame(); // Dibuja los elementos del juego
		wnd->display(); // Muestra los cambios en la ventana
	}
}

void Game::UpdatePhysics()
{
	//phyWorld->Step(frameTime, 8, 8); // Avanza la simulación de física un paso
	//phyWorld->ClearForces(); // Limpia las fuerzas acumuladas
	//phyWorld->DebugDraw(); // Dibuja la representación de debug de la simulación
	ballPosition += ballVelocity * frameTime;
	if (ballPosition.x - ballRadius < 0.0f)
	{
		float x = ballRadius - ballPosition.x;
		float force = -K * x;
		float acceleration = force / ballMass;
		ballVelocity.x += acceleration * frameTime;
	}

	if(ballPosition.x + ballRadius > 800)
	{
		float x = ballPosition.x + ballRadius - 800;
		float force = -K * x;
		float acceleration = force / ballMass;
		ballVelocity.x += acceleration * frameTime;
	}

	if (ballPosition.y + ballRadius < 0)
	{
		float x = ballRadius - ballPosition.y;
		float force = -K * x;
		float acceleration = force / ballMass;
		ballVelocity.y += acceleration * frameTime;
	}

	if (ballPosition.y + ballRadius > 600)
	{
		float x = ballPosition.y + ballRadius - 600;
		float force = -K * x;
		float acceleration = force / ballMass;
		ballVelocity.y += acceleration * frameTime;
	}
}


void Game::DrawGame()
{
	// Función para dibujar los elementos del juego (a implementar)
	// Dibujar el cuerpo de control (círculo)
	sf::CircleShape controlShape(ballRadius);
	controlShape.setFillColor(sf::Color::Magenta);
	controlShape.setPosition(ballPosition);
	wnd->draw(controlShape);

	// Dibujar el suelo
	sf::RectangleShape groundShape(sf::Vector2f(100, 5));
	groundShape.setFillColor(sf::Color::Red);
	groundShape.setPosition(0, 95);
	wnd->draw(groundShape);

	// Dibujar el techo
	sf::RectangleShape ceilShape(sf::Vector2f(100, 5));
	ceilShape.setFillColor(sf::Color::Red);
	ceilShape.setPosition(0, 0);
	wnd->draw(ceilShape);

	// Dibujamos las paredes
	sf::RectangleShape leftWallShape(sf::Vector2f(5, 100)); // Alto de la ventana
	leftWallShape.setFillColor(sf::Color::Red);
	leftWallShape.setPosition(0, 0); // X = 100 para que comience donde termina el suelo
	wnd->draw(leftWallShape);

	sf::RectangleShape rightWallShape(sf::Vector2f(5, 100)); // Alto de la ventana
	rightWallShape.setFillColor(sf::Color::Red);
	rightWallShape.setPosition(95, 0); // X = 90 para que comience donde termina el suelo
	wnd->draw(rightWallShape);;
}

void Game::DoEvents()
{
	Event evt;
	while (wnd->pollEvent(evt)) // Procesa todos los eventos acumulados
	{
		switch (evt.type)
		{
		case Event::Closed: // Si se solicita cerrar la ventana
			wnd->close(); // Cierra la ventana
			break;
		}
	}
}

void Game::CheckCollitions()
{
	// Verificación de colisiones (a implementar)
}

void Game::SetZoom()
{
	View camara;
	camara.setSize(100.0f, 100.0f); // Establece el tamaño de la vista
	camara.setCenter(50.0f, 50.0f); // Centra la vista en un punto del mundo
	wnd->setView(camara); // Aplica la vista a la ventana
}

void Game::InitPhysics()
{
	/*
	
	// Inicializa el mundo de Box2D con una gravedad hacia abajo
	phyWorld = new b2World(b2Vec2(0.0f, 9.8f));

	debugRender = new SFMLRenderer(wnd); // Crea un renderizador de debug para SFML
	debugRender->SetFlags(UINT_MAX); // Configura el renderizador para dibujar todas las formas de debug
	phyWorld->SetDebugDraw(debugRender); // Establece el renderizador de debug para el mundo de Box2D

	// Crea cuerpos estáticos para simular el suelo y las paredes
	// También crea cuerpos dinámicos (como círculos) y los une con resortes al techo para demostrar la simulación física
	b2Body* groundBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10);
	groundBody->SetTransform(b2Vec2(50.0f, 100.0f), 0.0f);

	b2Body* leftWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
	leftWallBody->SetTransform(b2Vec2(0.0f, 50.0f), 0.0f);

	b2Body* rightWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
	rightWallBody->SetTransform(b2Vec2(100.0f, 50.0f), 0.0f);

	// Creamos un techo
	b2Body* topWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10);
	topWallBody->SetTransform(b2Vec2(50.0f, 0.0f), 0.0f);

	// Creamos un circulo que ataremos al primer resorte
	b2Body* circuloCentral = Box2DHelper::CreateCircularDynamicBody(phyWorld, 5, 1.0f, 0.5, 0.1f);
	circuloCentral->SetTransform(b2Vec2(50.0f, 10.0f), 0.0f);

	// Atamos la esfera mediante un resorte al techo
	Box2DHelper::CreateDistanceJoint(phyWorld, circuloCentral, circuloCentral->GetWorldCenter(),
		topWallBody, topWallBody->GetWorldCenter(), 20.0f, 0.1f, 0.0f);

	// Creamos un circulo que ataremos al resorte derecho
	b2Body* circuloIzq = Box2DHelper::CreateCircularDynamicBody(phyWorld, 5, 1.0f, 0.5, 0.1f);
	circuloIzq->SetTransform(b2Vec2(80.0f, 10.0f), 0.0f);

	// Atamos la esfera mediante un resorte al techo. Este resorte se encuentra dampeado totalmente
	Box2DHelper::CreateDistanceJoint(phyWorld, circuloIzq, circuloIzq->GetWorldCenter(),
		topWallBody, topWallBody->GetWorldCenter() + b2Vec2(35.0f, 0.0f), 20.0f, 0.1f, 1.0f);

	// Creamos un circulo que ataremos al resorte izquierdo
	b2Body* circuloDer = Box2DHelper::CreateCircularDynamicBody(phyWorld, 5, 1.0f, 0.5, 0.1f);
	circuloDer->SetTransform(b2Vec2(20.0f, 10.0f), 0.0f);

	// Atamos la esfera mediante un resorte al techo
	Box2DHelper::CreateDistanceJoint(phyWorld, circuloDer, circuloDer->GetWorldCenter(),
		topWallBody, topWallBody->GetWorldCenter() + b2Vec2(-35.0f, 0.0f), 20.0f, 0.1f, 1.0f);
		*/
}

Game::~Game(void)
{
	// Destructor de la clase Game (a implementar si es necesario)
}