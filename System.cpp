#include "System.h"


//act(dt)
void sys::act(float dt) {
    control(dt);
    TurnTowardsPoint();
    rotate(dt);
    move(dt);
    outOfBounds();
}
void sys::move(float dt) {
    auto& View = Reg.view<cmp::Velocity>();
    for (auto&& [Ent, Vel] : View) {
        auto CPos = Reg.findComponentOrNull<cmp::Position>(Ent);
        CPos->x += Vel.dx;
        CPos->y += Vel.dy;
    }
}

void sys::rotate(float dt) {
    auto& View = Reg.view<cmp::Rotation>();
    for (auto&& [Ent, Angle] : View) {
        auto CTr = Reg.findComponentOrNull<cmp::Sprite>(Ent);
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
    auto& View = Reg.view<cmp::Control>();
    for (auto&& [Ent, CControl] : View) {  
        auto CSpeed = Reg.findComponentOrNull<cmp::Velocity>(Ent);
        if (!CSpeed || !CControl.has_controller)
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
    }
}

void sys::TurnTowardsPoint() {
    auto& View = Reg.view<cmp::LookTowards>();
    for (auto&& [Ent, Dir] : View) {
        auto CRot = Reg.findComponentOrNull<cmp::Rotation>(Ent);
        auto CPos = Reg.findComponentOrNull<cmp::Position>(Ent);
        auto CControl = Reg.findComponentOrNull<cmp::Control>(Ent);
        if (!CRot || !CPos|| !CControl)
            continue;
        if (CControl->has_controller) {
            vec2<float> NewDir(get_cursor_x() - CPos->x, get_cursor_y() - CPos->y);
            CRot->phi = angle_between(Dir.dir, NewDir);
            rotateVector(CRot->phi, Dir.dir);
        }
    }
}

//draw(buffer)
void sys::draw(BuffTy Buffer) {

    auto& View = Reg.view<cmp::Sprite>();

    for (auto&& [Ent, Tr] : View) {
        auto CColor = Reg.findComponentOrNull<Color>(Ent);
        auto CPos = Reg.findComponentOrNull<cmp::Position>(Ent);
        if (!CPos || !CColor)
            continue;

        vec2<int> p(static_cast<int>(CPos->x), static_cast<int>(CPos->y));
        vec2<int> y = get_min_max(Tr.v1.y, Tr.v2.y, Tr.v3.y) + vec2<int>(p.y, p.y);
        vec2<int> x = get_min_max(Tr.v1.x, Tr.v2.x, Tr.v3.x) + vec2<int>(p.x, p.x);
        for (int j = y.x; j <= y.y; ++j)
            for (int i = x.x; i <= x.y; ++i) {
                if (point_in_triangle(vec2<int>(i, j), Tr.v1 + *CPos, Tr.v2 + *CPos, Tr.v3 + *CPos)) {
                    if (i<0 || i > SCREEN_WIDTH || j < 0 || j > SCREEN_HEIGHT)
                        continue;
                    buffer[j][i] = CColor->data;
                }
            }
    }
}