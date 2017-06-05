/*
 *  Sistemas de Informacao
 *  UFG
 *
 *  Aluno:
 *    - Guilherme Ferreira Schults
 */

// Includes ---------------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Constantes -------------------------------------------------------------------------------------

static const int BUSCA_NOME        = 0;
static const int BUSCA_CODIGO      = 1;
static const int BUSCA_NOME_CODIGO = 2;

// Structs ----------------------------------------------------------------------------------------

/*
 *  Endereco
 */
typedef struct
{
	char *logradouro;
	char *complemento;
	char *cep;
	char *bairro;
	char *cidade;
	char *estado;
	
}Endereco;

/*
 *  Fornecedor
 */
typedef struct
{
	char *codigo;
	char *nome;
	char *cnpj;
	char *telefone;
	Endereco endereco;

}Fornecedor;

/*
 *  Produto
 */
typedef struct
{
	char  *codigo;
	char  *nome;
	char  *fornecedor;
	char  *categoria;
	int    quantidade;
	float  valor_unitario;
	
}Produto;

/*
 *  Janela
 */
typedef struct
{
	int    linhas_count;
	int    separadores_count;
	int   *separadores;
	char  *rotulo;
	char **linhas;

}Janela;

// Defincoes de funcoes --------------------------------------------------------------------------

// Opcoes de menu

void informacoes_gerenciais(Fornecedor*, int, Produto*, int);

void cadastrar_fornecedor(Fornecedor*, int);
void listar_fornecedores (Fornecedor*, int);
void buscar_fornecedor   (Fornecedor*, int);
void atualizar_fornecedor(Fornecedor*, int);

void cadastrar_produto(Produto*, int);
void listar_produtos  (Produto*, int);
void buscar_produto   (Produto*, int);
void atualizar_produto(Produto*, int);
void atualizar_estoque(Produto*, int);
void excluir_produto  (Produto*, int);

// Funcoes auxiliares - Fornecedor

Fornecedor obter_cadastro_fornecedor();
char*      fornecedor_to_string     (Fornecedor);
int        procurar_fornecedor      (int, char*, Fornecedor*, int);

// Funcoes auxiliares - Produto

Produto obter_cadastro_produto();
char*   produto_to_string     (Produto);
int     procurar_produto      (int, char*, Produto*, int);

// Acesso aos dados

int obter_fornecedores  (Fornecedor**);
void salvar_fornecedores(Fornecedor*, int);

int obter_produtos  (Produto**);
void salvar_produtos(Produto*, int);

// Exibicao de menu

char obter_menu_principal();
char obter_menu_fornecedores();
char obter_menu_produtos();

// Entrada de dados

char* get_string();
char  get_char();

// Manipulacao dos dados de exibicao

Janela obter_janela();
void  adicionar_rotulo(Janela*, char*);
void  adicionar_linha(Janela*, char*);
void  adicionar_separador(Janela*);
void  exibir_janela(Janela);
void  exibir_mensagem(char*, char*);

// Funcoes auxiliares -----------------------------------------------------------------------------

void print_many (char c, int count);
void string_copy(char**, char*);

void ordenar_fornecedores(Fornecedor *vetor, int);
void ordenar_produtos(Produto *vetor, int);
int get_lines(char *path, unsigned char ***tmp_lines);

// Funcoes ----------------------------------------------------------------------------------------

/*
 *  main
 */
int main()
{
	char opcao;
	
	while(1)
	{
		Fornecedor *fornecedores;
		Produto    *produtos;

		int tamanho_f = obter_fornecedores(&fornecedores),
		    tamanho_p = obter_produtos(&produtos);

		opcao = obter_menu_principal();
	
		// Opcao Sair Escolhida

		if(opcao == 'S' && opcao == 's')
		{
			exit(0);
		}

		// Opcao Informacoes Escolhida

		else if(opcao == 'I' || opcao == 'i')
		{
			informacoes_gerenciais(fornecedores, tamanho_f, produtos, tamanho_p);
		}

		// Opcao Fornecedor Escolhida

		else if(opcao == 'F' || opcao == 'f')
		{
			opcao = obter_menu_fornecedores();
			
			if(opcao == 'C' || opcao == 'c')
			{
				cadastrar_fornecedor(fornecedores, tamanho_f);
			}
			else if(opcao == 'L' || opcao == 'l')
			{
				listar_fornecedores(fornecedores, tamanho_f);
			}
			else if(opcao == 'B' || opcao == 'b')
			{
				buscar_fornecedor(fornecedores, tamanho_f);
			}
			else if(opcao == 'A' || opcao == 'a')
			{
				atualizar_fornecedor(fornecedores, tamanho_f);
			}
		}
		
		// Opcao Produto Escolhida

		else if(opcao == 'P' || opcao == 'p')
		{
			opcao = obter_menu_produtos();

			if(opcao == 'C' || opcao == 'c')
			{
				cadastrar_produto(produtos, tamanho_p);
			}
			else if(opcao == 'L' || opcao == 'l')
			{
				listar_produtos(produtos, tamanho_p);
			}
			else if(opcao == 'B' || opcao == 'b')
			{
				buscar_produto(produtos, tamanho_p);
			}
			else if(opcao == 'A' || opcao == 'a')
			{
				atualizar_produto(produtos, tamanho_p);
			}
			else if(opcao == 'S' || opcao == 's')
			{
				atualizar_estoque(produtos, tamanho_p);
			}
			else if(opcao == 'E' || opcao == 'e')
			{
				excluir_produto(produtos, tamanho_p);
			}

		}
	}
	return 0;
}

