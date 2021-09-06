#include "player.h"
#include "plane.h"
Player::Player()
    :InteractiveObject()
{
    // Front
       m_vertices.push_back(Vertex{ -0.5f, -0.5f,  0.5f, 1.f, 0.f, 0.f});
       m_vertices.push_back(Vertex{  0.5f, -0.5f,  0.5f, 0.f, 1.f, 0.f});
       m_vertices.push_back(Vertex{  0.5f,  0.5f,  0.5f, 0.f, 0.f, 1.f});
       m_vertices.push_back(Vertex{  0.5f,  0.5f,  0.5f, 0.f, 0.f, 1.f});
       m_vertices.push_back(Vertex{ -0.5f,  0.5f,  0.5f, 0.f, 1.f, 0.f});
       m_vertices.push_back(Vertex{ -0.5f, -0.5f,  0.5f, 1.f, 0.f, 0.f});

       // Back
       m_vertices.push_back(Vertex{  0.5f, -0.5f, -0.5f, 1.f, 0.f, 0.f});
       m_vertices.push_back(Vertex{ -0.5f, -0.5f, -0.5f, 0.f, 1.f, 0.f});
       m_vertices.push_back(Vertex{ -0.5f,  0.5f, -0.5f, 0.f, 0.f, 1.f});
       m_vertices.push_back(Vertex{ -0.5f,  0.5f, -0.5f, 0.f, 0.f, 1.f});
       m_vertices.push_back(Vertex{  0.5f,  0.5f, -0.5f, 0.f, 1.f, 0.f});
       m_vertices.push_back(Vertex{  0.5f, -0.5f, -0.5f, 1.f, 0.f, 0.f});

       // Right
       m_vertices.push_back(Vertex{  0.5f, -0.5f,  0.5f, 1.f, 0.f, 0.f});
       m_vertices.push_back(Vertex{  0.5f, -0.5f, -0.5f, 0.f, 1.f, 0.f});
       m_vertices.push_back(Vertex{  0.5f,  0.5f, -0.5f, 0.f, 0.f, 1.f});
       m_vertices.push_back(Vertex{  0.5f,  0.5f, -0.5f, 0.f, 0.f, 1.f});
       m_vertices.push_back(Vertex{  0.5f,  0.5f,  0.5f, 0.f, 1.f, 0.f});
       m_vertices.push_back(Vertex{  0.5f, -0.5f,  0.5f, 1.f, 0.f, 0.f});

       // Left
       m_vertices.push_back(Vertex{ -0.5f, -0.5f, -0.5f, 1.f, 0.f, 0.f});
       m_vertices.push_back(Vertex{ -0.5f, -0.5f,  0.5f, 0.f, 1.f, 0.f});
       m_vertices.push_back(Vertex{ -0.5f,  0.5f,  0.5f, 0.f, 0.f, 1.f});
       m_vertices.push_back(Vertex{ -0.5f,  0.5f,  0.5f, 0.f, 0.f, 1.f});
       m_vertices.push_back(Vertex{ -0.5f,  0.5f, -0.5f, 0.f, 1.f, 0.f});
       m_vertices.push_back(Vertex{ -0.5f, -0.5f, -0.5f, 1.f, 0.f, 0.f});

       // Bot
       m_vertices.push_back(Vertex{ -0.5f, -0.5f,  0.5f, 1.f, 0.f, 0.f});
       m_vertices.push_back(Vertex{  0.5f, -0.5f,  0.5f, 0.f, 1.f, 0.f});
       m_vertices.push_back(Vertex{  0.5f, -0.5f, -0.5f, 0.f, 0.f, 1.f});
       m_vertices.push_back(Vertex{  0.5f, -0.5f, -0.5f, 0.f, 0.f, 1.f});
       m_vertices.push_back(Vertex{ -0.5f, -0.5f, -0.5f, 0.f, 1.f, 0.f});
       m_vertices.push_back(Vertex{ -0.5f, -0.5f,  0.5f, 1.f, 0.f, 0.f});

       // Top
       m_vertices.push_back(Vertex{ -0.5f,  0.5f,  0.5f, 1.f, 0.f, 0.f});
       m_vertices.push_back(Vertex{  0.5f,  0.5f,  0.5f, 0.f, 1.f, 0.f});
       m_vertices.push_back(Vertex{  0.5f,  0.5f, -0.5f, 0.f, 0.f, 1.f});
       m_vertices.push_back(Vertex{  0.5f,  0.5f, -0.5f, 0.f, 0.f, 1.f});
       m_vertices.push_back(Vertex{ -0.5f,  0.5f, -0.5f, 0.f, 1.f, 0.f});
       m_vertices.push_back(Vertex{ -0.5f,  0.5f,  0.5f, 1.f, 0.f, 0.f});

       m_modelMatrix.setToIdentity();

}

void Player::move(QVector3D inVector)
{
    m_position.translate(inVector);
    updateHeight();
    updateModelMatrix();
}

void Player::updateHeight()
{
    float height = plane->getHeight(getPosition());
    m_position.translate( (QVector3D(0, -getPosition().y(),0)));
    m_position.translate(QVector3D(0, height,0));
//    setPosition(QVector3D(1.f, height, 1.f));
}

Plane *Player::getPlane() const
{
    return plane;
}

void Player::setPlane(Plane *value)
{
    plane = value;
}

