#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winbgim.h>
#include <graphics.h>

#define MAX1 40
#define MAX2 100
#define FUNDAL BLACK

using namespace std;

float zoom=8.0;
float zoomMen=10.0;
unsigned nrPieseMeniu=20;

struct descriere
{
    char comanda[MAX1];
    unsigned nComenzi;
    float x1[MAX1],y1[MAX1],x2[MAX1],y2[MAX1];
};

struct punctLegatura
{
    float x,y;
    bool ocupat=false;
    unsigned piesa;
};

struct piesa
{
    char nume[MAX1];
    int x,y;
    unsigned orientare;
    int dimensiune;
    unsigned nrLegaturi;
    punctLegatura pLeg[MAX1];
    descriere descr;
};

struct legatura
{
    punctLegatura pStart,pEnd;
    unsigned varianta=1;
    int dimensiune;
};

piesa Meniu[MAX1];

void initializeaza(piesa& P);
void deseneaza(piesa& P, int culoare, int z);
void myLine(piesa P, unsigned i,float z);
void myRectangle(piesa P, unsigned i, float z);
void myEllipse(piesa P, unsigned i, float z);
void myArc(piesa P, unsigned i, float z);
void roteste(float x, float y, float& xnou, float& ynou);
void amplaseaza(piesa& P, unsigned x, unsigned y, unsigned orient);
void meniu();
unsigned alegePiesa(piesa vectorPiese[MAX2], unsigned nrPiese);
void punePiesa(piesa& P);
void selectieMeniu(int &mX, int &mY);
void rotestePiesa(unsigned nrPiesaAleasa, piesa Piesa[MAX2], unsigned nrPiese, legatura Legatura[MAX2], unsigned nrLegaturi);
void deseneazaLegatura(piesa Piesa[MAX2], unsigned nrPiese, legatura leg, int culoare);
void mutaPiesa(int nrPiesaAleasa, piesa Piesa[MAX2], unsigned nrPiese, legatura Legatura[MAX2], unsigned nrLegaturi);
void redimPiesa(unsigned nrPiesaAleasa, piesa Piesa[MAX2], unsigned nrPiese, legatura Legatura[MAX2], unsigned nrLegaturi, int z);
void stergePiesa(unsigned nrPiesaAleasa, piesa Piesa[MAX2], unsigned& nrPiese, legatura Legatura[MAX2], unsigned& nrLegaturi);
bool verificaPunctLegatura(piesa Piesa[MAX2], unsigned nrPiese, int x, int y, legatura &leg);
void deseneazaLegaturaStart(piesa Piesa[MAX2], punctLegatura pStart, int xEnd, int yEnd, int culoare);
void refresh(piesa Piesa[MAX2], unsigned nrPiese, legatura Legatura[MAX2], unsigned nrLegaturi);
punctLegatura alegePunctLegatura(piesa Piesa[MAX2], unsigned nrPiese);
void puneLegatura(piesa Piesa[MAX2],unsigned nrPiese,legatura Legatura[MAX2],unsigned &nrLegaturi);
void initializareFisier(char numeFis[MAX1]);
void scrieInFisier(piesa Piesa[MAX2],legatura Legatura[MAX2], unsigned nrPiese,unsigned nrLegaturi, FILE *fisier);
void citesteDinFisier(piesa Piesa[MAX2],legatura Legatura[MAX2], unsigned &nrPiese,unsigned &nrLegaturi, FILE *fisier);
void buton(int x,int y,char *text);
void fundal();
void meniu_info();

void initializeaza(piesa& P)
{
    P.orientare=0;
    char numeFisier[256],s[MAX1];
    strcpy(numeFisier,P.nume);
    strcat(numeFisier,".txt");
    FILE* f=fopen(numeFisier,"r");
    if(f==NULL)
    {
        printf("Eroare la deschiderea fisierului! \n");
        exit(1);
    }
    fscanf(f,"%s",s);
    fscanf(f,"%d",&P.nrLegaturi);
    for(unsigned i=1; i<=P.nrLegaturi; i++)
    {
        fscanf(f,"%f%f",&P.pLeg[i].x,&P.pLeg[i].y);
    }
    fscanf(f,"%s",s);
    unsigned i=1;
    while(!feof(f))
    {
        fscanf(f,"%c%f%f",&P.descr.comanda[i],&P.descr.x1[i],&P.descr.y1[i]);
        fscanf(f,"%f%f\n",&P.descr.x2[i],&P.descr.y2[i]);
        i++;
    }
    P.descr.nComenzi=--i;
    fclose(f);
}

void deseneaza(piesa& P, int culoare, int z)
{
    setcolor(culoare);
    P.dimensiune=z;
    for(unsigned i=1; i<=P.descr.nComenzi; i++)
        switch(P.descr.comanda[i])
        {
        case 'L':
            myLine(P,i,z);
            break;
        case 'R':
            myRectangle(P,i,z);
            break;
        case 'O':
            myEllipse(P,i,z);
            break;
        case 'A':
            myArc(P,i,z);
            break;
        }
}

void roteste(float x, float y, float& xnou, float& ynou)
{

    xnou=y;
    ynou=-x;
}

void myLine(piesa P, unsigned i, float z)
{
    float x_1,y_1,x_2,y_2;
    float x1,y1,x2,y2;
    x1=P.descr.x1[i];
    y1=P.descr.y1[i];
    x2=P.descr.x2[i];
    y2=P.descr.y2[i];
    switch(P.orientare)
    {
    case 0:
        x_1=x1;
        y_1=y1;
        x_2=x2;
        y_2=y2;
        break;
    case 1:
        roteste(x1,y1,x_1,y_1);
        roteste(x2,y2,x_2,y_2);
        break;
    case 2:
        roteste(x1,y1,x_1,y_1);
        roteste(x_1,y_1,x1,y1);
        x_1=x1;
        y_1=y1;
        roteste(x2,y2,x_2,y_2);
        roteste(x_2,y_2,x2,y2);
        x_2=x2;
        y_2=y2;
        break;
    case 3:
        roteste(x1,y1,x_1,y_1);
        roteste(x_1,y_1,x1,y1);
        roteste(x1,y1,x_1,y_1);
        roteste(x2,y2,x_2,y_2);
        roteste(x_2,y_2,x2,y2);
        roteste(x2,y2,x_2,y_2);
        break;
    }
    line(P.x+z*x_1,P.y+z*y_1,P.x+z*x_2,P.y+z*y_2);
}

