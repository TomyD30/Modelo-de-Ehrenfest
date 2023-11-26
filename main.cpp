#include "glut.h"
#include <time.h>
#include <iostream>
#include <fstream>

using namespace std;

class Vector2D{
    public:
    int x,y;
    Vector2D(){x = 0 , y = 0;}
    Vector2D(int x_ , int y_){x = x_ , y = y_;}
    Vector2D operator+(Vector2D v){
        return Vector2D{x+v.x,y+v.y};
    }
    bool operator==(Vector2D v){
        return x == v.x && y == v.y;
    }
    void operator<<(ostream& os){
        os << x << " " << y;
    }
};

class lista4{
    public:
    float x1,x2,x3,x4;
    lista4(){x1 = 0 , x2 = 0 , x3 = 0 , x4 = 0;}
    void operator<<(ostream& os){
        os << x1 << " " << x2 << " " << x3 << " " << x4;
    }
    lista4 operator/(float n){
        x1 /= n , x2 /= n , x3 /= n , x4 /= n;
        return *this;
    }
};

enum Direccion{ARRIBA,ABAJO,DERECHA,IZQUIERDA};
Vector2D dirAVel(Direccion dir){
    switch (dir)
    {
    case ARRIBA:
        return Vector2D{0,1};
        break;
    case ABAJO:
        return Vector2D{0,-1};
        break;
    case DERECHA:
        return Vector2D{1,0};
        break;
    case IZQUIERDA:
        return Vector2D{-1,0};
        break;
    }
    return Vector2D{0,0};
}
Direccion velADir(Vector2D vel){
    if(vel.x == 0 && vel.y == 1) return ARRIBA;
    else if(vel.x == 0 && vel.y == -1) return ABAJO;
    else if(vel.x == 1 && vel.y == 0) return DERECHA;
    else return IZQUIERDA;
}

class Particula{
    private:
    Vector2D pos , pos0 , vel , vel0;
    Direccion dir;
    int pasos;
    bool orbitaCompleta;
    public:
        Particula(){}
        Particula(int x0 , int y0 , Direccion dir0){
            pasos = 0;
            pos = Vector2D(x0,y0);
            pos0 = Vector2D(x0,y0);
            dir = dir0;
            vel = dirAVel(dir0);
            vel0 = Vector2D{vel.x,vel.y};
        }
        //devuelve true si completo una orbita
        void mover(int xmin , int xmax , int ymin , int ymax){
            pos = pos + vel;
            pasos++;
            if(pos.x < xmin) pos.x = xmax;
            else if(pos.x > xmax) pos.x = xmin;
            if(pos.y < ymin) pos.y = ymax;
            else if(pos.y > ymax) pos.y = ymin;
            //para completar una orbita tiene que estar en la misma condicion inicial: pos = pos0 y vel = vel0
            if((pos == pos0 && vel == vel0) || pasos > 1000000) orbitaCompleta = true;
        }
        void graficar(){
            dibujarParticula(pos.x*800/10+5, pos.y*800/10+5);
        }
        Vector2D getPos(){
            return pos;
        }
        Vector2D getPos0(){
            return pos0;
        }
        Vector2D getVel(){
            return vel;
        }
        void setVel(Vector2D vel){
            this->vel = vel;
            dir = velADir(vel);
        }
        int getPasos(){
            return pasos;
        } 
        Direccion getDir(){
            return dir;
        }
        bool getOrbitaCompleta(){
            return orbitaCompleta;
        }
};

class Espejo{
    private:
    int tipo; // 1 o -1
    Vector2D pos;
    public:
        Espejo(){
            tipo = rand()%2 == 0 ? 1 : -1;
        }
        void setPos(Vector2D pos){
            this->pos = pos;
        }
        void graficar(){
            dibujarEspejo(pos.x*800/10+5, pos.y*800/10+5,tipo);
        }
        int getTipo(){
            return tipo;
        }
};

