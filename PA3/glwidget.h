#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QString>
#include <QKeyEvent>
#include <QMatrix4x4>
#include <QMainWindow>
#include <QOpenGLWidget>
#include <QOpenGLWindow>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QOpenGLShader>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

#include "camera3d.h"
#include "transform3d.h"

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    GLWidget(int step, QWidget *parent = 0);
    ~GLWidget();

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

    void loadTexture(QString file);
    void loadCube();
    void loadObj (int indSize, int vertCount, uint *inds, QVector3D *verts, QVector3D *norms, QVector3D *tex);
    void changeToFlat ();
    void changeToNorm ();
    void changeToMean ();
    void connectUpdate();
    void disconnectUpdate();
    void reload_shader ();
    void switchMappig (QString shad);

    QVector3D difusiveColor;
    QVector3D ambientColor;
    QVector3D specularColor;
    float bumpness;
    float displacement;
    float shininess;
    float albedo;
    float roughness;
    int shading = 0;
    int reflectmodel = 0;
    int textureMapping = 0;
    bool changed = false;

    QOpenGLShaderProgram *m_program;
    QOpenGLShader *shader;

public slots:
    void cleanup();

protected:
    void paintGL() override;
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;


protected slots:
  void update();

private:
    void printContextInformation();
    void LoadCube();

    QOpenGLBuffer m_vertex;
    QOpenGLBuffer m_normal;
    QOpenGLBuffer m_tex, m_tan;

    QOpenGLVertexArrayObject m_object;

    // Shader Information
    int u_modelToWorld;
    int u_cameraToView;
    int u_worldToCamera;
    int u_lightPosition;
    int u_difuseColor;
    int u_ambientColor;
    int u_specularColor;
    int u_shadding;
    int u_albedo;
    int u_roughness;
    int u_bumpness;
    int u_displacement;

    Camera3D m_camera;
    QMatrix4x4 m_projection;
    Transform3D m_transform;

    QVector3D vLightPosition;

    int actual;
    int factor;
    int step_;
    int indiceSize_, normalIndiceSize_;
    int vertexCount_, normalCount_;
    int m_normalAttr;
    int m_texAttr, m_tanAttr;
    int u_shininess;
    QVector3D color_;
    QVector3D *vertices, *tangents, *sg_vertexes_, *sg_normals_, *sg_tangent_;
    QVector2D *Texture, *sg_texture_;
    QVector3D *normals, *flatNormals, *meanNormals;

    QMetaObject::Connection m_connection;

};

#endif
