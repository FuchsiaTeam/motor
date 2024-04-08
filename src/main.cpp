#define CURL_STATICLIB
#include "motor/motor.hpp"
#include "motor/components/empty.hpp"
#include "motor/object/gameobject.hpp"
#include "motor/components/sprite.hpp"
#include "motor/components/collider.hpp"
#include "motor/components/text.hpp"
#include "motor/components/phy.hpp"
#include "motor/event.hpp"
#include "testmoving.h"
#include "motor/internal.hpp"
#include "motor/discord.hpp"
#include "motor/paths.hpp"
#include "motor/gameobjects/debugtext.hpp"
#include "motor/gameobjects/keylistener.hpp"
#include "motor/registry/registries.hpp"

#pragma comment(lib, "libcurl_a.lib")
#pragma comment(lib, "Normaliz.lib")
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Wldap32.lib")
#pragma comment(lib, "Crypt32.lib")
#pragma comment(lib, "advapi32.lib")

#undef main
MOTOR_DLL_API Motor::Getriebe getriebe;
#ifdef EXE

void t() {
	std::cout << __FILE__ << "_" << __LINE__ << std::endl;
}


Fuchs::Discord::Discord discordApi{};
Motor::GameObject* objectz7;
std::vector<Motor::GameObject*> objBuffer{};
void discordThreadFunction() {
	bool discordRunning = true;
	while (discordRunning) {
		int tries = 0;

		while (discordApi.getCurrentUser().GetId() == 0) {
			std::this_thread::sleep_for(std::chrono::milliseconds(5000));
			if (discordApi.getCurrentUser().GetId() != 0) {
				json rawData = discordApi.getUserRawData(discordApi.getCurrentUser().GetId(), "");
				string avatar = discordApi.getAvatarId(rawData);
				auto avatarStream = ifstream{ "avatar.png" };
				bool avatarExists = avatarStream.good();
				avatarStream.close();
				if (!avatarExists) {
					if (discordApi.saveAvatar(rawData, "avatar.png", 512)) MOTOR_LOG("Got Avatar!");
				}
				Motor::Texture* texture = new Motor::Texture{ "avatar.png" };
				texture->load();
				objectz7->getComponent<Motor::SpriteComponent>()->setTexture(texture);
				for (Motor::GameObject* obj : objBuffer) obj->getComponent<Motor::SpriteComponent>()->setTexture(texture);
				MOTOR_LOG("Got Data! " + to_string(discordApi.getCurrentUser().GetId()));
				discordRunning = false;
				break;
			}
			++tries;
			MOTOR_LOG("Trying to get Discord Data... [" + to_string(tries) + "/45]");
			if (tries == 45) {
				MOTOR_LOG("Could not get Discord Data!");
				discordRunning = false;
				break;
			}
		}
	}
}

bool objGenDone = false;
void addPlaceHolderObjsThread() {
	auto* texture = new Motor::Texture("test.png");
	texture->load();
	int k = 0;
	int j = 0;
	int amount = 25;
	for (int i = 0; i < amount; i++) {
		auto objP = new TestHover;
		objP->getComponent<Motor::TransformComponent>()->position.set(k, j);
		objP->getComponent<Motor::TransformComponent>()->scale.set(128, 128);
		objP->addComponent<Motor::SpriteComponent>(texture);
		objP->addComponent<Motor::SpriteColliderComponent>();
		objP->getComponent<Motor::SpriteColliderComponent>()->registerMouseHoverEvent();
		k += 175;
		if (k >= amount * 8) {
			k = 0;
			j += 175;
		}
		objBuffer.push_back(objP);
	}

	objGenDone = true;
}

