/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkFlexibleBinaryFunctorImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2012-02-17 11:01:02 $
  Version:   $Revision: 1.37 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkFlexibleBinaryFunctorImageFilter_h
#define __itkFlexibleBinaryFunctorImageFilter_h

#include "itkInPlaceImageFilter.h"
#include "itkImageRegionIteratorWithIndex.h"

namespace itk
{
  
/** \class FlexibleBinaryFunctorImageFilter
 * \brief Implements pixel-wise generic operation of two images.
 *
 * This class is parameterized over the types of the two input images
 * and the type of the output image.  It is also parameterized by the
 * operation to be applied.  A Functor style is used. The  main difference
 * wrt the standard BinaryFunctorImageFilter is that the second input's
 * dimensions/spacing/origin can differ from those of the first. The 
 * functor is then only called if the pixel from input1 is inside 
 * input2's largest region.
 * 
 * \sa UnaryFunctorImageFilter BinaryFunctorImageFilter TernaryFunctorImageFilter
 *
 * \ingroup IntensityImageFilters   Multithreaded
 */
template <class TInputImage1, class TInputImage2, 
          class TOutputImage, class TFunction    >
class ITK_EXPORT FlexibleBinaryFunctorImageFilter :
    public InPlaceImageFilter<TInputImage1,TOutputImage> 
{
public:
  /** Standard class typedefs. */
  typedef FlexibleBinaryFunctorImageFilter                       Self;
  typedef InPlaceImageFilter<TInputImage1,TOutputImage>  Superclass;
  typedef SmartPointer<Self>                             Pointer;
  typedef SmartPointer<const Self>                       ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(FlexibleBinaryFunctorImageFilter, InPlaceImageFilter);

  /** Some convenient typedefs. */
  typedef TFunction                              FunctorType;
  typedef TInputImage1                           Input1ImageType;
  typedef typename Input1ImageType::ConstPointer Input1ImagePointer;
  typedef typename Input1ImageType::RegionType   Input1ImageRegionType; 
  typedef typename Input1ImageType::PixelType    Input1ImagePixelType; 

  typedef TInputImage2                           Input2ImageType;
  typedef typename Input2ImageType::ConstPointer Input2ImagePointer;
  typedef typename Input2ImageType::RegionType   Input2ImageRegionType; 
  typedef typename Input2ImageType::PixelType    Input2ImagePixelType; 

  typedef TOutputImage                           OutputImageType;
  typedef typename OutputImageType::Pointer      OutputImagePointer;
  typedef typename OutputImageType::RegionType   OutputImageRegionType;
  typedef typename OutputImageType::PixelType    OutputImagePixelType;

  /** Connect one of the operands for pixel-wise addition */
  void SetInput1( const TInputImage1 * image1);

  /** Connect one of the operands for pixel-wise addition */
  void SetInput2( const TInputImage2 * image2);

  /** Get the functor object.  The functor is returned by reference.
   * (Functors do not have to derive from itk::LightObject, so they do
   * not necessarily have a reference count. So we cannot return a
   * SmartPointer.) */
  FunctorType& GetFunctor() { return m_Functor; }

  /** Get the functor object.  The functor is returned by reference.
   * (Functors do not have to derive from itk::LightObject, so they do
   * not necessarily have a reference count. So we cannot return a
   * SmartPointer.) */
  const FunctorType& GetFunctor() const
    {
    return m_Functor;
    }

  /** Set the functor object.  This replaces the current Functor with a
   * copy of the specified Functor. This allows the user to specify a
   * functor that has ivars set differently than the default functor.
   * This method requires an operator!=() be defined on the functor
   * (or the compiler's default implementation of operator!=() being
   * appropriate). */
  void SetFunctor(const FunctorType& functor)
    {
    if (m_Functor != functor)
      {
      m_Functor = functor;
      this->Modified();
      }
    }

  virtual void GenerateOutputInformation();
  virtual void GenerateInputRequestedRegion();

  /** ImageDimension constants */
  itkStaticConstMacro(
    InputImage1Dimension, unsigned int, TInputImage1::ImageDimension);
  itkStaticConstMacro(
    InputImage2Dimension, unsigned int, TInputImage2::ImageDimension);
  itkStaticConstMacro(
    OutputImageDimension, unsigned int, TOutputImage::ImageDimension);

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(SameDimensionCheck1,
    (Concept::SameDimension<itkGetStaticConstMacro(InputImage1Dimension),
                            itkGetStaticConstMacro(InputImage2Dimension)>));
  itkConceptMacro(SameDimensionCheck2,
    (Concept::SameDimension<itkGetStaticConstMacro(InputImage1Dimension),
                            itkGetStaticConstMacro(OutputImageDimension)>));
  /** End concept checking */
#endif

protected:
  FlexibleBinaryFunctorImageFilter();
  virtual ~FlexibleBinaryFunctorImageFilter() {}

  /** FlexibleBinaryFunctorImageFilter can be implemented as a multithreaded filter.
   * Therefore, this implementation provides a ThreadedGenerateData() routine
   * which is called for each processing thread. The output image data is
   * allocated automatically by the superclass prior to calling
   * ThreadedGenerateData().  ThreadedGenerateData can only write to the
   * portion of the output image specified by the parameter
   * "outputRegionForThread"
   *
   * \sa ImageToImageFilter::ThreadedGenerateData(),
   *     ImageToImageFilter::GenerateData()  */
  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                            itk::ThreadIdType threadId );

private:
  FlexibleBinaryFunctorImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  FunctorType m_Functor;
  Input2ImagePointer m_Input2;
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkFlexibleBinaryFunctorImageFilter.txx"
#endif

#endif
