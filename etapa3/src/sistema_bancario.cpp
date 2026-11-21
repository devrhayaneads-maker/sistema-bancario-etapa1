// NOVABANK - Etapa 3
// INF101 - Programação de Computadores I - UNIVIÇOSA
// Rhayane Freitas

#include <iostream>  // cout e getline
#include <fstream>   // ifstream e ofstream (arquivos)
#include <string>    // string
#include <iomanip>   // setw e setprecision
#include <cmath>     // round
#include <cstdlib>   // exit

#ifdef _WIN32        // só no Windows: arruma os acentos no terminal
#include <windows.h>
#endif

using namespace std;   // pra escrever cout em vez de std::cout

// ===== CONTAS =====
// globais (fora do main) pra todas as funções enxergarem.
// vetores paralelos: a mesma posição em todos é a mesma conta
const int MAX_CONTAS = 100;
int numeroConta[MAX_CONTAS];
string nomeCliente[MAX_CONTAS];
string cpf[MAX_CONTAS];
int tipoConta[MAX_CONTAS];         // 1 = corrente, 2 = poupança
double saldo[MAX_CONTAS];
bool contaAtiva[MAX_CONTAS];
int quantidadeContas = 0;          // também é a próxima posição livre

// ===== HISTÓRICO =====
const int MAX_MOVIMENTACOES = 500;
int contaMovimentacao[MAX_MOVIMENTACOES];     // número da conta
char tipoMovimentacao[MAX_MOVIMENTACOES];     // 'D' = entrada, 'S' = saída
double valorMovimentacao[MAX_MOVIMENTACOES];
int contaTransferencia[MAX_MOVIMENTACOES];    // a outra conta da transferência, ou 0
int quantidadeMovimentacoes = 0;

const double VALOR_MAXIMO = 1000000000.00;   // limite por operação
const int MAX_CAMPOS = 6;                    // a linha da conta tem 6 pedaços, é a maior

// ===== PROTÓTIPOS =====
// aviso ao compilador que essas funções existem (o código delas vem depois do main)

// arquivos (item 2.7)
void carregarContas();
void salvarContas();
void carregarMovimentacoes();
void salvarMovimentacoes();
int separarCampos(string linha, string campos[]);

// opções do menu
void cadastrarConta();
void listarContas();
void consultarConta();
void depositar();
void sacar();
void transferir();
void registrarMovimentacao(int conta, char tipo, double valor, int outraConta);
void exibirExtrato();
void ativarDesativarConta();
void excluirConta();

// leituras e ajudantes
string lerTexto();
string tirarEspacos(string texto);
int lerNumero();
double lerValor(string pergunta, bool aceitaZero);
int textoParaNumero(string texto);
double textoParaValor(string texto);
bool cpfValido(string texto);
int buscarConta(int numero);
int pedirConta(string pergunta);
double paraCentavos(double valor);

// ===== MENU =====
int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);   // acento no que o programa escreve
    SetConsoleCP(CP_UTF8);         // acento no que a pessoa digita
#endif

    cout << fixed << setprecision(2);   // todo double sai com 2 casas: 1000.00

    // antes do menu eu trago de volta o que ficou salvo (item 2.2).
    // as contas vêm primeiro porque a movimentação só entra se a conta dela existir
    carregarContas();
    carregarMovimentacoes();

    int opcao;

    do {
        cout << "\n";
        cout << "***********************************************\n";
        cout << "**                 NOVABANK                  **\n";
        cout << "**       Sistema de Contas Bancárias         **\n";
        cout << "***********************************************\n";
        cout << " Contas: " << quantidadeContas << " de " << MAX_CONTAS
             << "   Movimentações: " << quantidadeMovimentacoes << " de " << MAX_MOVIMENTACOES << "\n";
        cout << "***********************************************\n";
        cout << " 1 - Cadastrar conta\n";
        cout << " 2 - Listar contas\n";
        cout << " 3 - Consultar conta\n";
        cout << " 4 - Depositar\n";
        cout << " 5 - Sacar\n";
        cout << " 6 - Transferir\n";
        cout << " 7 - Extrato\n";
        cout << " 8 - Ativar/Desativar conta\n";
        cout << " 9 - Excluir conta\n";
        cout << " 0 - Sair\n";
        cout << "***********************************************\n";
        cout << "Escolha uma opção: ";

        opcao = lerNumero();   // -1 se não for número

        switch (opcao) {
            case 1: cadastrarConta();       break;
            case 2: listarContas();         break;
            case 3: consultarConta();       break;
            case 4: depositar();            break;
            case 5: sacar();                break;
            case 6: transferir();           break;
            case 7: exibirExtrato();        break;
            case 8: ativarDesativarConta(); break;
            case 9: excluirConta();         break;

            case 0:   // tudo já foi salvo em cada operação, então não preciso confirmar
                cout << "\n>> Os dados estão salvos em contas.txt e movimentacoes.txt.\n";
                cout << ">> Encerrando o NOVABANK. Obrigado por utilizar nossos serviços!\n";
                break;

            default:
                cout << "\n>> Opção inválida! Escolha um número de 0 a 9.\n";
                break;
        }

        // pausa pra pessoa ler antes do menu voltar
        if (opcao != 0) {
            cout << "\nPressione ENTER para continuar...";
            lerTexto();
        }

    } while (opcao != 0);

    return 0;
}

