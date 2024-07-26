#include "System.h"

#include <bitset>

//act(dt)
void sys::act(float dt) { 

    transform(dt);
    //upgate
    playerControl(dt);
    EnemyAI(dt);
    //collisions:
    checkPlayerEnemyCollide();
    bulletCollideEnemy();

    updateHealth(dt);
    updateScenario(dt);

    
}


//Transform system
void sys::transform(float dt) {
    rotate();
    move(dt); 
    outOfBounds();
}
void sys::move(float dt) {
    auto& View = Reg.view<cmp::Velocity>();
    for (auto&& [Ent, Vel] : View) {
        auto CActive = Reg.findComponent<cmp::IsActive>(Ent);
        if (!CActive->is_active)
            continue;

        auto CPos = Reg.findComponent<cmp::Position>(Ent);
        CPos->position += Vel.velocity * dt;
        //for all entities that are not bullets, the velocity fades
        auto CBullet = Reg.findComponentOrNull<cmp::IsBullet>(Ent);
        if (!CBullet)
            Vel.velocity *= SPEED_FADE;
    }
}
void sys::rotate() {
    auto& View = Reg.view<cmp::Rotation>();
    for (auto&& [Ent, Angle] : View) {
        auto CTr = Reg.findComponent<cmp::TriangleSprite>(Ent);
        CTr->sprite.rotate(Angle.phi);
        Angle.phi = 0.f;
    }
}
void sys::outOfBounds() {
    auto& View = Reg.view<cmp::Position>();
    for (auto&& [Ent, Pos] : View) {
        bool IsOut = false;

        auto CActive = Reg.findComponent<cmp::IsActive>(Ent);
        if (!CActive->is_active)
            continue;

        auto CVel = Reg.findComponent<cmp::Velocity>(Ent);
        if (Pos.position.x < BOUND_WIDTH) {
            CVel->velocity.x = 0;
            Pos.position.x = BOUND_WIDTH;
            IsOut = true;
        }
        if (Pos.position.y < BOUND_WIDTH) {
            CVel->velocity.y = 0;
            Pos.position.y = BOUND_WIDTH;
            IsOut = true;
        }
        if (Pos.position.x > SCREEN_WIDTH - BOUND_WIDTH) {
            CVel->velocity.x = 0;
            Pos.position.x = SCREEN_WIDTH - BOUND_WIDTH;
            IsOut = true;
        }
        if (Pos.position.y > SCREEN_HEIGHT - BOUND_WIDTH) {
            CVel->velocity.y = 0;
            Pos.position.y = SCREEN_HEIGHT - BOUND_WIDTH;
            IsOut = true;
        }
        //if the bullet has reached the border, then it returns to the ammo storage
        auto CBullet = Reg.findComponentOrNull<cmp::IsBullet>(Ent);
        if (CBullet && IsOut) { // until the bullet goes beyond the border, it cannot be used
            CActive->is_active = false;
        }
    }
}
void sys::turnTowards(EntityId EntId, vec2<float> Pos) {
    auto CRot = Reg.findComponent<cmp::Rotation>(EntId);
    auto CPos = Reg.findComponent<cmp::Position>(EntId);
    auto CDir = Reg.findComponent<cmp::Direction>(EntId);
    CRot->phi = angle_between(CDir->direction, Pos - CPos->position);
    rotateVector(CRot->phi, CDir->direction);
}
void sys::moveInDir(EntityId EntId, float dt) {
    auto CDir = Reg.findComponent<cmp::Direction>(EntId);
    auto CVel = Reg.findComponent<cmp::Velocity>(EntId);
    if ((CVel->velocity).length() > CVel->max_speed)
        return;
    if (CVel->speed_scale > 0)
        CVel->velocity += CDir->direction * (CVel->speed_scale * dt);
    else
        CVel->velocity = CDir->direction * CVel->max_speed;
}
bool sys::isEntitiesCollide(EntityId EntId1, EntityId EntId2) {
    auto CActive1 = Reg.findComponent<cmp::IsActive>(EntId1);
    auto CActive2 = Reg.findComponent<cmp::IsActive>(EntId2);

    if (!(CActive1->is_active) || !(CActive2->is_active))
        return false;
    auto CPos1 = Reg.findComponent<cmp::Position>(EntId1);
    auto CColl1 = Reg.findComponent<cmp::Collision>(EntId1);
    auto CPos2 = Reg.findComponent<cmp::Position>(EntId2);
    auto CColl2 = Reg.findComponent<cmp::Collision>(EntId2);

    float Dist = CColl1->collide_distance + CColl2->collide_distance;
    return (CPos1->position - CPos2->position).sq_length() < Dist * Dist;
}

