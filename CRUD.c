#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char nome[50], autor[50];
  float preco;
  int estoque;
} Disco;

// Create
Disco *criarDisco(char nome[50], char autor[50], float preco, int estoque) {
  Disco *disco = (Disco *)malloc(sizeof(Disco));
  if (disco == NULL) {
    printf("Erro ao alocar memória para o disco.\n");
    return NULL;
  }

  strcpy(disco->nome, nome);
  strcpy(disco->autor, autor);
  disco->preco = preco;
  disco->estoque = estoque;
  return disco;
}

// Read
void lerDisco(Disco *disco) {
  if (disco != NULL) {
    printf("Nome do Disco: %s\n", disco->nome);
    printf("Autor do Disco: %s\n", disco->autor);
    printf("Preço: R$ %.2f\n", disco->preco);
    printf("Estoque: %d\n", disco->estoque);
  } else {
    printf("Disco não encontrado.\n");
  }
}

void listarDisco(Disco *discos[], int numDiscos) {
  if (numDiscos > 0) {
    printf("\n--- Lista de Discos ---\n");
    for (int i = 0; i < numDiscos; i++) {
      printf("Disco %d:\n", i + 1);
      lerDisco(discos[i]);
      printf("\n");
    }
  } else {
    printf("Nenhum disco cadastrado.\n");
  }
}

// Update
void atualizarDisco(Disco *disco, char nome[], char autor[], float preco,
                    int estoque) {
  if (disco != NULL) {
    strcpy(disco->nome, nome);
    strcpy(disco->autor, autor);
    disco->preco = preco;
    disco->estoque = estoque;
    printf("Disco atualizado com sucesso!\n");
  } else {
    printf("Disco não encontrado.\n");
  }
}

// Delete
void deletarDisco(Disco *disco) {
  if (disco != NULL) {
    free(disco);
    printf("Disco deletado com sucesso.\n");
  } else {
    printf("Disco não encontrado.\n");
  }
}

int main() {
  Disco *discos[100];
  int numDiscos = 0;
  int opcao;
  char nome[50], autor[50];
  float preco;
  int estoque;

  do {
    printf("\n--- Loja de Discos ---\n");
    printf("1. Cadastrar disco\n");
    printf("2. Listar discos\n");
    printf("3. Atualizar disco\n");
    printf("4. Deletar disco\n");
    printf("5. Sair\n");
    printf("Escolha uma opção: ");
    scanf("%d", &opcao);
    getchar(); // Limpa o buffer

    switch (opcao) {
    case 1:
      printf("Nome do disco: ");
      fgets(nome, sizeof(nome), stdin);
      nome[strcspn(nome, "\n")] = 0;

      printf("Autor: ");
      fgets(autor, sizeof(autor), stdin);
      autor[strcspn(autor, "\n")] = 0;

      printf("Preço: ");
      scanf("%f", &preco);
      getchar();

      printf("Estoque: ");
      scanf("%d", &estoque);
      getchar();

      discos[numDiscos] = criarDisco(nome, autor, preco, estoque);
      if (discos[numDiscos] != NULL) {
        numDiscos++;
      }
      break;

    case 2:
      listarDisco(discos, numDiscos);
      break;

    case 3: {
      printf("Digite o nome do disco para atualizar: ");
      fgets(nome, sizeof(nome), stdin);
      nome[strcspn(nome, "\n")] = 0;

      Disco *encontrado = NULL;
      for (int i = 0; i < numDiscos; i++) {
        if (strcmp(discos[i]->nome, nome) == 0) {
          encontrado = discos[i];
          break;
        }
      }

      if (encontrado != NULL) {
        printf("Novo nome: ");
        fgets(nome, sizeof(nome), stdin);
        nome[strcspn(nome, "\n")] = 0;

        printf("Novo autor: ");
        fgets(autor, sizeof(autor), stdin);
        autor[strcspn(autor, "\n")] = 0;

        printf("Novo preço: ");
        scanf("%f", &preco);
        getchar();

        printf("Novo estoque: ");
        scanf("%d", &estoque);
        getchar();

        atualizarDisco(encontrado, nome, autor, preco, estoque);
      } else {
        printf("Disco não encontrado.\n");
      }
      break;
    }

    case 4: {
      printf("Digite o nome do disco para deletar: ");
      fgets(nome, sizeof(nome), stdin);
      nome[strcspn(nome, "\n")] = 0;

      int encontrado = -1;
      for (int i = 0; i < numDiscos; i++) {
        if (strcmp(discos[i]->nome, nome) == 0) {
          deletarDisco(discos[i]);
          encontrado = i;
          for (int j = i; j < numDiscos - 1; j++) {
            discos[j] = discos[j + 1];
          }
          numDiscos--;
          break;
        }
      }
      if (encontrado == -1) {
        printf("Disco não encontrado.\n");
      }
      break;
    }

    case 5:
      printf("Encerrando o programa.\n");
      break;

    default:
      printf("Opção inválida.\n");
    }

  } while (opcao != 5);

  // Liberar memória
  for (int i = 0; i < numDiscos; i++) {
    free(discos[i]);
  }

  return 0;
}