// Opcoes de menu ---------------------------------------------------------------------------------

/*
 *
 */
void informacoes_gerenciais(Fornecedor *fornecedores, int tamanho_f, Produto *produtos, int tamanho_p)
{
	int maior_categoria = 0,
	    maior_fornecedor = 0,
	    categoria_count = 0,
	    i, j,
	    texto_index = 0;

	char **categorias = (char**) malloc(sizeof(char*) * tamanho_p);

	char *texto = (char*) malloc(sizeof(char) * 100000);

	for(i = 0; i < tamanho_p; i++)
	{
		char *categoria = produtos[i].categoria;

		int tamanho_categoria = strlen(categoria),
		    achou_categoria = 0;

		for(j = 0; j < categoria_count; j++)
			if(strcmp(categoria, categorias[j]) == 0)
				achou_categoria = 1;

		if(achou_categoria == 0)
		{
			categorias[categoria_count] = (char*) malloc(sizeof(char) * (tamanho_categoria + 1));

			for(j = 0; j < tamanho_categoria; j++)
				categorias[categoria_count][j] = categoria[j];

			categorias[categoria_count++][j] = '\0';
		}

		if(tamanho_categoria > maior_categoria)
			maior_categoria = tamanho_categoria;
	}

	for(i = 0; i < tamanho_f; i++)
		if(strlen(fornecedores[i].nome) > maior_fornecedor)
			maior_fornecedor = strlen(fornecedores[i].nome);

	for(i = 0; i < categoria_count; i++)
	{
		char *categoria = categorias[i];
		int tamanho_c = strlen(categoria),
		    itens = 0,
		    quantidade = 0;

		float valor = 0.0;

		texto_index += sprintf(texto + texto_index, "%s:\n\n", categoria);

		for(j = 0; j < tamanho_p; j++)
			if(strcmp(categoria, produtos[j].categoria) == 0){
				itens++;
				quantidade += produtos[j].quantidade;
				valor += (produtos[j].quantidade * produtos[j].valor_unitario);
			}

		texto_index += sprintf(texto + texto_index, "  Itens:    %d\n", itens);
		texto_index += sprintf(texto + texto_index, "  Produtos: %d\n", quantidade);
		texto_index += sprintf(texto + texto_index, "  Valor:    %.2f\n", valor);

		if(i != categoria_count - 1)
			texto_index += sprintf(texto + texto_index, "\n\n");

	}

	Janela janela = obter_janela();
	adicionar_rotulo(&janela, "Informacoes Gerenciais Produtos");
	adicionar_linha(&janela, texto);
	exibir_janela(janela);

	texto_index = 0;

	for(i = 0; i < tamanho_f; i++)
	{
		int itens = 0,
		    quantidade = 0;

		float valor = 0.0;

		texto_index += sprintf(texto + texto_index, "%s:\n\n", fornecedores[i].nome);

		for(j = 0; j < tamanho_p; j++)
		{
			if(strcmp(fornecedores[i].codigo, produtos[j].fornecedor) == 0)
			{
				itens++;
				quantidade += produtos[j].quantidade;
				valor += (produtos[j].quantidade * produtos[j].valor_unitario);
			}
		}

		texto_index += sprintf(texto + texto_index, "  Itens:       %d\n", itens);
		texto_index += sprintf(texto + texto_index, "  Quantidade:  %d\n", quantidade);
		texto_index += sprintf(texto + texto_index, "  Valor:       %.2f\n", valor);

		if(i != tamanho_f - 1)
			texto_index += sprintf(texto + texto_index, "\n\n");
	}

	janela = obter_janela();
	adicionar_rotulo(&janela, "Informacoes Gerenciais Fornecedores");
	adicionar_linha(&janela, texto);
	exibir_janela(janela);

	janela = obter_janela();
	adicionar_rotulo(&janela, "Produtos com Pouco Estoque");

	for(i = 0; i < tamanho_p; i++)
	{
		if(produtos[i].quantidade < 5)
		{
			texto_index = 0;
			texto_index += sprintf(texto + texto_index, "Produto:    %s\n", produtos[i].nome);

			char *fornecedor;

			for(j = 0; j < tamanho_f; j++)
				if(strcmp(fornecedores[j].codigo, produtos[i].fornecedor) == 0)
					string_copy(&fornecedor, fornecedores[j].nome);

			texto_index += sprintf(texto + texto_index, "Fornecedor: %s\n", fornecedor);
			texto_index += sprintf(texto + texto_index, "Quantidade: %d\n", produtos[i].quantidade);

			adicionar_linha(&janela, texto);
			adicionar_separador(&janela);
		}
	}

	exibir_janela(janela);


}

