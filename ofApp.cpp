#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofSetColor(39);
	ofSetLineWidth(1.5);
	ofEnableDepthTest();

	this->line.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->face.clear();
	this->line.clear();

	int deg_span = 10;
	int radius_span = 1;
	int radius_max = 600;

	ofColor color;

	for (int deg = 0; deg < 360; deg += deg_span) {

		auto noise_seed = glm::vec3(ofRandom(1000), ofRandom(1000), ofRandom(1000));
		color.setHsb(ofMap(deg, 0, 360, 0, 255), 180, 255);

		for (int radius = 60; radius < radius_max; radius += radius_span) {

			auto rotation_x = glm::rotate(glm::mat4(), ofMap(ofNoise(noise_seed.x, ofGetFrameNum() * 0.015 + radius * 0.005), 0, 1, -PI, PI), glm::vec3(1, 0, 0));
			auto rotation_y = glm::rotate(glm::mat4(), ofMap(ofNoise(noise_seed.y, ofGetFrameNum() * 0.015 + radius * 0.005), 0, 1, -PI, PI), glm::vec3(0, 1, 0));
			auto rotation_z = glm::rotate(glm::mat4(), ofMap(ofNoise(noise_seed.z, ofGetFrameNum() * 0.015 + radius * 0.005), 0, 1, -PI, PI), glm::vec3(0, 0, 1));

			vector<glm::vec3> vertices;
			vertices.push_back(glm::vec3(radius * cos((deg - deg_span * 0.5 + 0.5) * DEG_TO_RAD), radius * sin((deg - deg_span * 0.5 + 0.5) * DEG_TO_RAD), 0));
			vertices.push_back(glm::vec3(radius * cos((deg + deg_span * 0.5 - 0.5) * DEG_TO_RAD), radius * sin((deg + deg_span * 0.5 - 0.5) * DEG_TO_RAD), 0));

			for (auto& vertex : vertices) {

				vertex = glm::vec4(vertex, 0) * rotation_z * rotation_y * rotation_x;
			}

			this->face.addVertices(vertices);
			this->line.addVertices(vertices);

			this->face.addColor(color);
			this->face.addColor(color);

			if (radius > 60) {

				this->face.addIndex(this->face.getNumVertices() - 1); this->face.addIndex(this->face.getNumVertices() - 2); this->face.addIndex(this->face.getNumVertices() - 4);
				this->face.addIndex(this->face.getNumVertices() - 1); this->face.addIndex(this->face.getNumVertices() - 4); this->face.addIndex(this->face.getNumVertices() - 3);

				this->line.addIndex(this->line.getNumVertices() - 1); this->line.addIndex(this->line.getNumVertices() - 3);
				this->line.addIndex(this->line.getNumVertices() - 2); this->line.addIndex(this->line.getNumVertices() - 4);
			}
		}

		this->line.addIndex(this->line.getNumVertices() - 1); this->line.addIndex(this->line.getNumVertices() - 2);
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateX(ofGetFrameNum() * 0.185);
	ofRotateY(ofGetFrameNum() * 0.36);

	this->face.draw();
	this->line.drawWireframe();

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}