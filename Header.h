#pragma once
#include "ofMain.h"



class Anchor {
private:
	bool delivery = false;
	bool success = false;
	int timer = 0;

public:
	Anchor() {}

	void draw(ofVec3f vec1, ofVec3f vec2) {
		if (delivery) {
			ofSetLineWidth(1);
			ofSetColor(200, 50, 0, 255);
			timer++;
			if (timer > 50) {
				timer = 0;
				success = true;
				delivery = false;
			}
		}
		else {
			ofSetLineWidth(0.5);
		}
		ofDrawLine(vec1, vec2);

	}

	void setDelivery(bool b) {
		delivery = b;
	}
	bool getSuccess() {
		return success;
	}
	void setSuccess(bool s) {
		success = s;
	}

};


class Node {
private:
	double power = 4;
	double temperature = 1;
	ofVec3f position;
	ofVec3f centre;
	ofVec3f base_velocity;
	double rotation;
	int timer = 0;
	vector<shared_ptr<Anchor>> connections;
	vector<shared_ptr<Node>> other_nodes;
	double speed = 0.01;
	bool marked_for_destruction = false;
	bool delivery = false;
	bool colonized = false;
	bool base = false;

public:
	/// RULES
	// BASIC
	// new planets give nearby stars 1 power each
	// each colony gives 1 population
	// colonies will send out ships to colonize anchored planets if their temperature < 50 and power <= 7

	// POWER
	// if star power >= 8, double its total amount of power is transferred to nearby stars
	// power transfer decreases temperature by 25%
	// if a base has > 5 power, excess will be transferred to player storage

	// TEMPERATURE
	// temperature will rise if distance between stars < 50, by the amount of power/5
	// temperature > 250 or power > 35 results in supernova
	// temperature > 50 results in death of a star population




	Node(ofVec3f _position, ofVec3f _centre, bool _isbase) {
		position = _position;
		centre = _centre;
		rotation = ofRandom(1000);
		base_velocity = ofVec3f(ofRandom(-0.05, 0.05), ofRandom(-0.05, 0.05), 0);
		base = _isbase;
	}

	void draw() {
		timer++;
		move();

		if (colonized) {
			float alpha = sin(rotation / 35) * 200;
			if (alpha < 0) {
				alpha *= -1;
			}
			ofSetColor(0, alpha/2, 200, alpha);
			ofDrawSphere(position, power * 2);
			if (temperature > 50) {
				colonized = false;
				base = false;
			}
		}

		ofSetColor(temperature, temperature/2, 0);
		if (base) {
			ofSetColor(0, 100, 200, 150);
		}
		ofDrawSphere(position, power * 1.2);


		// Power
		if (power >= 8 || (base && power > 4)) {
			ofSetColor(255, 150, 0, 200);
			ofDrawSphere(position, power / 2);

			if (timer % 200 == 0 && temperature < 250) {
				delivery = true;
				for (int i = connections.size() - 1; i >= 0; i--) {
					if (other_nodes[i] && (other_nodes[i]->isBase() || other_nodes[i]->isColonized())) {
						connections[i]->setDelivery(true);
					}
				}
			}
		}

		// Temperature
		if (temperature > 250 || power > 35) {
			temperature++;
			power++;
			if (temperature < 300 && !marked_for_destruction) {
				for (int i = 0; i < other_nodes.size(); i++) {
					if (other_nodes[i]) {
						other_nodes[i]->addTemperature(power/ 50);
					}
				}
			}
			else {
				marked_for_destruction = true;
			}
		}


		// Connections and Power Exchange
		ofSetColor(0);
		for (int i = connections.size() - 1; i >= 0; i--) {
			if (delivery) {
				if (connections[i]->getSuccess()) {
					connections[i]->setSuccess(false);
					if (other_nodes[i]) {
						if (other_nodes[i]->isBase()) {
							other_nodes[i]->addPower(1);
							power -= 1;
						}
						else if (other_nodes[i]->isColonized()) {
							other_nodes[i]->addPower(2);
							power -= 2;
						}
					}
					if (temperature > 0) {
						temperature /= 1.25;
					}
					delivery = false;
				}
			}
			if (other_nodes[i] && !other_nodes[i]->getMarkedForDestruction()) {
				connections[i]->draw(position, other_nodes[i]->getPosition());
			}
		}
	}


