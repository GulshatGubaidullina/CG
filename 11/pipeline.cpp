#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "math_3d.h"
#include <glm/glm.hpp>
#include "pipeline.h"
#include "math_3d.cpp"

using namespace glm;

#define ToRadian(x) ((x) * 3.141593f / 180.0f)

void Pipeline::InitScaleTransform(glm::mat4& m) const
{
    m[0][0] = m_scale.x; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
    m[1][0] = 0.0f; m[1][1] = m_scale.y; m[1][2] = 0.0f; m[1][3] = 0.0f;
    m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = m_scale.z; m[2][3] = 0.0f;
    m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
}

void Pipeline::InitRotateTransform(glm::mat4& m) const
{
    glm::mat4 rx, ry, rz;

    const float x = ToRadian(m_rotateInfo.x);
    const float y = ToRadian(m_rotateInfo.y);
    const float z = ToRadian(m_rotateInfo.z);

    rx[0][0] = 1.0f; rx[0][1] = 0.0f; rx[0][2] = 0.0f; rx[0][3] = 0.0f;
    rx[1][0] = 0.0f; rx[1][1] = cosf(x); rx[1][2] = -sinf(x); rx[1][3] = 0.0f;
    rx[2][0] = 0.0f; rx[2][1] = sinf(x); rx[2][2] = cosf(x); rx[2][3] = 0.0f;
    rx[3][0] = 0.0f; rx[3][1] = 0.0f; rx[3][2] = 0.0f; rx[3][3] = 1.0f;

    ry[0][0] = cosf(y); ry[0][1] = 0.0f; ry[0][2] = -sinf(y); ry[0][3] = 0.0f;
    ry[1][0] = 0.0f; ry[1][1] = 1.0f; ry[1][2] = 0.0f; ry[1][3] = 0.0f;
    ry[2][0] = sinf(y); ry[2][1] = 0.0f; ry[2][2] = cosf(y); ry[2][3] = 0.0f;
    ry[3][0] = 0.0f; ry[3][1] = 0.0f; ry[3][2] = 0.0f; ry[3][3] = 1.0f;

    rz[0][0] = cosf(z); rz[0][1] = -sinf(z); rz[0][2] = 0.0f; rz[0][3] = 0.0f;
    rz[1][0] = sinf(z); rz[1][1] = cosf(z); rz[1][2] = 0.0f; rz[1][3] = 0.0f;
    rz[2][0] = 0.0f; rz[2][1] = 0.0f; rz[2][2] = 1.0f; rz[2][3] = 0.0f;
    rz[3][0] = 0.0f; rz[3][1] = 0.0f; rz[3][2] = 0.0f; rz[3][3] = 1.0f;

    m = rz * ry * rx;
}

void Pipeline::InitTranslationTransform(glm::mat4& m) const
{
    m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = m_worldPos.x;
    m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = m_worldPos.y;
    m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = m_worldPos.z;
    m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
}
void Pipeline::InitTranslationTransformC(glm::mat4& m) const
{
    m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = -m_camera.Pos.x;
    m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = -m_camera.Pos.y;
    m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = -m_camera.Pos.z;
    m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
}

void Pipeline::InitPerspectiveProj(glm::mat4& m) const
{
    const float ar = m_persProj.Width / m_persProj.Height;
    const float zNear = m_persProj.zNear;
    const float zFar = m_persProj.zFar;
    const float zRange = zNear - zFar;
    const float tanHalfFOV = tanf(ToRadian(m_persProj.FOV / 2.0));

    m[0][0] = 1.0f / (tanHalfFOV * ar);
    m[0][1] = 0.0f;
    m[0][2] = 0.0f;
    m[0][3] = 0.0f;

    m[1][0] = 0.0f;
    m[1][1] = 1.0f / tanHalfFOV;
    m[1][2] = 0.0f;
    m[1][3] = 0.0f;

    m[2][0] = 0.0f;
    m[2][1] = 0.0f;
    m[2][2] = (-zNear - zFar) / zRange;
    m[2][3] = 2.0f * zFar * zNear / zRange;

    m[3][0] = 0.0f;
    m[3][1] = 0.0f;
    m[3][2] = 1.0f;
    m[3][3] = 0.0f;
}

