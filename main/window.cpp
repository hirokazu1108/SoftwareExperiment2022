#include "header.h"

/* 画像パス */
static const char *imgFile[IMG_NUM] = { "name.png", "skill.png","special.png","status.png", "explain_skill.png","nowSelectButton.png", "changeButton.png","backButton.png",  "skill_attack.png", "skill_hp.png","skill_speed.png", "pin.png", "back.png","nameChange.png","skillChange.png","selectHikouki.png","title_sky.png","title_sky2.png","cloud.png", "castle.png","masao.png","masao_face.png"};
static const char *textStr[TEXT_NUM] = {"Space Battle","SERVER","CLIENT","CUSTOMIZE","input client num.","input passcode.","del","Enter","self","input device num.","clpc","nowLoading...","Result","Exit", "toTitle","0","1","2","3","4","5","6","7","8","9","a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z","A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"," ", "-"};
/* フォントパス */
static char gFontFile[] = "../fonts/Yomogi-Regular.ttf";

/* ボタンの位置 */
std::vector<SDL_Rect> buttonPos[SCENE_NUM];

float scroll_back = -50.0; //背景のスクロール

float masao_loop = 0;
float masao_rotate = 0;
float masao_x = 500;
float masao_y = 500;

/* パラメータの場所 */
static int parm_x[11] = {380,425,475,520,567,618,664,710,756,802,850};//ピンのx座標
static int parmText_y[PARAMATER_NUM] ={1025,1160,1297,1435,1565};//数値のy座標

static const SDL_Color gWhite = { 255, 255, 255, 255 };
static const SDL_Color gBlue  = { 0, 0, 255, 255 };

ImgInfo uiImg[IMG_NUM];
ImgInfo textImg[TEXT_NUM];

/* 関数 */
static int MakeMessage(void);
static int rectangleColorRect(SDL_Renderer *render, SDL_Rect *rect, Uint32 color);
static int boxColorRect(SDL_Renderer *render, SDL_Rect *rect, Uint32 color);
static textName retTextNameFromChar(char ch);
static void DrawBackGround(void);

