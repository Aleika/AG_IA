#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <algorithm>
#include <math.h>

#define MIN 0
#define MAX 100
#define tam_crom 20

using namespace std;

class Cromossomo{

private:
    double valor;
public:
    double aptidao;

    Cromossomo(){
        valor = MIN + static_cast <double> (rand()) /( static_cast <double> (RAND_MAX/(MAX-MIN)));
        calcularFitness();
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
            //c->calcularFitness();
            populacao.push_back(*c);
            //populacao[i].calcularFitness();
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

        if(taxa_mutacao <= 0.01){
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

double mediaDaPop(vector<Cromossomo> populacao){
    double total = 0;
    for(int i=0; i<tam_crom;i++){
        total+=populacao[i].getValor();
    }

    return total/tam_crom;
}

double varianciaDaPop(vector<Cromossomo> populacao){
    double media = mediaDaPop(populacao);
    double variancia = 0;

    for(int i=0; i<tam_crom; i++){
        variancia+= pow((populacao[i].getValor() - media),2);
    }
    return variancia;
}

double desvioPadraoPop(vector<Cromossomo> populacao){ //Valor alto: pontos espalhados; Valor baixo: pontos concentrados em torno da média
    double media = mediaDaPop(populacao);
    double variancia = varianciaDaPop(populacao);
    double desvioPadr = sqrt((1.0/(tam_crom-1))*variancia);
    return desvioPadr;
}

int main()
{
    srand(time(NULL));

    clock_t Ticks[3];
    Ticks[0] = clock();

    GA *ga = new GA();
    int num_geracoes = 0;
    double desvioPadrao = desvioPadraoPop(ga->populacao);

    double tempo = 0;
    while(tempo<=10000 && num_geracoes < 250 && desvioPadrao > 0.1){
        ga->selecao();
        ga->crossover();
        ga->mutacao();
        ga->avaliacao();

        num_geracoes++;
        cout << endl << endl <<"Desvio Padrão: "<< desvioPadrao << endl;
        desvioPadrao = desvioPadraoPop(ga->populacao);

        Ticks[1] = clock();
        tempo = (Ticks[1] - Ticks[0] * 1000.0/CLOCKS_PER_SEC);
    }

    sort(ga->populacao.begin(), ga->populacao.end(), ga->ordenacao);

    /*for(int i = 0;i< tam_crom;i++){
        cout << ga->populacao[i].getValor() << endl;
    }*/

    cout << endl;
    cout << "Melhor resultado encontrado: " << ga->populacao.front().getValor() << endl;

    Ticks[2] = clock();
    double tempoFinal = (Ticks[2] - Ticks[0] * 1000.0/CLOCKS_PER_SEC);
    cout << " o tempo final: " << tempoFinal <<endl;

}