void myRectangle(piesa P, unsigned i, float z)
{
    float x_1,y_1,x_2,y_2;
    float x1,y1,x2,y2;
    x1=P.descr.x1[i];
    y1=P.descr.y1[i];
    x2=P.descr.x2[i];
    y2=P.descr.y2[i];
    switch(P.orientare)
    {
    case 0:
        x_1=x1;
        y_1=y1;
        x_2=x2;
        y_2=y2;
        break;
    case 1:
        roteste(x1,y1,x_1,y_1);
        roteste(x2,y2,x_2,y_2);
        break;
    case 2:
        roteste(x1,y1,x_1,y_1);
        roteste(x_1,y_1,x1,y1);
        x_1=x1;
        y_1=y1;
        roteste(x2,y2,x_2,y_2);
        roteste(x_2,y_2,x2,y2);
        x_2=x2;
        y_2=y2;
        break;
    case 3:
        roteste(x1,y1,x_1,y_1);
        roteste(x_1,y_1,x1,y1);
        roteste(x1,y1,x_1,y_1);
        roteste(x2,y2,x_2,y_2);
        roteste(x_2,y_2,x2,y2);
        roteste(x2,y2,x_2,y_2);
        break;
    }
    rectangle(P.x+z*x_1,P.y+z*y_1,P.x+z*x_2,P.y+z*y_2);
}

void myEllipse(piesa P, unsigned i, float z)
{
    float x_1,y_1,x_2,y_2;
    float x1,y1,x2,y2;
    x1=P.descr.x1[i];
    y1=P.descr.y1[i];
    x2=P.descr.x2[i];
    y2=P.descr.y2[i];
    switch(P.orientare)
    {
    case 0:
        x_1=x1;
        y_1=y1;
        x_2=x2;
        y_2=y2;
        break;
    case 1:
        roteste(x1,y1,x_1,y_1);
        x_2=y2;
        y_2=x2;
        break;
    case 2:
        roteste(x1,y1,x_1,y_1);
        roteste(x_1,y_1,x1,y1);
        x_1=x1;
        y_1=y1;
        x_2=x2;
        y_2=y2;
        break;
    case 3:
        roteste(x1,y1,x_1,y_1);
        roteste(x_1,y_1,x1,y1);
        roteste(x1,y1,x_1,y_1);
        x_2=y2;
        y_2=x2;
        break;
    }
    ellipse(P.x+x_1*z,P.y+y_1*z,0,360,x_2*z,y_2*z);
}

void myArc(piesa P, unsigned i, float z)
{
    float x1,y1,x2;
    float x_1,y_1;
    x1=P.descr.x1[i];
    y1=P.descr.y1[i];
    x2=P.descr.x2[i];
    switch(P.orientare)
    {
    case 0:
        arc(P.x+x1*z,P.y+y1*z,-90,90,x2*z);
        break;
    case 1:
        roteste(x1,y1,x_1,y_1);
        arc(P.x+x_1*z,P.y+y_1*z,0,180,x2*z);
        break;
    case 2:
        roteste(x1,y1,x_1,y_1);
        roteste(x_1,y_1,x1,y1);
        arc(P.x+x1*z,P.y+y1*z,90,270,x2*z);
        break;
    case 3:
        roteste(x1,y1,x_1,y_1);
        roteste(x_1,y_1,x1,y1);
        roteste(x1,y1,x_1,y_1);
        arc(P.x+x_1*z,P.y+y_1*z,-180,0,x2*z);
        break;
    }
}

void amplaseaza(piesa& P, unsigned x, unsigned y, unsigned orient)
{
    P.x=x;
    P.y=y;
    P.orientare=orient;
}