// メインウインドウの表示，設定
int InitWindow(void)
{
    /* SDL_image初期化 */
    if (IMG_INIT_PNG != IMG_Init(IMG_INIT_PNG)) {
        return PrintError("failed to initialize SDL_image");
    }

    /** メインのウインドウ(表示画面)とレンダラーの作成 **/
    game.window = SDL_CreateWindow("main", 0, 0, WD_Width, WD_Height, 0);
    if (game.window == NULL)
        return PrintError(SDL_GetError());

    game.render = SDL_CreateRenderer(game.window, -1, 0);
    if (game.render == NULL)
        return PrintError(SDL_GetError());

    /** キャラ画像の読み込み **/
    /* 画像は，
     *  横方向にアニメーションパターン
     *  縦方向に向きパターン（時計回り，最初が↑）
     *  があると想定
     */
    for (int i = 0; i < IMG_NUM; i++) {
        char pass[128];
        sprintf(pass,"../images/%s",imgFile[i]);
        printf("%s\n",pass);
        SDL_Surface *s = IMG_Load(pass);
        if (NULL == s) {
            return PrintError("failed to open ui image.");
        }
        uiImg[i].texture = SDL_CreateTextureFromSurface(game.render, s);
        SDL_FreeSurface(s);
        if (uiImg[i].texture == NULL) {
            return PrintError(SDL_GetError());
        }
        if (0 > SDL_QueryTexture(uiImg[i].texture, NULL, NULL, &uiImg[i].w, &uiImg[i].h)) {
            PrintError(SDL_GetError());
        }
    }

    /** メッセージ作成 **/
    if (MakeMessage())
        return -1;

    /* ボタンの座標の格納 */
    buttonPos[SCENE_Title].push_back({200,300,600,100}); //server
    buttonPos[SCENE_Title].push_back({200,500,600,100}); //client
    buttonPos[SCENE_Title].push_back({200,700,600,100}); //customize
    buttonPos[SCENE_SERVER_0].push_back({200,400,100,100}); //2
    buttonPos[SCENE_SERVER_0].push_back({350,400,100,100}); //3
    buttonPos[SCENE_SERVER_0].push_back({500,400,100,100}); //4
    buttonPos[SCENE_SERVER_1].push_back({120,400,80,80});  //0
    buttonPos[SCENE_SERVER_1].push_back({220,400,80,80}); //1
    buttonPos[SCENE_SERVER_1].push_back({320,400,80,80}); //2
    buttonPos[SCENE_SERVER_1].push_back({420,400,80,80}); //3
    buttonPos[SCENE_SERVER_1].push_back({520,400,80,80}); //4
    buttonPos[SCENE_SERVER_1].push_back({120,520,80,80}); //5
    buttonPos[SCENE_SERVER_1].push_back({220,520,80,80}); //6
    buttonPos[SCENE_SERVER_1].push_back({320,520,80,80}); //7
    buttonPos[SCENE_SERVER_1].push_back({420,520,80,80}); //8
    buttonPos[SCENE_SERVER_1].push_back({520,520,80,80}); //9
    buttonPos[SCENE_SERVER_1].push_back({620,460,80,80}); //?
    buttonPos[SCENE_SERVER_1].push_back({200,600,400,100}); //確定
    buttonPos[SCENE_CLIENT_0].push_back({120,400,80,80});  //0
    buttonPos[SCENE_CLIENT_0].push_back({220,400,80,80}); //1
    buttonPos[SCENE_CLIENT_0].push_back({320,400,80,80}); //2
    buttonPos[SCENE_CLIENT_0].push_back({420,400,80,80}); //3
    buttonPos[SCENE_CLIENT_0].push_back({520,400,80,80}); //4
    buttonPos[SCENE_CLIENT_0].push_back({120,520,80,80}); //5
    buttonPos[SCENE_CLIENT_0].push_back({220,520,80,80}); //6
    buttonPos[SCENE_CLIENT_0].push_back({320,520,80,80}); //7
    buttonPos[SCENE_CLIENT_0].push_back({420,520,80,80}); //8
    buttonPos[SCENE_CLIENT_0].push_back({520,520,80,80}); //9
    buttonPos[SCENE_CLIENT_0].push_back({620,460,80,80}); //?
    buttonPos[SCENE_CLIENT_0].push_back({80,650,100,100}); //localHost
    buttonPos[SCENE_CLIENT_0].push_back({200,650,400,100}); //確定
    buttonPos[SCENE_CLIENT_1].push_back({120,400,80,80});  //0
    buttonPos[SCENE_CLIENT_1].push_back({220,400,80,80}); //1
    buttonPos[SCENE_CLIENT_1].push_back({320,400,80,80}); //2
    buttonPos[SCENE_CLIENT_1].push_back({420,400,80,80}); //3
    buttonPos[SCENE_CLIENT_1].push_back({520,400,80,80}); //4
    buttonPos[SCENE_CLIENT_1].push_back({120,520,80,80}); //5
    buttonPos[SCENE_CLIENT_1].push_back({220,520,80,80}); //6
    buttonPos[SCENE_CLIENT_1].push_back({320,520,80,80}); //7
    buttonPos[SCENE_CLIENT_1].push_back({420,520,80,80}); //8
    buttonPos[SCENE_CLIENT_1].push_back({520,520,80,80}); //9
    buttonPos[SCENE_CLIENT_1].push_back({620,460,80,80}); //?
    buttonPos[SCENE_CLIENT_1].push_back({200,600,400,100}); //確定
    buttonPos[SCENE_CUSTOMIZE].push_back({36,57-scrollValue,236,120}); //戻るボタン
    buttonPos[SCENE_CUSTOMIZE].push_back({930,130-scrollValue,0,0}); //名前変更(丸ボタン)
    buttonPos[SCENE_CUSTOMIZE].push_back({900,370-scrollValue,0,0}); //スキル変更(丸ボタン)
    buttonPos[SCENE_CUSTOMIZE].push_back({900,700-scrollValue,0,0}); //スペシャル変更(丸ボタン)
    buttonPos[SCENE_Result].push_back({200,820,200,80}); //exit
    buttonPos[SCENE_Result].push_back({500,820,200,80}); //totitle



    if(game.scene == SCENE_Title)
        RenderTitleWindow();
    else if(game.scene == SCENE_CLIENT_WAIT)
        RenderClientWaitWindow();

    /* image利用終了(テクスチャに転送後はゲーム中に使わないので) */
    IMG_Quit();

    return 0;
}

