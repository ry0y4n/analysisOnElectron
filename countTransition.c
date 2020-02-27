#include<stdio.h>
#include<string.h>

// 視線データの最大数（静的に確保しないと面倒だから，これくらい確保してけばいいだろう的な値）
#define DATA_MAX 5500

// 正面図のサイズ
#define FRONT_VIEW_LEFT 288
#define FRONT_VIEW_RIGHT 811
#define FRONT_VIEW_TOP 597
#define FRONT_VIEW_BOTTOM 1051

// 平面図のサイズ
#define PLAN_VIEW_LEFT 229
#define PLAN_VIEW_RIGHT 853
#define PLAN_VIEW_TOP 0
#define PLAN_VIEW_BOTTOM 563

// 右側面図のサイズ
#define RIGHT_SIDE_VIEW_LEFT 853
#define RIGHT_SIDE_VIEW_RIGHT 1768
#define RIGHT_SIDE_VIEW_TOP 597
#define RIGHT_SIDE_VIEW_BOTTOM 952

// 視線データを格納する構造体
typedef struct csv_template {
    double x;
    double y;
    double t;
} Csv;


// メイン関数
int main(int argc, char *argv[]) {

    // 諸々の変数を定義
    int i, data_count, transition_count;
    FILE *fp;
    char file_name[50];
    strcpy(file_name, argv[1]);
    Csv csv[DATA_MAX];

    // csvファイルを読み込む．読み込めないとエラーを書き込み（エラーが起こったファイルを特定するため）
    if ((fp = fopen(file_name, "r")) == NULL) {
        fprintf(stderr, "%s\n", "error: cant't read file.");
        return -1;
    }

    // データ数を数える
    i = 0;
    while (fscanf(fp, "%lf, %lf, %lf", &csv[i].x, &csv[i].y, &csv[i].t) != EOF) {
        i++;
    }
    data_count = i;

    // ファイルをクローズする
    fclose(fp);

    int transition_count_of_into_front = 0, transition_count_of_into_plan = 0, transition_count_of_into_right_side = 0;

    // 視線がオブジェクト内に存在して，尚且つ，前の視線が違うオブジェクト上にあった場合にカウント
    for (i = 1; i < data_count; i++) {

        if (((FRONT_VIEW_LEFT < csv[i].x) && (csv[i].x < FRONT_VIEW_RIGHT)) && ((FRONT_VIEW_TOP < csv[i].y) && (csv[i].y < FRONT_VIEW_BOTTOM))) 
            if (!(((FRONT_VIEW_LEFT < csv[i-1].x) && (csv[i-1].x < FRONT_VIEW_RIGHT)) && ((FRONT_VIEW_TOP < csv[i-1].y) && (csv[i-1].y < FRONT_VIEW_BOTTOM)))) 
                transition_count_of_into_front++;
        
        if (((PLAN_VIEW_LEFT < csv[i].x) && (csv[i].x < PLAN_VIEW_RIGHT)) && ((PLAN_VIEW_TOP < csv[i].y) && (csv[i].y < PLAN_VIEW_BOTTOM))) 
            if (!(((PLAN_VIEW_LEFT < csv[i-1].x) && (csv[i-1].x < PLAN_VIEW_RIGHT)) && ((PLAN_VIEW_TOP < csv[i-1].y) && (csv[i-1].y < PLAN_VIEW_BOTTOM))))
                transition_count_of_into_plan++;
        
        if (((RIGHT_SIDE_VIEW_LEFT < csv[i].x) && (csv[i].x < RIGHT_SIDE_VIEW_RIGHT)) && ((RIGHT_SIDE_VIEW_TOP < csv[i].y) && (csv[i].y < RIGHT_SIDE_VIEW_BOTTOM)))
            if ((((RIGHT_SIDE_VIEW_LEFT < csv[i-1].x) && (csv[i-1].x < RIGHT_SIDE_VIEW_RIGHT)) && ((RIGHT_SIDE_VIEW_TOP < csv[i-1].y) && (csv[i-1].y < RIGHT_SIDE_VIEW_BOTTOM))))
                transition_count_of_into_right_side++;
    }

    transition_count = transition_count_of_into_front + transition_count_of_into_plan + transition_count_of_into_right_side;

    FILE *resfp;
    char res_file_name[50];
    strcpy(res_file_name, argv[2]);
    resfp = fopen(res_file_name, "w");
    fprintf(resfp, "transition_count_of_into_front,transition_count_of_into_plan,transition_count_of_into_right_side,transition_count\n");
    fprintf(resfp, "%d,%d,%d,%d\n",transition_count_of_into_front,transition_count_of_into_plan,transition_count_of_into_right_side,transition_count);

    fclose(resfp);

    // データ出力
    /*
    for (i = 0; i < data_count; i++) {
        printf("%.2f, %.2f, %.2f\n", csv[i].x, csv[i].y, csv[i].t);
    }
    */
    printf("the number of data: %d\n", data_count);
    printf("the number of transition into frnot: %d\n", transition_count_of_into_front);
    printf("the number of transition into plan: %d\n", transition_count_of_into_plan);
    printf("the number of transition into right side: %d\n", transition_count_of_into_right_side);
    printf("the number of transition: %d\n", transition_count);

    return 1;
}