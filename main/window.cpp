#include "header.h"

/* ??Á≥∏???????? */
static const char *imgFile[IMG_NUM] = { "inputClientNum.png","inputPasscode.png","inputDeviceNum.png","name.png", "skill.png","special.png","status.png", "explain_skill.png","nowSelectButton.png", "changeButton.png","backButton.png",  "skill_attack.png", "skill_hp.png","skill_speed.png", "pin.png","rightSelect.png","leftSelect.png", "back.png","nameChange.png","skillChange.png","specialChange.png","selectHikouki.png","barrier.png","disabled.png","bigbullet.png","barrier_icon.png","disabled_icon.png","bigbullet_icon.png","logo.png", "rankingBoard.png", "rankingBack.png","title_sky.png","title_sky2.png","cloud.png", "castle.png","masao.png","masao_face.png"};
static const char *textStr[TEXT_NUM] = {"Space Battle","SERVER","CLIENT","CUSTOMIZE","input client num.","input passcode.","del","Enter","self","input device num.","clpc","nowLoading...","Result","Exit","Detail","Title","SCORE RANKING","KILL RANKING","DEATH RANKING", "KILL ENEMY RANKING","0","1","2","3","4","5","6","7","8","9","a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z","A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"," ", "-"};
/* ????????ÊΩ????????? */
static char gFontFile[] = "../fonts/Yomogi-Regular.ttf";

/* ?????Â∏•?ÊΩ???ÁØ??ËáÄ? */
std::vector<SDL_Rect> buttonPos[SCENE_NUM];
std::vector<SDL_Rect> special_iconPos;

float scroll_back = -50.0; //???????????È¥ª????„è??Ëã•??

int rankingMode = 0; // 0:score  ,1:kill num of player  ,2:death  ,3:kill num of enemy and boss

float masao_loop = 0;
float masao_rotate = 0;
float masao_x = 500;
float masao_y = 500;

/* ?????????º??Ëã•?Â∏•????Ë¥??? */
static int parm_x[11] = {380,425,475,520,567,618,664,710,756,802,850};//?????ÊΩ???xÁ∂∫—???
static int parmText_y[PARAMATER_NUM] ={1025,1160,1297,1435,1565};//??Âåª?„Ç???yÁ∂∫—???

static const SDL_Color gWhite = { 255, 255, 255, 255 };
static const SDL_Color gBlue  = { 0, 0, 255, 255 };

ImgInfo uiImg[IMG_NUM];
ImgInfo textImg[TEXT_NUM];

/* ??‚àΩ?? */
static int MakeMessage(void);
static int rectangleColorRect(SDL_Renderer *render, SDL_Rect *rect, Uint32 color);
static int boxColorRect(SDL_Renderer *render, SDL_Rect *rect, Uint32 color);
static textName retTextNameFromChar(char ch);
static void DrawBackGround(void);

// ???º??„Ç??ÊΩ?????„Ç??ÊΩ?????????Ëåµ?ËÖìÂâä??Ëçê„ä§??
int InitWindow(void)
{
    /* SDL_image????????? */
    if (IMG_INIT_PNG != IMG_Init(IMG_INIT_PNG)) {
        return PrintError("failed to initialize SDL_image");
    }

    /** ???º??„Ç??ÊΩ????????„Ç??ÊΩ??????(Ëåµ?ËÖìÊ¶??ËÑ???)????????ÊΩ????????Ëã•??ÁØ????? **/
    game.window = SDL_CreateWindow("main", 0, 0, WD_Width, WD_Height, 0);
    if (game.window == NULL)
        return PrintError(SDL_GetError());

    game.render = SDL_CreateRenderer(game.window, -1, 0);
    if (game.render == NULL)
        return PrintError(SDL_GetError());

    /** ??„è???Ω?????Á≥∏?????ËåØ„è??Á≤ãÊò≠??? **/
    /* ??Á≥∏?????Èê??
     *  ÁΩ????Âä´???????‚â™?????º??Ëã•?ÊøÄ?—??ÊΩ?????Â∏•?Ëã•??
     *  ËÜ∞???Âä´????????????????Â∏•?Ëã•?È∞π?????Ëç????????Èê??????????????Èê??
     *  ??????????????Âñ???
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

    /** ???º?????Á•??Ëã•?Â®Ø????? **/
    if (MakeMessage())
        return -1;

    /* ?????Â∏•?ÊΩ???Á∂∫—????????‰∏ª?? */
    buttonPos[SCENE_Title].push_back({270,450,600,100}); //server
    buttonPos[SCENE_Title].push_back({270,620,600,100}); //client
    buttonPos[SCENE_Title].push_back({270,790,600,100}); //customize
    buttonPos[SCENE_SERVER_0].push_back({30,45,uiImg[uname_backButton].w/1.2,uiImg[uname_backButton].h/1.2}); //back
    buttonPos[SCENE_SERVER_0].push_back({350,480,100,100}); //2
    buttonPos[SCENE_SERVER_0].push_back({500,480,100,100}); //3
    buttonPos[SCENE_SERVER_0].push_back({650,480,100,100}); //4
    buttonPos[SCENE_SERVER_1].push_back({30,45,uiImg[uname_backButton].w/1.2,uiImg[uname_backButton].h/1.2}); //back 
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
    buttonPos[SCENE_SERVER_1].push_back({200,600,400,100}); //ËÖ?Èò™??
    buttonPos[SCENE_CLIENT_0].push_back({30,45,uiImg[uname_backButton].w/1.2,uiImg[uname_backButton].h/1.2}); //back
    buttonPos[SCENE_CLIENT_0].push_back({170,500,80,80});  //0
    buttonPos[SCENE_CLIENT_0].push_back({270,500,80,80}); //1
    buttonPos[SCENE_CLIENT_0].push_back({370,500,80,80}); //2
    buttonPos[SCENE_CLIENT_0].push_back({470,500,80,80}); //3
    buttonPos[SCENE_CLIENT_0].push_back({570,500,80,80}); //4
    buttonPos[SCENE_CLIENT_0].push_back({170,620,80,80}); //5
    buttonPos[SCENE_CLIENT_0].push_back({270,620,80,80}); //6
    buttonPos[SCENE_CLIENT_0].push_back({370,620,80,80}); //7
    buttonPos[SCENE_CLIENT_0].push_back({470,620,80,80}); //8
    buttonPos[SCENE_CLIENT_0].push_back({570,620,80,80}); //9
    buttonPos[SCENE_CLIENT_0].push_back({670,560,80,80}); //?
    buttonPos[SCENE_CLIENT_0].push_back({150,750,100,100}); //localHost
    buttonPos[SCENE_CLIENT_0].push_back({270,750,400,100}); //ËÖ?Èò™??
    buttonPos[SCENE_CLIENT_1].push_back({30,45,uiImg[uname_backButton].w/1.2,uiImg[uname_backButton].h/1.2}); //back
    buttonPos[SCENE_CLIENT_1].push_back({120,400,80,80}); //0
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
    buttonPos[SCENE_CLIENT_1].push_back({200,600,400,100}); //ËÖ?Èò™??
    buttonPos[SCENE_CUSTOMIZE].push_back({36,57-scrollValue,236,120}); //??Á•????????Â∏•??
    buttonPos[SCENE_CUSTOMIZE].push_back({930,130-scrollValue,0,0}); //??????Á¥?????(Á≠ùÂêæ????Â∏•??)
    buttonPos[SCENE_CUSTOMIZE].push_back({900,370-scrollValue,0,0}); //??È¥ª?„è???Á¥?????(Á≠ùÂêæ????Â∏•??)
    buttonPos[SCENE_CUSTOMIZE].push_back({900,700-scrollValue,0,0}); //??È¥ª????ÊøÄ??Ω???Á¥?????(Á≠ùÂêæ????Â∏•??)
    buttonPos[SCENE_Result].push_back({590,850,200,90}); //left
    buttonPos[SCENE_Result].push_back({830,850,200,90}); //right
    buttonPos[SCENE_Result].push_back({350,850,200,90}); //exit
    buttonPos[SCENE_Result].push_back({590,850,200,90}); //detail
    buttonPos[SCENE_Result].push_back({830,850,200,90}); //totitle

    special_iconPos.push_back({23,58,uiImg[uname_backButton].w,uiImg[uname_backButton].h});//??Á•????????Â∏•??
    special_iconPos.push_back({220,320,128,128});
    for(int i=2; i<SPECIAL_NUM+1; i++){
        special_iconPos.push_back({special_iconPos[i-1].x+special_iconPos[i-1].w+20,special_iconPos[i-1].y,128,128});
    }

    if(game.scene == SCENE_Title)
        RenderTitleWindow();
    else if(game.scene == SCENE_CLIENT_WAIT)
        RenderClientWaitWindow();

    /* image??????ËÖ??ÁØ??(????????È¥ª?????Ω???Ëçµ‚à´??Á∑???????Ëö??Ëã•??Á≠ù„èç??ÁØèÂ∏•??????????????) */
    IMG_Quit();

    return 0;
}

