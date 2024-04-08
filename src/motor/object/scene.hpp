/* Jayson Hartmann 27/03/2022 */
#pragma once

#include <fuchslib/shorter.hpp>
#include <fuchslib/ruid.hpp>
#include "gameobject.hpp"
#include "camera.hpp"
#include <memory>

namespace Motor {
	

	class ObjectHolder {
	public:
		GameObject* obj_ = nullptr;

	public:
		explicit ObjectHolder(GameObject* object): obj_(object) {}

		bool exists() {
			return obj_;
		}

		GameObject* get() {
			return obj_;
		}

		void remove() {
			delete obj_;
			obj_ = nullptr;
		}
	};

	inline bool compareDepths(pair<int, ObjectHolder*>& a, pair<int, ObjectHolder*>& b) {
		return a.first < b.first;
	}

	class Game;
	class Scene {
	Fuchs::RUID ruid_ = Fuchs::generateRUID("SCENE");
	vector<ObjectHolder*> objects_{};
	Camera camera_{};
	vector<GameObject*> objectsCache_{};
	bool shouldSortObjects = true;

	public:
		Fuchs::RUID getRUID() const { return this->ruid_; }
		Fuchs::RUID& getRUID() { return this->ruid_; }
		Camera& getCamera() { return this->camera_; }
		void setCamera(Camera camera) { this->camera_ = camera; }

        void update();

		/*
			Adds an Object to this Scene
			If it has not been created before (BOOL), onCreate will be called
			!WHEN CREATING AN OBJECT WITH 'NEW' IT'S NOT CREATED!
		*/

		void addObject(GameObject* objIn, bool sortObj = true) {
			objectsCache_.push_back(objIn);
			shouldSortObjects = sortObj;
		}

		void addCachedObjects() {
			for (GameObject* objIn : objectsCache_) {
				auto* holder = new ObjectHolder{ objIn };
				objects_.emplace_back(std::move(holder));
				objIn->holder_ = &objects_.back();
				if (!objIn->created_) {
					objIn->ruid_ = Fuchs::generateRUID();
					objIn->inner_create();
					objIn->onCreate();
					objIn->created_ = true;
					Events::gameObjectSceneAdd.dispatch(&holder);
				}
			}
			if (!objectsCache_.empty() && shouldSortObjects) {
				sortObjects();
				shouldSortObjects = true;
			}
			objectsCache_.clear();
		}

		vector<ObjectHolder*>& getObjects() {
			return objects_;
		}

        void sortObjects() {
            std::thread thread = std::thread(&Scene::sortObjectsThread, this);
            thread.detach();
        }
		void sortObjectsThread();
		void cleanObjects();
        void readdPersistent(std::vector<GameObject*>& objs) {
            for (const auto &item: objs) {
                item->addToCurrentScene();
            }
            objs.clear();
        }
		virtual void init(Game* game) {};
		virtual void onSwitch(Scene* newScene, Game* game) {}
	};
}