//Player act system
void sys::playerControl(float dt) {
    control(dt);
    turnTowardsCursor();
    ammoReload(dt);
}
void sys::control(float dt) {
    auto PlayerEnt = Reg.getPlayer();
    auto CVel = Reg.findComponent<cmp::Velocity>(PlayerEnt);

    float Scale = SPEED_SCALE * dt;
    if (is_key_pressed('D')) {
        if (CVel->velocity.x <= MAX_SPEED)
            CVel->velocity.x += Scale;
    }
    if (is_key_pressed('A')) {
        if (CVel->velocity.x >= -MAX_SPEED)
            CVel->velocity.x -= Scale;
    }
    if (is_key_pressed('S')) {
        if (CVel->velocity.y <= MAX_SPEED)
            CVel->velocity.y += Scale;
    }
    if (is_key_pressed('W')) {
        if (CVel->velocity.y >= -MAX_SPEED)
            CVel->velocity.y -= Scale;
    }
    if (is_mouse_button_pressed(0)) {
        auto CShoot = Reg.findComponent<cmp::CanShoot>(PlayerEnt);
        if (CShoot->can_shoot) {
            sys::shoot(dt);
            CShoot->reload_timer = AMMO_RELOAD;
            CShoot->can_shoot = false;
        }   
    }
}
void sys::turnTowardsCursor() {
    auto PlayerEnt = Reg.getPlayer();
    turnTowards(PlayerEnt, vec2<float>(static_cast<float>(get_cursor_x()), static_cast<float>(get_cursor_y())));
}
void sys::shoot(float dt) {
    auto PlayerEnt = Reg.getPlayer();
    auto CPlayerPos = Reg.findComponent<cmp::Position>(PlayerEnt);
    auto CPlayerDir = Reg.findComponent<cmp::Direction>(PlayerEnt);
    auto CAmmo = Reg.findComponent<cmp::Ammo>(PlayerEnt);
    for (int i = 0; i < AMMO_AMOUNT; ++i) {
        EntityId BulletEnt = CAmmo->ammo_store[i];
        auto CActive = Reg.findComponent<cmp::IsActive>(BulletEnt);
        auto CPos = Reg.findComponent<cmp::Position>(BulletEnt);

        if (CActive->is_active || !CPos)
            continue;
        CActive->is_active = true;
        CPos->position = CPlayerPos->position;

        turnTowards(BulletEnt, CPlayerPos->position + CPlayerDir->direction);
        moveInDir(BulletEnt, dt);
        break; // need to find first not active bullet
    }
}
void sys::ammoReload(float dt) {
    auto PlayerEnt = Reg.getPlayer();
    auto CShoot = Reg.findComponent<cmp::CanShoot>(PlayerEnt);
    if (CShoot->reload_timer < FLOAT_PRECISE) {
        CShoot->can_shoot = true;
    } else {
        CShoot->reload_timer -= dt;
    }
}

//Enemy AI
void sys::EnemyAI(float dt) {
    auto PlayerEnt = Reg.getPlayer();
    auto CPlayerPos = Reg.findComponent<cmp::Position>(PlayerEnt);
    auto& View = Reg.view<cmp::IsEnemy>();
    for (auto&& [EnemyEnt, Enemy] : View) {
        turnTowards(EnemyEnt, CPlayerPos->position);
        moveInDir(EnemyEnt, dt);
    }
}

