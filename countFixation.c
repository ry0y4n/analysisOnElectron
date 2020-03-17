#include<stdio.h>
#include<string.h>

// 視線データの最大数（静的に確保しないと面倒だから，これくらい確保してけばいいだろう的な値）
#define DATA_MAX 6000

// 実験時間
#define EXPERIMENT_TIME 60000

// 注視時間
#define FIXATION_TIME 3000

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

// 各図のボーダーを配列に格納
int borders[3][4] = {
    {FRONT_VIEW_LEFT, FRONT_VIEW_RIGHT, FRONT_VIEW_TOP, FRONT_VIEW_BOTTOM},
    {PLAN_VIEW_LEFT, PLAN_VIEW_RIGHT, PLAN_VIEW_TOP, PLAN_VIEW_BOTTOM},
    {RIGHT_SIDE_VIEW_LEFT, RIGHT_SIDE_VIEW_RIGHT, RIGHT_SIDE_VIEW_TOP, RIGHT_SIDE_VIEW_BOTTOM}
};

// 注視回数の初期化
int fixation_count = 0;

// 上から注視の開始時間・注視継続時間・注視対象オブジェクト
int start_time_list[EXPERIMENT_TIME / FIXATION_TIME];
int fixation_time_list[EXPERIMENT_TIME / FIXATION_TIME];
int fixation_position_list[EXPERIMENT_TIME / FIXATION_TIME];

// 視線データを格納する構造体
typedef struct {
    double x;
    double y;
    int t;
} Csv;

// 注視解析関数
// 1回で1つの図についての注視解析ができる．"num_of_pos"で対象オブジェクトを指定できる．
// 0: 正面図，1: 平面図，2: 右側面図
void countFixation(int num_of_pos, Csv *csv, int data_count) {
    int i;
    double start_time = 0;
    int time_flag = 0;
    int count_flag = 0;
    for (i = 0; i < data_count; i++) {
        if (((borders[num_of_pos][0] < csv[i].x) && (csv[i].x < borders[num_of_pos][1])) && ((borders[num_of_pos][2] < csv[i].y) && (csv[i].y < borders[num_of_pos][3]))) {
            if (!time_flag) {
                start_time = csv[i].t;
                time_flag = 1;
                count_flag = 1;
            }
            
            if (((csv[i].t - start_time) >= FIXATION_TIME) && count_flag) {
                count_flag = 0;
                start_time_list[fixation_count] = ((start_time - csv[0].t) / 1000);
                fixation_position_list[fixation_count++] = num_of_pos;
            }
        }
        else {
            if (time_flag && ((csv[i-1].t - start_time) >= FIXATION_TIME)) fixation_time_list[fixation_count-1] = (csv[i-1].t - start_time);
            time_flag = 0;
            count_flag = 0;
            
        }
    }
    if (time_flag && ((csv[i-1].t - start_time) >= FIXATION_TIME)) fixation_time_list[fixation_count-1] = (csv[i-1].t - start_time);

}

// 値を入れ替える関数
void swap (int a[], int x, int y) {
  int temp;

  temp = a[x];
  a[x] = a[y];
  a[y] = temp;

  temp = fixation_position_list[x];
  fixation_position_list[x] = fixation_position_list[y];
  fixation_position_list[y] = temp;

  temp = fixation_time_list[x];
  fixation_time_list[x] = fixation_time_list[y];
  fixation_time_list[y] = temp;
}

/***
* pivotを決め、
* 全データをpivotを境目に振り分け、
* pivotの添え字を返す
***/
int partition (int array[], int left, int right) {
  int i, j, pivot;
  i = left;
  j = right + 1;
  pivot = left;   // 先頭要素をpivotとする

  do {
    do { i++; } while (array[i] < array[pivot]);
    do { j--; } while (array[pivot] < array[j]);
    // pivotより小さいものを左へ、大きいものを右へ
    if (i < j) { swap(array, i, j); }
  } while (i < j);

  swap(array, pivot, j);   //pivotを更新

  return j;
}

// クイックソート(O(nlogn)なので速い．アルゴリズムは別に追わなくてもいい)
void quick_sort (int array[], int left, int right) {
  int pivot;

  if (left < right) {
    pivot = partition(array, left, right);
    quick_sort(array, left, pivot-1);   // pivotを境に再帰的にクイックソート
    quick_sort(array, pivot+1, right);
  }
}

