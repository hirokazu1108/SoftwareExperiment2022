#include "header.h"

static Uint32 AniTimer(Uint32 interval, void* param);
static Scene InputEvent(void);
static Scene InputNameEvent(void);
void ExcuteInput(void);

bool isReadRankingflg = false;//isread rannkinng
settingGame game;
char tempName[NAME_MAX_LENGTH+1];
Mix_Music *bgm;

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

    /* System Init */
    InitSystem();
    /* SDL Init */
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO) < 0) {
        return PrintError(SDL_GetError());
    }
    /* UI Init */
    if (InitWindow() < 0) {
        PrintError("failed to initialize Windows");
        EndProgram();
        return 0;
    }
    if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT,2, 1024)==-1) {
        printf("Failed: %s\n", Mix_GetError());
        exit(-1);
    }
    bgm = Mix_LoadMUS("TITLE.wav"); // Music型で読み込み
    Mix_PlayMusic(bgm, -1); // Music型サウンドを無限に再生

    int framecnt = 0;

    /* game loop */
    while (game.scene != SCENE_None) {

        /* Title Scene */
        while(game.scene == SCENE_Title){
            game.scene = InputEvent();
            ExcuteInput();
            RenderTitleWindow();
            SDL_Delay(10);
            framecnt++;
        }

        /* Server client number */
        while(game.scene == SCENE_SERVER_0){
            game.scene = InputEvent();
            ExcuteInput();
            RenderServerWindow_0();
            SDL_Delay(10);
            framecnt++;
        }     

        /* Server port number */
        while(game.scene == SCENE_SERVER_1 || game.scene == SCENE_CLIENT_1){
            game.scene = InputEvent();
            ExcuteInput();
            if(game.scene == SCENE_SERVER_1 || game.scene == SCENE_CLIENT_1 )
                RenderPasscodeWindow();
            SDL_Delay(10);
            framecnt++;
        }       

        /* Client device number */
        while(game.scene == SCENE_CLIENT_0){
            game.scene = InputEvent();
            ExcuteInput();
            RenderDeviceNumWindow();
            SDL_Delay(10);
            framecnt++;
        }

        /* Customize Scene */
        while(game.scene == SCENE_CUSTOMIZE){
            if(game.popScene != PopUp_Name)
            {
                game.scene = InputEvent();
                ExcuteInput();
            }
            else{
                game.scene = InputNameEvent();
            }         
            RenderCustomizeWindow();
            SDL_Delay(10);
            framecnt++;
        }

        /* Client wait scene */
        while(game.scene == SCENE_CLIENT_WAIT){
            game.scene = InputEvent();
            ExcuteInput();
            ReadMatchFile();
            RenderClientWaitWindow();
            SDL_Delay(10);
            framecnt++;
        }
        
        /* Result scene */
        while(game.scene == SCENE_Result){
            if(!isReadRankingflg){
                ReadRankingFile();
                isReadRankingflg = true;
                rankIndex = (int*)malloc(sizeof(int)*game.clientNum);
                rankNumber = (int*)malloc(sizeof(int)*game.clientNum);
                SortRanking(rankingMode);
            }
            game.scene = InputEvent();
            ExcuteInput();
            RenderResultWindow();
            SDL_Delay(10);
            framecnt++;
        }
    }

    // audio quit
    Mix_FreeMusic(bgm);
    bgm=NULL; // 解放したことを明示するために
    Mix_CloseAudio();
    Mix_Quit();

    system("exit");
    return 0;
}

// PrintError()
int PrintError(const char* str)
{
    fprintf(stderr, "%s\n", str);
    return -1;
}

// EndProgram()
void EndProgram(void){
    DestroyWindow();
    SDL_Quit();
}

// InputEvent()
Scene InputEvent(void)
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            return SCENE_None;
        case SDL_KEYDOWN:
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
                if(SDLK_0 <= event.key.keysym.sym && event.key.keysym.sym <= SDLK_9){
                    game.input = static_cast<INPUT>(event.key.keysym.sym - SDLK_0 + INPUT_0);
                    std::cout << game.input<<'\n';
                }
                break;
            }
            break;
        case SDL_KEYUP:
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