// ===== ARQUIVOS =====
// contas.txt         numero;nome;cpf;tipo;saldo;ativa    ex: 1001;Ana Silva;12345678900;1;700.00;1
// movimentacoes.txt  conta;tipo;valor                    ex: 1001;D;500.00
// na transferência a linha leva a outra conta no fim     ex: 1001;S;300.00;1002

// lê o contas.txt e coloca cada linha nos vetores das contas
void carregarContas() {
    ifstream arquivo("contas.txt");
    if (!arquivo.is_open()) {   // primeira vez: o arquivo ainda não existe
        cout << ">> contas.txt ainda não existe: o sistema começa sem contas cadastradas.\n";
        return;
    }

    string linha;
    string campos[MAX_CAMPOS];
    int numeroLinha = 0;
    int naoCouberam = 0;

    while (getline(arquivo, linha)) {   // uma linha por vez, até o arquivo acabar
        numeroLinha++;
        int quantidade = separarCampos(linha, campos);

        if (quantidade == 1 && campos[0] == "") {
            // linha em branco: pulo
        } else if (quantidadeContas >= MAX_CONTAS) {
            naoCouberam++;
        } else {
            // confiro cada pedaço antes de guardar, pra linha estragada não virar conta
            bool valida = (quantidade == 6);
            int numero = 0;
            int tipo = 0;
            double valor = -1;
            if (valida) {
                numero = textoParaNumero(campos[0]);
                tipo = textoParaNumero(campos[3]);
                valor = textoParaValor(campos[4]);
                valida = numero > 0 && buscarConta(numero) == -1 &&
                         campos[1] != "" && cpfValido(campos[2]) &&
                         (tipo == 1 || tipo == 2) && valor >= 0 &&
                         (campos[5] == "1" || campos[5] == "0");
            }

            if (valida) {
                int i = quantidadeContas;
                numeroConta[i] = numero;
                nomeCliente[i] = campos[1];
                cpf[i] = campos[2];
                tipoConta[i] = tipo;
                saldo[i] = valor;
                contaAtiva[i] = (campos[5] == "1");
                quantidadeContas++;
            } else {
                cout << ">> Linha " << numeroLinha << " do contas.txt com erro: não foi carregada.\n";
            }
        }
    }
    arquivo.close();

    if (naoCouberam > 0) {
        cout << ">> " << naoCouberam << " conta(s) do arquivo não couberam (limite de "
             << MAX_CONTAS << ").\n";
    }
    cout << ">> " << quantidadeContas << " conta(s) carregada(s) do contas.txt\n";
}

// grava todas as contas no contas.txt (item 2.3).
// sem o ios::app o arquivo começa do zero, mas como eu gravo todas de novo nada se perde
void salvarContas() {
    ofstream arquivo("contas.txt");
    if (arquivo.is_open()) {
        arquivo << fixed << setprecision(2);   // saldo com 2 casas: 700.00
        for (int i = 0; i < quantidadeContas; i++) {
            arquivo << numeroConta[i] << ";"
                    << nomeCliente[i] << ";"
                    << cpf[i] << ";"
                    << tipoConta[i] << ";"
                    << saldo[i] << ";"
                    << contaAtiva[i] << endl;   // bool sai como 1 ou 0
        }
        arquivo.close();
    } else {
        cout << "\n>> ERRO: não foi possível salvar o contas.txt.\n";
    }
}

