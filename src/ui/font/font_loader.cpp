#include "font_loader.hpp"

#include <filesystem>
#include <iostream>
#include <sstream>

#include "../../../thirdparty/glad/include/glad/glad.h"
#include "../../../thirdparty/tinyxml/tinyxml2.h"
#include "../../../thirdparty/stb/stb_image.h"
#include "../../textures/texture.hpp"

using namespace tinyxml2;
using std::istringstream;


FontLoader::FontLoader(ResourceManager<Texture>& txLoader): m_txLoader(txLoader) {}

/// @brief Loads a font atlas resource
/// @param path Path of the font to load
/// @return Shared pointer to loaded font
std::shared_ptr<FontAtlas> FontLoader::load_resource(std::string path) const
{
    std::cout << "[FONT LOADER] Loading atlas at: " << path << std::endl;
    std::string metaPath = path;
    metaPath.append(".xml");

    XMLDocument metaFile;
    XMLError status = metaFile.LoadFile(metaPath.c_str()); 
    if (status != XMLError::XML_SUCCESS)
        return nullptr;

    std::cout << "[FONT LOADER] Loaded meta file: " << metaPath << std::endl;

    XMLElement* rootElem = metaFile.FirstChildElement("meta");
    XMLElement* glyphSizeElem = rootElem->FirstChildElement("atlas_size");
    XMLElement* startCharElem = rootElem->FirstChildElement("start_char");

    unsigned int cols = glyphSizeElem->FirstChildElement("columns")->UnsignedText();
    unsigned int rows = glyphSizeElem->FirstChildElement("rows")->UnsignedText();
    unsigned int startChar = startCharElem->UnsignedText();

    std::cout << "[FONT LOADER] cols: " << cols << " rows: " << rows << " startChar: " << startChar << std::endl;
    if (cols == 0 || rows == 0 || startChar == 0)
        return nullptr;

    std::shared_ptr<Texture> atlasTx = m_txLoader.load_resource(path);
    FontAtlas* fontAtlas = new FontAtlas(atlasTx, startChar, cols, rows);

    std::cout << "[FONT LOADER] Created font atlas with tx id: " << atlasTx.get()->get_ID() << std::endl;
    return std::shared_ptr<FontAtlas>(fontAtlas);
}