#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <algorithm>
#include <math.h>

#define MIN 0
#define MAX 50
#define tam_crom 20
#define tam_aux 40

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
        valor = v;
    }

    double funcao(double x){
        return pow(x,2);
    }

    void calcularFitness(){
        aptidao = funcao(valor);
    }

};

class GA{

public:
    vector<Cromossomo> populacao;

    GA(){
        cout<<"Gerando populacao inicial: " << endl;
        for(int i=0; i< tam_crom; i++){
            Cromossomo *c = new Cromossomo();
            cout << c->getValor() << " ";
            populacao.push_back(*c);
        }
    }

    void selecao(){

        vector<Cromossomo> ganhadores;
        int num_torneios = 1;
        cout << endl << "Selecionando mais aptos:" << endl;

        while(num_torneios <= tam_crom){

            int posicao1 = rand() % tam_crom;
            int posicao2 = rand() % tam_crom;

            if(posicao1 != posicao2){

                cout << "Torneio " << num_torneios << endl;
                cout << "Valores: " << populacao[posicao1].getValor() << " X " << populacao[posicao2].getValor() << endl;
                cout << "Aptidoes: " << populacao[posicao1].aptidao << " X " << populacao[posicao2].aptidao << endl;
                num_torneios++;

                if(populacao[posicao1].aptidao < populacao[posicao2].aptidao){
                    ganhadores.push_back(populacao[posicao1]);
                }else{
                    ganhadores.push_back(populacao[posicao2]);
                }
            }
        }

        cout<<"Ganhadores dos torneios:"<<endl;
        for(int i = 0; i < tam_crom; i++){
            cout << ganhadores[i].getValor() << " ";
        }
        populacao = ganhadores;
    }

    vector<Cromossomo> crossover(){

        vector<Cromossomo> filhos;

        double taxa_reproducao = ((double) rand()) / (double) RAND_MAX;

        int num_it = 0;
        while(num_it < tam_crom/2){

            double taxa_reproducao = ((double) rand()) / (double) RAND_MAX;
            int posicao1 = rand() % tam_crom;
            int posicao2 = rand() % tam_crom;

            if(posicao1 != posicao2){
                num_it++;
                if(taxa_reproducao < 0.8){
                    Cromossomo *f1 = new Cromossomo();
                    Cromossomo *f2 = new Cromossomo();

                    double beta = ((double) rand()) / (double) RAND_MAX;

                    double val_f1 = populacao[posicao1].getValor() + beta*(populacao[posicao2].getValor() - populacao[posicao1].getValor());
                    double val_f2 = populacao[posicao2].getValor() + beta*(populacao[posicao1].getValor() - populacao[posicao2].getValor());

                    f1->setValor(val_f1);
                    f1->calcularFitness();
                    filhos.push_back(*f1);

                    f2->setValor(val_f2);
                    f2->calcularFitness();
                    filhos.push_back(*f2);
                }
            }
        }

        return filhos;
    }

    void mutacao(vector<Cromossomo> filhos){
        double taxa_mutacao = ((double) rand()) / (double) RAND_MAX;

        if(taxa_mutacao <= 0.015){
            int pos_crom = rand() % tam_crom;
            double valor_mutacao = filhos[pos_crom].getValor() - 0.1;

            cout << "Mutacao do cromossomo " << pos_crom << endl;
            filhos[pos_crom].setValor(valor_mutacao);
            filhos[pos_crom].calcularFitness();

        }

        for(int i = 0; i < filhos.size(); i++){
            populacao.push_back(filhos[i]);
        }
    }

    static bool ordenacao(Cromossomo c1, Cromossomo c2){
        return c1.getValor() < c2.getValor();
    }

    void avaliacao(){
        sort(populacao.begin(), populacao.end(), ordenacao);

        cout << endl << "Avaliando populacao" << endl;
        for(int i=0; i<=tam_crom;i++){
            cout << populacao[i].getValor()<<" ";
        }

        int pop = populacao.size();
        for(int i = pop; i >= tam_crom; i--){
            populacao.pop_back();
        }
    }


    double mediaDaPop(){
        double total = 0;
        for(int i=0; i<tam_crom;i++){
            total += populacao[i].getValor();
        }

        return total/tam_crom;
    }

    double varianciaDaPop(){
        double media = mediaDaPop();
        double variancia = 0;

        for(int i=0; i<tam_crom; i++){
            variancia+= pow((populacao[i].getValor() - media),2);
        }
        return variancia;
    }

    double desvioPadraoPop(){ //Valor alto: pontos espalhados; Valor baixo: pontos concentrados em torno da média
        double variancia = varianciaDaPop();
        double desvioPadr = sqrt(variancia);
        return desvioPadr;
    }
};

int main()
{
    srand(time(NULL));

    /*clock_t Ticks[3];
    Ticks[0] = clock();
    double tempo = 0;*/


    GA *ga = new GA();


    int num_geracoes = 0;
    double desvioPadrao = ga->desvioPadraoPop();


    while(desvioPadrao > 0.01){
        ga->selecao();
        vector<Cromossomo> filhos_aux;
        filhos_aux = ga->crossover();
        ga->mutacao(filhos_aux);
        ga->avaliacao();

        num_geracoes++;
        desvioPadrao = ga->desvioPadraoPop();

        //Ticks[1] = clock();
        //tempo = (Ticks[1] - Ticks[0] * 1000.0/CLOCKS_PER_SEC);
    }

    cout << endl << endl <<"Desvio Padrao: "<< desvioPadrao << endl;

    sort(ga->populacao.begin(), ga->populacao.end(), ga->ordenacao);

    for(int i = 0; i<tam_crom;i++){
        cout << ga->populacao[i].getValor() << " ";
    }

    cout << endl;
    cout << "Melhor resultado encontrado: " << ga->populacao.front().getValor() << endl;

    /*Ticks[2] = clock();
    double tempoFinal = (Ticks[2] - Ticks[0] * 1000.0/CLOCKS_PER_SEC);
    cout << "O tempo final: " << tempoFinal <<endl;
    */
}
