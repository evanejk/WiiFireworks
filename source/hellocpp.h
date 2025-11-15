    #ifdef __cplusplus
    extern "C" {
    #endif
    void drawShots();
    void shoot(long long currentTime,float fromX,float fromY,float fromZ,float targetX,float targetY,float targetZ);
    int loadShots(long long currentTime, int timePassed);
    void beamBlocks(float fromX, float fromY, float fromZ,  long long timeLongLong);//
    void drawBeams();
    void drawBeamsSpecial();
    int loadBeams(int timePassed);
    int loadBeamsSpecial(int timePassed);
    char *get_static_string();
    void loadYstars(int level);
    void drawY_Stars();
    void moveStarsBeamsSpecialEtc(float moveX, float moveY, float moveZ);
    void loopAroundOutOfBoundsWorldObjects();
    double calculateDistance(float x1, float y1, float z1, float x2, float y2, float z2);
    void drawSky(float x, float y, float z);
    int moveYStars(long long timeLongLong, int timePassed);
    #ifdef __cplusplus
    }
    #endif
