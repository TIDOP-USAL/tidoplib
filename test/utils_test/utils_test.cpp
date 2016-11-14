#include <gtest/gtest.h>
#include "..\..\src\core\utils.h"

using namespace I3D;

//
TEST(isDirectoryTest, Expect){
  EXPECT_TRUE(isDirectory("D:/Desarrollo"));
  EXPECT_TRUE(isDirectory("D:/Desarrollo/tidop/test"));
  EXPECT_TRUE(isDirectory("D:\\Desarrollo"));
  EXPECT_TRUE(isDirectory("D:\\Desarrollo\\tidop\\test"));
  EXPECT_FALSE(isDirectory("D:/Desarrollo_"));
  EXPECT_FALSE(isDirectory("D:/Desarrollo/no_existe_dir"));
  EXPECT_FALSE(isDirectory("D:\\Desarrollo_"));
  EXPECT_FALSE(isDirectory("D:\\Desarrollo\\no_existe_dir"));
}

TEST(isDirectoryTest, Null){
  EXPECT_FALSE(isDirectory(""));
}

TEST(createDir, ExistDir){
  char name[I3D_MAX_PATH];
  EXPECT_EQ(0, getFileDriveDir(getRunfile(), name));
  //Se prueba a crear un directorio ya existente
  EXPECT_EQ(1,createDir(name));
}

TEST(createDir, NewDir){
  //Borro por si acaso existia
  deleteDir("D:\\Desarrollo\\tidop\\test\\create_dir");
  EXPECT_EQ(0,createDir("D:\\Desarrollo\\tidop\\test\\create_dir\\1"));
  EXPECT_EQ(0,createDir("D:/Desarrollo/tidop/test/create_dir/1/2"));
}

TEST(DeleteDir, ExistDir){
  EXPECT_EQ(0,deleteDir("D:\\Desarrollo\\tidop\\test\\create_dir\\1\\2"));
  EXPECT_EQ(0,deleteDir("D:/Desarrollo/tidop/test/create_dir"));
}

TEST(DeleteDir, NoDir){
  EXPECT_EQ(1,deleteDir("D:\\Desarrollo\\tidop\\test\\create_dir"));
}

/* ---------------------------------------------------------------------------------- */
/*                        Test Operaciones con cadenas                                */
/* ---------------------------------------------------------------------------------- */

TEST(splitToNumbers, NullString){
  std::vector<int> vOut;
  EXPECT_EQ(0, splitToNumbers("", vOut));
  EXPECT_EQ(0, vOut.size());
  std::vector<double> vOutD;
  EXPECT_EQ(0, splitToNumbers("", vOutD));
  EXPECT_EQ(0, vOutD.size());
}


TEST(splitToNumbers, DefaultSeparatorCharacter){
  std::vector<int> vOut;
  EXPECT_EQ(0, splitToNumbers("1,2,3,4", vOut));
  EXPECT_EQ(4, vOut.size());
  EXPECT_EQ(1, vOut[0]);
  EXPECT_EQ(2, vOut[1]);
  EXPECT_EQ(3, vOut[2]);
  EXPECT_EQ(4, vOut[3]);
  vOut.resize(0);
  EXPECT_EQ(0, splitToNumbers("0,5,6", vOut));
  EXPECT_EQ(3, vOut.size());
  EXPECT_EQ(0, vOut[0]);
  EXPECT_EQ(5, vOut[1]);
  EXPECT_EQ(6, vOut[2]);
  std::vector<double> vOutD;
  EXPECT_EQ(0, splitToNumbers("1.0,3.5,5.2", vOutD));
  EXPECT_EQ(3, vOutD.size());
  EXPECT_EQ(1.0, vOutD[0]);
  EXPECT_EQ(3.5, vOutD[1]);
  EXPECT_EQ(5.2, vOutD[2]);
  vOutD.resize(0);
  EXPECT_EQ(0, splitToNumbers("234.6,235253.6", vOutD));
  EXPECT_EQ(2, vOutD.size());
  EXPECT_EQ(234.6, vOutD[0]);
  EXPECT_EQ(235253.6, vOutD[1]);
}

