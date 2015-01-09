#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__


#include "cocos2d.h"
#include "SimpleAudioEngine.h"


using namespace std;
USING_NS_CC;

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();    
	void menuCallbackStart(Ref*);
	void menuCallbackOption(Ref*);
	void menuCallbackExit(Ref*);
	CREATE_FUNC(HelloWorld); virtual bool init();
};






class Snake : public Layer{
public:
	int x, y; // ͷ
	int xx, yy; // β
	int dir, dlen, speed, counter; // ���򣬳��ȵ��������ٶȣ�������
	vector<Sprite*> body; // ����
	deque<int> dirr; // ÿ������ķ���	
	void move(); bool isDead();
	CREATE_FUNC(Snake); virtual bool init(); void reset(int, int, int);
};

class Frute : public Layer{
public:
	int x, y;
	Sprite* body;
	void gen();
	CREATE_FUNC(Frute); virtual bool init();
};

class Game : public Layer{
public:
	Snake* p1; Frute* frute; int score;
	void gameLogic(float t);
	void onKeyPressed(EventKeyboard::KeyCode, Event*);
	void eat(Snake*);

	void restart();
	static Scene* scene(); void menuCloseCallback(Ref* sender);
	CREATE_FUNC(Game); virtual bool init();
};


#endif // __HELLOWORLD_SCENE_H__