// InputNameEvent()
Scene InputNameEvent(void)
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            return SCENE_None;
        case SDL_KEYDOWN:
            if (event.key.repeat)
                break;
            switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
                sprintf(tempName,"%s",game.clientName);
                game.popScene = PopUp_None;
                break;
            case SDLK_BACKSPACE:
                if(strlen(tempName)>0){
                    tempName[strlen(tempName)-1] = '\0';
                }
                break;
            case SDLK_RETURN:
                sprintf(game.clientName,"%s",tempName);
                SaveGameData();
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
            int adjust = -1;/* enter position adjust */
            if(strlen(game.port)>=4)
                adjust = 0;
            switch(game.input){
                case INPUT_UP:
                    if(game.selectButton == 0)
                        game.selectButton = 12+adjust; 
                    else if(game.selectButton == 12)
                        game.selectButton = 6;
                    else if(game.selectButton<=5 && game.selectButton >=1)
                        game.selectButton = 0;
                    else if(game.selectButton<=10 && game.selectButton >=6)
                        shiftSelect(-5,maxButtonNum[game.scene]+adjust,&game.selectButton);
                    else if(game.selectButton==12)
                        game.selectButton = 11;
                    break;
                case INPUT_DOWN:
                    if(game.selectButton == 0)
                        game.selectButton = 1;
                    else if(((game.selectButton<=11 && game.selectButton >=6)) && adjust == -1)
                        game.selectButton = 0;
                    else if(((game.selectButton<=11 && game.selectButton >=6)) && adjust == 0)
                        game.selectButton = 12;
                    else if(game.selectButton<=5 && game.selectButton >=1)
                        shiftSelect(+5,maxButtonNum[game.scene]+adjust,&game.selectButton);
                    else if(game.selectButton == 12){
                        game.selectButton = 0;
                    }
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
                    if(game.scene == SCENE_SERVER_1)
                        game.scene = SCENE_SERVER_0;
                    else if(game.scene == SCENE_CLIENT_1)
                        game.scene = SCENE_CLIENT_0;
                    game.selectButton = 1;
                    for(int i=0; game.deviceNum[i]!='\0';i++){
                        game.deviceNum[i] = '\0';
                    }
                    break;
                case INPUT_BACKSPACE:
                    game.selectButton = 11;
                    PushedButton();
                    break;
                default:
                    if(INPUT_0 <= game.input && game.input <= INPUT_9){
                        game.selectButton = (int)(game.input-INPUT_0+1);
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
                    if(game.selectButton == 0)
                        game.selectButton = 13+adjust; 
                    else if(game.selectButton == 13)
                        game.selectButton = 6;
                    else if(game.selectButton<=5 && game.selectButton >=1 || game.selectButton == 11)
                        game.selectButton = 0;
                    else if(game.selectButton<=10 && game.selectButton >=6)
                        shiftSelect(-5,maxButtonNum[game.scene]+adjust,&game.selectButton);
                    else if(game.selectButton==12)
                        game.selectButton = 11;
                    break;
                case INPUT_DOWN:
                    if(game.selectButton == 0)
                        game.selectButton = 1;
                    else if(game.selectButton == 13)
                        game.selectButton = 0;
                    else if(((game.selectButton<=10 && game.selectButton >=6)||(game.selectButton == 12)) && adjust == -1)
                        game.selectButton = 0;
                    else if(((game.selectButton<=10 && game.selectButton >=6)||(game.selectButton == 12)) && adjust == 0)
                        game.selectButton = 13;
                    else if(game.selectButton<=5 && game.selectButton >=1)
                        shiftSelect(+5,maxButtonNum[game.scene]+adjust,&game.selectButton);
                    else if(game.selectButton==11)
                        game.selectButton = 12;
                    break;
                case INPUT_RIGHT:
                        if(game.selectButton == 5)
                            game.selectButton = 11;
                        else if(game.selectButton == 10)
                            game.selectButton = 12;
                        else if(game.selectButton != 11 && game.selectButton != 12)
                            shiftSelect(+1,maxButtonNum[game.scene]+adjust,&game.selectButton);
                    break;
                case INPUT_LEFT:
                        if(game.selectButton == 11)
                            game.selectButton = 5;
                        else if(game.selectButton == 12)
                            game.selectButton = 10;
                        else if(game.selectButton != 12 && game.selectButton != 6 && game.selectButton != 1)
                            shiftSelect(-1,maxButtonNum[game.scene]+adjust,&game.selectButton);
                    break;
                case INPUT_RETURN:
                    PushedButton();
                    break;
                case INPUT_ESCAPE:
                    game.scene = SCENE_CUSTOMIZE;
                    game.selectButton = 1;
                    scrollValue = 0.0f;
                    break;
                case INPUT_BACKSPACE:
                    game.selectButton = 11;
                    PushedButton();
                    break;
                default:
                    if(INPUT_0 <= game.input && game.input <= INPUT_9){
                        game.selectButton = (int)(game.input-INPUT_0+1);
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
                                if(scrollValue <=905)
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
                                    scrollValue = 905;
                                }
                                else if(game.selectButton == 9){
                                    scrollValue = 0;
                                }
                                shiftSelect(+1,maxButtonNum[game.scene],&game.selectButton);
                                break;
                            case INPUT_UP:
                                if(game.selectButton == 4){
                                    scrollValue = 0;
                                }
                                else if(game.selectButton == 0){
                                    scrollValue = 905;
                                }
                                shiftSelect(-1,maxButtonNum[game.scene],&game.selectButton);
                                break;
                            case INPUT_LEFT:
                                if(game.selectButton >=4)
                                    shiftParmSelect(-1,static_cast<PARAMATER>(game.selectButton-4));
                                    SaveGameData(); 
                                break;
                            case INPUT_RIGHT:
                                if(game.selectButton >=4)
                                    if(retSumParamater() < PARAMATER_SUM_MAX)
                                        shiftParmSelect(+1,static_cast<PARAMATER>(game.selectButton-4));
                                        SaveGameData();
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
                            else if(1<=game.selectButton_sub && game.selectButton_sub<=4){
                                game.selectButton_sub +=4;
                            }
                            else{
                                game.selectButton_sub = 0;
                            }
                            break;
                        case INPUT_UP:
                            if(game.selectButton_sub==0)
                                game.selectButton_sub = 5;
                            else if(1<=game.selectButton_sub && game.selectButton_sub<=4){
                                game.selectButton_sub = 0;
                            }
                            else{
                                game.selectButton_sub -= 4;
                            }
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