void Pipeline::InitCameraTransform(glm::mat4& m) const //, const glm::vec3& Target, const glm::vec3& Up
{
    glm::vec3 N = m_camera.Target;
    /*const float Length = sqrtf(N[0] * N[0] + N[1] * N[1] + N[2] * N[2]);

    N[0] /= Length;
    N[1] /= Length;
    N[2] /= Length;*/

    N= glm::normalize(N);

    glm::vec3 U = m_camera.Up;

    U = glm::normalize(U);

    /*const float Length2 = sqrtf(U[0] * U[0] + U[1] * U[1] + U[2] * U[2]);

    U[0] /= Length2;
    U[1] /= Length2;
    U[2] /= Length2;*/



    //glm::vec3& u = U;
    glm::vec3 n= N;
    //U = Cross(U, N);

    //glm::vec3 glm::cross(const glm::vec3 &U, const glm::vec3 &N);

    U = glm::cross(U,N);
    //U*N
    /*U[1] * N[2] - N[1] * U[2];
    U[2] * N[0] - N[2] * U[0];
    U[0] * N[1] - N[0] * U[1];*/

    /*U[0] = U[1] * N[2] - U[2] * N[1];
    U[1] = U[2] * N[1] - U[0] * N[2];
    U[2] = U[0] * N[1] - U[1] * N[0];

    n[0] = n[1] * U[2] - n[2] *U[1];
    n[1] = n[2] * U[1] - n[0] * U[2];
    n[2] = n[0] * U[1] - n[1] * U[0];*/
    glm::vec3 V = glm::cross(N, U);
    //N*U
    /*V[0] = N[1] * U[2] - U[1] * N[2];
    V[1] = N[2] * U[0] - U[2] * N[0];
    V[2] = N[0] * U[1] - U[0] * N[1];*/
   

   

    m[0][0] = U[0];   m[0][1] = U[1];   m[0][2] = U[2];   m[0][3] = 0.0f;
    m[1][0] = V[0];   m[1][1] = V[1];   m[1][2] = V[2];   m[1][3] = 0.0f;
    m[2][0] = N[0];   m[2][1] = N[1];   m[2][2] = N[2];   m[2][3] = 0.0f;
    m[3][0] = 0.0f;  m[3][1] = 0.0f;  m[3][2] = 0.0f;  m[3][3] = 1.0f;
}

/*glm::vec3 Cross(glm::vec3& u, const glm::vec3& n)
{
    u[0] = u[1] * n[2] - u[2] * n[1];
    u[1] = u[2] * n[1] - u[0] * n[2];
    u[2] = u[0] * n[1] - u[1] * n[0];
    return u;
}*/

void Pipeline::InitPersProjTransform(glm::mat4& m) //, float FOV, float Width, float Height, float zNear, float zFar
{
    const float ar = m_persProj.Width / m_persProj.Height;
    const float zRange = m_persProj.zNear - m_persProj.zFar;
    const float tanHalfFOV = tanf(ToRadian(m_persProj.FOV / 2.0f));
    m[0][0] = 1.0f / (tanHalfFOV * ar); m[0][1] = 0.0f;            m[0][2] = 0.0f;          m[0][3] = 0.0;
    m[1][0] = 0.0f;                   m[1][1] = 1.0f / tanHalfFOV; m[1][2] = 0.0f;          m[1][3] = 0.0;
    m[2][0] = 0.0f;                   m[2][1] = 0.0f;            m[2][2] = (-m_persProj.zNear - m_persProj.zFar) / zRange; m[2][3] = 2.0f * m_persProj.zFar * m_persProj.zNear / zRange;
    m[3][0] = 0.0f;                   m[3][1] = 0.0f;            m[3][2] = 1.0f;          m[3][3] = 0.0;
}

const glm::mat4* Pipeline::GetTrans()
{
    glm::mat4 ScaleTrans, RotateTrans, TranslationTrans, CameraTranslationTrans, CameraRotateTrans,  PersProjTrans;

    InitScaleTransform(ScaleTrans);
    InitRotateTransform(RotateTrans);
    InitTranslationTransform(TranslationTrans);
    InitTranslationTransformC(CameraTranslationTrans);
    InitCameraTransform(CameraRotateTrans);
    //InitPerspectiveProj(PersProjTrans);
    InitPersProjTransform(PersProjTrans);

   

    m_transformation = PersProjTrans * CameraRotateTrans * CameraTranslationTrans * TranslationTrans * RotateTrans * ScaleTrans;
    return &m_transformation;
}