/* ウインドウの終了処理 */
void DestroyWindow(void)
{
    /* テクスチャなど */
    for (int i = 0; i < IMG_NUM; i++)
        SDL_DestroyTexture(uiImg[i].texture);
    for (int i = 0; i < TEXT_NUM; i++)
        SDL_DestroyTexture(textImg[i].texture);

    SDL_DestroyRenderer(game.render);
    SDL_DestroyWindow(game.window);
}

/* メッセージ作成
 *
 * 返値
 *   正常終了: 0
 *   エラー  : 負数
 */
static int MakeMessage(void)
{
    int ret = 0;
    /* フォントからメッセージテクスチャ作成 */
    /* 初期化 */
    if (TTF_Init() < 0) {
        return PrintError(TTF_GetError());
    }
    /* フォントを開く */
    TTF_Font *ttf = TTF_OpenFont(gFontFile, 90);
    if (NULL == ttf) {
        ret = PrintError(TTF_GetError());
    }
    /* メッセージ作成 */
    SDL_Color cols[TEXT_NUM] = { gBlue };
    for (int i = 0; i < TEXT_NUM && ttf; i++) {
        SDL_Surface *sf;
        /* フォントと文字列，色からサーフェイス作成 */
        sf = TTF_RenderUTF8_Blended(ttf, textStr[i], cols[i]);
        if (NULL == sf) {
            ret = PrintError(TTF_GetError());
        } else {
            /* テクスチャへ */
            textImg[i].texture = SDL_CreateTextureFromSurface(game.render, sf);
            if (NULL == textImg[i].texture) {
                ret = PrintError(SDL_GetError());
            }
            if (0 > SDL_QueryTexture(textImg[i].texture, NULL, NULL, &textImg[i].w, &textImg[i].h)) {
                PrintError(SDL_GetError());
            }
            /* サーフェイス解放(テクスチャに転送後はゲーム中に使わないので) */
            SDL_FreeSurface(sf);
        }
    }

    /* フォントを閉じる */
    TTF_CloseFont(ttf);
    /* フォント利用終了(テクスチャに転送後はゲーム中に使わないので) */
    TTF_Quit();

    return ret;
}

/* ウインドウ描画
 *  メインウインドウに背景，キャラ，メッセージなどを転送する
 */
void RenderTitleWindow(void)
{
    //背景
    uiImg[uname_title_sky].drawTexture(-50,0,uiImg[uname_title_sky].w/1.07,uiImg[uname_title_sky].h/1.07);
    
    // 文字の表示　「Game Title」
    textImg[tname_title].drawTexture(300,80);

    //ボタンの表示　「サーバー」
    rectangleColorRect(game.render,&buttonPos[SCENE_Title][0],0xff000000);
    textImg[tname_server].drawTexture(350,280);
    
    //ボタンの表示　「クライアント」
    rectangleColorRect(game.render,&buttonPos[SCENE_Title][1],0xff000000);
    textImg[tname_client].drawTexture(350,480);

    //ボタンの表示　「カスタマイズ」
    rectangleColorRect(game.render,&buttonPos[SCENE_Title][2],0xff000000);
    textImg[tname_customize].drawTexture(300,680);

    //選択状況の表示
    boxColorRect(game.render,&buttonPos[SCENE_Title][game.selectButton],0x77777777);

    // masaloop
    uiImg[uname_masao].drawTexture(600+400*sin(masao_loop),700,uiImg[uname_masao].w/2,uiImg[uname_masao].h/2);
    uiImg[uname_masao_face].drawRotateTexture(masao_x,masao_y,uiImg[uname_masao_face].w,uiImg[uname_masao_face].h,masao_rotate);
    masao_rotate +=8;
    masao_x += (random()%2 == 1) ? 5: -5;
    masao_y += (random()%2 == 1) ? 5: -5;
    if(masao_x >= 1000)
    {
        masao_x -= 5;
    }
    else if(masao_x <=0){
        masao_x += 5;
    }
    else if(masao_y >= 1000){
        masao_y -= 5;
    }
    else if(masao_y <= -200){
        masao_y += 5;
    }

    SDL_RenderPresent(game.render);
}

