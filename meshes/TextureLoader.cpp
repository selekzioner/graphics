#include "TextureLoader.h"

TLResult TextureLoader::Load(const std::string& path, 
	std::shared_ptr<QOpenGLTexture>& texture, const QOpenGLTexture::Filter filter)
{
	const auto textureImage = QImage(path.c_str());

	if (!textureImage.isNull()) {
		texture.reset(new QOpenGLTexture(textureImage));
		texture->setWrapMode(QOpenGLTexture::MirroredRepeat);
		texture->setMinificationFilter(filter);
		return TLResult::tlOK;
	}
	return TLResult::tlFAIL;
}