//Bullet->Enemy system
void sys::bulletCollideEnemy() {
    auto PlayerEnt = Reg.getPlayer();
    auto CAmmo = Reg.findComponent<cmp::Ammo>(PlayerEnt);
    for (int i = 0; i < AMMO_AMOUNT; ++i) {
        EntityId BulletEnt = CAmmo->ammo_store[i];
        auto CActive = Reg.findComponent<cmp::IsActive>(BulletEnt);
        auto& View = Reg.view<cmp::IsEnemy>();
        for (auto&& [EnemyEnt, _] : View) {
        
            if(!isEntitiesCollide(BulletEnt, EnemyEnt))
                continue;
            CActive->is_active = false;
            getDamage(EnemyEnt, PlayerEnt);
            
        }
    }
}
//Enemy->Player system
void sys::checkPlayerEnemyCollide() {
    auto PlayerEnt = Reg.getPlayer();
    auto CPlayerPos = Reg.findComponent<cmp::Position>(PlayerEnt);
    auto CPlayerColl = Reg.findComponent<cmp::Collision>(PlayerEnt);
    auto& View = Reg.view<cmp::IsEnemy>();
    for (auto&& [EnemyEnt, EnemyCost] : View) {
        auto CEnemyPos = Reg.findComponent<cmp::Position>(EnemyEnt);
        auto CEnemyColl = Reg.findComponent<cmp::Collision>(EnemyEnt);
        auto CEnemyActive = Reg.findComponent<cmp::IsActive>(EnemyEnt);
        if (!CEnemyActive->is_active)
            continue;
        float Dist = CPlayerColl->collide_distance + CEnemyColl->collide_distance;
        if ((CPlayerPos->position - CEnemyPos->position).sq_length() > Dist * Dist)
            continue;
        PlayerEnemyCollide(EnemyEnt);
    }
}
void sys::PlayerEnemyCollide(EntityId& enemy_id) {
    auto CEnemyVel = Reg.findComponent<cmp::Velocity>(enemy_id);
    pushEnemy(CEnemyVel->velocity);
    getDamage(Reg.getPlayer(), enemy_id);
}
void sys::pushEnemy(vec2<float>& EnemySpeed) {
    EnemySpeed *= (-PUSH_ENEMIES);
    if (EnemySpeed.length() > 10 * ENEMY_MAX_SPEED)
        EnemySpeed = EnemySpeed.normalized() * 10 * ENEMY_MAX_SPEED;
}
void sys::killEnemy(EntityId enemy_id) {
    auto PlayerEnt = Reg.getPlayer();
    auto CScore = Reg.findComponent<cmp::IsPlayer>(PlayerEnt);
    auto CEnemy = Reg.findComponent<cmp::IsEnemy>(enemy_id);
    CScore->curr_score += CEnemy->cost;
}

// Health system
void sys::updateHealth(float dt) {
    checkHealth();
    Regeneration(dt);
    diaplayPlayerHealth();
}
void sys::checkHealth() {
    auto PlayerEnt = Reg.getPlayer();
    auto& View = Reg.view<cmp::Health>();
   
    for (auto&& [Ent, CHealth] : View) {
        auto CActive = Reg.findComponent<cmp::IsActive>(Ent);
        if (!CActive->is_active || CHealth.curr_health > 0)
            continue;
        CActive->is_active = false;

        if (auto CEnemy = Reg.findComponentOrNull<cmp::IsEnemy>(Ent))
            killEnemy(Ent); // enemy health <= 0
        else if (auto CPlayer = Reg.findComponentOrNull<cmp::IsPlayer>(Ent))
            schedule_quit_game(); // here should be die screen
    }
}
void sys::Regeneration(float dt) {
    auto& View = Reg.view<cmp::Health>();
    for (auto&& [Ent, CHealth] : View) {
        if (!(CHealth.can_regenerate) || (CHealth.curr_health >= CHealth.max_health))
            continue;
        if (CHealth.curr_regeneration_time < CHealth.regeneration_time) {
            CHealth.curr_regeneration_time += dt;
            continue;
        }
        CHealth.curr_health++;
        CHealth.curr_regeneration_time = 0.f;
    }
}
void sys::diaplayPlayerHealth() {
    auto PlayerEnt = Reg.getPlayer();
    auto CPlayerHealth = Reg.findComponent<cmp::Health>(PlayerEnt);
    auto CPlayerColor = Reg.findComponent<cmp::Color>(PlayerEnt);
    if (CPlayerHealth->curr_health >= CPlayerHealth->max_health)
        return;
    unsigned char red = static_cast<unsigned char>(255 - 255 * CPlayerHealth->curr_health / CPlayerHealth->max_health);
    CPlayerColor->r = red;
    CPlayerColor->g = 255 - red;
}
void sys::getDamage(EntityId EntId, EntityId damager_id) {
    auto CHealth = Reg.findComponent<cmp::Health>(EntId);
    auto CDamage = Reg.findComponent<cmp::Damage>(damager_id);
    CHealth->curr_health -= CDamage->damage;
}

