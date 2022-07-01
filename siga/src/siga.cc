#include <string>
#include <sstream>
#include <vector>
#include <string.h>

using namespace std;

#include "siga.h"
#include "sort.h"


Siga::Siga(string arquivo_dados_estudante)
{

    this->arquivo_nome = arquivo_dados_estudante;
    this->file_stream.open(this->arquivo_nome, ios::in | ios::out | ios::binary );

    if(this->file_stream.is_open())
    {
        cout << "SIGA: Inicializado com sucesso" << endl;
    }
    else
    {
        cout << "SIGA: Erro ao abrir arquivo" << endl;
        return; 
    }

    cout << this->ObterNumeroEstudantesArmazenados() << " registros de estudantes" << endl;
}


int  Siga::PesquisaPorMatricula(int matricula)
{
    //Posicionando o cursor no inicio do arquivo
    this->file_stream.seekg(0, this->file_stream.beg);

    //Declarando Estudante
    Estudante est = Estudante();

    for ( int i = 0; i < this->n_estudantes; i++){
        //Ler do arquivo binário
        est.LerDoArquivoBinario(this->file_stream);
        //Verifica se a matricula é a mesma
        if ( est.ObterMatricula() == matricula )
            return i;
    }

    //Retornando o cursor pro fim de leitura
    this->file_stream.seekg(0, this->file_stream.end);

    //Caso não encontrar a matrícula retorna -1
    return -1;
}
        
void Siga::CadastraEstudante(Estudante est)
{
    //Verifica se est já está cadastrado
    if( PesquisaPorMatricula(est.ObterMatricula() ) != -1 )
        return;

    //Colocando o Cursor no Fim
    this->file_stream.seekp(0, this->file_stream.end);

    //Adicionando o aluno
    est.EscreverNoArquivoBinario(this->file_stream);

    //Incrementando o número de estudantes
    this->n_estudantes = this->n_estudantes + 1;

}
        
void Siga::ImprimeEstudantePorMatricula(int matricula)
{
    int index = PesquisaPorMatricula(matricula);
    if( index == -1){
        cout << "Estudante não cadastrado" << endl;
    } else {
        this->file_stream.seekg((index * ESTUDANTE_SIZE), this->file_stream.beg );
        Estudante est = Estudante();

        est.LerDoArquivoBinario(this->file_stream);

        est.Imprimir();
    }
}
        
void Siga::SalvaListaEstudanteEmTexto(string arquivo_txt)
{
    //Cursor no inicio
    this->file_stream.seekg(0, this->file_stream.beg);

    //Verifica se o arquivo está vazio
    if(this->n_estudantes == 0)
        return;

    //Abre o arquivo
    fstream arq;
    arq.open(arquivo_txt, ios::in | ios::out | ios::trunc);
    if(!arq.is_open()){
        cout << "FALHA AO ABRIR O ARQUIVO" << endl;
        return;
    }

    //Cria um vetor de alunos
    Estudante estudante;
        
    //Salva a primeira linha
    arq << "Matricula;Nome;Ano de Ingresso;Curso;IRA" << endl;

    //Le o vetor de Estudantes
    for( int i = 0; i < this->n_estudantes; i++){
        estudante.LerDoArquivoBinario(this->file_stream);
        arq << estudante.ObterMatricula() << ";";
        arq << estudante.ObterNome() << ";";
        arq << estudante.ObterAnoIngresso() << ";";
        arq << estudante.ObterCurso() << ";";
        arq << estudante.ObterIRA() << endl;

    }

    //Fechando o arquivo
    arq.close();


}
        
        
void Siga::AlteraCadastroEstudante(Estudante est)
{
    //Pesquisa o aluno
    int index = PesquisaPorMatricula( est.ObterMatricula() );

    //Caso nao encontrado imprime e retorna
    if ( index == -1 ){
        cout << "Estudante não cadastrado" << endl;
        return;
    }

    //Coloca o cursor no estudante que será reescrito
    this->file_stream.seekp(index * ESTUDANTE_SIZE, this->file_stream.beg);

    //Reescreve os dados dele
    est.EscreverNoArquivoBinario(this->file_stream);

    cout << "Dados Alterados" << endl;
}
        
Siga::~Siga()
{
    this->file_stream.close();
}

