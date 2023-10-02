/*************************************************************************/
/*                                                                       */
/*                                                                       */
/****************************CHABOTS ROBOTNIK 2021************************/
/*                                                                       */
/*TSSP V 1.0    												         */
/*S.L.P MEXICO                           TIME:14:28:48 PM DATE:06/02/2020*/
/*************************************************************************/

#ifndef TSSP_h
#define TSSP_h

#define NUM_TSSP 16

class TSSP{
public:
    TSSP();
    void sensorInfo();
    int angleIR();
    int hipIR();
    int intIR();
private:
    long timeLimit=833;
    int count=0;
    unsigned long time_ms = 0;
    int lectura[NUM_TSSP];
    int tssp[NUM_TSSP]={2,3,4,5,6,7,8,9,10,11,12,13,14,15,18,19};
    double vecx[NUM_TSSP]={0.9238, 0.7071, 0.3826, 0, -0.3826, -0.7071, -0.9238,-1, -0.9238, -0.7071, -0.3826, 0, 0.3826, 0.7071, 0.9238, 1};
    double vecy[NUM_TSSP]={0.3826, 0.7071, 0.9238, 1, 0.9238, 0.7071, 0.3826, 0, -0.3826, -0.7071, -0.9238, -1, -0.9238, -0.7071, -0.3826, 0};
    float delta=1.0;
    double sum_x;
    double sum_y;
};

#endif