/* サーバーの人数入力の画面 */
void RenderServerWindow_0(void){
    //背景
    DrawBackGround();
    
    // 文字の表示　「クライアント人数」
    textImg[tname_inputNum].drawTexture(300,100);

    //ボタンの表示　「2」
    rectangleColorRect(game.render,&buttonPos[SCENE_SERVER_0][0],0xff000000);
    textImg[tname_2].drawTexture(buttonPos[SCENE_SERVER_0][0].x +10,buttonPos[SCENE_SERVER_0][0].y-10);
    
    //ボタンの表示　「3」
    rectangleColorRect(game.render,&buttonPos[SCENE_SERVER_0][1],0xff000000);
    textImg[tname_3].drawTexture(buttonPos[SCENE_SERVER_0][1].x+10,buttonPos[SCENE_SERVER_0][1].y-10);

    //ボタンの表示　「4」
    rectangleColorRect(game.render,&buttonPos[SCENE_SERVER_0][2],0xff000000);
    textImg[tname_4].drawTexture(buttonPos[SCENE_SERVER_0][2].x+10,buttonPos[SCENE_SERVER_0][2].y-10);

    //選択状況の表示
    boxColorRect(game.render,&buttonPos[SCENE_SERVER_0][game.selectButton],0x77777777);

    SDL_RenderPresent(game.render);
}

/* パスコード入力の画面 */
void RenderPasscodeWindow(){
    //背景
    DrawBackGround();
    
    // 文字の表示　「パスコード」
    textImg[tname_inputPasscode].drawTexture(300,100);

    //入力したポート番号の表示
    for(int i=0; game.port[i] != '\0'; i++)
    {
        textImg[tname_0+game.port[i]-'0'].drawTexture(300+i*80,180);
    }

    //ボタンの表示　「0」~「9」
    for(int i=0; i<=9; i++){
        rectangleColorRect(game.render,&buttonPos[game.scene][i],0xff000000);
        textImg[tname_0+i].drawTexture(buttonPos[game.scene][i].x +10,buttonPos[game.scene][i].y-20);
    }

    //ボタンの表示　「X」
    rectangleColorRect(game.render,&buttonPos[game.scene][10],0xff000000);
    textImg[tname_del].drawTexture(buttonPos[game.scene][10].x +10,buttonPos[game.scene][10].y-20);
    
    /* ポート番号が5桁入力されたら */
    if(strlen(game.port)>=4){
        //ボタンの表示　「確定」
        rectangleColorRect(game.render,&buttonPos[game.scene][11],0xff000000);
        textImg[tname_enter].drawTexture(buttonPos[game.scene][11].x +10,buttonPos[game.scene][11].y-20);
    }

    //選択状況の表示
    boxColorRect(game.render,&buttonPos[game.scene][game.selectButton],0x77777777);

    SDL_RenderPresent(game.render);
}

/* デバイス番号入力の画面 */
void RenderDeviceNumWindow(void){
    //背景
    DrawBackGround();
    
    // 文字の表示　「デバイス名を入力してください」
    textImg[tname_inputDevice].drawTexture(300,100);

    // 文字の表示　「clpc」
    textImg[tname_clpc].drawTexture(300,250);

    //入力したデバイス番号の表示
    for(int i=0; game.deviceNum[i] != '\0'; i++)
    {
        textImg[tname_0+game.deviceNum[i]-'0'].drawTexture(500+i*80,250);
    }

    //ボタンの表示　「0」~「9」
    for(int i=0; i<=9; i++){
        rectangleColorRect(game.render,&buttonPos[SCENE_CLIENT_0][i],0xff000000);
        textImg[tname_0+i].drawTexture(buttonPos[SCENE_CLIENT_0][i].x +10,buttonPos[SCENE_CLIENT_0][i].y-20);
    }

    //ボタンの表示　「X」
    rectangleColorRect(game.render,&buttonPos[SCENE_CLIENT_0][10],0xff000000);
    textImg[tname_del].drawTexture(buttonPos[SCENE_CLIENT_0][10].x +10,buttonPos[SCENE_CLIENT_0][10].y-20);
    
    //localHostボタンの表示
    rectangleColorRect(game.render,&buttonPos[SCENE_CLIENT_0][11],0xff000000);
    textImg[tname_self].drawTexture(buttonPos[SCENE_CLIENT_0][11].x +10,buttonPos[SCENE_CLIENT_0][11].y-20,textImg[tname_self].w/2,textImg[tname_self].h/2);

    /* デバイス番号が3桁入力されたら */
    if(strlen(game.deviceNum)>=3){
        //ボタンの表示　「確定」
        rectangleColorRect(game.render,&buttonPos[SCENE_CLIENT_0][12],0xff000000);
        textImg[tname_enter].drawTexture(buttonPos[SCENE_CLIENT_0][12].x +10,buttonPos[SCENE_CLIENT_0][12].y-20);
    }

    //選択状況の表示
    boxColorRect(game.render,&buttonPos[SCENE_CLIENT_0][game.selectButton],0x77777777);

    SDL_RenderPresent(game.render);
}

