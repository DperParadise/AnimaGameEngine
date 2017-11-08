#ifndef __IMPORTER_H__
#define __IMPORTER_H__

#include "libraries/assimp/include/assimp/Importer.hpp"

class Importer {

public:
	Importer();
	~Importer();

	Assimp::Importer *GetInstance();
	
private:
	static std::auto_ptr<Assimp::Importer> instance;
	
};


#endif