void meniu()
{
    setcolor(GREEN);
    rectangle(0,0,getmaxx(),getmaxy());
    line(120,0,120,getmaxy());
    line(0,90,getmaxx(),90);
    strcpy(Meniu[1].nume,"DIODA");
    strcpy(Meniu[2].nume,"ZENNER");
    strcpy(Meniu[3].nume,"TRANZNPN");
    strcpy(Meniu[4].nume,"TRANZPNP");
    strcpy(Meniu[5].nume,"CONDENS");
    strcpy(Meniu[6].nume,"REZIST");
    strcpy(Meniu[7].nume,"BATERIE");
    strcpy(Meniu[8].nume,"POLARIZ");
    strcpy(Meniu[9].nume,"SINU");
    strcpy(Meniu[10].nume,"SERVOMOT");
    strcpy(Meniu[11].nume,"AMPLOP");
    strcpy(Meniu[12].nume,"NOD");
    strcpy(Meniu[13].nume,"LINIE");
    strcpy(Meniu[14].nume,"MUTA");
    strcpy(Meniu[15].nume,"ROTESTE");
    strcpy(Meniu[16].nume,"PLUS");
    strcpy(Meniu[17].nume,"MINUS");
    strcpy(Meniu[18].nume,"STERGE");
    strcpy(Meniu[19].nume,"SALVEAZA");
    strcpy(Meniu[20].nume,"STOP");
    for(unsigned i=1; i<=nrPieseMeniu; i++)
    {
        initializeaza(Meniu[i]);
        if(i<=12)
        {
            amplaseaza(Meniu[i],60,i*60,0);
            deseneaza(Meniu[i],WHITE,zoomMen);
            setcolor(GREEN);
            line(0,i*60+30,120,i*60+30);
        }
        else if(i==13)
        {
            amplaseaza(Meniu[i],60,i*60-10,0);
            deseneaza(Meniu[i],WHITE,zoomMen);
            setcolor(WHITE);
            outtextxy(40,i*60-10,Meniu[i].nume);
            setcolor(GREEN);
            line(0,i*60+30,120,i*60+30);
        }

        else if(i==15 || i==19)
        {
            amplaseaza(Meniu[i],(i-13)*120+30,45,0);
            deseneaza(Meniu[i],WHITE,zoomMen);
            setcolor(GREEN);
            line((i-12)*120,0,(i-12)*120,90);
            setcolor(WHITE);
            outtextxy((i-13)*120+30,45,Meniu[i].nume);
            setcolor(GREEN);
        }
        else if(i==18)
        {
            amplaseaza(Meniu[i],(i-13)*120+35,45,0);
            deseneaza(Meniu[i],WHITE,zoomMen);
            setcolor(GREEN);
            line((i-12)*120,0,(i-12)*120,90);
            setcolor(WHITE);
            outtextxy((i-13)*120+35,45,Meniu[i].nume);
            setcolor(GREEN);
        }
        else if(i==20)
        {
            amplaseaza(Meniu[i],(i-13)*120+40,45,0);
            deseneaza(Meniu[i],WHITE,zoomMen);
            setcolor(GREEN);
            line((i-12)*120,0,(i-12)*120,90);
            setcolor(RED);
            outtextxy((i-13)*120+40,45,Meniu[i].nume);
            setcolor(GREEN);
        }
        else
        {
            amplaseaza(Meniu[i],(i-13)*120+40,45,0);
            deseneaza(Meniu[i],WHITE,zoomMen);
            setcolor(GREEN);
            line((i-12)*120,0,(i-12)*120,90);
            setcolor(WHITE);
            outtextxy((i-13)*120+40,45,Meniu[i].nume);
            setcolor(GREEN);
        }
    }
}

unsigned alegePiesa(piesa vectorPiese[MAX2], unsigned nrPiese)
{
    bool click=false;
    unsigned i,nrPiesaAleasa=0;
    int xMouse, yMouse;
    do
        if(ismouseclick(WM_LBUTTONDOWN) && !click)
        {
            clearmouseclick(WM_LBUTTONDOWN);
            xMouse=mousex();
            yMouse=mousey();
            for (i=1; i<=nrPiese; i++)
                if (abs(vectorPiese[i].x-xMouse)<=40 && abs(vectorPiese[i].y-yMouse)<=20)
                {
                    nrPiesaAleasa=i;
                    click=true;
                }
        }
    while(!click);
    return nrPiesaAleasa;
}

void punePiesa(piesa& P)
{
    bool click=false;
    int xMouse,yMouse;
    do
        if(ismouseclick(WM_LBUTTONDOWN) && !click)
        {
            clearmouseclick(WM_LBUTTONDOWN);
            xMouse=mousex();
            yMouse=mousey();
            if(xMouse>120 && xMouse<getmaxx()-30 && yMouse>90 && yMouse<getmaxy()-30)
            {
                amplaseaza(P,xMouse,yMouse,0);
                deseneaza(P,WHITE,zoom);
                click=true;
            }
        }
    while(!click);
}

void selectieMeniu(int &mX, int &mY)
{
    bool click=false;

    while(!click)
    {
        if(ismouseclick(WM_LBUTTONDOWN) && !click)
        {
            clearmouseclick(WM_LBUTTONDOWN);
            mX=mousex();
            mY=mousey();
            click=true;
        }
    }
}

void mutaPiesa(int nrPiesaAleasa, piesa Piesa[MAX2], unsigned nrPiese, legatura Legatura[MAX2], unsigned nrLegaturi)
{
    int xMouse,yMouse;
    while(!ismouseclick(WM_LBUTTONDOWN))
    {
        deseneaza(Piesa[nrPiesaAleasa],FUNDAL,Piesa[nrPiesaAleasa].dimensiune);
        for(unsigned i=1; i<=nrLegaturi; i++)
            if(Legatura[i].pStart.piesa==nrPiesaAleasa || Legatura[i].pEnd.piesa==nrPiesaAleasa)
                deseneazaLegatura(Piesa,nrPiese,Legatura[i],FUNDAL);
        if(ismouseclick(WM_MOUSEMOVE))
            getmouseclick(WM_MOUSEMOVE,xMouse,yMouse);
        amplaseaza(Piesa[nrPiesaAleasa],xMouse,yMouse,Piesa[nrPiesaAleasa].orientare);
        deseneaza(Piesa[nrPiesaAleasa],WHITE,Piesa[nrPiesaAleasa].dimensiune);
        for(int i=1; i<=nrLegaturi; i++)
            if(Legatura[i].pStart.piesa==nrPiesaAleasa || Legatura[i].pEnd.piesa==nrPiesaAleasa)
                deseneazaLegatura(Piesa,nrPiese,Legatura[i],WHITE);

    }
    refresh(Piesa,nrPiese,Legatura,nrLegaturi);
    clearmouseclick(WM_LBUTTONUP);
}

