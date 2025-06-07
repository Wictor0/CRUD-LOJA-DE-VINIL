#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h> // Essencial para GTK

typedef struct {
  char nome[50], autor[50];
  float preco;
  int estoque;
} Disco;

// --- Funções CRUD (adaptadas para não imprimir diretamente, exceto erros críticos) ---

Disco *criarDisco(char nome[50], char autor[50], float preco, int estoque) {
  Disco *disco = (Disco *)malloc(sizeof(Disco));
  if (disco == NULL) {
    fprintf(stderr, "Erro ao alocar memória para o disco.\n");
    return NULL;
  }

  // Parte em que as infos são copiadas para a estrutura
  strcpy(disco->nome, nome); 
  strcpy(disco->autor, autor);
  disco->preco = preco;
  disco->estoque = estoque;
  return disco;
}

// Retorna 1 se atualizado, 0 se não encontrado
int atualizarDiscoGUI(Disco *disco, const char nome[], const char autor[], float preco, int estoque) {
  if (disco != NULL) {
    strcpy(disco->nome, nome);
    strcpy(disco->autor, autor);
    disco->preco = preco;
    disco->estoque = estoque;
    return 1; // Sucesso
  }
  return 0; // Não encontrado
}

// Retorna 1 se deletado, 0 se não encontrado
int deletarDiscoGUI(Disco *disco) {
  if (disco != NULL) {
    // A memória é liberada pelo chamador após remover do array
    // free(disco); // Não libere aqui se o ponteiro ainda está no array 'discos'
    return 1; // Sucesso
  }
  return 0; // Não encontrado
}

// Array de Discos e Estado de Edição ---
#define MAX_DISCOS 100
Disco *discos[MAX_DISCOS];
int numDiscos = 0;
int indice_disco_sendo_editado = -1; // -1 significa que nenhum disco está sendo editado

// Widgets da GUI (globais para simplificar o exemplo) ---
GtkWidget *entry_nome;
GtkWidget *entry_autor;
GtkWidget *entry_preco;
GtkWidget *entry_estoque;
GtkWidget *list_discos_view; // GtkTreeView
GtkListStore *list_store;    // Modelo para GtkTreeView
GtkWidget *button_salvar_alteracoes; // Para habilitar/desabilitar

// Protótipos de Callbacks
static void on_cadastrar_button_clicked(GtkWidget *widget, gpointer data);
static void on_deletar_button_clicked(GtkWidget *widget, gpointer data);
static void on_carregar_edicao_button_clicked(GtkWidget *widget, gpointer data);
static void on_salvar_alteracoes_button_clicked(GtkWidget *widget, gpointer data);
void atualizar_lista_gui();
void limpar_campos_entrada();

// Função para limpar os campos de entrada
void limpar_campos_entrada() {
    gtk_entry_set_text(GTK_ENTRY(entry_nome), "");
    gtk_entry_set_text(GTK_ENTRY(entry_autor), "");
    gtk_entry_set_text(GTK_ENTRY(entry_preco), "");
    gtk_entry_set_text(GTK_ENTRY(entry_estoque), "");
}

// Função para atualizar a exibição da lista de discos na GUI
void atualizar_lista_gui() {
    gtk_list_store_clear(list_store);
    GtkTreeIter iter;
    for (int i = 0; i < numDiscos; i++) {
        if (discos[i] != NULL) {
            char preco_str[20];
            char estoque_str[10];
            sprintf(preco_str, "%.2f", discos[i]->preco); // Sem R$ aqui para facilitar parse no 'carregar'
            sprintf(estoque_str, "%d", discos[i]->estoque);

            gtk_list_store_append(list_store, &iter);
            gtk_list_store_set(list_store, &iter,
                               0, discos[i]->nome,
                               1, discos[i]->autor,
                               2, preco_str,
                               3, estoque_str,
                               -1);
        }
    }
}

