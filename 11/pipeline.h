#pragma once
#ifndef PIPELINE_H
#define	PIPELINE_H

#include "math_3d.h"

class Pipeline
{
public:
    Pipeline()
    {
        m_scale = glm::vec3(1.0f, 1.0f, 1.0f);
        m_worldPos = glm::vec3(0.0f, 0.0f, 0.0f);
        m_rotateInfo = glm::vec3(0.0f, 0.0f, 0.0f);
        //m_persProj();
    }

    void Scale(float ScaleX, float ScaleY, float ScaleZ)
    {
        m_scale.x = ScaleX;
        m_scale.y = ScaleY;
        m_scale.z = ScaleZ;
    }

    void WorldPos(float x, float y, float z)
    {
        m_worldPos.x = x;
        m_worldPos.y = y;
        m_worldPos.z = z;
    }

    void Rotate(float RotateX, float RotateY, float RotateZ)
    {
        m_rotateInfo.x = RotateX;
        m_rotateInfo.y = RotateY;
        m_rotateInfo.z = RotateZ;
    }

    void SetPerspectiveProj(float FOV, float Width, float Height, float zNear, float zFar)
    {
        m_persProj.FOV = FOV;
        m_persProj.Width = Width;
        m_persProj.Height = Height;
        m_persProj.zNear = zNear;
        m_persProj.zFar = zFar;
    }

    void SetCamera(const glm::vec3& Pos, const glm::vec3& Target, const glm::vec3& Up)
    {
        m_camera.Pos = Pos;
        m_camera.Target = Target;
        m_camera.Up = Up;
    }
    void InitPersProjTransform(glm::mat4& m);

    const glm::mat4* GetTrans();

private:
    void InitScaleTransform(glm::mat4& m) const;
    void InitRotateTransform(glm::mat4& m) const;
    void InitTranslationTransform(glm::mat4& m) const;
    void InitTranslationTransformC(glm::mat4& m) const;
    void InitPerspectiveProj(glm::mat4& m) const;

    void InitCameraTransform(glm::mat4& m) const;

    glm::vec3 m_scale;
    glm::vec3 m_worldPos;
    glm::vec3 m_rotateInfo;
    glm::mat4 m_transformation;

    struct {
        float FOV;
        float Width;
        float Height;
        float zNear;
        float zFar;
    } m_persProj;

    struct {
        glm::vec3 Pos;
        glm::vec3 Target;
        glm::vec3 Up;
    } m_camera;
};



#endif	/* PIPELINE_H */