#include "./include/arquivos.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>


using namespace std;




void salvarDados(const vector<Aeronave> &aeronaves,
                 const vector<Piloto> &pilotos,
                 const vector<Passageiro> &passageiros,
                 const vector<Voo> &voos)
{
    // Salvar Aeronaves
    ofstream fa("./files/aeronaves.csv");
    if (fa.is_open())
    {
        for (const auto &a : aeronaves)
        {
            fa << a.getCodigo() << ","
               << a.getCapacidade() << ","
               << a.getModelo() << ","
               << a.getVelocidadeMedia() << ","
               << a.getAutonomiaDeVoo() << endl;
        }
        fa.close();
    }

    // Salvar Pilotos
    ofstream fp("./files/pilotos.csv");
    if (fp.is_open())
    {
        for (const auto &p : pilotos)
        {
            fp << p.getNome() << ","
               << p.getMatricula() << ","
               << p.getBreve() << ","
               << p.getHorasDeVoo() << endl;
        }
        fp.close();
    }

    // Salvar Passageiros
    ofstream fpa("./files/passageiros.csv");
    if (fpa.is_open())
    {
        for (const auto &p : passageiros)
        {
            fpa << p.getNome() << ","
                << p.getCpf() << ","
                << p.getNumeroBilhete() << endl;
        }
        fpa.close();
    }

    // Salvar Voos
    ofstream fv("./files/voos.csv");
    if (fv.is_open())
    {
        for (const auto &v : voos)
        {
            fv << v.getCodigo() << ","
               << v.getOrigem() << ","
               << v.getDestino() << ","
               << v.getDistancia() << ","
               << v.getHoraDeSaida() << ","
               << v.getNumeroDeEscalas() << ","
               << v.getTempoEstimado() << ","
               << v.getAeronave().getCodigo() << ","
               << v.getComandante().getMatricula() << ","
               << v.getPrimeiroOficial().getMatricula() << ","
               << v.getPassageiros().size() << ",";

            // Escrever os CPFs dos passageiros separados por '|'
            const auto &passageiros = v.getPassageiros();
            for (int i = 0; i < passageiros.size(); i++)
            {
                fv << passageiros[i].getCpf();
                if (i != passageiros.size() - 1)
                    fv << "|";
            }
            fv << endl;
        }
        fv.close();
    }
}


void carregarDados(vector<Aeronave> &aeronaves,
                  vector<Piloto> &pilotos,
                  vector<Passageiro> &passageiros,
                  vector<Voo> &voos) {
    // Limpar os vetores existentes
    aeronaves.clear();
    pilotos.clear();
    passageiros.clear();
    voos.clear();

    // Carregar Aeronaves
    ifstream fa("./files/aeronaves.csv");
    if (fa.is_open()) {
        string linha;
        while (getline(fa, linha)) {
            stringstream ss(linha);
            string modelo;
            int capacidade, codigo;
            double velocidadeMedia, autonomiaDeVoo;

            ss >> codigo;
            ss >> capacidade;
            ss.ignore();
            getline(ss, modelo, ',');
            ss >> velocidadeMedia;
            ss.ignore();
            ss >> autonomiaDeVoo;

            Aeronave a(codigo, capacidade, modelo, velocidadeMedia, autonomiaDeVoo);
            aeronaves.push_back(a);
        }
        fa.close();
    }

    // Carregar Pilotos
    ifstream fp("./files/pilotos.csv");
    if (fp.is_open()) {
        string linha;
        while (getline(fp, linha)) {
            stringstream ss(linha);
            string nome, matricula, breve;
            int horasDeVoo;

            getline(ss, nome, ',');
            getline(ss, matricula, ',');
            getline(ss, breve, ',');
            ss >> horasDeVoo;

            Piloto p(nome, matricula, breve, horasDeVoo);
            pilotos.push_back(p);
        }
        fp.close();
    }

    // Carregar Passageiros
    ifstream fpa("./files/passageiros.csv");
    if (fpa.is_open()) {
        string linha;
        while (getline(fpa, linha)) {
            stringstream ss(linha);
            string nome, cpf, numeroBilhete;

            getline(ss, nome, ',');
            getline(ss, cpf, ',');
            getline(ss, numeroBilhete);

            Passageiro pa(nome, cpf, numeroBilhete);
            passageiros.push_back(pa);
        }
        fpa.close();
    }

    // Carregar Voos
    ifstream fv("./files/voos.csv");
    if (fv.is_open()) {
        string linha;
        while (getline(fv, linha)) {
            stringstream ss(linha);
            string origem, destino, matComandante, matOficial, horaSaida;
            double distancia, tempEstimado;
            int numPassageiros, codigo, escalas, codAeronave;
            vector<Passageiro> passageirosVoo;

            ss >> codigo;
            ss.ignore();
            getline(ss, origem, ',');
            getline(ss, destino, ',');
            ss >> distancia;
            ss.ignore();
            getline(ss, horaSaida, ',');
            ss >> escalas;
            ss.ignore();
            ss >> tempEstimado;
            ss.ignore();
            ss >> codAeronave;
            ss.ignore();
            getline(ss, matComandante, ',');
            getline(ss, matOficial, ',');
            ss >> numPassageiros;
            ss.ignore();

            // Ler passageiros do voo
            string passageirosStr;
            getline(ss, passageirosStr);
            if (numPassageiros > 0) {
                stringstream ssPass(passageirosStr);
                string cpf;
                while (getline(ssPass, cpf, '|')) {
                        passageirosVoo.push_back(passageiros[encontrarIndicePassageiroPorCpf(passageiros, cpf)]);
                }
            }

            // Criar o voo
            Voo v(codigo, origem, destino, distancia, horaSaida, escalas, tempEstimado);
            
            // Configurar aeronave e pilotos se existirem
            v.setAeronave(aeronaves[encontrarIndiceAeronavePorCodigo(aeronaves, codAeronave)]);
            v.setComandante(pilotos[encontrarIndicePilotoPorMatricula(pilotos, matComandante)]);            
            v.setPrimeiroOficial(pilotos[encontrarIndicePilotoPorMatricula(pilotos, matOficial)]);
           
            // Adicionar passageiros
            for (auto &p : passageirosVoo) {
                v.adicionarPassageiro(p);
            }

            voos.push_back(v);
        }
        fv.close();
    }
}