class Mapa{
    public:
    Mapa(int N , int n): n(n), N(N){
        int Nespejos = n*(N*N);
        espejos = new Espejo[Nespejos];
        mapa = new int*[N];
        for (int i = 0; i < N; i++) mapa[i] = new int[N];
        for (int i = 0; i < N; i++) for (int j = 0; j < N; j++) mapa[i][j] = 0; //pongo en cero todo el mapa, 0 = sin espejo, +-1 = espejo
        for (int i = 0; i < Nespejos; i++){
            int xi , yi;
            while(mapa[xi = rand()%N][yi = rand()%N] != 0); //busco una posicion vacia
            mapa[xi][yi] = espejos[i].getTipo();
            espejos[i].setPos(Vector2D{xi, yi});
        }
    }
    // void crearEspejos(int n){
    //     this->n = n;
    //     for (int i = 0; i < N; i++) mapa[i] = new int[N];
    //     for (int i = 0; i < N; i++) for (int j = 0; j < N; j++) mapa[i][j] = 0;
    //     for (int i = 0; i < n; i++){
    //         int xi , yi;
    //         while(mapa[xi = rand()%N][yi = rand()%N] != 0); //busco una posicion vacia
    //         mapa[xi][yi] = espejos[i].getTipo();
    //         espejos[i].setPos(Vector2D{xi, yi});
    //     }
    // }
    void reiniciar(float n_){
        n = n_;
        int Nespejos = n*(N*N);
        for (int i = 0; i < N; i++) for (int j = 0; j < N; j++) mapa[i][j] = 0;
        delete espejos;
        espejos = new Espejo[Nespejos];
        for (int i = 0; i < Nespejos; i++){
            int xi , yi;
            while(mapa[xi = rand()%N][yi = rand()%N] != 0);
            mapa[xi][yi] = espejos[i].getTipo();
            espejos[i].setPos(Vector2D{xi, yi});
        }
    }
    int getMapPos(int x , int y){
        return mapa[x][y];
    }
    void graficar(){
        for (int i = 0; i < n; i++)
        {
            espejos[i].graficar();
        }
    }
    private:
    Espejo* espejos;
    int** mapa;
    float n; //densidad de espejos
    int N; //tamaño del mapa
    int x0 , y0;
    Direccion dir0;  
};

class Campo{
    public:
};

