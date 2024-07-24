#include "System.h"


//act(dt)
void sys::act(float dt) {
    control(dt);
    turnTowards();
    rotate(dt);
    move(dt);
    outOfBounds();
}
void sys::move(float dt) {
    auto& View = Reg.view<cmp::Velocity>();
    for (auto&& [Ent, Vel] : View) {
        auto CPos = Reg.findComponentOrNull<cmp::Position>(Ent);
        if (!CPos)
            continue;
        (vec2<float>&)*CPos += Vel;
    }
}

void sys::rotate(float dt) {
    auto& View = Reg.view<cmp::Rotation>();
    for (auto&& [Ent, Angle] : View) {
        auto CTr = Reg.findComponentOrNull<cmp::Sprite>(Ent);
        if (!CTr)
            continue;
        rotateSprite(Angle.phi, CTr->v1, CTr->v2, CTr->v3);
    }
}

bool sys::outOfBounds() {
    auto& View = Reg.view<cmp::Position>();
    bool is_out = false;
    for (auto&& [Ent, Pos] : View) {
        if (Pos.x < BOUND_WIDTH) {
            Pos.x = BOUND_WIDTH;
            is_out = true;
        }
        if (Pos.y < BOUND_WIDTH) {
            Pos.y = BOUND_WIDTH;
            is_out = true;
        }
        if (Pos.x > SCREEN_WIDTH - BOUND_WIDTH) {
            Pos.x = SCREEN_WIDTH - BOUND_WIDTH;
            is_out = true;
        }
        if (Pos.y > SCREEN_HEIGHT - BOUND_WIDTH) {
            Pos.y = SCREEN_HEIGHT - BOUND_WIDTH;
            is_out = true;
        }
    }
    return is_out;
}

void sys::control(float dt) {
    auto& View = Reg.view<cmp::IsPlayer>();
    for (auto&& [Ent, Player] : View) {  
        auto CSpeed = Reg.findComponentOrNull<cmp::Velocity>(Ent);
        if (!CSpeed)
            continue;
        float scale_x = SPEED_SCALE;
        float scale_y = SPEED_SCALE;
    
        if (is_key_pressed('D')) {
            if (CSpeed->dx <= MAX_SPEED)
                CSpeed->dx += scale_x;
        }
        if (is_key_pressed('A')) {
            if (CSpeed->dx >= -MAX_SPEED)
                CSpeed->dx -= scale_x;
        }
        if (is_key_pressed('S')) {
            if (CSpeed->dy <= MAX_SPEED)
                CSpeed->dy += scale_y;
        }
        if (is_key_pressed('W')) {
            if (CSpeed->dy >= -MAX_SPEED)
                CSpeed->dy -= scale_y;
        }
        if (!is_key_pressed('A') && !is_key_pressed('D')) { // speed fading
            CSpeed->dx *= SPEED_FADE;
        }
        if (!is_key_pressed('W') && !is_key_pressed('S')) { // speed fading
            CSpeed->dy *= SPEED_FADE;
        }
        if (is_mouse_button_pressed(0)) {
            sys::shoot();
        }
    }
}
void sys::shoot() {
    auto& View = Reg.view<cmp::IsBullet>();
    for (auto&& [Ent, Bullet] : View) {
        auto CPos = Reg.findComponentOrNull<cmp::Position>(Ent);
        auto CVel = Reg.findComponentOrNull<cmp::Velocity>(Ent);
        auto CDir = Reg.findComponentOrNull<cmp::Direction>(Ent);
        auto CRot = Reg.findComponentOrNull<cmp::Rotation>(Ent);
        auto CActive = Reg.findComponentOrNull<cmp::IsActive>(Ent);
        auto CPlayerPos = Reg.findComponentOrNull<cmp::Position>(Bullet.player_id);
        auto CPlayerDir = Reg.findComponentOrNull<cmp::Direction>(Bullet.player_id);
        if (CActive) CActive->is_active = true;
        if (!CPos || !CVel || !CDir || !CRot || !CPlayerPos || !CPlayerDir)
            continue;
        (vec2<float>&)* CPos = (vec2<float>&) * CPlayerPos;
        CRot->phi = angle_between((vec2<float>&) * CDir, (vec2<float>&) * CPlayerDir);
        (vec2<float>&)* CDir = (vec2<float>&) * CPlayerDir;
        //Speed Update
        (vec2<float>&)* CVel = (vec2<float>&) * CDir * BULLET_SPEED;
        break;
    }
}

