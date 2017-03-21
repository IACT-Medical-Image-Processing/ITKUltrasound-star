#ifndef __itkBlockMatchingImageRegistrationMethod_h
#define __itkBlockMatchingImageRegistrationMethod_h

#include "itkImageToImageFilter.h"

#include "itkBlockMatchingMetricImageFilter.h"
#include "itkBlockMatchingMetricImageToDisplacementCalculator.h"

namespace itk
{
namespace BlockMatching
{

/** \class ImageRegistrationMethod
 *
 * This class defines the interface to perform deformable image registration by
 * block matching.
 *
 * Displacements are calculated at every block from the FixedImage to the Moving
 * Image.
 *
 * Blocks are neighborhoods with a fixed radius, and they are located on a grid
 * in the fixed image.
 *
 * An Image of search Regions in the moving image specifies each block's search
 * area.  The information from the search region image (origin, spacing, region,
 * etc) determines the information in the output displacement image.
 *
 * \sa ImageRegistrationMethod
 * \ingroup RegistrationFilters
 * \ingroup Ultrasound
 */
template<class TFixedImage, class TMovingImage,
  class TMetricImage, class TDisplacementImage, class TCoordRep >
class ITK_EXPORT ImageRegistrationMethod :
  public ImageToImageFilter< 
  itk::Image< typename TMovingImage::RegionType,
    TDisplacementImage::ImageDimension >,
  TDisplacementImage > 
{
public:
  /** ImageDimension enumeration. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TDisplacementImage::ImageDimension);

  /** Type of the fixed image. */
  typedef TFixedImage                         FixedImageType;
  typedef typename FixedImageType::RegionType FixedRegionType;

  /** Type of the radius used to characterized the fixed image block. */
  typedef typename FixedImageType::SizeType RadiusType;

  /** Type of the moving image. */
  typedef TMovingImage                         MovingImageType;
  typedef typename MovingImageType::RegionType MovingRegionType;

  /** Type of the metric image. */
  typedef TMetricImage  MetricImageType;

  /** Type of the displacement image. */
  typedef TDisplacementImage ImageType;

  typedef typename ImageType::RegionType RegionType;
  typedef typename RegionType::IndexType IndexType;
  typedef typename RegionType::SizeType  SizeType;

  typedef typename ImageType::SpacingType   SpacingType;
  typedef typename ImageType::DirectionType DirectionType;
  typedef typename ImageType::PointType     OriginType;

  /** Type of the search region image. */
  typedef typename itk::Image< typename MovingImageType::RegionType, 
    ImageDimension > SearchRegionImageType;

  /** Standard class typedefs. */
  typedef ImageRegistrationMethod     Self;
  typedef ImageToImageFilter< SearchRegionImageType, TDisplacementImage >
    Superclass;

  typedef SmartPointer< Self >           Pointer;
  typedef SmartPointer< const Self >     ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(ImageRegistrationMethod, ImageSource);

  /** Type of the point use for determing the location in the fixed image of a
   * block's center. */
  typedef typename itk::Point< TCoordRep, ImageDimension > CoordRepType;

  /** Type of the MetricImageFilter. */
  typedef MetricImageFilter< FixedImageType, MovingImageType, MetricImageType >
    MetricImageFilterType;

  /** Type of the MetricImageToDisplacementCalculator. */
  typedef MetricImageToDisplacementCalculator< TMetricImage, TDisplacementImage >
    MetricImageToDisplacementCalculatorType;

  /** Set the fixed image. */
  void SetFixedImage( FixedImageType * fixedImage );
  const FixedImageType * GetFixedImage() const 
    { return this->m_FixedImage.GetPointer(); }

  /** Set the moving image. */
  void SetMovingImage( MovingImageType * movingImage );
  const MovingImageType * GetMovingImage() const
    { return this->m_MovingImage.GetPointer(); }

  /** Set the MetricImageFilter. */
  itkSetObjectMacro( MetricImageFilter, MetricImageFilterType );
  itkGetConstObjectMacro( MetricImageFilter, MetricImageFilterType );

  /** Set/Get the MetricImageToDisplacementCalculator.  This defaults to a
   * MaximumPixelDisplacementCalculator. */
  itkSetObjectMacro( MetricImageToDisplacementCalculator,
    MetricImageToDisplacementCalculatorType );
  itkGetConstObjectMacro( MetricImageToDisplacementCalculator,
    MetricImageToDisplacementCalculatorType );

  /** Whether or not to use streaming.  Streaming is achieved by streaming each
   * block matching.  This allows for deformable registration of very large
   * images, but it comes at a performance penalty.  By default it is OFF. */
  itkSetMacro( UseStreaming, bool );
  itkGetConstMacro( UseStreaming, bool );
  itkBooleanMacro( UseStreaming );

  /** Set the radius for blocks in the fixed image to be matched against the
   * moving image.  This is a radius defined similarly to an itk::Neighborhood
   * radius, i.e., the size of the block in the i'th direction is 2*radius[i] +
   * 1.  Every fixed image block to be registered uses the same radius.
   */
  virtual void SetRadius( const RadiusType& radius )
    {
    m_Radius = radius;
    this->Modified();
    }
  /** Set the radius to the given value in all directions. */
  virtual void SetRadius( const unsigned int rad )
    {
    RadiusType radius;
    radius.Fill( rad );
    this->SetRadius( radius );
    }
  itkGetConstReferenceMacro( Radius, RadiusType );

  /** Set/Get the search region image.  The SearchRegionImage has the same
   * LargestPossibleRegion as the output displacement image.  It contains
   * ImageRegions in the moving image that define the search region for each
   * block in the fixed image.
   *
   * The metric image is created by calculating the
   * value of the metric between the fixed image block and the corresponding
   * area/volume in the moving image.  The center of the fixed image block is
   * translated between the corners of the given search region, evaluating the
   * metric at the spacing of the metric image.  Therefore, contributing region
   * in the moving image to the search is actually the given search region
   * dilated by the radius of the kernel block. */
  virtual void SetSearchRegionImage( SearchRegionImageType * searchRegionImage )
    {
    this->SetInput( searchRegionImage );
    }


protected:
  ImageRegistrationMethod();
  virtual ~ImageRegistrationMethod() {}

  virtual void GenerateOutputInformation();

  virtual void GenerateInputRequestedRegion();
  
  virtual void EnlargeOutputRequestedRegion( DataObject * data );

  /** Initialize by setting the interconnects between the components. */
  virtual void Initialize() throw (ExceptionObject);

  virtual void GenerateData();

  typename FixedImageType::Pointer  m_FixedImage;
  typename MovingImageType::Pointer m_MovingImage;

  typename MetricImageFilterType::Pointer  m_MetricImageFilter;
  typename MetricImageToDisplacementCalculatorType::Pointer
    m_MetricImageToDisplacementCalculator;

  bool          m_UseStreaming;
  RadiusType    m_Radius;

private:
  ImageRegistrationMethod( const Self& ); // purposely not implemented
  void operator=( const Self& ); // purposely not implemented
};

} // end namespace BlockMatching
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkBlockMatchingImageRegistrationMethod.txx"
#endif

#endif
