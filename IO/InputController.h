//InputController.h

#ifndef INPUTCONTROLLER_H
#define INPUTCONTROLLER_H

class InputController {

	public:
		InputController();
		void updateMouse(int,int,int,int);
		void updatePassiveMouse(int,int);

		int getMouseX();
		int getMouseY();

	private:
		bool* leftMouse;
		bool* middleMouse;
		bool* rightMouse;

		int mouseX;
		int mouseY;
};

#endif