/*
 *
 */
void cadastrar_fornecedor(Fornecedor *fornecedores, int tamanho)
{
	fornecedores = (Fornecedor*) realloc(fornecedores, (++tamanho) * sizeof(Fornecedor));
	fornecedores[tamanho-1] = obter_cadastro_fornecedor();
	salvar_fornecedores(fornecedores, tamanho);
}

/*
 *
 */
void listar_fornecedores(Fornecedor *fornecedores, int tamanho)
{
	printf("\n Deseja filtrar por estado? (S/N)? ");
	char op = get_char();
	char *filtro;

	Fornecedor *selecionados = (Fornecedor*) malloc(sizeof(Fornecedor) * tamanho);
	
	int tamanho_selecionados = 0,
	    i;

	while(op != 'S' && op != 's' && op != 'n' && op != 'N')
	{
		printf(" Opcao invalida. Digite S ou N: ");
		op = get_char();
	}

	if(op == 'S' || op == 's')
	{
		printf(" Filtro por estado: ");
		filtro = get_string();

		for(i = 0; i < tamanho; i++)
			if(strcmp(filtro, fornecedores[i].endereco.estado) == 0)
				selecionados[tamanho_selecionados++] = fornecedores[i];
	}
	else
	{
		selecionados = fornecedores;
		tamanho_selecionados = tamanho;
	}

	if(tamanho_selecionados == 0)
	{
		exibir_mensagem("Informacao", "Nenhum fornecedor foi encontrado!");
		return;
	}

	Janela janela = obter_janela();
	adicionar_rotulo(&janela, "Lista de fornecedores");

	for(i = 0; i < tamanho_selecionados; i++)
	{
		adicionar_linha(&janela, fornecedor_to_string(selecionados[i]));

		if(i < tamanho_selecionados - 1)
			adicionar_separador(&janela);
	}

	exibir_janela(janela);
}

/*
 *
 */
void buscar_fornecedor(Fornecedor *fornecedores, int tamanho)
{
	printf("\n Digite o nome ou codigo do fornecedor: ");
	char *busca = get_string();

	int posicao = procurar_fornecedor(BUSCA_NOME_CODIGO, busca, fornecedores, tamanho);

	if(posicao < 0)
	{
		exibir_mensagem("Informacao", "Nenhum fornecedor foi encontrado!");
	}
	else
	{
		Janela janela = obter_janela();
		adicionar_rotulo(&janela, "Fornecedor encontrado!");
		adicionar_linha(&janela, fornecedor_to_string(fornecedores[posicao]));
		exibir_janela(janela);
	}
}

/*
 *
 */
void atualizar_fornecedor(Fornecedor *fornecedores, int tamanho)
{
	printf("\n Digite o codigo do fornecedor: ");

	char *codigo = get_string();

	int posicao = procurar_fornecedor(BUSCA_CODIGO, codigo, fornecedores, tamanho);

	if(posicao < 0)
	{
		exibir_mensagem("Erro", "Nenhum fornecedor foi encontrado");
	}
	else
	{
		Fornecedor new_f = obter_cadastro_fornecedor(),
		           old_f = fornecedores[posicao];

		Endereco new_e = new_f.endereco,
		         old_e = old_f.endereco;

		if(strlen(new_f.nome) > 0)     strcpy(old_f.nome,     new_f.nome);
		if(strlen(new_f.cnpj) > 0)     strcpy(old_f.cnpj,     new_f.cnpj);
		if(strlen(new_f.telefone) > 0) strcpy(old_f.telefone, new_f.telefone);
		if(strlen(new_f.codigo) > 0)   strcpy(old_f.codigo,   new_f.codigo);
		
		if(strlen(new_e.logradouro) > 0)  strcpy(old_e.logradouro,  new_e.logradouro);
		if(strlen(new_e.complemento) > 0) strcpy(old_e.complemento, new_e.complemento);
		if(strlen(new_e.cep) > 0)         strcpy(old_e.cep,         new_e.cep);
		if(strlen(new_e.bairro) > 0)      strcpy(old_e.bairro,      new_e.bairro);
		if(strlen(new_e.cidade) > 0)      strcpy(old_e.cidade,      new_e.cidade);
		if(strlen(new_e.estado) > 0)      strcpy(old_e.estado,      new_e.estado);
	}

	salvar_fornecedores(fornecedores, tamanho);
}

/*
 *
 */
void cadastrar_produto(Produto *produtos, int tamanho)
{
	produtos = (Produto*) realloc(produtos, (++tamanho) * sizeof(Produto));
	produtos[tamanho-1] = obter_cadastro_produto();
	salvar_produtos(produtos, tamanho);
}

/*
 *
 */