// Scenario
void sys::updateScenario(float dt) {
    auto PlayerEnt = Reg.getPlayer();
    auto& View = Reg.view<cmp::Progress>();
    auto PScore = Reg.findComponent<cmp::IsPlayer>(PlayerEnt);
    for (auto&& [ScenarioEnt, Progress] : View) {
        auto CEnemies = Reg.findComponentOrNull<cmp::Enemies>(ScenarioEnt);
        if (!CEnemies)
            continue;
        Progress.curr_time += dt;
        Progress.score = PScore->curr_score;
        Progress.timer_before_act -= dt;
        if (PScore->curr_score <= ENEMY_AMOUNT / 2 || Progress.timer_before_act > FLOAT_PRECISE)
            continue;
        Progress.timer_before_act = 1.f;
        for (int i = 0; i < CEnemies->enemy_store.size()-ENEMY_AMOUNT; ++i) {
            EntityId Enemy = CEnemies->enemy_store[i];
            ActivatelEnemy(Enemy);
        }
        if(Progress.curr_time > 10.f)
            for (int i = ENEMY_AMOUNT-1; i < CEnemies->enemy_store.size() ; ++i) {
                EntityId Enemy = CEnemies->enemy_store[i];
                ActivatelEnemy(Enemy);
            }
    }
}
void sys::ActivatelEnemy(EntityId enemy_id) {
    auto CEnemyActive = Reg.findComponent<cmp::IsActive>(enemy_id);
    if (CEnemyActive->is_active)
        return;

    auto CEnemyHealth = Reg.findComponent<cmp::Health>(enemy_id);
    auto CEnemyPos = Reg.findComponent<cmp::Position>(enemy_id);
    CEnemyActive->is_active = true;
    CEnemyHealth->curr_health = CEnemyHealth->max_health;
    float x_pos = random(BOUND_WIDTH, BOUND_WIDTH + 50);
    float y_pos = random(BOUND_WIDTH, BOUND_WIDTH + 50);
    CEnemyPos->position = vec2<float>(x_pos, y_pos);
    turnTowards(enemy_id, CEnemyPos->position + vec2<float>(0.f, 1.f));
}

static std::bitset<7> getBits(char Digit) {
    switch (Digit)
    {
    case 0:
        return 0b1011111;
    case 1:
        return 0b0001100;
    case 2:
        return 0b1110110;
    case 3:
        return 0b1111100;
    case 4:
        return 0b0101101;
    case 5:
        return 0b1111001;
    case 6:
        return 0b1111011;
    case 7:
        return 0b0011100;
    case 8:
        return 0b1111111;
    case 9:
        return 0b1111101;
    default:
        throw std::runtime_error{ "unexpected digit" };
    }
}