void sys::turnTowardsCursor() {
    auto& View = Reg.view<cmp::IsPlayer>();
    for (auto&& [Ent, Player] : View) {
        auto CRot = Reg.findComponentOrNull<cmp::Rotation>(Ent);
        auto CPos = Reg.findComponentOrNull<cmp::Position>(Ent);
        auto CDir = Reg.findComponentOrNull<cmp::Direction>(Ent);
        if (!CRot || !CPos || !CDir)
            continue;
        vec2<float> NewDir(get_cursor_x() - CPos->x, get_cursor_y() - CPos->y);
        CRot->phi = angle_between((vec2<float>&)*CDir, NewDir);
        rotateVector(CRot->phi, (vec2<float>&)*CDir);
    }
}
void sys::turnTowardsPlayer() {
    auto& View = Reg.view<cmp::IsEnemy>();
    for (auto&& [Ent, Enemy] : View) {
        auto CRot = Reg.findComponentOrNull<cmp::Rotation>(Ent);
        auto CPos = Reg.findComponentOrNull<cmp::Position>(Ent);
        auto CDir = Reg.findComponentOrNull<cmp::Direction>(Ent);
        auto CPlayerPos = Reg.findComponentOrNull<cmp::Position>(Enemy.player_id);

        if (!CRot || !CPos || !CDir || !CPlayerPos)
            continue;
        vec2<float> NewDir(CPlayerPos->x - CPos->x, CPlayerPos->y - CPos->y);
        CRot->phi = angle_between((vec2<float>&)*CDir, NewDir);
        rotateVector(CRot->phi, (vec2<float>&)*CDir);
        //Speed Update
        auto CVel = Reg.findComponentOrNull<cmp::Velocity>(Ent);
        if(CVel)
            (vec2<float>&)* CVel = (vec2<float>&)*CDir * MAX_ENEMY_SPEED; 
    }
}
void sys::turnTowards() {
    sys::turnTowardsCursor();
    sys::turnTowardsPlayer();
}


//draw(buffer)
void sys::draw(BuffTy Buffer) {

    auto& View = Reg.view<cmp::Sprite>();

    for (auto&& [Ent, Tr] : View) {
        auto CColor = Reg.findComponentOrNull<Color>(Ent);
        auto CPos = Reg.findComponentOrNull<cmp::Position>(Ent);
        auto CActive = Reg.findComponentOrNull<cmp::IsActive>(Ent);

        if (!CPos || !CColor || !CActive->is_active)
            continue;

        vec2<int> p(static_cast<int>(CPos->x), static_cast<int>(CPos->y));
        vec2<int> y = get_min_max(Tr.v1.y, Tr.v2.y, Tr.v3.y) + vec2<int>(p.y, p.y); // y = (y_min, y_max)
        vec2<int> x = get_min_max(Tr.v1.x, Tr.v2.x, Tr.v3.x) + vec2<int>(p.x, p.x); // x = (x_min, x_max)
        for (int j = y.x; j <= y.y; ++j) {
            for (int i = x.x; i <= x.y; ++i) {
                if (point_in_triangle(vec2<int>(i, j), Tr.v1 + *CPos, Tr.v2 + *CPos, Tr.v3 + *CPos)) {
                    if (i<0 || i > SCREEN_WIDTH || j < 0 || j > SCREEN_HEIGHT)
                        continue;
                    buffer[j][i] = CColor->data;
                }
            }
        }
    }
}