// Callback para o botão "Cadastrar"
static void on_cadastrar_button_clicked(GtkWidget *widget, gpointer data) {
    const char *nome = gtk_entry_get_text(GTK_ENTRY(entry_nome));
    const char *autor = gtk_entry_get_text(GTK_ENTRY(entry_autor));
    const char *preco_str = gtk_entry_get_text(GTK_ENTRY(entry_preco));
    const char *estoque_str = gtk_entry_get_text(GTK_ENTRY(entry_estoque));

    if (strlen(nome) == 0 || strlen(autor) == 0 || strlen(preco_str) == 0 || strlen(estoque_str) == 0) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(widget)),
                                                 GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK,
                                                 "Todos os campos são obrigatórios!");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }
    // Verificar se já existe um disco com o mesmo nome
    for (int i = 0; i < numDiscos; i++) {
        if (discos[i] && strcmp(discos[i]->nome, nome) == 0) {
            GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(widget)),
                                                     GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK,
                                                     "Já existe um disco com este nome!");
            gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);
            return;
        }
    }


    float preco = atof(preco_str); // converter STR pra float
    int estoque = atoi(estoque_str); //converter STR pra INT

    // Valida os valores de preço e estoque pra caso forem incorretos exibir uma mensagem de erro.
    if (preco <= 0 || estoque < 0) {
         GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(widget)),
                                                 GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK,
                                                 "Preço deve ser maior que zero e estoque não pode ser negativo.");
        gtk_dialog_run(GTK_DIALOG(dialog)); 
        gtk_widget_destroy(dialog);
        return;
    }


    if (numDiscos < MAX_DISCOS) { // Verifica se ainda há espaço no array 'discos' para adicionar um novo disco.
        discos[numDiscos] = criarDisco((char*)nome, (char*)autor, preco, estoque); // Tenta criar um novo disco e o armazena na próxima posição livre do array.
        if (discos[numDiscos] != NULL) { // Verifica se a criação do disco (alocação de memória) foi bem-sucedida.
            numDiscos++; // Incrementa o contador de discos se o cadastro foi um sucesso.
            atualizar_lista_gui(); // Atualiza a exibição da lista de discos na interface gráfica.
            limpar_campos_entrada(); // Limpa os campos de entrada do formulário.
            GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(widget)),
                                                     GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK,
                                                     "Disco cadastrado com sucesso!"); // Cria uma caixa de diálogo de sucesso.
            gtk_dialog_run(GTK_DIALOG(dialog)); // Exibe a caixa de diálogo.
            gtk_widget_destroy(dialog); // Destrói a caixa de diálogo.
        } else {
            GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(widget)),
                                                     GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK,
                                                     "Erro ao cadastrar disco (memória)!"); // Cria uma caixa de diálogo de erro de memória.
            gtk_dialog_run(GTK_DIALOG(dialog)); // Exibe a caixa de diálogo de erro.
            gtk_widget_destroy(dialog); // Destrói a caixa de diálogo.
        }
    } else {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(widget)),
                                                  GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK,
                                                 "Limite de discos atingido!"); // Cria uma caixa de diálogo de aviso de limite.
        gtk_dialog_run(GTK_DIALOG(dialog)); // Exibe a caixa de diálogo de aviso.
        gtk_widget_destroy(dialog); // Destrói a caixa de diálogo.
    }
    indice_disco_sendo_editado = -1; // Reseta a variável que controla o disco em edição.
    gtk_widget_set_sensitive(button_salvar_alteracoes, FALSE); // Desabilita o botão "Salvar Alterações".
}

// Callback para o botão "Carregar para Edição"
static void on_carregar_edicao_button_clicked(GtkWidget *widget, gpointer data) {
    GtkTreeSelection *selection;
    GtkTreeModel *model;
    GtkTreeIter iter;

    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(list_discos_view));
    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        char *nome_disco_selecionado;
        char *autor_disco_selecionado;
        char *preco_disco_selecionado; // String do modelo
        char *estoque_disco_selecionado; // String do modelo

        gtk_tree_model_get(model, &iter,
// "le" o conteudo das colunas da linha apontada pelo 'iter' no 'model'
                           0, &nome_disco_selecionado, // Obtem o valor da coluna 0 (Nome) e armazena em 'nome_disco_selecionado'
                           1, &autor_disco_selecionado, // Obtem o valor da coluna 1 (Autor) e armazena em 'autor_disco_selecionado'
                           2, &preco_disco_selecionado, // Obtem o valor da coluna 2 (Preco) e armazena em 'preco_disco_selecionado'
                           3, &estoque_disco_selecionado, // Obtem o valor da coluna 3 (Estoque) e armazena em 'estoque_disco_selecionado'
                           -1); // Marcador para indicar o fim dos argumentos da funcao

        // Encontrar o índice real no array `discos`
        // (Importante se a lista na GUI puder ser reordenada ou filtrada no futuro)
        int selected_row_path_indices = gtk_tree_path_get_indices(gtk_tree_model_get_path(model, &iter))[0];
        
        indice_disco_sendo_editado = -1; // Reset antes de procurar
        for(int i=0; i < numDiscos; ++i) {
            if(discos[i] && strcmp(discos[i]->nome, nome_disco_selecionado) == 0) {
                 indice_disco_sendo_editado = i;
                 break;
            }
        }


