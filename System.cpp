#include "System.h"


//act(dt)
void sys::act(float dt) { // moving, rotations
    turnTowards(dt);
    rotate();
    move(dt);
    playerRegeneration(dt);
    control(dt);
    ammoReload(dt);

}
void sys::update() {//changes the way for act depending on the situation
    checkHealth();
    outOfBounds();
    playerCollideEnemy();
    bulletCollideEnemy();
}

void sys::move(float dt) {
    auto& View = Reg.view<cmp::Velocity>();
    for (auto&& [Ent, Vel] : View) {
        auto CPos = Reg.findComponentOrNull<cmp::Position>(Ent);
        auto CActive = Reg.findComponentOrNull<cmp::IsActive>(Ent);
        if (!CPos || !CActive->is_active)  
            continue;
        (vec2<float>&)*CPos += (vec2<float>&)Vel;
        //for all entities that are not bullets, the velocity fades
        auto CBullet = Reg.findComponentOrNull<cmp::IsBullet>(Ent);
        if (!CBullet)
            (vec2<float>&)Vel *= SPEED_FADE;
    }
}

void sys::rotate() {
    auto& View = Reg.view<cmp::Rotation>();
    for (auto&& [Ent, Angle] : View) {
        auto CTr = Reg.findComponentOrNull<cmp::Sprite>(Ent);
        if (!CTr)  continue;
        rotateSprite(Angle.phi, CTr->v1, CTr->v2, CTr->v3);
        Angle.phi = 0.f;
    }
}

void sys::outOfBounds() {
    auto& View = Reg.view<cmp::Position>();
    for (auto&& [Ent, Pos] : View) {
        bool is_out = false;

        auto CActive = Reg.findComponentOrNull<cmp::IsActive>(Ent);
        auto CVel = Reg.findComponentOrNull<cmp::Velocity>(Ent);

        if (!CVel || !(CActive->is_active))
            continue;
        if (Pos.x < BOUND_WIDTH) {
            CVel->dx = 0;
            Pos.x = BOUND_WIDTH;
            is_out = true;
        }
        if (Pos.y < BOUND_WIDTH) {
            CVel->dy = 0;
            Pos.y = BOUND_WIDTH;
            is_out = true;
        }
        if (Pos.x > SCREEN_WIDTH - BOUND_WIDTH) {
            CVel->dx = 0;
            Pos.x = SCREEN_WIDTH - BOUND_WIDTH;
            is_out = true;
        }
        if (Pos.y > SCREEN_HEIGHT - BOUND_WIDTH) {
            CVel->dy = 0;
            Pos.y = SCREEN_HEIGHT - BOUND_WIDTH;
            is_out = true;
        }
        //if the bullet has reached the border, then it returns to the ammo storage
        auto CBullet = Reg.findComponentOrNull<cmp::IsBullet>(Ent);
        if (CBullet && is_out) { // until the bullet goes beyond the border, it cannot be used
            CActive->is_active = false;
        }
    }
}
void sys::checkHealth() {
    auto& View = Reg.view<cmp::Health>();
    for (auto&& [Ent, CHealth] : View) {
        if (CHealth.curr_health > 0)
            continue;
        auto CActive = Reg.findComponentOrNull<cmp::IsActive>(Ent);
        if (!CActive->is_active)
            continue;
        CActive->is_active = false;
        auto CPlayer = Reg.findComponentOrNull<cmp::IsPlayer>(Ent);
        if(CPlayer)
            schedule_quit_game(); // here should be die screen
    }
}

