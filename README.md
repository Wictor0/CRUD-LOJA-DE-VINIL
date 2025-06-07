# 🎵 Loja de Discos - CRUD com GTK em C

Bem-vindo ao nosso sistema de gerenciamento para lojas de discos! Este projeto é um CRUD (Create, Read, Update, Delete) desenvolvido em C com uma interface gráfica construída usando a biblioteca GTK. Ele permite gerenciar um catálogo de discos de forma visual e intuitiva.

## 📚 Sobre o Projeto

Este sistema foi desenvolvido como parte da disciplina de **Programação de software básico em C**, sob a orientação do professor **Guibson Santana**. O objetivo principal era aplicar os conceitos fundamentais da linguagem C na construção de uma aplicação de desktop completa, com interface gráfica e gerenciamento de dados em memória.

---

## 👥 Equipe

| Professor           | Alunos                                                  |
| :------------------ | :------------------------------------------------------ |
| 👨‍🏫 Guibson Santana | 👨‍💻 Caio Porto <br> 👨‍💻 Rhauan Rafael <br> 👨‍💻 Wictor Melo |

---

## ⚙️ Funcionalidades e Características

O sistema foi projetado com as seguintes capacidades e características técnicas:

### **Tópicos Funcionais (O que o sistema faz)**

* **💿 Gerenciamento Completo de Discos (CRUD):**
    * **Cadastrar (Create):** Adiciona novos discos ao catálogo com informações de nome, autor, preço e estoque.
    * **Visualizar (Read):** Exibe todos os discos cadastrados em uma lista clara e organizada.
    * **Atualizar (Update):** Permite selecionar um disco, carregar seus dados, modificar as informações e salvar as alterações.
    * **Deletar (Delete):** Remove um disco selecionado do sistema de forma permanente.

* **🛡️ Validação de Dados:**
    * **Campos Obrigatórios:** Garante que todos os campos sejam preenchidos antes de um cadastro ou atualização.
    * **Regras de Negócio:** Valida se o preço do disco é maior que zero e se o estoque não é um valor negativo.
    * **Unicidade de Registros:** Impede o cadastro de discos com nomes duplicados para manter a integridade dos dados.

* **🖥️ Interação e Feedback com o Usuário:**
    * A interface é atualizada em tempo real após cada operação.
    * O sistema exibe mensagens de sucesso, erro e aviso para guiar o usuário.
    * O botão "Salvar Alterações" é habilitado dinamicamente, apenas quando um item está em modo de edição, melhorando a usabilidade.

### **Tópicos Não Funcionais (Como o sistema funciona)**

* **✨ Usabilidade:**
    * Interface gráfica intuitiva construída com GTK, facilitando o uso sem a necessidade de comandos de terminal.
    * Layout organizado e feedback visual para o usuário.

* **🧠 Gerenciamento de Memória:**
    * Utiliza alocação dinâmica (`malloc` e `free`) para gerenciar os registros dos discos, otimizando o uso de memória.
    * Implementa rotinas para liberar a memória ao deletar registros e ao fechar a aplicação, prevenindo vazamentos de memória (*memory leaks*).

* **🔧 Robustez:**
    * O sistema realiza tratamento de erros, como falhas na alocação de memória e entradas de dados inválidas.

* **⛓️ Dependências e Portabilidade:**
    * O projeto depende da biblioteca **GTK**, podendo ser compilado e executado em diferentes sistemas operacionais (Linux, Windows, etc.) que a suportem.

* **⚠️ Limitações Conhecidas:**
    * **Escalabilidade:** O sistema está limitado a um máximo de **100 discos** em memória.
    * **Persistência de Dados:** Os dados são armazenados apenas em memória. **Ao fechar a aplicação, todas as informações são perdidas.**

---

## 🎓 Principais Aprendizados

* Manipulação avançada de **`structs`**, **ponteiros** e **alocação dinâmica** em C.
* Desenvolvimento de **interfaces gráficas de usuário (GUI)** com a biblioteca **GTK**.
* Implementação do padrão **CRUD** em uma aplicação de desktop.
* Criação de um código modular e organizado em funções com responsabilidades únicas.
* Aplicação de boas práticas de programação, como validação de entradas e gerenciamento de memória.