// メイン関数
int main(int argc, char *argv[]) {

    // 諸々の変数を定義
    int i, data_count, num_of_pos;
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
    while (fscanf(fp, "%lf, %lf, %d", &csv[i].x, &csv[i].y, &csv[i].t) != EOF) {
        i++;
    }
    data_count = i;

    // ファイルをクローズする
    fclose(fp);

    // オブジェクトを一定時間（3秒），注視した回数をカウント
    for (num_of_pos = 0; num_of_pos < 3; num_of_pos++) {
        countFixation(num_of_pos, csv, data_count);
    }
    // 各図ごとに注視解析したので配列内を時系列順にソートする
    quick_sort(start_time_list, 0, fixation_count-1);

    FILE *resfp;
    char result_file_name[50];
    strcpy(result_file_name, argv[2]);
    

    if ((resfp = fopen(result_file_name, "w")) == NULL) {
        fprintf(stderr, "countFixation.csvファイルのオープンに失敗しました．\n");
        return -1;
    }

    fprintf(resfp, "start_time,fixation_length,fixation_object\n");
    for (i = 0; i < fixation_count; i++) fprintf(resfp, "%d,%d,%d\n", start_time_list[i], fixation_time_list[i], fixation_position_list[i]);

    fclose(resfp);

    /*

    // データ出力
    printf("the number of data: %d\n", data_count);
    printf("the number of fixation %d\n", fixation_count);
    
    

    for (i = 0; i < fixation_count; i++) printf("%d, %d, %d\n", start_time_list[i], fixation_time_list[i], fixation_position_list[i]);
    printf("\n%lu\n", sizeof(start_time_list) / sizeof(int));
    printf("\n%lu\n", sizeof(fixation_time_list) / sizeof(int));
    printf("\n%lu\n", sizeof(fixation_position_list) / sizeof(int));

    */


    return 1;
}


    /*
    // FRONT_VIEW用の注視回数カウント(3000ms)
    time_flag = 0;
    count_flag = 0;
    for (i = 0; i < data_count; i++) {
        if (((FRONT_VIEW_LEFT < csv[i].x) && (csv[i].x < FRONT_VIEW_RIGHT)) && ((FRONT_VIEW_TOP < csv[i].y) && (csv[i].y < FRONT_VIEW_BOTTOM))) {
            if (!time_flag) {
                start_time = csv[i].t;
                time_flag = 1;
                count_flag = 1;
            }
            
            if (((csv[i].t - start_time) >= 3000) && count_flag) {
                fixation_count++;
                count_flag = 0;
            }
        }
        else {
            time_flag = 0;
        }
    }
    // PLAN_VIEW用の注視回数カウント(3000ms)
    time_flag = 0;
    count_flag = 0;
    for (i = 0; i < data_count; i++) {
        if (((PLAN_VIEW_LEFT < csv[i].x) && (csv[i].x < PLAN_VIEW_RIGHT)) && ((PLAN_VIEW_TOP < csv[i].y) && (csv[i].y < PLAN_VIEW_BOTTOM))) {
            if (!time_flag) {
                start_time = csv[i].t;
                time_flag = 1;
                count_flag = 1;
            }
            
            if (((csv[i].t - start_time) >= 3000) && count_flag) {
                fixation_count++;
                count_flag = 0;
            }
        }
        else {
            time_flag = 0;
        }
    }
    // RIGHT_SIDE_VIEW用の注視回数カウント(3000ms)
    time_flag = 0;
    count_flag = 0;
    for (i = 0; i < data_count; i++) {
        if (((RIGHT_SIDE_VIEW_LEFT < csv[i].x) && (csv[i].x < RIGHT_SIDE_VIEW_RIGHT)) && ((RIGHT_SIDE_VIEW_TOP < csv[i].y) && (csv[i].y < RIGHT_SIDE_VIEW_BOTTOM))) {
            if (!time_flag) {
                start_time = csv[i].t;
                time_flag = 1;
                count_flag = 1;
            }
            
            if (((csv[i].t - start_time) >= 3000) && count_flag) {
                fixation_count++;
                count_flag = 0;
            }
        }
        else {
            time_flag = 0;
        }
    }
    */