#include "header.h"

/* ??¢æ?? */
static Uint32 AniTimer(Uint32 interval, void* param);
static Scene InputEvent(void);
static Scene InputNameEvent(void);
void ExcuteInput(void);

bool isReadRankingflg = false;//isread rannkinng
settingGame game;
char tempName[NAME_MAX_LENGTH+1];

/* main */
int main(int argc, char* argv[])
{
    char ch[16];
    switch (argc) {
    case 1:
        break;
    case 2:
        sprintf(ch, "%s", argv[1]);
        if(strcmp(ch,"title") == 0)
        {
            game.scene = SCENE_Title;
        }
        else if(strcmp(ch,"result") == 0){
            game.scene = SCENE_Result;
        }
        break;
    default:
        return 1;
    }
    /** ??????????????? **/
    /* ??·ã?¹ã????????????????? */
    InitSystem();
    /* SDL */
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        return PrintError(SDL_GetError());
    }
    /** UI????????? **/
    if (InitWindow() < 0) {
        PrintError("failed to initialize Windows");
        EndProgram();
        return 0;
    }


    /* ç°¡æ??????????¼ã??????????³ã??
     *  ??¡ã?¤ã?³å?????????????¼ã???????°ã??????????³ã?????ï¼?
     *  ??¿ã?¤ã????¼å???????????1????????¼ã???????????ç°¡æ??è¨?ç®???????
     */
    int framecnt = 0;
    /* //??¿ã?¤ã????¼èµ·???
    SDL_TimerID atimer = SDL_AddTimer(100, AniTimer, &framecnt);
    if (atimer == 0) {
        PrintError(SDL_GetError());
        goto DESTROYALL;
    }*/


    /* ??¡ã?¤ã?³ã????¼ã?? */
    while (game.scene != SCENE_None) {

        /* ??¿ã?¤ã???????»é?? */
        while(game.scene == SCENE_Title){
            game.scene = InputEvent();
            ExcuteInput();
            /** ?????»å????? **/
            RenderTitleWindow();

            /* å°????å¾????
            *  PC??°å??????????£ã????????(??????)????????????èª¿æ?´ã?????????????????
            *  ?????¤ã??????????¾ã?????ï¼????????????°æ??????????§å??ä½??????????????????????
            *  æ¶????????????¹ã???????????????????¾ã??
            */
            SDL_Delay(10);
            /* ????????¼ã??????????³ã?? */
            framecnt++;
        }

        /* ??µã?¼ã?????0 */
        while(game.scene == SCENE_SERVER_0){
            game.scene = InputEvent();
            ExcuteInput();
            /** ?????»å????? **/
            RenderServerWindow_0();
            SDL_Delay(10);
            framecnt++;
        }     

        /* ??µã?¼ã?????1 ????????¤ã?¢ã?³ã??1 ?????¹ã?³ã?¼ã?? */
        while(game.scene == SCENE_SERVER_1 || game.scene == SCENE_CLIENT_1){
            game.scene = InputEvent();
            ExcuteInput();
            /** ?????»å????? **/
            if(game.scene == SCENE_SERVER_1 || game.scene == SCENE_CLIENT_1 )
                RenderPasscodeWindow();
            SDL_Delay(10);
            framecnt++;
        }       

        /*????????¤ã?¢ã?³ã??0 */
        while(game.scene == SCENE_CLIENT_0){
            game.scene = InputEvent();
            ExcuteInput();
            /** ?????»å????? **/
            RenderDeviceNumWindow();
            SDL_Delay(10);
            framecnt++;
        }

        /* ????????¤ã?¢ã?³ã?? ?????¹ã?¿ã????¤ã?ºç?»é?? */
        while(game.scene == SCENE_CUSTOMIZE){
            if(game.popScene != PopUp_Name)
            {
                game.scene = InputEvent();
                ExcuteInput();
            }
            else{
                game.scene = InputNameEvent();
            }
                
            /** ?????»å????? **/            
            RenderCustomizeWindow();
            SDL_Delay(10);
            framecnt++;
        }
        /*????????¤ã?¢ã?³ã??å¾?æ©???»é?? */
        while(game.scene == SCENE_CLIENT_WAIT){
            game.scene = InputEvent();
            ExcuteInput();
            ReadMatchFile();
            /** ?????»å????? **/
            RenderClientWaitWindow();
            SDL_Delay(10);
            framecnt++;
        }
        
        /*Result */
        while(game.scene == SCENE_Result){
            if(!isReadRankingflg){
                ReadRankingFile();
                isReadRankingflg = true;
                    /*?????³ã?­ã?³ã?°ã??ä½¿ç??*/
                rankIndex = (int*)malloc(sizeof(int)*game.clientNum);
                rankNumber = (int*)malloc(sizeof(int)*game.clientNum);
                SortRanking(rankingMode);
            }
            game.scene = InputEvent();
            ExcuteInput();
            /** ï¿½ï¿½ï¿½ï¿½ï¿½ç³¸ï¿½ï¿½ï¿½ï¿½ï¿? **/
            RenderResultWindow();
            SDL_Delay(10);
            framecnt++;
        }
    }
    system("exit");
    return 0;
}

