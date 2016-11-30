#include <gtest/gtest.h>

#include "core/flags.h"

#include <cstdint>

using namespace I3D;

//enums para pruebas

// unscoped enumeration type
enum class ePrueba1 {

};

enum class ePrueba2 : int8_t {
  flag01 = (1 << 0),
  flag02 = (1 << 1),
  flag03 = (1 << 2),
  flag04 = (1 << 3),
  flag05 = (1 << 4),
  flag06 = (1 << 5),
  flag07 = (1 << 6)
};

ALLOW_BITWISE_FLAG_OPERATIONS(ePrueba2)

enum class ePrueba3 : int16_t {
  flag01 = (1 << 0),
  flag02 = (1 << 1),
  flag03 = (1 << 2),
  flag04 = (1 << 3),
  flag05 = (1 << 4),
  flag06 = (1 << 5),
  flag07 = (1 << 6),
  flag08 = (1 << 7),
  flag09 = (1 << 8),
  flag10 = (1 << 9),
  flag11 = (1 << 10),
  flag12 = (1 << 11),
  flag13 = (1 << 12),
  flag14 = (1 << 13),
  flag15 = (1 << 14)
};

ALLOW_BITWISE_FLAG_OPERATIONS(ePrueba3)

//enum class ePrueba : int32_t {
//
//};
//
//enum class ePrueba : int64_t {
//
//};
//
//enum class ePrueba : uint8_t {
//
//};
//
//enum class ePrueba : uint16_t {
//
//};
//
//enum class ePrueba : uint32_t {
//
//};
//
//enum class ePrueba : uint64_t {
//
//};

// Constructor EnumFlags
TEST(EnumFlags, Constructor) {

  EnumFlags<ePrueba2> flagEmpty;
  EXPECT_EQ(0,static_cast<EnumFlags<ePrueba2>::Type>(flagEmpty.getFlags()));

  EnumFlags<ePrueba2> flag(ePrueba2::flag01);

  EXPECT_TRUE(flag.isActive(ePrueba2::flag01));
  EXPECT_FALSE(flag.isActive(ePrueba2::flag04));
}

TEST(EnumFlags, Assign) {

  EnumFlags<ePrueba2> flagEmpty;
  flagEmpty = ePrueba2::flag01;

  EXPECT_TRUE(flagEmpty.isActive(ePrueba2::flag01));

  flagEmpty = ePrueba2::flag02 | ePrueba2::flag03;

  EXPECT_FALSE(flagEmpty.isActive(ePrueba2::flag01));
  EXPECT_TRUE(flagEmpty.isActive(ePrueba2::flag02));
  EXPECT_TRUE(flagEmpty.isActive(ePrueba2::flag03));

  flagEmpty = ePrueba2::flag02 | ePrueba2::flag03 | ePrueba2::flag01;
  EXPECT_TRUE(flagEmpty.isActive(ePrueba2::flag01));
  EXPECT_TRUE(flagEmpty.isActive(ePrueba2::flag02));
  EXPECT_TRUE(flagEmpty.isActive(ePrueba2::flag03));
}

TEST(clear, Test) {

  EnumFlags<ePrueba2> flag(ePrueba2::flag02 | ePrueba2::flag03);
  EXPECT_TRUE(flag.isActive(ePrueba2::flag02));
  EXPECT_TRUE(flag.isActive(ePrueba2::flag03));
  flag.clear();
  EXPECT_FALSE(flag.isActive(ePrueba2::flag02));
  EXPECT_FALSE(flag.isActive(ePrueba2::flag03));

}

TEST(flagOnOff, Test) {

  EnumFlags<ePrueba3> flag2(ePrueba3::flag15);

  EXPECT_TRUE(flag2.isActive(ePrueba3::flag15));
  EXPECT_FALSE(flag2.isActive(ePrueba3::flag03));

  flag2.flagOff(ePrueba3::flag15);
  flag2.flagOn(ePrueba3::flag03);

  EXPECT_FALSE(flag2.isActive(ePrueba3::flag15));
  EXPECT_TRUE(flag2.isActive(ePrueba3::flag03));

  //Prueba a activar un flag ya activo
  ePrueba3 ef = flag2.getFlags();
  flag2.flagOn(ePrueba3::flag03);
  ePrueba3 ef2 = flag2.getFlags();
  EXPECT_TRUE(flag2.isActive(ePrueba3::flag03));
}

TEST(switchFlag, Test) {

  EnumFlags<ePrueba3> flag2(ePrueba3::flag15 | ePrueba3::flag06 );
  EXPECT_TRUE(flag2.isActive(ePrueba3::flag15));
  EXPECT_TRUE(flag2.isActive(ePrueba3::flag06));
  EXPECT_FALSE(flag2.isActive(ePrueba3::flag01));
  EXPECT_FALSE(flag2.isActive(ePrueba3::flag03));

  flag2.switchFlag(ePrueba3::flag15);
  flag2.switchFlag(ePrueba3::flag06);
  flag2.switchFlag(ePrueba3::flag01);
  flag2.switchFlag(ePrueba3::flag03);

  EXPECT_FALSE(flag2.isActive(ePrueba3::flag15));
  EXPECT_FALSE(flag2.isActive(ePrueba3::flag06));
  EXPECT_TRUE(flag2.isActive(ePrueba3::flag01));
  EXPECT_TRUE(flag2.isActive(ePrueba3::flag03));

}

GTEST_API_ int main(int argc, char **argv) {
  printf("Running main() from gtest_main.cc\n");
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
