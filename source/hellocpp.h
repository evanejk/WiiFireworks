    #ifdef __cplusplus
    extern "C" {
    #endif
    int addNumbers(int a, int b);
    void beamBlocks(float fromX, float fromY, float fromZ, int timePassed);
    void special(int timePassed);
    void drawBeams();
    void drawBeamsSpecial();
    int loadBeams(int timePassed);
    int loadBeamsSpecial(int timePassed);
    char *get_static_string();
    void loadYstars();
    void drawY_Stars();
    void moveStarsBeamsSpecial(float moveX, float moveY, float moveZ);
    #ifdef __cplusplus
    }
    #endif