void listar_produtos(Produto *produtos, int tamanho)
{
	printf("\n Deseja filtrar por categoria? (S/N)? ");
	char op = get_char();
	char *filtro;

	Produto *selecionados = (Produto*) malloc(sizeof(Produto) * tamanho);
	
	int tamanho_selecionados = 0,
	    i;

	while(op != 'S' && op != 's' && op != 'n' && op != 'N')
	{
		printf(" Opcao invalida. Digite S ou N: ");
		op = get_char();
	}

	if(op == 'S' || op == 's')
	{
		printf(" Filtro por categoria: ");
		filtro = get_string();

		for(i = 0; i < tamanho; i++)
			if(strcmp(filtro, produtos[i].categoria) == 0)
				selecionados[tamanho_selecionados++] = produtos[i];
	}
	else
	{
		selecionados = produtos;
		tamanho_selecionados = tamanho;
	}

	if(tamanho_selecionados == 0)
	{
		exibir_mensagem("Informacao", "Nenhum produto foi encontrado!");
		return;
	}

	Janela janela = obter_janela();
	adicionar_rotulo(&janela, "Lista de produtos");

	for(i = 0; i < tamanho_selecionados; i++)
	{
		adicionar_linha(&janela, produto_to_string(selecionados[i]));

		if(i < tamanho_selecionados - 1)
			adicionar_separador(&janela);
	}

	exibir_janela(janela);
}

/*
 *
 */
void buscar_produto(Produto *produtos, int tamanho)
{
	printf("\n Digite o nome ou codigo do produto: ");
	char *busca = get_string();

	int posicao = procurar_produto(BUSCA_NOME_CODIGO, busca, produtos, tamanho);

	if(posicao < 0)
	{
		exibir_mensagem("Informacao", "Nenhum produto foi encontrado!");
	}
	else
	{
		Janela janela = obter_janela();
		adicionar_rotulo(&janela, "Produto encontrado!");
		adicionar_linha(&janela, produto_to_string(produtos[posicao]));
		exibir_janela(janela);
	}
}

/*
 *
 */
void atualizar_produto(Produto *produtos, int tamanho)
{
	printf("\n Digite o codigo do produto: ");

	char *codigo = get_string();

	int posicao = procurar_produto(BUSCA_CODIGO, codigo, produtos, tamanho);

	if(posicao < 0)
	{
		exibir_mensagem("Erro", "Nenhum produto foi encontrado");
	}
	else
	{
		Produto new = obter_cadastro_produto(),
		        old = produtos[posicao];

		if(strlen(new.codigo) > 0)     strcpy(old.codigo,         new.codigo);
		if(strlen(new.nome) > 0)       strcpy(old.nome,           new.nome);
		if(strlen(new.fornecedor) > 0) strcpy(old.fornecedor,     new.fornecedor);
		if(strlen(new.categoria) > 0)  strcpy(old.categoria,      new.categoria);

		if(new.valor_unitario > 0) produtos[posicao].valor_unitario = new.valor_unitario;
		if(new.quantidade > 0)     produtos[posicao].quantidade     = new.quantidade;
	}

	salvar_produtos(produtos, tamanho);
}

/*
 *
 */
void atualizar_estoque(Produto *produtos, int tamanho)
{
	printf("\n Digite o codigo do produto: ");

	char *codigo = get_string();

	int posicao = procurar_produto(BUSCA_CODIGO, codigo, produtos, tamanho);

	if(posicao < 0)
	{
		exibir_mensagem("Erro", "Nenhum produto foi encontrado");
	}
	else
	{
		printf("\n Digite o quantidade do produto: ");

		char *quantidade = get_string();

		if(strlen(quantidade) > 0)
			produtos[posicao].quantidade = atoi(quantidade);
	}

	salvar_produtos(produtos, tamanho);
}

/*
 *
 */
void excluir_produto(Produto *produtos, int tamanho)
{
	printf("\n Digite o codigo do produto: ");

	char *codigo = get_string();

	int posicao = procurar_produto(BUSCA_CODIGO, codigo, produtos, tamanho);

	if(posicao < 0)
	{
		exibir_mensagem("Erro", "Nenhum produto foi encontrado");
	}
	else
	{
		Produto *selecionados = (Produto*) malloc(sizeof(Produto) * (tamanho-1));
		int selecionados_count = 0;

		int i;
		for(i = 0; i < tamanho; i++)
			if(i != posicao)
				selecionados[selecionados_count++] = produtos[i];

		salvar_produtos(selecionados, selecionados_count);
	}
}

// Funcoes auxiliares - Fornecedor ----------------------------------------------------------------

/*
 *
 */
Fornecedor obter_cadastro_fornecedor()
{
	Fornecedor f;

	printf("\n Nome do Fornecedor: "); f.nome = get_string();
	printf(" CNPJ: ");                 f.cnpj = get_string();
	printf(" Telefone: ");             f.telefone = get_string();
	printf(" Codigo: ");               f.codigo = get_string();
	
	printf("\n Endereco\n\n");

	printf(" Logradouro: ");      f.endereco.logradouro = get_string();
	printf(" Complemento: ");     f.endereco.complemento = get_string();
	printf(" CEP: ");             f.endereco.cep = get_string();
	printf(" Bairro: ");          f.endereco.bairro = get_string();
	printf(" Cidade: ");          f.endereco.cidade = get_string();
	printf(" Sigla do Estado: "); f.endereco.estado = get_string();

	printf("\n");
	return f;
}