/* カスタマイズ画面の描画 */
void RenderCustomizeWindow(void){
    
    //ボタンの場所更新
    buttonPos[SCENE_CUSTOMIZE][1].y = 57-scrollValue;
    buttonPos[SCENE_CUSTOMIZE][1].y = 130-scrollValue;
    buttonPos[SCENE_CUSTOMIZE][2].y = 370-scrollValue;
    buttonPos[SCENE_CUSTOMIZE][3].y = 700-scrollValue;
    int buttonSize[maxButtonNum[game.scene]];
    for(int i=0; i<maxButtonNum[game.scene]; i++){
        buttonSize[i] = (i==game.selectButton) ? 1 : 0;
    }

    //背景色（黄土色）
    /*
    SDL_SetRenderDrawColor(game.render,253,245,230,255);
    SDL_RenderClear(game.render);*/
    //背景
    DrawBackGround();

    // 文字の表示　「CUSTOMIZE」
    textImg[tname_customize].drawTexture(350,25-scrollValue);
    uiImg[uname_explain_skill].drawTexture(790,25-scrollValue,uiImg[uname_explain_skill].w/1.5,uiImg[uname_explain_skill].h/1.5);//説明

    // 戻るボタン
    uiImg[uname_backButton].drawTexture(30-buttonSize[0]*20,45-buttonSize[0]*20-scrollValue,uiImg[uname_backButton].w/1.2+buttonSize[0]*40,uiImg[uname_backButton].h/1.2+buttonSize[0]*40);

    // 画像の表示　「名前入力欄」
    uiImg[uname_name].drawTexture(50,10-scrollValue,uiImg[uname_name].w/1.2,uiImg[uname_name].h/1.2);

    
    // 文字の表示　「名前」
    for(int i=0; game.clientName[i]!='\0'; i++){
            textName tn = retTextNameFromChar( game.clientName[i]);
            textImg[tn].drawTexture(370+ i*27,160-scrollValue,textImg[tn].w/1.8,textImg[tn].h/1.8);
    }
    // 画像の表示　「変更ボタン」
    uiImg[uname_change].drawTexture(buttonPos[SCENE_CUSTOMIZE][1].x-(buttonSize[1]*25),buttonPos[SCENE_CUSTOMIZE][1].y-(buttonSize[1]*25),(buttonSize[1]*50)+uiImg[uname_change].w/4,(buttonSize[1]*50)+uiImg[uname_change].h/4);

    // 画像の表示　「スキル欄」
    uiImg[uname_skill].drawTexture(50,120-scrollValue,uiImg[uname_skill].w/1.2,uiImg[uname_skill].h/1.2);
    // 画像の表示　「選択しているスキル」
    uiImg[uname_skill_attack+(int)game.skill].drawTexture(150,360-scrollValue,uiImg[uname_skill_attack+(int)game.skill].w/1.4,uiImg[uname_skill_attack+(int)game.skill].h/1.4);
    // 画像の表示　「選択中」
    uiImg[uname_nowselect].drawTexture(80,340-scrollValue,uiImg[uname_nowselect].w/4,uiImg[uname_nowselect].h/4);
    // 画像の表示　「変更ボタン」
    uiImg[uname_change].drawTexture(buttonPos[SCENE_CUSTOMIZE][2].x-(buttonSize[2]*25),buttonPos[SCENE_CUSTOMIZE][2].y-(buttonSize[2]*25),(buttonSize[2]*50)+uiImg[uname_change].w/4,(buttonSize[2]*50)+uiImg[uname_change].h/4);

    // 画像の表示　「スペシャル欄」
    uiImg[uname_special].drawTexture(50,410-scrollValue,uiImg[uname_special].w/1.2,uiImg[uname_special].h/1.2);
    // 画像の表示　「変更ボタン」
    uiImg[uname_change].drawTexture(buttonPos[SCENE_CUSTOMIZE][3].x-(buttonSize[3]*25),buttonPos[SCENE_CUSTOMIZE][3].y-(buttonSize[3]*25),(buttonSize[3]*50)+uiImg[uname_change].w/4,(buttonSize[3]*50)+uiImg[uname_change].h/4);

    // 画像の表示　「ステータス欄」
    uiImg[uname_status].drawTexture(50,760-scrollValue,uiImg[uname_status].w/1.2,uiImg[uname_status].h/1.2);
    // 画像の表示　「ピンの表示」
    uiImg[uname_pin].drawTexture(parm_x[game.parm[0]+5]-(buttonSize[4]*10) ,1005-scrollValue-(buttonSize[4]*10),uiImg[uname_pin].w/5+(buttonSize[4]*20),uiImg[uname_pin].h/5+(buttonSize[4]*20));
    uiImg[uname_pin].drawTexture(parm_x[game.parm[1]+5]-(buttonSize[5]*10) ,1140-scrollValue-(buttonSize[5]*10),uiImg[uname_pin].w/5+(buttonSize[5]*20),uiImg[uname_pin].h/5+(buttonSize[5]*20));
    uiImg[uname_pin].drawTexture(parm_x[game.parm[2]+5]-(buttonSize[6]*10) ,1277-scrollValue-(buttonSize[6]*10),uiImg[uname_pin].w/5+(buttonSize[6]*20),uiImg[uname_pin].h/5+(buttonSize[6]*20));
    uiImg[uname_pin].drawTexture(parm_x[game.parm[3]+5]-(buttonSize[7]*10) ,1415-scrollValue-(buttonSize[7]*10),uiImg[uname_pin].w/5+(buttonSize[7]*20),uiImg[uname_pin].h/5+(buttonSize[7]*20));
    uiImg[uname_pin].drawTexture(parm_x[game.parm[4]+5]-(buttonSize[8]*10) ,1545-scrollValue-(buttonSize[8]*10),uiImg[uname_pin].w/5+(buttonSize[8]*20),uiImg[uname_pin].h/5+(buttonSize[8]*20));
    // 数値の表示
    for(int i=0; i<PARAMATER_NUM; i++)
    {
        if(game.parm[i]<0){
            textImg[tname_hyphen].drawTexture(943,parmText_y[i]+12-scrollValue,textImg[tname_hyphen].w/2,textImg[tname_hyphen].h/2);
            textImg[tname_0+(-game.parm[i])].drawTexture(965,parmText_y[i]-scrollValue,textImg[tname_0+(-game.parm[i])].w/1.5,textImg[tname_0+(-game.parm[i])].h/1.5);
        }
        else{
            textImg[tname_0+game.parm[i]].drawTexture(955,parmText_y[i]-scrollValue,textImg[tname_0+game.parm[i]].w/1.5,textImg[tname_0+game.parm[i]].h/1.5);
        }
    }
    // ステータスパラメータの合計数値の表示
    int rest = PARAMATER_SUM_MAX - retSumParamater();
    textImg[tname_0+rest/10].drawTexture(830,1675-scrollValue,textImg[tname_0+rest/10].w/1.2,textImg[tname_0+rest/10].h/1.2);
    textImg[tname_0+rest%10].drawTexture(875,1675-scrollValue,textImg[tname_0+rest%10].w/1.2,textImg[tname_0+rest%10].h/1.2);

    //名前の入力画面
    if(game.popScene == PopUp_Name){
        uiImg[uname_back].drawTexture(0,0); //後ろを暗くする
        uiImg[uname_nameChange].drawTexture(0,0);
        for(int i=0; tempName[i]!='\0'; i++){
            textName tn = retTextNameFromChar(tempName[i]);
            textImg[tn].drawTexture(150+ i*45,210);
        }
    }
    //スキル変更画面
    else if(game.popScene == PopUp_Skill){
        uiImg[uname_back].drawTexture(0,0); //後ろを暗くする
        uiImg[uname_skillChange].drawTexture(300,50);//タイトル
        boxColor(game.render,860,10,1190,110,0xffe6f5fd);
        uiImg[uname_explain_skill].drawTexture(860,10,uiImg[uname_explain_skill].w/1.6,uiImg[uname_explain_skill].h/1.6);//説明
        uiImg[uname_skill_attack+(int)game.skill].drawTexture(200,200,uiImg[uname_skill_attack+(int)game.skill].w/1.2,uiImg[uname_skill_attack+(int)game.skill].h/1.2);//選択中のスキル
        uiImg[uname_nowselect].drawTexture(170,170,uiImg[uname_nowselect].w/4,uiImg[uname_nowselect].h/4);//選択中
        boxColor(game.render,150,370,1100,890,0xffe6f5fd);//黄土色の枠組み
        for(int i=0; i<SKILL_NUM; i++){
            uiImg[uname_skill_attack+i].drawTexture(300,400+i*150,uiImg[uname_skill_attack+i].w/1.4,uiImg[uname_skill_attack+i].h/1.4);
        }
        if(game.selectButton_sub != 0){
            uiImg[uname_backButton].drawTexture(30,65,uiImg[uname_backButton].w/1.1,uiImg[uname_backButton].h/1.1);//戻るボタン
            uiImg[uname_selectHikouki].drawTexture(170,380+(game.selectButton_sub-1)*150,uiImg[uname_selectHikouki].w/1.3,uiImg[uname_selectHikouki].h/1.3);//選択していることを示す飛行機
        }
        else{
            uiImg[uname_backButton].drawTexture(23,58,uiImg[uname_backButton].w,uiImg[uname_backButton].h);//戻るボタン
            boxColor(game.render,30,73,270,150,0x77777777);
        }
    }
    //スペシャル変更の画面
    else if(game.popScene == PopUp_Special){
        uiImg[uname_back].drawTexture(0,0); //後ろを暗くする
        uiImg[uname_skillChange].drawTexture(300,50);//タイトル
        boxColor(game.render,860,10,1190,110,0xffe6f5fd);
        uiImg[uname_explain_skill].drawTexture(860,10,uiImg[uname_explain_skill].w/1.6,uiImg[uname_explain_skill].h/1.6);//説明
        if(game.selectButton_sub != 0){
            uiImg[uname_backButton].drawTexture(30,65,uiImg[uname_backButton].w/1.1,uiImg[uname_backButton].h/1.1);//戻るボタン
        }
        else{
            uiImg[uname_backButton].drawTexture(23,58,uiImg[uname_backButton].w,uiImg[uname_backButton].h);//戻るボタン
            boxColor(game.render,30,73,270,150,0x77777777);
        }
    }

    SDL_RenderPresent(game.render);
}