// lê o movimentacoes.txt e coloca cada linha nos vetores do histórico
void carregarMovimentacoes() {
    ifstream arquivo("movimentacoes.txt");
    if (!arquivo.is_open()) {   // ainda não teve depósito, saque nem transferência
        cout << ">> movimentacoes.txt ainda não existe: nenhuma movimentação registrada até agora.\n";
        return;
    }

    string linha;
    string campos[MAX_CAMPOS];
    int numeroLinha = 0;
    int naoCouberam = 0;

    while (getline(arquivo, linha)) {
        numeroLinha++;
        int quantidade = separarCampos(linha, campos);

        if (quantidade == 1 && campos[0] == "") {
            // linha em branco: pulo
        } else if (quantidadeMovimentacoes >= MAX_MOVIMENTACOES) {
            naoCouberam++;
        } else {
            // 3 pedaços, ou 4 quando é transferência
            bool valida = (quantidade == 3 || quantidade == 4);
            int conta = 0;
            double valor = -1;
            int outraConta = 0;
            if (valida) {
                conta = textoParaNumero(campos[0]);
                valor = textoParaValor(campos[2]);
                if (quantidade == 4) {
                    outraConta = textoParaNumero(campos[3]);
                }
                // a conta tem que estar no contas.txt, senão a movimentação fica solta
                valida = buscarConta(conta) != -1 &&
                         (campos[1] == "D" || campos[1] == "S") &&
                         valor > 0 && outraConta >= 0;
            }

            if (valida) {
                registrarMovimentacao(conta, campos[1][0], valor, outraConta);   // "D" vira 'D'
            } else {
                cout << ">> Linha " << numeroLinha << " do movimentacoes.txt com erro: não foi carregada.\n";
            }
        }
    }
    arquivo.close();

    if (naoCouberam > 0) {
        cout << ">> " << naoCouberam << " movimentação(ões) do arquivo não couberam (limite de "
             << MAX_MOVIMENTACOES << ").\n";
    }
    cout << ">> " << quantidadeMovimentacoes << " movimentação(ões) carregada(s) do movimentacoes.txt\n";
}

// grava o histórico inteiro no movimentacoes.txt (item 2.4)
void salvarMovimentacoes() {
    ofstream arquivo("movimentacoes.txt");
    if (arquivo.is_open()) {
        arquivo << fixed << setprecision(2);
        for (int m = 0; m < quantidadeMovimentacoes; m++) {
            arquivo << contaMovimentacao[m] << ";"
                    << tipoMovimentacao[m] << ";"
                    << valorMovimentacao[m];
            if (contaTransferencia[m] != 0) {   // transferência: gravo a outra conta também
                arquivo << ";" << contaTransferencia[m];
            }
            arquivo << endl;
        }
        arquivo.close();
    } else {
        cout << "\n>> ERRO: não foi possível salvar o movimentacoes.txt.\n";
    }
}

// separa a linha nos pedaços que estão entre os ";" e devolve quantos pedaços tinha
// ex: "1001;D;500.00" fica campos[0] = "1001", campos[1] = "D", campos[2] = "500.00"
int separarCampos(string linha, string campos[]) {
    // alguns editores (o bloco de notas, por exemplo) gravam 3 caracteres invisíveis
    // no começo do arquivo, o "BOM". se eles vierem eu pulo, senão a primeira linha dava erro
    int inicio = 0;
    if (linha.length() >= 3 && linha[0] == '\xEF' && linha[1] == '\xBB' && linha[2] == '\xBF') {
        inicio = 3;
    }

    int quantidade = 1;
    campos[0] = "";
    int tamanho = linha.length();
    for (int c = inicio; c < tamanho; c++) {
        if (linha[c] == ';') {
            quantidade++;   // começa o próximo pedaço
            if (quantidade <= MAX_CAMPOS) {
                campos[quantidade - 1] = "";
            }
        } else if (linha[c] != '\r' && quantidade <= MAX_CAMPOS) {   // \r é resto do enter do Windows
            campos[quantidade - 1] = campos[quantidade - 1] + linha[c];
        }
    }

    // tiro os espaços que sobraram em volta de cada pedaço.
    // quem copia o exemplo do PDF às vezes leva um espaço junto no fim da linha
    for (int p = 0; p < quantidade && p < MAX_CAMPOS; p++) {
        campos[p] = tirarEspacos(campos[p]);
    }
    return quantidade;
}

// ===== LEITURAS =====

// lê a linha inteira que a pessoa digitou
string lerTexto() {
    string digitado;
    getline(cin, digitado);
    if (cin.fail()) exit(0);   // a entrada acabou: fecho o programa inteiro
    return tirarEspacos(digitado);   // " 1001 " vale como "1001"
}

