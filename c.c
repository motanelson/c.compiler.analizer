#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_FUNCOES 100
#define MAX_LINHAS 1000
#define MAX_NOME 64
#define MAX_LINHA 512
char backLine[MAX_LINHA];
char *backLines=backLine;
int came1=0;
int lineb=0;
int linec=0;
int linhas=0;
typedef struct {
    char nome[MAX_NOME];
    char linhas[MAX_LINHAS][MAX_LINHA];
    int linha_count;
    char vars[MAX_LINHAS][MAX_LINHA];
    int var_count;
} Subrotina;

Subrotina funcoes[MAX_FUNCOES];
int func_count = 0;
char chamadas_indefinidas[MAX_FUNCOES][MAX_NOME];
int chamadas_count = 0;
int for_count = 0;
int if_count = 0;
int while_count = 0;

void trim(char* s) {
    char* p = s;
    int l = strlen(p);
    while (isspace(p[l - 1])) p[--l] = 0;
    while (*p && isspace(*p)) ++p, --l;
    memmove(s, p, l + 1);
}

void adicionar_chamada_indefinida(const char* nome) {
    for (int i = 0; i < func_count; i++)
        if (strcmp(funcoes[i].nome, nome) == 0) return;

    for (int i = 0; i < chamadas_count; i++)
        if (strcmp(chamadas_indefinidas[i], nome) == 0) return;

    strcpy(chamadas_indefinidas[chamadas_count++], nome);
}

