#ifndef GlobalStorageH
#define GlobalStorageH

#include "Utils.h"
#include "Bitmap.h"
#include "Animation.h"

class Storage{
private:
    map<String, Animation*> mapAnimation;
	map<String, Bitmap*> mapBitmap;
	map<String, double> mapDouble;
	map<String, Vec2i> mapVec2i;
	map<String, Range> mapRange;

//---------------------------------------------------------------------------
public:
	Storage() {};
	~Storage() {
		for(auto anim : mapAnimation)
			delete anim.second;

		for(auto bmp : mapBitmap)
			delete bmp.second;
	};

	void AddBitmap(String Name, Bitmap* X) { mapBitmap[Name] = X; };
	void AddDouble(String Name, double X) { mapDouble[Name] = X; };
	void AddVec2i(String Name, Vec2i X) { mapVec2i[Name] = X; };
	void AddRange(String Name, Range X) { mapRange[Name] = X; };
	void AddAnimation(String Name, Animation* X) { mapAnimation[Name] = X; };

	Bitmap* GetBitmap(String Name) {
		if(mapBitmap.find(Name) == mapBitmap.end())
			throw Name + " not found in BITMAPs";
		return mapBitmap[Name];
	};
	double GetDouble(String Name) {
		if(mapDouble.find(Name) == mapDouble.end())
			throw Name + " not found in DOUBLEs";
		return mapDouble[Name];
	};
	Vec2i GetVec2i(String Name) {
		if(mapVec2i.find(Name) == mapVec2i.end())
			throw Name + " not found in VECTORs";
		return mapVec2i[Name];
	};
	Range GetRange(String Name) {
		if(mapRange.find(Name) == mapRange.end())
			throw Name + " not found in RANGEs";
		return mapRange[Name];
	};
	Animation* GetAnimation(String Name) {
		if(mapAnimation.find(Name) == mapAnimation.end())
			throw Name + " not found in ANIMATIONs";
        return mapAnimation[Name];
	};
};

//---------------------------------------------------------------------------
#endif