// tira os espaços do começo e do fim: "  Ana Silva " fica "Ana Silva".
// os do meio ficam, senão o nome virava "AnaSilva"
string tirarEspacos(string texto) {
    int inicio = 0;
    int fim = texto.length() - 1;
    while (inicio <= fim && (texto[inicio] == ' ' || texto[inicio] == '\t')) {
        inicio++;
    }
    while (fim >= inicio && (texto[fim] == ' ' || texto[fim] == '\t')) {
        fim--;
    }
    return texto.substr(inicio, fim - inicio + 1);
}

// lê um número do teclado. -1 se não for número
int lerNumero() {
    return textoParaNumero(lerTexto());
}

// lê dinheiro do teclado. pergunta até vir um valor válido.
// aceitaZero diz se o 0 vale (só no saldo inicial)
double lerValor(string pergunta, bool aceitaZero) {
    double valor = 0;
    bool valido = false;

    cout << "(centavos com ponto ou vírgula, ex: 1500.50 - sem ponto de milhar)\n";
    do {
        cout << pergunta;
        double valorLido = textoParaValor(lerTexto());

        if (valorLido == -1) {
            cout << "   >> Inválido! Digite apenas números, ex: 1500.50\n";
        } else if (valorLido == -2) {   // ex: 2.000
            cout << "   >> Inválido! Os centavos têm no máximo duas casas.\n";
            cout << "      (dois mil reais se digita 2000, sem ponto de milhar)\n";
        } else if (valorLido > VALOR_MAXIMO) {
            cout << "   >> Inválido! O valor máximo por operação é R$ " << VALOR_MAXIMO << "\n";
        } else if (valorLido == 0 && !aceitaZero) {
            cout << "   >> Inválido! O valor deve ser maior que zero.\n";
        } else {
            valor = valorLido;
            valido = true;
        }
    } while (!valido);

    return valor;
}

// texto pra número inteiro: só vale se tiver de 1 a 9 algarismos e mais nada. senão devolve -1.
// uso pro que a pessoa digita e pro que vem do arquivo
int textoParaNumero(string texto) {
    int tamanho = texto.length();
    if (tamanho == 0 || tamanho > 9) return -1;   // vazio ou grande demais

    int numero = 0;
    for (int c = 0; c < tamanho; c++) {
        if (texto[c] < '0' || texto[c] > '9') return -1;   // letra ou símbolo
        numero = numero * 10 + (texto[c] - '0');           // "25": 2, depois 2*10+5 = 25
    }
    return numero;
}

// texto pra dinheiro, com ponto ou vírgula nos centavos.
// devolve -1 se não for número e -2 se tiver mais de duas casas depois da vírgula
double textoParaValor(string texto) {
    // junto os algarismos e divido no final: "1500,50" vira 150050 / 100 = 1500.50
    double valor = 0;
    double divisor = 1;
    int casas = -1;            // casas depois da vírgula (-1 = não teve vírgula)
    int algarismos = 0;
    int tamanho = texto.length();
    for (int c = 0; c < tamanho; c++) {
        if (texto[c] >= '0' && texto[c] <= '9') {
            algarismos++;
            valor = valor * 10 + (texto[c] - '0');
            if (casas >= 0) {   // já passou da vírgula
                casas++;
                divisor = divisor * 10;
            }
        } else if ((texto[c] == '.' || texto[c] == ',') && casas == -1) {
            casas = 0;          // achei a vírgula
        } else {
            return -1;          // letra, sinal de menos, segunda vírgula...
        }
    }
    if (algarismos == 0) return -1;
    if (casas > 2) return -2;
    return valor / divisor;
}

// CPF: exatamente 11 algarismos
bool cpfValido(string texto) {
    if (texto.length() != 11) return false;
    for (int c = 0; c < 11; c++) {
        if (texto[c] < '0' || texto[c] > '9') return false;
    }
    return true;
}

// ===== AJUDANTES =====

// busca sequencial: devolve a posição da conta, ou -1 se não achar
int buscarConta(int numero) {
    for (int i = 0; i < quantidadeContas; i++) {
        if (numeroConta[i] == numero) {
            return i;
        }
    }
    return -1;
}

// pergunta o número da conta, procura e avisa se der errado. devolve a posição ou -1
int pedirConta(string pergunta) {
    if (quantidadeContas == 0) {
        cout << "\n>> Nenhuma conta cadastrada. Use a opção 1.\n";
        return -1;
    }

    cout << pergunta;
    int numero = lerNumero();
    if (numero <= 0) {
        cout << "\n>> Número de conta inválido.\n";
        return -1;
    }

    int posicao = buscarConta(numero);
    if (posicao == -1) {
        cout << "\n>> Conta " << numero << " não encontrada.\n";
    }
    return posicao;
}

