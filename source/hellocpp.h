    #ifdef __cplusplus
    extern "C" {
    #endif
    void drawShots();
    void shoot(long long currentTime,float fromX,float fromY,float fromZ,float targetX,float targetY,float targetZ);
    int loadShots(long long currentTime, int timePassed);
    int addNumbers(int a, int b);
    void beamBlocks(float fromX, float fromY, float fromZ,  long long timeLongLong);//
    //void special(int timePassed);
    void drawBeams();
    void drawBeamsSpecial();
    int loadBeams(int timePassed);
    int loadBeamsSpecial(int timePassed);
    char *get_static_string();
    void loadYstars();
    void drawY_Stars();
    void moveStarsBeamsSpecialEtc(float moveX, float moveY, float moveZ);
    void loopAroundOutOfBoundsWorldObjects();
    #ifdef __cplusplus
    }
    #endif
