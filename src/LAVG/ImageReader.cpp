#include "ImageReader.h"

GLFWimage * ImageReader::readGLFWimage(const char * filePath)
{
	// Read the file
	cv::Mat src = cv::imread(filePath, cv::IMREAD_UNCHANGED);
	if (src.data == NULL)
		return NULL;

	cv::Mat image(src.rows, src.cols, CV_8UC4);
	cv::cvtColor(src, image, cv::COLOR_BGR2RGBA);
	const int w = image.cols;
	const int h = image.rows;

	size_t size = w * h * 4;
	unsigned char *pixels = new unsigned char[size];
	std::memcpy(pixels, image.data, size);

	GLFWimage *GLFWImage = new GLFWimage;
	GLFWImage->width = w;
	GLFWImage->height = h;
	GLFWImage->pixels = pixels;
	return GLFWImage;
}

bool ImageReader::LoadTextureFromImage(const char * filePath, GLuint& texId , bool SaveToCache /*= false*/)
{
	cv::Mat src = cv::imread(filePath, cv::IMREAD_UNCHANGED);
	if (src.data == NULL)
		return false;

	//cv::Mat image(src.rows, src.cols, CV_8UC4);
	cv::cvtColor(src, src, cv::COLOR_BGRA2RGBA);
	
	float Scale = SystemSetting::GetInstance() ->getScale();
	if (Scale < 1.f)
		cv::resize(src, src, cv::Size(), Scale, Scale, cv::INTER_AREA);
	else
		cv::resize(src, src, cv::Size(), Scale, Scale, cv::INTER_CUBIC);

	glGenTextures(1, &texId);
	if (texId == 0 )
		return false;

	GLuint lastTexId = 0;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, (GLint *)&lastTexId);
	glBindTexture(GL_TEXTURE_2D, texId);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, src.cols, src.rows, 0, GL_RGBA, GL_UNSIGNED_BYTE, src.data);
	glBindTexture(GL_TEXTURE_2D, lastTexId);

	if (SaveToCache)
		ImageRender::GetInstance()->AddTextureToCache(texId, filePath);

	return true;
}

bool ImageReader::ReloadTextureImage(const char* filePath, GLuint& texId)
{
	GLuint lastTexId = 0;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, (GLint *)&lastTexId);
	glDeleteTextures(1, &texId);
	bool suc = LoadTextureFromImage(filePath, texId, false);
	glBindTexture(GL_TEXTURE_2D, lastTexId);
	return suc;
}