/* ????????¼ã?¡ã????»ã?¼ã?¸è¡¨ç¤?
 *
 * å¼????
 *   str: ????????¼ã?¡ã????»ã?¼ã??
 *
 * è¿????: -1
 */
int PrintError(const char* str)
{
    fprintf(stderr, "%s\n", str);
    return -1;
}

void EndProgram(void){
    /** çµ?äº??????? **/
    //SDL_RemoveTimer(atimer);
    DestroyWindow();
    SDL_Quit();

}

/* //??¿ã?¤ã????¼å?????(??¢ã????¡ã?¼ã?·ã?§ã?³ã????´æ??)
Uint32 AniTimer(Uint32 interval, void* param)
{??????
     //??????å¢?????????´æ??
      //1????????¼ã??????????????ç°¡æ??è¨?ç®???????
    if (*(int*)param > 0) {
        gGame.timeStep = 0.1 / *(int*)param;
        printf("FPS: %d\r", *(int*)param * 10);
        *(int*)param = 1;
    }

    //è»¢é?????ç¯???²ã????´æ??(??¢ã????¡ã?¼ã?·ã?§ã??) 
    for (int i = 0; i < gCharaNum; i++) {
        /* ??¢ã????¡ã?¼ã?·ã?§ã?³ã????¿ã?¼ã?³ã????´æ?? 
        gChara[i].anipat = (gChara[i].anipat + 1) % (gChara[i].img->anipatnum);
    }

    return interval;
}*/

/* ??¥å????¤ã????³ã?????èª­ã?¿å????? */
Scene InputEvent(void)
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            /** çµ?äº????ç¤? **/
            return SCENE_None;
        case SDL_KEYDOWN:
            /** ??­ã?¼ã????¼ã??????????¹å?????è¨­å?? **/
            switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
                game.input = INPUT_ESCAPE;
                break;
            case SDLK_RSHIFT:
            case SDLK_LSHIFT:
                game.isShift = true;
                break;
            case SDLK_UP:
                game.input = INPUT_UP;
                break;
            case SDLK_DOWN:
                game.input = INPUT_DOWN;
                break;
            case SDLK_RIGHT:
                game.input = INPUT_RIGHT;
                break;
            case SDLK_LEFT:
                game.input = INPUT_LEFT;
                break;
            case SDLK_RETURN:
                game.input = INPUT_RETURN;
                break;
            case SDLK_BACKSPACE:
                game.input = INPUT_BACKSPACE;
                break;
            default:
                //??°å????­ã??
                if(SDLK_0 <= event.key.keysym.sym && event.key.keysym.sym <= SDLK_9){
                    game.input = static_cast<INPUT>(event.key.keysym.sym - SDLK_0 + INPUT_0);
                    std::cout << game.input<<'\n';
                }
                break;
            }
            break;
        case SDL_KEYUP:
            /** ??­ã?¼ã????¼ã??????????¹å?????è¨­å?? **/
            switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
                game.input = INPUT_NONE;
                break;
            case SDLK_RSHIFT:
            case SDLK_LSHIFT:
                game.isShift = false;
                break;
            case SDLK_UP:
                game.input = INPUT_NONE;
                break;
            case SDLK_DOWN:
                game.input = INPUT_NONE;
                break;
            case SDLK_RIGHT:
                game.input = INPUT_NONE;
                break;
            case SDLK_LEFT:
                game.input = INPUT_NONE;
                break;
            case SDLK_BACKSPACE:
                game.input =INPUT_NONE;
                break;
            default:
                //??°å????­ã??
                if(SDLK_0 <= event.key.keysym.sym && event.key.keysym.sym <= SDLK_9){
                    game.input = INPUT_NONE;
                }
                break;
            }
            break;
        }
    }
    return game.scene;
}

