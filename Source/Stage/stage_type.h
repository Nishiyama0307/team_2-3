#pragma once

#include "stage.h"



// 部屋のステージクラス

class Stage1_Volcano : public Stage
{
public:
    Stage1_Volcano();
    ~Stage1_Volcano();

};

class Stage2_Desert : public Stage
{
public:
    Stage2_Desert();
    ~Stage2_Desert();

};

class Stage3_Volcano : public Stage
{
public:
    Stage3_Volcano();
    ~Stage3_Volcano();

};

class Stage4_Volcano : public Stage
{
public:
    Stage4_Volcano();
    ~Stage4_Volcano();

};

class castle : public Stage
{
public:
    castle();
    ~castle();

};

class All_Stage : public Stage
{
public:
    All_Stage();
    ~All_Stage();

};

class Sky : public Stage
{
public:
    Sky();
    ~Sky();

};