TEST(splitToNumbers, CustomSeparatorCharacter){
  std::vector<int> vOut;
  EXPECT_EQ(0, splitToNumbers("1#2#3#4", vOut,"#"));
  EXPECT_EQ(4, vOut.size());
  EXPECT_EQ(1, vOut[0]);
  EXPECT_EQ(2, vOut[1]);
  EXPECT_EQ(3, vOut[2]);
  EXPECT_EQ(4, vOut[3]);
  vOut.resize(0);
  EXPECT_EQ(0, splitToNumbers("0-5-6", vOut,"-"));
  EXPECT_EQ(3, vOut.size());
  EXPECT_EQ(0, vOut[0]);
  EXPECT_EQ(5, vOut[1]);
  EXPECT_EQ(6, vOut[2]);
  std::vector<double> vOutD;
  EXPECT_EQ(0, splitToNumbers("1.0 3.5 5.2", vOutD, " "));
  EXPECT_EQ(3, vOutD.size());
  EXPECT_EQ(1.0, vOutD[0]);
  EXPECT_EQ(3.5, vOutD[1]);
  EXPECT_EQ(5.2, vOutD[2]);
  vOutD.resize(0);
  EXPECT_EQ(0, splitToNumbers("234.6@235253.6", vOutD, "@"));
  EXPECT_EQ(2, vOutD.size());
  EXPECT_EQ(234.6, vOutD[0]);
  EXPECT_EQ(235253.6, vOutD[1]);

  vOut.resize(0);
  EXPECT_EQ(1, splitToNumbers("345@45", vOut, ","));
  EXPECT_EQ(0, vOut.size());
  vOutD.resize(0);
  EXPECT_EQ(1, splitToNumbers("234.6@235253.6", vOutD, ","));
  EXPECT_EQ(0, vOutD.size());
}

TEST(replaceString, test1){
  
  std::string cadena = "Prueba cadena";
  replaceString(&cadena, " ", "_");
  EXPECT_STREQ("Prueba_cadena", cadena.c_str());
  replaceString(&cadena, " ", " ");
  ASSERT_STRNE("Prueba cadena", cadena.c_str());
}

TEST(getFileDir, ValidDir){

  const char *file = "D:\\path\\file.ext";
  char file_dir[I3D_MAX_PATH];
  EXPECT_EQ(0,getFileDir(file, file_dir));
  EXPECT_STREQ("\\path\\", file_dir);
  const char *file2 = "D:/path/file.ext";
  EXPECT_EQ(0, getFileDir(file2, file_dir));
  EXPECT_STREQ("/path/", file_dir);
}

TEST(getFileDir, InvalidDir){

  const char *file = "";
  char file_dir[I3D_MAX_PATH];
  EXPECT_EQ(0, getFileDir(file, file_dir));
  EXPECT_STREQ("", file_dir);
  const char *file2 = "file.ext";
  EXPECT_EQ(0, getFileDir(file2, file_dir));
  EXPECT_STREQ("", file_dir);
}

TEST(getFileDrive, ValidDir){

  const char *path = "D:\\path\\file.ext";
  char drive[I3D_MAX_PATH];
  EXPECT_EQ(0, getFileDrive(path, drive));
  EXPECT_STREQ("D:", drive);
  const char *file2 = "D:/path/file.ext";
  EXPECT_EQ(0, getFileDrive(file2, drive));
  EXPECT_STREQ("D:", drive);
}

TEST(getFileExtension, ValidExtension){

  const char *path = "D:\\path\\file.ext";
  char ext[I3D_MAX_PATH];
  EXPECT_EQ(0, getFileExtension(path, ext));
  EXPECT_STREQ(".ext", ext);
  const char *file2 = "D:/path/file.ext";
  EXPECT_EQ(0, getFileExtension(file2, ext));
  EXPECT_STREQ(".ext", ext);
}

TEST(getFileExtension, InvalidExtension){

  const char *path = "D:\\path\\file";
  char ext[I3D_MAX_PATH];
  EXPECT_EQ(0, getFileExtension(path, ext));
  EXPECT_STREQ("", ext);
  const char *file2 = "D:/path/file";
  EXPECT_EQ(0, getFileExtension(file2, ext));
  EXPECT_STREQ("", ext);
}

TEST(getFileName, ValidName){

  const char *path = "D:\\path\\file.ext";
  char name[I3D_MAX_PATH];
  EXPECT_EQ(0, getFileName(path, name));
  EXPECT_STREQ("file", name);
  const char *path2 = "D:/path/file.ext";
  EXPECT_EQ(0, getFileName(path2, name));
  EXPECT_STREQ("file", name);
}