static void drawScore(BuffTy Buffer) {

    auto Player = Reg.getPlayer();
    auto Score = Reg.findComponent<cmp::IsPlayer>(Player)->curr_score;

    
    static constexpr int LWidth =  SCALE_SCORE;
    static constexpr int LHeight = 5 * SCALE_SCORE;

    static constexpr int PlateWidth = 6 * SCALE_SCORE;
    static constexpr int PlateHeigh = 12 * SCALE_SCORE;
    static constexpr int PosX = SCREEN_WIDTH -20 - PlateWidth - LWidth*2;
    static constexpr int PosY = 20+ LHeight + LWidth * 2;
    static constexpr int xOffset = PlateWidth + LWidth*3;
    int currOff = 0;
    do {

        if (currOff + PosX + PlateWidth > SCREEN_WIDTH ||  PosY + PlateHeigh > SCREEN_HEIGHT)
            return;

        auto Digit = Score % 10;

        auto Bits = getBits(Digit);
        Rectangle r;
        if (Bits[0]) {
            
            auto LTX = PosX - PlateWidth / 2 + currOff;
            auto LTY = PosY - PlateHeigh / 2;
             r = Rectangle(LTX, LTY, LTX + LWidth, LTY + LHeight + LWidth * 2);
            r.draw(buffer);

        }
        if (Bits[1]) {
            auto LTX = PosX - PlateWidth / 2 + currOff;
            auto LTY = PosY;

            r = Rectangle(LTX, LTY, LTX + LWidth, LTY + LHeight + LWidth * 2);
            r.draw(buffer);
        }
        if (Bits[2]) {
            auto LTX = PosX + PlateWidth / 2 + currOff;
            auto LTY = PosY - PlateHeigh / 2;

            r = Rectangle(LTX, LTY, LTX + LWidth, LTY + LHeight + LWidth * 2);
            r.draw(buffer);
        }
        if (Bits[3]) {
            auto LTX = PosX + PlateWidth / 2 + currOff;
            auto LTY = PosY;

            r = Rectangle(LTX, LTY, LTX + LWidth, LTY + LHeight + LWidth * 2);
            r.draw(buffer);
        }
        if (Bits[4]) {
            auto LTX = PosX - PlateWidth / 2 + currOff;
            auto LTY = PosY - PlateHeigh / 2;
            r = Rectangle(LTX, LTY, LTX + LHeight + LWidth*2, LTY + LWidth);
            r.draw(buffer);
 
        }
        if (Bits[5]) {
            auto LTX = PosX - PlateWidth / 2 + currOff;
            auto LTY = PosY;

            r = Rectangle(LTX, LTY, LTX + LHeight + LWidth*2, LTY + LWidth);
            r.draw(buffer);
        }
        if (Bits[6]) {
            auto LTX = PosX - PlateWidth / 2 + currOff;
            auto LTY = PosY + PlateHeigh / 2;

            r = Rectangle(LTX, LTY, LTX + LHeight + LWidth*2, LTY + LWidth);
            r.draw(buffer);
        }

        Score /= 10;
        currOff -= xOffset;
    } while (Score > 0);

}

//draw(buffer)
void sys::draw(BuffTy Buffer) {

    auto& View = Reg.view<cmp::TriangleSprite>();

    for (auto&& [Ent, Tr] : View) {
        auto CColor = Reg.findComponent<cmp::Color>(Ent);
        auto CPos = Reg.findComponent<cmp::Position>(Ent);
        auto CActive = Reg.findComponent<cmp::IsActive>(Ent);

        if (!CActive->is_active)
            continue;

        vec2<int> p(static_cast<int>(CPos->position.x), static_cast<int>(CPos->position.y));
        vec2<int> x = get_min_max(Tr.sprite.v1.x, Tr.sprite.v2.x, Tr.sprite.v3.x) + vec2<int>(p.x, p.x); // x = (x_min, x_max)
        vec2<int> y = get_min_max(Tr.sprite.v1.y, Tr.sprite.v2.y, Tr.sprite.v3.y) + vec2<int>(p.y, p.y); // y = (y_min, y_max)

        for (int j = y.x; j <= y.y; ++j) {
            for (int i = x.x; i <= x.y; ++i) {
                if (point_in_triangle(vec2<int>(i, j) - p, Tr.sprite.v1, Tr.sprite.v2, Tr.sprite.v3)) {
                    if (i < 0 || i >= SCREEN_WIDTH || j < 0 || j >= SCREEN_HEIGHT)
                        continue;//out of bounds
                    buffer[j][i] = CColor->data;
                }
            }
        }
    }

    drawScore(Buffer);
}
