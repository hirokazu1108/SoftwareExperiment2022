#include "../constants.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#define WD_Height 1000
#define WD_Width 1200
#define IMG_NUM 47
#define TEXT_NUM 84
#define SCENE_NUM 8

typedef enum{
    SCENE_Title,
    SCENE_SERVER_0,
    SCENE_SERVER_1,
    SCENE_CLIENT_0,
    SCENE_CLIENT_1,
    SCENE_CLIENT_WAIT,
    SCENE_CUSTOMIZE,
    SCENE_Result,
    SCENE_None,//çµ?äº????ç¤ºã??
}Scene;

// imgFile????????????????????¤ã?????
typedef enum{
    uname_inputClientNum,
    uname_inputPasscodeNum,
    uname_inputDeviceNum,
    uname_name,
    uname_skill,
    uname_special,
    uname_status,
    uname_explain_skill,
    uname_nowselect,
    uname_change,
    uname_backButton,
    uname_skill_attack,
    uname_skill_hp,
    uname_skill_speed,
    uname_pin,
    uname_rightSelect,
    uname_leftSelect,
    uname_back,
    uname_nameChange,
    uname_skillChange,
    uname_specialChange,
    uname_selectHikouki,
    uname_barrier,
    uname_disabled,
    uname_bigbullet,
    uname_lines,
    uname_damageArea,
    uname_beam,
    uname_chase,
    uname_transform,
    uname_barrier_icon,
    uname_disabled_icon,
    uname_bigbullet_icon,
    uname_lines_icon,
    uname_damageArea_icon,
    uname_beam_icon,
    uname_chase_icon,
    uname_transform_icon,
    uname_logo,
    uname_rankingBoard,
    uname_rankingBack,
    uname_title_sky,
    uname_title_sky2,
    uname_cloud,
    uname_castle,
    uname_masao,
    uname_masao_face,
}uiName;

// textStr????????????????????¤ã?????
typedef enum{
    tname_title,
    tname_server,
    tname_client,
    tname_customize,
    tname_inputNum,
    tname_inputPasscode,
    tname_del,
    tname_enter,
    tname_self,
    tname_inputDevice,
    tname_clpc,
    tname_nowloading,
    tname_result,
    tname_exit,
    tname_detail,
    tname_totitle,
    tname_score_rank,
    tname_kill_rank,
    tname_death_rank,
    tname_killenemy_rank,
    tname_0,
    tname_1,
    tname_2,
    tname_3,
    tname_4,
    tname_5,
    tname_6,
    tname_7,
    tname_8,
    tname_9,
    tname_a,
    tname_b,
    tname_c,
    tname_d,
    tname_e,
    tname_f,
    tname_g,
    tname_h,
    tname_i,
    tname_j,
    tname_k,
    tname_l,
    tname_m,
    tname_n,
    tname_o,
    tname_p,
    tname_q,
    tname_r,
    tname_s,
    tname_t,
    tname_u,
    tname_v,
    tname_w,
    tname_x,
    tname_y,
    tname_z,
    tname_A,
    tname_B,
    tname_C,
    tname_D,
    tname_E,
    tname_F,
    tname_G,
    tname_H,
    tname_I,
    tname_J,
    tname_K,
    tname_L,
    tname_M,
    tname_N,
    tname_O,
    tname_P,
    tname_Q,
    tname_R,
    tname_S,
    tname_T,
    tname_U,
    tname_V,
    tname_W,
    tname_X,
    tname_Y,
    tname_Z,
    tname_space,
    tname_hyphen,
}textName;

typedef enum{
    INPUT_NONE,
    INPUT_UP,
    INPUT_DOWN,
    INPUT_RIGHT,
    INPUT_LEFT,
    INPUT_RETURN,
    INPUT_ESCAPE,
    INPUT_BACKSPACE,
    INPUT_DELETE,
    INPUT_0,
    INPUT_1,
    INPUT_2,
    INPUT_3,
    INPUT_4,
    INPUT_5,
    INPUT_6,
    INPUT_7,
    INPUT_8,
    INPUT_9,
}INPUT;

typedef enum{
    PopUp_None,
    PopUp_Name,
    PopUp_Skill,
    PopUp_Special,
}PopUpScne;

class settingGame{
    public:
    SDL_Window *window;
    SDL_Renderer *render;
    Scene scene;
    PopUpScne popScene;
    int selectButton;
    int selectButton_sub;//popup??»é?¢ã??ä½¿ç??
    int clientNum;
    char port[5];
    char deviceNum[4];
    char clientName[NAME_MAX_LENGTH+1];
    INPUT input;
    bool isShift;
    char command[256];
    SKILL skill;
    SPECIAL special;
    int parm[5];   //??¹ã????¼ã?¿ã?¹ã?????(5ç¨?é¡?)???index???PARAMATER??????ä½???????
    RankingData rankingData;
};

/* ??»å??????????? */
typedef struct {
    public:
    int w;
    int h;         
    SDL_Texture* texture;
    void drawRotateTexture(int x, int y, int width=0,int height=0, int rad=0, SDL_RendererFlip mode=SDL_FLIP_HORIZONTAL);
    void drawTexture(int x, int y, int width=0,int height=0);
    
}ImgInfo;

/* main.cpp */
extern settingGame game;
extern char tempName[NAME_MAX_LENGTH+1];
extern int PrintError(const char* str);
extern void EndProgram(void);

/* window.cpp */
extern int InitWindow(void);
extern void DestroyWindow(void);
extern void RenderTitleWindow(void);
extern void RenderServerWindow_0(void);
extern void RenderPasscodeWindow();
extern void RenderDeviceNumWindow(void);
extern void RenderCustomizeWindow(void);
extern void RenderClientWaitWindow(void);
extern void RenderResultWindow(void);
extern std::vector<SDL_Rect> buttonPos[SCENE_NUM];
extern int rankingMode;

/* system.cpp */
extern void InitSystem(void);
extern int maxButtonNum[SCENE_NUM];
extern int scrollValue;
extern void shiftSelect(int shift, int max, int *target);
extern void shiftParmSelect(int shift,PARAMATER index);
extern int retSumParamater(void);
extern void PushedButton(void);
extern bool retExists(const char *file);
extern void WriteDataFile(SaveData *data);
extern void ReadDataFile(SaveData *data);
extern void SaveGameData(void);
extern void ReadMatchFile(void);
extern void WriteMatchFile(int value);
extern void ReadRankingFile(void);
extern void SortRanking(int mode);
extern int GetIndexFromName(char name[]);
extern int *rankIndex;
extern int *rankNumber;
