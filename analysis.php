<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Document</title>
</head>
<body>

<?php
if (is_uploaded_file($_FILES["csvfile"]["tmp_name"])) {
    $file_tmp_name = $_FILES["csvfile"]["tmp_name"];
    $file_name = $_FILES["csvfile"]["name"];

    print $file_tmp_name.'<br>';
    print $file_name.'<br>';
  
    //拡張子を判定
    if (pathinfo($file_name, PATHINFO_EXTENSION) != 'csv') {
      $err_msg = 'CSVファイルのみ対応しています。';
    } else {
      //ファイルをcsvディレクトリに移動
      if (move_uploaded_file($file_tmp_name, "./csv/" . $file_name)) {
        //後で削除できるように権限を644に
        chmod("./csv/" . $file_name, 0644);
        $msg = $file_name . "をアップロードしました。";
        $file = './csv/'.$file_name;
        $fp   = fopen($file, "r");
  
        //配列に変換する
        while (($data = fgetcsv($fp, 0, ",")) !== FALSE) {
          $asins[] = $data;
        }
        foreach($asins as $row)
        {
            var_dump($row);
            print $row[0];
            print "<br>";
        }
        fclose($fp);
        //ファイルの削除
        //unlink('./csv/'.$file_name);
      } else {
        $err_msg = "ファイルをアップロードできません。";
      }
    }
  } else {
    $err_msg = "ファイルが選択されていません。";
  }
?>
    
</body>
</html>