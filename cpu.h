struct CPU {
    bool in_use;
    int IP;
    char IR[1000];
    int quanta;
    }cpu;
int run(int quanta);