class Simulacion{
    public:
    Simulacion(int N , string carpeta , bool activarGrafica): N(N) , mapa(N,0) , carpeta(carpeta), activarGrafica(activarGrafica),
    n(0) , t(0){
    }
    void temporal(int nParticulas , int nEspejos){
        ofstream datos("dist.dat");
        ofstream datosS("entropia.dat");
        datos << "tiempo ARRIBA ABAJO DERECHA IZQUIERDA" << endl;
        datosS << "t S" << endl;
        mapa.reiniciar(nEspejos);
        n = nParticulas;
        particulas = new Particula[nParticulas];
        for(int i = 0; i < nParticulas; i++) particulas[i] = Particula(rand()%N,rand()%N,Direccion(rand()%4));
        while(1){
            datos << t << " " , dist_Vels() << datos , datos << endl;
            datosS << t << " " << entropia() << endl;
            t++;
            for(int i = 0; i < nParticulas; i++) actualizar(&particulas[i]);
            if(activarGrafica) graficar();
        }
    }
    void temporal(int nParticulas , float nEspejos, Direccion dir0){
        ofstream datos("dist.dat");
        ofstream datosS("entropia.dat");
        datos << "tiempo ARRIBA ABAJO DERECHA IZQUIERDA" << endl;
        datosS << "t S" << endl;
        mapa.reiniciar(nEspejos);
        n = nParticulas;
        particulas = new Particula[nParticulas];
        for(int i = 0; i < nParticulas; i++) particulas[i] = Particula(rand()%N,rand()%N,dir0);
        while(1){
            datos << t << " " , dist_Vels() << datos , datos << endl;
            datosS << t << " " << entropia() << endl;
            t++;
            for(int i = 0; i < nParticulas; i++) actualizar(&particulas[i]);
            if(activarGrafica) graficar();
        }
    }
    void temporal2(int nParticulas, float nMin , float nMax, float nPasos , int tmax , Direccion dir0){
        lista4 dist;
        float promArriba  = 0.0, promAbajo  = 0.0, promDerecha  = 0.0, promIzquierda = 0.0;
        ofstream datos("distFinal.dat");
        datos << "densidad ARRIBA ABAJO DERECHA IZQUIERDA" << endl;
        n = nParticulas;
        for(float n_ = nMin ; n_ < nMax ; n_+=nPasos){
            promArriba  = 0.0, promAbajo  = 0.0, promDerecha  = 0.0, promIzquierda = 0.0;
            mapa.reiniciar(n_);
            particulas = new Particula[nParticulas];
            for(int i = 0; i < nParticulas; i++) particulas[i] = Particula(rand()%N,rand()%N,dir0);
            t = 0;
            while(t < tmax){
                t++;
                for(int i = 0; i < nParticulas; i++) actualizar(&particulas[i]);
                if(activarGrafica) graficar();
                dist = dist_Vels();
                promArriba += dist.x1 , promAbajo += dist.x2 , promDerecha += dist.x3 , promIzquierda += dist.x4;
            }
            datos << n_ << " " << promArriba/tmax << " " << promAbajo/tmax << " " << promDerecha/tmax << " " << promIzquierda/tmax << endl;
            delete particulas;
        }
    }
    lista4 dist_Vels(){
        lista4 dist;
        for(int i = 0 ; i < n ; i++){
            Direccion dir = particulas[i].getDir();
            if(dir == ARRIBA) dist.x1++;
            else if(dir == ABAJO) dist.x2++;
            else if(dir == DERECHA) dist.x3++;
            else dist.x4++;
        }
        return dist/n;
    }
    float entropia(){
        lista4 dist = dist_Vels();
        float S = 0;
        if(dist.x1 != 0) S += dist.x1*log2(dist.x1);
        if(dist.x2 != 0) S += dist.x2*log2(dist.x2);
        if(dist.x3 != 0) S += dist.x3*log2(dist.x3);
        if(dist.x4 != 0) S += dist.x4*log2(dist.x4);
        return -S;
    }
    void comenzar(float nMin , float nMax , float nPasos, int its){
        n = 1;
        orbitas = new int**[N];
        for (int i = 0; i < N; i++){
            orbitas[i] = new int*[N];
            for (int j = 0; j < N; j++) orbitas[i][j] = new int[4];
        }
        for (int i = 0; i < N; i++) for (int j = 0; j < N; j++) for (int k = 0; k < 4; k++) orbitas[i][j][k] = 0;
        particulas = new Particula[1];
        int pasos;
        ofstream datos;
        for(int n = nMin ; n < nMax ; n += nPasos){
            datos.open(carpeta + to_string(n) + ".dat");
            for(int i = 0 ; i < its ; i++){
                mapa.reiniciar(n);
                reiniciar();
                //el modelo es determinista, recorriendo todas las condiciones iniciales tengo todos los datos posible
                for(int dir = 0 ; dir < 4 ; dir++){
                    for(int x = 0 ; x < N ; x++){
                        for(int y = 0 ; y < N ; y++){
                            //chequeo que no haya visitado esa posicion con esa direccion, ya que da la misma orbita
                            if(!posicionLibre(x,y,Direccion(dir))) continue;
                            particulas[0] = Particula(x,y,Direccion(dir));
                            Particula *particula = &particulas[0];  
                            while(!particula->getOrbitaCompleta()){
                                Vector2D posActual = particula->getPos();
                                orbitas[posActual.x][posActual.y][particula->getDir()] = 1; //marco la posicion actual como visitada
                                actualizar(particula);
                                if(activarGrafica) graficar();
                            }
                            datos << pasos << endl;
                        }
                    }   
                }
            }
            datos.close();
        }
    }
    void actualizar(Particula *particula){
        Vector2D posActual = particula->getPos();
        //chequeo que hay en el mapa en la posicion de la particula
        int mapPos = mapa.getMapPos(posActual.x,posActual.y);
        //si hay un espejo
        if(mapPos != 0){
            Vector2D vel = particula->getVel();
            //me fijo que tipo de espejo es
            if(mapPos == 1){ //espejo "\"
                if(vel.x > 0) particula->setVel(Vector2D{0,1});
                else if(vel.x < 0) particula->setVel(Vector2D{0,-1});
                else if(vel.y > 0) particula->setVel(Vector2D{1,0});
                else particula->setVel(Vector2D{-1,0});

            }
            else{ //espejo "/"
                if(vel.x > 0) particula->setVel(Vector2D{0,-1}); //puedo reducir los 4 casos a dos, poniendo el signo segun el signo de la velocidad con la que viene
                else if(vel.x < 0) particula->setVel(Vector2D{0,1});
                else if(vel.y > 0) particula->setVel(Vector2D{-1,0});
                else particula->setVel(Vector2D{1,0});
            }
            //cout << "Velocidad antes del choque: " << vel << endl;
        }
        particula->mover(0,N-1,0,N-1);
    }
    void reiniciar(){
        for (int i = 0; i < N; i++) for (int j = 0; j < N; j++) for (int k = 0; k < 4; k++) orbitas[i][j][k] = 0;   
    }
    bool posicionLibre(int x0 , int y0 , Direccion dir0){
        return (orbitas[x0][y0][dir0] == 0);
    }
    void graficar(){
        glClear(GL_COLOR_BUFFER_BIT);
        mapa.graficar();
        for(int i = 0 ; i < n ; i++) particulas[i].graficar();
        Sleep(400);
        glutSwapBuffers();
        glutMainLoopEvent();
    }
    private:
    Mapa mapa;
    int N;
    string carpeta;
    bool activarGrafica;
    Particula *particulas;
    int n;
    int*** orbitas; //matriz 3D que indica si una posicion con una direccion ya fue visitada
    int t;
};

int main(int argc, const char** argv){
    srand(time(NULL));
    //glutStuff(argc, argv);

    int N = 100;
    // int nMin = 1 , nMax = 2  , nPasos = 1 , its = 1;
    string carpeta = "";
    Simulacion simulacion(N,carpeta,false);
    //simulacion.comenzar(nMin,nMax,nPasos,its);
    float n = 0.2;
    //simulacion.temporal(10000,n);
    simulacion.temporal2(10000,0.1,1.0,0.05,10000,ARRIBA);
    return 0;
}


