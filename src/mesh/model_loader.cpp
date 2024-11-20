#include "model_loader.hpp"

#include <iostream>

#include "../../thirdparty/assimp/include/assimp/postprocess.h"
#include "../../thirdparty/assimp/include/assimp/scene.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "../../thirdparty/glm/gtx/string_cast.hpp"


void process_model_scene(const aiNode* node, const aiScene* scene, Model& model);
glm::mat4 ai_matrix44_glm(const aiMatrix4x4& from);
glm::vec3 ai_vector3_glm(const aiVector3D& from);
Mesh* mesh_from_node(const aiNode* node, const aiScene* scene, Model& model);

std::shared_ptr<Model> ModelLoader::load_resource(std::string path) const
{
    Assimp::Importer m_importer;
    const aiScene* scene = m_importer.ReadFile(path, 
        aiProcess_Triangulate            |
        aiProcess_JoinIdenticalVertices  |
        aiProcess_SortByPType
    );


    if (scene == nullptr)
    {
        std::cout << "[MODEL LOADER] Couldn't load model: " << path << std::endl;
        std::cout << "[MODEL LOADER] " << m_importer.GetErrorString() << std::endl;
        return nullptr;
    }

    Model* model = new Model();
    const aiNode* rootNode = scene->mRootNode;
    process_model_scene(rootNode, scene, *model);
    std::cout << "[MODEL LOADER] Loaded model at: " << path << std::endl;
    return std::shared_ptr<Model>(model);
}

void process_model_scene(const aiNode* node, const aiScene* scene, Model& model)
{
    std::unique_ptr<Mesh> mesh = std::unique_ptr<Mesh>(mesh_from_node(node, scene, model));
    model.add_mesh(std::move(mesh));
    for (unsigned int m = 0; m < node->mNumChildren; m++)
        process_model_scene(node->mChildren[m], scene, model);
}

// thanks stackoverflow
glm::mat4 ai_matrix44_glm(const aiMatrix4x4& from)
{
    glm::mat4 to;
    to[0][0] = (GLfloat)from.a1; to[0][1] = (GLfloat)from.b1;  to[0][2] = (GLfloat)from.c1; to[0][3] = (GLfloat)from.d1;
    to[1][0] = (GLfloat)from.a2; to[1][1] = (GLfloat)from.b2;  to[1][2] = (GLfloat)from.c2; to[1][3] = (GLfloat)from.d2;
    to[2][0] = (GLfloat)from.a3; to[2][1] = (GLfloat)from.b3;  to[2][2] = (GLfloat)from.c3; to[2][3] = (GLfloat)from.d3;
    to[3][0] = (GLfloat)from.a4; to[3][1] = (GLfloat)from.b4;  to[3][2] = (GLfloat)from.c4; to[3][3] = (GLfloat)from.d4;

    return to;
}

glm::vec3 ai_vector3_glm(const aiVector3D& from)
{
    return {from.x, from.y, from.z};
}

Mesh* mesh_from_node(const aiNode* node, const aiScene* scene, Model& model)
{
    std::cout << "[MODEL LOADER] " << node->mName.C_Str() << std::endl;
    std::cout << glm::to_string(ai_matrix44_glm(node->mTransformation)) << std::endl;
    Geometry geo;
    if (node->mNumMeshes > 0)
    {
        // build geometry
        unsigned int meshIndex = node->mMeshes[0];
        const aiMesh* mesh = scene->mMeshes[meshIndex];
        const aiVector3D* verts = mesh->mVertices;
        const aiVector3D* norms = mesh->mNormals;
        const aiVector3D* uvs = mesh->mTextureCoords[0];

        for(unsigned int v = 0; v < mesh->mNumVertices; ++v)
        {
            glm::vec3 p = ai_vector3_glm(verts[v]);
            glm::vec3 n = ai_vector3_glm(norms[v]);
            glm::vec2 u = ai_vector3_glm(uvs[v]);

            geo.vertices.push_back({p, n, u});
        }

        const aiFace* faces = mesh->mFaces;
        for (unsigned int f = 0; f < mesh->mNumFaces; ++f)
        {
            geo.indices.push_back(faces[f].mIndices[0]);
            geo.indices.push_back(faces[f].mIndices[1]);
            geo.indices.push_back(faces[f].mIndices[2]);
        }

        std::cout << "[MODEL LOADER] Loading non empty mesh" << std::endl;
        return new Mesh(geo, ai_matrix44_glm(node->mTransformation));
    }
    
    std::cout << "[MODEL LOADER] Loading empty mesh" << std::endl;
    return new Mesh(ai_matrix44_glm(node->mTransformation));
}