#include <iostream>
#include <fstream>
#include <sstream>
#include "Registro.h"
#include "Buffer.h"

using namespace std;

// Função para verificar se o arquivo existe
bool arquivoExiste(const string& nomeArquivo) {
    ifstream arquivo(nomeArquivo);
    return arquivo.good();
}

// Função para remover espaços em branco no início e fim da string
string trim(const string& str) {
    size_t first = str.find_first_not_of(' ');
    if (string::npos == first) {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

int main() {
    string arquivoEntrada = "Dados.txt";
    string arquivoDelimitado = "registros_delimitados.txt";
    string arquivoDescritor = "registros_descritores.bin";

    // Verificar se o arquivo de entrada existe
    if (!arquivoExiste(arquivoEntrada)) {
        cerr << "Erro: Arquivo " << arquivoEntrada << " não encontrado!" << endl;
        return 1;
    }

    // Abrir arquivo de entrada
    ifstream arquivoDados(arquivoEntrada);
    if (!arquivoDados.is_open()) {
        cerr << "Erro ao abrir arquivo de entrada!" << endl;
        return 1;
    }

    // Criar buffers para os arquivos de saída
    Buffer bufferDelimitado(arquivoDelimitado);
    Buffer bufferDescritor(arquivoDescritor);

    // Abrir arquivos de saída
    if (!bufferDelimitado.abrirEscrita()) {
        cerr << "Erro ao criar arquivo delimitado!" << endl;
        return 1;
    }
    if (!bufferDescritor.abrirEscritaBinaria()) {
        cerr << "Erro ao criar arquivo com descritores!" << endl;
        return 1;
    }

    string linha;
    bool primeiraLinha = true;

    // Ler arquivo linha por linha
    while (getline(arquivoDados, linha)) {
        // Pular o cabeçalho
        if (primeiraLinha) {
            primeiraLinha = false;
            continue;
        }

        // Processar linha
        stringstream ss(linha);
        string nomeCompleto, telefone, nascimento;

        // Ler campos separados por |
        getline(ss, nomeCompleto, '|');
        getline(ss, telefone, '|');
        getline(ss, nascimento);

        // Separar nome e sobrenome
        size_t espacoPos = nomeCompleto.find_last_of(' ');
        string nome = trim(nomeCompleto.substr(0, espacoPos));
        string sobrenome = trim(nomeCompleto.substr(espacoPos + 1));

        // Remover possíveis espaços extras dos outros campos
        telefone = trim(telefone);
        nascimento = trim(nascimento);

        // Criar registro
        Registro reg(nome, sobrenome, telefone, nascimento);

        // Escrever nos dois formatos
        bufferDelimitado.escreverRegistroDelimitado(reg);
        bufferDescritor.escreverRegistroDescritor(reg);
    }

    // Fechar arquivo de entrada
    arquivoDados.close();
    bufferDelimitado.fechar();
    bufferDescritor.fechar();

    cout << "Conversão concluída!" << endl;

    // Demonstração de leitura dos arquivos convertidos
    cout << "\nLendo registros do arquivo delimitado:" << endl;
    if (bufferDelimitado.abrirLeitura()) {
        Registro reg;
        while (bufferDelimitado.lerRegistroDelimitado(reg)) {
            cout << reg << endl;
        }
        bufferDelimitado.fechar();
    }

    cout << "\nLendo registros do arquivo com descritores:" << endl;
    if (bufferDescritor.abrirLeituraBinaria()) {
        Registro reg;
        while (bufferDescritor.lerRegistroDescritor(reg)) {
            cout << reg << endl;
        }
        bufferDescritor.fechar();
    }

    return 0;
}
