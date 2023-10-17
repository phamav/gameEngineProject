#include "JennComponent.h"
#include <iostream>
JennComponent::JennComponent()
	:Component(500)
{

} // end JennComponent constructor

void JennComponent::initialize()
{
	//cout << "initialize method called " << endl;

} //end initialize

void JennComponent::update(const float& deltaTime) {
	//cout << "update method called " << endl;
} // end update


void JennComponent::processInput() {
	//cout << "processInput method called " << endl;
} // end process input