/*
 *
 */
char* fornecedor_to_string(Fornecedor f)
{
	int MAX = 1024 * 1024;

	Endereco e = f.endereco;

	char *string = (char*) malloc(sizeof(char) * MAX);
	
	int index = 0;
	index += snprintf(string + index, MAX, "Codigo:   %s\n", f.codigo);
	index += snprintf(string + index, MAX, "Nome:     %s\n", f.nome);
	index += snprintf(string + index, MAX, "CNPJ:     %s\n", f.cnpj);
	index += snprintf(string + index, MAX, "Telefone: %s\n", f.telefone);
	index += snprintf(string + index, MAX, "Endereco:\n");
	index += snprintf(string + index, MAX, "  %s\n", e.logradouro);
	index += snprintf(string + index, MAX, "  %s\n", e.complemento);
	index += snprintf(string + index, MAX, "  %s - %s\n", e.bairro, e.cep);
	index += snprintf(string + index, MAX, "  %s - %s", e.cidade, e.estado);

	return string;
}

/*
 *
 */
int procurar_fornecedor(int tipo_busca, char *busca, Fornecedor *fornecedores, int tamanho)
{
	int i;
	for(i = 0; i < tamanho; i++)
	{
		Fornecedor f = fornecedores[i];

		if(tipo_busca == BUSCA_NOME && strcmp(busca, f.nome) == 0)
				return i;

		if(tipo_busca == BUSCA_CODIGO && strcmp(busca, f.codigo) == 0)
				return i;

		if(tipo_busca == BUSCA_NOME_CODIGO &&
			strcmp(busca, f.nome) == 0 || strcmp(busca, f.codigo) == 0)
				return i;
	}

	return -1;
}

// Funcoes auxiliares - Produto -------------------------------------------------------------------

/*
 *
 */
Produto obter_cadastro_produto()
{
	Produto p;

	char *valor,
	     *quantidade;

	printf("\n");
	printf(" Codigo: ");     p.codigo     = get_string();
	printf(" Nome: ");       p.nome       = get_string();
	printf(" Fornecedor: "); p.fornecedor = get_string();
	printf(" Categoria: ");  p.categoria  = get_string();
	printf(" Valor: ");      valor        = get_string();
	printf(" Quantidade: "); quantidade   = get_string();
	printf("\n");

	if(strlen(valor) == 0)
		p.valor_unitario = 0.0;
	else
		p.valor_unitario = atof(valor);
	
	if(strlen(quantidade) == 0)
		p.quantidade = 0;
	else
		p.quantidade = atoi(quantidade);

	return p;
}

/*
 *
 */
char* produto_to_string(Produto p)
{
	int MAX = 1024 * 1024;

	char *string = (char*) malloc(sizeof(char) * MAX);
	
	int index = 0;
	index += snprintf(string + index, MAX, "Codigo:     %s\n",   p.codigo);
	index += snprintf(string + index, MAX, "Nome:       %s\n",   p.nome);
	index += snprintf(string + index, MAX, "Fornecedor: %s\n",   p.fornecedor);
	index += snprintf(string + index, MAX, "Categoria:  %s\n",   p.categoria);
	index += snprintf(string + index, MAX, "Valor:      %.2f\n", p.valor_unitario);
	index += snprintf(string + index, MAX, "Quantidade: %d",     p.quantidade);

	return string;
}

/*
 *
 */
int procurar_produto(int tipo_busca, char *busca, Produto *produtos, int tamanho)
{
	int i;
	for(i = 0; i < tamanho; i++)
	{
		Produto p = produtos[i];

		if(tipo_busca == BUSCA_NOME && strcmp(busca, p.nome) == 0)
				return i;

		if(tipo_busca == BUSCA_CODIGO && strcmp(busca, p.codigo) == 0)
				return i;

		if(tipo_busca == BUSCA_NOME_CODIGO &&
			strcmp(busca, p.nome) == 0 || strcmp(busca, p.codigo) == 0)
				return i;
	}

	return -1;
}

// Acesso aos dados -------------------------------------------------------------------------------

/*
 * 
 */