if (indice_disco_sendo_editado != -1 && discos[indice_disco_sendo_editado] != NULL) {
    // Verifica se há um disco válido selecionado para edição.
    gtk_entry_set_text(GTK_ENTRY(entry_nome), discos[indice_disco_sendo_editado]->nome);
    // Preenche o campo 'Nome' com o nome do disco.
    gtk_entry_set_text(GTK_ENTRY(entry_autor), discos[indice_disco_sendo_editado]->autor);
    // Preenche o campo 'Autor' com o autor do disco.

    char preco_str_format[20];
    sprintf(preco_str_format, "%.2f", discos[indice_disco_sendo_editado]->preco);
    gtk_entry_set_text(GTK_ENTRY(entry_preco), preco_str_format);
    // Converte o preço (float) para string formatada e preenche o campo 'Preço'.

    char estoque_str_format[10];
    sprintf(estoque_str_format, "%d", discos[indice_disco_sendo_editado]->estoque);
    gtk_entry_set_text(GTK_ENTRY(entry_estoque), estoque_str_format);
    // Converte o estoque (int) para string e preenche o campo 'Estoque'.

            gtk_widget_set_sensitive(button_salvar_alteracoes, TRUE); // Habilita o botão "Salvar"
        } else {
            // Isso não deveria acontecer se a seleção for válida e o array estiver sincronizado
            GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(widget)),
                                                     GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK,
                                                     "Erro ao encontrar disco selecionado para edição.");
            gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);
            indice_disco_sendo_editado = -1;
            gtk_widget_set_sensitive(button_salvar_alteracoes, FALSE);
        }

        g_free(nome_disco_selecionado);
        g_free(autor_disco_selecionado);
        g_free(preco_disco_selecionado);
        g_free(estoque_disco_selecionado);

    } else {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(widget)),
                                                 GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK,
                                                 "Nenhum disco selecionado para carregar.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        indice_disco_sendo_editado = -1;
        gtk_widget_set_sensitive(button_salvar_alteracoes, FALSE);
    }
}

// Callback para o botão "Salvar Alterações"
static void on_salvar_alteracoes_button_clicked(GtkWidget *widget, gpointer data) {
    if (indice_disco_sendo_editado == -1 || discos[indice_disco_sendo_editado] == NULL) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(widget)),
                                                 GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK,
                                                 "Nenhum disco carregado para edição.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    const char *nome = gtk_entry_get_text(GTK_ENTRY(entry_nome));
    const char *autor = gtk_entry_get_text(GTK_ENTRY(entry_autor));
    const char *preco_str = gtk_entry_get_text(GTK_ENTRY(entry_preco));
    const char *estoque_str = gtk_entry_get_text(GTK_ENTRY(entry_estoque));

    if (strlen(nome) == 0 || strlen(autor) == 0 || strlen(preco_str) == 0 || strlen(estoque_str) == 0) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(widget)),
                                                 GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK,
                                                 "Todos os campos são obrigatórios!");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }
    // Verifica se o novo nome (se alterado) já existe em OUTRO disco
    for (int i = 0; i < numDiscos; i++) {
        if (i != indice_disco_sendo_editado && discos[i] && strcmp(discos[i]->nome, nome) == 0) {
            GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(widget)),
                                                     GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK,
                                                     "Já existe outro disco com este novo nome!");
            gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);
            return;
        }
    }


    float preco = atof(preco_str);
    int estoque = atoi(estoque_str);

     if (preco <= 0 || estoque < 0) {
         GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(widget)),
                                                 GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK,
                                                 "Preço deve ser maior que zero e estoque não pode ser negativo.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    if (atualizarDiscoGUI(discos[indice_disco_sendo_editado], nome, autor, preco, estoque)) {
        atualizar_lista_gui();
        limpar_campos_entrada();
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(widget)),
                                                 GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK,
                                                 "Disco atualizado com sucesso!");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    } else {
        // Teoricamente, não deveria acontecer se indice_disco_sendo_editado é válido
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(widget)),
                                                 GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK,
                                                 "Erro ao atualizar disco.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
    indice_disco_sendo_editado = -1; // Reseta o modo de edição
    gtk_widget_set_sensitive(button_salvar_alteracoes, FALSE); // Desabilita "Salvar"
}

