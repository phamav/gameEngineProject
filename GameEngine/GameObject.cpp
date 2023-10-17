#include "GameObject.h"

#include "Component.h"
#include "MeshComponent.h"

#include "Game.h"

#define VERBOSE false

// ***** Definition of static members of the Game Object class *****
Game* GameObject::owningGame;

std::vector<std::shared_ptr<class GameObject>> GameObject::pendingChildren;

std::vector<std::shared_ptr<class GameObject>> GameObject::removedGameObjects;

std::vector<ReparentPair> GameObject::reparentedGameObjects;

// ********************************************************************

GameObject::GameObject()
	: gameObjectState(ACTIVE)
{

}


GameObject::~GameObject()
{
	// Remove the game object from the game
	if (VERBOSE) cout << "GameObject destructor called" << endl;
	 
} // end GameObject destructor


void GameObject::initialize()
{
	// Initialize the components that are attached to this game object
	for (auto & component : components) {

		component->initialize();
	}

	// Make sure the any initialization of position, orientation
	// or scale done by Components is accounted for on the first 
	// frame
	this->updateModelingTransformation();

	for (auto& gameObject : this->children) {

		gameObject->initialize();
	}

} // end initialize

void GameObject::update(const float & deltaTime)
{
	// Check to see if this game object is active
	if (gameObjectState == ACTIVE) {

		// Update the components that are attached to to this game object
		for (auto & component : this->components) {

			component->update(deltaTime);
		}

		// Call the super class method to update the modeling transformation
		updateModelingTransformation();

		// Update the children of this game object
		for (auto& gameObject : this->children) {

			gameObject->update(deltaTime);
		}
	}

} // end update

//void GameObject::updateGameObject(const float & deltaTime)
//{
//	// Override to create specialize update
//
//} // end updateGameObject

void GameObject::processInput()
{
	// Check to see if this game object is active
	if (gameObjectState == ACTIVE) {

		// First process input for components
		for (auto & component : this->components) {
			component->processInput();
		}
	}

	// Call process input on all the children
	for (auto& gameObject : this->children) {

		gameObject->processInput();
	}

} // end processInput

//void GameObject::gameObjectInput()
//{
//	// Override to create custom input response
//
//} // end gameObjectInput

void GameObject::addComponent(std::shared_ptr<Component> component)
{
	// Dependency injection (give the Component a reference to this GameObject
	component->owningGameObject = shared_from_this();

	// Add the component to the components vector
	components.emplace_back(component);

	// Sort the components vector based on their update order.
	std::sort(components.begin(), components.end(), Component::CompareUpdateOrder);

	// Check if the component is a MeshComponent
	if (component->getComponentType() == MESH) {

		// Add the mesh to the static vector of MeshComponents
		MeshComponent::addMeshComp(std::static_pointer_cast<MeshComponent>(component));
	}

} // end addComponent


void GameObject::removeComponent(std::shared_ptr< Component> component)
{
	// Find the component to be removed
	auto iter = std::find(components.begin(), components.end(), component);

	// Check if the component was found
	if (iter != components.end()) {

		if (VERBOSE) cout << "Component" << " removed." << endl;

		// Check if the component is a Mesh
		if (component->getComponentType() == MESH) {

			// Remove the mesh from the static vector of MeshComponents
			MeshComponent::removeMeshComp(std::static_pointer_cast<MeshComponent>(component));

		}

		std::iter_swap(iter, components.end() - 1);
		components.pop_back();
	}

} // end removeComponent


void GameObject::setState(STATE state)
{
	gameObjectState = state;
}

void GameObject::addChildGameObject(std::shared_ptr<class GameObject> gameObject)
{
	if (gameObject != NULL) {

		// Dependency injection so the game object can
		// access the game that it is a part of
		gameObject->owningGame = owningGame;

		// Dependency injection so the game object has a
		// reference to its parent GameObject
		gameObject->parent =  static_cast<std::shared_ptr<GameObject>>(this);

		// Check if the game has started
		if (owningGame->isRunning) {

			if (VERBOSE) cout << "pending add" << endl;
			// Add to the pending list so the object is
			// added after the next update
			pendingChildren.emplace_back(gameObject);
		}
		else {

			if (VERBOSE) cout << "direct add" << endl;
			// Game has not started. Add directly to the 
			// vector of game objects in the game.
			children.emplace_back(gameObject);
		}
	}

} // end addGameObject

void GameObject::removeAndDelete()
{
	// Add to objects to be removed after update is complete
	removedGameObjects.emplace_back(shared_from_this());

} // end removeGameObject