int obter_fornecedores(Fornecedor **fornecedores)
{
	unsigned char **linhas;

	int line_count = get_lines("fornecedores.txt", &linhas),
	    fornecedor_count = line_count/10;

	Fornecedor *tmp = (Fornecedor*) malloc(sizeof(Fornecedor) * (fornecedor_count));

	int i;
	for(i = 0; i < (fornecedor_count); i++)
	{
		string_copy(&tmp[i].nome,                 linhas[(i*10)]);
		string_copy(&tmp[i].cnpj,                 linhas[(i*10)+1]);
		string_copy(&tmp[i].telefone,             linhas[(i*10)+2]);
		string_copy(&tmp[i].codigo,               linhas[(i*10)+3]);
		string_copy(&tmp[i].endereco.logradouro,  linhas[(i*10)+4]);
		string_copy(&tmp[i].endereco.complemento, linhas[(i*10)+5]);
		string_copy(&tmp[i].endereco.cep,         linhas[(i*10)+6]);
		string_copy(&tmp[i].endereco.bairro,      linhas[(i*10)+7]);
		string_copy(&tmp[i].endereco.cidade,      linhas[(i*10)+8]);
		string_copy(&tmp[i].endereco.estado,      linhas[(i*10)+9]);
	}

	ordenar_fornecedores(tmp, fornecedor_count);
	(*fornecedores) = tmp;
	return fornecedor_count;
}

/*
 *
 */
void salvar_fornecedores(Fornecedor *fornecedores, int tamanho)
{
	FILE *arquivo;
	
	if((arquivo = fopen("fornecedores.txt", "w")) == NULL)
	{
		exibir_mensagem("Erro", "Falha ao abrir o arquivo fornecedores.txt");
		exit(-1);
	}

	int i;
	for(i = 0; i < tamanho; i++)
	{
		fprintf(arquivo, "%s\n", fornecedores[i].nome);
		fprintf(arquivo, "%s\n", fornecedores[i].cnpj);
		fprintf(arquivo, "%s\n", fornecedores[i].telefone);
		fprintf(arquivo, "%s\n", fornecedores[i].codigo);
		fprintf(arquivo, "%s\n", fornecedores[i].endereco.logradouro);
		fprintf(arquivo, "%s\n", fornecedores[i].endereco.complemento);
		fprintf(arquivo, "%s\n", fornecedores[i].endereco.cep);
		fprintf(arquivo, "%s\n", fornecedores[i].endereco.bairro);
		fprintf(arquivo, "%s\n", fornecedores[i].endereco.cidade);
		fprintf(arquivo, "%s\n", fornecedores[i].endereco.estado);
	}
	
	fclose(arquivo);
}


/*
 * 
 */
int obter_produtos(Produto **produtos)
{
	unsigned char **linhas;

	int line_count = get_lines("produtos.txt", &linhas),
	    produto_count = line_count/6;

	Produto *tmp = (Produto*) malloc(sizeof(Produto) * (produto_count));

	int i;
	for(i = 0; i < (produto_count); i++)
	{
		char *valor_unitario,
		     *quantidade;

		string_copy(&tmp[i].codigo,     linhas[(i*6)]);
		string_copy(&tmp[i].nome,       linhas[(i*6)+1]);
		string_copy(&tmp[i].fornecedor, linhas[(i*6)+2]);
		string_copy(&tmp[i].categoria,  linhas[(i*6)+3]);
		string_copy(&valor_unitario,    linhas[(i*6)+4]);
		string_copy(&quantidade,        linhas[(i*6)+5]);

		tmp[i].valor_unitario = atof(valor_unitario);
		tmp[i].quantidade = atoi(quantidade);
	}

	ordenar_produtos(tmp, produto_count);
	(*produtos) = tmp;
	return produto_count;
}

/*
 *
 */
void salvar_produtos(Produto *produtos, int tamanho)
{
	FILE *arquivo;
	
	if((arquivo = fopen("produtos.txt", "w")) == NULL)
	{
		exibir_mensagem("Erro", "Falha ao abrir o arquivo produtos.txt");
		exit(-1);
	}

	int i;
	for(i = 0; i < tamanho; i++)
	{
		fprintf(arquivo, "%s\n",   produtos[i].codigo);
		fprintf(arquivo, "%s\n",   produtos[i].nome);
		fprintf(arquivo, "%s\n",   produtos[i].fornecedor);
		fprintf(arquivo, "%s\n",   produtos[i].categoria);
		fprintf(arquivo, "%.2f\n", produtos[i].valor_unitario);
		fprintf(arquivo, "%d\n",   produtos[i].quantidade);
	}
	
	fclose(arquivo);
}

// Exibicao de menu -------------------------------------------------------------------------------

/*
 *
 */
char obter_menu_principal()
{
	Janela janela = obter_janela();
	adicionar_rotulo(&janela, "Menu Principal");
	adicionar_linha(&janela, "F - Gerenciar Fornecedores");
	adicionar_linha(&janela, "P - Gerenciar Produtos");
	adicionar_linha(&janela, "I - Informacoes Gerenciais");
	adicionar_linha(&janela, "S - Sair");
	exibir_janela(janela);
	printf("\n Comando: ");
	return get_char();
}

/*
 *
 */
char obter_menu_fornecedores()
{
	Janela janela = obter_janela();
	adicionar_rotulo(&janela, "Fornecedores");
	adicionar_linha(&janela, "C - Cadastrar");
	adicionar_linha(&janela, "L - Listar");
	adicionar_linha(&janela, "B - Buscar");
	adicionar_linha(&janela, "A - Atualizar");
	exibir_janela(janela);
	printf("\n Comando: ");
	return get_char();
}

