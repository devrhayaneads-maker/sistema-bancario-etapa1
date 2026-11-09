// NOVABANK - Etapa 2
// INF101 - Programação de Computadores I - UNIVIÇOSA
// Rhayane Freitas

#include <iostream>  // cout e getline
#include <string>    // string
#include <iomanip>   // setw e setprecision
#include <cmath>     // round

#ifdef _WIN32        // só no Windows: arruma os acentos no terminal
#include <windows.h>
#endif

using namespace std;   // pra escrever cout em vez de std::cout

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);   // acento no que o programa escreve
    SetConsoleCP(CP_UTF8);         // acento no que a pessoa digita
#endif

    cout << fixed << setprecision(2);   // todo double sai com 2 casas: 1000.00

    // ===== CONTAS (item 2.1) =====
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

    string digitado;   // o que a pessoa digitou
    int tamanho;       // quantos caracteres tem
    int numero;        // o número lido
    int opcao = 0;

    // ===== MENU (repete até a opção ser 0) =====
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

        // ----- LEITURA DE NÚMERO -----
        // esse bloco se repete igualzinho mais 4 vezes. só vale se tiver de 1 a 9
        // algarismos e mais nada; senão o número vira -1 (inválido)
        getline(cin, digitado);
        if (cin.fail()) return 0;   // a entrada acabou: fecho o programa
        numero = 0;
        tamanho = digitado.length();
        if (tamanho == 0 || tamanho > 9) numero = -1;   // vazio ou grande demais
        for (int c = 0; c < tamanho && numero != -1; c++) {
            if (digitado[c] >= '0' && digitado[c] <= '9') {
                numero = numero * 10 + (digitado[c] - '0');   // "25": 2, depois 2*10+5 = 25
            } else {
                numero = -1;   // letra ou símbolo
            }
        }
        opcao = numero;

        // ----- ACHAR A CONTA (opções 3 a 9) -----
        // pergunto o número e procuro a posição dela uma vez só, aqui
        int posicao = -1;   // -1 = não achei

        if (opcao >= 3 && opcao <= 9) {
            if (quantidadeContas == 0) {
                cout << "\n>> Nenhuma conta cadastrada. Use a opção 1.\n";
            } else if (opcao == 6 && quantidadeContas < 2) {
                cout << "\n>> Para transferir é preciso ter pelo menos duas contas cadastradas.\n";
            } else {
                if (opcao == 6) {
                    cout << "\nConta de origem: ";
                } else {
                    cout << "\nInforme o número da conta: ";
                }

                // leitura de número
                getline(cin, digitado);
                if (cin.fail()) return 0;
                numero = 0;
                tamanho = digitado.length();
                if (tamanho == 0 || tamanho > 9) numero = -1;
                for (int c = 0; c < tamanho && numero != -1; c++) {
                    if (digitado[c] >= '0' && digitado[c] <= '9') {
                        numero = numero * 10 + (digitado[c] - '0');
                    } else {
                        numero = -1;
                    }
                }

                if (numero <= 0) {
                    cout << "\n>> Número de conta inválido.\n";
                } else {
                    // busca sequencial: olho conta por conta
                    for (int i = 0; i < quantidadeContas; i++) {
                        if (numeroConta[i] == numero) {
                            posicao = i;
                        }
                    }
                    if (posicao == -1) {
                        cout << "\n>> Conta " << numero << " não encontrada.\n";
                    }
                }
            }
        }

        switch (opcao) {

            // ===== 1 - CADASTRAR =====
            case 1: {
                if (quantidadeContas >= MAX_CONTAS) {
                    cout << "\n>> Limite de " << MAX_CONTAS << " contas atingido.\n";
                    break;
                }

                int i = quantidadeContas;   // a conta nova vai na próxima posição livre
                cout << "\n--------- CADASTRO DE CONTA ---------\n";

                // número: maior que zero e sem repetir
                bool numeroValido;
                do {
                    cout << "Número da conta....: ";

                    // leitura de número
                    getline(cin, digitado);
                    if (cin.fail()) return 0;
                    numero = 0;
                    tamanho = digitado.length();
                    if (tamanho == 0 || tamanho > 9) numero = -1;
                    for (int c = 0; c < tamanho && numero != -1; c++) {
                        if (digitado[c] >= '0' && digitado[c] <= '9') {
                            numero = numero * 10 + (digitado[c] - '0');
                        } else {
                            numero = -1;
                        }
                    }
                    numeroConta[i] = numero;

                    numeroValido = (numeroConta[i] > 0);
                    if (!numeroValido) {
                        cout << "   >> Inválido! O número da conta deve ser um inteiro maior que zero.\n";
                    } else {
                        for (int j = 0; j < quantidadeContas; j++) {   // já existe?
                            if (numeroConta[j] == numeroConta[i]) {
                                numeroValido = false;
                            }
                        }
                        if (!numeroValido) {
                            cout << "   >> Esse número de conta já está cadastrado.\n";
                        }
                    }
                } while (!numeroValido);

                // nome: pelo menos uma letra, até 80 caracteres
                bool nomeValido;
                do {
                    cout << "Nome do titular....: ";
                    getline(cin, nomeCliente[i]);
                    if (cin.fail()) return 0;

                    nomeValido = false;
                    tamanho = nomeCliente[i].length();
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
                    getline(cin, cpf[i]);
                    if (cin.fail()) return 0;

                    tamanho = cpf[i].length();
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

                    // leitura de número
                    getline(cin, digitado);
                    if (cin.fail()) return 0;
                    numero = 0;
                    tamanho = digitado.length();
                    if (tamanho == 0 || tamanho > 9) numero = -1;
                    for (int c = 0; c < tamanho && numero != -1; c++) {
                        if (digitado[c] >= '0' && digitado[c] <= '9') {
                            numero = numero * 10 + (digitado[c] - '0');
                        } else {
                            numero = -1;
                        }
                    }
                    tipoConta[i] = numero;

                    if (tipoConta[i] != 1 && tipoConta[i] != 2) {
                        cout << "   >> Inválido! Digite 1 para Corrente ou 2 para Poupança.\n";
                    }
                } while (tipoConta[i] != 1 && tipoConta[i] != 2);

                // ----- LEITURA DE DINHEIRO (se repete no depósito/saque/transferência) -----
                // aceita vírgula ou ponto nos centavos. junto os algarismos e divido
                // no final: "1500,50" vira 150050 / 100 = 1500.50
                cout << "(centavos com ponto ou vírgula, ex: 1500.50 - sem ponto de milhar)\n";
                bool saldoValido = false;
                do {
                    cout << "Saldo inicial R$...: ";
                    getline(cin, digitado);
                    if (cin.fail()) return 0;

                    double valorLido = 0;
                    double divisor = 1;
                    int casas = -1;            // casas depois da vírgula (-1 = não teve vírgula)
                    int algarismos = 0;
                    bool soNumero = true;
                    tamanho = digitado.length();
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
                    } else {
                        saldo[i] = valorLido;   // no saldo inicial o zero vale
                        saldoValido = true;
                    }
                } while (!saldoValido);

                saldoInicial[i] = saldo[i];
                contaAtiva[i] = true;
                quantidadeContas++;   // só agora a conta passa a contar

                cout << "\n>> Conta " << numeroConta[i] << " cadastrada com sucesso e ATIVADA!\n";
                break;
            }

            // ===== 2 - LISTAR =====
            case 2:
                if (quantidadeContas == 0) {
                    cout << "\n>> Nenhuma conta cadastrada. Use a opção 1.\n";
                    break;
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
                break;

            // ===== 3 - CONSULTAR =====
            case 3: {
                if (posicao == -1) break;   // não achou a conta (já avisou)

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
                break;
            }

            // ===== 4, 5 e 6 - DEPÓSITO, SAQUE E TRANSFERÊNCIA =====
            // os três fazem quase tudo igual, então ficam juntos
            case 4:
            case 5:
            case 6: {
                if (posicao == -1) break;

                if (!contaAtiva[posicao]) {
                    if (opcao == 6) {
                        cout << "\n>> Operação não permitida: a conta de origem está INATIVA.\n";
                    } else {
                        cout << "\n>> Operação não permitida: a conta está INATIVA.\n";
                    }
                    break;
                }

                // confiro o espaço no histórico antes de mexer no saldo
                int registros = 1;
                if (opcao == 6) {
                    registros = 2;   // transferência grava 2
                }
                if (quantidadeMovimentacoes + registros > MAX_MOVIMENTACOES) {
                    if (quantidadeMovimentacoes < MAX_MOVIMENTACOES) {
                        cout << "\n>> Sem espaço no histórico para a transferência: ela grava 2 movimentações e só resta 1.\n";
                    } else {
                        cout << "\n>> Histórico de movimentações cheio (limite de "
                             << MAX_MOVIMENTACOES << ").\n";
                    }
                    break;
                }

                // destino (só na transferência): existe, é outra conta e está ativo
                int destino = -1;
                if (opcao == 6) {
                    cout << "Conta de destino: ";

                    // leitura de número
                    getline(cin, digitado);
                    if (cin.fail()) return 0;
                    numero = 0;
                    tamanho = digitado.length();
                    if (tamanho == 0 || tamanho > 9) numero = -1;
                    for (int c = 0; c < tamanho && numero != -1; c++) {
                        if (digitado[c] >= '0' && digitado[c] <= '9') {
                            numero = numero * 10 + (digitado[c] - '0');
                        } else {
                            numero = -1;
                        }
                    }

                    if (numero <= 0) {
                        cout << "\n>> Número de conta inválido.\n";
                        break;
                    }
                    for (int i = 0; i < quantidadeContas; i++) {
                        if (numeroConta[i] == numero) {
                            destino = i;
                        }
                    }
                    if (destino == -1) {
                        cout << "\n>> Conta " << numero << " não encontrada.\n";
                        break;
                    }
                    if (destino == posicao) {
                        cout << "\n>> A conta de destino precisa ser diferente da conta de origem.\n";
                        break;
                    }
                    if (!contaAtiva[destino]) {
                        cout << "\n>> Operação não permitida: a conta de destino está INATIVA.\n";
                        break;
                    }
                }

                if (opcao == 4) {
                    cout << "\n--------- DEPÓSITO ---------\n";
                } else if (opcao == 5) {
                    cout << "\n--------- SAQUE ---------\n";
                } else {
                    cout << "\n--------- TRANSFERÊNCIA ---------\n";
                }

                if (opcao == 6) {
                    cout << "De:   conta nº " << numeroConta[posicao] << " - " << nomeCliente[posicao] << "\n";
                    cout << "Para: conta nº " << numeroConta[destino] << " - " << nomeCliente[destino] << "\n";
                    cout << "Saldo da conta de origem: R$ " << saldo[posicao] << "\n";
                } else {
                    cout << "Conta nº " << numeroConta[posicao] << " - " << nomeCliente[posicao] << "\n";
                    cout << "Saldo atual: R$ " << saldo[posicao] << "\n";
                }

                // leitura de dinheiro (igual ao saldo inicial, mas aqui o zero não vale)
                double valor = 0;
                cout << "(centavos com ponto ou vírgula, ex: 1500.50 - sem ponto de milhar)\n";
                bool valorValido = false;
                do {
                    cout << "Valor R$: ";
                    getline(cin, digitado);
                    if (cin.fail()) return 0;

                    double valorLido = 0;
                    double divisor = 1;
                    int casas = -1;
                    int algarismos = 0;
                    bool soNumero = true;
                    tamanho = digitado.length();
                    for (int c = 0; c < tamanho; c++) {
                        if (digitado[c] >= '0' && digitado[c] <= '9') {
                            algarismos++;
                            valorLido = valorLido * 10 + (digitado[c] - '0');
                            if (casas >= 0) {
                                casas++;
                                divisor = divisor * 10;
                            }
                        } else if ((digitado[c] == '.' || digitado[c] == ',') && casas == -1) {
                            casas = 0;
                        } else {
                            soNumero = false;
                        }
                    }
                    valorLido = valorLido / divisor;

                    if (!soNumero || algarismos == 0) {
                        cout << "   >> Inválido! Digite apenas números, ex: 1500.50\n";
                    } else if (casas > 2) {
                        cout << "   >> Inválido! Os centavos têm no máximo duas casas.\n";
                        cout << "      (dois mil reais se digita 2000, sem ponto de milhar)\n";
                    } else if (valorLido > VALOR_MAXIMO) {
                        cout << "   >> Inválido! O valor máximo por operação é R$ " << VALOR_MAXIMO << "\n";
                    } else if (valorLido == 0) {
                        cout << "   >> Inválido! O valor deve ser maior que zero.\n";
                    } else {
                        valor = valorLido;
                        valorValido = true;
                    }
                } while (!valorValido);

                // conta com dinheiro sempre em centavos: round(x * 100) faz 0.70 virar 70.
                // no double, 0.70 + 0.10 dava 0.7999999 e sacar 0.80 dava "insuficiente"
                if (opcao == 4) {
                    // depósito: soma e grava um 'D' (item 2.4)
                    saldo[posicao] = (round(saldo[posicao] * 100) + round(valor * 100)) / 100;

                    contaMovimentacao[quantidadeMovimentacoes] = numeroConta[posicao];
                    tipoMovimentacao[quantidadeMovimentacoes] = 'D';
                    valorMovimentacao[quantidadeMovimentacoes] = valor;
                    contaTransferencia[quantidadeMovimentacoes] = 0;
                    quantidadeMovimentacoes++;

                    cout << "\n>> Depósito realizado! Novo saldo: R$ " << saldo[posicao] << "\n";

                } else if (round(valor * 100) > round(saldo[posicao] * 100)) {
                    // saque ou transferência maior que o saldo
                    cout << "\n>> Saldo insuficiente! Saldo disponível: R$ " << saldo[posicao] << "\n";

                } else if (opcao == 5) {
                    // saque: tira e grava um 'S'
                    saldo[posicao] = (round(saldo[posicao] * 100) - round(valor * 100)) / 100;

                    contaMovimentacao[quantidadeMovimentacoes] = numeroConta[posicao];
                    tipoMovimentacao[quantidadeMovimentacoes] = 'S';
                    valorMovimentacao[quantidadeMovimentacoes] = valor;
                    contaTransferencia[quantidadeMovimentacoes] = 0;
                    quantidadeMovimentacoes++;

                    cout << "\n>> Saque realizado! Novo saldo: R$ " << saldo[posicao] << "\n";

                } else {
                    // transferência: tira da origem, põe no destino e grava 2 registros (item 2.5)
                    saldo[posicao] = (round(saldo[posicao] * 100) - round(valor * 100)) / 100;
                    saldo[destino] = (round(saldo[destino] * 100) + round(valor * 100)) / 100;

                    contaMovimentacao[quantidadeMovimentacoes] = numeroConta[posicao];   // 'S' na origem
                    tipoMovimentacao[quantidadeMovimentacoes] = 'S';
                    valorMovimentacao[quantidadeMovimentacoes] = valor;
                    contaTransferencia[quantidadeMovimentacoes] = numeroConta[destino];

                    contaMovimentacao[quantidadeMovimentacoes + 1] = numeroConta[destino];   // 'D' no destino
                    tipoMovimentacao[quantidadeMovimentacoes + 1] = 'D';
                    valorMovimentacao[quantidadeMovimentacoes + 1] = valor;
                    contaTransferencia[quantidadeMovimentacoes + 1] = numeroConta[posicao];

                    quantidadeMovimentacoes = quantidadeMovimentacoes + 2;

                    cout << "\n>> Transferência realizada!\n";
                    cout << "   Conta nº " << numeroConta[posicao] << " - " << nomeCliente[posicao]
                         << ": R$ " << saldo[posicao] << "\n";
                    cout << "   Conta nº " << numeroConta[destino] << " - " << nomeCliente[destino]
                         << ": R$ " << saldo[destino] << "\n";
                }
                break;
            }

            // ===== 7 - EXTRATO (item 2.6) =====
            case 7: {
                if (posicao == -1) break;

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
                break;
            }

            // ===== 8 - ATIVAR/DESATIVAR =====
            case 8:
                if (posicao == -1) break;

                if (contaAtiva[posicao]) {
                    contaAtiva[posicao] = false;
                    cout << "\n>> Conta nº " << numeroConta[posicao] << " DESATIVADA.\n";
                } else {
                    contaAtiva[posicao] = true;
                    cout << "\n>> Conta nº " << numeroConta[posicao] << " ATIVADA.\n";
                }
                break;

            // ===== 9 - EXCLUIR =====
            case 9: {
                if (posicao == -1) break;

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
                    break;
                }
                // hoje nunca acontece; fica pronto pro dia que tiver cheque especial
                if (saldo[posicao] < 0) {
                    cout << "\n>> Não é possível excluir: a conta está com saldo negativo.\n";
                    cout << "   Regularize as pendências antes de encerrar a conta.\n";
                    break;
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
                getline(cin, digitado);
                if (cin.fail()) return 0;

                if (digitado != "S" && digitado != "s") {
                    cout << "\n>> Exclusão cancelada.\n";
                    break;
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
                break;
            }

            // ===== 0 - SAIR (confirmo porque nada fica salvo) =====
            case 0:
                cout << "\nAs contas e movimentações ficam só na memória e serão perdidas.\n";
                cout << "Deseja realmente sair? (S/N): ";
                getline(cin, digitado);
                if (cin.fail()) return 0;

                if (digitado == "S" || digitado == "s") {
                    cout << "\n>> Encerrando o NOVABANK. Obrigado por utilizar nossos serviços!\n";
                } else {
                    opcao = -1;   // pro do-while não terminar
                    cout << "\n>> Saída cancelada.\n";
                }
                break;

            default:   // qualquer outro número, ou -1 de letra digitada
                cout << "\n>> Opção inválida! Escolha um número de 0 a 9.\n";
                break;
        }

        // pausa pra pessoa ler antes do menu voltar
        if (opcao != 0) {
            cout << "\nPressione ENTER para continuar...";
            getline(cin, digitado);
            if (cin.fail()) return 0;
        }

    } while (opcao != 0);

    return 0;
}
