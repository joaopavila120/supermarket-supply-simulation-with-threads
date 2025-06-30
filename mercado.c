// João Paulo de Avila
// 178898@upf.br


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>

// semaforo
sem_t acesso_area;

int caixas_na_area  = 0;      
int total_entregue  = 0;  
int total_retiradas = 0;   
int id_funcionario  = 0; 
int N, B, C, F, T, L;

void *thread_caminhao(void *arg);
void *thread_funcionario(void *arg);
const char *nome_funcionario(int id);

const char *nomes_base[] = 
{
    "Bilguêiti", 
    "Torvalino", 
    "Ólanmusgo"
};

int main(int argc, char *argv[]) 
{
    if (argc != 7) 
    {
        printf("Uso correto é: %s N B C F T L\n", argv[0]);
        exit(1);
    }

    N = atoi(argv[1]); // = total de caixas
    B = atoi(argv[2]); // capacidade da area de carga
    C = atoi(argv[3]); // qt maxima de caixas por caminhao 
    F = atoi(argv[4]); // qt funcionarios 
    T = atoi(argv[5]); // deve-se gerar um valor aleatório para a quantidade de caixas de um pedido e efetuar a entrega
    L = atoi(argv[6]); // uso de usleep por um tempo aleatório, variando de acordo com L

    if (N <= 1 || N >= 100 || B <= 1 || B >= 10 || C <= 1 || C >= 5 ||
        F <= 1 || F >= 3   || T <= 1 || T >= 20 || L <= 1 || L >= 15) 
    {
        printf("Parametros inválidos. Favor verificar os limites.\n");
        exit(1);
    }

    // inicio 
    time_t inicio = time(NULL);
    printf("Início: %s", ctime(&inicio));

    srand(time(NULL));

    sem_init(&acesso_area, 0, 1);

    pthread_t th_caminhao;
    pthread_t th_func[F];

    printf("Criada thread que simula chegada de caminhões\n");
    pthread_create(&th_caminhao, NULL, thread_caminhao, NULL);

    usleep(500000);

    for (int i = 0; i < F; i++) 
    {
        printf("Criada thread que simula o funcionário %s\n", nome_funcionario(i));
        pthread_create(&th_func[i], NULL, thread_funcionario, NULL);
    }

    pthread_join(th_caminhao, NULL);

    for (int i = 0; i < F; i++) 
    {
        pthread_join(th_func[i], NULL);
    }

    sem_destroy(&acesso_area);

    time_t fim = time(NULL);
    printf("Fim: %s", ctime(&fim));

    return 0;
}

void *thread_caminhao(void *arg) 
{
    
    int caminhao_id = 1;

    while (total_entregue < N) {
        // aleatorio minimo 1 até C
        int pedido = (rand() % C) + 1;

        if (pedido + total_entregue > N) 
        {
            pedido = N - total_entregue;
        }

        printf("Caminhão %d, vai entregar %d caixa(s) na área de carga\n", caminhao_id, pedido);

        while (pedido > 0) 
        {
            // area de carga
            sem_wait(&acesso_area);

            int espaco      = B - caixas_na_area;
            int descarregar = (pedido <= espaco) ? pedido : espaco;

            if (descarregar > 0) 
            {
                caixas_na_area += descarregar;
                total_entregue += descarregar;
                pedido         -= descarregar;
                printf("Caminhão %d, descarregou %d caixa(s) na área de carga\n", caminhao_id, descarregar);
            }

            sem_post(&acesso_area);

            if (pedido > 0) 
            {
                usleep(500000);
            }
        }

        caminhao_id++;

        if (total_entregue >= N) 
        {
            printf("Encerrou thread caminhão\n");
            break;
        }

        sleep(rand() % T + 1);
    }

    return NULL;
}


const char *nome_funcionario(int id) 
{
    if (id >= 0 && id < 3) 
    {
        return nomes_base[id];
    } 
    else 
    {
        return "Funcionário Substituto";
    }
}


void *thread_funcionario(void *arg) 
{
    int id;

    sem_wait(&acesso_area);
    id = id_funcionario++;
    sem_post(&acesso_area);

    const char *nome = nome_funcionario(id);

    while (1) 
    {
        int retirou = 0;

        sem_wait(&acesso_area);

        if (caixas_na_area > 0) 
        {
            caixas_na_area--;
            total_retiradas++;
            retirou = 1;
        }

        else if (total_retiradas >= N && total_entregue >= N) 
        {
            sem_post(&acesso_area);
            printf("Trabalho do funcionário %s acabou\n", nome);
            break;
        }

        sem_post(&acesso_area);

        if (retirou) 
        {
            printf("Funcionário %s, levou uma caixa ao estoque\n", nome);

            usleep((rand() % L + 1) * 100000);
        } 
        else 
        {
            usleep(200000);
        }
    }

    return NULL;
}