/*
 *
 */
char obter_menu_produtos()
{
	Janela janela = obter_janela();
	adicionar_rotulo(&janela, "Produtos");
	adicionar_linha(&janela, "C - Cadastrar");
	adicionar_linha(&janela, "L - Listar");
	adicionar_linha(&janela, "B - Buscar");
	adicionar_linha(&janela, "A - Atualizar Produto");
	adicionar_linha(&janela, "S - Atualizar Estoque");
	adicionar_linha(&janela, "E - Excluir");
	exibir_janela(janela);
	printf("\n Comando: ");
	return get_char();
}

// Entrada de dados -------------------------------------------------------------------------------

/*
 *
 */
char* get_string()
{
	int MAX = 1024 * 1024,
	    string_index = 0;

	char *string = (char*) malloc(sizeof(char) * MAX);

	char current_char[2];

	while(1)
	{
		fgets(current_char, 2, stdin);

		if(current_char[0] == '\n')
			break;
	
		string[string_index++] = current_char[0];
	}

	char *texto = (char*) malloc(sizeof(char) * (string_index + 1));

	int i;
	for(i = 0; i < string_index; i++)
		texto[i] = string[i];

	texto[i] = '\0';

	return texto;
}


/*
 *
 */
char get_char()
{
	return get_string()[0];
}

// Manipulacao dos dados de exibicao --------------------------------------------------------------

/*
 *
 */
Janela obter_janela()
{
	Janela janela;
	janela.rotulo = "";
	janela.linhas = (char**) malloc( (janela.linhas_count = 0) );
	janela.separadores = (int*) malloc( (janela.separadores_count = 0) );

	return janela;
}

/*
 *
 */
void adicionar_rotulo(Janela *janela, char *rotulo)
{
	size_t rotulo_size = strlen(rotulo);

	janela->rotulo = (char*) malloc(sizeof(char) * (rotulo_size + 1));

	int i;
	for(i = 0; i < rotulo_size; i++)
		janela->rotulo[i] = rotulo[i];

	janela->rotulo[rotulo_size] = '\0';
}

/*
 *
 */
void adicionar_linha(Janela *janela, char *linha)
{
	size_t linha_size = strlen(linha),
	       newline_count = 0,
	       *newline_positions = (size_t*) malloc(0),
	       i,
	       j;

	char **linhas = (char**) malloc(sizeof(char*)),
	      *linha_atual = (char*) malloc(sizeof(char) * linha_size);

	int linhas_size = 0,
	    linha_atual_index = 0;

	for(i = 0; i <= linha_size; i++)
	{
		if(linha[i] == '\n' || i == linha_size)
		{
			linhas = (char**) realloc(linhas, ++linhas_size * sizeof(char*));
			linhas[linhas_size-1] = (char*) malloc(sizeof(char) * (linha_atual_index + 1));

			for(j = 0; j < linha_atual_index; j++)
				linhas[linhas_size-1][j] = linha_atual[j];

			linhas[linhas_size-1][j] = '\0';
			linha_atual_index = 0;
		}
		else
		{
			linha_atual[linha_atual_index++] = linha[i];
		}
	}

	for(i = 0; i < linhas_size; i++)
	{

		int linhas_count = janela->linhas_count++,
		    linha_size = strlen(linhas[i]);

		janela->linhas = (char**) realloc(janela->linhas, sizeof(char*) * janela->linhas_count);
		janela->linhas[linhas_count] = (char*) malloc(sizeof(char) * (linha_size + 1));

		for(j = 0; j < linha_size; j++)
			janela->linhas[linhas_count][j] = linhas[i][j];

		janela->linhas[linhas_count][linha_size] = '\0';
	}
}

/*
 *
 */
void adicionar_separador(Janela *janela)
{
	janela->separadores = (int*) realloc(janela->separadores, ++janela->separadores_count * sizeof(int));
	janela->separadores[janela->separadores_count-1] = janela->linhas_count - 1;
}

/*
 *
 */
void exibir_janela(Janela janela)
{
	size_t rotulo_size = strlen(janela.rotulo),
	       linha_size = 0,
	       rotulo_offset;

	int i, j;

	for(i = 0; i < janela.linhas_count; i++)
		if(strlen(janela.linhas[i]) > linha_size)
			linha_size = strlen(janela.linhas[i]);

	rotulo_offset = (linha_size - rotulo_size) / 2;

	if(rotulo_size >= linha_size)
		linha_size = rotulo_size;

	if((linha_size - rotulo_size) % 2 == 0)
		rotulo_offset = (linha_size - rotulo_size) / 2;
	else
		rotulo_offset = (++linha_size - rotulo_size) / 2;

	printf("\n +");
	print_many('-', linha_size+2);
	printf("+\n");

	if(rotulo_size > 0)
	{
		printf(" | ");
		print_many(' ', rotulo_offset);
		printf("%s", janela.rotulo);
		print_many(' ', rotulo_offset);
		printf(" |\n");
		printf(" +");
		print_many('-', linha_size+2);
		printf("+\n");
	}

	for(i = 0; i < janela.linhas_count; i++)
	{
		printf(" | %s", janela.linhas[i]);
		print_many(' ', linha_size - strlen(janela.linhas[i]));
		printf(" | \n");

		for(j = 0; j < janela.separadores_count; j++)
		{
			if(i == janela.separadores[j])
			{
				printf(" +");
				print_many('-', linha_size+2);
				printf("+\n");
			}
		}
	}

	printf(" +");
	print_many('-', linha_size+2);
	printf("+\n");
}

