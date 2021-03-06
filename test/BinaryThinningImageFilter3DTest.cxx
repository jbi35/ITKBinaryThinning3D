/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImageRegionIterator.h"
#include "itkBinaryThinningImageFilter3D.h"

#include <iostream>
#include <stdlib.h> // for atoi()
using namespace std;

int BinaryThinningImageFilter3DTest(int argc, char *argv[])
{
  // Verify the number of parameters in the command line
  if (argc <= 2)
  {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " inputImageFile outputImageFile" << std::endl;
    return EXIT_FAILURE;
  }
  char *infilename = argv[1];
  char *outfilename = argv[2];

  const unsigned int Dimension = 3;
  typedef signed short PixelType; // must be signed for CT since Hounsfield units can be < 0
  typedef itk::Image<PixelType, Dimension> ImageType;

  // Read image
  typedef itk::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infilename);
  try
  {
    reader->Update();
  }
  catch (itk::ExceptionObject &ex)
  {
    std::cout << ex << std::endl;
    return EXIT_FAILURE;
  }
  cout << infilename << " sucessfully read." << endl;

  // Define the thinning filter
  typedef itk::BinaryThinningImageFilter3D<ImageType, ImageType> ThinningFilterType;
  ThinningFilterType::Pointer thinningFilter = ThinningFilterType::New();
  thinningFilter->SetInput(reader->GetOutput());
  thinningFilter->Update();

  // output to file
  typedef itk::ImageFileWriter<ImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(thinningFilter->GetOutput());
  writer->SetFileName(outfilename);

  try
  {
    writer->Update();
  }
  catch (itk::ExceptionObject &ex)
  {
    std::cout << ex << std::endl;
    return EXIT_FAILURE;
  }
  cout << outfilename << " sucessfully written." << endl;

  cout << "Program terminated normally." << endl;
  return EXIT_SUCCESS;
}
