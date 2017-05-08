#include <stdio.h>

// Cabeceras tidopLib
#include "core/console.h"
#include "core/messages.h"
#include "img/imgio.h"

#include "experimental/experimental.h"
#include "EDSDK.h"

#include "opencv2/core/core.hpp"

using namespace I3D;

#ifdef VISUAL_LEAK_DETECTOR
#include <vld.h>
#endif

/*!
 * read_image: 
 *
 *
 * uso:
 */
int main(int argc, char** argv)
{

  char name[I3D_MAX_FNAME];
  getFileName(getRunfile(), name, I3D_MAX_FNAME);

  CmdParser cmdParser(name, "");
  cmdParser.addParameter("img", "Imagen");
  cmdParser.addParameter("img_out", "Imagen de salida");
  CmdParser::Status status = cmdParser.parse(argc, argv);
  if (status == CmdParser::Status::PARSE_ERROR ) {
    exit(EXIT_FAILURE);
  } else if (status == CmdParser::Status::PARSE_HELP) {
    exit(EXIT_SUCCESS);
  }

  std::string img = cmdParser.getValue<Path>("img").toString();
  std::string img_out = cmdParser.getValue<Path>("img_out").toString();


  // Fichero de log
  Log &log = Log::getInstance();
  //Configuración de log y mensajes por consola
  char logfile[I3D_MAX_PATH];
  if (changeFileExtension(getRunfile(), "log", logfile, I3D_MAX_PATH) == 0) {
    log.setLogFile(logfile);
  }
  log.setLogLevel(MessageLevel::MSG_INFO);

  // Consola
  Console console;
  console.setLogLevel(MessageLevel::MSG_INFO);
  console.setConsoleUnicode();

  //Configuración de mensajes
  MessageManager &msg_h = MessageManager::getInstance();
  msg_h.addListener(&log);
  msg_h.addListener(&console);


  EdsError err;
  if ( (err = EdsInitializeSDK()) != EDS_ERR_OK) exit(EXIT_FAILURE);

  EdsStreamRef input_stream;
  err = EdsCreateFileStream("C:\\Desarrollo\\datos\\Omega\\2017_03_02\\pruebas\\IMG_0001.CR2", kEdsFileCreateDisposition_OpenExisting, kEdsAccess_Read, &input_stream);
  if (err == EDS_ERR_OK) {
    //Abrir imagen
    EdsImageRef input_image;
    err = EdsCreateImageRef(input_stream, &input_image);
    if (err == EDS_ERR_OK) {
      //Propiedades
      EdsDataType type;
			EdsUInt32 _size;
			err = EdsGetPropertySize( input_image, kEdsPropID_FocusInfo , 0 , &type, &_size);
      
      EdsImageInfo outImageInfo;
      err = EdsGetImageInfo(input_image, kEdsImageSrc_FullView, &outImageInfo);

      EdsUInt64 inReadSize;
      err = EdsGetLength(input_stream, &inReadSize);
      
      //EdsUInt64 *outPosition;
      //EdsGetPosition(input_stream, outPosition);
      //cv::Mat img;
      //img.create(outImageInfo.height, outImageInfo.width, CV_MAKETYPE(CV_16U, 3));

      //uchar *buff = img.ptr();
      //size_t _size = 8688 * 65536 * 3;
      //EdsVoid *outBuffer = (EdsVoid*)std::malloc(8688 * 65536 * 3); //TODO: Reservar memoria
      //EdsUInt64 outReadSize;
      //err = EdsRead(input_stream, _size, outBuffer, &outReadSize);
      //
      //
      //cv::Mat buff = cv::Mat(1, outImageInfo.width, CV_MAKETYPE(CV_16U, 3), (void *)outBuffer);
      //cv::Mat output;
      //buff.convertTo(output, CV_8UC3);
      // The picture area to request is acquired from an editing box.

	    EdsRect rect;
	    rect.point.x	= 0;
	    rect.point.y	= 0;
	    rect.size.width	= outImageInfo.width;
	    rect.size.height= outImageInfo.height;
      
      /// Demand size is adjusted so that an aspect ratio may not change.
      EdsSize size;
	    size.width = outImageInfo.width;
	    size.height = outImageInfo.height;
      
      /// Creation of an output stream.
      EdsStreamRef DstStreamRef;
	    err = EdsCreateMemoryStream( 0 , &DstStreamRef );
      
      err = EdsGetImage( input_image, kEdsImageSrc_RAWFullView , kEdsTargetImageType_RGB , rect , size , DstStreamRef );
      if (err == EDS_ERR_OK) {
        // The start address of the acquired picture is acquired.
		    EdsVoid* pBuff;
		    EdsGetPointer( DstStreamRef , &pBuff );
		    /// A picture is updated to a drawing instance.
		    //m_pDrawImage->SetImage( size.width , size.height , pBuff );
        cv::Mat buff = cv::Mat(size.height, size.width, CV_MAKETYPE(CV_8U, 3), pBuff);
      
      
      }    
    
    }
    
    
    
    // Guardar imagen
    EdsStreamRef output_stream;
    err = EdsCreateFileStream("C:\\Desarrollo\\datos\\Omega\\2017_03_02\\pruebas\\IMG_0003.tif", kEdsFileCreateDisposition_CreateAlways, kEdsAccess_Write, &output_stream);
    if (err == EDS_ERR_OK) {

      


      /// A progress callback function is set as the output stream.
			//TODO: interesante enlazar con función de progreso
      //err = EdsSetProgressCallback( output_stream, ProgressFunc , kEdsProgressOption_Periodically, this);

      EdsSaveImageSetting settings;
	    settings.iccProfileStream = 0;
	    settings.reserved = 0;
      settings.JPEGQuality = 10;

      //EdsStreamRef ICCRef=NULL;
      //if( !icc.IsEmpty() )
		    //{
			   // /// ICC is set when adding ICC.
			   // err = EdsCreateFileStream( icc , kEdsFileCreateDisposition_OpenExisting , kEdsAccess_Read , &ICCRef );
			   // if( err == EDS_ERR_OK )
			   // {
				  //  settings.iccProfileStream = ICCRef;
			   // }
		    //}


      err = EdsSaveImage(input_image, kEdsTargetImageType_TIFF/*kEdsTargetImageType_Jpeg*/, settings, output_stream);
      if (err != EDS_ERR_OK) exit(EXIT_FAILURE);

      /// Progress is reset.
	    //CProgressCtrl *pProgress=(CProgressCtrl*)GetDlgItem(IDC_PROGRESS1);
	    //pProgress->SetPos(0);
    }

    // Liberar recursos
    EdsRelease(input_image);
    EdsRelease(output_stream);
  }

  EdsRelease(input_stream);

  err = EdsTerminateSDK();




  RasterGraphics image;
  Helmert2D<PointI> trf;
  cv::Mat mat_out;
  double scale = 4.;   // 25% de escala de la resolución original 

  if (image.open(img.c_str()) == Status::OPEN_FAIL) exit(EXIT_FAILURE);

  // saveAs() tiene que crear una copia con la libreria adecuada si tienen modo de copia
  image.saveAs(img_out.c_str());

  //try {    
  //  WindowI w(PointI(-100, -100), PointI(3900, 3900)); // Ventana de 4000x4000                          
  //  image.read(&mat_out, w, scale, &trf);

  //} catch (I3D::Exception &e) {
  //  printError(e.what());
  //  exit(EXIT_FAILURE);
  //}

  //// Imagen que se guarda
  //RasterGraphics imageOut;
  //imageOut.open(img_out.c_str(), Mode::Create);
  //imageOut.create(1000, 1000, 3, 0);
  //imageOut.write(mat_out, &trf);


  //GeoRasterGraphics geoRaster;
  //geoRaster.open("D://Desarrollo//datos//mtn25_epsg25830_0033-3.tif", Mode::Read);
  //WindowD w_g(PointD(377386.335, 4801010.256), PointD(380324.165, 4803176.748));
  //geoRaster.read(&mat_out, w_g, scale);

  exit(EXIT_SUCCESS);
}
