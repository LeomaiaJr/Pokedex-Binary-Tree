#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <locale.h>

using namespace std;

struct Pokemon
{
    string nome;
    string tipo;
    int numero;
    string pokebola;

    Pokemon *esquerda;
    Pokemon *direita;

    Pokemon(const string &nome, const string &tipo, int numero, const string &pokebola)
        : nome(nome), tipo(tipo), numero(numero), pokebola(pokebola), esquerda(NULL), direita(NULL)
    {
    }
};

class Pokedex
{
private:
    Pokemon *raiz;

    void inserirPokemon(Pokemon *&raiz, const Pokemon &pokemon)
    {
        if (raiz == NULL)
        {
            raiz = new Pokemon(pokemon.nome, pokemon.tipo, pokemon.numero, pokemon.pokebola);
        }
        else
        {
            if (pokemon.nome < raiz->nome)
            {
                inserirPokemon(raiz->esquerda, pokemon);
            }
            else
            {
                inserirPokemon(raiz->direita, pokemon);
            }
        }
    }

    bool verificarPokemon(const Pokemon *raiz, const string &nome)
    {
        if (raiz == NULL)
        {
            return false;
        }
        else if (raiz->nome == nome)
        {
            return true;
        }
        else if (nome < raiz->nome)
        {
            return verificarPokemon(raiz->esquerda, nome);
        }
        else
        {
            return verificarPokemon(raiz->direita, nome);
        }
    }

    void imprimirEmOrdem(const Pokemon *raiz)
    {
        if (raiz != NULL)
        {
            imprimirEmOrdem(raiz->esquerda);
            cout << "Nome: " << raiz->nome << ", Tipo: " << raiz->tipo << ", Numero: " << raiz->numero << ", Pokebola: " << raiz->pokebola << endl;
            imprimirEmOrdem(raiz->direita);
        }
    }

    int contarPorTipo(const Pokemon *raiz, const string &tipo)
    {
        if (raiz == NULL)
        {
            return 0;
        }

        int count = contarPorTipo(raiz->esquerda, tipo) + contarPorTipo(raiz->direita, tipo);
        if (raiz->tipo == tipo)
        {
            count++;
        }
        return count;
    }

    Pokemon *removerPokemon(Pokemon *raiz, const string &pokebola)
    {
        if (raiz == NULL)
        {
            return NULL;
        }

        if (pokebola < raiz->pokebola)
        {
            raiz->esquerda = removerPokemon(raiz->esquerda, pokebola);
        }
        else if (pokebola > raiz->pokebola)
        {
            raiz->direita = removerPokemon(raiz->direita, pokebola);
        }
        else
        {
            if (raiz->esquerda == NULL)
            {
                Pokemon *temp = raiz->direita;
                delete raiz;
                return temp;
            }
            else if (raiz->direita == NULL)
            {
                Pokemon *temp = raiz->esquerda;
                delete raiz;
                return temp;
            }

            Pokemon *temp = encontrarMenor(raiz->direita);
            raiz->nome = temp->nome;
            raiz->tipo = temp->tipo;
            raiz->numero = temp->numero;
            raiz->pokebola = temp->pokebola;

            raiz->direita = removerPokemon(raiz->direita, temp->pokebola);
        }

        return raiz;
    }

    Pokemon *encontrarMenor(Pokemon *raiz)
    {
        Pokemon *atual = raiz;
        while (atual->esquerda != NULL)
        {
            atual = atual->esquerda;
        }
        return atual;
    }

    Pokemon *removerTodosPorPokebola(Pokemon *raiz, const string &pokebola)
    {
        if (raiz == NULL)
        {
            return NULL;
        }

        raiz->esquerda = removerTodosPorPokebola(raiz->esquerda, pokebola);
        raiz->direita = removerTodosPorPokebola(raiz->direita, pokebola);

        if (raiz->pokebola == pokebola)
        {
            if (raiz->esquerda == NULL)
            {
                Pokemon *temp = raiz->direita;
                delete raiz;
                return temp;
            }
            else if (raiz->direita == NULL)
            {
                Pokemon *temp = raiz->esquerda;
                delete raiz;
                return temp;
            }

            Pokemon *temp = encontrarMenor(raiz->direita);
            raiz->nome = temp->nome;
            raiz->tipo = temp->tipo;
            raiz->numero = temp->numero;
            raiz->pokebola = temp->pokebola;

            raiz->direita = removerPokemon(raiz->direita, temp->pokebola);
        }

        return raiz;
    }

    void imprimirPorPokebola(const Pokemon *raiz, const string &pokebola)
    {
        if (raiz != NULL)
        {
            imprimirPorPokebola(raiz->esquerda, pokebola);
            if (raiz->pokebola == pokebola)
            {
                cout << "Nome: " << raiz->nome << ", Tipo: " << raiz->tipo << ", Numero: " << raiz->numero << ", Pokebola: " << raiz->pokebola << endl;
            }
            imprimirPorPokebola(raiz->direita, pokebola);
        }
    }

