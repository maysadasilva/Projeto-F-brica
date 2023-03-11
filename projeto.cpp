#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<conio.h>
#include<locale.h>

void cadastro();
void consulta_fabrica();
void consulta_gastos_fabrica();
void alteracao();
void exclusao_logica();
void exclusao_fisica();
void ordencacao_nome_decrescente();
void ordencacao_codigo_crescente();
void busca_codigo();
void recuperarbackup_fabrica();
int achar_fabrica();

struct fabrica
{
    char nome[38];
    int codigo;
    int total_funcionario;
    float gastos;
    int e;

}F;

FILE*fptr;

int main()
{
    setlocale(LC_ALL,"Portuguese");
    char op,s;
    int pos,elem;

    do{
        printf("\n1-Cadastro");
        printf("\n2-Consulta todas as fábrica");
        printf("\n3-Consulta as fábricas com gastos entre 10 milhões e 70 milhões");
        printf("\n4-Alteração");
        printf("\n5-Exclusão lógica");
        printf("\n6-Exclusão física");
        printf("\n7-Ordenação pelo nome em ordem decrescente");
        printf("\n8-Busca binária pelo código");
        printf("\n9-Recuperar backup de uma determinada fábrica");
        printf("\nS-sair");
        printf("\nDigite uma opção do menu:");
        op=getche();

        switch(op)
        {
            case '1':
            cadastro();
            break;

            case '2':
                consulta_fabrica();
            break;

            case '3':
                consulta_gastos_fabrica();
            break;

            case '4':
                alteracao();
            break;

            case '5':
                exclusao_logica();
            break;

            case '6':
                exclusao_fisica();
            break;

            case '7':
                ordencacao_nome_decrescente();
            break;

            case'8':
                busca_codigo();
                op = getche();
            break;

            case '9':
                recuperarbackup_fabrica();
                op = getche();
            break;

            case 'S':printf("\nSaindo...");
            break;

            default: printf("\nOpcao %c invalida!", op);

        }

    }while(op!='9' && op!='S');

    getchar();

    return(0);
}
void cadastro()
{
    if((fptr=fopen("fabrica.bi","rb+"))==NULL)
    {
        if((fptr=fopen("fabrica.bi", "wb"))==NULL)
        {
            printf("\nErro");
            exit(1);
        }
    }

    printf("\n-----------");
    printf("\nCodigo:");
    scanf("%d", &F.codigo);
    printf("-----------");
    printf("\nTotal de funcionários:");
    scanf("%d", &F.total_funcionario);
    printf("-----------");
    printf("\nNome:");
    setbuf(stdin,NULL);
    scanf("%[^\n]", F.nome);
    printf("-----------");
    printf("\nGastos da fábrica:");
    scanf("%f", &F.gastos);


    F.e=0;

    fseek(fptr,0,2);

    fwrite(&F,sizeof(F),1,fptr);
    fclose(fptr);
    printf("\n--------------------------");
}
void consulta_fabrica()
{
    if((fptr=fopen("fabrica.bi", "rb"))==NULL)
    {
        printf("\nErro");
        exit(1);

    }
    fseek(fptr,0,0);
    while(fread(&F, sizeof(F),1,fptr))
    {
        if (F.e==0)
        {
            printf("\nNome: %s", F.nome);
            printf("\n-----------");
            printf("\nCodigo: %d",F.codigo);
            printf("\n-----------");
            printf("\nGastos: %.2f",F.gastos);
            printf("\n-----------");
            printf("\nTotal de funcionários: %d",F.total_funcionario);
            printf("\n------------------------------------");
        }
    }
    printf("\nPrecione ENTER para continuar...");
    getch();
    fclose(fptr);
    printf("\n--------------------------");
}

