#include "GameUtil.h"
#include "Types.h"

//设置人物坐标，若输入值为负，相当于从人物层清除
void Role::setPosition(int x, int y)
{
    if (position_layer_ == nullptr)
    {
        return;
    }
    if (X_ >= 0 && Y_ >= 0)
    {
        position_layer_->data(X_, Y_) = nullptr;
    }
    if (x >= 0 && y >= 0)
    {
        position_layer_->data(x, y) = this;
    }
    X_ = x;
    Y_ = y;
}

//显示用的，比内部数组用的多1
int Role::getRoleShowLearnedMagicLevel(int i)
{
    return getRoleMagicLevelIndex(i) + 1;
}

//获取武学等级，返回值是0~9，可以直接用于索引武功的威力等数据
int Role::getRoleMagicLevelIndex(int i)
{
    int l = MagicLevel[i] / 100;
    if (l < 0)
    {
        l = 0;
    }
    if (l > 9)
    {
        l = 9;
    }
    return l;
}

//已学习武学的数量
int Role::getLearnedMagicCount()
{
    int n = 0;
    for (int i = 0; i < ROLE_MAGIC_COUNT; i++)
    {
        if (MagicID[i] > 0)
        {
            n++;
        }
    }
    return n;
}

//依据武学指针获取等级，-1表示未学得
int Role::getMagicLevelIndex(Magic* magic)
{
    return getMagicLevelIndex(magic->ID);
}

int Role::getMagicLevelIndex(int magic_id)
{
    for (int i = 0; i < ROLE_MAGIC_COUNT; i++)
    {
        if (MagicID[i] == magic_id)
        {
            return getRoleMagicLevelIndex(i);
        }
    }
    return -1;
}

//武学在角色的栏位编号
int Role::getMagicOfRoleIndex(Magic* magic)
{
    for (int i = 0; i < ROLE_MAGIC_COUNT; i++)
    {
        if (MagicID[i] == magic->ID)
        {
            return i;
        }
    }
    return -1;
}

//限制人物的属性
void Role::limit()
{
    auto r_max = Role::getMaxValue();
    GameUtil::limit2(Level, 0, r_max->Level);

    GameUtil::limit2(Exp, 0, r_max->Exp);
    GameUtil::limit2(ExpForItem, 0, r_max->Exp);
    GameUtil::limit2(ExpForMakeItem, 0, r_max->Exp);

    GameUtil::limit2(Poison, 0, r_max->Poison);

    GameUtil::limit2(MaxHP, 0, r_max->HP);
    GameUtil::limit2(MaxMP, 0, r_max->MP);
    GameUtil::limit2(HP, 0, MaxHP);
    GameUtil::limit2(MP, 0, MaxMP);
    GameUtil::limit2(PhysicalPower, 0, r_max->PhysicalPower);

    GameUtil::limit2(Attack, 0, r_max->Attack);
    GameUtil::limit2(Defence, 0, r_max->Defence);
    GameUtil::limit2(Speed, 0, r_max->Speed);

    GameUtil::limit2(Medcine, 0, r_max->Medcine);
    GameUtil::limit2(UsePoison, 0, r_max->UsePoison);
    GameUtil::limit2(Detoxification, 0, r_max->Detoxification);
    GameUtil::limit2(AntiPoison, 0, r_max->AntiPoison);

    GameUtil::limit2(Fist, 0, r_max->Fist);
    GameUtil::limit2(Sword, 0, r_max->Sword);
    GameUtil::limit2(Knife, 0, r_max->Knife);
    GameUtil::limit2(Unusual, 0, r_max->Unusual);
    GameUtil::limit2(HiddenWeapon, 0, r_max->HiddenWeapon);

    GameUtil::limit2(Knowledge, 0, r_max->Knowledge);
    GameUtil::limit2(Morality, 0, r_max->Morality);
    GameUtil::limit2(AttackWithPoison, 0, r_max->AttackWithPoison);
    GameUtil::limit2(Fame, 0, r_max->Fame);
    GameUtil::limit2(IQ, 0, r_max->IQ);

    for (int i = 0; i < ROLE_MAGIC_COUNT; i++)
    {
        GameUtil::limit2(MagicLevel[i], 0, MAX_MAGIC_LEVEL);
    }
}

