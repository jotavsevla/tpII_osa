#ifndef REGISTRO_H
#define REGISTRO_H

#include <string>
#include <sstream>
#include <cstring>

using namespace std;

class Registro {
private:
    string nome;
    string sobrenome;
    string telefone;
    string dataNascimento;

public:
    // Construtores
    Registro() = default;
    Registro(const string& n, const string& s, const string& t, const string& d);

    // Getters
    string getNome() const;
    string getSobrenome() const;
    string getTelefone() const;
    string getDataNascimento() const;

    // Setters
    void setNome(const string& n);
    void setSobrenome(const string& s);
    void setTelefone(const string& t);
    void setDataNascimento(const string& d);

    // Métodos de empacotamento/desempacotamento
    string packDelimitado() const;
    void unpackDelimitado(const string& str);
    string packDescritor() const;
    bool unpackDescritor(const string& buffer);

    // Operador de saída
    friend ostream& operator<<(ostream& os, const Registro& reg);
};

#endif
