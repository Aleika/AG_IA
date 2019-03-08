#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <algorithm>

#define MIN 0
#define MAX 100
#define tamanho 10

using namespace std;

class Gene {
private:
    float valor;
public:
    int getValor() {
        return valor;
    }
    /*
     * Cria um gene aleatoriamente
     */
    Gene(){
        this->valor = MIN + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(MAX-MIN)));
    }
};

class Cromossomo{

public:
    vector<Gene> Xgene;

    Cromossomo(){
        for(int i=0; i< tamanho; i++){
            Gene *a = new Gene();
            Xgene.push_back(*a);
        }
    }

    void mostraCromossomo(){
        for(int i = 0; i < Xgene.size();i++){
            cout << Xgene[i].getValor() <<  " ";
        }
    }

    void calcularFitness(){
        vector<float> resultFitness;
        for(int i =0;i<tamanho;i++){
            resultFitness.push_back(funcao(Xgene[i].getValor()));
        }
    }

    float funcao(float x){
        return x*x;
    }

};

class GA{
    Cromossomo *c = new Cromossomo();

    vector<Gene> selecao(){
        //sort (c->Xgene.begin(), c->Xgene.end());
        vector<Gene> ganhadores;
        int num_torneios = 0;
        while(num_torneios < tamanho){
            int posicao1 = rand() % tamanho;
            int posicao2 = rand() % tamanho;

            if(posicao1 != posicao2){
                num_torneios++;

                if(c->Xgene[posicao1].getValor() >= c->Xgene[posicao2].getValor()){
                    ganhadores.push_back(c->Xgene[posicao1]);
                }else{
                    ganhadores.push_back(c->Xgene[posicao2]);

                }
            }
        }
        return ganhadores;
    }

};

int main()
{
    srand(time(NULL));

    Cromossomo *c1 = new Cromossomo();
    c1->mostraCromossomo();
    c1->calcularFitness();

}
