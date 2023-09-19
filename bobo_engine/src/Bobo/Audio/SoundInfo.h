#include "bobopch.h"

namespace Bobo {
    struct SoundInfo {
    public:
        SoundInfo(const char* filePath = "", bool isLoop = false, bool is3D = false, float x = 0.0f, float y = 0.0f, float z = 0.0f)
            : filePath(filePath), isLoop(isLoop), is3D(is3D), x(x), y(y), z(z)
        {
            uniqueID = filePath;
        }

        std::string uniqueID;

        const char* filePath;

        bool isLoop;
        bool is3D;
        bool isLoaded;

        float x, y, z;

        // convienience method to set the 3D coordinates of the sound.
        void set3DCoords(float x, float y, float z) {
            this->x = x, this->y = y, this->z = z;
        }
    };
}