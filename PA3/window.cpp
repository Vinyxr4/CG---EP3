#include <QSlider>
#include <QKeyEvent>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QApplication>
#include <QDesktopWidget>

#include "window.h"
#include "glwidget.h"
#include "mainwindow.h"

Window::Window(MainWindow *mw, int step, int shininess)
    : mainWindow(mw), step_(step), shininess_(shininess) {
  glWidget = new GLWidget(step);

  QTabWidget *colorSlidersTab = createColorSliders();
  QGroupBox *shadingBoxes = createShadingRadialBoxes();
  QGroupBox *mappingBoxes = createMappingRadialBoxes();
  QGroupBox *controllersGroup = createParameterSliders();
  QGroupBox *reflectanceGroup = createReflectanceRadialBoxes();
  QTabWidget *BumpController = createBumpSlider();
  QTabWidget *DisplacementController = createDisplacementSlider();

  QVBoxLayout *mainLayout = new QVBoxLayout;
  QGridLayout *container = new QGridLayout;

  container->addWidget(glWidget, 0, 0, 6, 1);
  container->addWidget(mappingBoxes, 0, 3);
  container->addWidget(shadingBoxes, 0, 2);
  container->addWidget(reflectanceGroup, 0, 1);
  container->addWidget(controllersGroup, 1, 1, 1, 3);
  container->addWidget(colorSlidersTab, 2, 1, 2, 3);
  container->addWidget(BumpController, 4, 1, 1, 3);
  container->addWidget(DisplacementController, 5, 1, 1, 3);
  container->setColumnStretch(0, 10);

  QWidget *centralWidget = new QWidget;
  centralWidget->setLayout(container);
  mainLayout->addWidget(centralWidget);

  setLayout(mainLayout);
}

QTabWidget* Window::createBumpSlider() {
  QTabWidget *bumpTab = new QTabWidget;
  QWidget *bumpness = createBumpness();

  bumpTab->addTab(bumpness, "Bumpness");

  return bumpTab;
}

QTabWidget* Window::createDisplacementSlider() {
  QTabWidget *displacementTab = new QTabWidget;
  QWidget *displacement = createDisplacement();

  displacementTab->addTab(displacement, "Displacement");

  return displacementTab;
}

QTabWidget* Window::createColorSliders() {
  QTabWidget *colorTab = new QTabWidget;
  QWidget *ambientColors = createAmbientColors();
  QWidget *diffusiveColors = createDiffusiveColors();
  QWidget *specularColors = createSpecularColors();

  colorTab->addTab(ambientColors, "Ambient");
  colorTab->addTab(diffusiveColors, "Diffusive");
  colorTab->addTab(specularColors, "Specular");
  return colorTab;
}

QWidget* Window::createBumpness() {
  QWidget *bumpWidget = new QWidget;
  QGridLayout *bumpSliderLayout = new QGridLayout;

  int currentRow = 0;

  bumpSlider = createSlider();
  bumpLabel = new QLabel(tr("Bumpness:"));
  bumpSliderLayout->addWidget(bumpLabel, currentRow, 0);
  bumpSliderLayout->addWidget(bumpSlider, currentRow, 1);
  bumpSlider->setValue(step_/2);
  glWidget->bumpness = 0.5;
  connect(bumpSlider, SIGNAL(valueChanged(int)), this, SLOT(valueBMP(int)));
  currentRow++;

  bumpWidget->setLayout(bumpSliderLayout);
  return bumpWidget;
}

QWidget* Window::createDisplacement() {
  QWidget *displacementWidget = new QWidget;
  QGridLayout *displacementSliderLayout = new QGridLayout;

  int currentRow = 0;

  displacementSlider = createSlider();
  displacementLabel = new QLabel(tr("Displacement:"));
  displacementSliderLayout->addWidget(displacementLabel, currentRow, 0);
  displacementSliderLayout->addWidget(displacementSlider, currentRow, 1);
  displacementSlider->setValue(step_/2);
  glWidget->displacement = 0.5;
  connect(displacementSlider, SIGNAL(valueChanged(int)), this, SLOT(valueDSP(int)));
  currentRow++;

  displacementWidget->setLayout(displacementSliderLayout);
  return displacementWidget;
}

