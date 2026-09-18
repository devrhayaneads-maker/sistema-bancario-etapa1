// NOVABANK - Etapa 1
// INF101 - Programacao de Computadores I - UNIVICOSA
// Rhayane Freitas
//
// Cadastro de uma conta bancaria usando so variaveis individuais,
// if/else, switch e do-while. Vetor e funcao sao da Etapa 2, entao
// nao usei nenhum dos dois aqui.

#include <iostream>
#include <string>
#include <iomanip>   // setprecision, pra mostrar o saldo com 2 casas
#include <limits>    // numeric_limits, usado no cin.ignore

#ifdef _WIN32
#include <windows.h> // sem isso os acentos saem errados no terminal do Windows
#endif

using namespace std;

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    // as seis variaveis pedidas no item 3.1
    int numeroConta = 0;
    string nomeCliente = "";
    string cpf = "";
    int tipoConta = 0;   // 1 corrente, 2 poupanca
    double saldo = 0.0;
    bool contaAtiva = false;

    // essas tres eu acrescentei
    int agencia = 0;
    string dataAbertura = "";
    bool contaCadastrada = false;   // serve pra saber se as variaveis ja foram preenchidas

    int opcao = 0;

    do {
        cout << "\n";
        cout << "***********************************************\n";
        cout << "**                 NOVABANK                  **\n";
        cout << "**       Sistema de Contas Bancárias         **\n";
        cout << "***********************************************\n";
        cout << " 1 - Cadastrar conta\n";
        cout << " 2 - Consultar conta\n";
        cout << " 3 - Verificar saldo\n";
        cout << " 4 - Alterar tipo da conta\n";
        cout << " 5 - Ativar/Desativar conta\n";
        cout << " 6 - Sair\n";
        cout << " 7 - Remover conta\n";
        cout << "***********************************************\n";
        cout << "Escolha uma opção: ";

        cin >> opcao;
        if (cin.fail()) {   // se digitar letra o cin trava, entao limpo o erro
            if (cin.eof()) return 0;   // a entrada acabou, nao da pra perguntar de novo
            cin.clear();
            opcao = 0;      // assim cai no default e avisa
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
        cin.ignore(numeric_limits<streamsize>::max(), '\n');   // joga fora o resto da linha

        switch (opcao) {

            // 1 - cadastrar
            case 1: {
                cout << "\n--------- CADASTRO DE CONTA ---------\n";

                // como so existe um conjunto de variaveis, cadastrar de novo
                // apaga a conta anterior. por isso pergunto antes.
                if (contaCadastrada) {
                    char resposta;
                    cout << "Já existe a conta nº " << numeroConta << " cadastrada.\n";
                    cout << "Deseja substituir os dados? (S/N): ";
                    cin >> resposta;
                    if (cin.fail()) {
                        if (cin.eof()) return 0;   // a entrada acabou, nao da pra perguntar de novo
                        cin.clear();
                        resposta = 'N';
                    }
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    if (resposta != 'S' && resposta != 's') {
                        cout << "\n>> Cadastro cancelado.\n";
                        break;
                    }
                }

                // numero tem que ser maior que zero (item 3.4)
                do {
                    cout << "Número da conta....: ";
                    cin >> numeroConta;
                    if (cin.fail()) {
                        if (cin.eof()) return 0;   // a entrada acabou, nao da pra perguntar de novo
                        cin.clear();
                        numeroConta = 0;
                    } else {
                        // pula o espaco que sobrou depois do numero
                        while (cin.peek() == ' ' || cin.peek() == '\t') {
                            cin.get();
                        }
                        // se ainda sobrou alguma coisa, foi letra colada no numero (ex: 12abc)
                        if (!cin.eof() && cin.peek() != '\n') {
                            numeroConta = 0;
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "   >> Inválido! Não digite letras junto do número.\n";
                            continue;   // volta a perguntar sem exibir o outro aviso
                        }
                    }
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    if (numeroConta <= 0) {
                        cout << "   >> Inválido! O número da conta deve ser maior que zero.\n";
                    }
                } while (numeroConta <= 0);

                // mesma regra pra agencia
                do {
                    cout << "Agência............: ";
                    cin >> agencia;
                    if (cin.fail()) {
                        if (cin.eof()) return 0;   // a entrada acabou, nao da pra perguntar de novo
                        cin.clear();
                        agencia = 0;
                    } else {
                        // pula o espaco que sobrou depois do numero
                        while (cin.peek() == ' ' || cin.peek() == '\t') {
                            cin.get();
                        }
                        // se ainda sobrou alguma coisa, foi letra colada no numero (ex: 12abc)
                        if (!cin.eof() && cin.peek() != '\n') {
                            agencia = 0;
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "   >> Inválido! Não digite letras junto do número.\n";
                            continue;   // volta a perguntar sem exibir o outro aviso
                        }
                    }
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    if (agencia <= 0) {
                        cout << "   >> Inválido! A agência deve ser maior que zero.\n";
                    }
                } while (agencia <= 0);

                // uso getline porque o nome tem espaco no meio
                bool nomeValido;
                do {
                    cout << "Nome do titular....: ";
                    getline(cin, nomeCliente);
                    if (cin.fail()) return 0;   // entrada acabou no meio do cadastro

                    // testando vi que espaco sozinho, 12345 e ate !!! passavam
                    // como nome, entao agora exijo pelo menos uma letra
                    nomeValido = false;
                    for (int c = 0; c < (int) nomeCliente.length(); c++) {
                        char letra = nomeCliente[c];
                        if ((letra >= 'A' && letra <= 'Z') ||
                            (letra >= 'a' && letra <= 'z') ||
                            letra < 0) {   // letra com acento ocupa 2 bytes e vem negativa
                            nomeValido = true;
                        }
                    }
                    if (!nomeValido) {
                        cout << "   >> Inválido! O nome precisa ter letras, ex: Maria Silva\n";
                    } else if (nomeCliente.length() > 80) {
                        nomeValido = false;
                        cout << "   >> Inválido! O nome pode ter no máximo 80 caracteres.\n";
                    }
                } while (!nomeValido);

                bool cpfValido;
                do {
                    cout << "CPF (11 dígitos)...: ";
                    getline(cin, cpf);
                    if (cin.fail()) return 0;   // entrada acabou no meio do cadastro

                    cpfValido = (cpf.length() == 11);
                    // confere caractere por caractere se e digito
                    for (int i = 0; i < (int) cpf.length() && cpfValido; i++) {
                        if (cpf[i] < '0' || cpf[i] > '9') {
                            cpfValido = false;
                        }
                    }
                    if (!cpfValido) {
                        cout << "   >> Inválido! Digite exatamente 11 dígitos, sem pontos ou traços.\n";
                    }
                } while (!cpfValido);

                // so aceita 1 ou 2
                do {
                    cout << "Tipo (1=Corrente, 2=Poupança): ";
                    cin >> tipoConta;
                    if (cin.fail()) {
                        if (cin.eof()) return 0;   // a entrada acabou, nao da pra perguntar de novo
                        cin.clear();
                        tipoConta = 0;
                    } else {
                        // pula o espaco que sobrou depois do numero
                        while (cin.peek() == ' ' || cin.peek() == '\t') {
                            cin.get();
                        }
                        // se ainda sobrou alguma coisa, foi letra colada no numero (ex: 12abc)
                        if (!cin.eof() && cin.peek() != '\n') {
                            tipoConta = 0;
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "   >> Inválido! Não digite letras junto do número.\n";
                            continue;   // volta a perguntar sem exibir o outro aviso
                        }
                    }
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    if (tipoConta != 1 && tipoConta != 2) {
                        cout << "   >> Inválido! Digite 1 para Corrente ou 2 para Poupança.\n";
                    }
                } while (tipoConta != 1 && tipoConta != 2);

                // Leio o saldo como texto pra conferir cada caractere. Testando
                // vi que o cin >> aceitava errado e calado: 1500,50 virava 1500
                // (sumia com os centavos) e 2.000 virava 2 reais. Aqui aceito
                // ponto ou virgula nos centavos e recuso ponto de milhar.
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
                        saldo = 0.0;
                        double divisor = 1.0;
                        bool depoisDoSeparador = false;

                        for (int c = 0; c < (int) valorDigitado.length(); c++) {
                            char letra = valorDigitado[c];
                            if (letra == '.' || letra == ',') {
                                depoisDoSeparador = true;
                            } else if (letra >= '0' && letra <= '9') {
                                saldo = saldo * 10 + (letra - '0');
                                if (depoisDoSeparador) {
                                    divisor = divisor * 10;
                                }
                            }
                        }

                        saldo = saldo / divisor;
                        if (negativo) {
                            saldo = -saldo;
                        }

                        if (saldo < 0) {
                            saldoValido = false;
                            cout << "   >> Inválido! O saldo inicial não pode ser negativo.\n";
                        }
                    }
                } while (!saldoValido);

                if (saldo == 0) {
                    saldo = 0.0;   // digitando -0 o extrato sairia como R$ -0.00
                }

                // A data e um campo que eu acrescentei. Testando vi que ela
                // aceitava qualquer coisa (ate vazio e 99/99/9999), entao confiro
                // o formato dd/mm/aaaa e se o dia existe mesmo naquele mes.
                bool dataValida;
                do {
                    cout << "Data de abertura (dd/mm/aaaa): ";
                    getline(cin, dataAbertura);
                    if (cin.fail()) return 0;   // entrada acabou no meio do cadastro

                    dataValida = (dataAbertura.length() == 10 &&
                                  dataAbertura[2] == '/' && dataAbertura[5] == '/');

                    // fora das barras, tudo tem que ser algarismo
                    for (int c = 0; c < (int) dataAbertura.length() && dataValida; c++) {
                        if (c != 2 && c != 5) {
                            if (dataAbertura[c] < '0' || dataAbertura[c] > '9') {
                                dataValida = false;
                            }
                        }
                    }

                    if (dataValida) {
                        int dia = (dataAbertura[0] - '0') * 10 + (dataAbertura[1] - '0');
                        int mes = (dataAbertura[3] - '0') * 10 + (dataAbertura[4] - '0');
                        int ano = (dataAbertura[6] - '0') * 1000 + (dataAbertura[7] - '0') * 100 +
                                  (dataAbertura[8] - '0') * 10 + (dataAbertura[9] - '0');

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

                contaAtiva = true;   // conta nova ja nasce ativa
                contaCadastrada = true;

                cout << "\n>> Conta cadastrada com sucesso e ATIVADA!\n";
                break;
            }

            // 2 - consultar
            case 2:
                if (!contaCadastrada) {
                    cout << "\n>> Nenhuma conta cadastrada. Use a opção 1.\n";
                    break;
                }

                cout << "\n--------- DADOS DA CONTA ---------\n";
                cout << "Número da conta : " << numeroConta  << "\n";
                cout << "Agência         : " << agencia      << "\n";
                cout << "Titular         : " << nomeCliente  << "\n";
                cout << "CPF             : " << cpf          << "\n";

                cout << "Tipo            : ";
                if (tipoConta == 1) {
                    cout << "Corrente\n";
                } else {
                    cout << "Poupança\n";
                }

                cout << "Abertura        : " << dataAbertura << "\n";
                cout << "Saldo           : R$ " << fixed << setprecision(2) << saldo << "\n";

                cout << "Situação        : ";
                if (contaAtiva) {
                    cout << "ATIVA\n";
                } else {
                    cout << "INATIVA\n";
                }
                break;

            // 3 - saldo
            case 3:
                if (!contaCadastrada) {
                    cout << "\n>> Nenhuma conta cadastrada. Use a opção 1.\n";
                    break;
                }
                // conta desativada nao mostra saldo
                if (!contaAtiva) {
                    cout << "\n>> Operação não permitida: a conta está INATIVA.\n";
                    break;
                }

                cout << "\n--------- SALDO ---------\n";
                cout << "Conta nº " << numeroConta << " - " << nomeCliente << "\n";
                cout << "Saldo disponível: R$ " << fixed << setprecision(2) << saldo << "\n";
                break;

            // 4 - trocar corrente por poupanca e vice-versa
            case 4: {
                if (!contaCadastrada) {
                    cout << "\n>> Nenhuma conta cadastrada. Use a opção 1.\n";
                    break;
                }
                if (!contaAtiva) {
                    cout << "\n>> Operação não permitida: a conta está INATIVA.\n";
                    break;
                }

                cout << "\n--------- ALTERAR TIPO ---------\n";
                cout << "Tipo atual: ";
                if (tipoConta == 1) {
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

                if (novoTipo == tipoConta) {
                    cout << "\n>> A conta já era desse tipo. Nada foi alterado.\n";
                } else {
                    tipoConta = novoTipo;
                    cout << "\n>> Tipo alterado com sucesso para ";
                    if (tipoConta == 1) {
                        cout << "Corrente.\n";
                    } else {
                        cout << "Poupança.\n";
                    }
                }
                break;
            }

            // 5 - ativa se estiver inativa e o contrario
            case 5:
                if (!contaCadastrada) {
                    cout << "\n>> Nenhuma conta cadastrada. Use a opção 1.\n";
                    break;
                }

                if (contaAtiva) {
                    contaAtiva = false;
                    cout << "\n>> Conta nº " << numeroConta << " DESATIVADA.\n";
                } else {
                    contaAtiva = true;
                    cout << "\n>> Conta nº " << numeroConta << " ATIVADA.\n";
                }
                break;

            case 6:
                cout << "\n>> Encerrando o NOVABANK. Obrigado por utilizar nossos serviços!\n";
                break;

            // 7 - opcao que eu coloquei alem das que foram pedidas
            case 7:
                if (!contaCadastrada) {
                    cout << "\n>> Nenhuma conta cadastrada. Use a opção 1.\n";
                    break;
                }

                cout << "\n--------- REMOVER CONTA ---------\n";
                cout << "Conta nº " << numeroConta << " - " << nomeCliente << "\n";
                cout << "Deseja realmente remover esta conta? (S/N): ";

                char respostaRemocao;
                cin >> respostaRemocao;
                if (cin.fail()) {
                    if (cin.eof()) return 0;   // a entrada acabou, nao da pra perguntar de novo
                    cin.clear();
                    respostaRemocao = 'N';
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                if (respostaRemocao != 'S' && respostaRemocao != 's') {
                    cout << "\n>> Remoção cancelada.\n";
                    break;
                }

                // volta tudo pro valor inicial, como se nada tivesse sido cadastrado
                numeroConta = 0;
                agencia = 0;
                nomeCliente = "";
                cpf = "";
                tipoConta = 0;
                saldo = 0.0;
                contaAtiva = false;
                dataAbertura = "";
                contaCadastrada = false;

                cout << "\n>> Conta removida com sucesso.\n";
                break;

            default:
                cout << "\n>> Opção inválida! Escolha um número de 1 a 7.\n";
                break;
        }

        // sem essa pausa o menu volta antes da pessoa ler o resultado
        if (opcao != 6) {
            cout << "\nPressione ENTER para continuar...";
            // uso ignore no lugar de get: se a pessoa digitar alguma coisa antes
            // do ENTER, o texto vai junto e nao sobra pro menu ler como opcao
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

    } while (opcao != 6);

    return 0;
}