void sys::control(float dt) {
    auto& View = Reg.view<cmp::IsPlayer>();
    for (auto&& [Ent, _] : View) {  
        auto CSpeed = Reg.findComponentOrNull<cmp::Velocity>(Ent);

        if (!CSpeed)  
            continue;
        float scale_x = SPEED_SCALE * dt;
        float scale_y = SPEED_SCALE * dt;
        
    
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
        
        if (is_mouse_button_pressed(0)) {
            auto CShoot = Reg.findComponentOrNull<cmp::CanShoot>(Ent);
            if (CShoot->can_shoot) {
                sys::shoot();
                CShoot->timer = AMMO_RELOAD;
                CShoot->can_shoot = false;
            }   
        }
    }
}
void sys::shoot() {
    auto PlayerEnt = Reg.getPlayer();
    auto CPlayerPos = Reg.findComponentOrNull<cmp::Position>(PlayerEnt);
    auto CPlayerDir = Reg.findComponentOrNull<cmp::Direction>(PlayerEnt);

    auto CAmmo = Reg.findComponentOrNull<cmp::Ammo>(PlayerEnt);

    for (int i = 0; i < AMMO_AMOUNT; ++i) {
        EntityId BulletEnt = CAmmo->ammo_store[i];
        auto CActive = Reg.findComponentOrNull<cmp::IsActive>(BulletEnt);
        if (CActive->is_active)
            continue;
        CActive->is_active = true;
        auto CPos = Reg.findComponentOrNull<cmp::Position>(BulletEnt);
        auto CVel = Reg.findComponentOrNull<cmp::Velocity>(BulletEnt);
        auto CDir = Reg.findComponentOrNull<cmp::Direction>(BulletEnt);
        auto CRot = Reg.findComponentOrNull<cmp::Rotation>(BulletEnt);
        if (!CPos || !CVel || !CDir || !CRot )
            continue;
        //turn towards the direction of the player
        (vec2<float>&)* CPos = (vec2<float>&) * CPlayerPos;
        CRot->phi = angle_between((vec2<float>&) * CDir, (vec2<float>&) * CPlayerDir);
        (vec2<float>&)* CDir = (vec2<float>&) * CPlayerDir;
        //Speed Update
        (vec2<float>&)* CVel = (vec2<float>&) * CDir * BULLET_SPEED;
        break;
    }
}

void sys::bulletCollideEnemy() {
    auto PlayerEnt = Reg.getPlayer();
    auto CAmmo = Reg.findComponentOrNull<cmp::Ammo>(PlayerEnt);
    for (int i = 0; i < AMMO_AMOUNT; ++i) {
        EntityId BulletEnt = CAmmo->ammo_store[i];
        auto CActive = Reg.findComponentOrNull<cmp::IsActive>(BulletEnt);
        auto CPos = Reg.findComponentOrNull<cmp::Position>(BulletEnt);
        if (!CActive->is_active || !CActive || !CPos)
            continue;

        auto& View = Reg.view<cmp::IsEnemy>();
        for (auto&& [EnemyEnt, _] : View) {
            auto CEnemyPos = Reg.findComponentOrNull<cmp::Position>(EnemyEnt);
            auto CEnemyActive = Reg.findComponentOrNull<cmp::IsActive>(EnemyEnt);
            if (!CEnemyPos || !(CEnemyActive->is_active))
                continue;
            if (((vec2<float>&) * CPos - (vec2<float>&) * CEnemyPos).sq_length() <= ENEMY_SQ_COLLIDE_DISTANCE) {
                CActive->is_active = false;
                auto CPlayerDamage = Reg.findComponentOrNull<cmp::Damage>(PlayerEnt);
                auto CEnemyHealth = Reg.findComponentOrNull<cmp::Health>(EnemyEnt);
                if (!CPlayerDamage || !CEnemyHealth)
                    continue;
                CEnemyHealth->curr_health -= CPlayerDamage->damage;
            }
        }
    }
}

void sys::playerCollideEnemy() {
    auto PlayerEnt = Reg.getPlayer();
    auto CPlayerPos = Reg.findComponentOrNull<cmp::Position>(PlayerEnt);
    auto CPlayerVel = Reg.findComponentOrNull<cmp::Velocity>(PlayerEnt);

    auto& View = Reg.view<cmp::IsEnemy>();
    for (auto&& [EnemyEnt, _] : View) {
        auto CEnemyPos = Reg.findComponentOrNull<cmp::Position>(EnemyEnt);
        auto CEnemyVel = Reg.findComponentOrNull<cmp::Velocity>(EnemyEnt);

        auto CEnemyActive = Reg.findComponentOrNull<cmp::IsActive>(EnemyEnt);

        if (!CEnemyActive->is_active || !CEnemyPos)
            continue;
        if (((vec2<float>&) * CPlayerPos - (vec2<float>&) * CEnemyPos).sq_length() <= SQ_COLLIDE_DISTANCE) {
            pushEnemy((vec2<float>&) * CEnemyVel);
            auto CPlayerHealth = Reg.findComponentOrNull<cmp::Health>(PlayerEnt);
            auto CEnemyDamage = Reg.findComponentOrNull<cmp::Damage>(EnemyEnt);
            if (!CPlayerHealth || !CEnemyDamage)
                continue;
            playerHasDamage( CEnemyDamage->damage);
        }
    }
}
void sys::playerHasDamage(int damage) {
    auto PlayerEnt = Reg.getPlayer();
    auto CPlayerHealth = Reg.findComponentOrNull<cmp::Health>(PlayerEnt);
    if (!CPlayerHealth)
        return;
    CPlayerHealth->curr_health -= damage;
    updatePlayerColor();
}

