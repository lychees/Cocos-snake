#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;
using namespace CocosDenshion;
using namespace std;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();


    /*auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));*/

    // create menu, it's an autorelease object
    
	/*auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);*/

    auto l1 = Label::createWithTTF("Start", "fonts/Marker Felt.ttf", 32);        
    //l1->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height / 7*4));		
	auto ll1 = MenuItemLabel::create(l1, CC_CALLBACK_1(HelloWorld::menuCallbackStart, this));
	ll1->setPosition(Vec2(0, -0));
	ll1->setName("Start");

	auto l2 = Label::createWithTTF("Option", "fonts/Marker Felt.ttf", 32);	
	auto ll2 = MenuItemLabel::create(l2, CC_CALLBACK_1(HelloWorld::menuCallbackOption, this));
	ll2->setPosition(Vec2(0, -50));
	ll2->setName("Option");

	auto l3 = Label::createWithTTF("Exit", "fonts/Marker Felt.ttf", 32);	
	auto ll3 = MenuItemLabel::create(l3, CC_CALLBACK_1(HelloWorld::menuCallbackExit, this));	
	ll3->setPosition(Vec2(0, -100));
	ll3->setName("Exit");
    
	auto menu = Menu::create(ll1, ll2, ll3, NULL);
	this->addChild(menu);
    return true;
}

void HelloWorld::menuCallbackStart(Ref* p)
{
	Director::getInstance()->replaceScene(Game::scene());
}
void HelloWorld::menuCallbackOption(Ref* p)
{

}
void HelloWorld::menuCallbackExit(Ref* p)
{
	Director::getInstance()->end();
}





const int dx[] = { 0, 0, -1, 1 };
const int dy[] = { 1, -1, 0, 0 };
// 上 下 左 右

int MapX = 64, MapY = 64, Offset = 5, CellSize = 4, InitDlen = 10, FruteDlen = 5;
int GameSpeed = 50, SnakeInitSpeed = 5;

bool Deaded = false;


void getRandom(int &x, int &y){
	x = rand() % MapX, y = rand() % MapY;
}

bool Snake::init(){
	if (!Layer::init()){
		return false;
	}
	dlen = InitDlen;
	speed = SnakeInitSpeed;
	counter = 0;
	return true;
}

void Snake::reset(int _x, int _y, int _d){
	removeAllChildren();
	body.clear(); dirr.clear();

	xx = x = _x, yy = y = _y, dir = _d; dlen = InitDlen;
	speed = SnakeInitSpeed; counter = 0;

	auto t = Sprite::create("white.png", Rect(0, 0, 4, 4));
	body.push_back(t); t->setPosition(Point(x, y)*CellSize); addChild(t);
	dirr.push_back(dir);
}

bool Snake::isDead(){
	if (x < 0 || y < 0 || x >= MapX || y >= MapY) return 1; // on board ?

	// self intersect ?
	set<pair<int, int>> H; int xx = x, yy = y; H.insert(make_pair(x, y));
	for each(auto i in dirr){
		xx += dx[i], yy += dy[i];
		if (H.find(make_pair(xx, yy)) != H.end()) return 1;
		H.insert(make_pair(xx, yy));
	}
	return 0;
}

void Snake::move(){

	counter += 1;
	CCLOG("%d %d", counter, speed);
	if (counter != speed) return;
	counter = 0;

	dirr.push_front(dir);
	x += dx[dir]; y += dy[dir];

	//CCLOG("%d %d\n", body.size(), dirr.size());
	//assert(body.size() <= dirr.size());

	for (int i = 0; i < body.size(); ++i){
		body[i]->runAction(MoveBy::create(0.3, Point(dx[dirr[i]], dy[dirr[i]])*CellSize));
	}

	if (dlen > 0){
		--dlen;
		auto t = Sprite::create("white.png", Rect(0, 0, 4, 4));
		t->setPosition(Point(xx, yy)*CellSize);
		body.push_back(t); this->addChild(t);
	}
	else{
		dirr.pop_back();
		xx += dx[dirr.back()]; yy += dy[dirr.back()];
	}
}

bool Frute::init(){
	if (!Layer::init()){
		return false;
	}
	auto t = Sprite::create("yellow.png", Rect(0, 0, 4, 4));
	this->addChild(t);
	return 1;
}