// reais pra centavos (0.70 vira 70). conta com dinheiro sempre em centavos:
// no double, 0.70 + 0.10 dava 0.7999999 e sacar 0.80 dava "insuficiente"
double paraCentavos(double valor) {
    return round(valor * 100);
}

// coloca uma linha no histórico (quem chama já conferiu se tem espaço)
void registrarMovimentacao(int conta, char tipo, double valor, int outraConta) {
    contaMovimentacao[quantidadeMovimentacoes] = conta;
    tipoMovimentacao[quantidadeMovimentacoes] = tipo;
    valorMovimentacao[quantidadeMovimentacoes] = valor;
    contaTransferencia[quantidadeMovimentacoes] = outraConta;
    quantidadeMovimentacoes++;
}

// ===== 1 - CADASTRAR =====
void cadastrarConta() {
    if (quantidadeContas >= MAX_CONTAS) {
        cout << "\n>> Limite de " << MAX_CONTAS << " contas atingido.\n";
        return;
    }

    int i = quantidadeContas;   // a conta nova vai na próxima posição livre
    cout << "\n--------- CADASTRO DE CONTA ---------\n";

    // número: maior que zero e sem repetir
    bool numeroValido;
    do {
        cout << "Número da conta....: ";
        numeroConta[i] = lerNumero();

        numeroValido = (numeroConta[i] > 0);
        if (!numeroValido) {
            cout << "   >> Inválido! O número da conta deve ser um inteiro maior que zero.\n";
        } else if (buscarConta(numeroConta[i]) != -1) {   // já existe
            numeroValido = false;
            cout << "   >> Esse número de conta já está cadastrado.\n";
        }
    } while (!numeroValido);

    // nome: pelo menos uma letra, até 80 caracteres e sem ";"
    bool nomeValido;
    do {
        cout << "Nome do titular....: ";
        nomeCliente[i] = lerTexto();

        nomeValido = false;
        bool temPontoEVirgula = false;
        int tamanho = nomeCliente[i].length();
        for (int c = 0; c < tamanho; c++) {
            char letra = nomeCliente[i][c];
            if ((letra >= 'A' && letra <= 'Z') ||
                (letra >= 'a' && letra <= 'z') ||
                letra < 0) {   // letra com acento vem negativa
                nomeValido = true;
            }
            if (letra == ';') {
                temPontoEVirgula = true;
            }
        }
        if (!nomeValido) {
            cout << "   >> Inválido! O nome precisa ter letras, ex: Maria Silva\n";
        } else if (tamanho > 80) {
            nomeValido = false;
            cout << "   >> Inválido! O nome pode ter no máximo 80 caracteres.\n";
        } else if (temPontoEVirgula) {   // o ";" separa os dados no arquivo
            nomeValido = false;
            cout << "   >> Inválido! O nome não pode ter ponto e vírgula (;).\n";
        }
    } while (!nomeValido);

    // CPF: exatamente 11 algarismos
    bool cpfOk;
    do {
        cout << "CPF (11 dígitos)...: ";
        cpf[i] = lerTexto();
        cpfOk = cpfValido(cpf[i]);
        if (!cpfOk) {
            cout << "   >> Inválido! Digite exatamente 11 dígitos, sem pontos, traços ou espaços.\n";
        }
    } while (!cpfOk);

    // tipo: 1 ou 2
    do {
        cout << "Tipo (1=Corrente, 2=Poupança): ";
        tipoConta[i] = lerNumero();
        if (tipoConta[i] != 1 && tipoConta[i] != 2) {
            cout << "   >> Inválido! Digite 1 para Corrente ou 2 para Poupança.\n";
        }
    } while (tipoConta[i] != 1 && tipoConta[i] != 2);

    saldo[i] = lerValor("Saldo inicial R$...: ", true);   // true: aqui o 0 vale
    contaAtiva[i] = true;
    quantidadeContas++;   // só agora a conta passa a contar

    salvarContas();

    cout << "\n>> Conta " << numeroConta[i] << " cadastrada com sucesso e ATIVADA!\n";
}

