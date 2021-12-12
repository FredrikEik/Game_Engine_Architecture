#ifndef WIDGETMATERIAL_H
#define WIDGETMATERIAL_H
/**
  In the game-engine-editor, there are re-positionable sub-windows, Widgetmaterial controls materials on gameobjects.
 */
namespace Ui {
class WidgetMaterial;
}

class WidgetMaterial : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetMaterial(QWidget *parent = nullptr);
    ~WidgetMaterial();

    void readMaterialData();

    void fillShaders();
    void fillTextures();

    int indexInSceneArray{-1};
    class Scene* mCurrentScene{nullptr};

    class Material *mCurrentMaterial{nullptr};

    bool fillTexturesDone{false};

private slots:
    void on_specularStrength_valueChanged(double arg1);

    void on_specularExponent_valueChanged(int arg1);

    void on_colorButton_clicked();

    void on_textureComboBox_currentIndexChanged(int index);

private:
    Ui::WidgetMaterial *ui;
};

#endif // WIDGETMATERIAL_H
