#include "texture_loader.hpp"

#include <filesystem>
#include <iostream>

#include "../../thirdparty/glad/include/glad/glad.h"
#include "../../thirdparty/tinyxml/tinyxml2.h"
#include "../../thirdparty/stb/stb_image.h"

using namespace tinyxml2;

GLenum texture_type_from_str(std::string str);
GLenum texture_wrap_from_str(std::string str);
GLenum texture_filt_from_str(std::string str);
GLenum texture_format_from_str(std::string str);
std::vector<std::string> get_cubemap_files(const std::string& base);
Texture* load_2d_text(const std::string& path, GLuint id, GLint int_format, GLenum format, GLenum data_type, bool hasMipMap);
Texture* load_cubemap(const std::vector<std::string>& path, GLuint id, GLint int_format, GLenum format, GLenum data_type);

/// @brief Loads the texture in a path.
/// @param path Path of the texture
/// @return Shared pointer to the loaded texture
std::shared_ptr<Texture> TextureLoader::load_resource(std::string path) const
{
    std::string metaPath = path;
    metaPath.append(".xml");

    XMLDocument metaFile;
    XMLError status = metaFile.LoadFile(metaPath.c_str()); 
    if (status != XMLError::XML_SUCCESS)
        return nullptr;

    bool hasMipMap;
    bool isArray;
    XMLElement* rootElem = metaFile.FirstChildElement("meta");
    XMLElement* txTypeElem = rootElem->FirstChildElement("texture_type");
    XMLElement* txWrapElem = rootElem->FirstChildElement("wrapping");
    XMLElement* txFiltElem = rootElem->FirstChildElement("filtering");
    XMLElement* txFormElem = rootElem->FirstChildElement("format");
    XMLElement* minFiltElem = txFiltElem->FirstChildElement("min_filter");
    XMLElement* magFiltElem = txFiltElem->FirstChildElement("mag_filter");

    isArray = txTypeElem->BoolAttribute("array", false);
    hasMipMap = txFiltElem->BoolAttribute("mipmap", false);

    GLint int_format = texture_format_from_str(txFormElem->GetText());
    GLenum format = texture_format_from_str(txFormElem->GetText());
    GLenum data_type = GL_UNSIGNED_BYTE;

    GLuint id;
    GLenum txTypeEnum = texture_type_from_str(txTypeElem->GetText());
    glGenTextures(1, &id);
    glBindTexture(txTypeEnum, id);

    GLenum txWrapEnum = texture_wrap_from_str(txWrapElem->GetText());
    glTexParameteri(txTypeEnum, GL_TEXTURE_WRAP_S, txWrapEnum);
    glTexParameteri(txTypeEnum, GL_TEXTURE_WRAP_T, txWrapEnum);
    
    GLenum minFiltEnum = texture_filt_from_str(minFiltElem->GetText());
    GLenum magFiltEnum = texture_filt_from_str(magFiltElem->GetText());
    glTexParameteri(txTypeEnum, GL_TEXTURE_MIN_FILTER, minFiltEnum);
    glTexParameteri(txTypeEnum, GL_TEXTURE_MAG_FILTER, magFiltEnum);

    Texture* tx;
    switch (txTypeEnum)
    {
    case GL_TEXTURE_2D:
        tx = load_2d_text(path, id, int_format, format, data_type, hasMipMap);
        break;
    case GL_TEXTURE_CUBE_MAP:
        tx = load_cubemap(get_cubemap_files(path), id, int_format, format, data_type);
        /* code */
    default:
        break;
    }
    
    glBindTexture(txTypeEnum, 0);
    return std::shared_ptr<Texture>(tx);
}

/// @brief Retrieves GL texture type from a str
/// @param str str to convert
/// @return GL texture type
GLenum texture_type_from_str(std::string str)
{
    if (str == "GL_TEXTURE_2D")
        return GL_TEXTURE_2D;
    
    if (str == "GL_TEXTURE_CUBE_MAP")
        return GL_TEXTURE_CUBE_MAP;
    
    return GL_INVALID_ENUM;
}

/// @brief Retrieves GL texture wrap from a str
/// @param str str to convert
/// @return GL texture wrap
GLenum texture_wrap_from_str(std::string str)
{
    if (str == "GL_REPEAT")
        return GL_REPEAT;
    
    if (str == "GL_MIRRORED_REPEAT")
        return GL_MIRRORED_REPEAT;

    if (str == "GL_CLAMP_TO_EDGE")
        return GL_CLAMP_TO_EDGE;

    if (str == "GL_CLAMP_TO_BORDER")
        return GL_CLAMP_TO_BORDER;
    
    return GL_INVALID_ENUM;
}

