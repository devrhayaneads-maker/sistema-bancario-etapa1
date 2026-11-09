// NOVABANK - Etapa 2
// INF101 - Programação de Computadores I - UNIVIÇOSA
// Rhayane Freitas

#include <iostream>  // cout e getline
#include <string>    // string
#include <iomanip>   // setw e setprecision
#include <cmath>     // round
#include <cstdlib>   // exit

#ifdef _WIN32        // só no Windows: arruma os acentos no terminal
#include <windows.h>
#endif

using namespace std;   // pra escrever cout em vez de std::cout

// ===== CONTAS (item 2.1) =====
// globais (fora do main) pra todas as funções enxergarem.
// vetores paralelos: a mesma posição em todos é a mesma conta
const int MAX_CONTAS = 100;
int numeroConta[MAX_CONTAS];
string nomeCliente[MAX_CONTAS];
string cpf[MAX_CONTAS];
int tipoConta[MAX_CONTAS];         // 1 = corrente, 2 = poupança
double saldo[MAX_CONTAS];
bool contaAtiva[MAX_CONTAS];
double saldoInicial[MAX_CONTAS];   // acrescentei, pro extrato
int quantidadeContas = 0;          // também é a próxima posição livre

// ===== HISTÓRICO (item 2.2) =====
const int MAX_MOVIMENTACOES = 500;
int contaMovimentacao[MAX_MOVIMENTACOES];     // número da conta
char tipoMovimentacao[MAX_MOVIMENTACOES];     // 'D' = entrada, 'S' = saída
double valorMovimentacao[MAX_MOVIMENTACOES];
int contaTransferencia[MAX_MOVIMENTACOES];    // acrescentei: a outra conta da transferência, ou 0
int quantidadeMovimentacoes = 0;

const double VALOR_MAXIMO = 1000000000.00;   // limite por operação

// ===== PROTÓTIPOS =====
// aviso ao compilador que essas funções existem (o código delas vem depois do main)

// as que o enunciado sugeriu
void cadastrarConta();
void listarContas();
void consultarConta();
void depositar();
void sacar();
void transferir();
void registrarMovimentacao(int conta, char tipo, double valor, int outraConta);
void exibirExtrato();

// as que eu criei
void ativarDesativarConta();
void excluirConta();
string lerTexto();
int lerNumero();
double lerValor(string pergunta, bool aceitaZero);
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

            case 0: {   // confirmo porque nada fica salvo
                cout << "\nAs contas e movimentações ficam só na memória e serão perdidas.\n";
                cout << "Deseja realmente sair? (S/N): ";
                string resposta = lerTexto();

                if (resposta == "S" || resposta == "s") {
                    cout << "\n>> Encerrando o NOVABANK. Obrigado por utilizar nossos serviços!\n";
                } else {
                    opcao = -1;   // pro do-while não terminar
                    cout << "\n>> Saída cancelada.\n";
                }
                break;
            }

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

// ===== LEITURAS =====

// lê a linha inteira que a pessoa digitou
string lerTexto() {
    string digitado;
    getline(cin, digitado);
    if (cin.fail()) exit(0);   // a entrada acabou: fecho o programa inteiro
    return digitado;
}

// lê um número: só vale se tiver de 1 a 9 algarismos e mais nada. senão devolve -1
int lerNumero() {
    string digitado = lerTexto();
    int tamanho = digitado.length();
    if (tamanho == 0 || tamanho > 9) return -1;   // vazio ou grande demais

    int numero = 0;
    for (int c = 0; c < tamanho; c++) {
        if (digitado[c] < '0' || digitado[c] > '9') return -1;   // letra ou símbolo
        numero = numero * 10 + (digitado[c] - '0');               // "25": 2, depois 2*10+5 = 25
    }
    return numero;
}

