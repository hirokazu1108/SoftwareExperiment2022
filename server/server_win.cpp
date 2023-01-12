#include "server.h"

/* 画像パス */
static const char *imgFile[IMG_NUM] = { "title_sky.png","title_sky2.png" };
static const char *textStr[TEXT_NUM] = {"Wait Client","PassCode:","0","1","2","3","4","5","6","7","8","9"};
/* フォントパス */
static char gFontFile[] = "../fonts/FreeSerifBoldItalic.ttf";

/* ボタンの位置 */
std::vector<SDL_Rect> buttonPos[SCENE_NUM];


static const SDL_Color gWhite = { 255, 255, 255, 255 };
static const SDL_Color gBlue  = { 0, 0, 255, 255 };

ImgInfo uiImg[IMG_NUM];
ImgInfo textImg[TEXT_NUM];


/* 関数 */
static int MakeMessage(void);
static int rectangleColorRect(SDL_Renderer *render, SDL_Rect *rect, Uint32 color);
static int boxColorRect(SDL_Renderer *render, SDL_Rect *rect, Uint32 color);

// メインウインドウの表示，設定
int InitWindow(void)
{
    /* SDL_image初期化 */
    if (IMG_INIT_PNG != IMG_Init(IMG_INIT_PNG)) {
        return PrintError("failed to initialize SDL_image");
    }

    /** メインのウインドウ(表示画面)とレンダラーの作成 **/
    gUi.window = SDL_CreateWindow("main", 0, 0, WD_Width, WD_Height, 0);
    if (gUi.window == NULL)
        return PrintError(SDL_GetError());

    gUi.render = SDL_CreateRenderer(gUi.window, -1, 0);
    if (gUi.render == NULL)
        return PrintError(SDL_GetError());

    /** キャラ画像の読み込み **/
    /* 画像は，
     *  横方向にアニメーションパターン
     *  縦方向に向きパターン（時計回り，最初が↑）
     *  があると想定
     */
    for (int i = 0; i < IMG_NUM; i++) {
        SDL_Surface *s = IMG_Load(imgFile[i]);
        if (NULL == s) {
            return PrintError("failed to open ui image.");
        }
        uiImg[i].texture = SDL_CreateTextureFromSurface(gUi.render, s);
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

    /** ウインドウへの描画 **/
    RenderWaitClientWindow();

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

    SDL_DestroyRenderer(gUi.render);
    SDL_DestroyWindow(gUi.window);
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
            textImg[i].texture = SDL_CreateTextureFromSurface(gUi.render, sf);
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
    if (0 > SDL_RenderCopy(gUi.render, texture, &src, &dst)) {
        PrintError(SDL_GetError());
    }
}
static int rectangleColorRect(SDL_Renderer *render, SDL_Rect *rect, Uint32 color){
    return rectangleColor(render,rect->x, rect->y, rect->x+rect->w, rect->y+rect->h, color);
}
static int boxColorRect(SDL_Renderer *render, SDL_Rect *rect, Uint32 color){
    return boxColor(render,rect->x, rect->y, rect->x+rect->w, rect->y+rect->h, color);
}

void RenderWaitClientWindow(void){
    //背景
    uiImg[uname_title_sky2].drawTexture(0,0,uiImg[uname_title_sky2].w,uiImg[uname_title_sky2].h);

    // 文字の表示　「クライアントを待っています」
    textImg[tname_WaitClient].drawTexture(300,100);

    // 文字の表示 「パスコード」
    textImg[tname_Passcode].drawTexture(150,200);
    for(int i=1; i<5; i++){
        textImg[tname_0+gUi.port[i]-'0'].drawTexture(550+i*50,200);
    }

    // 文字の表示　「現在の参加人数」
    textImg[tname_0+gUi.currentNum].drawTexture(300,500);

    //文字の表示　　「総数」
    textImg[tname_0+gClientNum].drawTexture(450,600);
    
    // 文字の表示  「時間」
    int timeText[3];
    timeText[0] = gUi.time_sec/100;
    timeText[1] = (gUi.time_sec%100)/10;
    timeText[2] = (gUi.time_sec%100)%10;
    for(int i=0; i<3; i++){
        textImg[tname_0+timeText[i]].drawTexture(800+i*40,800);
    }
    

    SDL_RenderPresent(gUi.render);
}