/* ??¥å????¤ã????³ã?????èª­ã?¿å????? */
Scene InputNameEvent(void)
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            /** çµ?äº????ç¤? **/
            return SCENE_None;
        case SDL_KEYDOWN:
            if (event.key.repeat)
                break;
            /** ??­ã?¼ã????¼ã??????????¹å?????è¨­å?? **/
            switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
                sprintf(tempName,"%s",game.clientName);//å¤???´å??????????????ä¿?å­?
                game.popScene = PopUp_None;
                break;
            case SDLK_BACKSPACE:
                if(strlen(tempName)>0){
                    tempName[strlen(tempName)-1] = '\0';
                }
                break;
            case SDLK_RETURN:
                sprintf(game.clientName,"%s",tempName);//å¤???´ã??????????????ä¿?å­?
                SaveGameData(); //??²ã?¼ã???????¼ã?¿ã??ä¿?å­?
                game.popScene = PopUp_None;
                break;
            case SDLK_RSHIFT:
            case SDLK_LSHIFT:
                game.isShift = true;
                break;
            case SDLK_DELETE:
                for(int i=0; tempName[i] != '\0'; i++){
                    tempName[i] = '\0';
                }
                break;
            default:
                if(strlen(tempName) < NAME_MAX_LENGTH){
                    char ch = event.key.keysym.sym;
                    if('a'<=ch && ch<='z' && game.isShift)
                    {
                        ch += 'A' - 'a';
                        tempName[strlen(tempName)] = ch;
                        tempName[strlen(tempName)+1] = '\0';
                    }
                    else if(('0'<=ch && ch<='9') || ('a'<=ch && ch<='z')){
                        tempName[strlen(tempName)] = ch;
                        tempName[strlen(tempName)+1] = '\0';
                    }
                    else if(ch == ' '){
                        tempName[strlen(tempName)] = ch;
                        tempName[strlen(tempName)+1] = '\0';
                    }
                    
                }
                break;
            }
            break;
        case SDL_KEYUP:
            switch(event.key.keysym.sym){
                case SDLK_RSHIFT:
                case SDLK_LSHIFT:
                    game.isShift = false;
                    break;
            }
            break;
        }
    }
    return game.scene;
}

