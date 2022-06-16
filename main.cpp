#include <iostream>
#include <string>
#include <fstream>

#define INT_MAX 2147483647
#define ARQUIVO "P-n51-k10.txt"

using namespace std;

/*

1: P-n16-k8.txt
2: P-n19-k2.txt
3: P-n20-k2.txt
4: P-n23-k8.txt
5: P-n45-k5.txt
6: P-n50-k10.txt
7: P-n51-k10.txt
8: P-n55-k7.txt

*/

void LeituraArquivo();
int CalculaDistancia();
void VizinhoMProx();
void Swap();
void DoisOpt();
void Reinsertion();
void VND();

typedef struct Route
{

    int *percurso;
    int distanciaPercurso = 0;
    int tamanhoPercurso = 0;
    bool foiUsada;

} Route;

int dimensao, capacidade;

int *demanda;
int **matrizDistancia;

Route *rotas;
int indiceRota = 0;

bool melhorou = false;

int main()
{

    LeituraArquivo();

    VizinhoMProx();

    VND();

    cout << "Solucao = " << CalculaDistancia() << endl;

    return 0;
}

void LeituraArquivo()
{
    int i, j;

    ifstream arq;
    arq.open(ARQUIVO);
    arq >> dimensao;
    arq >> capacidade;

    demanda = (int *)malloc(dimensao * sizeof(int));

    for (i = 0; i < dimensao; i++)
    {

        arq >> demanda[i];
    }

    //criacao do array bidimensional
    matrizDistancia = (int **)malloc(dimensao * sizeof(int));
    for (i = 0; i < dimensao; i++)
    {
        matrizDistancia[i] = (int *)malloc(dimensao * sizeof(int));
    }

    for (j = 0; j < dimensao; j++)
    {
        for (i = 0; i < dimensao; i++)
        {
            arq >> matrizDistancia[i][j];
        }
    }

    arq.close();
}

int CalculaDistancia()
{
    int soma = 0;
    int i = 0;

    while (rotas[i].foiUsada == true)
    {
        soma += rotas[i].distanciaPercurso;
        i++;
    }

    return soma;
}

void VizinhoMProx()
{
    int i, j;
    int vaiGalpao = 0;
    int casa_atual, casa_minimo, dist_minimo = INT_MAX;
    int capacidade_atual;
    int soma = 0;
    int cidades_visitadas = 0;

    int auxiliarPercurso[dimensao];
    auxiliarPercurso[0] = 0;
    int contAuxPercurso = 1;

    rotas = (Route *)malloc(dimensao * sizeof(Route));

    for (i = 0; i < dimensao; i++)
    {
        rotas[i].foiUsada = false;
    }

    bool jaPassou[dimensao];

    for (i = 0; i < dimensao; i++)
        jaPassou[i] = false;

    casa_atual = 0;
    jaPassou[0] = true;
    capacidade_atual = capacidade;

    while (1)
    {

        vaiGalpao = 1;

        for (i = 0; i < dimensao; i++)
        {

            if (casa_atual != i)
            {
                if (jaPassou[i] == false)
                {
                    if (demanda[i] <= capacidade_atual)
                    {
                        if (matrizDistancia[casa_atual][i] < dist_minimo)
                        {
                            casa_minimo = i;                              //6
                            dist_minimo = matrizDistancia[casa_atual][i]; //12
                            vaiGalpao = 0;

                        } //menor distancia
                    }     //tem espaco
                }         //se ja passou
            }             //lugar igual
        }

        if (vaiGalpao == 0)
        {

            jaPassou[casa_minimo] = true;
            cidades_visitadas++;
            auxiliarPercurso[contAuxPercurso] = casa_minimo;
            contAuxPercurso++;

            soma = soma + matrizDistancia[casa_atual][casa_minimo];
            casa_atual = casa_minimo;
            capacidade_atual = capacidade_atual - demanda[casa_atual];
            dist_minimo = INT_MAX;
        }
        else
        {

            soma = soma + matrizDistancia[casa_atual][0];
            casa_atual = 0;
            capacidade_atual = capacidade;
            dist_minimo = INT_MAX;

            rotas[indiceRota].percurso = (int *)malloc((contAuxPercurso + 1) * sizeof(int));

            for (i = 0; i < contAuxPercurso; i++)
            {

                rotas[indiceRota].percurso[i] = auxiliarPercurso[i];
            }

            rotas[indiceRota].percurso[contAuxPercurso] = 0;

            rotas[indiceRota].distanciaPercurso = soma;
            rotas[indiceRota].tamanhoPercurso = contAuxPercurso;
            rotas[indiceRota].foiUsada = true;

            indiceRota++;
            contAuxPercurso = 1;
            soma = 0;
            if (cidades_visitadas == dimensao - 1)
                break;
        }
    }
}