int main() {
#ifdef LINUX
	sysinfo(&sysInfo);
#endif
	Motor::Path::set_save_game_folder("motor");
	srand(time(nullptr));
	getriebe.init("Test", 1920, 1080, SDL_WINDOW_RESIZABLE, SDL_RENDERER_ACCELERATED);
	getriebe.getGame()->forceSetScene(new Motor::Scene());
	getriebe.getGame()->setTextFPS(14);

	//SDL_Surface* surface = Motor::SPR::create(Motor::SPR::logo8, 8);
	//SDL_SetWindowIcon(getriebe.sdl_window(), surface);
	//SDL_FreeSurface(surface);

	auto autoMover = new AutoMoverObject;
	getriebe.getGame()->getCurrentScene()->addObject(autoMover);

	auto testMoving = new TestMovingObject();
	getriebe.getGame()->getCurrentScene()->addObject(testMoving);
	testMoving->addComponent<Motor::SpriteComponent>("test.png", SDL_PIXELFORMAT_RGB332);
	testMoving->getComponent<Motor::TransformComponent>()->scale.set(128, 128);
	testMoving->getComponent<Motor::SpriteComponent>()->ignoreCamera_ = true;
	testMoving->getComponent<Motor::TransformComponent>()->position.set(0, 0);
	testMoving->addComponent<Motor::PhyComponent>();
	//testMoving->addComponent<Motor::TextComponentBlended>("test2.ttf", 124)->setContent("YEET");
	testMoving->addComponent<Motor::SpriteColliderComponent>();
	testMoving->autoMover = autoMover;
	getriebe.getGame()->getCurrentScene()->getCamera().setTarget(testMoving);

	auto object = new TestHover();
	getriebe.getGame()->getCurrentScene()->addObject(object);
	object->addComponent<Motor::SpriteComponent>("test3.png", SDL_PIXELFORMAT_ARGB4444);
	object->getComponent<Motor::TransformComponent>()->position.set(Motor::Renderer::logicalWidth / 2, Motor::Renderer::logicalHeight / 2);
	object->getComponent<Motor::TransformComponent>()->scale.set(128, 128);
	object->addComponent<Motor::SpriteColliderComponent>();
	object->getComponent<Motor::SpriteComponent>()->ignoreCamera_ = false;

	objectz7 = new Motor::GameObject();
	getriebe.getGame()->getCurrentScene()->addObject(objectz7);
	objectz7->addComponent<Motor::SpriteComponent>("pixel.png");
	objectz7->getComponent<Motor::TransformComponent>()->position.set(1920 - 300 / 2 - 5, 300 / 2 + 5);
	objectz7->getComponent<Motor::TransformComponent>()->scale.set(300, 300);
	objectz7->getComponent<Motor::TransformComponent>()->depth = -1;
	objectz7->addComponent<Motor::SpriteColliderComponent>();
	objectz7->getComponent<Motor::SpriteComponent>()->translatePosition_ = false;
	objectz7->getComponent<Motor::SpriteComponent>()->ignoreCamera_ = true;

	auto objectz6 = new MouseRectObj();
	getriebe.getGame()->getCurrentScene()->addObject(objectz6);
	objectz6->addComponent<Motor::SpriteComponent>("pixel.png");
	objectz6->getComponent<Motor::TransformComponent>()->position.set(Motor::Renderer::logicalWidth / 2, Motor::Renderer::logicalHeight / 2);
	objectz6->getComponent<Motor::TransformComponent>()->scale.set(32, 32);
	objectz6->getComponent<Motor::TransformComponent>()->depth = -1;
	//objectz7->getComponent<Motor::TransformComponent>()->color.set(15, 100, 20, 255);
	objectz6->addComponent<Motor::SpriteColliderComponent>();
	objectz6->getComponent<Motor::SpriteComponent>()->translatePosition_ = false;
	//objectz7->getComponent<Motor::SpriteComponent>()->blendMode_ = SDL_BLENDMODE_BLEND;
	//getriebe.getGame()->hideMouse();

	auto debugText = new DebugText;
	debugText->coordinateObject = testMoving;
	debugText->addToCurrentScene();
	debugText->addComponent<Motor::SpriteComponent>("");

	auto keyListener = new KeyListener;
	keyListener->addToCurrentScene();

	Motor::Event<std::function<float(int, int)>, int, int> testEvent{};
	std::function<float(int, int)> test = [](int a, int b) { return a + b; };
	testEvent.attach(&test);
	//std::cout << testEvent.dispatch(1, 2) << std::endl;

	std::thread addObjectsThread{ addPlaceHolderObjsThread };

	Motor::GameObject* cursor = Motor::GameObject::find("cursor");

	auto* dynTexture = new Motor::DynamicTexture("test4.png");
	auto destroyObj = new DestroyObj1();
	destroyObj->addComponent<Motor::DynamicSpriteComponent>(dynTexture);
	destroyObj->getComponent<Motor::TransformComponent>()->position.set(-170, -50);
	destroyObj->getComponent<Motor::TransformComponent>()->scale.set(64, 64);
	destroyObj->addToCurrentScene();

	auto destroyObj2 = new DestroyObj2();
	destroyObj2->addComponent<Motor::DynamicSpriteComponent>(dynTexture);
	destroyObj2->addComponent<Motor::DynamicSpriteComponent>(dynTexture);
	destroyObj2->getComponent<Motor::TransformComponent>()->position.set(-450, -50);
	destroyObj2->getComponent<Motor::TransformComponent>()->scale.set(64, 64);
	destroyObj2->addComponent<Motor::SpriteColliderComponent>();
	destroyObj2->getComponent<Motor::DynamicSpriteComponent>()->translate_.set(64, 64);
	destroyObj2->addToCurrentScene();
	destroyObj2->getComponent<Motor::SpriteColliderComponent>(5);

    Motor::Reg::GAME_OBJECT->registerObject(destroyObj2, "test");
	MOTOR_LOG(Motor::Reg::GAME_OBJECT->getObjects()[0]->getKey().asString());
	auto colorTest = new Motor::GameObject;
	colorTest->addComponent<Motor::MultiColorText>("Anonymous.ttf", 120);
	colorTest->getComponent<Motor::MultiColorText>()->setContent("Start COLOR{\"color\":[144,0,0,255],\"text\":\" Test2 \"}COLOR End. Next COLOR{\"color\":[144,0,0,255],\"text\":\" Text \"}COLOR continue i hate this so fucking much COLOR{\"color\":[52,52,155,255],\"text\":\" AMOGUS \"}COLOR.");
	colorTest->getComponent<Motor::MultiColorText>()->ignoreCamera_ = true;
	colorTest->getComponent<Motor::MultiColorText>()->alignment_ = Motor::TextAlignment::LEFT;
	colorTest->getComponent<Motor::TransformComponent>()->position.set(3, 650);
	colorTest->getComponent<Motor::TransformComponent>()->scale.set(32, 32);
	colorTest->addToCurrentScene();

	getriebe.getGame()->getCurrentScene()->sortObjects();
	getriebe.getGame()->forceChangePixelFormat(SDL_PIXELFORMAT_RGB332);
	//getriebe.getGame()->changeResolution(2560, 1440);
	//SDL_SetWindowFullscreen(getriebe.sdl_window(), true);
	SDL_DisplayMode mode{};
	SDL_GetCurrentDisplayMode(0, &mode);
	std::cout << mode.refresh_rate << std::endl;
	getriebe.getGame()->setFPS(mode.refresh_rate);
	//getriebe.getGame()->setFPS(1000);
	//getriebe.getGame()->changeResolution(mode.w * 0.75, mode.h * 0.75);
	discordApi.start(903707517236961290, EDiscordCreateFlags::DiscordCreateFlags_NoRequireDiscord);
	std::thread discordThread{ discordThreadFunction };
	//getriebe.start();
	std::cout << "Built On " << __DATE__ << " ;; " << __TIME__ << " ;; STD " << __cplusplus << std::endl;
	while (getriebe.getGame()->running_) {
		if (objGenDone) {
			for (Motor::GameObject* obj : objBuffer) {
				//obj->getComponent<Motor::SpriteColliderComponent>()->addMouseClick();
				//obj->getComponent<Motor::SpriteColliderComponent>()->addMouseHover();
				obj->addToCurrentScene();
			}
			objGenDone = false;
			//objBuffer.clear();
			getriebe.getGame()->getCurrentScene()->sortObjects();
		}
		getriebe.getGame()->loop();
	}
	discordApi.interrupt();
	discordThread.join();
	getriebe.stop();
	return 0;
}
#endif