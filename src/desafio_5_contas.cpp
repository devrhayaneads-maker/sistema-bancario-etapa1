// NOVABANK - desafio do item 3.5
// INF101 - Programacao de Computadores I - UNIVICOSA
// Rhayane Freitas
//
// A pergunta era: e se quisessemos cadastrar ate 5 contas?
// Com variavel individual eu teria que criar numeroConta1, numeroConta2 ate
// numeroConta5 e repetir isso pros outros 5 campos, dando 30 variaveis, e
// ainda copiar o menu inteiro pra cada uma. Com vetor da pra guardar as 5 na
// mesma declaracao e mudar so o indice, entao fiz assim.
//
// Usei vetores paralelos: a posicao i de todos os vetores e a mesma conta.

#include <iostream>
#include <string>
#include <iomanip>
#include <limits>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    const int MAX_CONTAS = 5;   // mudando so esse numero o sistema aceita mais contas

    // os mesmos campos da Etapa 1, agora um vetor pra cada um
    int numeroConta[MAX_CONTAS];
    string nomeCliente[MAX_CONTAS];
    string cpf[MAX_CONTAS];
    int tipoConta[MAX_CONTAS];
    double saldo[MAX_CONTAS];
    bool contaAtiva[MAX_CONTAS];
    int agencia[MAX_CONTAS];
    string dataAbertura[MAX_CONTAS];

    int totalContas = 0;   // quantas posicoes do vetor ja estao preenchidas
    int opcao = 0;

    do {
        cout << "\n";
        cout << "***********************************************\n";
        cout << "**                 NOVABANK                  **\n";
        cout << "**    Sistema de Contas Bancárias (até 5)    **\n";
        cout << "***********************************************\n";
        cout << " Contas cadastradas: " << totalContas << " de " << MAX_CONTAS << "\n";
        cout << "***********************************************\n";
        cout << " 1 - Cadastrar conta\n";
        cout << " 2 - Consultar conta\n";
        cout << " 3 - Verificar saldo\n";
        cout << " 4 - Alterar tipo da conta\n";
        cout << " 5 - Ativar/Desativar conta\n";
        cout << " 6 - Sair\n";
        cout << "***********************************************\n";
        cout << "Escolha uma opção: ";

        cin >> opcao;
        if (cin.fail()) {
            if (cin.eof()) return 0;   // a entrada acabou, nao da pra perguntar de novo
            cin.clear();
            opcao = 0;
        } else {
            // pula o espaco que sobrou depois do numero
            while (cin.peek() == ' ' || cin.peek() == '\t') {
                cin.get();
            }
            // se ainda sobrou alguma coisa, foi letra colada no numero (ex: 12abc)
            if (!cin.eof() && cin.peek() != '\n') {
                opcao = 0;
            }
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        // as opcoes 2 a 5 mexem em uma conta so, entao antes do switch eu
        // pergunto qual e e procuro em que posicao ela esta guardada
        int pos = -1;   // -1 quer dizer que nenhuma conta foi escolhida

        if (opcao >= 2 && opcao <= 5) {
            if (totalContas == 0) {
                cout << "\n>> Nenhuma conta cadastrada. Use a opção 1.\n";
            } else {
                cout << "\nContas disponíveis: ";
                for (int i = 0; i < totalContas; i++) {
                    cout << numeroConta[i] << " ";
                }

                int busca = 0;
                cout << "\nInforme o número da conta: ";
                cin >> busca;
                if (cin.fail()) {
                    if (cin.eof()) return 0;   // a entrada acabou, nao da pra perguntar de novo
                    cin.clear();
                    busca = 0;
                } else {
                    // pula o espaco que sobrou depois do numero
                    while (cin.peek() == ' ' || cin.peek() == '\t') {
                        cin.get();
                    }
                    // se ainda sobrou alguma coisa, foi letra colada no numero (ex: 12abc)
                    if (!cin.eof() && cin.peek() != '\n') {
                        busca = 0;
                    }
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                // busca sequencial: olho uma posicao de cada vez ate achar
                for (int i = 0; i < totalContas; i++) {
                    if (numeroConta[i] == busca) {
                        pos = i;
                    }
                }

                if (pos == -1) {
                    cout << "\n>> Conta " << busca << " não encontrada.\n";
                }
            }
        }

        switch (opcao) {

            // 1 - cadastrar
            case 1: {
                // o vetor tem tamanho fixo, entao preciso testar se ainda cabe
                if (totalContas >= MAX_CONTAS) {
                    cout << "\n>> Limite de " << MAX_CONTAS << " contas atingido.\n";
                    break;
                }

                int i = totalContas;   // proxima posicao livre
                cout << "\n--------- CADASTRO DE CONTA ---------\n";

                bool numeroValido;
                do {
                    cout << "Número da conta....: ";
                    cin >> numeroConta[i];
                    if (cin.fail()) {
                        if (cin.eof()) return 0;   // a entrada acabou, nao da pra perguntar de novo
                        cin.clear();
                        numeroConta[i] = 0;
                    } else {
                        // pula o espaco que sobrou depois do numero
                        while (cin.peek() == ' ' || cin.peek() == '\t') {
                            cin.get();
                        }
                        // se ainda sobrou alguma coisa, foi letra colada no numero (ex: 12abc)
                        if (!cin.eof() && cin.peek() != '\n') {
                            numeroConta[i] = 0;
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "   >> Inválido! Não digite letras junto do número.\n";
                            numeroValido = false;
                            continue;   // volta a perguntar sem exibir o outro aviso
                        }
                    }
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    numeroValido = (numeroConta[i] > 0);
                    if (!numeroValido) {
                        cout << "   >> Inválido! O número da conta deve ser maior que zero.\n";
                    } else {
                        // aqui percorro as contas que ja existem pra nao repetir numero
                        for (int j = 0; j < totalContas; j++) {
                            if (numeroConta[j] == numeroConta[i]) {
                                numeroValido = false;
                            }
                        }
                        if (!numeroValido) {
                            cout << "   >> Esse número de conta já está cadastrado.\n";
                        }
                    }
                } while (!numeroValido);

                do {
                    cout << "Agência............: ";
                    cin >> agencia[i];
                    if (cin.fail()) {
                        if (cin.eof()) return 0;   // a entrada acabou, nao da pra perguntar de novo
                        cin.clear();
                        agencia[i] = 0;
                    } else {
                        // pula o espaco que sobrou depois do numero
                        while (cin.peek() == ' ' || cin.peek() == '\t') {
                            cin.get();
                        }
                        // se ainda sobrou alguma coisa, foi letra colada no numero (ex: 12abc)
                        if (!cin.eof() && cin.peek() != '\n') {
                            agencia[i] = 0;
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "   >> Inválido! Não digite letras junto do número.\n";
                            continue;   // volta a perguntar sem exibir o outro aviso
                        }
                    }
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    if (agencia[i] <= 0) {
                        cout << "   >> Inválido! A agência deve ser maior que zero.\n";
                    }
                } while (agencia[i] <= 0);

                bool nomeValido;
                do {
                    cout << "Nome do titular....: ";
                    getline(cin, nomeCliente[i]);
                    if (cin.fail()) return 0;   // entrada acabou no meio do cadastro

                    // mesma regra da Etapa 1: tem que ter letra e caber em 80
                    nomeValido = false;
                    for (int k = 0; k < (int) nomeCliente[i].length(); k++) {
                        char letra = nomeCliente[i][k];
                        if ((letra >= 'A' && letra <= 'Z') ||
                            (letra >= 'a' && letra <= 'z') ||
                            letra < 0) {   // letra com acento ocupa 2 bytes e vem negativa
                            nomeValido = true;
                        }
                    }
                    if (!nomeValido) {
                        cout << "   >> Inválido! O nome precisa ter letras, ex: Maria Silva\n";
                    } else if (nomeCliente[i].length() > 80) {
                        nomeValido = false;
                        cout << "   >> Inválido! O nome pode ter no máximo 80 caracteres.\n";
                    }
                } while (!nomeValido);

                bool cpfValido;
                do {
                    cout << "CPF (11 dígitos)...: ";
                    getline(cin, cpf[i]);
                    if (cin.fail()) return 0;   // entrada acabou no meio do cadastro

                    cpfValido = (cpf[i].length() == 11);
                    for (int k = 0; k < (int) cpf[i].length() && cpfValido; k++) {
                        if (cpf[i][k] < '0' || cpf[i][k] > '9') {
                            cpfValido = false;
                        }
                    }
                    if (!cpfValido) {
                        cout << "   >> Inválido! Digite exatamente 11 dígitos, sem pontos ou traços.\n";
                    }
                } while (!cpfValido);

                do {
                    cout << "Tipo (1=Corrente, 2=Poupança): ";
                    cin >> tipoConta[i];
                    if (cin.fail()) {
                        if (cin.eof()) return 0;   // a entrada acabou, nao da pra perguntar de novo
                        cin.clear();
                        tipoConta[i] = 0;
                    } else {
                        // pula o espaco que sobrou depois do numero
                        while (cin.peek() == ' ' || cin.peek() == '\t') {
                            cin.get();
                        }
                        // se ainda sobrou alguma coisa, foi letra colada no numero (ex: 12abc)
                        if (!cin.eof() && cin.peek() != '\n') {
                            tipoConta[i] = 0;
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "   >> Inválido! Não digite letras junto do número.\n";
                            continue;   // volta a perguntar sem exibir o outro aviso
                        }
                    }
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    if (tipoConta[i] != 1 && tipoConta[i] != 2) {
                        cout << "   >> Inválido! Digite 1 para Corrente ou 2 para Poupança.\n";
                    }
                } while (tipoConta[i] != 1 && tipoConta[i] != 2);

                // mesma leitura da Etapa 1: como texto, porque o cin >> aceitava
                // 2.000 como 2 reais e 1500,50 como 1500 sem reclamar de nada
                cout << "(centavos com ponto ou vírgula, ex: 1500.50 - sem ponto de milhar)\n";
                bool saldoValido;
                do {
                    cout << "Saldo inicial R$...: ";
                    string valorDigitado;
                    getline(cin, valorDigitado);
                    if (cin.fail()) return 0;   // entrada acabou no meio do cadastro

                    // tira o espaco que a pessoa deixou nas pontas
                    while (!valorDigitado.empty() &&
                           (valorDigitado[0] == ' ' || valorDigitado[0] == '\t')) {
                        valorDigitado.erase(0, 1);
                    }
                    while (!valorDigitado.empty() &&
                           (valorDigitado[valorDigitado.length() - 1] == ' ' ||
                            valorDigitado[valorDigitado.length() - 1] == '\t')) {
                        valorDigitado.erase(valorDigitado.length() - 1, 1);
                    }

                    bool soNumero = true;     // apareceu letra ou simbolo estranho?
                    int digitos = 0;          // quantos algarismos o valor tem
                    bool negativo = false;
                    int separadores = 0;      // quantos . ou , foram digitados
                    int casasDecimais = 0;    // quantos algarismos vieram depois deles

                    for (int c = 0; c < (int) valorDigitado.length(); c++) {
                        char letra = valorDigitado[c];
                        if (letra == '-' && c == 0) {
                            negativo = true;
                        } else if (letra == '.' || letra == ',') {
                            separadores++;
                            casasDecimais = 0;
                        } else if (letra >= '0' && letra <= '9') {
                            digitos++;
                            if (separadores > 0) {
                                casasDecimais++;
                            }
                        } else {
                            soNumero = false;
                        }
                    }

                    saldoValido = false;
                    if (!soNumero || digitos == 0) {
                        cout << "   >> Inválido! Digite apenas números, ex: 1500.50\n";
                    } else if (digitos > 15) {
                        cout << "   >> Inválido! Esse valor é alto demais.\n";
                    } else if (separadores > 1) {
                        cout << "   >> Inválido! Use um separador só, ex: 1500.50\n";
                    } else if (separadores == 1 && casasDecimais > 2) {
                        cout << "   >> Inválido! Os centavos têm no máximo duas casas.\n";
                        cout << "      (dois mil reais se digita 2000, sem ponto de milhar)\n";
                    } else {
                        saldoValido = true;
                    }

                    if (saldoValido) {
                        // monto o numero na mao: vou juntando os algarismos e conto
                        // quantos vieram depois da virgula pra dividir no final
                        saldo[i] = 0.0;
                        double divisor = 1.0;
                        bool depoisDoSeparador = false;

                        for (int c = 0; c < (int) valorDigitado.length(); c++) {
                            char letra = valorDigitado[c];
                            if (letra == '.' || letra == ',') {
                                depoisDoSeparador = true;
                            } else if (letra >= '0' && letra <= '9') {
                                saldo[i] = saldo[i] * 10 + (letra - '0');
                                if (depoisDoSeparador) {
                                    divisor = divisor * 10;
                                }
                            }
                        }

                        saldo[i] = saldo[i] / divisor;
                        if (negativo) {
                            saldo[i] = -saldo[i];
                        }

                        if (saldo[i] < 0) {
                            saldoValido = false;
                            cout << "   >> Inválido! O saldo inicial não pode ser negativo.\n";
                        }
                    }
                } while (!saldoValido);

                if (saldo[i] == 0) {
                    saldo[i] = 0.0;   // digitando -0 o extrato sairia como R$ -0.00
                }

                // A data e um campo que eu acrescentei. Testando vi que ela
                // aceitava qualquer coisa (ate vazio e 99/99/9999), entao confiro
                // o formato dd/mm/aaaa e se o dia existe mesmo naquele mes.
                bool dataValida;
                do {
                    cout << "Data de abertura (dd/mm/aaaa): ";
                    getline(cin, dataAbertura[i]);
                    if (cin.fail()) return 0;   // entrada acabou no meio do cadastro

                    dataValida = (dataAbertura[i].length() == 10 &&
                                  dataAbertura[i][2] == '/' && dataAbertura[i][5] == '/');

                    // fora das barras, tudo tem que ser algarismo
                    for (int c = 0; c < (int) dataAbertura[i].length() && dataValida; c++) {
                        if (c != 2 && c != 5) {
                            if (dataAbertura[i][c] < '0' || dataAbertura[i][c] > '9') {
                                dataValida = false;
                            }
                        }
                    }

                    if (dataValida) {
                        int dia = (dataAbertura[i][0] - '0') * 10 + (dataAbertura[i][1] - '0');
                        int mes = (dataAbertura[i][3] - '0') * 10 + (dataAbertura[i][4] - '0');
                        int ano = (dataAbertura[i][6] - '0') * 1000 + (dataAbertura[i][7] - '0') * 100 +
                                  (dataAbertura[i][8] - '0') * 10 + (dataAbertura[i][9] - '0');

                        // quantos dias tem o mes digitado
                        int diasDoMes = 31;
                        if (mes == 4 || mes == 6 || mes == 9 || mes == 11) {
                            diasDoMes = 30;
                        } else if (mes == 2) {
                            if ((ano % 4 == 0 && ano % 100 != 0) || ano % 400 == 0) {
                                diasDoMes = 29;   // ano bissexto
                            } else {
                                diasDoMes = 28;
                            }
                        }

                        if (mes < 1 || mes > 12 || dia < 1 || dia > diasDoMes ||
                            ano < 1900 || ano > 2100) {
                            dataValida = false;
                        }
                    }

                    if (!dataValida) {
                        cout << "   >> Inválido! Use o formato dd/mm/aaaa, ex: 30/09/2026\n";
                    }
                } while (!dataValida);

                contaAtiva[i] = true;
                totalContas++;   // so agora a posicao i passa a valer

                cout << "\n>> Conta cadastrada com sucesso e ATIVADA!\n";
                cout << ">> Restam " << (MAX_CONTAS - totalContas) << " vaga(s).\n";
                break;
            }

            // 2 - consultar
            case 2:
                if (pos == -1) break;   // a busca la em cima nao achou, ja avisou

                cout << "\n--------- DADOS DA CONTA ---------\n";
                cout << "Número da conta : " << numeroConta[pos]  << "\n";
                cout << "Agência         : " << agencia[pos]      << "\n";
                cout << "Titular         : " << nomeCliente[pos]  << "\n";
                cout << "CPF             : " << cpf[pos]          << "\n";

                cout << "Tipo            : ";
                if (tipoConta[pos] == 1) {
                    cout << "Corrente\n";
                } else {
                    cout << "Poupança\n";
                }

                cout << "Abertura        : " << dataAbertura[pos] << "\n";
                cout << "Saldo           : R$ " << fixed << setprecision(2) << saldo[pos] << "\n";

                cout << "Situação        : ";
                if (contaAtiva[pos]) {
                    cout << "ATIVA\n";
                } else {
                    cout << "INATIVA\n";
                }
                break;

            // 3 - saldo
            case 3:
                if (pos == -1) break;

                if (!contaAtiva[pos]) {
                    cout << "\n>> Operação não permitida: a conta está INATIVA.\n";
                    break;
                }

                cout << "\n--------- SALDO ---------\n";
                cout << "Conta nº " << numeroConta[pos] << " - " << nomeCliente[pos] << "\n";
                cout << "Saldo disponível: R$ " << fixed << setprecision(2) << saldo[pos] << "\n";
                break;

            // 4 - trocar o tipo
            case 4: {
                if (pos == -1) break;

                if (!contaAtiva[pos]) {
                    cout << "\n>> Operação não permitida: a conta está INATIVA.\n";
                    break;
                }

                cout << "\n--------- ALTERAR TIPO ---------\n";
                cout << "Tipo atual: ";
                if (tipoConta[pos] == 1) {
                    cout << "Corrente\n";
                } else {
                    cout << "Poupança\n";
                }

                int novoTipo = 0;
                do {
                    cout << "Novo tipo (1=Corrente, 2=Poupança): ";
                    cin >> novoTipo;
                    if (cin.fail()) {
                        if (cin.eof()) return 0;   // a entrada acabou, nao da pra perguntar de novo
                        cin.clear();
                        novoTipo = 0;
                    } else {
                        // pula o espaco que sobrou depois do numero
                        while (cin.peek() == ' ' || cin.peek() == '\t') {
                            cin.get();
                        }
                        // se ainda sobrou alguma coisa, foi letra colada no numero (ex: 12abc)
                        if (!cin.eof() && cin.peek() != '\n') {
                            novoTipo = 0;
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "   >> Inválido! Não digite letras junto do número.\n";
                            continue;   // volta a perguntar sem exibir o outro aviso
                        }
                    }
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    if (novoTipo != 1 && novoTipo != 2) {
                        cout << "   >> Inválido! Digite 1 ou 2.\n";
                    }
                } while (novoTipo != 1 && novoTipo != 2);

                if (novoTipo == tipoConta[pos]) {
                    cout << "\n>> A conta já era desse tipo. Nada foi alterado.\n";
                } else {
                    tipoConta[pos] = novoTipo;
                    cout << "\n>> Tipo alterado com sucesso para ";
                    if (tipoConta[pos] == 1) {
                        cout << "Corrente.\n";
                    } else {
                        cout << "Poupança.\n";
                    }
                }
                break;
            }

            // 5 - ativa ou desativa
            case 5:
                if (pos == -1) break;

                if (contaAtiva[pos]) {
                    contaAtiva[pos] = false;
                    cout << "\n>> Conta nº " << numeroConta[pos] << " DESATIVADA.\n";
                } else {
                    contaAtiva[pos] = true;
                    cout << "\n>> Conta nº " << numeroConta[pos] << " ATIVADA.\n";
                }
                break;

            case 6:
                cout << "\n>> Encerrando o NOVABANK. Obrigado por utilizar nossos serviços!\n";
                break;

            default:
                cout << "\n>> Opção inválida! Escolha um número de 1 a 6.\n";
                break;
        }

        if (opcao != 6) {
            cout << "\nPressione ENTER para continuar...";
            // uso ignore no lugar de get: se a pessoa digitar alguma coisa antes
            // do ENTER, o texto vai junto e nao sobra pro menu ler como opcao
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

    } while (opcao != 6);

    return 0;
}
