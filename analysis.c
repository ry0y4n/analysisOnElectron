// countFixationとcountTransitionをまとめてresult.csvに書き込みを行うプログラム
#include<stdio.h>
#include<string.h>

// 視線データの最大数（静的に確保しないと面倒だから，これくらい確保してけばいいだろう的な値）
#define DATA_MAX 5500

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

// 境界値をcountFixationで呼び出し易くするために多重配列化
int borders[3][4] = {
    {FRONT_VIEW_LEFT, FRONT_VIEW_RIGHT, FRONT_VIEW_TOP, FRONT_VIEW_BOTTOM},
    {PLAN_VIEW_LEFT, PLAN_VIEW_RIGHT, PLAN_VIEW_TOP, PLAN_VIEW_BOTTOM},
    {RIGHT_SIDE_VIEW_LEFT, RIGHT_SIDE_VIEW_RIGHT, RIGHT_SIDE_VIEW_TOP, RIGHT_SIDE_VIEW_BOTTOM}
};

// 注視回数
int fixation_count = 0;

// start_time_list : 注視が始まった時間を格納する配列
// fixation_position_list : 注視がどのオブジェクトで起こったのかを判別するための配列
// (0:正面図, 1:平面図, 2右側面図)
int start_time_list[EXPERIMENT_TIME / FIXATION_TIME];
int fixation_time_list[EXPERIMENT_TIME / FIXATION_TIME];
int fixation_position_list[EXPERIMENT_TIME / FIXATION_TIME];

// 視線データを格納する構造体
typedef struct {
    double x;
    double y;
    double t;
} Csv;

// データの個人情報を格納する構造体
typedef struct {
    int grade;
    char class;
} Member;

// 視線の注視をカウントする関数
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

// クイックソート
void quick_sort (int array[], int left, int right) {
  int pivot;

  if (left < right) {
    pivot = partition(array, left, right);
    quick_sort(array, left, pivot-1);   // pivotを境に再帰的にクイックソート
    quick_sort(array, pivot+1, right);
  }
}