// ===== 2 - LISTAR =====
void listarContas() {
    if (quantidadeContas == 0) {
        cout << "\n>> Nenhuma conta cadastrada. Use a opção 1.\n";
        return;
    }

    cout << "\n--------- CONTAS CADASTRADAS ---------\n";
    // nome por último: o setw conta letra com acento como duas
    cout << "     Conta  Tipo      Situação     Saldo (R$)  Titular\n";
    for (int i = 0; i < quantidadeContas; i++) {
        cout << setw(10) << numeroConta[i] << "  ";
        if (tipoConta[i] == 1) {
            cout << "Corrente  ";
        } else {
            cout << "Poupança  ";
        }
        if (contaAtiva[i]) {
            cout << "ATIVA   ";
        } else {
            cout << "INATIVA ";
        }
        cout << setw(15) << saldo[i] << "  " << nomeCliente[i] << "\n";
    }
    cout << "\nTotal: " << quantidadeContas << " conta(s)\n";
}

// ===== 3 - CONSULTAR =====
void consultarConta() {
    int posicao = pedirConta("\nInforme o número da conta: ");
    if (posicao == -1) return;

    int movimentacoesDaConta = 0;
    for (int m = 0; m < quantidadeMovimentacoes; m++) {
        if (contaMovimentacao[m] == numeroConta[posicao]) {
            movimentacoesDaConta++;
        }
    }

    cout << "\n--------- DADOS DA CONTA ---------\n";
    cout << "Número da conta : " << numeroConta[posicao] << "\n";
    cout << "Titular         : " << nomeCliente[posicao] << "\n";
    cout << "CPF             : " << cpf[posicao] << "\n";

    cout << "Tipo            : ";
    if (tipoConta[posicao] == 1) {
        cout << "Corrente\n";
    } else {
        cout << "Poupança\n";
    }

    cout << "Saldo           : R$ " << saldo[posicao] << "\n";

    cout << "Situação        : ";
    if (contaAtiva[posicao]) {
        cout << "ATIVA\n";
    } else {
        cout << "INATIVA\n";
    }
    cout << "Movimentações   : " << movimentacoesDaConta << "\n";
}

// ===== 4 - DEPOSITAR (grava um 'D') =====
void depositar() {
    int posicao = pedirConta("\nInforme o número da conta: ");
    if (posicao == -1) return;

    if (!contaAtiva[posicao]) {
        cout << "\n>> Operação não permitida: a conta está INATIVA.\n";
        return;
    }
    if (quantidadeMovimentacoes + 1 > MAX_MOVIMENTACOES) {
        cout << "\n>> Histórico de movimentações cheio (limite de "
             << MAX_MOVIMENTACOES << ").\n";
        return;
    }

    cout << "\n--------- DEPÓSITO ---------\n";
    cout << "Conta nº " << numeroConta[posicao] << " - " << nomeCliente[posicao] << "\n";
    cout << "Saldo atual: R$ " << saldo[posicao] << "\n";

    double valor = lerValor("Valor R$: ", false);   // false: aqui o 0 não vale

    saldo[posicao] = (paraCentavos(saldo[posicao]) + paraCentavos(valor)) / 100;
    registrarMovimentacao(numeroConta[posicao], 'D', valor, 0);   // 0: não é transferência

    salvarContas();          // o saldo mudou
    salvarMovimentacoes();   // e o histórico ganhou uma linha

    cout << "\n>> Depósito realizado! Novo saldo: R$ " << saldo[posicao] << "\n";
}

// ===== 5 - SACAR (grava um 'S') =====
void sacar() {
    int posicao = pedirConta("\nInforme o número da conta: ");
    if (posicao == -1) return;

    if (!contaAtiva[posicao]) {
        cout << "\n>> Operação não permitida: a conta está INATIVA.\n";
        return;
    }
    if (quantidadeMovimentacoes + 1 > MAX_MOVIMENTACOES) {
        cout << "\n>> Histórico de movimentações cheio (limite de "
             << MAX_MOVIMENTACOES << ").\n";
        return;
    }

    cout << "\n--------- SAQUE ---------\n";
    cout << "Conta nº " << numeroConta[posicao] << " - " << nomeCliente[posicao] << "\n";
    cout << "Saldo atual: R$ " << saldo[posicao] << "\n";

    double valor = lerValor("Valor R$: ", false);

    if (paraCentavos(valor) > paraCentavos(saldo[posicao])) {
        cout << "\n>> Saldo insuficiente! Saldo disponível: R$ " << saldo[posicao] << "\n";
        return;
    }

    saldo[posicao] = (paraCentavos(saldo[posicao]) - paraCentavos(valor)) / 100;
    registrarMovimentacao(numeroConta[posicao], 'S', valor, 0);

    salvarContas();
    salvarMovimentacoes();

    cout << "\n>> Saque realizado! Novo saldo: R$ " << saldo[posicao] << "\n";
}