void rotestePiesa(unsigned nrPiesaAleasa, piesa Piesa[MAX2], unsigned nrPiese, legatura Legatura[MAX2], unsigned nrLegaturi)
{
    deseneaza(Piesa[nrPiesaAleasa],FUNDAL,Piesa[nrPiesaAleasa].dimensiune);
    for(int i=1; i<=nrLegaturi; i++)
        if(Legatura[i].pStart.piesa==nrPiesaAleasa || Legatura[i].pEnd.piesa==nrPiesaAleasa)
            deseneazaLegatura(Piesa,nrPiese,Legatura[i],FUNDAL);
    Piesa[nrPiesaAleasa].orientare=(Piesa[nrPiesaAleasa].orientare+1)%4;
    for(int i=1; i<=Piesa[nrPiesaAleasa].nrLegaturi; i++)
        roteste(Piesa[nrPiesaAleasa].pLeg[i].x,Piesa[nrPiesaAleasa].pLeg[i].y,Piesa[nrPiesaAleasa].pLeg[i].x,Piesa[nrPiesaAleasa].pLeg[i].y);
    deseneaza(Piesa[nrPiesaAleasa],WHITE,Piesa[nrPiesaAleasa].dimensiune);
    for(int i=1; i<=nrLegaturi; i++)
    {
        if(Legatura[i].pStart.piesa==nrPiesaAleasa)
        {
            roteste(Legatura[i].pStart.x,Legatura[i].pStart.y,Legatura[i].pStart.x,Legatura[i].pStart.y);
            deseneazaLegatura(Piesa,nrPiese,Legatura[i],WHITE);
        }
        if(Legatura[i].pEnd.piesa==nrPiesaAleasa)
        {
            roteste(Legatura[i].pEnd.x,Legatura[i].pEnd.y,Legatura[i].pEnd.x,Legatura[i].pEnd.y);
            deseneazaLegatura(Piesa,nrPiese,Legatura[i],WHITE);
        }
    }
    refresh(Piesa,nrPiese,Legatura,nrLegaturi);
}

void redimPiesa(unsigned nrPiesaAleasa, piesa Piesa[MAX2], unsigned nrPiese, legatura Legatura[MAX2], unsigned nrLegaturi, int z)
{
    deseneaza(Piesa[nrPiesaAleasa],FUNDAL,Piesa[nrPiesaAleasa].dimensiune);
    for(int i=1; i<=nrLegaturi; i++)
        if(Legatura[i].pStart.piesa==nrPiesaAleasa || Legatura[i].pEnd.piesa==nrPiesaAleasa)
            deseneazaLegatura(Piesa,nrPiese,Legatura[i],FUNDAL);
    Piesa[nrPiesaAleasa].dimensiune+=z;
    deseneaza(Piesa[nrPiesaAleasa],WHITE,Piesa[nrPiesaAleasa].dimensiune);
    for(int i=1; i<=nrLegaturi; i++)
        if(Legatura[i].pStart.piesa==nrPiesaAleasa || Legatura[i].pEnd.piesa==nrPiesaAleasa)
            deseneazaLegatura(Piesa,nrPiese,Legatura[i],WHITE);
    refresh(Piesa,nrPiese,Legatura,nrLegaturi);
}

void stergePiesa(unsigned nrPiesaAleasa, piesa Piesa[MAX2], unsigned& nrPiese, legatura Legatura[MAX2], unsigned& nrLegaturi)
{
    deseneaza(Piesa[nrPiesaAleasa],FUNDAL,Piesa[nrPiesaAleasa].dimensiune);
    for(int i=1; i<=nrLegaturi; i++)
        if(Legatura[i].pStart.piesa==nrPiesaAleasa || Legatura[i].pEnd.piesa==nrPiesaAleasa)
        {
            deseneazaLegatura(Piesa,nrPiese,Legatura[i],FUNDAL);
            for(int j=i; j<nrLegaturi; j++)
                Legatura[j]=Legatura[j+1];
            nrLegaturi--;
            i--;
        }
    for(int i=nrPiesaAleasa; i<nrPiese; i++)
        Piesa[i]=Piesa[i+1];
    nrPiese--;
    refresh(Piesa,nrPiese,Legatura,nrLegaturi);
}

void deseneazaLegatura(piesa Piesa[MAX2], unsigned int nrPiese, legatura leg, int culoare)
{
    setcolor(culoare);
    if(leg.varianta == 1)
    {
        line(Piesa[leg.pStart.piesa].x + (Piesa[leg.pStart.piesa].dimensiune * leg.pStart.x), Piesa[leg.pStart.piesa].y + (Piesa[leg.pStart.piesa].dimensiune * leg.pStart.y), Piesa[leg.pStart.piesa].x + (Piesa[leg.pStart.piesa].dimensiune * leg.pStart.x), Piesa[leg.pEnd.piesa].y + (Piesa[leg.pEnd.piesa].dimensiune * leg.pEnd.y));
        line(Piesa[leg.pStart.piesa].x + (Piesa[leg.pStart.piesa].dimensiune * leg.pStart.x), Piesa[leg.pEnd.piesa].y + (Piesa[leg.pEnd.piesa].dimensiune * leg.pEnd.y), Piesa[leg.pEnd.piesa].x + (Piesa[leg.pEnd.piesa].dimensiune * leg.pEnd.x), Piesa[leg.pEnd.piesa].y + (Piesa[leg.pEnd.piesa].dimensiune * leg.pEnd.y));
    } //desenez mai intai  linia verticala si dupa orizontala
    else
    {
        line(Piesa[leg.pStart.piesa].x + (Piesa[leg.pStart.piesa].dimensiune * leg.pStart.x), Piesa[leg.pStart.piesa].y + (Piesa[leg.pStart.piesa].dimensiune * leg.pStart.y), Piesa[leg.pEnd.piesa].x + (Piesa[leg.pEnd.piesa].dimensiune * leg.pEnd.x), Piesa[leg.pStart.piesa].y + (Piesa[leg.pStart.piesa].dimensiune * leg.pStart.y));
        line(Piesa[leg.pEnd.piesa].x + (Piesa[leg.pEnd.piesa].dimensiune * leg.pEnd.x), Piesa[leg.pStart.piesa].y + (Piesa[leg.pStart.piesa].dimensiune * leg.pStart.y), Piesa[leg.pEnd.piesa].x + (Piesa[leg.pEnd.piesa].dimensiune * leg.pEnd.x), Piesa[leg.pEnd.piesa].y + (Piesa[leg.pEnd.piesa].dimensiune * leg.pEnd.y));
    } //desenez orizontala si dupa verticala

}