// メイン関数
int main() {

    // 諸々の変数を定義
    int i, j, k, csv_data_count, data_count, num_of_pos;
    FILE *fp;
    char file_name[20];
    Csv csv[DATA_MAX];
    Member member[DATA_MAX];

    // 視線の遷移回数を格納する変数
    int transition_count;
    int transition_count_of_into_front, transition_count_of_into_plan, transition_count_of_into_right_side;

    // 解析結果を書き込みcsvファイルとストリームの指定
    FILE *resfp;
    char result_file_name[] = "raw_result2.csv";
    FILE *memfp;
    char member_file_name[] = "members2.csv";

    // ファイルの読み込みに失敗した時用
    if ((resfp = fopen(result_file_name, "w")) == NULL) {
        fprintf(stderr, "result.csvファイルのオープンに失敗しました．\n");
        return -1;
    }
    if ((memfp = fopen(member_file_name, "r")) == NULL) {
        fprintf(stderr, "members2.csvファイルのオープンに失敗しました．\n");
        return -1;
    }

    i = 0;
    while (fscanf(memfp, "%d,%c", &member[i].grade, &member[i].class) != EOF) {
        i++;
    }
    csv_data_count = i;

    // for(i = 0; i < csv_data_count; i++) printf("%d, %c\n", member[i].grade, member[i].class);
    // printf("%d", csv_data_count);

    fclose(memfp);

    // ファイルの1行目にデータのラベルを書き込んでおく
    // (今後さらにresult.csvを読み込んで作業したい場合はラベルが邪魔になるかもしれない．その時は以下の行をコメントアウトすること)
    fprintf(resfp, "number,front,plan,right,transition,fixation,class\n");

    // 視線のcsvファイルの数だけforを回し解析する
    for (i = 1; i <= csv_data_count; i++) {

        // 読み込みファイル名の指定
        sprintf(file_name, "%db.csv", i);
        // csvファイルを読み込む．読み込めないとエラーを書き込み（エラーが起こったファイルを特定するコードを追加してもよし）
        if ((fp = fopen(file_name, "r")) == NULL) {
            fprintf(stderr, "%s\n", "error: cant't read file.");
            return -1;
        }

        // データ数を数える(data_count)
        j = 0;
        while (fscanf(fp, "%lf, %lf, %lf", &csv[j].x, &csv[j].y, &csv[j].t) != EOF) {
            j++;
        }
        data_count = j;

        // ファイルをクローズ
        fclose(fp);

        // オブジェクトを一定時間(FIXATION_TIME秒)，注視した回数をカウント(fixation_count)
        for (num_of_pos = 0; num_of_pos < 3; num_of_pos++) {
            countFixation(num_of_pos, csv, data_count);
        }

        // 視線がオブジェクト内に存在して，尚且つ，前の視線が違うオブジェクト上にあった場合にカウント(transition_count)
        transition_count_of_into_front = 0, transition_count_of_into_plan = 0, transition_count_of_into_right_side = 0;
        for (k = 1; k < data_count; k++) {

            if (((FRONT_VIEW_LEFT < csv[k].x) && (csv[k].x < FRONT_VIEW_RIGHT)) && ((FRONT_VIEW_TOP < csv[k].y) && (csv[k].y < FRONT_VIEW_BOTTOM))) 
                if (!(((FRONT_VIEW_LEFT < csv[k-1].x) && (csv[k-1].x < FRONT_VIEW_RIGHT)) && ((FRONT_VIEW_TOP < csv[k-1].y) && (csv[k-1].y < FRONT_VIEW_BOTTOM)))) 
                    transition_count_of_into_front++;
            
            if (((PLAN_VIEW_LEFT < csv[k].x) && (csv[k].x < PLAN_VIEW_RIGHT)) && ((PLAN_VIEW_TOP < csv[k].y) && (csv[k].y < PLAN_VIEW_BOTTOM))) 
                if (!(((PLAN_VIEW_LEFT < csv[k-1].x) && (csv[k-1].x < PLAN_VIEW_RIGHT)) && ((PLAN_VIEW_TOP < csv[k-1].y) && (csv[k-1].y < PLAN_VIEW_BOTTOM))))
                    transition_count_of_into_plan++;
            
            if (((RIGHT_SIDE_VIEW_LEFT < csv[k].x) && (csv[k].x < RIGHT_SIDE_VIEW_RIGHT)) && ((RIGHT_SIDE_VIEW_TOP < csv[k].y) && (csv[k].y < RIGHT_SIDE_VIEW_BOTTOM)))
                if ((((RIGHT_SIDE_VIEW_LEFT < csv[k-1].x) && (csv[k-1].x < RIGHT_SIDE_VIEW_RIGHT)) && ((RIGHT_SIDE_VIEW_TOP < csv[k-1].y) && (csv[k-1].y < RIGHT_SIDE_VIEW_BOTTOM))))
                    transition_count_of_into_right_side++;
        }

        transition_count = transition_count_of_into_front + transition_count_of_into_plan + transition_count_of_into_right_side;

        // csvファイルに解析結果の書き込み
        fprintf(resfp, "%d,%d,%d,%d,%d,%d,%c", i, transition_count_of_into_front, transition_count_of_into_plan, transition_count_of_into_right_side, transition_count, fixation_count, member[i-1].class);

        // start_time_listは各オブジェクトを順番に取ったので時系列順になっていない．
        // なのでソートする．ちなクイックソートはオーダーが低くて優秀なので使った．バブルでもマージでもなんでも良い
        quick_sort(start_time_list, 0, fixation_count-1);

        // ソートした2つの配列をcsvファイルに書き込んで改行する
        //for (k = 0; k < fixation_count; k++) fprintf(resfp, ",%d,%d,%d", start_time_list[k], fixation_time_list[k], fixation_position_list[k]);
        fprintf(resfp, "\n");
        
        // 注視回数のリセット
        fixation_count = 0;
        
    }

    fclose(resfp);

    return 0;
}