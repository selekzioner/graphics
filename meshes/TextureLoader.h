#pragma once

#include <QOpenGLTexture>

enum class TLResult { tlOK = 1, tlFAIL = 0 };

class TextureLoader final {
public:
	static TLResult Load(const std::string& path, std::shared_ptr<QOpenGLTexture>& texture, QOpenGLTexture::Filter filter);
};