void ExcuteInput(void){
    switch(game.scene){
        case SCENE_Title:
            switch(game.input){
                case INPUT_UP:
                    shiftSelect(-1,maxButtonNum[game.scene],&game.selectButton);
                    break;
                case INPUT_DOWN:
                    shiftSelect(+1,maxButtonNum[game.scene],&game.selectButton);
                    break;
                case INPUT_RETURN:
                    PushedButton();
                    break;
                case INPUT_ESCAPE:
                    game.scene = SCENE_None;
                    break;
                default:
                    break;
            }
            break;
        case SCENE_SERVER_0:
            switch(game.input){
                case INPUT_RIGHT:
                    shiftSelect(+1,maxButtonNum[game.scene],&game.selectButton);
                    break;
                case INPUT_LEFT:
                    shiftSelect(-1,maxButtonNum[game.scene],&game.selectButton);
                    break;
                case INPUT_RETURN:
                    PushedButton();
                    break;
                case INPUT_ESCAPE:
                    game.scene = SCENE_Title;
                    game.selectButton = 0;
                    break;
                default:
                    if(INPUT_1 <= game.input && game.input <= INPUT_4){
                        game.selectButton = (int)(game.input-INPUT_0) - 2;
                    }
                    break;
            }
            break;
        case SCENE_SERVER_1: 
        case SCENE_CLIENT_1:{
            int adjust = -1;//Enter??­ã?¼ã??å¾?????????ºã???????§ã?????èª¿æ??
            if(strlen(game.port)>=4)
                adjust = 0;
            switch(game.input){
                case INPUT_UP:
                    if(game.selectButton<10 && game.selectButton >=5)
                        shiftSelect(-5,maxButtonNum[game.scene]+adjust,&game.selectButton);
                    else if(game.selectButton<5 && game.selectButton >=0 && adjust == 0)
                        game.selectButton = 11;
                    else if(game.selectButton == 11)
                        shiftSelect(-6,maxButtonNum[game.scene]+adjust,&game.selectButton);
                    break;
                case INPUT_DOWN:
                    if(game.selectButton<5 && game.selectButton >=0)
                        shiftSelect(+5,maxButtonNum[game.scene]+adjust,&game.selectButton);
                    else if(game.selectButton<10 && game.selectButton >=5 && adjust == 0)
                        game.selectButton = 11;
                    else if(game.selectButton == 11)
                        shiftSelect(+1,maxButtonNum[game.scene]+adjust,&game.selectButton);
                    break;
                case INPUT_RIGHT:
                        shiftSelect(+1,maxButtonNum[game.scene]+adjust,&game.selectButton);
                    break;
                case INPUT_LEFT:
                        shiftSelect(-1,maxButtonNum[game.scene]+adjust,&game.selectButton);
                    break;
                case INPUT_RETURN:
                    PushedButton();
                    break;
                case INPUT_ESCAPE:
                    game.scene = SCENE_None;
                    break;
                case INPUT_BACKSPACE:
                    game.selectButton = 10;//?????¤ã????¿ã??
                    PushedButton();
                    break;
                default:
                    if(INPUT_0 <= game.input && game.input <= INPUT_9){
                        game.selectButton = (int)(game.input-INPUT_0);
                        PushedButton();
                    }
                    break;
            }
            break;
        }   
        case SCENE_CLIENT_0:{
            int adjust = -1;
            if(strlen(game.deviceNum)>=3)
                adjust = 0;
            switch(game.input){
                case INPUT_UP:
                    if(game.selectButton<10 && game.selectButton >=5)
                        shiftSelect(-5,maxButtonNum[game.scene]+adjust,&game.selectButton);
                    else if(game.selectButton<5 && game.selectButton >=0 && adjust == 0)
                        game.selectButton = 11;
                    else if(game.selectButton == 11)
                        shiftSelect(-6,maxButtonNum[game.scene]+adjust,&game.selectButton);
                    break;
                case INPUT_DOWN:
                    if(game.selectButton<5 && game.selectButton >=0)
                        shiftSelect(+5,maxButtonNum[game.scene]+adjust,&game.selectButton);
                    else if(game.selectButton<10 && game.selectButton >=5 && adjust == 0)
                        game.selectButton = 11;
                    else if(game.selectButton == 11)
                        shiftSelect(+1,maxButtonNum[game.scene]+adjust,&game.selectButton);
                    break;
                case INPUT_RIGHT:
                        shiftSelect(+1,maxButtonNum[game.scene]+adjust,&game.selectButton);
                    break;
                case INPUT_LEFT:
                        shiftSelect(-1,maxButtonNum[game.scene]+adjust,&game.selectButton);
                    break;
                case INPUT_RETURN:
                    PushedButton();
                    break;
                case INPUT_ESCAPE:
                    game.scene = SCENE_Title;
                    game.selectButton = 0;
                    break;
                case INPUT_BACKSPACE:
                    game.selectButton = 10;//?????¤ã????¿ã??
                    PushedButton();
                    break;
                default:
                    if(INPUT_0 <= game.input && game.input <= INPUT_9){
                        game.selectButton = (int)(game.input-INPUT_0);
                        PushedButton();
                    }
                    break;
            }
        }
            break;
        case SCENE_CUSTOMIZE:
            switch(game.popScene){
                case PopUp_None:
                    if(game.isShift){
                        switch(game.input)
                        {
                            case INPUT_DOWN:
                                if(scrollValue <=850)
                                    scrollValue += 15;
                                break;
                            case INPUT_UP:
                                if(scrollValue >=0)
                                    scrollValue -= 15;
                                break;
                            case INPUT_RETURN:
                                PushedButton();
                                break;
                            case INPUT_ESCAPE:
                                game.scene = SCENE_Title;
                                game.selectButton = 0;
                                break;
                        }
                    }
                    else{
                        switch(game.input)
                        {
                            case INPUT_DOWN:
                                if(game.selectButton == 3){
                                    scrollValue = 850;
                                }
                                else if(game.selectButton == 8){
                                    scrollValue = 0;
                                }
                                shiftSelect(+1,maxButtonNum[game.scene],&game.selectButton);
                                break;
                            case INPUT_UP:
                                if(game.selectButton == 4){
                                    scrollValue = 0;
                                }
                                else if(game.selectButton == 0){
                                    scrollValue = 850;
                                }
                                shiftSelect(-1,maxButtonNum[game.scene],&game.selectButton);
                                break;
                            case INPUT_LEFT:
                                //game.selectButton-4???1??¤ç???????¹ã????­ã?¼ã???????¼ã???????????
                                if(game.selectButton >=4)
                                    shiftParmSelect(-1,static_cast<PARAMATER>(game.selectButton-4));
                                    SaveGameData(); //??²ã?¼ã???????¼ã?¿ã??ä¿?å­?
                                break;
                            case INPUT_RIGHT:
                                if(game.selectButton >=4)
                                    if(retSumParamater() < PARAMATER_SUM_MAX)
                                        shiftParmSelect(+1,static_cast<PARAMATER>(game.selectButton-4));
                                        SaveGameData(); //??²ã?¼ã???????¼ã?¿ã??ä¿?å­?
                                break;
                            case INPUT_RETURN:
                                PushedButton();
                                break;
                            case INPUT_ESCAPE:
                                game.scene = SCENE_Title;
                                game.selectButton = 0;
                                break;
                        }
                    }
                    break;
                case PopUp_Skill:
                    switch(game.input){
                        case INPUT_DOWN:
                            shiftSelect(+1,SKILL_NUM+1,&game.selectButton_sub);
                            break;
                        case INPUT_UP:
                            shiftSelect(-1,SKILL_NUM+1,&game.selectButton_sub);
                            break;
                        case INPUT_RETURN:
                            PushedButton();
                            break;
                        case INPUT_ESCAPE:
                            game.popScene = PopUp_None;
                            game.selectButton_sub = 1;
                            break;
                    }
                    break;
                case PopUp_Special:
                    switch(game.input){
                        case INPUT_DOWN:
                            if(game.selectButton_sub==0)
                                shiftSelect(+1,SPECIAL_NUM+1,&game.selectButton_sub);
                            break;
                        case INPUT_UP:
                            if(game.selectButton_sub >= 1)
                                game.selectButton_sub = 0;
                            break;
                        case INPUT_RIGHT:
                            shiftSelect(+1,SPECIAL_NUM+1,&game.selectButton_sub);
                            break;
                        case INPUT_LEFT:
                            shiftSelect(-1,SPECIAL_NUM+1,&game.selectButton_sub);
                            break;
                        case INPUT_RETURN:
                            PushedButton();
                            break;
                        case INPUT_ESCAPE:
                            game.popScene = PopUp_None;
                            game.selectButton_sub = 1;
                            break;
                    }
                    break;
                default:
                    break;
            }
            break;
            case SCENE_Result:
            switch(game.input){
                case INPUT_RIGHT:
                    switch(game.selectButton){
                        case 0:
                            game.selectButton = 1;
                            shiftSelect(+1,4,&rankingMode);
                            SortRanking(rankingMode);
                            break;
                        case 1:
                            shiftSelect(+1,4,&rankingMode);
                            SortRanking(rankingMode);
                            break;
                        case 4:
                            game.selectButton = 2;
                            break;
                        default:
                            game.selectButton += 1;
                            break;
                    }                       
                    break;
                case INPUT_LEFT:
                    switch(game.selectButton){
                        case 1:
                            game.selectButton = 0;
                            shiftSelect(-1,4,&rankingMode);
                            SortRanking(rankingMode);
                            break;
                        case 0:
                            shiftSelect(-1,4,&rankingMode);
                            SortRanking(rankingMode);
                            break;
                        case 2:
                            game.selectButton = 4;
                            break;
                        default:
                            game.selectButton -= 1;
                            break;
                    }    
                    break;
                case INPUT_UP:
                case INPUT_DOWN:
                    if(game.selectButton == 0 || game.selectButton == 1){
                        game.selectButton = 2;
                    }
                    else{
                        game.selectButton = 0;
                    }
                    break;
                case INPUT_RETURN:
                    PushedButton();
                    break;
                case INPUT_ESCAPE:
                    game.scene = SCENE_None;
                    break;
                default:
                    break;
            }
            break;
    }
    game.input = INPUT_NONE;
}