	// Movement
	void move() {
		ofVec3f gravitation = { 0,0,0 };
		if (other_nodes.size() > 1) {
			for (int i = 0; i < other_nodes.size(); i++) {
				if (other_nodes[i] && !other_nodes[i]->getMarkedForDestruction()) {
					if (other_nodes[i]->getPosition().distance(position) < 300) {
						gravitation += (other_nodes[i]->getPosition().cross(position) * other_nodes[i]->getPower() / 3) / 40000;
					}
					if (other_nodes[i]->getPosition().distance(position) < 50) {
						temperature += power / 50;
					}
				}
			}
		}
		position += gravitation * speed;
		position += base_velocity;

		// Rotation
		rotation += 0.01;
		//position.x += sin(rotation) * 0.1;
		//position.y += cos(rotation) * 0.1;
	}


	// Create Connections
	void createConnection(shared_ptr<Node> otherNode) {
		bool has_connection = false;
		for (int i = 0; i < other_nodes.size(); i++) {
			if (other_nodes[i] && !other_nodes[i]->getMarkedForDestruction()) {
				if (otherNode == other_nodes[i]) {
					has_connection = true;
					break;
				}
			}
		}
		if (!has_connection) {
			other_nodes.push_back(otherNode);
			connections.push_back(make_unique<Anchor>());
		}
	}


	// Get and Set
	ofVec3f getPosition() {
		return position;
	}
	float getPower() {
		return power;
	}
	void addPower(float _power) {
		power += _power;
	}
	float getTemperature() {
		return temperature;
	}
	void addTemperature(float _temperature) {
		temperature += _temperature;
	}

	bool getMarkedForDestruction() {
		return marked_for_destruction;
	}
	bool isColonized() {
		return colonized;
	}
	bool isBase() {
		return base;
	}
	void setIsColonized(bool b) {
		colonized = b;
	}
	vector<shared_ptr<Node>> getNodes() {
		return other_nodes;
	}
};





class Ship {
private:
	ofVec3f position;
	shared_ptr<Node> target_node;
	double speed = 1;
	bool markedForDestruction = false;
	ofVec3f velocity = { 0,0,0 };
public:
	Ship(ofVec3f start, shared_ptr<Node> _target_node) {
		position = start;
		target_node = _target_node;
	}

	void draw() {
		ofSetColor(0, 100, 200);
		ofPushMatrix();
		ofTranslate(position);
		ofRotate(velocity.dot(ofVec3f(1,0,0)));
		ofDrawTriangle(-6, 0, 0, 6, 6, 0);
		ofPopMatrix();
	}

	void run() {
		draw();
		move();
	}
	void move() {
		if (target_node) {
			velocity = (target_node->getPosition() - position).normalize() * speed;
			position += velocity;
			if (position.distance(target_node->getPosition()) < 5) {
				markedForDestruction = true;
			}
		}
		else {
			markedForDestruction = true;
		}
	}
	bool getMarkedForDestruction() {
		return markedForDestruction;
	}
	shared_ptr<Node> getNode() {
		return target_node;
	}
};




class NodeSystem {
private:
	ofVec3f position;
	vector<shared_ptr<Node>> nodes;
	vector<unique_ptr<Ship>> ships;

	int timer = 0;
	double *playerPower = 0;
	double *playerPopulation = 0;
	double *totalStars = 0;
	double *totalPower = 0;

	bool first_colony_placed = false;

public:
	NodeSystem(ofVec3f _position, double *_player_power, double *_player_population, double *_totalStars, double *_totalPower) {
		position = _position;
		playerPower = _player_power;
		playerPopulation = _player_population;
		totalStars = _totalStars;
		totalPower = _totalPower;
	}


	// Spawn Node
	void spawnNode(bool _isbase, ofVec3f nodePosition = {}) {
		ofVec3f pos;
		if (!nodePosition.x) {
			pos = { ofRandom(-800,800),ofRandom(-450,450), 0 };
			pos += position;
		}
		else {
			pos = nodePosition;
		}

		nodes.push_back(make_shared<Node>(pos, position, _isbase));
		for (int i = 0; i < nodes.size() -1; i++) {
			if (nodes[i] && !nodes[i]->getMarkedForDestruction()) {
				float length = nodes[i]->getPosition().distance(nodes[nodes.size() - 1]->getPosition());
				if (length <= 150 || (length < 200 && _isbase && nodes[i]->isColonized())) {
					nodes[i]->createConnection(nodes[nodes.size() - 1]);
					nodes[nodes.size() - 1]->createConnection(nodes[i]);
					if (!nodes[i]->isColonized() && !nodes[i]->isBase()) {
						nodes[i]->addPower(1);
					}
				}
			}
		}
	}


