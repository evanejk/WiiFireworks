#ifndef Y_STAR_H
#define Y_STAR_H

#include "Block.h"


class Y_Star : public Block{
    public:
        Y_Star(int level, float fromX, float fromY, float fromZ, float xAcc, float yAcc, float zAcc);
        float blockSize;
        void changeDirection(float playerX, float playerY, float playerZ);
        void normalizeAcc();
        float xAcc;
        float yAcc;
        float zAcc;
        int level;
        long long reloadedTime;
    protected:

    private:

};

#endif // Y_STAR_H
