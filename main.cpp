#include "grafo.h"
#include "vtkPolyDataMapper.h"
#include "vtkSphereSource.h"
#include "vtkActor.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRendererCollection.h"
#include "vtkProperty.h"
#include "vtkCamera.h"
#include "vtkSmartPointer.h"
#include "vtkInteractorStyleTrackball.h"
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkInteractorObserver.h>
#include <vtkPropPicker.h>
#include <vtkCoordinate.h>
#include <vtkObjectFactory.h>
#include <vtkTDxInteractorStyleCamera.h>
#include <vtkTDxInteractorStyleSettings.h>

Grafo hola;

class MyMouseEvent : public vtkInteractorStyleTrackballCamera
{
public:
	static MyMouseEvent* New();
	vtkTypeMacro(MyMouseEvent, vtkInteractorStyleTrackballCamera);
	virtual void OnLeftButtonDown()
	{
		///imprimo las coordenadas de la posicion
		std::cout	<<"hola"
					<<this->Interactor->GetEventPosition()[0]<<" "
					<<this->Interactor->GetEventPosition()[1]<<" "
					<<this->Interactor->GetEventPosition()[2]
					<<std::endl;
		///guardo las coordenadas y las vuelvo coordenadas del mundo
		int x = Interactor->GetEventPosition()[0];
		int y = Interactor->GetEventPosition()[1];

		vtkSmartPointer<vtkCoordinate> coordinate = vtkSmartPointer<vtkCoordinate>::New();

		coordinate->SetCoordinateSystemToDisplay();
  		coordinate->SetValue(x,y,0);
  		double* world = coordinate->GetComputedWorldValue(this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
/*
		//creo una ball con cada click en las coordenas que se hizo click
		vtkSmartPointer<vtkSphereSource> sphere = vtkSmartPointer<vtkSphereSource>::New();
		sphere->SetCenter(world[0],world[1],world[2]);
		sphere->SetRadius(3.0);

		vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		mapper->SetInputConnection(sphere->GetOutputPort());

		vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
		actor->SetMapper(mapper);
		this->GetDefaultRenderer()->AddViewProp(actor);
	*/
		vtkSmartPointer<vtkSphereSource> sphere3 = vtkSmartPointer<vtkSphereSource>::New();
		sphere3->SetCenter(world[0],world[1],world[2]);
		sphere3->SetRadius(3.0);
		sphere3->Update();

		vtkSmartPointer<vtkPolyDataMapper> mapper3 = vtkSmartPointer<vtkPolyDataMapper>::New();
		mapper3->SetInputConnection(sphere3->GetOutputPort());

		vtkSmartPointer<vtkActor> actor3 = vtkSmartPointer<vtkActor>::New();
		actor3->SetMapper(mapper3);

		this->GetDefaultRenderer()->AddActor(actor3);
		//this->GetDefaultRenderWindow()->render();
		///sigue haciendo su comportamiento normal
		vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
	}
private:

};

vtkStandardNewMacro(MyMouseEvent);

int main(int, char*[])
{
   /* Grafo con maps
   map< string , map< string, double> >  m_Grafo;
   m_Grafo["Arequipa"]["Lima"]=100;
   m_Grafo["Arequipa"]["puno"]=1070;

   cout<<m_Grafo["Arequipa"]["puno"]<<endl;
  */
	
	hola.insert_vertex("aqp");
	hola.insert_vertex("lima");
	hola.insert_Arista("aqp","lima",13.5);
	hola.mostrar();

	//creo la 1ra bola
	vtkSmartPointer<vtkSphereSource> sphere = vtkSmartPointer<vtkSphereSource>::New();
	sphere->SetCenter(0.0,0.0,0.0);
	sphere->SetRadius(3.0);
	sphere->Update();

	//creo el 1er mapper
	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(sphere->GetOutputPort());

	//creo el 1er actor
	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);

	///creo la 2da bola
	vtkSmartPointer<vtkSphereSource> sphere2 = vtkSmartPointer<vtkSphereSource>::New();
	sphere2->SetCenter(10.0,10.0,10.0);
	sphere2->SetRadius(3.0);
	sphere2->Update();

	vtkSmartPointer<vtkPolyDataMapper> mapper2 = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper2->SetInputConnection(sphere2->GetOutputPort());

	vtkSmartPointer<vtkActor> actor2 = vtkSmartPointer<vtkActor>::New();
	actor2->SetMapper(mapper2);


	//creo el renderer
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();

	
	//creo la pantalla
	vtkSmartPointer<vtkRenderWindow> window = vtkSmartPointer<vtkRenderWindow>::New();
	window->AddRenderer(renderer);

	vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	interactor->SetRenderWindow(window);

	//añado mi propio click
	vtkSmartPointer<MyMouseEvent> style1 = vtkSmartPointer<MyMouseEvent>::New();
	style1->SetDefaultRenderer(renderer);
	interactor->SetInteractorStyle(style1);
	//window->StereoCapableWindowOn();
	//renderer->AddActor(actor);
	renderer->AddViewProp(actor);
	renderer->AddViewProp(actor2);
	renderer->SetBackground(1,1,1);
	window->Render();
///

///
	

	//añado mi propio click(wrong)
	//vtkSmartPointer<MyMouseEvent> style1 = vtkSmartPointer<MyMouseEvent>::New();
	//vtkSmartPointer<vtkInteractorStyleTrackballCamera> style2 = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
	
	//inicializacion
	//interactor->SetInteractorStyle(style1);

	//interactor->SetInteractorStyle(style2);

	//interactor->Initialize();
	interactor->Start();

	return EXIT_SUCCESS;
}
