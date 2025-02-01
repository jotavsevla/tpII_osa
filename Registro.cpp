#include "Registro.h"

Registro::Registro(const string& n, const string& s,
                   const string& t, const string& d)
        : nome(n), sobrenome(s), telefone(t), dataNascimento(d) {}

string Registro::getNome() const {
    return nome;
}

string Registro::getSobrenome() const {
    return sobrenome;
}

string Registro::getTelefone() const {
    return telefone;
}

string Registro::getDataNascimento() const {
    return dataNascimento;
}

void Registro::setNome(const string& n) {
    nome = n;
}

void Registro::setSobrenome(const string& s) {
    sobrenome = s;
}

void Registro::setTelefone(const string& t) {
    telefone = t;
}

void Registro::setDataNascimento(const string& d) {
    dataNascimento = d;
}

string Registro::packDelimitado() const {
    return nome + "|" + sobrenome + "|" + telefone + "|" + dataNascimento + "\n";
}

void Registro::unpackDelimitado(const string& str) {
    stringstream ss(str);
    getline(ss, nome, '|');
    getline(ss, sobrenome, '|');
    getline(ss, telefone, '|');
    getline(ss, dataNascimento);
}

string Registro::packDescritor() const {
    string dados = nome + "|" + sobrenome + "|" + telefone + "|" + dataNascimento;
    int tamanho = dados.length() + sizeof(int);

    string buffer;
    buffer.resize(sizeof(int));
    memcpy(&buffer[0], &tamanho, sizeof(int));
    buffer += dados;

    return buffer;
}

bool Registro::unpackDescritor(const string& buffer) {
    if (buffer.size() < sizeof(int)) return false;

    int tamanho;
    memcpy(&tamanho, buffer.data(), sizeof(int));

    string dados = buffer.substr(sizeof(int));
    stringstream ss(dados);

    getline(ss, nome, '|');
    getline(ss, sobrenome, '|');
    getline(ss, telefone, '|');
    getline(ss, dataNascimento);

    return true;
}

ostream& operator<<(ostream& os, const Registro& reg) {
    os << "Nome: " << reg.nome
       << ", Sobrenome: " << reg.sobrenome
       << ", Telefone: " << reg.telefone
       << ", Data Nascimento: " << reg.dataNascimento;
    return os;
}
