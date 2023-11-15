#include "Aimbot.h"
#include "stdio.h"

int8_t lastTarget = BAD_INDEX; //
int8_t smoothFor = BAD_INDEX;
float secondsSaver = 0;
float timerAimSmooth = 0;
float startPosition = 0.00f;
float startPositionY = 0.00f;
Vector3 lastView;

void aimbot()
{

    if (!setting.enableAimbot)
        return;

    int8_t i = BAD_INDEX;
    Vector2 angles = {0, 0};
    Vector2 aimspeed = {0, 0};

    if (isBadWeapon()) return; // knife or granade stop it

    if (setting.aimbotFocusMode == FROM_POSITION)
        i = getClosestTargetFromPosition();
    else
        i = getClosestTargetFromCrosshair();

    if (i == BAD_INDEX)
    {
        smoothFor = BAD_INDEX;
        return;
    }

    Vector3 ennemyPosition = getRelativePosition(localPlayer.position, entities[i].position);
    Vector2 perfectAngle = {computeYaw(ennemyPosition), computePitch(ennemyPosition)};

    // scale the aimSpeed by the time elapsed between frames : consistent across different frame rates. (NOT clean implementation here)
    QueryPerformanceCounter(&end);
    float elapsedTime = ((float)(end.QuadPart - start.QuadPart) / frequency.QuadPart);

    if (smoothFor == BAD_INDEX)
    {
        smoothFor = i;
        timerAimSmooth = 0;
        lastView = localPlayer.viewAngles;
    }

    float r = getRatio(timerAimSmooth * 1000, setting.aimTime);

    timerAimSmooth += elapsedTime;

    Vector2 smoothAngle = {SmoothAngle(lastView.x, perfectAngle.x, r), SmoothAngle(lastView.y, perfectAngle.y, r)};
    Vector2 angleDifference = {calculateAngularDifference(localPlayer.viewAngles.x, perfectAngle.x), perfectAngle.y - localPlayer.viewAngles.y};
    Vector2 imprecision = relativePrecision(getDistance(ennemyPosition));

    if (powf(angleDifference.x, 2.00f) > imprecision.x)
    { 
        updateYaw(smoothAngle.x); // not in dead zone
    }
    if (powf(angleDifference.y, 2.00f) > imprecision.y)
    {
        updatePitch(smoothAngle.y); // not in dead zone
    }
    else
    {
        smoothFor = BAD_INDEX;
    }

    return;
}

inline void updateViewAngles(Vector2 _angle)
{
    WriteProcessMemory(process.handle, &localPlayerPtr->viewAngles.x, &_angle, 8, NULL);
}

inline bool isBadWeapon()
{

    uint32_t i = getLocalPlayerWeaponIndex();

    if (i == GUN_KNIFE || i == GUN_GRENADE)
        return true;

    return false;
}

inline bool isValidAimedEntity(uint8_t _index)
{
    if (entities[_index].playerState != ALIVE)
        return false;
    if (setting.isTracingActive && !isEntityVisible(_index))
        return false;
    if (!setting.focusMate && entities[_index].team == localPlayer.team)
        return false;

    return true;
}

inline bool isEntityVisible(size_t _position)
{
    getEntitiesVisibilityValue();
    return !((entitiesVisibilityValue >> _position) & 0x01);
}

inline uint32_t getEntitiesVisibilityValue()
{
    ReadProcessMemory(process.handle, (LPCVOID)ENTITIES_VISIBILITY_CUSTOM, &entitiesVisibilityValue, 4, NULL);
    return entitiesVisibilityValue;
}

inline int8_t getClosestTargetFromPosition()
{

    float distance = 0.00f;
    float distance2 = 9999.00f;
    uint8_t entityIndex = 0;

    for (uint8_t i = 0; i < entityNbr - 1; i++)
    {
        if (!isValidAimedEntity(i))
            continue;

        distance = getDistance(getRelativePosition(localPlayer.position, entities[i].position));
        if (distance < distance2)
        {
            distance2 = distance;
            entityIndex = i;
        }
    }

    if (distance2 == 9999.00f)
        return BAD_INDEX; // no valid target

    return entityIndex;
}

inline int8_t getClosestTargetFromCrosshair()
{

    uint8_t index = BAD_INDEX;
    float distance = BAD_DISTANCE;
    Vector2 pos;
    Vector2 src;

    src.x = screen.width / 2;
    src.y = screen.height / 2;

    for (uint8_t i = 0; i < entityNbr - 1; i++)
    {

        if (!isValidAimedEntity(i) || !worldToScreen(entities[i].position, &pos))
            continue;
        // start from
        pos = getRelativePosition2d(pos, src); // convert to relative position
        distance = getDistance2d(pos);

        if (distance <= setting.radius)
        {
            index = i;
            break;
        }
    }

    if (distance == BAD_DISTANCE)
        return BAD_INDEX; // no valid target

    return index;
}

inline void updateYaw(float _yaw) { WriteProcessMemory(process.handle, &localPlayerPtr->viewAngles.x, &_yaw, 4, NULL); }
inline void updatePitch(float _pitch) { WriteProcessMemory(process.handle, &localPlayerPtr->viewAngles.y, &_pitch, 4, NULL); }

inline Vector2 relativePrecision(float _distance)
{
    Vector2 imprecision;
    _distance /= 80;

    imprecision.x = setting.aimInPrecisionX - _distance;
    imprecision.y = setting.aimInPrecisionX - _distance;

    return imprecision;
}