TEST(getFileDriveDir, ValidPath){

  const char *path = "D:\\path\\file.ext";
  char name[I3D_MAX_PATH];
  EXPECT_EQ(0, getFileDriveDir(path, name));
  EXPECT_STREQ("D:\\path\\", name);
  const char *path2 = "D:/path/file.ext";
  EXPECT_EQ(0, getFileDriveDir(path2, name));
  EXPECT_STREQ("D:/path/", name);
}

TEST(changeFileName, ValidPath){

  const char *path = "D:\\path\\file.ext";
  char new_path[I3D_MAX_PATH];
  EXPECT_EQ(0, changeFileName(path, "new_file_name", new_path));
  EXPECT_STREQ("D:\\path\\new_file_name.ext", new_path);
  const char *path2 = "D:/path/file.ext";
  EXPECT_EQ(0, changeFileName(path2, "new_file_name", new_path));
  EXPECT_STREQ("D:/path/new_file_name.ext", new_path);
}

TEST(changeFileExtension, ValidPath){

  const char *path = "D:\\path\\file.ext";
  char new_path[I3D_MAX_PATH];
  EXPECT_EQ(0, changeFileExtension(path, "aaa", new_path));
  EXPECT_STREQ("D:\\path\\file.aaa", new_path);
  const char *path2 = "D:/path/file.ext";
  EXPECT_EQ(0, changeFileExtension(path2, "bbb", new_path));
  EXPECT_STREQ("D:/path/file.bbb", new_path);
}

TEST(changeFileNameAndExtension, ValidPath){
  const char *path = "D:\\path\\file.ext";
  char new_path[I3D_MAX_PATH];
  EXPECT_EQ(0, changeFileNameAndExtension(path, "new_name.aaa", new_path));
  EXPECT_STREQ("D:\\path\\new_name.aaa", new_path);
  const char *path2 = "D:/path/file.ext";
  EXPECT_EQ(0, changeFileNameAndExtension(path2, "new_name.bbb", new_path));
  EXPECT_STREQ("D:/path/new_name.bbb", new_path);
}

TEST(split, NullString){
  std::vector<std::string> out;
  EXPECT_EQ(0, split("", out));
  EXPECT_EQ(0, out.size());
}


TEST(split, DefaultSeparatorCharacter){
  std::vector<std::string> out;
  EXPECT_EQ(0, split("cad1,cad2,cad3", out));
  EXPECT_EQ(3, out.size());
  EXPECT_EQ("cad1", out[0]);
  EXPECT_EQ("cad2", out[1]);
  EXPECT_EQ("cad3", out[2]);
}


TEST(split, CustomSeparatorCharacter){
  std::vector<std::string> out;
  EXPECT_EQ(0, split("cad1#cad2#cad3", out,"#"));
  EXPECT_EQ(3, out.size());
  EXPECT_EQ("cad1", out[0]);
  EXPECT_EQ("cad2", out[1]);
  EXPECT_EQ("cad3", out[2]);
}

/* ---------------------------------------------------------------------------------- */
/*                                    Vectores                                        */
/* ---------------------------------------------------------------------------------- */

TEST(vectorAngle, Test1){
  EXPECT_NEAR(0, vectorAngle(cv::Point(0,0),cv::Point(0,0)),0.001);
  EXPECT_NEAR(0.785, vectorAngle(cv::Point(0,2),cv::Point(1,1)),0.001);
  EXPECT_NEAR(0.516, vectorAngle(cv::Point(23,567),cv::Point(345,555)),0.001);
  EXPECT_NEAR(0.442, vectorAngle(cv::Point(123,234),cv::Point(456,343)),0.001);
  EXPECT_NEAR(1.471, vectorAngle(cv::Point2d(23.423,234234.5345),cv::Point2d(23423.345,2345.345)),0.001);
  EXPECT_NEAR(0.100, vectorAngle(cv::Point2d(2234.456,245234534.345),cv::Point2d(2452345.565,24357654.55)),0.001);
}


TEST(azimut,Test1){

  EXPECT_NEAR(0.7853, azimut(cv::Point(0,0),cv::Point(100,100)),0.0001);
}

