#include <stdio.h>
#include <string.h>
#include <time.h>
#define N 105
#define PROBlEM_NUM 10
#define abs(x) ((x > 0) ? (x) : -(x))

char name[N];       // 用户名
int id;             // 用户id
char id_str[N];     // 用户名的char*格式
const char *tag = "+-*/";
const double eps = 0.01;    // 精度，相差小于这个会被认为是正确的
typedef struct Student {
    int id;
    char name[N];
    double score;
    int num;
} student;


int str_to_int(char str[]) {
    int res = 0;
    for (int i = 0; str[i]; i++) {
        res = res * 10 + (str[i] - '0');
    }
    return res;
}

int get_nextId() {
    FILE* fp = fopen("./user.txt", "rw");
    rewind(fp);
    char cur[N], id_str[N];
    while (1) {
        if (fgets(cur, 1024, fp) == NULL) {
            int next_id = str_to_int(id_str) + 1;
            // 转化为字符串
            itoa(next_id, id_str, 10);
            fclose(fp);
            return next_id;
        }
        strcpy(id_str, cur);
    }
}

int check_register() {
    FILE* fp = fopen("./user.txt", "rw");
    rewind(fp);
    while (1) {
        char temp[N];
        if (fgets(temp, 1024, fp) == NULL)
            break;
        temp[strlen(temp) - 1] = '\0';
        if (strcmp(temp, name) == 0) {
            fgets(id_str, 1024, fp);
            id_str[strlen(id_str) - 1] = '\0';
            fclose(fp);
            return id = str_to_int(id_str);
        }
    }
    fclose(fp);
    return 0;
}

void write_user() {
    FILE* fp = fopen("./user.txt", "a+");
    if (fp != NULL) {
        fputs("\n", fp);
        fputs(name, fp);
        fputs("\n", fp);
        fputs(id_str, fp);
        fclose(fp);
    }
}

double get_result(int a, int b, int f) {
    switch (f) {
        case 0: return a + b;
        case 1: return a - b;
        case 2: return a * b;
        case 3: return (double) a / b;
    }
}

// return answer
double show_problem() {
    int a, b, f;
    a = rand() % 100;
    b = rand() % 100;
    f = rand() % 4;
    if (f != 3) {
        while(1) {
            a = rand() % 100;
            b = rand() % 100;
            double result = get_result(a, b, f);
            if (result >= 0 && result < 100) {
                break;
            }
        }
    } else {
        while(1) {
            b = rand() % 100;
            if (b != 0) break;
        }
        while(1) {
            a = rand() % 100;
            if (a % b == 0 && a / b < 100) {
                break;
            }
        }
    }
    printf("%d %c %d = ", a, tag[f], b);
    return get_result(a, b, f);
}

int ask_problem() {
    double answer = show_problem();
    double user_answer;
    scanf("%lf", &user_answer);
    if (abs(user_answer - answer) < eps) {
        return 1;
    } else {
        return 0;
    }
}

void ask_name() {
    printf("What's your name?\n");
    // 检查是否已经注册
    gets(name);
    if (check_register()) {
        // 如果注册过
        printf("You have already registered this system, welcome!\n");
    } else {
        // 未注册过，则需要获取id
        printf("Register successfully, welcome\n");
        id = get_nextId();
        itoa(id, id_str, 10);
        // 将id和name，写回到user.txt中
        write_user();
    }
}

void write_score(int score) {
    FILE* fp = fopen("./record.txt", "a+");
    fputs(id_str, fp);
    char temp[5];
    itoa(score, temp, 10);
    fputs("\n", fp);
    fputs(temp, fp);
    fputs("\n", fp);
    fclose(fp);
}