void Swap()
{

    int i = 0, j = 0, k = 0, maiorDiferenca = 1, indiceiMin = 0, indicejMin = 0, indicekMin = 0, novoPercurso = 0, aux;

    //k = indice de rotas
    //j = indice de dimensao 1
    //i = indice de dimensao 2

    melhorou = false;

    while (rotas[k].foiUsada == true)
    {
        if (rotas[k].tamanhoPercurso >= 4) //tamanho minimo pra entrar no swap
        {

            for (i = 1; i < rotas[k].tamanhoPercurso - 1; i++)
            {
                for (j = i + 1; j < rotas[k].tamanhoPercurso; j++)
                {
                    novoPercurso = 0;

                    if (j != i + 1)
                    {
                        novoPercurso = (rotas[k].distanciaPercurso) - (matrizDistancia[rotas[k].percurso[i - 1]][rotas[k].percurso[i]]) - (matrizDistancia[rotas[k].percurso[i]][rotas[k].percurso[i + 1]]) - (matrizDistancia[rotas[k].percurso[j - 1]][rotas[k].percurso[j]]) - (matrizDistancia[rotas[k].percurso[j]][rotas[k].percurso[j + 1]]) + (matrizDistancia[rotas[k].percurso[i - 1]][rotas[k].percurso[j]]) + (matrizDistancia[rotas[k].percurso[j]][rotas[k].percurso[i + 1]]) + (matrizDistancia[rotas[k].percurso[j - 1]][rotas[k].percurso[i]]) + (matrizDistancia[rotas[k].percurso[i]][rotas[k].percurso[j + 1]]);
                    }
                    else
                    {
                        novoPercurso = (rotas[k].distanciaPercurso) - (matrizDistancia[rotas[k].percurso[i - 1]][rotas[k].percurso[i]]) - (matrizDistancia[rotas[k].percurso[j]][rotas[k].percurso[j + 1]]) + (matrizDistancia[rotas[k].percurso[i - 1]][rotas[k].percurso[j]]) + (matrizDistancia[rotas[k].percurso[i]][rotas[k].percurso[j + 1]]);
                    }

                    if (novoPercurso < rotas[k].distanciaPercurso) //se o novo percurso for melhor
                    {

                        if ((rotas[k].distanciaPercurso - novoPercurso) > maiorDiferenca) // quanto maior a diferenca, melhor a rota
                        {

                            maiorDiferenca = rotas[k].distanciaPercurso - novoPercurso;
                            melhorou = true;
                            indiceiMin = i;
                            indicejMin = j;
                            indicekMin = k;
                        }
                    }
                }
            }
        }
        k++;
    }

    if (melhorou == true)
    {
        rotas[indicekMin].distanciaPercurso -= maiorDiferenca;
        aux = rotas[indicekMin].percurso[indicejMin];
        rotas[indicekMin].percurso[indicejMin] = rotas[indicekMin].percurso[indiceiMin];
        rotas[indicekMin].percurso[indiceiMin] = aux;
    }
}

