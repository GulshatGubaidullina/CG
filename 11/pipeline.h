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

    const glm::mat4* GetTrans();

private:
    void InitScaleTransform(glm::mat4& m) const;
    void InitRotateTransform(glm::mat4& m) const;
    void InitTranslationTransform(glm::mat4& m) const;

    glm::vec3 m_scale;
    glm::vec3 m_worldPos;
    glm::vec3 m_rotateInfo;
    glm::mat4 m_transformation;
};


#endif	/* PIPELINE_H */