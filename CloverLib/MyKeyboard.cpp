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
		// ‰¼ˆ—
		return this->key_state_.A;
	}

	bool MyKeyboard::IsLeftDown()
	{
		// ‰¼ˆ—
		return this->key_tracker_.IsKeyPressed(Keyboard::A);
	}

	bool MyKeyboard::IsRight()
	{
		// ‰¼ˆ—
		return this->key_state_.D;
	}

	bool MyKeyboard::IsRightDown()
	{
		// ‰¼ˆ—
		return this->key_tracker_.IsKeyPressed(Keyboard::D);
	}

	bool MyKeyboard::IsUp()
	{
		return this->key_state_.W;
	}

	bool MyKeyboard::IsUpDown()
	{
		return this->key_tracker_.IsKeyPressed(Keyboard::W);
	}

	bool MyKeyboard::IsDown()
	{
		return this->key_state_.S;
	}

	bool MyKeyboard::IsDownDown()
	{
		return this->key_tracker_.IsKeyPressed(Keyboard::S);
	}

	bool MyKeyboard::IsControlLeft()
	{
		return this->key_state_.Left;
	}

	bool MyKeyboard::IsControlRight()
	{
		return this->key_state_.Right;
	}

	bool MyKeyboard::IsControlUp()
	{
		return this->key_state_.Up;
	}

	bool MyKeyboard::IsControlDown()
	{
		return this->key_state_.Down;
	}
}