void DoisOpt()
{

    int i = 0, j = 0, k = 0;
    int novaDist = 0;
    int indiceiMin, indicejMin, indicekMin, maiorDiferenca = 1, aux;
    int nElementos = 0, nTrocas = 0;

    melhorou = false;

    while (rotas[k].foiUsada == true)
    {

        if (rotas[k].tamanhoPercurso >= 5)
        {

            for (i = 1; i < rotas[k].tamanhoPercurso - 3; i++)
            {
                for (j = i + 3; j < rotas[k].tamanhoPercurso; j++)
                {
                    novaDist = (rotas[k].distanciaPercurso) - (matrizDistancia[rotas[k].percurso[i - 1]][rotas[k].percurso[i]]) - (matrizDistancia[rotas[k].percurso[j]][rotas[k].percurso[j + 1]]) + (matrizDistancia[rotas[k].percurso[i]][rotas[k].percurso[j + 1]]) + (matrizDistancia[rotas[k].percurso[i - 1]][rotas[k].percurso[j]]);

                    if (novaDist < rotas[k].distanciaPercurso) //se o novo percurso for melhor
                    {
                        if ((rotas[k].distanciaPercurso - novaDist) > maiorDiferenca) // quanto maior a diferenca, melhor a rota
                        {
                            melhorou = true;
                            maiorDiferenca = rotas[k].distanciaPercurso - novaDist;
                            indiceiMin = i;
                            indicejMin = j;
                            indicekMin = k;
                        }
                    }
                }
            }
        }
        k++;
    }

    if (melhorou == true)
    {

        rotas[indicekMin].distanciaPercurso -= maiorDiferenca;

        nElementos = (indicejMin - indiceiMin) + 1;
        nTrocas = nElementos / 2;
        while (nTrocas)
        {

            aux = rotas[indicekMin].percurso[indicejMin];
            rotas[indicekMin].percurso[indicejMin] = rotas[indicekMin].percurso[indiceiMin];
            rotas[indicekMin].percurso[indiceiMin] = aux;

            indiceiMin++;
            indicejMin--;
            nTrocas--;
        }
    }
}

void Reinsertion()
{

    int i = 0, j = 0, k = 0;
    int novaDist = 0;
    int indiceiMin, indicejMin, indicekMin, maiorDiferenca = 1, aux;

    melhorou = false;

    while (rotas[k].foiUsada == true)
    {

        if (rotas[k].tamanhoPercurso >= 5)
        {

            for (i = 1; i < rotas[k].tamanhoPercurso; i++)
            {

                for (j = 1; j < rotas[k].tamanhoPercurso; j++)
                {
                    if (i == j || i == j - 1 || i == j + 1)
                    {
                        continue;
                    }

                    novaDist = rotas[k].distanciaPercurso - matrizDistancia[rotas[k].percurso[i - 1]][rotas[k].percurso[i]] - matrizDistancia[rotas[k].percurso[i]][rotas[k].percurso[i + 1]] - matrizDistancia[rotas[k].percurso[j]][rotas[k].percurso[j + 1]] + matrizDistancia[rotas[k].percurso[i - 1]][rotas[k].percurso[i + 1]] + matrizDistancia[rotas[k].percurso[j]][rotas[k].percurso[i]] + matrizDistancia[rotas[k].percurso[i]][rotas[k].percurso[j + 1]];

                    if (novaDist < rotas[k].distanciaPercurso) //se o novo percurso for melhor
                    {

                        if ((rotas[k].distanciaPercurso - novaDist) > maiorDiferenca) // quanto maior a diferenca, melhor a rota
                        {

                            melhorou = true;
                            maiorDiferenca = rotas[k].distanciaPercurso - novaDist;
                            indiceiMin = i;
                            indicejMin = j;
                            indicekMin = k;
                        }
                    }
                }
            }
        }
        k++;
    }

    if (melhorou == true)
    {

        rotas[indicekMin].distanciaPercurso -= maiorDiferenca;

        aux = rotas[indicekMin].percurso[indicejMin];
        rotas[indicekMin].percurso[indicejMin] = rotas[indicekMin].percurso[indiceiMin];

        if (indicejMin < indiceiMin)
        {

            while (indiceiMin > (indicejMin + 1))
            {
                rotas[indicekMin].percurso[indiceiMin] = rotas[indicekMin].percurso[indiceiMin - 1];
                indiceiMin--;
            }
            rotas[indicekMin].percurso[indicejMin + 1] = aux;
        }
        if (indicejMin > indiceiMin)
        {

            while (indiceiMin < (indicejMin - 1))
            {
                rotas[indicekMin].percurso[indiceiMin] = rotas[indicekMin].percurso[indiceiMin + 1];
                indiceiMin++;
            }
            rotas[indicekMin].percurso[indicejMin - 1] = aux;
        }
    }
}

void VND()
{
    int k = 1;

    while (k <= 3)
    {

        switch (k)
        {

        case 1:
            Swap();
            break;
        case 2:
            DoisOpt();
            break;
        case 3:
            Reinsertion();
            break;
        }

        if (melhorou)
        {
            k = 1;
        }
        else
        {
            k++;
        }
    }
}