punctLegatura alegePunctLegatura(piesa Piesa[MAX2], unsigned int nrPiese)
{
    bool click = false;
    //clicku controleaza cat timp ruleaza bucla while
    int xMouse, yMouse; //coordonatele cursorului
    punctLegatura p; //punctul ales
    do
        if(ismouseclick(WM_LBUTTONDOWN) && !click) //daca am facut click si pana atunci click era fals
        {
            clearmouseclick(WM_LBUTTONDOWN); //curat eventualele clickuri
            xMouse = mousex();
            yMouse = mousey(); //iau coord mouseului
            for(unsigned int i = 1; i <= nrPiese; i++) //parcurg piese
                for(unsigned int j = 1; j <= Piesa[i].nrLegaturi; j++) //parcurg puncte de leg
                    if(abs(Piesa[i].x + (zoom * Piesa[i].pLeg[j].x) - xMouse) <= 10 && abs(Piesa[i].y + (zoom * Piesa[i].pLeg[j].y) - yMouse) <= 10) //verific daca clickul a fost destul de aproape
                    {

                        p = Piesa[i].pLeg[j];
                        p.piesa = i;
                        click = true; //atribui variabilei p punctul de leg gasit

                    }
        }
    while(!click); //ruleaza pana fac click
    return p;
}

bool verificaPunctLegatura(piesa Piesa[MAX2], unsigned int nrPiese, int x, int y, legatura &leg)
{
    for(unsigned int i = 1; i <= nrPiese; i++) //parcurg piese
        for(unsigned int j = 1; j <= Piesa[i].nrLegaturi; j++) //parcurg puncte de leg
            if(abs(Piesa[i].x + (zoom * Piesa[i].pLeg[j].x) - x) <= 10 && abs(Piesa[i].y + (zoom * Piesa[i].pLeg[j].y) - y) <= 10) //verific daca am un punct aproape
            {

                leg.pEnd.x = Piesa[i].pLeg[j].x;
                leg.pEnd.y = Piesa[i].pLeg[j].y;
                leg.pEnd.piesa = i; //actualizez legatura
                return true;
            }
    return false;
}

void deseneazaLegaturaStart(piesa Piesa[MAX2], punctLegatura pStart, int xEnd, int yEnd, int culoare)
{
    setcolor(culoare);
    line(Piesa[pStart.piesa].x + (Piesa[pStart.piesa].dimensiune * pStart.x), Piesa[pStart.piesa].y + (Piesa[pStart.piesa].dimensiune * pStart.y), Piesa[pStart.piesa].x + (Piesa[pStart.piesa].dimensiune * pStart.x), yEnd);
    line(Piesa[pStart.piesa].x + (Piesa[pStart.piesa].dimensiune*pStart.x),yEnd,xEnd,yEnd);//linia vert si orizontala
}

void refresh(piesa Piesa[MAX2], unsigned int nrPiese, legatura Legatura[MAX2], unsigned int nrLegaturi)
{
    setcolor(WHITE);
    for(int i = 1; i <= nrPiese; i++)
        deseneaza(Piesa[i], WHITE,Piesa[i].dimensiune);
    for(int i = 1; i <= nrLegaturi; i++)
        deseneazaLegatura(Piesa, nrPiese, Legatura[i], WHITE); //redesenez toate piesele si legaturile
    int lat=getmaxx()/nrPieseMeniu;
    meniu();
}

void puneLegatura(piesa Piesa[MAX2], unsigned int nrPiese, legatura Legatura[MAX2], unsigned int &nrLegaturi, int zoom)
{

    punctLegatura legStart, legEnd; //punctu de start
    int xEnd, yEnd; //coord finale
    legStart = alegePunctLegatura(Piesa, nrPiese);
    nrLegaturi++;
    Legatura[nrLegaturi].pStart = legStart;
    Legatura[nrLegaturi].dimensiune=zoom;
    //inregistrez legatura
    do
    {
        while(!ismouseclick(WM_LBUTTONUP))
        {
            deseneazaLegaturaStart(Piesa, legStart, xEnd, yEnd, FUNDAL);//la fiecare iteratie reactualizez legatura
            delay(50);
            if(ismouseclick(WM_MOUSEMOVE)) //daca apas
                getmouseclick(WM_MOUSEMOVE, xEnd, yEnd); //actualizez coord
            deseneazaLegaturaStart(Piesa, legStart, xEnd, yEnd, WHITE);
            delay(50);
        }
        deseneazaLegaturaStart(Piesa, legStart, xEnd, yEnd, FUNDAL);
        getmouseclick(WM_LBUTTONUP, xEnd, yEnd);
        deseneazaLegaturaStart(Piesa, legStart, xEnd, yEnd, WHITE);
        //se actualizeaza desenul legaturii in functie de pozitia cursorului la fiecare iteratie
    }
    while(!verificaPunctLegatura(Piesa, nrPiese, xEnd, yEnd, Legatura[nrLegaturi]) && !ismouseclick(WM_RBUTTONDOWN) ); //ruleaza cat timp misc mouseul
    //am pus si cu dreapta ca nu se mai oprea
    deseneazaLegaturaStart(Piesa, legStart, xEnd, yEnd, FUNDAL);
    if(ismouseclick(WM_RBUTTONDOWN))
    {
        nrLegaturi--;
        clearmouseclick(WM_RBUTTONDOWN);
    }
    else
        deseneazaLegatura(Piesa, nrPiese, Legatura[nrLegaturi], WHITE);

    refresh(Piesa,nrPiese,Legatura,nrLegaturi); //actualizeaza starea generala a pieselor
    clearmouseclick(WM_LBUTTONUP); //curat mouseul
}

void initializareFisier(char numeFis[MAX1])
{
    char inceput[MAX2]="C:\\CodeBlocks\\";
    printf("Denumiti proiectul>> ");
    scanf("%s", numeFis);
    strcat(inceput,numeFis);
    strcat(inceput,".txt");
    strcpy(numeFis,inceput);
    strupr(numeFis);
}