QWidget* Window::createAmbientColors() {
  QWidget *colorWidget = new QWidget;
  QGridLayout *colorSliderLayout = new QGridLayout;

  int currentRow = 0;

  redAmbientSlider = createSlider();
  redAmbientLabel = new QLabel(tr("Red:"));
  colorSliderLayout->addWidget(redAmbientLabel, currentRow, 0);
  colorSliderLayout->addWidget(redAmbientSlider, currentRow, 1);
  redAmbientSlider->setValue(step_/2);
  glWidget->ambientColor.setX(0.5);
  connect(redAmbientSlider, SIGNAL(valueChanged(int)), this, SLOT(valueRA(int)));
  currentRow++;

  greenAmbientSlider = createSlider();
  greenAmbientLabel = new QLabel(tr("Green:"));
  colorSliderLayout->addWidget(greenAmbientLabel, currentRow, 0);
  colorSliderLayout->addWidget(greenAmbientSlider, currentRow, 1);
  greenAmbientSlider->setValue(step_/2);
  glWidget->ambientColor.setY(0.5);
  connect(greenAmbientSlider, SIGNAL(valueChanged(int)), this, SLOT(valueGA(int)));
  currentRow++;

  blueAmbientSlider = createSlider();
  blueAmbientLabel = new QLabel(tr("Blue:"));
  colorSliderLayout->addWidget(blueAmbientLabel, currentRow, 0);
  colorSliderLayout->addWidget(blueAmbientSlider, currentRow, 1);
  blueAmbientSlider->setValue(step_/2);
  glWidget->ambientColor.setZ(0.5);
  connect(blueAmbientSlider, SIGNAL(valueChanged(int)), this, SLOT(valueBA(int)));
  currentRow++;

  colorWidget->setLayout(colorSliderLayout);
  return colorWidget;
}

QWidget* Window::createDiffusiveColors() {
  QWidget *colorWidget = new QWidget;
  QGridLayout *colorSliderLayout = new QGridLayout;

  int currentRow = 0;

  redDiffusiveSlider = createSlider();
  redDiffusiveLabel = new QLabel(tr("Red:"));
  colorSliderLayout->addWidget(redDiffusiveLabel, currentRow, 0);
  colorSliderLayout->addWidget(redDiffusiveSlider, currentRow, 1);
  redDiffusiveSlider->setValue(step_/2);
  glWidget->difusiveColor.setX(0.5);
  connect(redDiffusiveSlider, SIGNAL(valueChanged(int)), this, SLOT(valueRD(int)));
  currentRow++;

  greenDiffusiveSlider = createSlider();
  greenDiffusiveLabel = new QLabel(tr("Green:"));
  colorSliderLayout->addWidget(greenDiffusiveLabel, currentRow, 0);
  colorSliderLayout->addWidget(greenDiffusiveSlider, currentRow, 1);
  greenDiffusiveSlider->setValue(step_/2);
  glWidget->difusiveColor.setY(0.5);
  connect(greenDiffusiveSlider, SIGNAL(valueChanged(int)), this, SLOT(valueGD(int)));
  currentRow++;

  blueDiffusiveSlider = createSlider();
  blueDiffusiveLabel = new QLabel(tr("Blue:"));
  colorSliderLayout->addWidget(blueDiffusiveLabel, currentRow, 0);
  colorSliderLayout->addWidget(blueDiffusiveSlider, currentRow, 1);
  blueDiffusiveSlider->setValue(step_/2);
  glWidget->difusiveColor.setZ(0.5);
  connect(blueDiffusiveSlider, SIGNAL(valueChanged(int)), this, SLOT(valueBD(int)));
  currentRow++;

  colorWidget->setLayout(colorSliderLayout);
  return colorWidget;
}

QWidget* Window::createSpecularColors() {
  QWidget *colorWidget = new QWidget;
  QGridLayout *colorSliderLayout = new QGridLayout;

  int currentRow = 0;

  redSpecularSlider = createSlider();
  redSpecularLabel = new QLabel(tr("Red:"));
  colorSliderLayout->addWidget(redSpecularLabel, currentRow, 0);
  colorSliderLayout->addWidget(redSpecularSlider, currentRow, 1);
  redSpecularSlider->setValue(step_/2);
  glWidget->specularColor.setX(0.5);
  connect(redSpecularSlider, SIGNAL(valueChanged(int)), this, SLOT(valueRS(int)));
  currentRow++;

  greenSpecularSlider = createSlider();
  greenSpecularLabel = new QLabel(tr("Green:"));
  colorSliderLayout->addWidget(greenSpecularLabel, currentRow, 0);
  colorSliderLayout->addWidget(greenSpecularSlider, currentRow, 1);
  greenSpecularSlider->setValue(step_/2);
  glWidget->specularColor.setY(0.5);
  connect(greenSpecularSlider, SIGNAL(valueChanged(int)), this, SLOT(valueGS(int)));
  currentRow++;

  blueSpecularSlider = createSlider();
  blueSpecularLabel = new QLabel(tr("Blue:"));
  colorSliderLayout->addWidget(blueSpecularLabel, currentRow, 0);
  colorSliderLayout->addWidget(blueSpecularSlider, currentRow, 1);
  blueSpecularSlider->setValue(step_/2);
  glWidget->specularColor.setZ(0.5);
  connect(blueSpecularSlider, SIGNAL(valueChanged(int)), this, SLOT(valueBS(int)));
  currentRow++;

  colorWidget->setLayout(colorSliderLayout);
  return colorWidget;
}

