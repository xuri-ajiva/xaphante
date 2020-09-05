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

bool ObjectHandler::DeconstructObjectFile(std::string* object_location, std::vector<Vertex>* vertices, std::vector<UInt32>* indices) {
	auto input = std::ifstream(*object_location, std::ios::in | std::ios::binary);

	if (!input.is_open()) {
		std::cout << "[ERROR]: Reading File: " << object_location << std::endl;
		return false;
	}

	char tmpLength;

	char l[6];				 //header
	l[5] = '\0';
	input.read(l, 5);

	if (std::strncmp(&l[0], "\4BOBF", 5) != 0) {
		std::cout << "[ERROR]: WRONG FILE FORMAT" << std::endl;
		return false;
	}

	char nl[2];
	nl[1] = '\0';

	input.read(nl, 1);

	std::vector<char> n;
	const auto        length = static_cast<int>(nl[0]);
	n.resize(length + 1);

	input.read(&n[0], length);
	n[length] = '\0';
	std::cout << "[NAME]: " << n.data() << std::endl;

	int size;
	input.read(reinterpret_cast<char*>(&size), sizeof(int)); //todo check

	input.read(reinterpret_cast<char*>(&NUM_VERTICES_), size);

	for (unsigned int i = 0; i < NUM_VERTICES_; ++i) {
		Vertex v {};											  
		input.read(reinterpret_cast<char*>(&v.pos), sizeof(float) * 3);	
		input.read(reinterpret_cast<char*>(&v.normal), sizeof(float) * 3);	
		//v.x += NUM_VERTICES_ / 100;

		//auto st = i % 6;
		//switch (st) {case 0:v.r = 1.0f;v.g = 0.0f;v.b = 0.0f;break;case 1:v.r = 0.0f;v.g = 1.0f;v.b = 0.0f;break;case 2:v.r = 0.0f;v.g = 0.0f;v.b = 1.0f;case 3:v.r = 0.0f;v.g = 1.0f;v.b = 1.0f;case 4:v.r = 1.0f;v.g = 1.0f;v.b = 0.0f;case 5:v.r = 1.0f;v.g = 0.0f;v.b = 1.0f;break;}
		//v.a = 1.0f;

		//std::cout << "{" << v.position.x << ", " << v.position.y << ", " << v.position.z << "}" << std::endl;
		vertices->push_back(v);
	}

	input.read(reinterpret_cast<char*>(&NUM_INDICES_), size);

	for (int i = 0; i < NUM_INDICES_; ++i) {
		UInt32 c;
		input.read(reinterpret_cast<char*>(&c), sizeof(UInt32));
		indices->push_back(c);
		//std::cout << c << std::endl;
	}

	return true;
}

void ObjectHandler::Init(std::string* object_location, ShaderHandler* shader) {
	auto* vertices = new std::vector<Vertex>();
	auto* indices  = new std::vector<UInt32>();

	if (!DeconstructObjectFile(object_location, vertices, indices)) {
		isInit = false;
		return;
	}
	isInit = true;

	VERTEX_BUFFER_ = new VertexBuffer(vertices->data(), NUM_VERTICES_);
	VertexBuffer::Unbind();

	INDEX_BUFFER_ = new IndexBuffer(indices->data(), NUM_INDICES_, sizeof(UInt32));
	IndexBuffer::Unbind();

	TEXTURE_HANDLER_ = new TextureHandler(object_location, shader);
	TextureHandler::Unbind();
}

void ObjectHandler::CleanUp(SceneCollection* node) const {
	if (!isInit) return;
	delete VERTEX_BUFFER_;
	delete INDEX_BUFFER_;
	delete TEXTURE_HANDLER_;
}

void ObjectHandler::Draw(WindowWrapper* handler) const {
	if (!isInit) return;
	VERTEX_BUFFER_->Bind();
	INDEX_BUFFER_->Bind();
	//TEXTURE_HANDLER_->Bind();

	glDrawElements(GL_TRIANGLES, NUM_INDICES_,GL_UNSIGNED_INT, nullptr);
	//glDrawArrays(GL_TRIANGLES, 0, NUM_VERTICES_);
	//VertexBuffer::Unbind();
	//IndexBuffer::Unbind();
	//TextureHandler::Unbind();
}

void ObjectHandler::GameLoop(float delta) {
	if (!isInit) return;
	
}