// ===== 6 - TRANSFERIR (item 2.5) =====
// só entre contas ativas; grava um 'S' na origem e um 'D' no destino
void transferir() {
    if (quantidadeContas == 1) {
        cout << "\n>> Para transferir é preciso ter pelo menos duas contas cadastradas.\n";
        return;
    }

    int origem = pedirConta("\nConta de origem: ");
    if (origem == -1) return;

    if (!contaAtiva[origem]) {
        cout << "\n>> Operação não permitida: a conta de origem está INATIVA.\n";
        return;
    }
    if (quantidadeMovimentacoes + 2 > MAX_MOVIMENTACOES) {   // precisam caber os 2 registros
        if (quantidadeMovimentacoes < MAX_MOVIMENTACOES) {
            cout << "\n>> Sem espaço no histórico para a transferência: ela grava 2 movimentações e só resta 1.\n";
        } else {
            cout << "\n>> Histórico de movimentações cheio (limite de "
                 << MAX_MOVIMENTACOES << ").\n";
        }
        return;
    }

    int destino = pedirConta("Conta de destino: ");
    if (destino == -1) return;

    if (destino == origem) {
        cout << "\n>> A conta de destino precisa ser diferente da conta de origem.\n";
        return;
    }
    if (!contaAtiva[destino]) {
        cout << "\n>> Operação não permitida: a conta de destino está INATIVA.\n";
        return;
    }

    cout << "\n--------- TRANSFERÊNCIA ---------\n";
    cout << "De:   conta nº " << numeroConta[origem] << " - " << nomeCliente[origem] << "\n";
    cout << "Para: conta nº " << numeroConta[destino] << " - " << nomeCliente[destino] << "\n";
    cout << "Saldo da conta de origem: R$ " << saldo[origem] << "\n";

    double valor = lerValor("Valor R$: ", false);

    if (paraCentavos(valor) > paraCentavos(saldo[origem])) {
        cout << "\n>> Saldo insuficiente! Saldo disponível: R$ " << saldo[origem] << "\n";
        return;
    }

    saldo[origem] = (paraCentavos(saldo[origem]) - paraCentavos(valor)) / 100;
    saldo[destino] = (paraCentavos(saldo[destino]) + paraCentavos(valor)) / 100;

    registrarMovimentacao(numeroConta[origem], 'S', valor, numeroConta[destino]);
    registrarMovimentacao(numeroConta[destino], 'D', valor, numeroConta[origem]);

    salvarContas();          // os dois saldos mudaram
    salvarMovimentacoes();   // e entraram as duas linhas

    cout << "\n>> Transferência realizada!\n";
    cout << "   Conta nº " << numeroConta[origem] << " - " << nomeCliente[origem]
         << ": R$ " << saldo[origem] << "\n";
    cout << "   Conta nº " << numeroConta[destino] << " - " << nomeCliente[destino]
         << ": R$ " << saldo[destino] << "\n";
}

// ===== 7 - EXTRATO =====
void exibirExtrato() {
    int posicao = pedirConta("\nInforme o número da conta: ");
    if (posicao == -1) return;

    // o arquivo não guarda o saldo inicial, então eu calculo: pego o saldo de agora
    // e desfaço as movimentações (tiro o que entrou e devolvo o que saiu)
    double centavosIniciais = paraCentavos(saldo[posicao]);
    for (int m = 0; m < quantidadeMovimentacoes; m++) {
        if (contaMovimentacao[m] == numeroConta[posicao]) {
            if (tipoMovimentacao[m] == 'D') {
                centavosIniciais = centavosIniciais - paraCentavos(valorMovimentacao[m]);
            } else {
                centavosIniciais = centavosIniciais + paraCentavos(valorMovimentacao[m]);
            }
        }
    }

    cout << "\n--------- EXTRATO ---------\n";
    cout << "Conta nº " << numeroConta[posicao] << " - " << nomeCliente[posicao];
    if (!contaAtiva[posicao]) {
        cout << " (INATIVA)";
    }
    cout << "\n\n";
    cout << "Saldo inicial: R$ " << centavosIniciais / 100 << "\n";

    // passo pelo histórico e mostro só o que é dessa conta
    int encontradas = 0;
    for (int m = 0; m < quantidadeMovimentacoes; m++) {
        if (contaMovimentacao[m] == numeroConta[posicao]) {
            encontradas++;

            if (tipoMovimentacao[m] == 'D') {
                if (contaTransferencia[m] != 0) {
                    cout << "(+) Transferência recebida da conta " << contaTransferencia[m];
                } else {
                    cout << "(+) Depósito";
                }
            } else if (tipoMovimentacao[m] == 'S') {
                if (contaTransferencia[m] != 0) {
                    cout << "(-) Transferência enviada para a conta " << contaTransferencia[m];
                } else {
                    cout << "(-) Saque";
                }
            }
            cout << ": R$ " << valorMovimentacao[m] << "\n";
        }
    }

    if (encontradas == 0) {
        cout << "Nenhuma movimentação registrada nesta conta.\n";
    }
    cout << "\nSaldo atual: R$ " << saldo[posicao] << "\n";
}