/*
 *
 */
void exibir_mensagem(char *rotulo, char *mensagem)
{
	Janela janela = obter_janela();

	if(strlen(rotulo) > 0)
		adicionar_rotulo(&janela, rotulo);

	adicionar_linha(&janela, mensagem);
	exibir_janela(janela);
}

// Funcoes auxiliares -----------------------------------------------------------------------------

/*
 *
 */
void print_many(char c, int count)
{
	int i;
	for(i = 0; i < count; i++)
		printf("%c", c);
}

/*
 *
 */
void string_copy(char **dest, char *orig)
{
	size_t length = strlen(orig);

	char *d = (char*) malloc(sizeof(char) * (length+1));

	int i;
	for(i = 0; i < length; i++)
		if(orig[i] != '\r')
			d[i] = orig[i];

	d[i] = '\0';

	(*dest) = d;
}

// ------------------------------------------------------------------------------------------------


/*
 *
 */
int buscar_fornecedor_codigo(char codigo[], Fornecedor *fornecedor, int quantidade)
{
	int i;
	for(i = 0; i < quantidade; i++)
	{
		if(strcmp(fornecedor[i].codigo, codigo) == 0){
			return i;
		}
	}
	return -1;
}


/*
 *
 */
void ordenar_fornecedores(Fornecedor *vetor, int tamanho)
{
	Fornecedor aux;
	int i, j, menor;
	for(i = 0; i < tamanho; i++){
		menor = i;
		for(j = i + 1; j < tamanho; j++){
			if (strcmp(vetor[j].nome, vetor[menor].nome) < 0){
					menor = j;
			}
		}
		if(i != menor){
			aux = vetor[i];
			vetor[i] = vetor[menor];
			vetor[menor] = aux;
		}
	}
}

/*
 *
 */
void ordenar_produtos(Produto *vetor, int tamanho)
{
	Produto aux;
	int i, j, menor;
	for(i = 0; i < tamanho; i++){
		menor = i;
		for(j = i + 1; j < tamanho; j++){
			if (strcmp(vetor[j].nome, vetor[menor].nome) < 0){
					menor = j;
			}
		}
		if(i != menor){
			aux = vetor[i];
			vetor[i] = vetor[menor];
			vetor[menor] = aux;
		}
	}
}

/*
 *
 */
int read_file(char *path, unsigned char **buffer)
{
	FILE * pFile;
	long lSize;
	size_t result;
	unsigned char *tmp_buf;

	pFile = fopen ( path , "rb" );

	if (pFile==NULL)
	{
		*buffer = "";
		return 0;
	}

	// obtain file size:
	fseek (pFile , 0 , SEEK_END);
	lSize = ftell (pFile);
	rewind (pFile);

	// allocate memory to contain the whole file:
	tmp_buf = (unsigned char*) malloc (sizeof(unsigned char)*lSize);
	if (tmp_buf == NULL) {fputs ("Memory error",stderr); exit (2);}

	// copy the file into the buffer:
	result = fread (tmp_buf,1,lSize,pFile);

	if (result != lSize) {fputs ("Reading error",stderr); exit (3);}

	fclose (pFile);
	*buffer = tmp_buf;
	return 0;
}

/*
 *
 */
int get_lines(char *path, unsigned char ***tmp_lines)
{
	unsigned char *text,
	              *line = (unsigned char*) malloc(1000000 * sizeof(unsigned char)),
	             **lines;

	int line_count = 0,
	    line_size = 0,
	    i,
	    j;

	size_t length;

	read_file(path, &text);
	length = strlen(text);

	for(i = 0; i < length; i++)
		if(text[i] == '\n')
	
	line_count++;

	lines = (unsigned char**) malloc((line_count + 1) * sizeof(unsigned char*));
	line_count = 0;

	for(i = 0; i < length; i++)
	{
		if(text[i] != '\n')
			line[line_size++] = text[i];

		if(text[i] == '\n' || i == length - 1)
		{
			lines[line_count] = (unsigned char*) malloc((line_size + 1) * sizeof(unsigned char*));

			for(j = 0; j < line_size; j++)
				lines[line_count][j] = line[j];

			lines[line_count][line_size] = '\0';
			line_size = 0;
			line_count++;
		}
	}

	*tmp_lines = lines;
	return line_count;
}