void consulta_gastos_fabrica()
{
     if((fptr=fopen("fabrica.bi", "rb"))==NULL)
    {
        printf("\nErro");
        exit(1);

    }
    fseek(fptr,0,0);
    while(fread(&F, sizeof(F),1,fptr))
    {
        if (F.e==0)
        {
            if(F.gastos>=10000000 && F.gastos<=70000000)
            {
                printf("\nNome: %s", F.nome);
                printf("\n-----------");
                printf("\nCodigo: %d",F.codigo);
                printf("\n-----------");
                printf("\nGastos: %.2f",F.gastos);
                printf("\n-----------");
            }
        }
    }
    printf("\nPrecione ENTER para continuar...");
    getch();
    fclose(fptr);
    printf("\n--------------------------");
}
void alteracao()
{
    char aux[38];

    if((fptr=fopen("fabrica.bi","rb+"))==NULL)
    {
        printf("\nErro");
        exit(1);
    }
    printf("\n------------------------------------------");
    printf("\nDigite o nome da fábrica a ser encontrada:");
    setbuf(stdin,NULL);
    scanf("%[^\n]",aux);
    fseek(fptr,0,0);
    while(fread(&F,sizeof(F),1,fptr))
    {
        if(F.e==0)
        {
            if(strcmp(F.nome,aux)==0)
            {
                printf("\nDigite o novo gasto:");
                scanf("%f",&F.gastos);
                fseek(fptr,-sizeof(F),1);
                fwrite(&F,sizeof(F),1,fptr);
                fseek(fptr,0,2);
            }
        }
    }
}

void exclusao_logica()
{
    char aux[38];

    if((fptr=fopen("fabrica.bi","rb+"))==NULL)
    {
        printf("\nErro");
        exit(1);
    }
    printf("\n------------------------------------------");
    printf("\nDigite o nome da fábrica a ser encontrada:");
    setbuf(stdin,NULL);
    scanf("%[^\n]",aux);
    fseek(fptr,0,0);
    while(fread(&F,sizeof(F),1,fptr))
    {
        if(F.e==0)
        {
            if(strcmp(F.nome,aux)==0)
            {
                F.e=1;
                fseek(fptr, ftell(fptr)-sizeof(F), 0);
                fwrite(&F,sizeof(F),1,fptr);
                fseek(fptr,0,2);
            }
        }
    }
    printf("------------------------------------------");
}
void exclusao_fisica()
{
    FILE*fptraux,*fptrback;

    if((fptr=fopen("fabrica.bi","rb+"))==NULL)
    {
        printf("\nErro");
        exit(1);
    }
    if((fptraux=fopen("auxiliar.bi","wb"))==NULL)
    {
        printf("\nErro!");
        exit(1);
    }
    if((fptrback=fopen("back.bi","wb"))==NULL)
    {
        printf("\Erro!");
        exit(1);
    }
    fseek(fptr,0,0);

    while(fread(&F,sizeof(F),1,fptr))
    {
        if(F.e==0)
        {
            fseek(fptraux, 0,2);
            fwrite(&F,sizeof(F),1,fptraux);
        }
        else
        {
            fseek(fptrback,0,2);
            fwrite(&F,sizeof(F), 1, fptrback);
        }
    }

        fclose(fptr);
        fclose(fptraux);
        fclose(fptrback);

        remove("fabrica.bi");
        rename("auxiliar.bi", "fabrica.bi");
}

void ordencacao_nome_decrescente()
{
    int i,j,n;

    struct fabrica FI,FJ;

    if((fptr=fopen("fabrica.bi","rb+"))==NULL)
    {
        printf("\nErro!");
        exit(1);
    }
    fseek(fptr,0,2);
    n=ftell(fptr)/sizeof(F);

    for(i=0;i<n-1;i++)
    {
        for(j=0;j<n;j++)
        {
            fseek(fptr, i*sizeof(FI), 0);
            fread(&FI, sizeof(FI), 1, fptr);
            fseek(fptr, j*sizeof(F), 0);
            fread(&FJ, sizeof(F), 1, fptr);

            if(strcmp(FI.nome,FJ.nome)>0)
            {
                fseek(fptr, i*sizeof(F), 0);
                fwrite(&FJ, sizeof(F), 1, fptr);
                fseek(fptr, j*sizeof(F), 0);
                fwrite(&FI, sizeof(F),1,fptr);
            }
        }
    }
    fclose(fptr);
}

void ordencacao_codigo_crescente()
{
    int i,j,n,aux;
    struct fabrica FI,FJ;

    if((fptr=fopen("fabrica.bi","rb+"))==NULL)
    {
        printf("\nErro!");
        exit(1);
    }
    fseek(fptr,0,2);
    n=ftell(fptr)/sizeof(F);

    for(i=0;i<n-1;i++)
    {
        for(j=i+1;j<n;j++)
        {
            fseek(fptr, i*sizeof(FI), 0);
            fread(&FI, sizeof(FI), 1, fptr);
            fseek(fptr, j*sizeof(F), 0);
            fread(&FJ, sizeof(F), 1, fptr);

            if(FI.codigo>FJ.codigo)
            {
                fseek(fptr,i*sizeof(F),0);
                fwrite(&FJ,sizeof(F),1,fptr);
                fseek(fptr,j*sizeof(F),0);
                fwrite(&FI,sizeof(F),1,fptr);

            }
        }
    }

    fclose(fptr);
}