// Callback para o botão "Deletar"
static void on_deletar_button_clicked(GtkWidget *widget, gpointer data) {
    GtkTreeSelection *selection;
    GtkTreeModel *model;
    GtkTreeIter iter;

    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(list_discos_view));
    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        char *nome_disco_selecionado;
        gtk_tree_model_get(model, &iter, 0, &nome_disco_selecionado, -1);

        int encontrado_idx = -1;
        for (int i = 0; i < numDiscos; i++) {
            if (discos[i] && strcmp(discos[i]->nome, nome_disco_selecionado) == 0) {
                encontrado_idx = i;
                break;
            }
        }
        g_free(nome_disco_selecionado);

        if (encontrado_idx != -1) {
            // Se o disco deletado era o que estava carregado para edição, limpa o estado
            if(encontrado_idx == indice_disco_sendo_editado) {
                indice_disco_sendo_editado = -1;
                limpar_campos_entrada();
                gtk_widget_set_sensitive(button_salvar_alteracoes, FALSE);
            }

            free(discos[encontrado_idx]); // Libera a memória do disco
            // remove o disco de um array estático e compacta o array pra preencher o espaço deixado ao ser removido
            for (int j = encontrado_idx; j < numDiscos - 1; j++) {
                discos[j] = discos[j + 1];
            }
            discos[numDiscos - 1] = NULL; // define o último ponteiro do array como NULL para limpar a referência
            numDiscos--;
            atualizar_lista_gui(); // atualiza a visualização da lista no GUI

            GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(widget)),
                                                     GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK,
                                                     "Disco deletado com sucesso!");
            gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);
        } else { // executado se o disco selecionado não foi encontrado nos dados internos
             GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(widget)),
                                                     GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK,
                                                     "Disco selecionado não encontrado nos dados internos.");
            gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);
        }
    } else { // executado se nenhuma linha foi selecionada na GtkTreeView
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(widget)),
                                                 GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK,
                                                 "Nenhum disco selecionado para deletar.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}

