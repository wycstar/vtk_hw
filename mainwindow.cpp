#include "mainwindow.h"

#include <QVTKOpenGLWindow.h>
#include <vtkGenericOpenGLRenderWindow.h>

#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow),
      coneActor(vtkSmartPointer<vtkActor>::New())
{
    ui->setupUi(this);

    vtkSmartPointer<vtkImageReader> reader =
        vtkSmartPointer<vtkImageReader>::New();
    reader->SetFileName(
        "X:/download/dicom/acquisition_no/"
        "1.3.46.670589.33.1.63721423846504795600002.5506639080640827700/"
        "1.3.46.670589.33.1.63721423894857561200001.5525695908347802767.dcm");
    reader->SetDataScalarTypeToFloat();
    reader->SetFileDimensionality(3);
    reader->SetDataExtent(0, 286, 0, 380, 0, 643);
    reader->SetHeaderSize(0);
    reader->Update();

    vtkNew<vtkVolumeProperty> volume_property;
    volume_property->ShadeOn();
    volume_property->SetInterpolationType(VTK_LINEAR_INTERPOLATION);

    vtkDataArray *arr = reader->GetOutput()->GetPointData()->GetScalars();
    double range[2];
    arr->GetRange(range);

    vtkNew<vtkColorTransferFunction> colorTF;
    colorTF->AddRGBPoint(-200, 0.0, 0.0, 0.0);
    colorTF->AddRGBPoint(110, 0.4, 0.4, 1.0);
    colorTF->AddRGBPoint(512, 1.0, 1.0, 1.0);
    colorTF->AddRGBPoint(range[1], 0.9, 0.1, 0.3);

    vtkNew<vtkPiecewiseFunction> scalarTF;
    scalarTF->AddPoint(-200, 0.00);
    scalarTF->AddPoint(110, 0.00);
    scalarTF->AddPoint(512, 0.5);
    scalarTF->AddPoint(range[1], 0.9);

    vtkNew<vtkPiecewiseFunction> gradientTF;
    gradientTF->AddPoint(-200, 0.0);
    gradientTF->AddPoint(range[1] / 4.0, 1.0);

    volume_property->SetScalarOpacity(scalarTF);
    volume_property->SetGradientOpacity(gradientTF);
    volume_property->SetColor(colorTF);

    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow =
        vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();

    // mapping data
    vtkNew<vtkGPUVolumeRayCastMapper> mapper;
    mapper->SetInputConnection(reader->GetOutputPort());
    mapper->SetBlendModeToComposite();
    mapper->SetUseJittering(1);

    // renderer and volume
    vtkNew<vtkRenderer> renderer;
    renderer->SetBackground(0.03, 0.33, 0.33);
    renderWindow->AddRenderer(renderer);

    vtkNew<vtkVolume> volume;
    volume->SetMapper(mapper);
    volume->SetProperty(volume_property);
    renderer->AddVolume(volume);

    renderer->ResetCamera();
    renderer->GetActiveCamera()->Zoom(1.3);

    vtkNew<vtkRenderWindowInteractor> interactor;
    interactor->SetRenderWindow(renderWindow);

    vtkNew<vtkInteractorStyleTrackballCamera> style;
    interactor->SetInteractorStyle(style);

    ui->openGLWidget->setRenderWindow(renderWindow);
}

MainWindow::~MainWindow() { delete ui; }