void busca_codigo()
{
    struct fabrica FI, FJ;
    int i,j,posicao;
    int n;

    if((fptr=fopen("fabrica.bi","rb+"))==NULL)
    {
        printf("\nErro!");
        exit(1);
    }
    fseek(fptr,0,2);
    n=ftell(fptr)/sizeof(F);

    for(i=0;i<n-1;i++)
    {
        for(j=i+1;j<n;j++)
        {
            fseek(fptr, i * sizeof(FI), 0);
            fread(&FI, sizeof(FI), 1, fptr);

            fseek(fptr, j * sizeof(FJ), 0);
            fread(&FJ, sizeof(FJ), 1, fptr);

            if(FI.codigo > FJ.codigo)
            {
                fseek(fptr, i * sizeof(FJ), 0);
                fwrite(&FJ, sizeof(FJ), 1, fptr);

                fseek(fptr, j * sizeof(FI), 0);
                fwrite(&FI, sizeof(FI), 1, fptr);
            }
        }
    }
    fclose(fptr);

    posicao=achar_fabrica();

    if(posicao!=-1)
    {
        printf("\nA fábrica foi encontrada está na posição:%d", posicao);
        printf("\n-----------------------------------------");
    }
    else
    {
        printf("\nFábrica não encontrada!");
    }

    getchar();
}
int achar_fabrica()
{
    int low, mid, high, n;
    int buscar_codigo;

    if((fptr=fopen("fabrica.bi","rb+"))==NULL)
    {
        printf("\nErro!");
        exit(1);
    }
    printf("\n----------------------------");
    printf("\nDigite o código a ser encontrado:");
    scanf("%d", &buscar_codigo);

    fseek(fptr, 0, 2);
    n = ftell(fptr) / sizeof(F);

    low=0;
    high=n-1;
    while(low<=high)
    {
        mid=(low+high)/2;
        fseek(fptr, mid*sizeof(F), 0);
        fread(&F, sizeof(F), 1, fptr);

        if(F.codigo>buscar_codigo)
        {
            high=mid-1;
        }
        else if(F.codigo<buscar_codigo)
        {
            low=mid+1;
        }
        else
        {
            return mid;
        }
    }
    fclose(fptr);
    return-1;

}

void recuperarbackup_fabrica()
{
    int codigo_fabrica;

    FILE*fptr_auxiliar,*fptr_backup;

    if((fptr=fopen("fabrica.bi","rb+"))==NULL)
    {
        if((fptr=fopen("fabrica.bi","wb"))==NULL)
        {
            printf("\nErro!");
            exit(1);
        }
    }
    if((fptr_auxiliar=fopen("auxiliar_fabrica.bi","wb"))==NULL)
    {
        printf("\nErro!");
        exit(1);
    }
    if((fptr_backup=fopen("backup_fabrica.bi","rb+"))==NULL)
    {
        if((fptr_backup=fopen("backup_fabrica.bi","wb"))==NULL)
        {
            printf("\nErro!");
            exit(1);
        }
    }

    fseek(fptr_backup,0,0);

    printf("\nDigite o código de qual fábrica que você deseja restaurar:");
    scanf("%d%*c", &codigo_fabrica);

    while(fread(&F,sizeof(F),1,fptr_backup))
    {
        if(F.e==1)
        {
            if(F.codigo==codigo_fabrica)
            {
                fseek(fptr,0,2);
                F.e=0;
                fwrite(&F,sizeof(F),1,fptr);
            }
            else
            {
                fseek(fptr,0,2);
                fwrite(&F, sizeof(F),1,fptr_auxiliar);
            }
        }
    }
    fclose(fptr);
    fclose(fptr_auxiliar);
    fclose(fptr_backup);

    remove("backup_fabrica.bi");
    rename("auxiliar_fabrica.bi", "backup_fabrica.bi");
}