// Função de ativação da aplicação GTK
static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *button_cadastrar, *button_deletar, *button_carregar_edicao;
    GtkWidget *label_nome, *label_autor, *label_preco, *label_estoque;
    GtkWidget *scrolled_window;
    GtkWidget *hbox_botoes_crud; // Box para botões de CRUD (Cadastrar, Salvar)
    GtkWidget *hbox_botoes_lista; // Box para botões da lista (Carregar, Deletar)


    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Loja de Discos");
    gtk_window_set_default_size(GTK_WINDOW(window), 700, 500); // Aumentei um pouco
    gtk_container_set_border_width(GTK_CONTAINER(window), 15);

    grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Campos de Entrada 
    label_nome = gtk_label_new("Nome:");
    gtk_widget_set_halign(label_nome, GTK_ALIGN_START);
    entry_nome = gtk_entry_new();
    gtk_widget_set_hexpand(entry_nome, TRUE);

    label_autor = gtk_label_new("Autor:");
    gtk_widget_set_halign(label_autor, GTK_ALIGN_START);
    entry_autor = gtk_entry_new();
    gtk_widget_set_hexpand(entry_autor, TRUE);

    label_preco = gtk_label_new("Preço:");
    gtk_widget_set_halign(label_preco, GTK_ALIGN_START);
    entry_preco = gtk_entry_new(); 
    gtk_widget_set_hexpand(entry_preco, TRUE);

    label_estoque = gtk_label_new("Estoque:");
    gtk_widget_set_halign(label_estoque, GTK_ALIGN_START);
    entry_estoque = gtk_entry_new(); 
    gtk_widget_set_hexpand(entry_estoque, TRUE);

    gtk_grid_attach(GTK_GRID(grid), label_nome, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_nome, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label_autor, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_autor, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label_preco, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_preco, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label_estoque, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_estoque, 1, 3, 1, 1);

    // botões de Ação CRUD (Cadastrar, Salvar)
    hbox_botoes_crud = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_widget_set_halign(hbox_botoes_crud, GTK_ALIGN_CENTER);

    button_cadastrar = gtk_button_new_with_label("Cadastrar Novo");
    g_signal_connect(button_cadastrar, "clicked", G_CALLBACK(on_cadastrar_button_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(hbox_botoes_crud), button_cadastrar, TRUE, TRUE, 0);

    button_salvar_alteracoes = gtk_button_new_with_label("Salvar Alterações");
    g_signal_connect(button_salvar_alteracoes, "clicked", G_CALLBACK(on_salvar_alteracoes_button_clicked), NULL);
    gtk_widget_set_sensitive(button_salvar_alteracoes, FALSE); // Começa desabilitado
    gtk_box_pack_start(GTK_BOX(hbox_botoes_crud), button_salvar_alteracoes, TRUE, TRUE, 0);
    
    gtk_grid_attach(GTK_GRID(grid), hbox_botoes_crud, 0, 4, 2, 1);


    // lista de Discos usando o GTKTreeView
    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_vexpand(scrolled_window, TRUE);
    gtk_widget_set_hexpand(scrolled_window, TRUE);

    list_store = gtk_list_store_new(4, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
    list_discos_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(list_store));
    g_object_unref(list_store); // TreeView agora tem a referência

    const char *column_titles[] = {"Nome", "Autor", "Preço (R$)", "Estoque"};
    for (int i = 0; i < 4; i++) {
        GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
        GtkTreeViewColumn *column = gtk_tree_view_column_new_with_attributes(column_titles[i],
                                                                           renderer, "text", i, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(list_discos_view), column);
    }
    gtk_tree_view_set_grid_lines(GTK_TREE_VIEW(list_discos_view), GTK_TREE_VIEW_GRID_LINES_BOTH); // Linhas de grade
    gtk_container_add(GTK_CONTAINER(scrolled_window), list_discos_view);
    gtk_grid_attach(GTK_GRID(grid), scrolled_window, 0, 5, 2, 1);

    // botões de Ação da Lista 
    hbox_botoes_lista = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_widget_set_halign(hbox_botoes_lista, GTK_ALIGN_CENTER);

    // botão Carregar para Edição com Ícone
    button_carregar_edicao = gtk_button_new_from_icon_name("document-edit-symbolic", GTK_ICON_SIZE_BUTTON);
    gtk_button_set_label(GTK_BUTTON(button_carregar_edicao), "Alterar Selecionado");
    // gtk_button_set_always_show_image (GTK_BUTTON(button_carregar_edicao), TRUE); // Para mostrar imagem e texto
    g_signal_connect(button_carregar_edicao, "clicked", G_CALLBACK(on_carregar_edicao_button_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(hbox_botoes_lista), button_carregar_edicao, TRUE, TRUE, 0);

    button_deletar = gtk_button_new_from_icon_name("edit-delete-symbolic", GTK_ICON_SIZE_BUTTON);
    gtk_button_set_label(GTK_BUTTON(button_deletar), "Deletar Selecionado");
    g_signal_connect(button_deletar, "clicked", G_CALLBACK(on_deletar_button_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(hbox_botoes_lista), button_deletar, TRUE, TRUE, 0);

    gtk_grid_attach(GTK_GRID(grid), hbox_botoes_lista, 0, 6, 2, 1);

    atualizar_lista_gui(); // Carrega dados iniciais, se houver
    gtk_widget_show_all(window);
}

// Função main
int main(int argc, char **argv) {

    GtkApplication *app;
    int status;

    app = gtk_application_new("com.example.lojadiscos", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    // Liberar memória dos discos ao sair
    for (int i = 0; i < numDiscos; i++) {
        if(discos[i]) free(discos[i]);
    }
    return status;
}