void Frute::gen(){
	getRandom(x, y);
	setPosition(x*CellSize, y*CellSize);
}

void Game::eat(Snake* p){
	if (p->x == frute->x && p->y == frute->y){
		frute->gen(); p->dlen += FruteDlen;
		SimpleAudioEngine::getInstance()->playEffect("eat.wav");
		score += 1; Label* t = (Label *)getChildByTag(1);
		char strscore[20]; sprintf(strscore, "Score: %d", score);
		t->setString(strscore);
	}
}

void Game::gameLogic(float t){
	eat(p1); p1->move();
	if (p1->isDead()){
		Deaded = true;
		Label* t = (Label *)getChildByTag(2);
		t->setVisible(1);
		Director::getInstance()->pause();
	}
}

void Game::restart(){
	Deaded = false;
	Label* t = (Label *)getChildByTag(2);
	t->setVisible(0);
	p1->reset(0, 60, 3);
	frute->gen();
	Director::getInstance()->resume();

}

void Game::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event){

	//CCLOG("Key with keycode %d pressed", keyCode);

	if (keyCode == EventKeyboard::KeyCode::KEY_W){
		p1->dir = 0;
	}
	else if (keyCode == EventKeyboard::KeyCode::KEY_S){
		p1->dir = 1;
	}
	else if (keyCode == EventKeyboard::KeyCode::KEY_A){
		p1->dir = 2;
	}
	else if (keyCode == EventKeyboard::KeyCode::KEY_D){
		p1->dir = 3;
	}
	else if (keyCode == EventKeyboard::KeyCode::KEY_SPACE){

		if (Deaded){ // 死亡
			Game::restart();
		}
		else{
			Label* t = (Label *)getChildByTag(3);
			SimpleAudioEngine::getInstance()->playEffect("Pausing SFX.wav");
			if (!t->isVisible()){
				t->setVisible(1);
				Director::getInstance()->pause();
			}
			else{
				t->setVisible(0);
				Director::getInstance()->resume();
			}
		}
	}
}
Scene* Game::scene(){
	auto scene = Scene::create();
	Game *layer = Game::create();
	scene->addChild(layer);
	return scene;
}

bool Game::init(){
	if (!Layer::init()){
		return false;
	}

	auto size = Director::getInstance()->getWinSize();
	setPosition(Point(10, 45));

	auto draw = DrawNode::create();
	Point lt = Point(-Offset, CellSize*MapY), rt = Point(CellSize*MapX, CellSize*MapY),
		lb = Point(-Offset, -Offset), rb = Point(CellSize*MapX, -Offset);
	draw->drawSegment(lb, rb, 1, Color4F(1, 1, 1, 1));
	draw->drawSegment(lb, lt, 1, Color4F(1, 1, 1, 1));
	draw->drawSegment(lt, rt, 1, Color4F(1, 1, 1, 1));
	draw->drawSegment(rb, rt, 1, Color4F(1, 1, 1, 1));
	this->addChild(draw);

	p1 = Snake::create();
	p1->reset(0, 60, 3);
	this->addChild(p1);

	frute = Frute::create(); frute->gen();
	this->addChild(frute);

	auto labelScore = Label::create("Score: 0", "宋体", 14);
	labelScore->setTag(1); score = 0;
	labelScore->setPosition(Point(size.width - 80, size.height - 50));
	this->addChild(labelScore);

	auto labelGameOver = Label::create("Game Over", "宋体", 42);
	labelGameOver->setVisible(0);
	labelGameOver->setTag(2);
	labelGameOver->setPosition(Point(size.width / 2, size.height / 2));
	this->addChild(labelGameOver);

	auto labelPause = Label::create("Pause", "宋体", 42);
	labelPause->setVisible(0);
	labelPause->setTag(3);
	labelPause->setPosition(Point(size.width / 2, size.height / 2));
	this->addChild(labelPause);


	//计划任务
	this->schedule(schedule_selector(Game::gameLogic), (float)1 / GameSpeed);
	//监听键盘事件
	auto e = EventListenerKeyboard::create();
	e->onKeyPressed = CC_CALLBACK_2(Game::onKeyPressed, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(e, this);;
	return true;
}