QGroupBox* Window::createParameterSliders() {
  QGroupBox *controllersGroup = new QGroupBox(tr("Material properties:"));
  QGridLayout *controllersLayout = new QGridLayout;
  int currentRow = 0;

  albedoSlider = createSlider();
  albedoLabel = new QLabel(tr("Albedo:"));
  controllersLayout->addWidget(albedoLabel, currentRow, 0);
  controllersLayout->addWidget(albedoSlider, currentRow, 1);
  albedoSlider->setValue(step_/2);
  glWidget->albedo = albedo_factor * 0.5;
  connect(albedoSlider, SIGNAL(valueChanged(int)), this, SLOT(valueAl(int)));
  currentRow++;

  roughnessSlider = createSlider();
  roughnessLabel = new QLabel(tr("Roughness:"));
  controllersLayout->addWidget(roughnessLabel, currentRow, 0);
  controllersLayout->addWidget(roughnessSlider, currentRow, 1);
  roughnessSlider->setValue(step_/2);
  glWidget->roughness = 0.5;
  connect(roughnessSlider, SIGNAL(valueChanged(int)), this, SLOT(valueRg(int)));
  currentRow++;

  shininessSlider = createShininessSlider();
  shininessLabel = new QLabel(tr("Shininess:"));
  controllersLayout->addWidget(shininessLabel, currentRow, 0);
  controllersLayout->addWidget(shininessSlider, currentRow, 1);
  shininessSlider->setValue(shininess_/2);
  glWidget->shininess = 32;
  connect(shininessSlider, SIGNAL(valueChanged(int)), this, SLOT(valueSh(int)));
  currentRow++;

  controllersGroup->setLayout(controllersLayout);
  return controllersGroup;
}

QGroupBox* Window::createReflectanceRadialBoxes() {
  QGroupBox *reflectanceGroup = new QGroupBox(tr("Reflectance model:"));
  QVBoxLayout *reflectanceLayout = new QVBoxLayout();

  blinn = new QRadioButton("Blinn-Phong", this);
  nayar = new QRadioButton("Oren-Nayar", this);

  reflectanceLayout->addWidget(blinn);
  reflectanceLayout->addWidget(nayar);

  connect(blinn, SIGNAL(clicked(bool)), this, SLOT(valueRmBF(bool)));
  connect(nayar, SIGNAL(clicked(bool)), this, SLOT(valueRmON(bool)));

  blinn->click();

  reflectanceGroup->setLayout(reflectanceLayout);
  return reflectanceGroup;
}

QGroupBox* Window::createShadingRadialBoxes() {
  QGroupBox *shadingGroup = new QGroupBox(tr("Shading model:"));
  QVBoxLayout *shadingLayout = new QVBoxLayout();

  flat = new QRadioButton("Flat", this);
  phong = new QRadioButton("Phong", this);
  gouraud = new QRadioButton("Gouraud", this);

  shadingLayout->addWidget(flat);
  shadingLayout->addWidget(gouraud);
  shadingLayout->addWidget(phong);

  connect(flat, SIGNAL(clicked(bool)), this, SLOT(valueShBF(bool)));
  connect(phong, SIGNAL(clicked(bool)), this, SLOT(valueShBP(bool)));
  connect(gouraud, SIGNAL(clicked(bool)), this, SLOT(valueShBG(bool)));

  flat->click();

  shadingGroup->setLayout(shadingLayout);
  return shadingGroup;
}

