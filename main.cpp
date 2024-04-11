#include <iostream>
#include "VolumeRegistration.h"
#include "Difference.h"
#include "Registration.h"

#include "itkImageRegistrationMethodv4.h"
#include "itkTranslationTransform.h"
#include "itkMeanSquaresImageToImageMetricv4.h"
#include "itkRegularStepGradientDescentOptimizerv4.h"


int main() {
	auto fixed = DcmReader(std::filesystem::path("E:") / "VolumeData" / "716" / "716^716_716_CT_2013-04-02_230000_716-1-01_716-1_n81__00000");
	auto moving = DcmReader(std::filesystem::path("E:") / "VolumeData" / "716" / "716^716_716_CT_2013-04-02_230000_716-2-02_716-2_n81__00000");

	auto registration = Registration();
	registration.SetFixed(fixed.GetOutput());
	registration.SetMoving(moving.GetOutput());
	auto transform = registration.Register();
	
	auto diff = Difference();
	diff.SetFixed(fixed.GetOutput());
	diff.SetMoving(moving.GetOutput());
	diff.SetTransform(transform);
	diff.Update();

	auto w = Export();

	w.WriteTo("differenceRegistered", "img", diff.GetOutput());

	return 0;
}