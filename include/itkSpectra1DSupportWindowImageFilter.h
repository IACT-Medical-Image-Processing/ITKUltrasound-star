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
#ifndef __itkSpectra1DSupportWindowImageFilter_h
#define __itkSpectra1DSupportWindowImageFilter_h

#include <list>

#include "itkImageToImageFilter.h"

namespace itk
{

/** Use to identify to location where a 1D FFT is computed. */
template< typename TIndex >
struct FFT1DRegion
{
  typedef TIndex IndexType;

  IndexType     Index;
  SizeValueType Size;
};

/** \class Spectra1DSupportWindowImageFilter
 * \brief Generate an image of local spectra computation support windows.
 *
 * The information from the input image is used to determine the output image
 * information. The pixel value of the input image is used to specify the
 * nominal number of lines on either side of the central FFT line to add to
 * the window. The nominal size of the 1D FFT is specified with SetFFTSize()
 *
 * \ingroup Ultrasound
 */
template< class TInputImage >
class Spectra1DSupportWindowImageFilter:
  public ImageToImageFilter< TInputImage,
                             Image< std::list< FFT1DRegion< typename TInputImage::IndexType > >, TInputImage::ImageDimension > >
{
public:
  itkStaticConstMacro( ImageDimension, unsigned int, TInputImage::ImageDimension );

  typedef TInputImage                              InputImageType;
  typedef typename InputImageType::IndexType       IndexType;
  typedef FFT1DRegion< IndexType >                 FFT1DRegionType;

  typedef std::list< FFT1DRegionType >             OutputPixelType;
  typedef Image< OutputPixelType, ImageDimension > OutputImageType;

  /** Standard class typedefs. */
  typedef Spectra1DSupportWindowImageFilter                     Self;
  typedef ImageToImageFilter< InputImageType, OutputImageType > Superclass;
  typedef SmartPointer< Self >                                  Pointer;
  typedef SmartPointer< const Self >                            ConstPointer;

  itkTypeMacro( Spectra1DSupportWindowImageFilter, ImageToImageFilter );
  itkNewMacro( Self );

  /** Set/Get the nominal size of the FFT.  This will be truncated at the
   * boundary of image. */
  itkGetConstMacro( FFTSize, SizeValueType );
  itkSetMacro( FFTSize, SizeValueType );

protected:
  Spectra1DSupportWindowImageFilter();
  virtual ~Spectra1DSupportWindowImageFilter() {};

  typedef typename OutputImageType::RegionType OutputImageRegionType;

  virtual void ThreadedGenerateData( const OutputImageRegionType & outputRegionForThread, ThreadIdType threadId );

private:
  Spectra1DSupportWindowImageFilter( const Self & ); // purposely not implemented
  void operator=( const Self & ); // purposely not implemented

  SizeValueType m_FFTSize;
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkSpectra1DSupportWindowImageFilter.hxx"
#endif

#endif // __itkSpectra1DSupportWindowImageFilter_h
