# SoftwareExperiment2022

<p  align="center"><img width="521" alt="image" src="https://github.com/hirokazu1108/SoftwareExperiment2022/assets/87222170/c3594f67-b97a-4885-b755-6ff7fe8b13ec"></p>

- ### 作品名
   Sky Wars

- ### 概要
   学部2年生後期の講義「ソフトウェア設計及び実験」における共同開発により作成したネットワーク対戦型3Dシューティングゲームです.総合優秀賞と技術賞を頂きました.製作期間は2022年10月末から2023年1月末の約3か月で,4人のメンバで制作を行いました.
   
   この制作では,C++のクラスなどを未収得のものもいる上,授業では初の大規模共同開発,3D開発,ネットワークプログラミングを3か月間で行う必要があったため,完成を優先させたためオブジェクト指向や命名規則などがあまり意識できていないコードが多くありますが,ご了承ください.

   この制作では以下を使用しています.
   - C/C++
   - Shell Script
   - [OpenGL(https://www.opengl.org/)](https://www.opengl.org/)
   - [OpenAL(https://www.openal.org/)](https://www.openal.org/)
   - [Blender(https://www.blender.org/)](https://www.blender.org/)
   ...etc

- ### プレイ動画・マニュアル
   [プレイ動画・マニュアル（Google Driveに移動）](https://drive.google.com/drive/folders/1wf6xUjTrAFtylgJ2kHkbrIom9IfR53eB?usp=drive_link)
   
   [作品紹介動画（外部サイトへ）](http://netadm.iss.tokushima-u.ac.jp/soft/2022/2nd/game.html)

- ### 主な担当箇所
   - タイトル・カスタマイズ・リザルト画面の設計

   - ゲームUIの設計

   - ネットワーク構築

   - 衝突判定

   - スキルやスペシャルの実装


- ### ゲームコンセプト
   3D ワールドの大空を自由に飛び回り，カスタマイズしたオリジナルの戦闘機でハイスコアを目指す.


- ### プレイ人数
   2～4人を想定（1人でも対戦はできませんが遊べます）


- ### プレイ時間
   約3分


- ### 使用機器
   PC（学内のLinux環境を想定）,ジョイパッド


- ### ゲームルール
   #### 1.カスタマイズ
       - 名前の設定,スキルの選択（3種類）
       - スペシャルの選択（8種類）
       - ステータスの設定（5項目）
   #### 2.メインゲーム
       - 弾を発射して敵の機体を倒す.弾を当てるとスコアが入る
      
       - 倒されると減点される
       
       - また,周囲を移動するエネミーを倒すことでもスコアを稼げる
       
       - 時間経過で溜まるMPを消費してスペシャルを発動
       
       - 3分経過後にリザルト画面に移動
   #### 3.リザルト
       - リザルトを確認しよう（倒した数,スコアなどいろいろなデータを確認できる）


- ### 実行方法
   #### 1.zipファイルをダウンロード後,展開する
   
   #### 2.Linuxターミナルでmainディレクトリに移動し./mainを実行
   
   #### 3.サーバを立てる
   
       - 3.1 SERVERボタンにカーソルを合わせクリック
        
       - 3.2 クライアント人数を入力
        
       - 3.3 パスコードを入力
     
   #### 4.クライアントで参加する
   
       - 4.1 サーバが立ち上がっていることを確認
        
       - 4.2 カスタマイズを行う
        
       - 4.3 デバイス番号を入力（学内PCを想定して作成したため,localを選択して一つのPCで複数ウィンドウを立ち上げてください）
        
       - 4.4 サーバで決めたパスコードを入力
        
   #### 5.マッチングが完了すると,暗転しゲームが開始する
   
      ※オンラインで行う方法（カスタマイズを上記の方法で行いデータが保存した後で下記の手順に従ってください）
   
      - サーバはserverディレクトリに移動し,  ./SB_server (人数) (ポート番号) を実行
      
        (実行例) ./SB_server 2 88888
        
      - クライアントはclientディレクトリに移動し,  ./SB_client (IPアドレス) (ポート番号) を実行
      
        (実行例) ./SB_client xxx.xxx.xx.xxx 88888
        
      - なお,IPアドレスは ip -4 a などで表示できます.

- ### 共同制作者
   - [RyuseiSakata - GitHub](https://github.com/RyuseiSakata)
   - [Yosuke-0921 - GitHub](https://github.com/Yosuke-0921)
   - [OgitaMasaki - GitHub](https://github.com/OgitaMasaki)
   