int Siga::ObterNumeroEstudantesArmazenados()
{
    if ( this->file_stream.is_open() ){
        this->file_stream.seekg(0, this->file_stream.end);
        int length = this->file_stream.tellg();
        this->file_stream.seekg(0, this->file_stream.beg);

        this->n_estudantes = length / ESTUDANTE_SIZE;

        return this->n_estudantes;
    }else {
        return 0;
    }
}


void Siga::ImportCSVData(string file)
{
    cout << "Importando dados do arquivo " << file << endl;
    int n_importados = this->n_estudantes; 

    // TODO: 
    // Abra um arquivo .csv com a seguinte formatação:
    // Matricula;Nome;Ano de Ingresso;Curso;IRA
    // Para cada linha do arquivo, criar um objeto Estudante e escrever no arquivo binário.

    //cria e abre um arquivo .csv para leitura:
    ifstream arq;
    arq.open(file, ios::in);
    //testa se a abertura do arquivo funcionou:
    if(!arq.is_open()){
        cout << "A abertura do arquivo falhou!" << endl;
        return;
    }
    string linha;
    //lê a primeira linha do arquivo (o cabeçalho):
    getline(arq, linha);
    //lê todas as linhas do arquivo, uma por uma, e armazena na string "linha":
    while(getline(arq, linha)){
        stringstream ss(linha);
        string ficha;
        //cria um vetor de strings sem tamanho pré-estabelecido:
        vector<string> fichas;
        //pega cada criterio (matricula, nome, curso...) da linha, separados por ";", e armazena na string "ficha":
        while(getline(ss, ficha, ';')){
            //adiciona cada critério no final do vetor "fichas" criado toda vez que uma nova linha é lida:
            fichas.push_back(ficha);
        }
        //o vetor "fichas" deve sair com o seguinte formato: 
        //0:Matricula  1:Nome  2:Ano de Ingresso  3:Curso  4:IRA
        //verifica se o vetor "fichas" tem o tamanho correto, com os 5 criterios necessarios:
        if(fichas.size() == 5){
            //cria um objeto Estudante:
            //a função "stoi" transforma string em int 
            //a função "stof" transforma string em float
            Estudante est(fichas[1].c_str(), stoi(fichas[0]), stoi(fichas[2]), stoi(fichas[3]), stof(fichas[4]));
            //cadastra o estudante criado, escrevendo-o no arquivo binário:
            this->CadastraEstudante(est);
        }
    }

    // Atualize o numero de registros no binário.
    n_importados = this->n_estudantes - n_importados;
    // Imprima o numero de estudantes importados:
    cout << "Importacao concluida: " << n_importados << " novos alunos cadastrados" << endl;
}

Estudante* Siga::arquivoParaVetor()
{
    //essa função lê todos os dados do arquivo binário e retorna um vetor de Estudantes
    //inicializa array de Estudantes com o numero de estudantes contidos no arquivo:
    Estudante* vetor_est = new Estudante[this->n_estudantes];
    //posiciona o cursor no inicio do arquivo:
    this->file_stream.seekg(0, ios::beg);
    //inicializa ponteiro para Estudante:
    Estudante *est = new Estudante();
    //inicializa um contador:
    int i=0;
    while(!this->file_stream.eof()){
        //lê os dados do estudante e armazena na variavel est:
        est->LerDoArquivoBinario(this->file_stream);
        //verifica se chegou no final do arquivo:
        if(this->file_stream.eof()){
            break;
        }
        //adiciona o Estudante no final do vetor:
        vetor_est[i] = *est;
        //incrementa o contador:
        i++;
    }
    //libera o ponteiro:
    delete est;
    //retorna o vetor de estudantes:
    return vetor_est;
}

bool comparaPorNome(Estudante &est1, Estudante &est2)
{
    //a função "strcmp" retorna valor menor que 0 se o primeiro caractere que não corresponde entre as strings
    //tem um valor menor em est1 do que em est2, ou seja, est1 vem antes na ordem alfabética
    // se retornar TRUE, significa que est1 vem antes de est2:
    return strcmp(est1.ObterNome(), est2.ObterNome()) < 0;
}

bool comparaPorIra(Estudante &est1, Estudante &est2)
{
    // se retornar TRUE, significa que est1 tem IRA maior do que est2, ou seja, vem antes:
    return est1.ObterIRA() > est2.ObterIRA();
}

