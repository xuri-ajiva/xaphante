#include "ObjectHandler.h"

#include <GL/glew.h>
#include <fstream>
#include <vector>

ObjectHandler::ObjectHandler() {}

inline std::string ReadLengthOfString(std::ifstream* s, int l) {
	std::string str;
	str.resize(l);

	char tmp;

	for (int i = 0; i < l; ++i) {
		s->read(&tmp, 8);
		str.append(&tmp);
	}
	return str.c_str();
}

bool ObjectHandler::DeconstructObjectFile(std::string* object_location, void* vertices, void* indices) {
	auto input = std::ifstream(*object_location, std::ios::in | std::ios::binary);

	if (!input.is_open()) {
		std::cout << "[ERROR]: Reading File: " << object_location << std::endl;
		return false;
	}

	char tmpLength;

	char l[5];				 //header
	l[4] = '\0';
	input.read(l, 4);

	if (std::strncmp(&l[0], "\3BSF", 4) != 0) {
		std::cout << "[ERROR]: WRONG FILE FORMAT" << std::endl;
		return false;
	}

	char nl[2];
	nl[1] = '\0';

	input.read(nl, 1);

	std::vector<char> n;
	const auto        length = static_cast<int>(nl[0]);
	n.resize(1 + length);

	input.read(&n[0], length);
	n[length] = '\0';
	std::cout << "[NAME]: " << n.data() << std::endl;

	int size;
	input.read((char*)(&size), sizeof(int)); //todo check

	///////////////////////// BEGIN READ VERTICES	 

	std::vector<Vertex>* vtc = new std::vector<Vertex>();
	std::vector<UInt32>* ind = new std::vector<UInt32>();

	input.read((char*)(&NUM_VERTICES_), size);

	for (int i = 0; i < NUM_VERTICES_; ++i) {
		Vertex v;
		input.read((char*)(&v.x), sizeof(float) * 3);
		v.u = 0;
		v.v = 0;
		v.r = 0;
		v.g = 1;
		v.b = 0;
		v.a = 1;

		vtc->push_back(v);
	}

	input.read((char*)(&NUM_INDICES_), size);

	for (int i = 0; i < NUM_VERTICES_; ++i) {
		UInt32 c;
		input.read((char*)(&c), sizeof(UInt32));
		ind->push_back(c);
	}

	vertices = vtc->data();
	indices  = ind->data();

	return true;
}

void ObjectHandler::Init(std::string* object_location, ShaderHandler* shader) {
	int VERTICES_;
	int INDICES_;

	if (!DeconstructObjectFile(object_location, &VERTICES_, &INDICES_)) {
		isInit = false;
		return;
	}
	isInit = true;

	VERTEX_BUFFER_ = new VertexBuffer(&VERTICES_, NUM_VERTICES_);
	VertexBuffer::Unbind();

	INDEX_BUFFER_ = new IndexBuffer(&INDICES_, NUM_INDICES_, sizeof(UInt32));
	IndexBuffer::Unbind();

	TEXTURE_HANDLER_ = new TextureHandler(object_location, shader);
	TextureHandler::Unbind();
}

void ObjectHandler::CleanUp(SceneCollection* node) {
	if (!isInit) return;
	delete VERTEX_BUFFER_;
	delete INDEX_BUFFER_;
	delete TEXTURE_HANDLER_;
}

void ObjectHandler::Draw(WindowWrapper* handler) const {
	if (!isInit) return;
	VERTEX_BUFFER_->Bind();
	INDEX_BUFFER_->Bind();
	TEXTURE_HANDLER_->Bind();

	glDrawElements(GL_TRIANGLES, NUM_INDICES_,GL_UNSIGNED_INT, nullptr);
	//glDrawArrays(GL_TRIANGLES, 0, NUM_VERTICES_);
	//VertexBuffer::Unbind();
	//IndexBuffer::Unbind();
	//TextureHandler::Unbind();
}

void ObjectHandler::GameLoop(SceneCollection* scene_collection) {
	if (!isInit) return;
}