void scrieInFisier(piesa Piesa[MAX2],legatura Legatura[MAX2], unsigned nrPiese,unsigned nrLegaturi, FILE *fisier)
{
    Piesa[0].x=nrPiese;
    Piesa[0].y=nrLegaturi;
    for(unsigned i=0; i<=nrPiese; i++)
        fwrite(&(Piesa[i]),sizeof(struct piesa),1,fisier);
    for(unsigned i=1; i<=nrLegaturi; i++)
        fwrite(&(Legatura[i]),sizeof(struct legatura),1,fisier);

}

void citesteDinFisier(piesa Piesa[MAX2],legatura Legatura[MAX2], unsigned &nrPiese,unsigned &nrLegaturi, FILE *fisier)
{

    fread(&Piesa[0],sizeof(struct piesa),1,fisier);
    nrPiese=Piesa[0].x;
    nrLegaturi=Piesa[0].y;
    for(unsigned i=1; i<=nrPiese; i++)
        fread(&Piesa[i],sizeof(struct piesa),1,fisier);
    for(unsigned i=1; i<=nrLegaturi; i++)
        fread(&Legatura[i],sizeof(struct legatura),1,fisier);
}

void buton(int x,int y,char *text)//pentru prima pg
{
    setfillstyle(SOLID_FILL,LIGHTGRAY);
    bar(x,y,x+220,y+80);
    rectangle(x,y,x+220,y+80);
    settextstyle(BOLD_FONT,HORIZ_DIR,2);
    int lg=textwidth(text);
    int h=textheight(text);
    outtextxy(x+(220-lg)/2,y+(80-h)/2,text);
}

void fundal() //pentru prima pg
{
    int lg=getmaxx();
    int h=getmaxy();
    int fontSize=20; //marimea nr de pe fundal

    setcolor(COLOR(0,50,0)); //estompare verde

    for(int i=0; i< lg; i+=fontSize)
        for(int j=0; j<h; j+=fontSize)
        {
            char digit=rand()%10+'0'; //generare cifra aleatorie
            outtextxy(i,j,&digit);
        }
}

void meniu_info()
{
    setcolor(GREEN);
    rectangle(0,0,getmaxx(),getmaxy());
    line(120,0,120,getmaxy());
    line(0,90,getmaxx(),90);
    strcpy(Meniu[1].nume,"DIODA");
    strcpy(Meniu[2].nume,"ZENNER");
    strcpy(Meniu[3].nume,"TRANZNPN");
    strcpy(Meniu[4].nume,"TRANZPNP");
    strcpy(Meniu[5].nume,"CONDENS");
    strcpy(Meniu[6].nume,"REZIST");
    strcpy(Meniu[7].nume,"BATERIE");
    strcpy(Meniu[8].nume,"POLARIZ");
    strcpy(Meniu[9].nume,"SINU");
    strcpy(Meniu[10].nume,"SERVOMOT");
    strcpy(Meniu[11].nume,"AMPLOP");
    strcpy(Meniu[12].nume,"NOD");
    strcpy(Meniu[13].nume,"LINIE");
    strcpy(Meniu[14].nume,"MUTA");
    strcpy(Meniu[15].nume,"ROTESTE");
    strcpy(Meniu[16].nume,"PLUS");
    strcpy(Meniu[17].nume,"MINUS");
    strcpy(Meniu[18].nume,"STERGE");
    strcpy(Meniu[19].nume,"SALVEAZA");
    strcpy(Meniu[20].nume,"STOP");
    for(unsigned i=1; i<=nrPieseMeniu; i++)
    {
        initializeaza(Meniu[i]);
        amplaseaza(Meniu[i],60,i*60,0);
        deseneaza(Meniu[i],WHITE,zoomMen);
        setcolor(GREEN);
        line(0,i*60+30,getmaxx(),i*60+30);
        setcolor(WHITE);

        if(i==1)
        {
            outtextxy(130,(i-1)*60+40,"DIODA este o componenta electronica cu doua terminale avand conductanta asimetrica. ");
            outtextxy(130,(i-1)*60+60,"Dioda are rezistenta mica la trecerea curentului intr-o directie si rezistenta mare la trecerea in cealalta directie.");
        }
        if(i==2)
        {
            outtextxy(130,(i-1)*60+40,"DIODA ZENNER este un dispozitiv electronic ce permite trecerea curentului doar intr-o singura directie.");
            outtextxy(130,(i-1)*60+60,"O dioda in mod normal conduce intr-un sens curentul si il blocheaza in celalalt.");
        }
        if(i==3)
            outtextxy(130,(i-1)*60+40,"TRANZISTORUL NPN este un tip de tranzistor bipolar format din semiconductor tip p care este aplicat intre doua semiconductoare de tip n.");
        if(i==4)
        {
            outtextxy(130,(i-1)*60+40,"TRANZISTORUL BIPOLAR este un dispozitiv semiconductor cu doua jonctiuni in succesine npn sau pnp. ");
            outtextxy(130,(i-1)*60+60,"Se foloseste in circuitele electronice atat digitale cat si analogice, de obicei pentru a amplifica sau a transmite un semnal electric.");
        }
        if(i==5){
            outtextxy(130,(i-1)*60+40,"CONDENSATORUL este un dispozitiv electric pasiv ce inmagazineaza energie sub forma unui camp electric intre doua armaturi incarcate cu o sarcina electrica egala, dar cu semn opus.");
            outtextxy(130,(i-1)*60+60,"Acesta mai este cunoscut si sub denumirea de capacitor.");
        }
        if(i==6){
            outtextxy(130,(i-1)*60+40,"REZISTORUL este o piesa componenta din circuitele electrice a carei principala proprietate este rezistenta electrica. ");
            outtextxy(130,(i-1)*60+60,"Rezistorul obisnuit are doua terminale; Curentul care curge prin rezistor este proportional cu tensiunea aplicata pe terminalele lui.");
        }
        if(i==7){
            outtextxy(130,(i-1)*60+40,"Bateria este un mediu electrochimic de stocare a energiei. ");
            outtextxy(130,(i-1)*60+60,"La descarcare se transforma energia chimica (stocata) in energie electrica cu ajutorul unor reactii de tip redox electrochimice.");
        }
        if(i==8){
            outtextxy(130,(i-1)*60+40,"CONDENSATORUL POLARIZAT este utilizat in general atunci cand sunt necesare valori foarte mari de capacitate. ");
            outtextxy(130,(i-1)*60+60,"In loc sa foloseasca un strat foarte subtire de film mecanic pentru unul din electrozi este utilizata o solutie de electrolit semi-lichida.");
    }
        if(i==9){
            outtextxy(130,(i-1)*60+40,"POARTA NAND denumita si <<si-nu logic>> este o poarta logica digitala care implementeaza operatorul lui Sheffler. ");
            outtextxy(130,(i-1)*60+60,"Pentru intrarile (0,0),(0,1),(1,0) va avea la iesire valoarea 1, iar pentru intrarea (1,1) valoarea 0.");
        }
        if(i==10){
            outtextxy(130,(i-1)*60+40,"SERVOMOTORUL este utilizat pentru pozitia exacta si controlul vitezei la cupluri mari. ");
            outtextxy(130,(i-1)*60+60,"Consta dintr-un motor adecvat, senzor de pozitie si un controler. Daca motorul de curent continuu este comandat folosind servomecanism se numeste servomotor DC.");
        }
        if(i==11){
            outtextxy(130,(i-1)*60+40,"AMPLIFICATORUL OPERATIONAL este un amplificator cuplat in curent continuu care amplifica puternic tensiuni aplicate la doua intrari si are uzual o singura iesire. ");
            outtextxy(130,(i-1)*60+60,"Are functional un punct de nul adica este alimentat de doua tensiuni: pozitiva si negativa.");
        }
        if(i==12)
            outtextxy(130,(i-1)*60+40,"NODUL este punctul unui circuit in care sunt interconectate cel putin trei elemente de circuit.");
        if(i==20)
            outtextxy(40,770,"INAPOI");
    }
}