// lê dinheiro, com ponto ou vírgula nos centavos. pergunta até vir um valor válido.
// aceitaZero diz se o 0 vale (só no saldo inicial)
double lerValor(string pergunta, bool aceitaZero) {
    double valor = 0;
    bool valido = false;

    cout << "(centavos com ponto ou vírgula, ex: 1500.50 - sem ponto de milhar)\n";
    do {
        cout << pergunta;
        string digitado = lerTexto();

        // junto os algarismos e divido no final: "1500,50" vira 150050 / 100 = 1500.50
        double valorLido = 0;
        double divisor = 1;
        int casas = -1;            // casas depois da vírgula (-1 = não teve vírgula)
        int algarismos = 0;
        bool soNumero = true;
        int tamanho = digitado.length();
        for (int c = 0; c < tamanho; c++) {
            if (digitado[c] >= '0' && digitado[c] <= '9') {
                algarismos++;
                valorLido = valorLido * 10 + (digitado[c] - '0');
                if (casas >= 0) {   // já passou da vírgula
                    casas++;
                    divisor = divisor * 10;
                }
            } else if ((digitado[c] == '.' || digitado[c] == ',') && casas == -1) {
                casas = 0;          // achei a vírgula
            } else {
                soNumero = false;   // letra, sinal de menos, segunda vírgula...
            }
        }
        valorLido = valorLido / divisor;

        if (!soNumero || algarismos == 0) {
            cout << "   >> Inválido! Digite apenas números, ex: 1500.50\n";
        } else if (casas > 2) {   // ex: 2.000
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

// grava uma linha no histórico (quem chama já conferiu se tem espaço)
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

    // nome: pelo menos uma letra, até 80 caracteres
    bool nomeValido;
    do {
        cout << "Nome do titular....: ";
        nomeCliente[i] = lerTexto();

        nomeValido = false;
        int tamanho = nomeCliente[i].length();
        for (int c = 0; c < tamanho; c++) {
            char letra = nomeCliente[i][c];
            if ((letra >= 'A' && letra <= 'Z') ||
                (letra >= 'a' && letra <= 'z') ||
                letra < 0) {   // letra com acento vem negativa
                nomeValido = true;
            }
        }
        if (!nomeValido) {
            cout << "   >> Inválido! O nome precisa ter letras, ex: Maria Silva\n";
        } else if (tamanho > 80) {
            nomeValido = false;
            cout << "   >> Inválido! O nome pode ter no máximo 80 caracteres.\n";
        }
    } while (!nomeValido);

    // CPF: exatamente 11 algarismos
    bool cpfValido;
    do {
        cout << "CPF (11 dígitos)...: ";
        cpf[i] = lerTexto();

        int tamanho = cpf[i].length();
        cpfValido = (tamanho == 11);
        for (int c = 0; c < tamanho; c++) {
            if (cpf[i][c] < '0' || cpf[i][c] > '9') {
                cpfValido = false;
            }
        }
        if (!cpfValido) {
            cout << "   >> Inválido! Digite exatamente 11 dígitos, sem pontos, traços ou espaços.\n";
        }
    } while (!cpfValido);

    // tipo: 1 ou 2
    do {
        cout << "Tipo (1=Corrente, 2=Poupança): ";
        tipoConta[i] = lerNumero();
        if (tipoConta[i] != 1 && tipoConta[i] != 2) {
            cout << "   >> Inválido! Digite 1 para Corrente ou 2 para Poupança.\n";
        }
    } while (tipoConta[i] != 1 && tipoConta[i] != 2);

    saldo[i] = lerValor("Saldo inicial R$...: ", true);   // true: aqui o 0 vale
    saldoInicial[i] = saldo[i];
    contaAtiva[i] = true;
    quantidadeContas++;   // só agora a conta passa a contar

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

// ===== 4 - DEPOSITAR (grava um 'D', item 2.4) =====
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

    cout << "\n>> Transferência realizada!\n";
    cout << "   Conta nº " << numeroConta[origem] << " - " << nomeCliente[origem]
         << ": R$ " << saldo[origem] << "\n";
    cout << "   Conta nº " << numeroConta[destino] << " - " << nomeCliente[destino]
         << ": R$ " << saldo[destino] << "\n";
}

// ===== 7 - EXTRATO (item 2.6) =====
void exibirExtrato() {
    int posicao = pedirConta("\nInforme o número da conta: ");
    if (posicao == -1) return;

    cout << "\n--------- EXTRATO ---------\n";
    cout << "Conta nº " << numeroConta[posicao] << " - " << nomeCliente[posicao];
    if (!contaAtiva[posicao]) {
        cout << " (INATIVA)";
    }
    cout << "\n\n";
    cout << "Saldo inicial: R$ " << saldoInicial[posicao] << "\n";

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
        saldoInicial[i] = saldoInicial[i + 1];
    }
    quantidadeContas--;

    cout << "\n>> Conta nº " << numeroExcluido << " excluída com sucesso.\n";
}
