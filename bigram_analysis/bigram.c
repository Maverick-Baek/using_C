
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define N 311

typedef struct Bucket{
    char Bigram[20];
    int frequency;
    struct Bucket* nextBucket;
}Bucket;


void Lower(char* lines);//소문자 변환 함수
void Hash(char* bigram);//hash 계산 함수
void Sort(char* str_list[]);// sort 함수
void find(char* bigram, int sum);// hashing 하기위해 insert하는함수
void Trim(char* str);//앞뒤 공백 및 개행문자 제거
void make_bigram(char str[][30], int max_ind);//bigram 만들어서 hashing


struct Bucket* Buckets[N];
char temp_bigram[30];
char* last_word;
int first_flag = 0;

int main(){
    FILE* fp = fopen("example.txt", "r");
    char sLine[100];
    char* temp_word = "";
    char word_list[30][30];//다음 시도에는 이거 없이 find로 구현해보기
    int ind = 0;

    while(fp != NULL){
        if(fgets(sLine,100,fp) == NULL){
            break;
        }
        memset(word_list, 0, sizeof(char**) * 30);
        
        ind = 0;
        
        Lower(sLine);
        Trim(sLine);
        temp_word = strtok(sLine, " ");
        while(temp_word != NULL){
            strcpy(word_list[ind++], temp_word);
            temp_word = strtok(NULL, " ");
        }
        make_bigram(word_list, ind);
        strcpy(temp_bigram, word_list[ind -1]);
    }
    Bucket* b;
    b = Buckets[0];
    while( b->nextBucket != NULL){
        printf("bigram : %s, fre : %d\n",b->Bigram,b->frequency);
        b = b->nextBucket;
    }
    printf("bigram : %s, fre : %d\n",b->Bigram,b->frequency);




    fclose(fp);

    return 0;
}


//소문자 변환 함수
void Lower(char* lines){
    lines = strlwr(lines);
}


//hash 계산 함수
void Hash(char* bigram){
    int sum = 0;
    int bi_len = strlen(bigram);
    for(int i = 0; i < bi_len; i++){
        sum += (int)(bigram[i]);
    }
    sum -= 32;
    find(bigram, sum);
}


//sort 함수
void Sort(char* str_list[]){

}


// hashing 하기위해 insert하는함수
void find(char* bigram, int sum){
    int find_flag = 0;
    sum = sum - ((int)(sum / N) * N);
    if(Buckets[sum] != NULL){
        Bucket* temp_bucket;
        temp_bucket = Buckets[sum];
        while(temp_bucket != NULL){
            if(strcmp(temp_bucket->Bigram,bigram) == 0){
                temp_bucket->frequency = temp_bucket->frequency + 1;
                find_flag++;
                break;
            }
        }
        temp_bucket = temp_bucket->nextBucket;
    }
    
    if(find_flag == 0){
        Bucket* new_bucket;
        new_bucket = (Bucket*)malloc(sizeof(Bucket));
        new_bucket->frequency = 1;
        strcpy(new_bucket->Bigram, bigram);
        if(Buckets[sum] != NULL){
            new_bucket->nextBucket = Buckets[sum];
        }
        Buckets[sum] = new_bucket;
    }
}

//앞뒤 공백 및 개행문자 제거
void Trim(char* str) {
    int start = 0;
    int end = strlen(str) - 1;
    while (isspace(str[start])) {
        start++;
    }
    while (end > start && isspace(str[end])) {
        end--;
    }
    for (int i = start; i <= end; i++) {
        str[i - start] = str[i];
    }

    str[end - start + 1] = '\0';
}


void make_bigram(char str[][30],int max_ind){
    if(first_flag != 0){
        strcat(temp_bigram, " ");
        strcat(temp_bigram, str[0]);
        Hash(temp_bigram);

    }
    memset(temp_bigram, 0, sizeof(char*) * 30);
    for(int i = 0; i < max_ind - 1; i++){
        strcpy(temp_bigram, str[i]);
        strcat(temp_bigram, " ");
        strcat(temp_bigram, str[i+1]);
        Hash(temp_bigram);
        memset(temp_bigram, 0, sizeof(char*) * 30);
    }
    first_flag++;
}

//모든 동적할당 struct를 free하게 하는 함수
void delete_all_node(Bucket* h)
{
    Bucket* cur;
    cur = h;

    Bucket* nxt;
    while(cur != NULL)
    {
        nxt = cur->nextBucket;

        free(cur);
        cur = nxt;
    }
    free(h);
}