/// @brief Retrieves GL texture filterig from a str
/// @param str str to convert
/// @return GL texture filterig
GLenum texture_filt_from_str(std::string str)
{
    if (str == "GL_NEAREST")
        return GL_NEAREST ;
    
    if (str == "GL_LINEAR")
        return GL_LINEAR ;

    if (str == "GL_NEAREST_MIPMAP_NEAREST")
        return GL_NEAREST_MIPMAP_NEAREST ;

    if (str == "GL_LINEAR_MIPMAP_NEAREST")
        return GL_LINEAR_MIPMAP_NEAREST ;

    if (str == "GL_NEAREST_MIPMAP_LINEAR")
        return GL_NEAREST_MIPMAP_LINEAR ;

    if (str == "GL_LINEAR_MIPMAP_LINEAR")
        return GL_LINEAR_MIPMAP_LINEAR ;
    
    return GL_INVALID_ENUM;
}

/// @brief Retrieves GL texture format from a str
/// @param str str to convert
/// @return GL texture format
GLenum texture_format_from_str(std::string str)
{
    if (str == "GL_RGB")
        return GL_RGB;
    
    if (str == "GL_RGBA")
        return GL_RGBA;
    
    return GL_INVALID_ENUM;
}

/// @brief Retrieves all of the files that compose a cubemap
/// @param path Base path of the cubemap
/// @return vector containing all the paths to all the cubemap files in the
//          following order: front, back, down, up, right, left
std::vector<std::string> get_cubemap_files(const std::string& path)
{
    std::filesystem::path basePath(path);
    std::string extension = basePath.extension();
    std::string baseStr = basePath.replace_extension("").c_str();

    std::string down(baseStr);
    std::string up(baseStr);
    std::string left(baseStr);
    std::string right(baseStr);
    std::string front(baseStr);
    std::string back(baseStr);

    return {
        front.append("_front").append(extension),
        back.append("_back").append(extension),
        down.append("_down").append(extension), 
        up.append("_up").append(extension), 
        right.append("_right").append(extension),
        left.append("_left").append(extension)
        };
}

/// @brief Loads a 2d texture from a path
/// @param path Filepath of the texture
/// @param int_format Internal format of the texture
/// @param format Format of the texture
/// @param data_type Type of data in the texture
Texture* load_2d_text(const std::string& path, GLuint id, GLint int_format, GLenum format, GLenum data_type, bool hasMipmap)
{
    std::vector<TextureMeta> file_metas{{}};
    unsigned char *data = stbi_load(path.c_str(), &file_metas.front().width, &file_metas.front().height, &file_metas.front().nrChannels, 0); 
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, int_format, file_metas.front().width, file_metas.front().height, 0, format, data_type, data);
        if (hasMipmap)
            glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture in path: " << path << std::endl;
        return nullptr;
    }
    
    file_metas.front().int_format = int_format;
    file_metas.front().data_type = data_type;
    file_metas.front().format = format;
    stbi_image_free(data);
    return new Texture(id, GL_TEXTURE_2D, file_metas);
}

/// @brief Loads a cubemap texture from a path
/// @param path Filepath of the textures composing the cubeap
/// @param int_format Internal format of the texture
/// @param format Format of the texture
/// @param data_type Type of data in the texture
Texture* load_cubemap(const std::vector<std::string>& path, GLuint id, GLint int_format, GLenum format, GLenum data_type)
{
    std::vector<TextureMeta> file_metas{{}};
    if (path.size() != 6)
        std::cout << "Wrong texture amount for a cubemap. Expected 6 textures." << std::endl;
        
    file_metas.assign(6, {.int_format=(int_format), .format=(format), .data_type=(data_type)});
    for (unsigned int i = 0; i < 6; i++)
    {
        unsigned char *data = stbi_load(path[i].c_str(), &file_metas[i].width, &file_metas[i].height, &file_metas[i].nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, file_metas[i].width, file_metas[i].height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << path[i] << std::endl;
            stbi_image_free(data);
            return nullptr;
        }
    }

    return new Texture(id, GL_TEXTURE_CUBE_MAP, file_metas);
}