void processar_linha(Subrotina* f, const char* linha) {
    char l[MAX_LINHA];
    strcpy(l, linha);
    trim(l);
    
    if (strncmp(l, "call ", 5) == 0) {
        char destino[1000];
        sscanf(l,"call %31[^; ];",destino);
        trim(destino);
        sprintf(f->linhas[f->linha_count++], "call %s", destino);
        adicionar_chamada_indefinida(destino);
        came1=1;

    } else if (strncmp(l, "int ", 4) == 0) {
        char* nome = strtok(l + 4, "=");
        char* valor = strtok(NULL, ";");
        if (nome && valor) {
            trim(nome);
            trim(valor);
            sprintf(f->vars[f->var_count++], "%s_%s: dw %s", nome, f->nome, valor);
            came1=1;
        }

    } else if (strstr(l, "char*") || strstr(l, "char *")) {
        char* nome = strtok(strstr(l, "*") + 1, "=");
        char* valor = strtok(NULL, ";");
        if (nome && valor) {
            trim(nome);
            trim(valor);
            if (valor[0] == '"') valor++;
            char* end = strchr(valor, '"');
            if (end) *end = '\0';
            sprintf(f->vars[f->var_count++], "%s_%s: db '%s', 0", nome, f->nome, valor);
            came1=1;
        }
   
   } else if (strstr(l, "char ") ) {
        char nomes[1000];
        char valores[1000];
        char* nome = nomes ;
        char* valor = valores;
        sscanf(l,"char %31[^=]=\"%100[^\"]\";",nomes,valores);
        trim(nomes);
        trim(valores);
        if (nome && valor) {
            trim(nome);
            trim(valor);
            if (valor[0] == '"') valor++;
            char* end = strchr(valor, '"');
            if (end) *end = '\0';
            sprintf(f->vars[f->var_count++], "%s_%s: db '%s', 0", nome, f->nome, valor);
            came1=1;
        }
    } else if (strncmp(l, "float ", 6) == 0) {
        char* nome = strtok(l + 6, "=");
        char* valor = strtok(NULL, ";");
        if (nome && valor) {
            trim(nome);
            trim(valor);
            sprintf(f->vars[f->var_count++], "%s_%s: dw %s", nome, f->nome, valor);
            came1=1;
        }
    } else if (strncmp(l, "long ", 5) == 0) {
        char* nome = strtok(l + 5, "=");
        char* valor = strtok(NULL, ";");
        if (nome && valor) {
            trim(nome);
            trim(valor);
            sprintf(f->vars[f->var_count++], "%s_%s: dd %s", nome, f->nome, valor);
            came1=1;
        }
    } else if (strstr(l, " = ") != NULL) {
        char* nome = strtok(l + 0, "=");
        char* valor = strtok(NULL, ";");
        if (nome && valor) {
            trim(nome);
            trim(valor);
            if(valor[0]>='0' && valor[0]<='9'){
                sprintf(f->linhas[f->linha_count++], "mov %s_%s, %s", nome, f->nome, valor);
            }else{
                sprintf(f->linhas[f->linha_count++], "mov ax, %s_%s",  valor, f->nome);
                sprintf(f->linhas[f->linha_count++], "mov %s_%s, ax", nome, f->nome);
            }
            came1=1;
        }


    } else if (strncmp(l, "return ", 7) == 0) {
        char vals[1000];
        char* val = vals;
        sscanf(l, "return %31[^; ];", vals);
        trim(vals);
        if(val[0]>='0' && val[0]<='9'){
            sprintf(f->linhas[f->linha_count++], "mov ax, %s", val);
        }else{
            sprintf(f->linhas[f->linha_count++], "mov ax, %s_%s", val, f->nome);
        }
        sprintf(f->linhas[f->linha_count++], "ret");
        came1=1;

    } else if (strncmp(l, "asm ", 4) == 0) {
        char vals[1000];
        char* val = vals;
        sscanf(l, "asm %100[^;];", vals);
        trim(vals);
        sprintf(f->linhas[f->linha_count++], "%s", val);
        
        came1=1;


    } else if (strncmp(l, "if (", 4) == 0) {
        char var[32], op[3], val[32];
        sscanf(l, "if (%31[^=<>!]%2[=!<>]%31[^)]", var, op, val);
        char label[32];
        sprintf(label, "if_end_%d", if_count++);
        trim(var); trim(val);

        if (strstr(l, "==")) {
            if(val[0]>='0' && val[0]<='9'){
                sprintf(f->linhas[f->linha_count++], "cmp %s_%s, %s", var, f->nome, val);
            }else{
               sprintf(f->linhas[f->linha_count++], "mov ax, %s_%s", var, f->nome);
               sprintf(f->linhas[f->linha_count++], "cmp ax, %s_%s", val, f->nome);

            }
            sprintf(f->linhas[f->linha_count++], "jne %s", label);
        } else if (strstr(l, "!=")) {
            if(val[0]>='0' && val[0]<='9'){
                sprintf(f->linhas[f->linha_count++], "cmp %s_%s, %s", var, f->nome, val);
            }else{
               sprintf(f->linhas[f->linha_count++], "mov ax, %s_%s", var, f->nome);
               sprintf(f->linhas[f->linha_count++], "cmp ax, %s_%s", val, f->nome);
            }
            sprintf(f->linhas[f->linha_count++], "je %s", label);
        } else if (strstr(l, ">=")) {
            if(val[0]>='0' && val[0]<='9'){
                sprintf(f->linhas[f->linha_count++], "cmp %s_%s, %s", var, f->nome, val);
            }else{
               sprintf(f->linhas[f->linha_count++], "mov ax, %s_%s", var, f->nome);
               sprintf(f->linhas[f->linha_count++], "cmp ax, %s_%s", val, f->nome);

            }
            sprintf(f->linhas[f->linha_count++], "jl %s", label);
        } else if (strstr(l, "<=")) {
            if(val[0]>='0' && val[0]<='9'){
                sprintf(f->linhas[f->linha_count++], "cmp %s_%s, %s", var, f->nome, val);
            }else{
               sprintf(f->linhas[f->linha_count++], "mov ax, %s_%s", var, f->nome);
               sprintf(f->linhas[f->linha_count++], "cmp ax, %s_%s", val, f->nome);

            }
             sprintf(f->linhas[f->linha_count++], "jg %s", label);
        } else if (strstr(l, ">")) {
            if(val[0]>='0' && val[0]<='9'){
                 sprintf(f->linhas[f->linha_count++], "cmp %s_%s, %s", var, f->nome, val);
            }else{
               sprintf(f->linhas[f->linha_count++], "mov ax, %s_%s", var, f->nome);
               sprintf(f->linhas[f->linha_count++], "cmp ax, %s_%s", val, f->nome);
            }
            sprintf(f->linhas[f->linha_count++], "jle %s", label);
        } else if (strstr(l, "<")) {
            if(val[0]>='0' && val[0]<='9'){
                sprintf(f->linhas[f->linha_count++], "cmp %s_%s, %s", var, f->nome, val);
            }else{
               sprintf(f->linhas[f->linha_count++], "mov ax, %s_%s", var, f->nome);
               sprintf(f->linhas[f->linha_count++], "cmp ax, %s_%s", val, f->nome);

            }
            sprintf(f->linhas[f->linha_count++], "jge %s", label);
        }

        sprintf(f->linhas[f->linha_count++], "; ... (bloco do if)");
        sprintf(backLines, "%s:", label);
        linec=0;
        lineb=1;
        came1=0;

    } else if (strncmp(l, "while (", 7) == 0) {
        char var[32], op[3], val[32];
        sscanf(l, "while (%31[^=<>!]%2[=!<>]%31[^)]", var, op, val);
        char label_topo[32], label_fim[32];
        sprintf(label_topo, "loop_%d", while_count);
        sprintf(label_fim, "endloop_%d", while_count);
        while_count++;
        trim(var); trim(val);
        sprintf(f->linhas[f->linha_count++], "%s:", label_topo);
        if (strstr(l, "==")) {
            
            sprintf(f->linhas[f->linha_count++], "cmp %s_%s, %s", var, f->nome, val);
            sprintf(f->linhas[f->linha_count++], "jne %s", label_fim);
        } else if (strstr(l, "!=")) {
            sprintf(f->linhas[f->linha_count++], "cmp %s_%s, %s", var, f->nome, val);
            sprintf(f->linhas[f->linha_count++], "je %s", label_fim);
        } else if (strstr(l, ">")) {
            sprintf(f->linhas[f->linha_count++], "cmp %s_%s, %s", var, f->nome, val);
            sprintf(f->linhas[f->linha_count++], "jle %s", label_fim);
        } else if (strstr(l, "<")) {
            sprintf(f->linhas[f->linha_count++], "cmp %s_%s, %s", var, f->nome, val);
            sprintf(f->linhas[f->linha_count++], "jge %s", label_fim);
        }

        sprintf(f->linhas[f->linha_count++], "; ... (bloco do while)");
        sprintf(f->linhas[f->linha_count++], "jmp %s", label_topo);
        sprintf(backLines, "%s:", label_fim);
        linec=0;
        lineb=1;
        came1=0;

    } else if (strncmp(l, "for (", 5) == 0) {
        char var[32], op[3], val[32],vars[32],values[32],adds[32];
        sscanf(l, "for (%31[^=]=%31[^;];%31[^=<>!]%2[=!<>]%31[^;];%31[^+-])",vars,values, var, op, val,adds);
        char label_topo[32], label_fim[32];
        sprintf(label_topo, "fors_%d", for_count);
        sprintf(label_fim, "endfors_%d", for_count);
        for_count++;
        trim(var); trim(val);
        sprintf(f->linhas[f->linha_count++], "mov %s_%s,%s", vars, f->nome,values);
        sprintf(f->linhas[f->linha_count++], "%s:", label_topo);
        if (strstr(l, "==")) {
            sprintf(f->linhas[f->linha_count++], "cmp %s_%s, %s", var, f->nome, val);
            sprintf(f->linhas[f->linha_count++], "jne %s", label_fim);
        } else if (strstr(l, "!=")) {
            sprintf(f->linhas[f->linha_count++], "cmp %s_%s, %s", var, f->nome, val);
            sprintf(f->linhas[f->linha_count++], "je %s", label_fim);
        } else if (strstr(l, ">")) {
            sprintf(f->linhas[f->linha_count++], "cmp %s_%s, %s", var, f->nome, val);
            sprintf(f->linhas[f->linha_count++], "jle %s", label_fim);
        } else if (strstr(l, "<")) {
            sprintf(f->linhas[f->linha_count++], "cmp %s_%s, %s", var, f->nome, val);
            sprintf(f->linhas[f->linha_count++], "jge %s", label_fim);
        }
        sprintf(f->linhas[f->linha_count++], "inc %s_%s", adds, f->nome);
        sprintf(f->linhas[f->linha_count++], "; ... (bloco do for)");
        sprintf(f->linhas[f->linha_count++], "jmp %s", label_topo);
        sprintf(backLines, "%s:", label_fim);
        linec=0;
        lineb=1;
        came1=0;
    } else {
        char cccc[1000];
        strcpy(cccc,l);
        trim(cccc);
        if(came1==0 && strlen(cccc)>0){
            printf("error on line:%d\n",linhas);
            sprintf(f->linhas[f->linha_count++], "; error line: %d", linhas);
        }
        sprintf(f->linhas[f->linha_count++], "; ignorado: %s", l);
    }
    if(lineb==1 && linec==1){
        lineb=0;
        sprintf(f->linhas[f->linha_count++], "%s",backLines);
    }
    linec++;
    
}

