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

#include "itkCurvilinearArraySpecialCoordinatesImage.h"

#include "itkUltrasoundImageFileReader.h"

#include "itkTestingMacros.h"

int itkCurvilinearArrayUltrasoundImageFileReaderTest( int argc, char * argv [] )
{
  if( argc < 2 )
    {
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputImage";
    std::cerr << std::endl;
    return EXIT_FAILURE;
    }
  const char * inputImageFileName = argv[1];

  const unsigned int Dimension = 3;
  typedef unsigned char PixelType;

  typedef itk::CurvilinearArraySpecialCoordinatesImage< PixelType, Dimension > SpecialCoordinatesImageType;

  typedef itk::UltrasoundImageFileReader< SpecialCoordinatesImageType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( inputImageFileName );
  try
    {
    reader->Update();
    }
  catch( itk::ExceptionObject & error )
    {
    std::cerr << "Error: " << error << std::endl;
    return EXIT_FAILURE;
    }

  SpecialCoordinatesImageType::ConstPointer image = reader->GetOutput();
  TEST_EXPECT_EQUAL( image->GetLateralAngularSeparation(), 0.00862832 );
  TEST_EXPECT_EQUAL( image->GetRadiusSampleSize(), 0.0513434294 );
  TEST_EXPECT_EQUAL( image->GetFirstSampleDistance(), 26.4 );

  return EXIT_SUCCESS;
}
