#include <iostream>
#include "VolumeRegistration.h"
#include "MultiResRegistration.h"
#include "SphereImage.h"

int main() {


	//auto fixed = DcmReader(std::filesystem::path("E:") / "VolumeData" / "716" / "716^716_716_CT_2013-04-02_230000_716-1-01_716-1_n81__00000");
	//auto moving = DcmReader(std::filesystem::path("E:") / "VolumeData" / "716" / "716^716_716_CT_2013-04-02_230000_716-2-02_716-2_n81__00000");

	auto fixed = SphereImage(40, 40, 40, INT16_MIN / 2, INT16_MAX / 2);
	auto moving = SphereImage(40, 40, 40, INT16_MIN / 2, INT16_MAX / 2);

	itk::TranslationTransform<double, 3>::Pointer translation = itk::TranslationTransform<double, 3>::New();
	itk::TranslationTransform<double, 3>::ParametersType shift(translation->GetNumberOfParameters());
	shift[0] = 3.2;
	shift[1] = 2.3;
	shift[2] = 1.7;

	translation->SetParameters(shift);
	moving.SetTransform(translation);

	auto registration = MultiResRegistration();
	registration.SetFixed(fixed.GetOutput());
	registration.SetMoving(moving.GetOutput());
	
	auto monitoring = RegistrationMonitoring::New();
	registration.GetOptimizerCC()->AddObserver(itk::IterationEvent(), monitoring);
	registration.GetOptimizerMI()->AddObserver(itk::IterationEvent(), monitoring);
	
	auto transform = registration.Register();
	



	auto diff = Difference();
	diff.SetFixed(fixed.GetOutput());
	diff.SetMoving(moving.GetOutput());
	diff.SetTransform(transform);
	diff.Update();


	auto w = Export();

	w.WriteTo("registeredSpheres", "img", diff.GetOutput());

	return 0;
}