double show_my_scores(char id_str[]) {
    // 遍历一遍record.txt
    FILE* fp = fopen("./record.txt", "rw");
    rewind(fp);
    int score = 0, num = 0;
    int time = 0;
    while (1) {
        char temp[N];
        if (fgets(temp, 1024, fp) == NULL)
            break;
        temp[strlen(temp) - 1] = '\0';
        if (strcmp(temp, id_str) == 0) {
            fgets(temp, 1024, fp);
            temp[strlen(temp) - 1] = '\0';
            int temp_score = str_to_int(temp);
            printf("The %dth score is %d\n", ++time, temp_score);
            score += str_to_int(temp);
            ++num;
        }
    }
    fclose(fp);
    return num ? (double) score / num : num;
}

void add_all_score(student *students) {
    // 遍历一遍record.txt
    FILE* fp = fopen("./record.txt", "rw");
    rewind(fp);
    int score, id;
    char score_str[N], id_str[N];
    while (1) {
        if (fgets(id_str, 1024, fp) == NULL)
            break;
        id_str[strlen(id_str) - 1] = '\0';
        fgets(score_str, 1024, fp);
        score_str[strlen(score_str) - 1] = '\0';
        id = str_to_int(id_str);
        score = str_to_int(score_str);
        students[id - 100001].score += score;
        ++students[id - 100001].num;
    }
    fclose(fp);
}

int cmp(const void *a_, const void *b_) {
    student *a = (student *) a_;
    student *b = (student *) b_;
    int avg_a = a->num ? (int) a->score / a->num * 100 : 0;
    int avg_b = b->num ? (int) b->score / b->num * 100 : 0;
    if (avg_a != avg_b) {
        return avg_a < avg_b ? 1 : -1;
    }
    return a->id > b->id ? 1 : -1;
}

void get_all_names(student *students, int total) {
    FILE* fp = fopen("./user.txt", "rw");
    rewind(fp);
    char temp[N];
    for (int i = 0; i < total; i++) {
        fgets(temp, 1024, fp);
        temp[strlen(temp) - 1] = '\0';
        strcpy(students[i].name, temp);
        fgets(temp, 1024, fp);
    }
    fclose(fp);
}

int run(int key) {
    if (key == 1) {
        srand(time(0));
        // 不断提问
        int score = 0;
        for (int i = 0; i < PROBlEM_NUM; i++) {
            if (ask_problem()) {
                printf("You are right!\n");
                ++score;
            } else {
                printf("Wrong answer!\n");
            }
        }
        // 写入用户的成绩
        printf("Your score is %d\n", score);
        write_score(score);
    } else if (key == 2) {
        printf("Average score = %.2lf\n", show_my_scores(id_str));
    } else if (key == 3) {
        int max_id = get_nextId() - 1;
        int total = max_id - 100000;
        student *students = (student*) malloc(sizeof(student) * total);
        memset(students, 0, sizeof(student) * total);
        for (int i = 0; i < total; i++) {
            students[i].id = i + 100001;
        }
        add_all_score(students);
        get_all_names(students, total);
        qsort(students, total, sizeof(student), cmp);
        int num = 0;
        int pre_score = -1, pre_num;
        for (int i = 0; i < total; i++) {
            printf("%d, %s, %.2lf, ", students[i].id, students[i].name,  students[i].num ? students[i].score / students[i].num : 0);
            if (students[i].score != pre_score) {
                printf("%d\n", i + 1);
                pre_num = i + 1;
            } else {
                printf("%d\n", pre_num);
            }
            pre_score = students[i].score;
        }
    } else if (key == 4){
        return 1;
    } else {
        printf("Wrong input\n");
    }
    return 0;
}

int main() {
    ask_name();
    while(1) {
        printf("=================================================\n");
        printf("If you want to have a try, please enter 1\n");
        printf("If you want to check your score, please enter 2\n");
        printf("If you want to view the grade sheet, please enter 3\n");
        printf("If you want to exit this game, please enter 4\n");
        int key;
        scanf("%d", &key);
        if (run(key)) {
            break;
        }
        printf("=================================================\n");
    }
}