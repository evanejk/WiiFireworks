//#ifndef SHOT_H
//#define SHOT_H

#include "Block.h"

class Shot : public Block
{
    public:
        float xAcc;
        float yAcc;
        float zAcc;
        long long endTime;
        Shot(float fromX, float fromY, float fromZ, float xAcc, float yAcc, float zAcc, long long endTime);

    protected:

    private:
};

//#endif // SHOT_H