int Role::learnMagic(Magic* magic)
{
    if (magic == nullptr || magic->ID <= 0)
    {
        return -1;
    }    //武学id错误
    return learnMagic(magic->ID);
}

int Role::learnMagic(int magic_id)
{
    if (magic_id <= 0)
    {
        return -1;
    }
    //检查是否已经学得
    int index = -1;
    for (int i = 0; i < ROLE_MAGIC_COUNT; i++)
    {
        if (MagicID[i] == magic_id)
        {
            if (MagicLevel[i] / 100 < MAX_MAGIC_LEVEL_INDEX)
            {
                MagicLevel[i] += 100;
                return 0;
            }
            else
            {
                return -2;    //满级
            }
        }
        //记录最靠前的空位
        if (MagicID[i] <= 0 && index == -1)
        {
            index = i;
        }
    }

    if (index < 0)
    {
        return -3;    //若进行到此index为负，表示武学栏已满
    }
    else
    {
        //增加武学
        MagicID[index] = magic_id;
        MagicLevel[index] = 0;
        return 0;
    }
}

void Role::setMaxValue(INIReader* ini)
{
#define GET_VALUE_INT(v, default_v)                                 \
    do                                                              \
    {                                                               \
        max_role_value_.v = ini->getInt("constant", #v, default_v); \
        printf("%s = %d\n", #v, max_role_value_.v);                 \
    } while (0)

    printf("Max values of roles: \n");

    GET_VALUE_INT(Level, 30);
    GET_VALUE_INT(HP, 999);
    GET_VALUE_INT(MP, 999);
    GET_VALUE_INT(PhysicalPower, 100);

    GET_VALUE_INT(Poison, 100);

    GET_VALUE_INT(Attack, 100);
    GET_VALUE_INT(Defence, 100);
    GET_VALUE_INT(Speed, 100);

    GET_VALUE_INT(Medcine, 100);
    GET_VALUE_INT(UsePoison, 100);
    GET_VALUE_INT(Detoxification, 100);
    GET_VALUE_INT(AntiPoison, 100);

    GET_VALUE_INT(Fist, 100);
    GET_VALUE_INT(Sword, 100);
    GET_VALUE_INT(Knife, 100);
    GET_VALUE_INT(Unusual, 100);
    GET_VALUE_INT(HiddenWeapon, 100);

    GET_VALUE_INT(Knowledge, 100);
    GET_VALUE_INT(Morality, 100);
    GET_VALUE_INT(AttackWithPoison, 100);
    GET_VALUE_INT(Fame, 999);
    GET_VALUE_INT(IQ, 100);

    GET_VALUE_INT(Exp, 99999);

    printf("\n");

#undef GET_VALUE_INT
}

Role Role::max_role_value_;

//设置某个事件的坐标，在一些MOD里面此语句有错误
void SubMapEvent::setPosition(int x, int y, SubMapInfo* submap_record)
{
    if (x < 0)
    {
        x = X_;
    }
    if (y < 0)
    {
        y = Y_;
    }
    auto index = submap_record->EventIndex(X_, Y_);
    submap_record->EventIndex(X_, Y_) = -1;
    X_ = x;
    Y_ = y;
    submap_record->EventIndex(X_, Y_) = index;
}

int Magic::calMaxLevelIndexByMP(int mp, int max_level)
{
    max_level = GameUtil::limit(max_level, 0, MAX_MAGIC_LEVEL_INDEX);
    if (NeedMP <= 0)
    {
        return max_level;
    }
    int level = GameUtil::limit(mp / (NeedMP * 2) * 2 - 1, 0, max_level);
    return level;
}

int Item::MoneyItemID = 174;
int Item::CompassItemID = 182;

bool Item::isCompass()
{
    return ID == CompassItemID;
}

void Item::setSpecialItems(INIReader* ini)
{
#define GET_VALUE_INT(v)                    \
    do                                      \
    {                                       \
        v = ini->getInt("constant", #v, v); \
        printf("%s = %d\n", #v, v);         \
    } while (0)

    GET_VALUE_INT(MoneyItemID);
    GET_VALUE_INT(CompassItemID);

#undef GET_VALUE_INT
}