void RenderClientWaitWindow(void){
    //背景
    DrawBackGround();

    textImg[tname_nowloading].drawTexture(100,100,textImg[tname_nowloading].w/1.2,textImg[tname_nowloading].h/1.2);

    SDL_RenderPresent(game.render);
}

void RenderResultWindow(void){
    //背景
    DrawBackGround();

    textImg[tname_result].drawTexture(100,100,textImg[tname_result].w/1.2,textImg[tname_result].h/1.2);

    int cnum = sizeof(game.rankingName)/sizeof(game.rankingName[0]);
    printf("num:%d\n",cnum);
    for(int i=0; i<cnum; i++){
        textImg[tname_1 + i].drawTexture(200 ,300 +i*100);
        for(int j=0; game.rankingName[i][j]!='\0'; j++){
            printf("name:%c\n",game.rankingName[i][j]);
            textName tn = retTextNameFromChar(game.rankingName[i][j]);
            textImg[tn].drawTexture(350+ j*45, 300+i*100);
        }
    }


    rectangleColorRect(game.render,&buttonPos[SCENE_Result][0],0xff000000);
    textImg[tname_exit].drawTexture(buttonPos[SCENE_Result][0].x,buttonPos[SCENE_Result][0].y, textImg[tname_exit].w/1.6, textImg[tname_exit].h/1.6);


    rectangleColorRect(game.render,&buttonPos[SCENE_Result][1],0xff000000);
    textImg[tname_totitle].drawTexture(buttonPos[SCENE_Result][1].x,buttonPos[SCENE_Result][1].y, textImg[tname_totitle].w/1.6, textImg[tname_totitle].h/1.6);


    boxColorRect(game.render,&buttonPos[SCENE_Result][game.selectButton],0x77777777);

    SDL_RenderPresent(game.render);
}

