#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <algorithm>

#define MIN 0
#define MAX 100
#define tam_gene 10
#define tam_crom 10

using namespace std;

class Gene {
private:
    double valor;
public:
    double getValor() {
        return valor;
    }

    void setValor(double v){
        this->valor+=v;
    }
    /*
     * Cria um gene aleatoriamente
     */
    Gene(){
        this->valor = MIN + static_cast <double> (rand()) /( static_cast <double> (RAND_MAX/(MAX-MIN)));
    }
};

class Cromossomo{

private:
    double valor;
public:
    vector<Gene> Xgene;
    double aptidao;

    Cromossomo(){
        for(int i=0; i< tam_gene; i++){
            Gene *a = new Gene();
            Xgene.push_back(*a);

            valor+=a->getValor();
        }
    }

    void mostraCromossomo(){
        for(int i = 0; i < Xgene.size();i++){
            cout << Xgene[i].getValor() <<  " ";
        }
    }

    void calcularFitness(){
        aptidao = funcao(valor);
    }

    double funcao(double x){
        return x*x;
    }

};

class GA{

    vector<Cromossomo> populacao;

    GA(){
        for(int i=0; i< tam_crom; i++){
            Cromossomo *c = new Cromossomo();
            populacao.push_back(*c);
        }
    }

    vector<Cromossomo> selecao(){
        //sort (c->Xgene.begin(), c->Xgene.end());
        vector<Cromossomo> ganhadores;
        int num_torneios = 0;

        while(num_torneios < tam_crom){
            int posicao1 = rand() % tam_crom;
            int posicao2 = rand() % tam_crom;

            if(posicao1 != posicao2){
                num_torneios++;

                if(populacao[posicao1].aptidao < populacao[posicao2].aptidao){
                    ganhadores.push_back(populacao[posicao1]);
                }else{
                    ganhadores.push_back(populacao[posicao2]);
                }
            }
        }
        return ganhadores;
    }

    void mutacao(){
        double taxa_mutacao = ((double) rand()) / (double) RAND_MAX;

        if(taxa_mutacao <= 0.025){
            int pos_crom = rand() % tam_crom;
            int pos_gene = rand() % tam_gene;
            double random = ((double) rand()) / (double) RAND_MAX;
            double valor_mutacao = random*0.1;

            populacao[pos_crom].Xgene[pos_gene].setValor(valor_mutacao);
        }
    }

    void crossover(){
        double taxa_reproducao = ((double) rand()) / (double) RAND_MAX;
        if(taxa_reproducao <= 0.9){
            //ocorre reproducao
        }
    }

};

int main()
{
    //srand(time(NULL));

    Cromossomo *c1 = new Cromossomo();
    c1->mostraCromossomo();
    c1->calcularFitness();

}