    int encontrarPokemon(Pokemon *raiz, int numero, string &percurso)
    {
        if (raiz == NULL)
        {
            return -1;
        }

        if (raiz->numero == numero)
        {
            return raiz->numero;
        }
        else if (numero < raiz->numero)
        {
            percurso += "EmOrdem|"; // EmOrdem
            int result = encontrarPokemon(raiz->esquerda, numero, percurso);
            if (result != -1)
            {
                return result;
            }

            percurso += "PosOrdem|"; // PosOrdem
            result = encontrarPokemon(raiz->direita, numero, percurso);
            if (result != -1)
            {
                return result;
            }

            percurso += "PreOrdem|"; // PreOrdem
        }
        else
        {
            percurso += "PreOrdem|"; // PreOrdem
            int result = encontrarPokemon(raiz->direita, numero, percurso);
            if (result != -1)
            {
                return result;
            }

            percurso += "PosOrdem|"; // PosOrdem
            result = encontrarPokemon(raiz->esquerda, numero, percurso);
            if (result != -1)
            {
                return result;
            }

            percurso += "EmOrdem|"; // EmOrdem
        }

        return -1;
    }

public:
    Pokedex()
        : raiz(NULL)
    {
    }

    void inserirPokemon(const Pokemon &pokemon)
    {
        inserirPokemon(raiz, pokemon);
    }

    void verificarPokemon(const string &nome)
    {
        if (verificarPokemon(raiz, nome))
        {
            cout << "Pokemon encontrado!" << endl;
        }
        else
        {
            cout << "Pokemon nao encontrado!" << endl;
        }
    }

    int contarPorTipo(const string &tipo)
    {
        return contarPorTipo(raiz, tipo);
    }

    void removerTodosPorPokebola(const string &pokebola)
    {
        raiz = removerTodosPorPokebola(raiz, pokebola);
    }

    void imprimirPorPokebola(const string &pokebola)
    {
        imprimirPorPokebola(raiz, pokebola);
    }

    int encontrarPokemon(int numero, string &percurso)
    {
        return encontrarPokemon(raiz, numero, percurso);
    }

    void imprimirTodosEmOrdem()
    {
        imprimirEmOrdem(raiz);
    }

    void imprimirPorTipo(const Pokemon *raiz, const string &tipo)
    {
        if (raiz != NULL)
        {
            imprimirPorTipo(raiz->esquerda, tipo);
            if (raiz->tipo == tipo)
            {
                cout << "Nome: " << raiz->nome << ", Tipo: " << raiz->tipo << ", Numero: " << raiz->numero << ", Pokebola: " << raiz->pokebola << endl;
            }
            imprimirPorTipo(raiz->direita, tipo);
        }
    }

    void imprimirPorTipoOrdenado()
    {
        imprimirPorTipo(raiz, "Agua");
        imprimirPorTipo(raiz, "Fogo");
        imprimirPorTipo(raiz, "Normal");
    }
};

int main()
{

    setlocale(LC_ALL, "");

    Pokedex pokedex;
    string nome, tipo, pokebola;
    int numero;
    string percurso;
    int numeroEncontrado;
    int opcao = -1;

    while (opcao != 0)
    {
        cout << "--- POKEDEX MENU ---" << endl;
        cout << "Selecione uma opcao:" << endl;
        cout << "1. Inserir Pokemon" << endl;
        cout << "2. Imprimir todos os Pokemon em ordem" << endl;
        cout << "3. Imprimir todos os Pokemon ordenados por tipo" << endl;
        cout << "4. Verificar se um Pokemon existe" << endl;
        cout << "5. Contar Pokemon de um determinado tipo" << endl;
        cout << "6. Remover todos os Pokemon com uma determinada pokebola" << endl;
        cout << "7. Imprimir todos os Pokemon com uma determinada pokebola" << endl;
        cout << "8. Encontrar Pokemon pelo numero (descobrir percurso)" << endl;
        cout << "0. Sair" << endl;
        cout << "--- --- --- --- ---" << endl
             << endl;
        cin >> opcao;

        switch (opcao)
        {
        case 1:
            cout << "Digite o nome, tipo, numero e pokebola do Pokemon:" << endl;
            cin >> nome >> tipo >> numero >> pokebola;
            pokedex.inserirPokemon(Pokemon(nome, tipo, numero, pokebola));
            break;
        case 2:
            pokedex.imprimirTodosEmOrdem();
            break;
        case 3:
            pokedex.imprimirPorTipoOrdenado();
            break;
        case 4:
            cout << "Digite o nome do Pokemon que voce deseja verificar:" << endl;
            cin >> nome;
            pokedex.verificarPokemon(nome);
            break;
        case 5:
            cout << "Digite o tipo de Pokemon que voce deseja contar:" << endl;
            cin >> tipo;
            cout << "Quantidade de pokemons do tipo '" << tipo << "': " << pokedex.contarPorTipo(tipo) << endl;
            break;
        case 6:
            cout << "Digite o tipo de pokebola que voce deseja remover:" << endl;
            cin >> pokebola;
            pokedex.removerTodosPorPokebola(pokebola);
            break;
        case 7:
            cout << "Digite o tipo de pokebola para exibir os Pokemon:" << endl;
            cin >> pokebola;
            pokedex.imprimirPorPokebola(pokebola);
            break;
        case 8:
            cout << "Digite o nemero do Pokemon que voce deseja encontrar:" << endl;
            cin >> numero;
            percurso;
            numeroEncontrado = pokedex.encontrarPokemon(numero, percurso);
            if (numeroEncontrado != -1)
            {
                cout << "Pokemon encontrado com numero " << numeroEncontrado << " pelo percurso: |" << percurso << endl;
            }
            else
            {
                cout << "Pokemon nao encontrado." << endl;
            }
            break;
        default:
            cout << "Opcao invalida. Tente novamente." << endl;
            break;
        }
    }

    return 0;
}