	// Draw Node System and Erase
	void draw() {
		ofSetColor(0);

		// Draw and Erase Nodes
		for (int i = nodes.size() -1; i >= 0; i--) {
			if (nodes[i] && !nodes[i]->getMarkedForDestruction()) {
				nodes[i]->draw();
			}
			else {
				if (!nodes[i]->isBase()) {
					ofVec3f pos = nodes[i]->getPosition() + ofRandom(-100, 100);
					pos.z = 0;
					spawnNode(false, pos);
				}
				nodes.erase(nodes.begin() + i);
			}
		}

		// Draw and Erase Ships
		for (int i = ships.size() - 1; i >= 0; i--) {
			if (ships[i]) {
				if (!ships[i]->getMarkedForDestruction()) {
					ships[i]->run();
				}
				else {
					ships[i]->getNode()->setIsColonized(true);
					ships.erase(ships.begin() + i);
					first_colony_placed = true;
				}
			}
			else {
				ships.erase(ships.begin() + i);
				first_colony_placed = true;
			}
		}
	}


	// Run
	void run() {
		timer++;
		if (timer < 70) {
			//if (timer < 2) {
			//	spawnNode(true, ofVec3f(200,200,0));
			//}
			spawnNode(false);
		}
		draw();

		if (timer % 100 == 0) {
			calculatePlayerPowerProduction();
		}
		if (timer % 200 == 0) {
			spawnNode(false);
		}
		if (timer % 500 == 0) {
			checkBounds();
		}
	}


	// Power Production and Population
	void calculatePlayerPowerProduction() {
		double power = 0;
		double population = 0;
		bool ship_sent = false;

		int total_power = 0;

		for (int i = 0; i < nodes.size(); i++) {
			if (nodes[i] && nodes[i]->getTemperature() < 250) {
				total_power += nodes[i]->getPower();
				if (nodes[i]->isColonized() && !nodes[i]->getMarkedForDestruction()) {

					// increase power and population
					population += 1;
					if (nodes[i]->getPower() > 4) {
						double diff = (nodes[i]->getPower() - 4);
						power += diff;
						nodes[i]->addPower(-diff);
					}

					// send ships
					/*
					if (!ship_sent && (timer % 300 == 0)) {
						for (int j = 0; j < nodes[i]->getNodes().size(); j++) {
							shared_ptr<Node> other_node = nodes[i]->getNodes()[j];
							if (other_node && !other_node->isColonized() && !other_node->getMarkedForDestruction() && other_node->getPower() <= 7 && other_node->getTemperature() < 50) {
								ships.push_back(make_unique<Ship>(nodes[i]->getPosition(), other_node));
								ship_sent = true;
								break;
							}
						}
					}
					*/
				}
			}
		}
		*playerPower += power;
		*playerPopulation = population;
		*totalStars = nodes.size();
		*totalPower = total_power;
	}

	bool colonize() {
		bool ship_sent = false;
		for (int i = 0; i < nodes.size(); i++) {
			if (nodes[i] && nodes[i]->getTemperature() < 250) {
				if ((nodes[i]->isColonized() || nodes[i]->isBase()) && !nodes[i]->getMarkedForDestruction()) {
					// send ships
					if (!ship_sent) {
						for (int j = 0; j < nodes[i]->getNodes().size(); j++) {
							shared_ptr<Node> other_node = nodes[i]->getNodes()[j];
							if (other_node && !other_node->isColonized() && !other_node->isBase() && !other_node->getMarkedForDestruction() && other_node->getPower() <= 7 && other_node->getTemperature() < 50) {
								ships.push_back(make_unique<Ship>(nodes[i]->getPosition(), other_node));
								ship_sent = true;
								break;
							}
						}
					}
				}
			}
		}
		return ship_sent;
	}


	void checkBounds() {
		for (int i = nodes.size() - 1; i >= 0; i--) {
			if (nodes[i]) {
				ofVec3f pos = nodes[i]->getPosition();
				bool destroy = false;
				if (pos.x > ofGetScreenWidth() || pos.x < 0) {
					destroy = true;
				}
				else if (pos.y > ofGetScreenHeight() || pos.y < 0) {
					destroy = true;
				}
				if (destroy) {
					nodes.erase(nodes.begin() + i);
				}
			}
		}
	}

	bool placedFirstColony() {
		return first_colony_placed;
	}
};