/* ?????„Ç??ÊΩ?????????ËÖ??ÁØ???????? */
void DestroyWindow(void)
{
    /* ????????È¥ª?????Ω?????? */
    for (int i = 0; i < IMG_NUM; i++)
        SDL_DestroyTexture(uiImg[i].texture);
    for (int i = 0; i < TEXT_NUM; i++)
        SDL_DestroyTexture(textImg[i].texture);

    SDL_DestroyRenderer(game.render);
    SDL_DestroyWindow(game.window);
}

/* ???º?????Á•??Ëã•?Â®Ø?????
 *
 * Ëè¥????
 *   ÁΩ©?Ω?ÂπªËÖ??ÁØ??: 0
 *   ?????????  : Ëé¢????
 */
static int MakeMessage(void)
{
    int ret = 0;
    /* ????????ÊΩ????????????º?????Á•??Ëã•?Âêæ???????È¥ª?????Ω?????? */
    /* ????????? */
    if (TTF_Init() < 0) {
        return PrintError(TTF_GetError());
    }
    /* ????????ÊΩ???????????? */
    TTF_Font *ttf = TTF_OpenFont(gFontFile, 90);
    if (NULL == ttf) {
        ret = PrintError(TTF_GetError());
    }
    /* ???º?????Á•??Ëã•?Â®Ø????? */
    SDL_Color cols[TEXT_NUM] = { gBlue };
    for (int i = 0; i < TEXT_NUM && ttf; i++) {
        SDL_Surface *sf;
        /* ????????ÊΩ?????????Áµ?????Èê????Ëö????????Ê≥£?Ëã•????—??„Ç??Ââ?????? */
        sf = TTF_RenderUTF8_Blended(ttf, textStr[i], cols[i]);
        if (NULL == sf) {
            ret = PrintError(TTF_GetError());
        } else {
            /* ????????È¥ª?????Ω??? */
            textImg[i].texture = SDL_CreateTextureFromSurface(game.render, sf);
            if (NULL == textImg[i].texture) {
                ret = PrintError(SDL_GetError());
            }
            if (0 > SDL_QueryTexture(textImg[i].texture, NULL, NULL, &textImg[i].w, &textImg[i].h)) {
                PrintError(SDL_GetError());
            }
            /* ??Ê≥£?Ëã•????—??„Ç??Â£ï–????(????????È¥ª?????Ω???Ëçµ‚à´??Á∑???????Ëö??Ëã•??Á≠ù„èç??ÁØèÂ∏•??????????????) */
            SDL_FreeSurface(sf);
        }
    }

    /* ????????ÊΩ??????????????? */
    TTF_CloseFont(ttf);
    /* ????????ÊΩ?????????ËÖ??ÁØ??(????????È¥ª?????Ω???Ëçµ‚à´??Á∑???????Ëö??Ëã•??Á≠ù„èç??ÁØèÂ∏•??????????????) */
    TTF_Quit();

    return ret;
}

/* ?????„Ç??ÊΩ????????????
 *  ???º??„Ç??ÊΩ?????„Ç??ÊΩ???????????????Èê????„è???Ω???Èê?????º?????Á•??Ëã•?Âêæ????????Ëçµ‚à´????????
 */
