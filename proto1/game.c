#include "common.h"

byte Board[BOARD_WIDTH * BOARD_HEIGHT * 3];
Cell* CellList[CELL_COUNT];
int frame_count;

int _I(int i, int j)
{
    return (i*BOARD_WIDTH+j)*3;
}

uint get(byte* b, int i, int j)
{
    return (b[_I(i,j)] << 16) + (b[_I(i,j) + 1] << 8) + b[_I(i,j) + 2];
}

void set(byte* b, int i, int j, int value)
{
    b[_I(i,j)] = (byte) ((value & 0x00FF0000) >> 16);
    b[_I(i,j)+1] = (byte) ((value & 0x0000FF00) >> 8);
    b[_I(i,j)+2] = (byte) (value & 0x000000FF);
    //b[_I(i,j)+3] = (byte) (value & 0xFF000000) >> 24;
}

float RandomFloat(float precision)
{
    return rand() * (precision / RAND_MAX);
}

int randX()
{
    return rand()%(BOARD_WIDTH - DEFAULT_CELL_SIZE) + DEFAULT_CELL_SIZE;
}

int randY()
{
    return rand()%(BOARD_HEIGHT - DEFAULT_CELL_SIZE) + DEFAULT_CELL_SIZE;
}

Cell* InitCell()
{
    struct vec2 pos = {
        randX(),
        randY(),
    };

    Cell* cell = malloc(sizeof(Cell));

    cell->color = ALIVE;
    cell->pos = pos;
    cell->angle = RandomFloat(1.0f) * 2.0f * PI;

    return cell;
}

float SenseCell(Cell* cell, float sensorAngleOffset) 
{
    float sensorAngle = cell->angle + sensorAngleOffset;
    float sensorDirX = cos(sensorAngle);
    float sensorDirY = sin(sensorAngle);

    int centerX = cell->pos.x + sensorDirX * SENSOR_OFFSET_DST;
    int centerY = cell->pos.y + sensorDirY * SENSOR_OFFSET_DST;

    float sum = 0;

    for(int offX = -SENSOR_SIZE; offX <= SENSOR_SIZE; offX++)
    {
        for(int offY = -SENSOR_SIZE; offY <= SENSOR_SIZE; offY++)
        {
            int posX = centerX + offX;
            int posY = centerY + offY;

            if(posX >= 0 && posX <= BOARD_WIDTH && posY >= 0 && posY < BOARD_HEIGHT)
                sum += get(Board, posY, posX) / ALIVE;
        }
    }

    return sum;
}

void UpdateMove(Cell* cell)
{
    float angle = RandomFloat(1.0f) * 2.0f * PI;

    float weightForward = SenseCell(cell, 0);
    float weightLeft = SenseCell(cell, angle);
    float weightRight = SenseCell(cell, -angle);

    float randomSteerStrength = RandomFloat(1.0f);
    float turnSpeed = CELL_TURN_SPEED * 2.0f * PI;

    if (weightForward > weightLeft && weightForward > weightRight) {
        cell->angle += 0;
    }
    else if (weightForward < weightLeft && weightForward < weightRight) {
        cell->angle += (randomSteerStrength - 0.5) * 2 * turnSpeed;
    }
    // Turn right
    else if (weightRight > weightLeft) {
        cell->angle -= randomSteerStrength * turnSpeed;
    }
    // Turn left
    else if (weightLeft > weightRight) {
        cell->angle += randomSteerStrength * turnSpeed;
    }

    float dirX = cos(cell->angle);
    float dirY = sin(cell->angle);

    vec2 newPos;

    newPos.x = cell->pos.x + dirX * 2;
    newPos.y = cell->pos.y + dirY * 2;

    if(newPos.x <= DEFAULT_CELL_SIZE || 
            newPos.y <= DEFAULT_CELL_SIZE || 
            newPos.x >= BOARD_WIDTH - DEFAULT_CELL_SIZE || 
            newPos.y >= BOARD_HEIGHT - DEFAULT_CELL_SIZE)
    {
        newPos.x = fmin((float)BOARD_WIDTH - DEFAULT_CELL_SIZE, fmax(0, newPos.x));
        newPos.y = fmin((float)BOARD_HEIGHT - DEFAULT_CELL_SIZE, fmax(0, newPos.y));
        cell->angle = RandomFloat(1.0f) * 2.0f * PI;
    }

    cell->pos = newPos;
}

void DiffuseTrail(int x, int y, double deltaTime)
{    
    uint sum_r = 0;
    uint sum_g = 0;
    uint sum_b = 0;

    // 3x3 blur
    for (int offsetX = -1; offsetX <= 1; offsetX ++) {
        for (int offsetY = -1; offsetY <= 1; offsetY ++) {
            int sampleX = fmin(BOARD_HEIGHT - DEFAULT_CELL_SIZE, fmax(0, x + offsetX));
            int sampleY = fmin(BOARD_WIDTH - DEFAULT_CELL_SIZE, fmax(0, y + offsetY));
            
            uint col = get(Board, sampleX, sampleY);

            sum_r += (col & 0x00FF0000) >> 16;
            sum_g += (col & 0x0000FF00) >> 8;
            sum_b += col & 0x000000FF;
        }
    }

    uint blurredCol = (sum_r/9 << 16) + (sum_g/9 << 8) + (sum_b/9);

    set(Board, x, y, fmax(0, (uint)blurredCol));
}

void GameInit()
{
    for(int i = 0; i < CELL_COUNT; i++)
        CellList[i] = InitCell();
}

void GameUpdate(double deltaTime)
{
    for(int mov = 0; mov < DEFAULT_MOV_SPEED; mov++)
    {
        for(int i = 0; i < CELL_COUNT; i++)
        {
            UpdateMove(CellList[i]);
            set(Board, CellList[i]->pos.y, CellList[i]->pos.x, ALIVE);
        }
    }

    if(frame_count >= 0)
    {
        for(int i = 0; i < BOARD_HEIGHT; i++)
        {
            for(int j = 0; j < BOARD_WIDTH; j++)
            {
                uint value = EVAPORATE_SPEED;

                uint color = get(Board, i, j) - (uint)((value << 16) + (value << 8) + value);

                set(Board, i, j, color >= 0x00FFFFFF ? DEAD : color);
                DiffuseTrail(i, j, deltaTime);
            }
        }

        frame_count = 0;
    }  

    frame_count++;
}

void GameUnload()
{
    for(int i = 0; i < CELL_COUNT; i++)
        free(CellList[i]);
}
