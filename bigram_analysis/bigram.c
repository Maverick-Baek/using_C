//version1
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define N 1003

typedef struct Bucket{
    char Bigram[50];
    int frequency;
    struct Bucket* nextBucket;
}Bucket;

char* Lower(char* lines);//소문자 변환 함수
void Hash(char* bigram);//hash 계산 함수
void Sort(struct Bucket* start);// sort 함수
void find(char* bigram, int sum);// hashing 하기위해 insert하는함수
void Trim(char* str);//앞뒤 공백 및 특수문자 제거
void make_bigram(char str[][50], int max_ind);//bigram 만들어서 hashing
int find_list_len(struct Bucket* h);//linked list의 길이 구하기
void quicksort(struct Bucket* h, int start, int fin); // quick sort
void swap(struct Bucket* h, int a, int b); //원하는 두 ind 값을 swap
struct Bucket* move(struct Bucket* h, int det); //원하는 list index까지 이동
void print_max(); // 최대개수부터 30개 출력

Bucket* Buckets[N];
char temp_bigram[30];
int first_flag = 0;
Bucket* fin;


int main(){
    FILE* fp = fopen("shakespeare.txt", "r");
    char sLine[100];
    char* temp_word = "";
    char word_list[30][50];//다음 시도에는 이거 없이 find로 구현해보기
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
   
    
    for(int i = 0 ;i<N; i++){
        Sort(Buckets[i]);
    }
    print_max();

    fclose(fp);

    return 0;
}


//소문자 변환 함수
char* Lower(char* lines){
    char* p = lines;
    while(*p){
        *p = tolower(*p);
        p++;
    }
    return lines;
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
void Sort(struct Bucket* start){
    quicksort(start,0,find_list_len(start) - 1);
}


// hashing 하기위해 insert하는함수
void find(char* bigram, int sum){
    int find_flag = 0;
    sum = sum - ((int)(sum / N) * N);
    Bucket* temp_bucket;
    if(Buckets[sum] != NULL){
        temp_bucket = Buckets[sum];
        while(temp_bucket != NULL){
            if(strcmp(temp_bucket->Bigram,bigram) == 0){
                temp_bucket->frequency = temp_bucket->frequency + 1;
                find_flag++;
                break;
            }
            temp_bucket = temp_bucket->nextBucket;
        }
    }
    
    if(find_flag == 0){
        temp_bucket = (Bucket*)malloc(sizeof(Bucket));
        temp_bucket->nextBucket = NULL;
        temp_bucket->frequency = 1;
        strcpy(temp_bucket->Bigram, bigram);
        if(Buckets[sum] != NULL){
            temp_bucket->nextBucket = Buckets[sum];
        }
        Buckets[sum] = temp_bucket;
    }
}

//앞뒤 공백 및 특수문자 제거
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


void make_bigram(char str[][50],int max_ind){
    if(first_flag != 0){
        strcat(temp_bigram, " ");
        strcat(temp_bigram, str[0]);
        Hash(temp_bigram);

    }
    memset(temp_bigram, 0, sizeof(char*) * 50);
    for(int i = 0; i < max_ind - 1; i++){
        strcpy(temp_bigram, str[i]);
        strcat(temp_bigram, " ");
        strcat(temp_bigram, str[i+1]);
        Hash(temp_bigram);
        memset(temp_bigram, 0, sizeof(char*) * 50);
    }
    first_flag++;
}



//linked list의 길이 구하기
int find_list_len(struct Bucket* h){
    Bucket* temp;
    temp = h;
    int len = 0;
    while(temp != NULL){
        len++;
        temp = temp->nextBucket;
    }
    return len;
}

//원하는 linked list index까지 이동
struct Bucket* move(struct Bucket* h, int det){
    Bucket* temp = h;
    for(int i = 0; i < det; i++){
        if((temp == NULL) || (temp->nextBucket == NULL)){
            break;
        }
        temp = temp->nextBucket;
    }
    return temp;
}

//원하는 두 ind 값을 swap
void swap(struct Bucket* h, int a, int b){
    if(a == b){
        return;
    }
    Bucket* aa = move(h,a);
    Bucket* bb = move(h,b);
    Bucket* temp ;
    int temp_i;
    temp = (Bucket*)malloc(sizeof(Bucket));
    strcpy(temp->Bigram, aa->Bigram);
    temp_i = aa->frequency;
    strcpy(aa->Bigram, bb->Bigram);
    aa->frequency = bb->frequency;
    strcpy(bb->Bigram, temp->Bigram);
    bb->frequency = temp_i;
    free(temp);
}

//quick sort
void quicksort(struct Bucket* h, int start, int fin){
    if(start >= fin){
        return;
    }
    else if(start - fin == -1){
        if(move(h, start) < move(h, fin)){
            swap(h,start,fin);
        }
        return;
    }
    int start_f = start + 1;
    int fin_s = fin;
    while(start_f < fin_s){
        int k = move(h,start)->frequency;
        if(k == move(h,start_f)->frequency){
            start_f++;
        }
        else if(k == move(h,fin_s)->frequency){
            fin_s--;
        }
        else if((k - move(h,start_f)->frequency) * (k - move(h,fin_s)->frequency) < 0 ){
            if(move(h,start_f)->frequency < move(h,fin_s)->frequency){
                swap(h,start_f,fin_s);
            }
            start_f++;
            fin_s--;
        }
        else if(move(h, start_f)->frequency < k){
            swap(h, start_f, fin_s - 1);
            fin_s--;
        }
        else if(move(h, start_f)->frequency > k){
            swap(h, start_f + 1,fin_s);
            start_f++;
        }
        else{
            start_f++;
            fin_s--;
        }
    }
    start_f--;
    fin_s++;
    if(start_f + 2 == fin_s){
        if(move(h,start)->frequency > move(h,start_f + 1)->frequency){
            swap(h,start,start_f);
            quicksort(h,start,start_f-1);
        }
        else{
            swap(h,start,start_f + 1);
            quicksort(h,start,start_f);
        }
        quicksort(h, fin_s, fin);
    }
    else{
        swap(h,start,start_f);
        quicksort(h,start,start_f - 1);
        quicksort(h, fin_s,fin);
    }


}

void print_max(){
    int max_fre = -1;
    int max_ind = -1;
    Bucket* temp;
    for(int i = 0; i <= 31; i++){
        max_fre = -1;
        max_ind = -1;
        for(int j = 0; j < N; j++){
            temp = Buckets[j];
            if(temp->frequency > max_fre){
                max_fre = temp->frequency;
                max_ind = j;
            }
        }
        if(i <= 1){
            continue;
        }
        printf("%dst bigram : %s, frequency : %d\n",i - 1,Buckets[max_ind]->Bigram,Buckets[max_ind]->frequency);
        temp = Buckets[max_ind];
        Buckets[max_ind] = temp->nextBucket;
    }
}
