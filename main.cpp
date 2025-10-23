#include "raylib.h"
#include <vector>
using namespace std;
int main(){
    const int chieudai = 1400;
    const int chieurong = 800;
    InitWindow(chieudai, chieurong, "PINK VS BLACK");
    InitAudioDevice();
    Sound BlackAttack = LoadSound("small-monster-attack-195712.mp3");
    SetSoundVolume(BlackAttack, 0.3f);
    Sound soundBullet = LoadSound("080884_bullet-hit-39872.mp3");
    Sound Pinkbidanh = LoadSound("grunt-1-85280.mp3");
    Texture2D Pink = LoadTexture("Pinkgun.png");
    Texture2D Black = LoadTexture("Black.png");
    Texture2D Bullet = LoadTexture("dan.png");  
    Texture2D blood = LoadTexture("blood.png");  
    Texture2D phongnen = LoadTexture("phongnen2.png");
    struct nvBlack
    {
        Vector2 vitriBlack;
        int huong;
        Rectangle hbBlack;
        bool alive;
    };
    struct Bullets {
        Vector2 vitriBullet;
        int huong;
        Rectangle hbBullet;
        bool alive;
    };
    vector<Bullets> dsBullet;
    vector<nvBlack> dsBlack;
    float vitrispawnBlackx;
    Vector2 vitriPink = {100, 600};
    struct Blood
    {
        Vector2 vitriblood;
        float timer;
    };
    vector<Blood> dsblood;
    int soframePink = 2;
    int huongPink;
    float tocdoY_Pink = 0;
    const int ground = 550;
    bool dangchamdatPink = true;
    const int trongluc = 1800;
    const int lucnhay = 900;
    int solannhayPink = 2;
    const int tocdonhanvat = 500;
    int mauPink = 10;
    float damagecooldown = 0.0f;
    bool gameover = false;
    float thoigianspawnBlack = 0.0f;
    float cooldownBullet = 0.1f;
    int soBlackdie = 0;
    //hitbox nhan vat
    Rectangle hbPink = {vitriPink.x + 35, vitriPink.y, (float)Pink.width- 70, (float)Pink.height };
    int xphongnen = 0.0;
    SetTargetFPS(60);
    while(!WindowShouldClose()){
        xphongnen -= 1.5f;
        // reset phong nen
        if (xphongnen <= -(chieudai)) xphongnen = 0.0f;
        //dat vitri cu
        Vector2 vitricuPink = {vitriPink.x, vitriPink.y};
        //dieu khien nhan vat Pink 
        if(IsKeyDown(KEY_A)){
             vitriPink.x -= tocdonhanvat  * GetFrameTime();
             huongPink = -1;
        }
        if(IsKeyDown(KEY_D)){
             vitriPink.x += tocdonhanvat  * GetFrameTime();
             huongPink = 1;
        }
        if(IsKeyPressed(KEY_W) && solannhayPink > 0){
            tocdoY_Pink = -lucnhay; // nhảy lên
            dangchamdatPink = false;
            solannhayPink -- ;
            }
        if(!dangchamdatPink){
            tocdoY_Pink += trongluc * GetFrameTime(); // cộng trọng lực
            vitriPink.y += tocdoY_Pink * GetFrameTime();
        }
        if(vitriPink.y >= ground){
            vitriPink.y = ground;
            tocdoY_Pink = 0;
            dangchamdatPink = true;
            solannhayPink = 2;
        }
        //hitbox Pink
        hbPink = {vitriPink.x + 35, vitriPink.y + 35, (float)Pink.width - 70, (float)Pink.height -70};
        //cooldown Bullet
        cooldownBullet -= GetFrameTime();
        if(cooldownBullet == 0) cooldownBullet = 0;
        if(IsKeyDown(KEY_F) && cooldownBullet <= 0.0f){
            Bullets bullets;
            bullets.vitriBullet.x = (vitriPink.x + Pink.width / 2);
            bullets.vitriBullet.y = vitriPink.y + 70 ;
            bullets.huong = huongPink;
            bullets.hbBullet = {bullets.vitriBullet.x, bullets.vitriBullet.y, (float)Bullet.width, (float)Bullet.height};
            bullets.alive = true;
            dsBullet.push_back(bullets);
            cooldownBullet = 0.1f;
            PlaySound (soundBullet);
        }
        // dieu khien Bullets
        for (auto &bullets : dsBullet){
            if(!bullets.alive) continue;
            bullets.vitriBullet.x += bullets.huong * 1400 * GetFrameTime();
            bullets.hbBullet = {bullets.vitriBullet.x, bullets.vitriBullet.y, (float)Bullet.width, (float)Bullet.height};
            if(bullets.vitriBullet.x >chieudai || bullets.vitriBullet.x <0){
            bullets.alive = false;
                }
            }
        
        // spawn black
        thoigianspawnBlack += GetFrameTime();
        if((thoigianspawnBlack >= 2.0f) || IsKeyDown(KEY_KP_1)){
            nvBlack black;
            black.vitriBlack = {(GetRandomValue(0,1)) == 0 ? -100.0f : 1500.0f,ground };
            black.huong = (black.vitriBlack.x < 0) ? 1 : -1; 
            black.hbBlack = { black.vitriBlack.x + 50, black.vitriBlack.y + 50, (float)Black.width - 100, (float)Black.height - 100 }; 
            black.alive = true;
            dsBlack.push_back(black);
            thoigianspawnBlack = 0.0f;
    }
        //dieu khien nhan vat Black
        for (auto &black : dsBlack)
        {
            if(!black.alive) continue;
            if(black.vitriBlack.x < vitriPink.x ) black.vitriBlack.x += GetRandomValue(300, 1) * GetFrameTime();
            if(black.vitriBlack.x > vitriPink.x ) black.vitriBlack.x -= GetRandomValue(300, 1) * GetFrameTime();
            black.hbBlack = { black.vitriBlack.x + 50, black.vitriBlack.y + 50, (float)Black.width - 100, (float)Black.height - 100 };
        }
        //kiem tra PInk bi danh
        damagecooldown -= GetFrameTime();
        if (damagecooldown == 0) damagecooldown =0;
        for(auto &black : dsBlack){
            if (!black.alive) continue;
            if( CheckCollisionRecs(hbPink, black.hbBlack) && damagecooldown <= 0.0f ){
                mauPink -= 1 ;
                damagecooldown = 1.0f;
                vitriPink = vitricuPink;
                if(vitriPink.x < black.vitriBlack.x) {
                    vitriPink.x += 20;  
                    tocdoY_Pink = -200;
                }
                if (vitriPink.x >black.vitriBlack.x){
                    vitriPink.x -= 20 ;
                    tocdoY_Pink = -200;
                }
                if(dangchamdatPink) dangchamdatPink = false;
                    PlaySound (BlackAttack);
                    PlaySound (Pinkbidanh);
                }
        }
        //kiem tra hitbox Bullet voi Black
        for(auto &bullets : dsBullet){
            if(!bullets.alive) continue;
            for(auto &black :dsBlack){
                if(!black.alive) continue;
                if(CheckCollisionRecs(bullets.hbBullet,black.hbBlack)){
                    black.alive = false;
                    bullets.alive = false;
                    soBlackdie ++;
                    Blood blood;
                    blood.vitriblood = black.vitriBlack;
                    blood.timer = 0.3;
                    dsblood.push_back(blood);
                    break;
                    
                }
            }
        }
        if (mauPink == 0) gameover = true;
        BeginDrawing();
        
        ClearBackground(GRAY);
        //ve phong nen
        DrawTexture(phongnen, xphongnen, 0, WHITE);
        DrawTexture(phongnen, xphongnen + chieudai, 0, WHITE);
        //ve blood
        for(int i = 0; i < dsblood.size(); i++){
            DrawTexture(blood, dsblood[i].vitriblood.x, dsblood[i].vitriblood.y, WHITE);
            dsblood[i].timer -= GetFrameTime();
            if(dsblood[i].timer <= 0){
                dsblood.erase(dsblood.begin()+i);
                i--;
            }
        }
        //ve nhan vat
        if(!gameover){
            Color ColorPink = (damagecooldown > 0.8f) ? RED : WHITE;
            if(IsKeyDown(KEY_F)){
                    DrawTextureRec(Pink,{0,(float) Pink.height/soframePink, (float)Pink.width * huongPink, (float)Pink.height / soframePink }, vitriPink, ColorPink);
            }
            else DrawTextureRec(Pink,{0, 0, (float)Pink.width, (float)Pink.height/soframePink}, vitriPink, ColorPink);
            for (auto &black : dsBlack){
                if(black.alive) DrawTexture(Black, black.vitriBlack.x, black.vitriBlack.y, WHITE);
            }
            DrawText(TextFormat("HP PINK : %d ", mauPink), 10,10, 30, mauPink <= 3 ? RED : BLACK );
            DrawText(TextFormat("SO BLACK DA TIEU DIET : %d", soBlackdie), 10, 40, 30, BLACK);
            for(auto &bullets : dsBullet){
                if(!bullets.alive) continue;
                DrawTexture(Bullet, bullets.vitriBullet.x , bullets.vitriBullet.y, WHITE );
                
            }
        }
        else {
            DrawText("GAME OVER", 100 ,400, 200, RED );
            DrawText("Nhan [R] de choi lai ", 600, 600, 20, BLACK );
            if (IsKeyPressed(KEY_R)) {
                mauPink =10;
                vitriPink = {100, 600};
                dsBlack.clear();
                soBlackdie = 0;
                 
                gameover = false;

            }
        }
        EndDrawing();
    }
    CloseWindow();
    UnloadTexture(Pink);
    UnloadTexture(Black);
    UnloadTexture(Bullet);
    UnloadTexture(phongnen);
    UnloadTexture(blood);
    UnloadSound(BlackAttack);
    UnloadSound(Pinkbidanh);
    UnloadSound(soundBullet);
    return 0;
}