std::vector<std::shared_ptr<GameObject>> GameObject::GetChildren()
{
	return children;

} // end GetChildren


void GameObject::reparent(std::shared_ptr<GameObject> child)
{
	// Store the game object with its new parent for 
	// actual reparenting after the next update cycle.
	reparentedGameObjects.emplace_back(ReparentPair(static_cast<std::shared_ptr<GameObject>>(shared_from_this()), child));
	
} // end reparent


void GameObject::UpdateSceneGraph()
{
	AddPendingGameObjects();
	RemoveDeletedGameObjects();
	ReparentGameObjects();

} // end ManageGameObjectChanges


void GameObject::RemoveDeletedGameObjects()
{
	// Set a found bool to an initial value of false.
	bool found = false;

	// Handle game objects marked for removal
	for (auto gameObject : removedGameObjects) {

		std::shared_ptr<class GameObject> parentGameObject = gameObject->parent;

		// Search children vector of the parent
		auto iter = std::find(parentGameObject->children.begin(), parentGameObject->children.end(), gameObject);

		// Check if the GameObject was found in children vector of the parent
		if (iter != parentGameObject->children.end()) {

			// Indicate the GameObject was found
			found = true;

			// Swap to end of vector and pop off (avoid erase copies)
			std::iter_swap(iter, parentGameObject->children.end() - 1);
			parentGameObject->children.pop_back();
		}

		// Was it found in the children of the parent?
		if (found == false) {

			// Search pendingChildren vector of the parent
			iter = std::find(pendingChildren.begin(), pendingChildren.end(), gameObject);

			// Check if the GameObject was found in pendingChildren vector of the parent
			if (iter != pendingChildren.end()) {

				// Indicate the GameObject was found
				found = true;

				// Swap to end of vector and pop off (avoid erase copies)
				std::iter_swap(iter, pendingChildren.end() - 1);
				pendingChildren.pop_back();
			}
		}
	}

	// Clear the list for the next update cycle
	removedGameObjects.clear();

} // end RemoveDeletedGameObjects

void GameObject::AddPendingGameObjects()
{
	// Attach any pending game objects to the game
	for (auto pending : pendingChildren) {

		if (VERBOSE) cout << "Delayed adddtion of pending object" << endl;

		// Cast to a GameObject * to get to the children vector
		std::shared_ptr<class GameObject> parentGameObject = pending->parent;

		// Add the pending gameObject to the parent's child list
		parentGameObject->children.emplace_back(pending);

		// Same as initializing at the begining of the game
		pending->initialize();

		// Ensure the modeling transformation is updated
		pending->updateModelingTransformation();

		// Helps keep the object from appearing at the World
		// coordinate origin for one frame
		pending->update(0.0f);
	}

	// All pending game objects have been added. Clear for next update.
	pendingChildren.clear();

} // end AddPendingGameObjects

void GameObject::ReparentGameObjects()
{
	// Handle all of the reparented GameObject
	for (auto& reparentPair : reparentedGameObjects) {

		if (VERBOSE) cout << "Reparenting game object." << endl;

		// Temporary variables of the correct type
		std::shared_ptr<class GameObject> oldParent = reparentPair.child->parent;
		//GameObject* oldParent = reparentPair.child->parent;

		// Get the World transform of the child
		glm::mat4 oldWorldTransformation = reparentPair.child->getWorldTransform();

		// Get the World transform of the new parent
		mat4 newParentWorldTrans = reparentPair.newParent->getWorldTransform();

		// Get inverse of the new parent World transformation and multiply times the child World.
		glm::mat4 newChildTransform = glm::inverse(newParentWorldTrans) * oldWorldTransformation;

		// Overwrite the position scale and rotation so that the child will not move
		// or rotate when it is reparented.
		reparentPair.child->localTransform = newChildTransform;

		// Find the reparented child among the old parent's children
		auto iterator = std::find(oldParent->children.begin(), oldParent->children.end(), reparentPair.child);

		// Check if a child of the parent
		if (iterator != oldParent->children.end()) {

			// Remove the reparented child from the old parent's "family"
			std::iter_swap(iterator, oldParent->children.end() - 1);
			oldParent->children.pop_back();
		}

		// Have the new parent adopt the child
		reparentPair.child->parent = reparentPair.newParent;

		// Add the child to the new parent's children
		reparentPair.newParent->children.emplace_back(reparentPair.child);
	}

	// Clear the list for the next update cycle
	reparentedGameObjects.clear();

} // end ReparentGameObjects