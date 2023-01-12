/*
JUEGO DE TANQUES SACADO DE YOUTUBE AVANZADO..???
*/

#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <windows.h>
#include <cmath>
#include<time.h> // clock

#define Width 605  //ancho
#define Height 155 //alto
#define dW 5 // corresponde la division de ancho
#define dH 5 // corresponde la division del largo
#define PI 3.14159265

using namespace std;



//Funciones mas matematicas y correctas para determinar rotacion, mov y hitbox

void gotoxy(int x, int y)
{
    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD dwPos;
    dwPos.X = x;
    dwPos.Y = y;
    SetConsoleCursorPosition(hCon, dwPos);
}

void drawf(char plano[Height/dH][Width/dW+1], int A, int B, char c)
{
    B=Height/dH-1-B;
    if(A<0 || B<0 || A>=Width/dW || B>=Height/dH) return;
    plano[B][A]=c;
}

struct coord{ //guardo xy para luego usarla en la class
    int x;
    int y;
};

class Vehiculo{
    private:
        coord XY = {10,10};
        coord xy;
        coord sxy=XY;
        int s,L=0,D=0;
        int S;
        float vy = 0.0f, vx=0.0f;
        bool Mod=false;
        bool shoot=false;

    public:

        void personaje(char plano[Height/dH][Width/dW+1])
        {
            drawf(plano, XY.x, XY.y,'*'); //centro atras de la cabeza
            drawf(plano, XY.x + xy.x, XY.y + xy.y,'*'); //cabeza del tanque

            drawf(plano, XY.x + xy.y, XY.y + xy.x,'*'); // hombro izquierdo
            drawf(plano, XY.x - xy.y, XY.y - xy.x,'*'); // hombro der

            drawf(plano, XY.x + xy.y - xy.x, XY.y + xy.x - xy.y,'*');// punta izq atras
            drawf(plano, XY.x - xy.y - xy.x, XY.y - xy.x - xy.y,'*'); //punta derecha atras
        }

        void rotar(int p)
        {
            xy.x=cos((p*PI)/180);
            xy.y=sin((p*PI)/180);
        }


        void mov(int m)
        {
            if(vy<=400.0f) //velocidad de tanque lento < rapido
            {
                vy=800.0f;
                S=m;
            }
        }

        void move(int m)
        {
            switch(m)
            {
            case 0:
                XY.x++;
                rotar(S);
                break;
            case 90:
                XY.y++;
                rotar(S);
                break;
            case 180:
                XY.x--;
                rotar(S);
                break;
            case 270:
                XY.y--;
                rotar(S);
                break;
            }
        }

        void update()
        {
            vy-=1150.0f*0.0416;
            if(shoot) vx-=1150.0f*0.0416;
            if(vy>=700.0f) Mod=true;
            else Mod=false;
        }

        bool checker(coord C) //limites
        {
            if(C.x < 2 || C.y < 2 || C.x >= Width/dW-2 || C.y >= Height/dH-2) return true;
            return false;
        }

        void displacement(char plano[Height/dH][Width/dW+1])
        {
            personaje(plano);
            if(Mod)
            {
                coord C=XY; //guardo poss previa
                move(S);
                if(checker(XY)) //checkeo si esta dentro del limite
                {
                    XY=C; //asigno poss previa para volver atras y no pasar el limite
                    Mod=false;
                }
                personaje(plano);
                Mod=false;
            }
        }

        void bala(char plano[Height/dH][Width/dW+1], char d)
        {
            drawf(plano, sxy.x, sxy.y, d);
        }

        void shots()
        {
            if(!shoot)
            {
                shoot=true;
                vx=800.0f;
                sxy=XY;
                L=30; //Largo del tiro
            }
        }

        void direction(int p, coord &d)
        {
            switch(p)
            {
            case 0:
                d.x++;
                break;
            case 90:
                d.y++;
                break;
            case 180:
                d.x--;
                break;
            case 270:
                d.y--;
                break;
            }
        }

        void disparo(char plano[Height/dH][Width/dW+1])
        {
            bala(plano,'7');
            {
                if(vx<750)
                {
                    coord C=sxy;
                    direction(s,sxy);
                    if(checker(sxy))
                    {
                        sxy=C;
                        shoot=false;
                        D=vx=0;
                    }
                    vx=800;
                    D++;
                }
                if(D>=L)
                {
                    shoot=false;
                    D=vx=0;
                }
            }
        }

};


main()
{
    char plano[Height/dH][Width/dW+1];

    for(int i=0;i<Height/dH;i++)
    {
        plano[i][Width/dW] = '\n';
    }
    plano[Height/dH+1][Width/dW] = '\v';


    float fps=24.0f;
    float dt=0.01/fps;
    float acumulador = 0.0f;

    clock_t start = clock(); //tiempo de ej de programa

    Vehiculo f1;

    while(true) //Ciclo del juego
    {
        clock_t end = clock();
        acumulador += float(end-start)/CLOCKS_PER_SEC;
        start = end;

        if(acumulador <= 0.02f) acumulador = 2.0f;

        while(acumulador>=dt)
        {
            if(GetKeyState(VK_UP)&0x8000)f1.mov(90);
            if(GetKeyState(VK_RIGHT)&0x8000)f1.mov(0);
            if(GetKeyState(VK_LEFT)&0x8000)f1.mov(180);
            if(GetKeyState(VK_DOWN)&0x8000)f1.mov(270);

            f1.update();
            acumulador -= dt;
        }

        for (int i=0;i<Height/dH;i++)
        {
            for (int j=0;j<Width/dW;j++)
            {
                plano[i][j]= ' ';
            }
        }

        if(GetKeyState(115)&0x8000)f1.disparo(plano);
        f1.displacement(plano);


        gotoxy(0,0);
        puts(plano[0]);
    }

    return 0;
}