/* テクスチャの描画 */
void ImgInfo::drawTexture(int x, int y, int width, int height){
    if(width == 0 || height == 0){
        width=w;
        height=h;
    }
    // 転送元設定
    SDL_Rect src = {0,0,w,h};
    // 転送先設定
    SDL_Rect dst = {x, y, width, height};
    // 転送
    if (0 > SDL_RenderCopy(game.render, texture, &src, &dst)) {
        PrintError(SDL_GetError());
    }
}
/* テクスチャの描画 */
void ImgInfo::drawRotateTexture(int x, int y, int width, int height, int rad){
    if(width == 0 || height == 0){
        width=w;
        height=h;
    }
    // 転送元設定
    SDL_Rect src = {0,0,w,h};
    // 転送先設定
    SDL_Rect dst = {x, y, width, height};
    // 転送
    if (0 > SDL_RenderCopyEx(game.render, texture, &src, &dst,rad, NULL, SDL_FLIP_HORIZONTAL)) {
        PrintError(SDL_GetError());
    }
}

static int rectangleColorRect(SDL_Renderer *render, SDL_Rect *rect, Uint32 color){
    return rectangleColor(render,rect->x, rect->y, rect->x+rect->w, rect->y+rect->h, color);
}
static int boxColorRect(SDL_Renderer *render, SDL_Rect *rect, Uint32 color){
    return boxColor(render,rect->x, rect->y, rect->x+rect->w, rect->y+rect->h, color);
}

