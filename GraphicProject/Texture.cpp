#include "Texture.h"
#include "lodepng.h"


Texture::Texture()
{
}


Texture::~Texture()
{
}

int ReadLine(FILE *fp, int size, char *buffer)
{
	int i;
	for (i = 0; i < size; i++)
	{
		buffer[i] = fgetc(fp);
		if (feof(fp) || buffer[i] == '\n' || buffer[i] == '\r')
		{
			buffer[i] = '\0';
			return i + 1;
		}
	}
	return i;
}

void Texture::SetName(char const *newName)
{
	if (name) delete[] name;
	if (newName)
	{
		int n = strlen(newName);
		name = new char[n + 1];
		for (int i = 0; i < n; i++) name[i] = newName[i];
		name[n] = '\0';
	}
	else
	{
		name = nullptr;
	}
}

bool LoadPPM(FILE *fp, int &width, int &height, std::vector<Color24> &data)
{
	const int bufferSize = 1024;
	char buffer[bufferSize];
	ReadLine(fp, bufferSize, buffer);
	if (buffer[0] != 'P' && buffer[1] != '6') return false;

	ReadLine(fp, bufferSize, buffer);
	while (buffer[0] == '#') ReadLine(fp, bufferSize, buffer);	// skip comments

	sscanf(buffer, "%d %d", &width, &height);

	ReadLine(fp, bufferSize, buffer);
	while (buffer[0] == '#') ReadLine(fp, bufferSize, buffer);	// skip comments

	// last read line should be "255\n"

	data.resize(width*height);
	fread(data.data(), sizeof(Color24), width*height, fp);

	return true;
}

bool Texture::Load()
{
	data.clear();
	width = 0;
	height = 0;
	char const *name = this->name ? this->name : "";
	if (name[0] == '\0') return false;

	int len = (int)strlen(name);
	if (len < 3) return false;

	bool success = false;

	char ext[3] = { (char)tolower(name[len - 3]), (char)tolower(name[len - 2]), (char)tolower(name[len - 1]) };

	if (strncmp(ext, "png", 3) == 0)
	{
		std::vector<unsigned char> d;
		unsigned int w, h;
		unsigned int error = lodepng::decode(d, w, h, name, LCT_RGB);
		if (error == 0)
		{
			width = w;
			height = h;
			data.resize(width*height);
			memcpy(data.data(), d.data(), width*height * 3);
		}
		success = (error == 0);
	}
	else if (strncmp(ext, "ppm", 3) == 0)
	{
		FILE *fp = fopen(name, "rb");
		if (!fp) return false;
		success = LoadPPM(fp, width, height, data);
		fclose(fp);
	}

	return success;
}

void Texture::Organize()
{
	cleaneddata.clear();
	cleaneddata.resize(width*height);


	for(int i = 0 ; i < width*height; i++)
	{
		cleaneddata[i].r = data[i].ToColor().r;
		cleaneddata[i].g = data[i].ToColor().g;
		cleaneddata[i].b = data[i].ToColor().b;
	}
}

