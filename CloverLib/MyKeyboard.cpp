#include "MyKeyboard.h"

using namespace DirectX;
using namespace std;

namespace CloverLib
{
	MyKeyboard::MyKeyboard()
		:keyboard_(make_unique<Keyboard>())
	{}


	MyKeyboard::~MyKeyboard()
	{}

	void MyKeyboard::Update()
	{
		this->key_state_ = this->keyboard_->GetState();
		this->key_tracker_.Update(this->key_state_);
	}

	bool MyKeyboard::IsLeft()
	{
		// ������
		return this->key_state_.A;
	}

	bool MyKeyboard::IsLeftDown()
	{
		// ������
		return this->key_tracker_.IsKeyPressed(Keyboard::A);
	}

	bool MyKeyboard::IsRight()
	{
		// ������
		return this->key_state_.D;
	}

	bool MyKeyboard::IsRightDown()
	{
		// ������
		return this->key_tracker_.IsKeyPressed(Keyboard::D);
	}
}