textName retTextNameFromChar(char ch){
    if('0' <= ch && ch <= '9'){
        return static_cast<textName>(tname_0 + (int)(ch - '0'));
    }
    else if('a' <= ch && ch <= 'z'){
        return static_cast<textName>(tname_a + (int)(ch -'a'));
    }
    else if('A' <= ch && ch <= 'Z'){
        return static_cast<textName>(tname_A + (int)(ch -'A'));
    }
    else if(ch == ' ')
    {
        return static_cast<textName>(tname_space);
    }
    else{
        return tname_enter;
    }
}

void DrawBackGround(void){
    scroll_back -= 0.3;
    masao_loop += M_PI/180;
    if(masao_loop >= 2*M_PI){
        masao_loop = 0;
    }
    //背景
    uiImg[uname_cloud].drawTexture((int)scroll_back,0,uiImg[uname_cloud].w,uiImg[uname_cloud].h);
    if(scroll_back <= -uiImg[uname_cloud].w/2){
        scroll_back = 0;
    }
    uiImg[uname_castle].drawTexture(0,0,uiImg[uname_castle].w,uiImg[uname_castle].h);
    uiImg[uname_masao].drawTexture(600+400*sin(masao_loop),700,uiImg[uname_masao].w/2,uiImg[uname_masao].h/2);
    uiImg[uname_masao_face].drawRotateTexture(masao_x,masao_y,uiImg[uname_masao_face].w,uiImg[uname_masao_face].h,masao_rotate);
    masao_rotate +=8;
    masao_x += (random()%2 == 1) ? 5: -5;
    masao_y += (random()%2 == 1) ? 5: -5;
    if(masao_x >= 1000)
    {
        masao_x -= 5;
    }
    else if(masao_x <=0){
        masao_x += 5;
    }
    else if(masao_y >= 1000){
        masao_y -= 5;
    }
    else if(masao_y <= -200){
        masao_y += 5;
    }
}

/*
    //画像の描画
    // 転送元設定
    src = {0,0,uiImg[0].w, uiImg[0].h};
    // 転送先設定
    dst = {0,0,uiImg[0].w, uiImg[0].h};
    // 転送
    if (0 > SDL_RenderCopy(game.render, uiImg[0].texture, &src, &dst)) {
        PrintError(SDL_GetError());
    }

*/

/*
    // 文字の表示　「」
    src = {0,0,textImg[0].w,textImg[0].h};
    dst = {0,0,textImg[0].w,textImg[0].h};
    if (0 > SDL_RenderCopy(game.render, textImg[0].texture, &src, &dst)) {
        PrintError(SDL_GetError());
    }
*/
