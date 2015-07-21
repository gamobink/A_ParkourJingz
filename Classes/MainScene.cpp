#include "MainScene.h"
#include "MainMenuLayer.h"
#include "BackgroundLayer.h"
#include "GameLayer.h"
USING_NS_CC;


MainScene::MainScene() 
	:_pSelfCamera(nullptr)
{
}

MainScene::~MainScene()	
{
	CC_SAFE_RELEASE_NULL(_pSelfCamera);
}


// on "init" you need to initialize your instance
bool MainScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

	Size winSize = Director::getInstance()->getWinSize();

	auto cameraBackground2D = Camera::createOrthographic(winSize.width, winSize.height, 0, 100);
	cameraBackground2D->setCameraFlag(CameraFlag::USER2);
	this->addChild(cameraBackground2D);
	cameraBackground2D->setDepth(-5);

	//todo ������������˳������з�ʽ����
	auto cameraMainUI = Camera::createOrthographic(winSize.width, winSize.height, 0, 100);
	cameraMainUI->setCameraFlag(CameraFlag::USER4);
	this->addChild(cameraMainUI);
	cameraMainUI->setDepth(5);
	

	auto camera3D = Camera::createPerspective(35, GLfloat(winSize.width) / GLfloat(winSize.height), 1, 1000);
	camera3D->setCameraFlag(CameraFlag::DEFAULT);
	camera3D->setPosition3D(Vec3(0, 15, 15));
	camera3D->lookAt(Vec3(0, 15, -60), Vec3(0, 1, 0));
	this->addChild(camera3D);




	//Զ������ʹ�����ӽǣ��̶������λ��
	auto pBgLayer = BackgroundLayer::create();
	this->addChild(pBgLayer, -20, "UI_Background");

	//���˵����棬ʹ�����ӽǣ��̶������λ��
	auto pMenuLayer = MainMenuLayer::create();
	this->addChild(pMenuLayer, 20, "UI_MainMenu");

	//3D
	auto pGameLayer = GameLayer::create();
	this->addChild(pGameLayer, 0, "Game_Main");

	
	

	//ÿ����ʹ�����Լ���������ӽ�
	pBgLayer->setCameraMask(static_cast<unsigned short>(CameraFlag::USER2));
	
	pGameLayer->setCameraMask(static_cast<unsigned short>(CameraFlag::DEFAULT));

	pMenuLayer->setCameraMask(static_cast<unsigned short>(CameraFlag::USER4));


	//scheduleUpdate();

    return true;
}

//void MainScene::update(float dt)
//{
//	
//}