/* ---------------------------------------------------------------------------------- */
/*                          Test conversión de ángulos                                */
/* ---------------------------------------------------------------------------------- */

//Datos para los test
std::vector<double> decimalDegrees{ 0., 0.5, 135.5742, 86.9997, -269.385 };
std::vector<std::vector<int>> dms{ { 0, 0, 0 }, { 0, 30, 0 }, { 135, 34, 27 }, { 86, 59, 59 }, { -269, 23, 6 } };
std::vector<double> radians{ 0.,0.00872664626,2.3662161708,1.518431213, -4.7016552055 };
std::vector<double> gradians{ 0., 0.555556, 150.63800, 96.666335, -299.3166718 };

TEST(degreesToDecimalDegrees, Test1) {
  for (int i = 0; i < dms.size(); i++) {
    EXPECT_NEAR(decimalDegrees[i], degreesToDecimalDegrees(dms[i][0], dms[i][1], dms[i][2]),0.001);
  }
}

TEST(degreesToRadians, Test1) {
  for (int i = 0; i < dms.size(); i++) {
    EXPECT_NEAR(radians[i], degreesToRadians(dms[i][0], dms[i][1], dms[i][2]), 0.000001);
  }
}

TEST(degreesToGradians, Test1) {
  for (int i = 0; i < dms.size(); i++) {
    EXPECT_NEAR(gradians[i], degreesToGradians(dms[i][0], dms[i][1], dms[i][2]), 0.0001);
  }
}

  
TEST(decimalDegreesToDegrees, Test1) {
  int degrees, minutes, seconds;
  for (int i = 0; i < decimalDegrees.size(); i++) {
    decimalDegreesToDegrees(decimalDegrees[i], &degrees, &minutes, &seconds);
    EXPECT_EQ(dms[i][0], degrees);
    EXPECT_EQ(dms[i][1], minutes);
    EXPECT_EQ(dms[i][2], seconds);
  }
}

TEST(decimalDegreesToRadians, Test1) {
  for (int i = 0; i < decimalDegrees.size(); i++) {
    EXPECT_NEAR(radians[i], decimalDegreesToRadians(decimalDegrees[i]), 0.000001);
  }
}

TEST(decimalDegreesToGradians, Test1) {
  for (int i = 0; i < decimalDegrees.size(); i++) {
    EXPECT_NEAR(gradians[i], decimalDegreesToGradians(decimalDegrees[i]), 0.0001);
  }
}

TEST(radiansToDegrees, Test1) {
  int degrees, minutes, seconds;
  for (int i = 0; i < radians.size(); i++) {
    radiansToDegrees(radians[i], &degrees, &minutes, &seconds);
    EXPECT_EQ(dms[i][0], degrees);
    EXPECT_EQ(dms[i][1], minutes);
    EXPECT_EQ(dms[i][2], seconds);
  }
}

TEST(radiansToDecimalDegrees, Test1) {
  for (int i = 0; i < radians.size(); i++) {
    EXPECT_NEAR(decimalDegrees[i], radiansToDecimalDegrees(radians[i]), 0.0001);
  }
}

TEST(radiansToGradians, Test1) {
  for (int i = 0; i < radians.size(); i++) {
    EXPECT_NEAR(gradians[i], radiansToGradians(radians[i]), 0.0001);
  }
}


TEST(gradiansToDegrees, Test1) {
  int degrees, minutes, seconds;
  for (int i = 0; i < gradians.size(); i++) {
    gradiansToDegrees(gradians[i], &degrees, &minutes, &seconds);
    EXPECT_EQ(dms[i][0], degrees);
    EXPECT_EQ(dms[i][1], minutes);
    EXPECT_EQ(dms[i][2], seconds);
  }
}

TEST(gradiansToDecimalDegrees, Test1) {
  for (int i = 0; i < gradians.size(); i++) {
    EXPECT_NEAR(decimalDegrees[i], gradiansToDecimalDegrees(gradians[i]), 0.0001);
  }
}

TEST(gradiansToRadians, Test1) {
  for (int i = 0; i < gradians.size(); i++) {
    EXPECT_NEAR(radians[i], gradiansToRadians(gradians[i]), 0.00001);
  }
}


GTEST_API_ int main(int argc, char **argv) {
  printf("Running main() from gtest_main.cc\n");
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
