#ifndef WIDGETMATERIAL_H
#define WIDGETMATERIAL_H

namespace Ui {
class WidgetMaterial;
}

class WidgetMaterial : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetMaterial(QWidget *parent = nullptr);
    ~WidgetMaterial();

    void readMaterialData(int currentObject, class RenderSystem *renderSystem);

    void fillShaders();
    void fillTextures();

//    int indexInSceneArray{-1};
//    class Scene* mCurrentScene{nullptr};

    struct MaterialComponent *mCurrentMaterial{nullptr};
    bool fillTexturesDone{false};

private slots:
    void on_specularStrength_valueChanged(double arg1);

    void on_specularExponent_valueChanged(int arg1);

    void on_colorButton_clicked();

    void on_textureComboBox_currentIndexChanged(int index);

private:
    Ui::WidgetMaterial *ui{nullptr};
};

#endif // WIDGETMATERIAL_H