QGroupBox* Window::createMappingRadialBoxes() {
  QGroupBox *mappingGroup = new QGroupBox(tr("Texture mapping:"));
  QVBoxLayout *mappingLayout = new QVBoxLayout();

  standard = new QRadioButton("Standard", this);
  bump = new QRadioButton("Bump", this);
  displacement = new QRadioButton("Displacement", this);

  mappingLayout->addWidget(standard);
  mappingLayout->addWidget(bump);
  mappingLayout->addWidget(displacement);

  connect(standard, SIGNAL(clicked(bool)), this, SLOT(valueTxSD(bool)));
  connect(bump, SIGNAL(clicked(bool)), this, SLOT(valueTxBP(bool)));
  connect(displacement, SIGNAL(clicked(bool)), this, SLOT(valueTxDP(bool)));

  standard->click();

  mappingGroup->setLayout(mappingLayout);
  return mappingGroup;
}

void Window::disconnectWidgetUpdate() {
  glWidget->disconnectUpdate();
}

void Window::connectWidgetUpdate() {
  glWidget->connectUpdate();
}

void Window::load(int indSize, int vertCount, uint *inds, QVector3D *verts, QVector3D *norms, QVector3D *tex) {
    glWidget->loadObj(indSize, vertCount, inds, verts, norms, tex);
}

void Window::loadTexture(QString file) {
    glWidget->loadTexture(file);
}

QSlider *Window::createSlider() {
  QSlider *slider = new QSlider(Qt::Horizontal);
  slider->setRange(0, step_);
  slider->setSingleStep(10);
  slider->setTickPosition(QSlider::TicksRight);
  return slider;
}

QSlider *Window::createShininessSlider() {
  QSlider *slider = new QSlider(Qt::Horizontal);
  slider->setRange(0, shininess_);
  slider->setSingleStep(8);
  slider->setTickPosition(QSlider::TicksRight);
  return slider;
}

void Window::valueBMP(int nv) {
    glWidget->bumpness = ((float) nv)/step_;
}

void Window::valueDSP(int nv) {
    glWidget->displacement = ((float) nv)/step_;
}

// Ambient light slider controller
void Window::valueRA(int nv) {
    glWidget->ambientColor.setX(float(nv)/step_);
}
void Window::valueGA(int nv) {
    glWidget->ambientColor.setY(float(nv)/step_);
}
void Window::valueBA(int nv) {
    glWidget->ambientColor.setZ(float(nv)/step_);
}

// Difusive light slider controller
void Window::valueRD(int nv) {
    glWidget->difusiveColor.setX(float(nv)/step_);
}
void Window::valueGD(int nv) {
    glWidget->difusiveColor.setY(float(nv)/step_);
}
void Window::valueBD(int nv) {
    glWidget->difusiveColor.setZ(float(nv)/step_);
}

// Specular light slider controller
void Window::valueRS(int nv) {
    glWidget->specularColor.setX(float(nv)/step_);
}
void Window::valueGS(int nv) {
    glWidget->specularColor.setY(float(nv)/step_);
}
void Window::valueBS(int nv) {
    glWidget->specularColor.setZ(float(nv)/step_);
}

// Shininess - Albedo - Roughness slider controller
void Window::valueSh(int nv) {
    glWidget->shininess = float(nv);
}
void Window::valueAl(int nv) {
    glWidget->albedo = albedo_factor * float(nv)/step_;
}
void Window::valueRg(int nv) {
    glWidget->roughness = float(nv)/step_;
}

// Shading Model button controller
void Window::valueShBF(bool nv) {
    if (nv == true) {
            glWidget->shading = 0;
            glWidget->changeToFlat();
            glWidget->changed = true;
    }
}
void Window::valueShBP(bool nv) {
    if (nv == true) {
            glWidget->shading = 2;
            glWidget->changeToNorm();
            glWidget->changed = true;
    }
}
void Window::valueShBG(bool nv) {
    if (nv == true) {
            glWidget->shading = 1;
            glWidget->changeToMean();
            glWidget->changed = true;
    }
}

// Reflectance Model button controller
void Window::valueRmBF(bool nv) {
    if (nv == true) {
        glWidget->reflectmodel = 0;
        glWidget->changed = true;
    }
}

void Window::valueRmON(bool nv) {
    if (nv == true) {
        glWidget->reflectmodel = 1;
        glWidget->changed = true;
    }
}

void Window::valueTxSD(bool nv) {
    if (nv == true) {
        glWidget->textureMapping = 0;
        glWidget->changed = true;
    }
}

void Window::valueTxBP(bool nv) {
    if (nv == true) {
        glWidget->textureMapping = 1;
        glWidget->changed = true;
    }
}

void Window::valueTxDP(bool nv) {
    if (nv == true) {
        glWidget->textureMapping = 2;
        glWidget->changed = true;
    }
}