void RenderTitleWindow(void)
{
    //??????
    uiImg[uname_title_sky].drawTexture(-50,0,uiImg[uname_title_sky].w/1.07,uiImg[uname_title_sky].h/1.07);
    
    // ???Áµ?????Ëåµ?ËÖìÂ?¥?????Game Title???
    uiImg[uname_logo].drawTexture(160,30,uiImg[uname_logo].w*2.7,uiImg[uname_logo].h*2.7);


    //?????Â∏•?ÊΩ???Ëåµ?ËÖìÂ?¥???????Ê≥£?Ëã•????Ëã•??
    boxColor(game.render,buttonPos[SCENE_Title][0].x,buttonPos[SCENE_Title][0].y,buttonPos[SCENE_Title][0].x+buttonPos[SCENE_Title][0].w,buttonPos[SCENE_Title][0].y+buttonPos[SCENE_Title][0].h,0xbbffffff);
    rectangleColorRect(game.render,&buttonPos[SCENE_Title][0],0xff000000);
    textImg[tname_server].drawTexture(buttonPos[SCENE_Title][0].x+140,buttonPos[SCENE_Title][0].y-20);
    
    //?????Â∏•?ÊΩ???Ëåµ?ËÖìÂ?¥?????????????„Ç??‚â™?ÊΩ??????
    boxColor(game.render,buttonPos[SCENE_Title][1].x,buttonPos[SCENE_Title][1].y,buttonPos[SCENE_Title][1].x+buttonPos[SCENE_Title][1].w,buttonPos[SCENE_Title][1].y+buttonPos[SCENE_Title][1].h,0xbbffffff);
    rectangleColorRect(game.render,&buttonPos[SCENE_Title][1],0xff000000);
    textImg[tname_client].drawTexture(buttonPos[SCENE_Title][1].x+30,buttonPos[SCENE_Title][1].y-20);

    //?????Â∏•?ÊΩ???Ëåµ?ËÖìÂ?¥??????????È¥ª?Â∏•????„Ç??ÂÜ¥??
    boxColor(game.render,buttonPos[SCENE_Title][2].x,buttonPos[SCENE_Title][2].y,buttonPos[SCENE_Title][2].x+buttonPos[SCENE_Title][2].w,buttonPos[SCENE_Title][2].y+buttonPos[SCENE_Title][2].h,0xbbffffff);
    rectangleColorRect(game.render,&buttonPos[SCENE_Title][2],0xff000000);
    textImg[tname_customize].drawTexture(buttonPos[SCENE_Title][2].x+20,buttonPos[SCENE_Title][2].y-20,textImg[tname_customize].w/0.95,textImg[tname_customize].h*0.95);

    //??Âæ°????ÂÄ∂?????Ëåµ?ËÖ??
    boxColorRect(game.render,&buttonPos[SCENE_Title][game.selectButton],0xaa666666);

    // masaloop
    if(strcmp(tempName,"masao")==0||strcmp(tempName,"Masao")==0||strcmp(tempName,"MASAO")==0){
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
    

    SDL_RenderPresent(game.render);
}

/* ??Ê≥£?Ëã•????Ëã•??ÁØãÂ?∫?Âåª?„É•???????ËÑ??? */
void RenderServerWindow_0(void){
    //??????
    DrawBackGround();

    //back
    if(game.selectButton == 0){
        uiImg[uname_backButton].drawTexture(buttonPos[SCENE_SERVER_0][0].x-20,buttonPos[SCENE_SERVER_0][0].y-20,buttonPos[SCENE_SERVER_0][0].w+40,buttonPos[SCENE_SERVER_0][0].h+40);
    }
    else{
        uiImg[uname_backButton].drawTexture(buttonPos[SCENE_SERVER_0][0].x,buttonPos[SCENE_SERVER_0][0].y,buttonPos[SCENE_SERVER_0][0].w,buttonPos[SCENE_SERVER_0][0].h);
    }

    // ???Áµ?????Ëåµ?ËÖìÂ?¥?????????????„Ç??‚â™?ÊΩ???ÁØãÂ?∫?ÈÅ???
    uiImg[uname_rankingBoard].drawTexture(100,320,uiImg[uname_rankingBoard].w*1.44,uiImg[uname_rankingBoard].h*0.2);
    uiImg[uname_inputClientNum].drawTexture(100,340,uiImg[uname_inputClientNum].w*2,uiImg[uname_inputClientNum].h*2);

    //?????Â∏•?ÊΩ???Ëåµ?ËÖìÂ?¥?????2???
    boxColorRect(game.render,&buttonPos[SCENE_SERVER_0][1],0xffffffff);
    rectangleColorRect(game.render,&buttonPos[SCENE_SERVER_0][1],0xff000000);
    textImg[tname_2].drawTexture(buttonPos[SCENE_SERVER_0][1].x +10,buttonPos[SCENE_SERVER_0][1].y-10);
    
    //?????Â∏•?ÊΩ???Ëåµ?ËÖìÂ?¥?????3???
    boxColorRect(game.render,&buttonPos[SCENE_SERVER_0][2],0xffffffff);
    rectangleColorRect(game.render,&buttonPos[SCENE_SERVER_0][2],0xff000000);
    textImg[tname_3].drawTexture(buttonPos[SCENE_SERVER_0][2].x+10,buttonPos[SCENE_SERVER_0][2].y-10);

    //?????Â∏•?ÊΩ???Ëåµ?ËÖìÂ?¥?????4???
    boxColorRect(game.render,&buttonPos[SCENE_SERVER_0][3],0xffffffff);
    rectangleColorRect(game.render,&buttonPos[SCENE_SERVER_0][3],0xff000000);
    textImg[tname_4].drawTexture(buttonPos[SCENE_SERVER_0][3].x+10,buttonPos[SCENE_SERVER_0][3].y-10);

    //??Âæ°????ÂÄ∂?????Ëåµ?ËÖ??
    if(game.selectButton != 0)
        boxColorRect(game.render,&buttonPos[SCENE_SERVER_0][game.selectButton],0xaa666666);

    SDL_RenderPresent(game.render);
}

/* ?????È¥ª?ÊΩ??Ëã•????„É•???????ËÑ??? */
void RenderPasscodeWindow(){
    //??????
    DrawBackGround();


    //back
    if(game.selectButton == 0){
        uiImg[uname_backButton].drawTexture(buttonPos[game.scene][0].x-20,buttonPos[game.scene][0].y-20,buttonPos[game.scene][0].w+40,buttonPos[game.scene][0].h+40);
    }
    else{
        uiImg[uname_backButton].drawTexture(buttonPos[game.scene][0].x,buttonPos[game.scene][0].y,buttonPos[game.scene][0].w,buttonPos[game.scene][0].h);
    }
    
    // ???Áµ?????Ëåµ?ËÖìÂ?¥??????????È¥ª?ÊΩ??Ëã•?????
    textImg[tname_inputPasscode].drawTexture(300,100);

    //??„É•?????????????Ëã•??????Âû©??Ëåµ?ËÖ??
    for(int i=0; game.port[i] != '\0'; i++)
    {
        textImg[tname_0+game.port[i]-'0'].drawTexture(300+i*80,180);
    }

    //?????Â∏•?ÊΩ???Ëåµ?ËÖìÂ?¥?????0???~???9???
    for(int i=0; i<=9; i++){
        rectangleColorRect(game.render,&buttonPos[game.scene][i+1],0xff000000);
        textImg[tname_0+i].drawTexture(buttonPos[game.scene][i+1].x +10,buttonPos[game.scene][i+1].y-20);
    }

    //?????Â∏•?ÊΩ???Ëåµ?ËÖìÂ?¥?????X???
    rectangleColorRect(game.render,&buttonPos[game.scene][11],0xff000000);
    textImg[tname_del].drawTexture(buttonPos[game.scene][11].x +10,buttonPos[game.scene][11].y-20);
    
    /* ?????Ëã•??????Âû©??5ÁΩ????„É•?????????????? */
    if(strlen(game.port)>=4){
        //?????Â∏•?ÊΩ???Ëåµ?ËÖìÂ?¥?????ËÖ?Èò™?????
        rectangleColorRect(game.render,&buttonPos[game.scene][12],0xff000000);
        textImg[tname_enter].drawTexture(buttonPos[game.scene][12].x +10,buttonPos[game.scene][12].y-20);
    }

    //??Âæ°????ÂÄ∂?????Ëåµ?ËÖ??
    if(game.selectButton != 0)
        boxColorRect(game.render,&buttonPos[game.scene][game.selectButton],0xaa666666);

    SDL_RenderPresent(game.render);
}

/* ????????„Ç??Âê????Âû∏?„É•???????ËÑ??? */
void RenderDeviceNumWindow(void){
    //??????
    DrawBackGround();

    //back
    if(game.selectButton == 0){
        uiImg[uname_backButton].drawTexture(buttonPos[SCENE_CLIENT_0][0].x-20,buttonPos[SCENE_CLIENT_0][0].y-20,buttonPos[SCENE_CLIENT_0][0].w+40,buttonPos[SCENE_CLIENT_0][0].h+40);
    }
    else{
        uiImg[uname_backButton].drawTexture(buttonPos[SCENE_CLIENT_0][0].x,buttonPos[SCENE_CLIENT_0][0].y,buttonPos[SCENE_CLIENT_0][0].w,buttonPos[SCENE_CLIENT_0][0].h);
    }
    
    // ???Áµ?????Ëåµ?ËÖìÂ?¥?????????????„Ç??Âä´???????„É•???????????????????????
    uiImg[uname_rankingBoard].drawTexture(100,220,uiImg[uname_rankingBoard].w*1.44,uiImg[uname_rankingBoard].h*0.2);
    uiImg[uname_inputDeviceNum].drawTexture(160,225,uiImg[uname_inputDeviceNum].w*2,uiImg[uname_inputDeviceNum].h*2);

    // ???Áµ?????Ëåµ?ËÖìÂ?¥?????clpc???
    boxColor(game.render,220,350,980,470,0xffffffff);
    textImg[tname_clpc].drawTexture(300,340);

    //??„É•????????????????„Ç??Âê????Âû©??Ëåµ?ËÖ??
    for(int i=0; game.deviceNum[i] != '\0'; i++)
    {
        textImg[tname_0+game.deviceNum[i]-'0'].drawTexture(500+i*80,250);
    }

    //?????Â∏•?ÊΩ???Ëåµ?ËÖìÂ?¥?????0???~???9???
    for(int i=0; i<=9; i++){
        rectangleColorRect(game.render,&buttonPos[SCENE_CLIENT_0][i+1],0xff000000);
        textImg[tname_0+i].drawTexture(buttonPos[SCENE_CLIENT_0][i+1].x +10,buttonPos[SCENE_CLIENT_0][i+1].y-20);
    }

    //?????Â∏•?ÊΩ???Ëåµ?ËÖìÂ?¥?????X???
    rectangleColorRect(game.render,&buttonPos[SCENE_CLIENT_0][11],0xff000000);
    textImg[tname_del].drawTexture(buttonPos[SCENE_CLIENT_0][11].x +10,buttonPos[SCENE_CLIENT_0][11].y-20);
    
    //localHost?????Â∏•?ÊΩ???Ëåµ?ËÖ??
    rectangleColorRect(game.render,&buttonPos[SCENE_CLIENT_0][12],0xff000000);
    textImg[tname_self].drawTexture(buttonPos[SCENE_CLIENT_0][12].x +10,buttonPos[SCENE_CLIENT_0][12].y-20,textImg[tname_self].w/2,textImg[tname_self].h/2);

    /* ????????„Ç??Âê????Âû©??3ÁΩ????„É•?????????????? */
    if(strlen(game.deviceNum)>=3){
        //?????Â∏•?ÊΩ???Ëåµ?ËÖìÂ?¥?????ËÖ?Èò™?????
        rectangleColorRect(game.render,&buttonPos[SCENE_CLIENT_0][13],0xff000000);
        textImg[tname_enter].drawTexture(buttonPos[SCENE_CLIENT_0][13].x +10,buttonPos[SCENE_CLIENT_0][13].y-20);
    }

    //??Âæ°????ÂÄ∂?????Ëåµ?ËÖ??
    if(game.selectButton != 0)
        boxColorRect(game.render,&buttonPos[SCENE_CLIENT_0][game.selectButton],0xaa666666);

    SDL_RenderPresent(game.render);
}

/* ?????È¥ª?Â∏•????„Ç??Ê¶??ËÑ??‚â™???????? */
void RenderCustomizeWindow(void){
    
    //?????Â∏•?ÊΩ?????Ë¥?????Ë¥???
    buttonPos[SCENE_CUSTOMIZE][1].y = 57-scrollValue;
    buttonPos[SCENE_CUSTOMIZE][1].y = 130-scrollValue;
    buttonPos[SCENE_CUSTOMIZE][2].y = 370-scrollValue;
    buttonPos[SCENE_CUSTOMIZE][3].y = 700-scrollValue;
    int buttonSize[maxButtonNum[game.scene]];
    for(int i=0; i<maxButtonNum[game.scene]; i++){
        buttonSize[i] = (i==game.selectButton) ? 1 : 0;
    }

    //????????Èß???Ëó•??????Èß???
    /*
    SDL_SetRenderDrawColor(game.render,253,245,230,255);
    SDL_RenderClear(game.render);*/
    //??????
    DrawBackGround();

    // ???Áµ?????Ëåµ?ËÖìÂ?¥?????CUSTOMIZE???
    textImg[tname_customize].drawTexture(285,25-scrollValue,textImg[tname_customize].w*0.95,textImg[tname_customize].h*0.95);
    boxColor(game.render,814,14-scrollValue,1180,120-scrollValue,0xffe6f5fd);//Ëó•??????Ëö??????ËÖ?????
    uiImg[uname_explain_skill].drawTexture(815,10-scrollValue,uiImg[uname_explain_skill].w/1.5,uiImg[uname_explain_skill].h/1.5);//ËåØ????

    // ??Á•????????Â∏•??
    uiImg[uname_backButton].drawTexture(30-buttonSize[0]*20,45-buttonSize[0]*20-scrollValue,uiImg[uname_backButton].w/1.2+buttonSize[0]*40,uiImg[uname_backButton].h/1.2+buttonSize[0]*40);

    
    // ??Á≥∏?????Ëåµ?ËÖìÂ?¥?????????????„É•??ÁΩ®????
    uiImg[uname_name].drawTexture(81,158-scrollValue,uiImg[uname_name].w/1.2,uiImg[uname_name].h/1.2);
    
    
    // ???Áµ?????Ëåµ?ËÖìÂ?¥??????????????
    for(int i=0; game.clientName[i]!='\0'; i++){
            textName tn = retTextNameFromChar( game.clientName[i]);
            textImg[tn].drawTexture(370+ i*27,160-scrollValue,textImg[tn].w/1.8,textImg[tn].h/1.8);
    }
    // ??Á≥∏?????Ëåµ?ËÖìÂ?¥?????Á¥????Áúº????Â∏•?ÊΩ???
    uiImg[uname_change].drawTexture(buttonPos[SCENE_CUSTOMIZE][1].x-(buttonSize[1]*25),buttonPos[SCENE_CUSTOMIZE][1].y-(buttonSize[1]*25),(buttonSize[1]*50)+uiImg[uname_change].w/4,(buttonSize[1]*50)+uiImg[uname_change].h/4);

    // ??Á≥∏?????Ëåµ?ËÖìÂ?¥???????È¥ª?„è???ÁΩ®????
    uiImg[uname_skill].drawTexture(81,270-scrollValue,uiImg[uname_skill].w/1.2,uiImg[uname_skill].h/1.2);
    // ??Á≥∏?????Ëåµ?ËÖìÂ?¥???????Âæ°????????????????È¥ª?„è??????
    uiImg[uname_skill_attack+(int)game.skill].drawTexture(160,370-scrollValue,uiImg[uname_skill_attack+(int)game.skill].w/1.4,uiImg[uname_skill_attack+(int)game.skill].h/1.4);
    // ??Á≥∏?????Ëåµ?ËÖìÂ?¥???????Âæ°??Á≠ù„èç??
    uiImg[uname_nowselect].drawTexture(90,350-scrollValue,uiImg[uname_nowselect].w/4,uiImg[uname_nowselect].h/4);
    // ??Á≥∏?????Ëåµ?ËÖìÂ?¥?????Á¥????Áúº????Â∏•?ÊΩ???
    uiImg[uname_change].drawTexture(buttonPos[SCENE_CUSTOMIZE][2].x-(buttonSize[2]*25),buttonPos[SCENE_CUSTOMIZE][2].y-(buttonSize[2]*25),(buttonSize[2]*50)+uiImg[uname_change].w/4,(buttonSize[2]*50)+uiImg[uname_change].h/4);

    // ??Á≥∏?????Ëåµ?ËÖìÂ?¥???????È¥ª????ÊøÄ??Ω???ÁΩ®????
    uiImg[uname_special].drawTexture(81,560-scrollValue,uiImg[uname_special].w/1.2,uiImg[uname_special].h/1.2);
    // ??Á≥∏?????Ëåµ?ËÖìÂ?¥???????Âæ°????????????????È¥ª????ÊøÄ??Ω??????
    uiImg[uname_barrier+(int)game.special].drawTexture(160,685-scrollValue,uiImg[uname_barrier+(int)game.special].w/1.4,uiImg[uname_barrier+(int)game.special].h/1.4);//??È¥ª????ÊøÄ??Ω??????ËåØ????
    // ??Á≥∏?????Ëåµ?ËÖìÂ?¥???????Âæ°??Á≠ù„èç??
    uiImg[uname_nowselect].drawTexture(90,650-scrollValue,uiImg[uname_nowselect].w/4,uiImg[uname_nowselect].h/4);
    // ??Á≥∏?????Ëåµ?ËÖìÂ?¥?????Á¥????Áúº????Â∏•?ÊΩ???
    uiImg[uname_change].drawTexture(buttonPos[SCENE_CUSTOMIZE][3].x-(buttonSize[3]*25),buttonPos[SCENE_CUSTOMIZE][3].y-(buttonSize[3]*25),(buttonSize[3]*50)+uiImg[uname_change].w/4,(buttonSize[3]*50)+uiImg[uname_change].h/4);

    // ??Á≥∏?????Ëåµ?ËÖìÂ?¥???????È¥ª????Ëã•?Â∏•?Âè∑?????
    uiImg[uname_status].drawTexture(81,902-scrollValue,uiImg[uname_status].w/1.2,uiImg[uname_status].h/1.2);
    // ??Á≥∏?????Ëåµ?ËÖìÂ?¥??????????ÊΩ???Ëåµ?ËÖìÂ?¥??
    uiImg[uname_pin].drawTexture(parm_x[game.parm[0]+5]-(buttonSize[4]*10) ,1005-scrollValue-(buttonSize[4]*10),uiImg[uname_pin].w/5+(buttonSize[4]*20),uiImg[uname_pin].h/5+(buttonSize[4]*20));
    uiImg[uname_pin].drawTexture(parm_x[game.parm[1]+5]-(buttonSize[5]*10) ,1140-scrollValue-(buttonSize[5]*10),uiImg[uname_pin].w/5+(buttonSize[5]*20),uiImg[uname_pin].h/5+(buttonSize[5]*20));
    uiImg[uname_pin].drawTexture(parm_x[game.parm[2]+5]-(buttonSize[6]*10) ,1277-scrollValue-(buttonSize[6]*10),uiImg[uname_pin].w/5+(buttonSize[6]*20),uiImg[uname_pin].h/5+(buttonSize[6]*20));
    uiImg[uname_pin].drawTexture(parm_x[game.parm[3]+5]-(buttonSize[7]*10) ,1415-scrollValue-(buttonSize[7]*10),uiImg[uname_pin].w/5+(buttonSize[7]*20),uiImg[uname_pin].h/5+(buttonSize[7]*20));
    uiImg[uname_pin].drawTexture(parm_x[game.parm[4]+5]-(buttonSize[8]*10) ,1545-scrollValue-(buttonSize[8]*10),uiImg[uname_pin].w/5+(buttonSize[8]*20),uiImg[uname_pin].h/5+(buttonSize[8]*20));
    // ??Âåª?„Ç???Ëåµ?ËÖ??
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
    // ??È¥ª????Ëã•?Â∏•?È¥ª????????º??Ëã•?Â∏•?????Ëç????Âåª?„Ç???Ëåµ?ËÖ??
    int rest = PARAMATER_SUM_MAX - retSumParamater();
    textImg[tname_0+rest/10].drawTexture(840,1675-scrollValue,textImg[tname_0+rest/10].w/1.2,textImg[tname_0+rest/10].h/1.2);
    textImg[tname_0+rest%10].drawTexture(885,1675-scrollValue,textImg[tname_0+rest%10].w/1.2,textImg[tname_0+rest%10].h/1.2);

    //???????????„É•????ËÑ???
    if(game.popScene == PopUp_Name){
        uiImg[uname_back].drawTexture(0,0); //Á∑????????????????????
        uiImg[uname_nameChange].drawTexture(0,0);
        for(int i=0; tempName[i]!='\0'; i++){
            textName tn = retTextNameFromChar(tempName[i]);
            textImg[tn].drawTexture(200+ i*45,310);
        }
    }
    //??È¥ª?„è???Á¥????Èõ??ËÑ???
    else if(game.popScene == PopUp_Skill){
        uiImg[uname_back].drawTexture(0,0); //Á∑????????????????????
        uiImg[uname_skillChange].drawTexture(300,50);//??Â∏•?„Ç??????
        boxColor(game.render,860,10,1190,110,0xffe6f5fd);
        uiImg[uname_explain_skill].drawTexture(860,10,uiImg[uname_explain_skill].w/1.6,uiImg[uname_explain_skill].h/1.6);//ËåØ????
        uiImg[uname_skill_attack+(int)game.skill].drawTexture(200,200,uiImg[uname_skill_attack+(int)game.skill].w/1.2,uiImg[uname_skill_attack+(int)game.skill].h/1.2);//??Âæ°??Á≠ù„èç????È¥ª?„è???
        uiImg[uname_nowselect].drawTexture(170,170,uiImg[uname_nowselect].w/4,uiImg[uname_nowselect].h/4);//??Âæ°??Á≠??
        boxColor(game.render,150,370,1100,890,0xffe6f5fd);//Ëó•??????Ëö??????ËÖ?????
        for(int i=0; i<SKILL_NUM; i++){
            uiImg[uname_skill_attack+i].drawTexture(300,400+i*150,uiImg[uname_skill_attack+i].w/1.4,uiImg[uname_skill_attack+i].h/1.4);
        }
        if(game.selectButton_sub != 0){
            uiImg[uname_backButton].drawTexture(30,65,uiImg[uname_backButton].w/1.1,uiImg[uname_backButton].h/1.1);//??Á•????????Â∏•??
            uiImg[uname_selectHikouki].drawTexture(170,380+(game.selectButton_sub-1)*150,uiImg[uname_selectHikouki].w/1.3,uiImg[uname_selectHikouki].h/1.3);//??Âæ°???????????????????????ËÖìÂ?¥??Ëï??Ëåµ?ÁΩ??
        }
        else{
            uiImg[uname_backButton].drawTexture(23,58,uiImg[uname_backButton].w,uiImg[uname_backButton].h);//??Á•????????Â∏•??
            boxColor(game.render,30,73,270,150,0xaa666666);
        }
    }
    //??È¥ª????ÊøÄ??Ω???Á¥????Áúº????ËÑ???
    else if(game.popScene == PopUp_Special){
        uiImg[uname_back].drawTexture(0,0); //Á∑????????????????????
        uiImg[uname_specialChange].drawTexture(300,50);//??Â∏•?„Ç??????
        boxColor(game.render,860,10,1190,110,0xffe6f5fd);//ËåØ?????????????
        uiImg[uname_explain_skill].drawTexture(860,10,uiImg[uname_explain_skill].w/1.6,uiImg[uname_explain_skill].h/1.6);//ËåØ????
        boxColor(game.render,180,280,1020,580,0xffe6f5fd);//??È¥ª????ÊøÄ??Ω?????‚â™?„Ç??ÊΩ??ÊΩ?????????
        for(int i=0;i<SPECIAL_NUM;i++){
            //?????Ëã•?Á¥????????????Ω???????????????Á¥ä—??????
            if(i==game.selectButton_sub-1){
                uiImg[uname_barrier_icon+i].drawTexture(special_iconPos[i+1].x-10,special_iconPos[i+1].y-10,uiImg[uname_barrier_icon+(int)game.special].w/2,uiImg[uname_barrier_icon+(int)game.special].h/2);
            }
            else{
                uiImg[uname_barrier_icon+i].drawTexture(special_iconPos[i+1].x,special_iconPos[i+1].y,uiImg[uname_barrier_icon+(int)game.special].w/2.4,uiImg[uname_barrier_icon+(int)game.special].h/2.4);
            }   
        }
        if(game.selectButton_sub != 0){
            uiImg[uname_backButton].drawTexture(special_iconPos[0].x+7,special_iconPos[0].y+7,uiImg[uname_backButton].w/1.1,uiImg[uname_backButton].h/1.1);//??Á•????????Â∏•??
            uiImg[uname_barrier+game.selectButton_sub-1].drawTexture(180,600,uiImg[uname_barrier+game.selectButton_sub-1].w*0.75,uiImg[uname_barrier+game.selectButton_sub-1].h*0.75);//??È¥ª????ÊøÄ??Ω??????ËåØ????
        }
        else{
            uiImg[uname_backButton].drawTexture(special_iconPos[0].x,special_iconPos[0].y,uiImg[uname_backButton].w,uiImg[uname_backButton].h);//??Á•????????Â∏•??
            boxColor(game.render,30,73,270,150,0xaa666666);
        }
        uiImg[uname_nowselect].drawTexture(special_iconPos[(int)game.special+1].x-40,special_iconPos[(int)game.special+1].y-40,uiImg[uname_nowselect].w/5,uiImg[uname_nowselect].h/5);//??Âæ°??Á≠??
    }

    SDL_RenderPresent(game.render);
}

void RenderClientWaitWindow(void){
    //??????
    DrawBackGround();

    textImg[tname_nowloading].drawTexture(100,100,textImg[tname_nowloading].w/1.2,textImg[tname_nowloading].h/1.2);

    SDL_RenderPresent(game.render);
}

void RenderResultWindow(void){
    //??????
    DrawBackGround();

    textImg[tname_result].drawTexture(55,40,textImg[tname_result].w/1.2,textImg[tname_result].h/1.3);
    //(game.selectButton==0?1:0)
    uiImg[uname_rankingBoard].drawTexture(65, 160, uiImg[uname_rankingBoard].w*1.5, uiImg[uname_rankingBoard].h*1.25);
    uiImg[uname_rightSelect].drawTexture(1095+(game.selectButton==1?5:1),430-(game.selectButton==1?20:1),uiImg[uname_rightSelect].w/8*(game.selectButton==1?1.4:1),uiImg[uname_rightSelect].h/8*(game.selectButton==1?1.4:1));
    uiImg[uname_leftSelect].drawTexture(25-(game.selectButton==0?30:1),430-(game.selectButton==0?20:1),uiImg[uname_leftSelect].w/8*(game.selectButton==0?1.4:1),uiImg[uname_leftSelect].h/8*(game.selectButton==0?1.4:1));
    textImg[tname_score_rank+rankingMode].drawTexture(285,170,textImg[tname_score_rank+rankingMode].w/1.3,textImg[tname_score_rank+rankingMode].h/1.3);
    for(int i=0; i<game.clientNum; i++)
    {
        uiImg[uname_rankingBack].drawTexture(120, 270+i*130, uiImg[uname_rankingBack].w*1.45, uiImg[uname_rankingBack].h*1.1);
    }
    for(int i=0; i<game.clientNum; i++){
        switch(rankNumber[i]){
            case 1:
                textImg[tname_1].drawTexture(160 ,270 +i*130,textImg[tname_1].w/1.3, textImg[tname_1].h/1.3);
                textImg[tname_s].drawTexture(195 ,270 +i*130,textImg[tname_s].w/1.3, textImg[tname_s].h/1.3);
                textImg[tname_t].drawTexture(230 ,270 +i*130,textImg[tname_t].w/1.3, textImg[tname_t].h/1.3);
                break;
            case 2:
                textImg[tname_2].drawTexture(160 ,270 +i*130,textImg[tname_2].w/1.3, textImg[tname_2].h/1.3);
                textImg[tname_n].drawTexture(195 ,270 +i*130,textImg[tname_n].w/1.3, textImg[tname_n].h/1.3);
                textImg[tname_d].drawTexture(230 ,270 +i*130,textImg[tname_d].w/1.3, textImg[tname_d].h/1.3);
                break;
            case 3:
                textImg[tname_3].drawTexture(160 ,270 +i*130,textImg[tname_3].w/1.3, textImg[tname_3].h/1.3);
                textImg[tname_r].drawTexture(195 ,270 +i*130,textImg[tname_r].w/1.3, textImg[tname_r].h/1.3);
                textImg[tname_d].drawTexture(230 ,270 +i*130,textImg[tname_d].w/1.3, textImg[tname_d].h/1.3);
                break;
            case 4:
                textImg[tname_4].drawTexture(160 ,270 +i*130,textImg[tname_4].w/1.3, textImg[tname_4].h/1.3);
                textImg[tname_t].drawTexture(195 ,270 +i*130,textImg[tname_t].w/1.3, textImg[tname_t].h/1.3);
                textImg[tname_h].drawTexture(230 ,270 +i*130,textImg[tname_h].w/1.3, textImg[tname_h].h/1.3);
                break;
        }
        for(int j=0; game.rankingData.clientName[rankIndex[i]][j]!='\0'; j++){
            textName tn = retTextNameFromChar(game.rankingData.clientName[rankIndex[i]][j]);
            textImg[tn].drawTexture(455+ j*30, 275+i*130,textImg[tn].w/1.4,textImg[tn].h/1.4);
        }
        switch(rankingMode){
            case 0: // score
                {
                    char text[20];
                    sprintf(text,"%05.0f",game.rankingData.score[rankIndex[i]]);
                    for(int j=0; text[j] != '\0'; j++){
                        textName tn = retTextNameFromChar(text[j]);
                        textImg[tn].drawTexture(870+ j*30, 270+i*130,textImg[tn].w/1.3,textImg[tn].h/1.3);
                    }
                }
                break;
            case 1: // kill num of player
                {
                    char text[20];
                    sprintf(text,"%02d",game.rankingData.kill_player[rankIndex[i]]);
                    for(int j=0; text[j] != '\0'; j++){
                        textName tn = retTextNameFromChar(text[j]);
                        textImg[tn].drawTexture(960+ j*30, 270+i*130,textImg[tn].w/1.3,textImg[tn].h/1.3);
                    }
                }
                break;
            case 2: // death
                {
                    char text[20];
                    sprintf(text,"%02d",game.rankingData.death[rankIndex[i]]);
                    for(int j=0; text[j] != '\0'; j++){
                        textName tn = retTextNameFromChar(text[j]);
                        textImg[tn].drawTexture(960+ j*30, 270+i*130,textImg[tn].w/1.3,textImg[tn].h/1.3);
                    }
                }
                break;
            case 3: // kill num of enemy  and boss
                {
                    char text[20];
                    sprintf(text,"%02d",game.rankingData.kill_enemy[rankIndex[i]]+game.rankingData.kill_boss[rankIndex[i]]);
                    for(int j=0; text[j] != '\0'; j++){
                        textName tn = retTextNameFromChar(text[j]);
                        textImg[tn].drawTexture(960+ j*30, 270+i*130,textImg[tn].w/1.3,textImg[tn].h/1.3);
                    }
                }
                break;
        }
        
    }

    boxColorRect(game.render,&buttonPos[SCENE_Result][2],0xffffffff);
    rectangleColorRect(game.render,&buttonPos[SCENE_Result][2],0xff000000);
    textImg[tname_exit].drawTexture(buttonPos[SCENE_Result][2].x+20,buttonPos[SCENE_Result][2].y, textImg[tname_exit].w/1.6, textImg[tname_exit].h/1.6);

    boxColorRect(game.render,&buttonPos[SCENE_Result][3],0xffffffff);
    rectangleColorRect(game.render,&buttonPos[SCENE_Result][3],0xff000000);
    textImg[tname_detail].drawTexture(buttonPos[SCENE_Result][3].x+20,buttonPos[SCENE_Result][3].y, textImg[tname_detail].w/1.6, textImg[tname_detail].h/1.6);

    boxColorRect(game.render,&buttonPos[SCENE_Result][4],0xffffffff);
    rectangleColorRect(game.render,&buttonPos[SCENE_Result][4],0xff000000);
    textImg[tname_totitle].drawTexture(buttonPos[SCENE_Result][4].x+20,buttonPos[SCENE_Result][4].y, textImg[tname_totitle].w/1.6, textImg[tname_totitle].h/1.6);

    if(2 <= game.selectButton && game.selectButton <= 4)
        boxColorRect(game.render,&buttonPos[SCENE_Result][game.selectButton],0x99555555);

    SDL_RenderPresent(game.render);
}

/* ????????È¥ª?????Ω????????? */
void ImgInfo::drawTexture(int x, int y, int width, int height){
    if(width == 0 || height == 0){
        width=w;
        height=h;
    }
    // Ëçµ‚à´?????Ëçê„ä§??
    SDL_Rect src = {0,0,w,h};
    // Ëçµ‚à´?????Ëçê„ä§??
    SDL_Rect dst = {x, y, width, height};
    // Ëçµ‚à´??
    if (0 > SDL_RenderCopy(game.render, texture, &src, &dst)) {
        PrintError(SDL_GetError());
    }
}
/* ????????È¥ª?????Ω????????? */
void ImgInfo::drawRotateTexture(int x, int y, int width, int height, int rad, SDL_RendererFlip mode){
    if(width == 0 || height == 0){
        width=w;
        height=h;
    }
    // Ëçµ‚à´?????Ëçê„ä§??
    SDL_Rect src = {0,0,w,h};
    // Ëçµ‚à´?????Ëçê„ä§??
    SDL_Rect dst = {x, y, width, height};
    // Ëçµ‚à´??
    if (0 > SDL_RenderCopyEx(game.render, texture, &src, &dst,rad, NULL, mode)) {
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
    //??????
    uiImg[uname_cloud].drawTexture((int)scroll_back,0,uiImg[uname_cloud].w,uiImg[uname_cloud].h);
    if(scroll_back <= -uiImg[uname_cloud].w/2){
        scroll_back = 0;
    }
    uiImg[uname_castle].drawTexture(0,0,uiImg[uname_castle].w,uiImg[uname_castle].h);


    if(strcmp(tempName,"masao")==0||strcmp(tempName,"Masao")==0||strcmp(tempName,"MASAO")==0){
        masao_loop += M_PI/180;
        if(masao_loop >= 2*M_PI){
            masao_loop = 0;
        }
        
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
}

/*
    //??Á≥∏???????????
    // Ëçµ‚à´?????Ëçê„ä§??
    src = {0,0,uiImg[0].w, uiImg[0].h};
    // Ëçµ‚à´?????Ëçê„ä§??
    dst = {0,0,uiImg[0].w, uiImg[0].h};
    // Ëçµ‚à´??
    if (0 > SDL_RenderCopy(game.render, uiImg[0].texture, &src, &dst)) {
        PrintError(SDL_GetError());
    }

*/

/*
    // ???Áµ?????Ëåµ?ËÖìÂ?¥????????
    src = {0,0,textImg[0].w,textImg[0].h};
    dst = {0,0,textImg[0].w,textImg[0].h};
    if (0 > SDL_RenderCopy(game.render, textImg[0].texture, &src, &dst)) {
        PrintError(SDL_GetError());
    }
*/