int mx,my;

int main()
{
eticheta1:
eticheta2:
    initwindow(1000,600);

    fundal();

    //Titlul
    setcolor(COLOR(0, 128, 0));
    settextstyle(BOLD_FONT, HORIZ_DIR, 4);
    outtextxy(425, 100, "Electron");

    //Butoane
    int buttonWidth=220;
    int buttonHeight=80;
    int screenWidth=getmaxx();
    int screenHeight=getmaxy();

    int X = (screenWidth - buttonWidth) / 2;
    int Y1 = (screenHeight - buttonHeight) / 2 - 100;
    int Y2 = (screenHeight - buttonHeight) / 2;
    int Y3 = (screenHeight - buttonHeight) / 2 + 100;

    //desenare butoane
    buton(X, Y1, "Incepe proiect nou");
    buton(X, Y2, "Deschide proiect");
    buton(X, Y3, "Informatii");

    //credits
    settextstyle(BOLD_FONT, HORIZ_DIR, 1);
    outtextxy(screenWidth - 300, screenHeight - 50, "Realizat de Martisca Diana");
    outtextxy(screenWidth - 300, screenHeight - 30, "si Maciuc Mihai");

    /*//sa astepte tasta
    while(!kbhit())
        delay(50);
    closegraph();

    cout<<X<<" "<<Y1<<endl;
    cout<<X<<" "<<Y2<<endl;
    cout<<X<<" "<<Y3<<endl;*/


    selectieMeniu(mx,my);
    cleardevice();
    closegraph();
    piesa Piesa[MAX2];
    unsigned nrPiese=0;
    legatura Legatura[MAX2];
    unsigned nrLegaturi=0;
    unsigned nrPiesaAleasa;
    char numeFisier[100];
    bool initializat=false;
    if(mx>=X &&mx<=X+buttonWidth &&my>=Y1 &&my<=Y1+buttonHeight) ///primul buton
    {

        initwindow(960,1000,"Electron");
        setfillstyle(1,FUNDAL);
        bar(0,0,960,1000);

        meniu();
        do
        {
            nrPiesaAleasa=alegePiesa(Meniu,nrPieseMeniu);
            if (nrPiesaAleasa<=12)
            {
                nrPiese++;
                Piesa[nrPiese]=Meniu[nrPiesaAleasa];
                punePiesa(Piesa[nrPiese]);
            }
            else
            {
                switch(nrPiesaAleasa)
                {
                case 13:
                    puneLegatura(Piesa,nrPiese,Legatura,nrLegaturi,Piesa[nrPiesaAleasa].dimensiune);
                    break;
                case 14:
                    nrPiesaAleasa=alegePiesa(Piesa,nrPiese);
                    mutaPiesa(nrPiesaAleasa,Piesa,nrPiese,Legatura,nrLegaturi);
                    break;
                case 15:
                    nrPiesaAleasa=alegePiesa(Piesa,nrPiese);
                    rotestePiesa(nrPiesaAleasa,Piesa,nrPiese,Legatura,nrLegaturi);
                    break;
                case 16:
                    nrPiesaAleasa=alegePiesa(Piesa,nrPiese);
                    redimPiesa(nrPiesaAleasa,Piesa,nrPiese,Legatura,nrLegaturi,2);
                    break;
                case 17:
                    nrPiesaAleasa=alegePiesa(Piesa,nrPiese);
                    redimPiesa(nrPiesaAleasa,Piesa,nrPiese,Legatura,nrLegaturi,-2);
                    break;
                case 18:
                    nrPiesaAleasa=alegePiesa(Piesa,nrPiese);
                    stergePiesa(nrPiesaAleasa,Piesa,nrPiese,Legatura,nrLegaturi);
                    break;
                case 19:
                    closegraph();
                    system("cls");
                    if(!initializat)
                    {
                        initializareFisier(numeFisier);
                        printf("Am salvat proiectul>> %s",numeFisier);
                        printf("\n");
                        initializat=true;
                    }
                    FILE *fisier;
                    fisier=fopen(numeFisier,"wb");
                    scrieInFisier(Piesa,Legatura,nrPiese,nrLegaturi,fisier);
                    fclose(fisier);
                    fisier=fopen(numeFisier,"rb");
                    citesteDinFisier(Piesa,Legatura,nrPiese,nrLegaturi,fisier);
                    fclose(fisier);
                    initwindow(960,1000,"Electron");
                    meniu();
                    setcolor(WHITE);
                    for(int i=1; i<=nrPiese; i++)
                        deseneaza(Piesa[i],WHITE,Piesa[i].dimensiune);
                    for(int i=1; i<=nrLegaturi; i++)
                        deseneazaLegatura(Piesa,nrPiese,Legatura[i],WHITE);
                    break;
                case 20:
                    closegraph();
                    goto eticheta2;
                    break;
                default:
                    break;
                }
            }
        }
        while(nrPiesaAleasa!=nrPieseMeniu);
        getch();
        closegraph();
    }
    else if(mx>=X &&mx<=X+buttonWidth &&my>=Y2 &&my<=Y2+buttonHeight) ///al doilea buton
    {
        printf("Numele proiectului este>> ");
        char numeFisier[MAX2],fisCitit[MAX1];
        strcpy(numeFisier,"C:\\CodeBlocks\\");
        scanf("%s",fisCitit);
        strcat(numeFisier,fisCitit);
        strcat(numeFisier,".txt");
        strupr(numeFisier);
        printf("Am deschis proiectul>> %s",numeFisier);
        printf("\n");
        FILE *fisier;
        fisier=fopen(numeFisier,"rb");
        citesteDinFisier(Piesa,Legatura,nrPiese,nrLegaturi,fisier);
        fclose(fisier);
        initwindow(960,1000,"Electron");
        meniu();
        setcolor(WHITE);
        for(int i = 1; i <= nrPiese; i++)
            deseneaza(Piesa[i], WHITE,Piesa[i].dimensiune);
        for(int i = 1; i <= nrLegaturi; i++)
            deseneazaLegatura(Piesa,nrPiese,Legatura[i],WHITE);
        initializat=true;
        do
        {
            nrPiesaAleasa=alegePiesa(Meniu,nrPieseMeniu);
            if (nrPiesaAleasa<=12)
            {
                nrPiese++;
                Piesa[nrPiese]=Meniu[nrPiesaAleasa];
                punePiesa(Piesa[nrPiese]);
            }
            else
            {
                switch(nrPiesaAleasa)
                {
                case 13:
                    puneLegatura(Piesa,nrPiese,Legatura,nrLegaturi,Piesa[nrPiesaAleasa].dimensiune);
                    break;
                case 14:
                    nrPiesaAleasa=alegePiesa(Piesa,nrPiese);
                    mutaPiesa(nrPiesaAleasa,Piesa,nrPiese,Legatura,nrLegaturi);
                    break;
                case 15:
                    nrPiesaAleasa=alegePiesa(Piesa,nrPiese);
                    rotestePiesa(nrPiesaAleasa,Piesa,nrPiese,Legatura,nrLegaturi);
                    break;
                case 16:
                    nrPiesaAleasa=alegePiesa(Piesa,nrPiese);
                    redimPiesa(nrPiesaAleasa,Piesa,nrPiese,Legatura,nrLegaturi,2);
                    break;
                case 17:
                    nrPiesaAleasa=alegePiesa(Piesa,nrPiese);
                    redimPiesa(nrPiesaAleasa,Piesa,nrPiese,Legatura,nrLegaturi,-2);
                    break;
                case 18:
                    nrPiesaAleasa=alegePiesa(Piesa,nrPiese);
                    stergePiesa(nrPiesaAleasa,Piesa,nrPiese,Legatura,nrLegaturi);
                    break;
                case 19:
                    closegraph();
                    system("cls");
                    if(!initializat)
                    {
                        initializareFisier(numeFisier);
                        printf("%s",numeFisier);
                        initializat=true;
                    }
                    FILE *fisier;
                    fisier=fopen(numeFisier,"wb");
                    scrieInFisier(Piesa,Legatura,nrPiese,nrLegaturi,fisier);
                    fclose(fisier);
                    fisier=fopen(numeFisier,"rb");
                    citesteDinFisier(Piesa,Legatura,nrPiese,nrLegaturi,fisier);
                    fclose(fisier);
                    initwindow(960,1000,"Electron");
                    meniu();
                    setcolor(WHITE);
                    for(int i=1; i<=nrPiese; i++)
                        deseneaza(Piesa[i],WHITE,Piesa[i].dimensiune);
                    for(int i=1; i<=nrLegaturi; i++)
                        deseneazaLegatura(Piesa,nrPiese,Legatura[i],WHITE);
                    break;
                case 20:
                    closegraph();
                    goto eticheta2;
                    break;
                default:
                    break;
                }
            }
        }
        while(nrPiesaAleasa!=nrPieseMeniu);
        getch();
        closegraph();
    }
    else if(mx>=X &&mx<=X+buttonWidth &&my>=Y3 &&my<=Y3+buttonHeight)  ///al treilea buton
    {
        initwindow(1800,1000,"Electron");
        setfillstyle(1,FUNDAL);
        bar(0,0,960,1000);
        int nx,ny;
        meniu_info();

        int h=textheight("INAPOI");
        int lg=textwidth("INAPOI");

        do
        {
            if (ismouseclick(WM_LBUTTONDOWN))
            {
                int x, y;
                getmouseclick(WM_LBUTTONDOWN, x, y);

                if (x >= 40 && x <= (40 + lg) && y >= 770 && y <= 770 + h)
                {
                    closegraph();
                    goto eticheta1;
                    break;

                }
            }

            delay(100);
        }
        while (1);


        closegraph();
    }

    return 0;
}
