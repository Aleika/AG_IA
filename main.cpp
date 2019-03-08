#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <algorithm>

#define MIN 0
#define MAX 100
#define tam_crom 10

using namespace std;

class Cromossomo{

private:
    double valor;
public:
    double aptidao;

    Cromossomo(){
        valor = MIN + static_cast <double> (rand()) /( static_cast <double> (RAND_MAX/(MAX-MIN)));
    }

    double getValor() {
        return valor;
    }

    void setValor(double v){
        valor+=v;
    }

    double funcao(double x){
        return x*x;
    }

    void calcularFitness(){
        aptidao = funcao(valor);
    }


};

class GA{

public:
    vector<Cromossomo> populacao;

    GA(){
        cout<<"Gerando população inicial: "<<endl;
        for(int i=0; i< tam_crom; i++){
            Cromossomo *c = new Cromossomo();
            cout<< c->getValor()<<" ";
            populacao.push_back(*c);
        }
    }

    vector<Cromossomo> selecao(){

        vector<Cromossomo> ganhadores;
        int num_torneios = 0;
        cout<<endl<<"Selecionando mais aptos:"<<endl;

        while(num_torneios < tam_crom){
            cout<<"Torneio "<<num_torneios<<endl;
            
            int posicao1 = rand() % tam_crom;
            int posicao2 = rand() % tam_crom;
            cout<< "Indices sorteados: "<< posicao1 <<", "<<posicao2<<endl;
            cout<< "Aptidão de ambos: "<< populacao[posicao1].aptidao << ", "<<populacao[posicao2].aptidao<<endl;

            if(posicao1 != posicao2){
                num_torneios++;

                if(populacao[posicao1].aptidao < populacao[posicao2].aptidao){
                    ganhadores.push_back(populacao[posicao1]);
                }else{
                    ganhadores.push_back(populacao[posicao2]);
                }
            }
        }
        cout<<"Ganhadores dos torneios:"<<endl;
        for(int i=0;i<tam_crom;i++){
            cout<<ganhadores[i].getValor()<<" ";
        }
        return ganhadores;
    }

    void mutacao(){
        double taxa_mutacao = ((double) rand()) / (double) RAND_MAX;

        if(taxa_mutacao <= 0.025){
            int pos_crom = rand() % tam_crom;
            double random = ((double) rand()) / (double) RAND_MAX;
            double valor_mutacao = random*0.1;

            populacao[pos_crom].setValor(valor_mutacao);
            populacao[pos_crom].calcularFitness();

        }
    }

    void crossover(){
        double taxa_reproducao = ((double) rand()) / (double) RAND_MAX;
        if(taxa_reproducao <= 0.9){
            vector<Cromossomo> pais = selecao();
            vector<Cromossomo> filhos;

            for(int i=0;i<pais.size()-1;i++){
                Cromossomo *f1 = new Cromossomo();
                Cromossomo *f2 = new Cromossomo();

                double beta = ((double) rand()) / (double) RAND_MAX;

                double val_f1 = pais[i].getValor() + beta*(pais[i+1].getValor() - pais[i].getValor());
                double val_f2 = pais[i+1].getValor() + beta*(pais[i].getValor() - pais[i+1].getValor());

                f1->setValor(val_f1);
                f2->setValor(val_f2);

                filhos.push_back(*f1);
                filhos.push_back(*f2);
            }

            for(int i = 0;i<filhos.size();i++){
                populacao.push_back(filhos[i]);
            }
        }
    }

    static bool ordenacao(Cromossomo c1, Cromossomo c2){
        return c1.getValor() < c2.getValor();
    }

    void avaliacao(){
        sort(populacao.begin(), populacao.end(), ordenacao);
        cout<<endl<<"Avaliando população"<<endl;
        for(int i=0; i<=tam_crom;i++){
            cout<<populacao[i].getValor()<<" ";
        }

        int pop = populacao.size();
        for(int i = pop;i>=tam_crom;i--){
            populacao.pop_back();
        }
    }

};

int main()
{
    //srand(time(NULL));

    GA *ga = new GA();
    ga->selecao();
    ga->avaliacao();


}