void processar_codigo(const char* codigo) {
    const char* p = codigo;
    while ((p = strstr(p, "void ")) != NULL) {
        linhas++;
        p += 5;
        char nome[MAX_NOME] = {0};
        sscanf(p, "%31[^()]()", nome);
        char* abre = strchr(p, '{');
        if (!abre) break;
        abre++;
        char* fecha = strchr(abre, '}');
        if (!fecha) break;

        Subrotina* f = &funcoes[func_count++];
        strcpy(f->nome, nome);
        came1=1;
        f->linha_count = 0;
        f->var_count = 0;

        char buffer[MAX_LINHA];
        const char* linha = abre;
        while (linha < fecha) {
            const char* prox = strchr(linha, '\n');
            int len = prox ? (prox - linha) : strlen(linha);
            strncpy(buffer, linha, len);
            buffer[len] = '\0';
            linhas++;
            processar_linha(f, buffer);
            came1=0;
            if (!prox) break;
            linha = prox + 1;
        }
        p = fecha + 1;
    }
}

void gravar_saida(const char* nome_saida) {
    FILE* f = fopen(nome_saida, "w");
    if (!f) {
        perror("Erro ao gravar");
        return;
    }

    fprintf(f, "; Código Assembly 16-bit (NASM)\n\nsection .text\n\n");

    for (int i = 0; i < func_count; i++) {
        fprintf(f, "%s:\n", funcoes[i].nome);
        for (int j = 0; j < funcoes[i].linha_count; j++)
            fprintf(f, "    %s\n", funcoes[i].linhas[j]);
        fprintf(f, "\n");
    }

    fprintf(f, "section .data\n\n");
    for (int i = 0; i < func_count; i++)
        for (int j = 0; j < funcoes[i].var_count; j++)
            fprintf(f, "%s\n", funcoes[i].vars[j]);

    for (int i = 0; i < chamadas_count; i++) {
        int definida = 0;
        for (int j = 0; j < func_count; j++)
            if (strcmp(funcoes[j].nome, chamadas_indefinidas[i]) == 0)
                definida = 1;
        if (!definida)
            fprintf(f, "%s:\n    ; chamada indefinida\n    ret\n\n", chamadas_indefinidas[i]);
    }

    fclose(f);
    printf("Compilação concluída: %s\n", nome_saida);
}

int main() {
    char nome[256];
    printf("\033c\033[43;30m\nNome do ficheiro C (.c): ");
    scanf("%s", nome);

    FILE* f = fopen(nome, "r");
    if (!f) {
        perror("Erro ao abrir o ficheiro");
        return 1;
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);

    char* buffer = (char*)malloc(size + 1);
    fread(buffer, 1, size, f);
    buffer[size] = '\0';
    fclose(f);

    processar_codigo(buffer);

    char saida[256];
    strcpy(saida, nome);
    char* ponto = strrchr(saida, '.');
    if (ponto) strcpy(ponto, ".S");
    else strcat(saida, ".S");

    gravar_saida(saida);
    free(buffer);
    return 0;
}