void sys::playerRegeneration(float dt) {
    auto PlayerEnt = Reg.getPlayer();
    auto CPlayerHealth = Reg.findComponentOrNull<cmp::Health>(PlayerEnt);
    if (!CPlayerHealth )
        return;
    if (CPlayerHealth->regeneration_time <= REGENERATION_TIME ||
        (CPlayerHealth->curr_health >= CPlayerHealth->max_health)) {
        CPlayerHealth->regeneration_time += dt;
        return;
    }   
    CPlayerHealth->curr_health ++;
    CPlayerHealth->regeneration_time = 0.f;
    updatePlayerColor();
}
void sys::updatePlayerColor() {
    auto PlayerEnt = Reg.getPlayer();
    auto CPlayerHealth = Reg.findComponentOrNull<cmp::Health>(PlayerEnt);
    auto CPlayerColor = Reg.findComponentOrNull<cmp::Color>(PlayerEnt);
    if (!CPlayerHealth || !CPlayerColor)
        return;
    unsigned char red = static_cast<unsigned char>(255 - 255 * CPlayerHealth->curr_health / CPlayerHealth->max_health);
    CPlayerColor->r = red;
    CPlayerColor->g = 255 - red;
}

void sys::pushEnemy(vec2<float>& enemy_speed) {
    enemy_speed *= (-PUSH_ENEMIES);
    if (enemy_speed.length() > 10 * ENEMY_MAX_SPEED)
        enemy_speed = enemy_speed.normalized() * 10 * ENEMY_MAX_SPEED;
}

void sys::ammoReload(float dt) {
    auto PlayerEnt = Reg.getPlayer();
    auto CShoot = Reg.findComponentOrNull<cmp::CanShoot>(PlayerEnt);
    if (CShoot->timer < FLOAT_PRECISE) {
        CShoot->can_shoot = true;
    }
    else {
        CShoot->timer-=dt;
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
void sys::turnTowardsPlayer(float dt) {
    auto& View = Reg.view<cmp::IsEnemy>();
    for (auto&& [EnemyEnt, Enemy] : View) {
        auto CRot = Reg.findComponentOrNull<cmp::Rotation>(EnemyEnt);
        auto CPos = Reg.findComponentOrNull<cmp::Position>(EnemyEnt);
        auto CDir = Reg.findComponentOrNull<cmp::Direction>(EnemyEnt);
        auto CPlayerPos = Reg.findComponentOrNull<cmp::Position>(Enemy.player_id);

        if (!CRot || !CPos || !CDir || !CPlayerPos)
            continue;
        CRot->phi = angle_between((vec2<float>&)*CDir, (vec2<float>&)*CPlayerPos - (vec2<float>&)*CPos);
        rotateVector(CRot->phi, (vec2<float>&)*CDir);
        //Speed Update
        auto CVel = Reg.findComponentOrNull<cmp::Velocity>(EnemyEnt);
        if (CVel && ((vec2<float>&) * CVel).length() < ENEMY_MAX_SPEED) {
            (vec2<float>&)* CVel += (vec2<float>&) * CDir * ENEMY_SPEED_SCALE * dt;
        }
    }
}
void sys::turnTowards(float dt) {
    sys::turnTowardsCursor();
    sys::turnTowardsPlayer(dt);
}


//draw(buffer)
void sys::draw(BuffTy Buffer) {

    auto& View = Reg.view<cmp::Sprite>();

    for (auto&& [Ent, Tr] : View) {
        auto CColor = Reg.findComponentOrNull<cmp::Color>(Ent);
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
                        continue;//out of bounds
                    buffer[j][i] = CColor->data;
                }
            }
        }
    }
}

