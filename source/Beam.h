#include "Block.h"


class Beam :public Block {
    public:
        float xAcc;
        float yAcc;
        float zAcc;
        int timer;
        Beam(float fromX, float fromY, float fromZ, float xAcc, float yAcc, float zAcc);

    protected:

    private:
};