bool comparaPorCurso(Estudante &est1, Estudante &est2)
{
    // se retornar TRUE, significa que o curso de est1 vem antes que o curso de est2:
    return est1.ObterCurso() < est2.ObterCurso();
}

void Siga::SalvarListaOrdendaEstudantesPorNome(string arquivo_txt, sorting_method method)
{
    // TODO:
    // Iremos aplicar a ordenação na memoria, para isso faça:
    // 1. Ler todos os dados do arquivo binário colocados em um vetor

    //cria um ponteiro vetor de Estudantes com o conteudo do arquivo binario:
    Estudante* vetor_est = arquivoParaVetor();

    // 2. Ordenar o vetor usando o metodo de ordenação escolhido:
    //    - Ordenar por nome
    switch(method){
        case BUBBLESORT:
            // Ordenação por nome
            bubble_sort<Estudante>(vetor_est, this->n_estudantes, comparaPorNome);
            break;
        case INSERTIONSORT:
            // Ordenação por nome
            insert_sort<Estudante>(vetor_est, this->n_estudantes, comparaPorNome);
            break;
        case SELECTIONSORT:
            // Ordenação por nome
            selection_sort<Estudante>(vetor_est, this->n_estudantes, comparaPorNome);
            break;
        default:
            cout << "metodo de ordenação não encontrado" << endl;
            break;
    }

    // 3. Escrever o vetor ordenado no arquivo csv:
    //    matricula;nome;ano de ingresso;curso;IRA

    //cria e abre um arquivo .csv para escrita:
    ofstream arq;
    arq.open(arquivo_txt, ios::out);
    //testa se a abertura do arquivo funcionou:
    if(!arq.is_open()){
        cout << "A abertura do arquivo falhou!" << endl;
        return;
    }
    //escreve todos os estudantes do vetor ordenado no arquivo:
    for(int i=0; i<this->n_estudantes; i++){
        arq << vetor_est[i].ObterMatricula() << ";";
        arq << vetor_est[i].ObterNome() << ";";
        arq << vetor_est[i].ObterAnoIngresso() << ";";
        arq << vetor_est[i].ObterCurso() << ";";
        arq << vetor_est[i].ObterIRA() << endl;
    }
    //fecha o arquivo:
    arq.close();
    //libera o ponteiro do vetor:
    delete[] vetor_est;
}

void Siga::SalvarListaOrdenadaEstudantes(std::string arquivo_txt)
{
    // TODO: 
    // Iremos aplicar a ordenação na memoria, para isso faca:
    // 1. Ler todos os dados do arquivo binário colocandos em um vetor

    //cria um ponteiro vetor de Estudantes com o conteudo do arquivo binario:
    Estudante* vetor_est = arquivoParaVetor();
    
    // 2. Ordenar o vetor usando a seguinte sequencia:
    //    - Ordenar por nome 
    //    - Ordenar por curso
    // Observação: escolha metodos que sejam estaveis na segunda chamada de ordenação.

    //utiliza o método de ordenação por inserção, que é estável e é considerado o método mais rárpido dentre os 3 métodos disponiveis:
    //ordena por nome:
    insert_sort<Estudante>(vetor_est, this->n_estudantes, comparaPorNome);
    //ordena por curso:
    insert_sort<Estudante>(vetor_est, this->n_estudantes, comparaPorCurso);


    // 3. Escrever o vetor ordenado no arquivo csv 
    //    matricula;nome;ano de ingresso;curso;IRA

    //cria e abre um arquivo .csv para escrita:
    ofstream arq;
    arq.open(arquivo_txt, ios::out);
    //testa se a abertura do arquivo funcionou:
    if(!arq.is_open()){
        cout << "A abertura do arquivo falhou!" << endl;
        return;
    }
    //escreve todos os estudantes do vetor ordenado no arquivo:
    for(int i=0; i<this->n_estudantes; i++){
        arq << vetor_est[i].ObterMatricula() << ";";
        arq << vetor_est[i].ObterNome() << ";";
        arq << vetor_est[i].ObterAnoIngresso() << ";";
        arq << vetor_est[i].ObterCurso() << ";";
        arq << vetor_est[i].ObterIRA() << endl;
    }
    //fecha o arquivo:
    arq.close();
    //libera o ponteiro do vetor:
    delete[] vetor_est;

}

