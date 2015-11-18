#include "GLElement.h"
#include "GLProg.h"


GLAssipElement::GLAssipElement(string path, int progid) {
	this->m_assipPath = path;
	this->m_progid = progid;
}

bool GLAssipElement::Initialize() {
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(m_assipPath, aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		cout << "Assimp error: " << importer.GetErrorString() << endl;
		return false;
	}
	this->ProcessNode(scene->mRootNode, scene);

	return true;
}

void GLAssipElement::Update() {
	for (int i = 0; i < m_elms.size(); i++) {
		m_elms[i]->DrawElements(GL_TRIANGLES);
	}

}

void GLAssipElement::ProcessNode(aiNode* node, const aiScene* scene) {
	for (GLuint i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		this->m_elms.push_back(this->ProcessMesh(mesh, scene));
	}
	for (GLuint i = 0; i < node->mNumChildren; i++) {
		this->ProcessNode(node->mChildren[i], scene);
	}
}

GLSimpleElement* GLAssipElement::ProcessMesh(aiMesh* mesh, const aiScene* scene) {
	GLSimpleElement* elm = new GLSimpleElement();

	vector<GLfloat> position, normal, texcoords;
	vector<GLushort> index;

	// position, normal and texCoords
	for (GLuint i = 0; i < mesh->mNumVertices; i++) {
		position.push_back(mesh->mVertices[i].x);
		position.push_back(mesh->mVertices[i].y);
		position.push_back(mesh->mVertices[i].z);

		normal.push_back(mesh->mNormals[i].x);
		normal.push_back(mesh->mNormals[i].y);
		normal.push_back(mesh->mNormals[i].z);

		if (mesh->mTextureCoords[0]) {
			texcoords.push_back(mesh->mTextureCoords[0][i].x);
			texcoords.push_back(mesh->mTextureCoords[0][i].y);
		}
	}

	// indexes
	for (GLuint i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (GLuint j = 0; j < face.mNumIndices; j++) {
			index.push_back(face.mIndices[j]);
		}
	}


	elm->LoadVertexArray(GLfloatArray(position.size() / 3, 3, position), 0);
	elm->LoadVertexArray(GLfloatArray(normal.size() / 3, 3, normal), 1);
	elm->LoadVertexArray(GLfloatArray(texcoords.size() / 2, 2, texcoords), 2);
	elm->LoadIndex(index);

	// texture
	if (mesh->mMaterialIndex >= 0) {
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
		string directory = this->m_assipPath.substr(0, m_assipPath.find_last_of('/'));
		int texTotal = 0;
		for (GLuint i = 0; i < material->GetTextureCount(aiTextureType_DIFFUSE); i++) {
			aiString str;
			material->GetTexture(aiTextureType_DIFFUSE, i, &str);
			char uniname[30];
			snprintf(uniname, 30, "texDiffuse%d", i);
			GLint loc = GLProg::GetFromID(m_progid)->GetUniformLocation(uniname);
			elm->LoadTexture(directory + "/" + str.C_Str(), texTotal++, loc);
		}
		for (GLuint i = 0; i < material->GetTextureCount(aiTextureType_SPECULAR); i++) {
			aiString str;
			material->GetTexture(aiTextureType_SPECULAR, i, &str);
			char uniname[30];
			snprintf(uniname, 30, "texSpecular%d", i);
			GLint loc = GLProg::GetFromID(m_progid)->GetUniformLocation(uniname);
			elm->LoadTexture(directory + "/" + str.C_Str(), texTotal++, loc);
		}
	}

	elm->InitGLResources();

	return elm;
}