// ===== 8 - ATIVAR/DESATIVAR =====
void ativarDesativarConta() {
    int posicao = pedirConta("\nInforme o número da conta: ");
    if (posicao == -1) return;

    if (contaAtiva[posicao]) {
        contaAtiva[posicao] = false;
        cout << "\n>> Conta nº " << numeroConta[posicao] << " DESATIVADA.\n";
    } else {
        contaAtiva[posicao] = true;
        cout << "\n>> Conta nº " << numeroConta[posicao] << " ATIVADA.\n";
    }

    salvarContas();   // a situação mudou
}

// ===== 9 - EXCLUIR =====
void excluirConta() {
    int posicao = pedirConta("\nInforme o número da conta: ");
    if (posicao == -1) return;

    cout << "\n--------- EXCLUIR CONTA ---------\n";
    cout << "Conta nº " << numeroConta[posicao] << " - " << nomeCliente[posicao] << "\n";
    cout << "Saldo: R$ " << saldo[posicao] << "\n";

    // com dinheiro não exclui, senão o dinheiro sumiria junto
    if (saldo[posicao] > 0) {
        cout << "\n>> Não é possível excluir: a conta ainda tem saldo.\n";
        cout << "   Saque ou transfira o dinheiro antes de encerrar a conta.\n";
        if (!contaAtiva[posicao]) {
            cout << "   (a conta está INATIVA: ative na opção 8 para poder sacar)\n";
        }
        return;
    }
    // hoje nunca acontece; fica pronto pro dia que tiver cheque especial
    if (saldo[posicao] < 0) {
        cout << "\n>> Não é possível excluir: a conta está com saldo negativo.\n";
        cout << "   Regularize as pendências antes de encerrar a conta.\n";
        return;
    }

    int movimentacoesDaConta = 0;
    for (int m = 0; m < quantidadeMovimentacoes; m++) {
        if (contaMovimentacao[m] == numeroConta[posicao]) {
            movimentacoesDaConta++;
        }
    }

    if (movimentacoesDaConta == 0) {
        cout << "\nA exclusão é permanente: a conta vai ser apagada.\n";
    } else {
        cout << "\nA exclusão é permanente: a conta e as " << movimentacoesDaConta
             << " movimentação(ões) dela vão ser apagadas.\n";
    }

    cout << "Deseja realmente excluir? (S/N): ";
    string resposta = lerTexto();
    if (resposta != "S" && resposta != "s") {
        cout << "\n>> Exclusão cancelada.\n";
        return;
    }

    int numeroExcluido = numeroConta[posicao];

    // tiro as movimentações dela: copio pra frente só as que ficam
    int ficaram = 0;
    for (int m = 0; m < quantidadeMovimentacoes; m++) {
        if (contaMovimentacao[m] != numeroExcluido) {
            contaMovimentacao[ficaram] = contaMovimentacao[m];
            tipoMovimentacao[ficaram] = tipoMovimentacao[m];
            valorMovimentacao[ficaram] = valorMovimentacao[m];
            contaTransferencia[ficaram] = contaTransferencia[m];
            ficaram++;
        }
    }
    quantidadeMovimentacoes = ficaram;

    // puxo as contas de trás uma posição pra frente (sem buraco no vetor)
    for (int i = posicao; i < quantidadeContas - 1; i++) {
        numeroConta[i] = numeroConta[i + 1];
        nomeCliente[i] = nomeCliente[i + 1];
        cpf[i] = cpf[i + 1];
        tipoConta[i] = tipoConta[i + 1];
        saldo[i] = saldo[i + 1];
        contaAtiva[i] = contaAtiva[i + 1];
    }
    quantidadeContas--;

    salvarContas();          // a conta sai do contas.txt
    salvarMovimentacoes();   // e as movimentações dela saem do movimentacoes.txt

    cout << "\n>> Conta nº " << numeroExcluido << " excluída com sucesso.\n";
}
